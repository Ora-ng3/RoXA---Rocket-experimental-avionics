#ifndef CANSAT_H
#define CANSAT_H

#include "Pins.h"
#include <PWMServo.h>

void initializeCanSat();

void closeCanSat();

void openCanSat();

void fireCanSat(float wait);

unsigned long getCanSatStartTime();

bool getCanSatFiring();

void setCansatFiring(bool firing);

#endif