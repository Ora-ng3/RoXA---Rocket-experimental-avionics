#include <Arduino.h>
#include "FC.h"

FlightComputer* FC;

void readBatteryLevel(float* batteryLevel) {
	float Vref = map(analogRead(BATT), 0, 1023, 0, 3.3);
	// Vout = Vin * R2 / (R1 + R2)
	// Vbat = (R1 + R2) / R2 * Vref
	// R1 = 1M, R2 = 300k
	// 1.3M / 300k = 4.33
	*batteryLevel = 4.33 * Vref;
}

void setupComplete() {
	led(0, 1, 0);
	biiip(3, 500);
	Serial.println("Setup complete");
}

void FlightComputer::initialize() {
	Serial.println("Initializing Flight Computer...");
	
	initializeLed();
	led(1, 0, 0);
	delay(500);
	led(0, 1, 0);
	delay(500);
	led(0, 0, 1);
	delay(500);

	initializeBuzzer();
	biiip(1, 1000);

	//initializeComm();
	initializeData();
    initializeMPU();
	initializeBarometer();
	//initializeGPS();
	#ifdef CANSAT
	initializeCanSat();
	#endif
	#ifdef REACTION_WHEEL
    initializeReactionWheel();
	#endif
	//initializePyro(); // not needed cause initialized globally :(
}

void FlightComputer::readSensors() {
    readMPU(&q, ypr);
    readBarometer();
	if (readingIndex == 0) {
		if (batt_temp_log_enabled) {
			readBatteryLevel(&batteryLevel);
			readTemperature(&temperature);
		}
	}
	readingIndex = (readingIndex + 1) % (FPS/log_rate);
}

void FlightComputer::set_state(State state) {
	switch (state)
	{
	case IDLE :
		Serial.println("IDLE");
		log_rate = SLOW_LOG_RATE; // Hz
		break;

	case ARMED :
		Serial.println("ARMED");
		led(1, 0.2, 0.5);
		log_rate = FAST_LOG_RATE; // Hz (or 100 ?)
		batt_temp_log_enabled = false;
		baro_apogee = 0;
		ypr[1] = 0;
		offsetBarometer();
		// offset_gps
		// turn on cameras and led
		break;

	case ASCENDING :
		Serial.println("ASCENDING");
		led(0, 0, 1);
		rw_enabled = true;
		// reset integral rw ?
		break;

	case DROGUE :
		Serial.println("DROGUE");
		led(1, 0, 0);
		rw_enabled = false;
		RW_control(0);
		// motor speed to zero
		// Apogee events :
		pyro1->fire(0, 1); // instant, for 1s
		// openCanSat(1); // after 1s, to avoid to much power draw
		pyro2->fire(1.5, 2); // after 1.5s, for 2s (longer firing to ensure deployment)
		// accelGain = 0; (or something disabling accel and gyro in Kalman and using only barometer)
		break;

	case MAIN :
		Serial.println("MAIN");
		led(0,0,1);
		rogallo_enabled = true;
		MAIN_deploy_time = millis();
		pyro3->fire(0, 2); // instant, for 2s because line cutting can be long
		pyro4->fire(2, 2); // after 2s, for 2s
		break;

	case LANDED :
		Serial.println("LANDED");
		led(0, 1, 0);
		rogallo_enabled = false;
		log_rate = SLOW_LOG_RATE; // Hz
		batt_temp_log_enabled = true;
		break;
	
	default:
		break;
	}

	this->state = state;
}

void FlightComputer::navigate() {
	if (altitude > baro_apogee) {
		baro_apogee = altitude;
	}
	acceleration.x = raw_acceleration.x;
	acceleration.y = raw_acceleration.y;
	acceleration.z = raw_acceleration.z;

	ypr[1] = raw_angular_velocity.y*1000/FPS;
}

void FlightComputer::inFrameComms(){
	//setState...
}

