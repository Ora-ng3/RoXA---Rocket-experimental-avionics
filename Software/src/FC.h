#ifndef FC_H
#define FC_H

#include "Led.h"
#include "Buzzer.h"
#include "Data.h"
#include "CanSat.h"
#include "Barometer.h"
#include "MPU.h"
#include "Reaction_wheel.h"
#include "Pyro.h"
#include "Rogallo.h"
#include "Config.h"

enum State {NOT_READY, IDLE, ARMED, ASCENDING, DROGUE, MAIN, LANDED};
// NOT_READY : before initialisation
// IDLE : initialized (low log frequency, battery log activated...)
// ARMED : ready to take off (stop unnecessary log, on pad so can offset sensors, gps...)
// ASCENDING : rw enabled, check for apogee
// DROGUE : check for main deplyment altitude, stop using accel and gyro, start sending gps
// MAIN : start Rogallo control, check barometer altitude to determine landing
// LANDED : wait 5s, send ok signal and gps position only, dump flash to sd

class FlightComputer {
	public:
		FlightComputer() : state(NOT_READY) {
			Serial.println("Instantiating Pyros");
			pyro1 = new Pyro(PYRO1, PYRO_CTY1),
			pyro2 = new Pyro(PYRO2, PYRO_CTY2),
			pyro3 = new Pyro(PYRO3, PYRO_CTY3),
			pyro4 = new Pyro(PYRO4, PYRO_CTY4);
			Serial.println("Pyros instantiated");
		}
		
		void initialize();
		void set_state(State state);

		void readSensors();
		void navigate();
		void control();

		void inFrameComms();

		void eventManager();

		void memoryManager();
	private:
		State state;

		// True state data
		VectorFloat position; // Computed by fusing barometer and MPU with a Kalman filter
		VectorFloat velocity; // Computed by fusing barometer and MPU with a Kalman filter
		VectorFloat acceleration; // 

		Quaternion q; // Computed by DMP
		float ypr[3]; // Computed from quaternion
		VectorFloat angular_velocity; // For rw and gimbal
		VectorFloat angular_integral; // For rw and gimbal
		//VectorFloat angular_acceleration;

		float temperature;

		float batteryLevel;

		int log_rate = 1; // Hz
		uint8_t readingIndex;

		float true_apogee = 0;
		float baro_apogee = 0;

		bool batt_temp_log_enabled = true;
		bool rw_enabled = false;
		bool rogallo_enabled = false;
		bool rogallo_fired_1 = false;
		bool rogallo_fired_2 = false;
		bool rogallo_fired_3 = false;

		unsigned long MAIN_deploy_time = 0;

		Pyro* pyro1;
		Pyro* pyro2;
		Pyro* pyro3;
		Pyro* pyro4;
};

void setupComplete();

void timeStepUpdate();

extern FlightComputer* FC;

#endif