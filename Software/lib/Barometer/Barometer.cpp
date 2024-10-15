#include <Arduino.h>
#include "Barometer.h"

Adafruit_BMP085 barometer;

// defintion for extern
float altitude = 0;

float altitude_offset = 0;
//uint8_t temperature_index = 0;

void initializeBarometer() {
    Serial.println("Initializing Barometer...");
    if (!barometer.begin()) {
        Serial.println("Could not find a valid BMP085 sensor, check wiring!");
        while (1);
    }
    Serial.println("Barometer initialized, begginning altitude offset calculation...");
}

void readBarometer() {
    altitude = barometer.readAltitude() - altitude_offset;
}

void readTemperature(float* temperature) {
    *temperature = barometer.readTemperature();
}

void offsetBarometer() {
    altitude_offset = 0;
    for (int i = 0; i < 40; i++) {
        altitude_offset += barometer.readAltitude();
        delay(50);
    }
    altitude_offset /= 40;
    Serial.print("Altitude offset done : ");
    Serial.println(altitude_offset);
}