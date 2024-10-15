#include <Arduino.h>
#include "CanSat.h"

PWMServo servoTop;
PWMServo servoBot;

// Servo top :
const int topOpenAngle = 92;
const int topCloseAngle = 56;

// Servo bottom :
const int botOpenAngle = 100;
const int botCloseAngle = 64;

// Firing :
unsigned long CanSat_StartTime;
bool CanSat_firing = false;

void initializeCanSat() {
    servoTop.attach(BRK_14);
    servoBot.attach(BRK_15);
    closeCanSat();
}

void closeCanSat() {
    servoTop.write(topCloseAngle);
    servoBot.write(botCloseAngle);
}

void openCanSat() {
    servoTop.write(topOpenAngle);
    servoBot.write(botOpenAngle);
}

void fireCanSat(float wait) {
    CanSat_StartTime = millis() + wait * 1000;
    CanSat_firing = true;
}

unsigned long getCanSatStartTime() {
    return CanSat_StartTime;
}

bool getCanSatFiring() {
    return CanSat_firing;
}

void setCansatFiring(bool firing) {
    CanSat_firing = firing;
}