#ifndef MPU_H
#define MPU_H

#include "MPU6050_6Axis_MotionApps20.h"
#include <Wire.h>
#include "I2Cdev.h"
#include "Config.h"

extern VectorFloat raw_angular_velocity;
extern VectorFloat raw_acceleration;

void initializeMPU();

void readMPU(Quaternion* q, float* ypr);

#endif