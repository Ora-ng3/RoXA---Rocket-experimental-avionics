#include <Arduino.h>
#include "MPU.h"
#include "Reaction_wheel.h"
#include "Pins.h"

double Kp=9, Ki=0.5, Kd=0.5;
double input, speed, speed_prev, output_percent = 0;
int last_RW_time = 0;
double p_error, i_error = 0, d_error=0, prev_error=0;

int target = 0;
int target_time = 0;
int minvalue = 100, max_value = 255;
// The motor can turn from 60% to 100%
int range = 2000;

float dt = 50; //in ms
double angle = 0.0;
float adrift = -0.05;
int outputmax = 500; //Kp * 90 + Kd * 250 + Ki * 30;

void initializeReactionWheel() {
  pinMode(MOT_1, OUTPUT);
  pinMode(MOT_2, OUTPUT);
}
/*
int RW_compute() {
  if (millis() - last_RW_time > dt*1000) {
    mpu.getRotation(&vgx, &vgy, &vgz); 
    vgz = map(vgz, -32767, 32767, -range, range);
    
    // z angle calculated by integration
    angle += vgz*dt - adrift;
    //Serial.print("Angle: "); Serial.print(angle);
    //Serial.print(" i_error: "); Serial.print(i_error);

    p_error = target - angle;
    
    i_error += p_error*dt;

    if (abs(p_error) > 2) i_error += p_error*dt;
    if (abs(p_error) < 0.5) i_error = i_error*0.1;
    //if (abs(p_error) < 0.5) i_error = 0;
    i_error = constrain(i_error, -50, 50);

    d_error = (p_error - prev_error) / dt;
    prev_error = p_error;

    speed = Kp * p_error + Ki * i_error + Kd * d_error;
    //Serial.print(" PID: "); Serial.print(speed);
    speed = map(speed, -outputmax, outputmax, -max_value, max_value);
    
    if (abs(speed) < minvalue) speed = 0;

    // signal filtering by taking a small percentage of the previous output
    speed = (1-output_percent)*speed+ output_percent*speed_prev;
    speed_prev = speed;

    last_RW_time = millis();

    //Serial.print(" Speed: "); Serial.print(speed);
    
    //Serial.println();
  }
  return speed;
}

void RW_control(int speed) {
  if (speed > 0) {
    digitalWrite(MOT_1, LOW);
    analogWrite(MOT_2, speed);
  } else {
    digitalWrite(MOT_2, LOW);
    analogWrite(MOT_1, speed);
  }
}
*/

int RW_compute(float angle) {
	p_error = target - angle;
    
    i_error += p_error*dt;

    if (abs(p_error) > 2) i_error += p_error*dt;
    if (abs(p_error) < 0.5) i_error = i_error*0.1;
    //if (abs(p_error) < 0.5) i_error = 0;
    i_error = constrain(i_error, -50, 50);

    d_error = (p_error - prev_error) / dt;
    prev_error = p_error;

    speed = Kp * p_error + Ki * i_error + Kd * d_error;
    //Serial.print(" PID: "); Serial.print(speed);
    speed = map(speed, -outputmax, outputmax, -max_value, max_value);
    
    if (abs(speed) < minvalue) speed = 0;

    // signal filtering by taking a small percentage of the previous output
    speed = (1-output_percent)*speed+ output_percent*speed_prev;
    speed_prev = speed;

    last_RW_time = millis();

  	return speed;
}

void RW_control(int speed) {
	if (speed > 0) {
		digitalWrite(MOT_1, LOW);
		analogWrite(MOT_2, speed);
	} else if (speed < 0) {
		digitalWrite(MOT_2, LOW);
		analogWrite(MOT_1, speed);
	} else {
		digitalWrite(MOT_1, LOW);
		digitalWrite(MOT_2, LOW);
	}
}