void FlightComputer::eventManager(){
	switch (state)
	{
	case IDLE :
		if(millis() > 15000){
			set_state(ARMED);
		}
		break;

	case ARMED :
		/*
		{
		float abs_acceleration = sqrt(acceleration.x*acceleration.x + acceleration.y*acceleration.y + acceleration.z*acceleration.z);
		Serial.print("Abs acceleration : ");
		Serial.print(abs_acceleration);
		Serial.print(" ; ");
		if (altitude > BARO_ASCENDING_TRIGGER or abs_acceleration > 2) {
			set_state(ASCENDING);
		}
		}
		*/

		#ifndef TEST
		if (altitude > BARO_ASCENDING_TRIGGER) { set_state(ASCENDING); }
		#else
		if (millis() > 20000) { set_state(ASCENDING); }
		#endif

		break;

	case ASCENDING :
		// checking for apogee
		// if detected, changes state to drogue

		#ifndef TEST
		if (altitude < baro_apogee - BARO_DROP) { set_state(DROGUE); }
		#else
		if (millis() > 25000) { set_state(DROGUE); }
		#endif

		break;

	case DROGUE :
		// checking for altitude to deploy main
		
		#ifndef TEST
		if (altitude < BARO_MAIN_DEPLOY) { set_state(MAIN); }
		#else
		if (millis() > 30000) { set_state(MAIN); }
		#endif

		break;

	case MAIN :
		// controlling Rogallo using gps or "path" or 
		
		#ifndef TEST
		if (altitude < 5) { set_state(LANDED); }
		#else
		if (millis() > 60000) { set_state(LANDED); }
		#endif
		
		break;

	case LANDED :
		// 
		break;
	
	default :
		break;
	}
}

void FlightComputer::control() {
	if (rw_enabled){
		RW_control(RW_compute(ypr[1]));
	}

	if (rogallo_enabled) {
		if (millis() > MAIN_deploy_time + 5000 and not rogallo_fired_1) {
			rotate_clockwise(0.25);
			rogallo_fired_1 = true;
		}
		if (millis() > MAIN_deploy_time + 10000 and not rogallo_fired_2) {
			rotate_anticlockwise(0.5);
			rogallo_fired_2 = true;
		}
		if (millis() > MAIN_deploy_time + 15000 and not rogallo_fired_3) {
			rotate_clockwise(0.25);
			rogallo_fired_3 = true;
		}
	}
}

// NOT_READY : before initialisation
// ARMED : ready to take off (stop unnecessary log, on pad so can offset sensors, gps...)
// ASCENDING : rw enabled, check for apogee
// DROGUE : check for main deplyment altitude, start sending gps
// MAIN : start Rogallo control, check barometer altitude to determine landing
// LANDED : wait 5s, send ok signal and gps position only, dump flash to sd

void FlightComputer::memoryManager() {
	int int_state = this->state;
	// prints altitude and state in letters
	//Serial.print(raw_acceleration.x);
	//Serial.print(" ; ");
	Serial.print(altitude);
	Serial.print(" ; ");
	Serial.print(baro_apogee);
	Serial.print(" ; ");
	switch (state)
	{
	case NOT_READY :
		Serial.println("NR");
		break;
	case IDLE :
		Serial.println("ID");
		break;
	case ARMED :
		Serial.println("AR");
		break;
	case ASCENDING :
		Serial.println("AS");
		break;
	case DROGUE :
		Serial.println("DR");
		break;
	case MAIN :	
		Serial.println("MA");
		break;
	case LANDED :
		Serial.println("LA");
		break;
	default:
		break;
	}

	// writes data to SD card
	// millis();altitude;baro_apogee;raw_acceleration.x;raw_acceleration.y;raw_acceleration.z;raw_angular_velocity.x;raw_angular_velocity.y;raw_angular_velocity.z;state

	unsigned long current_time = millis();
	dataLog(&current_time, &altitude, &baro_apogee, &raw_acceleration.x, &raw_acceleration.y, &raw_acceleration.z, &raw_angular_velocity.x, &raw_angular_velocity.y, &raw_angular_velocity.z, &int_state); 

}

void timeStepUpdate() {
	static unsigned long lastTime = 0;
	unsigned long currentTime = millis();
	unsigned long elapsedTime = currentTime - lastTime;
	if (elapsedTime < 1000/FPS) {
		delay(1000/FPS - elapsedTime);
	}
	lastTime = currentTime;
}