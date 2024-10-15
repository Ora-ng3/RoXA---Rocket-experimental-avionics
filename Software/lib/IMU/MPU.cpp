#include <Arduino.h>
#include "MPU.h"

MPU6050 mpu;

// definition for extern
//Quaternion raw_q;
//float raw_ypr[3];
VectorFloat raw_angular_velocity;
VectorFloat gravity;
VectorFloat raw_acceleration;

const int a_range = 16;
const float G = 9.81;
const int g_range = 2000;

uint8_t fifoBuffer[64]; // FIFO storage buffer

/*
void initializeMPU() {
    Wire.begin();
    //Wire.setClock(400000); // 400kHz I2C clock
    Serial.println("Initializing MPU6050...");
    mpu.initialize();
    mpu.setFullScaleGyroRange(MPU6050_GYRO_FS_2000);
    mpu.setFullScaleAccelRange(MPU6050_ACCEL_FS_16);
    Serial.println(mpu.testConnection() ? "MPU6050 connection successful" : 
                                          "MPU6050 connection failed");

    //uint8_t devStatus = mpu.dmpInitialize();
    mpu.dmpInitialize();

    // supply your own gyro offsets here, scaled for min sensitivity
    mpu.setXGyroOffset(GyroXOffset);
    mpu.setYGyroOffset(GyroYOffset);
    mpu.setZGyroOffset(GyroZOffset);
    mpu.setXAccelOffset(AccelXOffset);
    mpu.setYAccelOffset(AccelYOffset);
    mpu.setZAccelOffset(AccelZOffset);
    
    #ifdef FINE_ACC_GYRO_CALIBRATION
    if (devStatus == 0) {
        mpu.CalibrateAccel(6);
        mpu.CalibrateGyro(6);
        mpu.PrintActiveOffsets();
        mpu.setDMPEnabled(true);
    }
    #endif
}
*/

void initializeMPU() {
    Wire.begin();
    mpu.initialize();
    mpu.setFullScaleGyroRange(MPU6050_GYRO_FS_2000);
    mpu.setFullScaleAccelRange(MPU6050_ACCEL_FS_16);
    Serial.println(mpu.testConnection() ? "MPU6050 connection successful" : 
                                          "MPU6050 connection failed");
    mpu.setXGyroOffset(GyroXOffset);
    mpu.setYGyroOffset(GyroYOffset);
    mpu.setZGyroOffset(GyroZOffset);
    mpu.setXAccelOffset(AccelXOffset);
    mpu.setYAccelOffset(AccelYOffset);
    mpu.setZAccelOffset(AccelZOffset);
}


/*
void readMPU(Quaternion* q, float* ypr) {
    VectorInt16 int_acceleration;
    VectorInt16 int_angular_velocity;

    mpu.dmpGetQuaternion(q, fifoBuffer);
    mpu.dmpGetGravity(&gravity, q);
    mpu.dmpGetYawPitchRoll(ypr, q, &gravity);

    // Scale angular velocity to deg/s
    mpu.dmpGetGyro(&int_angular_velocity, fifoBuffer);
    raw_angular_velocity.x = map(int_angular_velocity.x, -32768, 32767, -g_range, g_range);
    raw_angular_velocity.y = map(int_angular_velocity.y, -32768, 32767, -g_range, g_range);
    raw_angular_velocity.z = map(int_angular_velocity.z, -32768, 32767, -g_range, g_range);

    // Scale acceleration to m/s^2
    mpu.dmpGetAccel(&int_acceleration, fifoBuffer);
    raw_acceleration.x = map(int_acceleration.x, -32768, 32767, -a_range*G, a_range*G);
    raw_acceleration.y = map(int_acceleration.y, -32768, 32767, -a_range*G, a_range*G);
    raw_acceleration.z = map(int_acceleration.z, -32768, 32767, -a_range*G, a_range*G);
}
*/

void readMPU(Quaternion* q, float* ypr) {
    VectorInt16 int_acceleration;
    VectorInt16 int_angular_velocity;
    
    mpu.getMotion6(&int_acceleration.x, &int_acceleration.y, &int_acceleration.z, 
                   &int_angular_velocity.x, &int_angular_velocity.y, &int_angular_velocity.z);

    raw_acceleration.x = map(int_acceleration.x, -32768, 32767, -a_range*G, a_range*G);
    raw_acceleration.y = map(int_acceleration.y, -32768, 32767, -a_range*G, a_range*G);
    raw_acceleration.z = map(int_acceleration.z, -32768, 32767, -a_range*G, a_range*G);

    raw_angular_velocity.x = map(int_angular_velocity.x, -32768, 32767, -g_range, g_range);
    raw_angular_velocity.y = map(int_angular_velocity.y, -32768, 32767, -g_range, g_range);
    raw_angular_velocity.z = map(int_angular_velocity.z, -32768, 32767, -g_range, g_range);
}

/*
void readMPU(Quaternion* q, float* ypr) {
    Serial.println("Reading MPU6050...");
    
    uint16_t fifoCount = mpu.getFIFOCount();
    uint8_t mpuIntStatus = mpu.getIntStatus();
    
    Serial.print("FIFO count: ");
    Serial.println(fifoCount);
    Serial.print("MPU Int Status: ");
    Serial.println(mpuIntStatus, BIN);
    
    // Check for FIFO overflow
    if ((mpuIntStatus & 0x10) || fifoCount == 1024) {
        // FIFO overflow happened
        mpu.resetFIFO();
        Serial.println("FIFO overflow!");
    } else if (mpuIntStatus & 0x02) {
        // Check if FIFO count is sufficient
        while (mpu.getFIFOCount() < mpu.dmpGetFIFOPacketSize()) {
            // Wait until we have enough data
        }

        if (mpu.dmpGetCurrentFIFOPacket(fifoBuffer)) {
            Serial.println("FIFO packet read");
            VectorInt16 int_acceleration;
            VectorInt16 int_angular_velocity;

            mpu.dmpGetQuaternion(q, fifoBuffer);
            mpu.dmpGetGravity(&gravity, q);
            mpu.dmpGetYawPitchRoll(ypr, q, &gravity);

            // Scale angular velocity to deg/s
            mpu.dmpGetGyro(&int_angular_velocity, fifoBuffer);
            raw_angular_velocity.x = map(int_angular_velocity.x, -32768, 32767, -g_range, g_range);
            raw_angular_velocity.y = map(int_angular_velocity.y, -32768, 32767, -g_range, g_range);
            raw_angular_velocity.z = map(int_angular_velocity.z, -32768, 32767, -g_range, g_range);

            // Scale acceleration to m/s^2
            mpu.dmpGetAccel(&int_acceleration, fifoBuffer);
            raw_acceleration.x = map(int_acceleration.x, -32768, 32767, -a_range*G, a_range*G);
            raw_acceleration.y = map(int_acceleration.y, -32768, 32767, -a_range*G, a_range*G);
            raw_acceleration.z = map(int_acceleration.z, -32768, 32767, -a_range*G, a_range*G);
        }
    }
}

*/