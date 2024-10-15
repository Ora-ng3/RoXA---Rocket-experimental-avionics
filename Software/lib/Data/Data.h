#ifndef DATA_H
#define DATA_H

#include <SD.h>
#include "Config.h"

void initializeData();

void dataLog(unsigned long* time, float* altitude, float* baro_apogee, float* raw_acceleration_x, float* raw_acceleration_y, float* raw_acceleration_z, float* raw_angular_velocity_x, float* raw_angular_velocity_y, float* raw_angular_velocity_z, int* state);

#endif