#ifndef BAROMETER_H
#define BAROMETER_H

#include <Wire.h>
#include <Adafruit_BMP085.h>

extern float altitude;

void initializeBarometer();

void readBarometer();

void readTemperature(float* temperature);

void offsetBarometer();

#endif