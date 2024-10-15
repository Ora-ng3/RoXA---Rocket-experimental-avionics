#include <Arduino.h>
#include <SD.h>
#include <SPI.h>
#include <LittleFS.h>
#include "Data.h"
#include "Buzzer.h"

bool SD_enabled = true;

#ifdef PSRAM
	void initializePSRAM();
#endif
void initializeFlash();
void initializeSD();

void initializeData() {
	#ifdef PSRAM
    	initializePSRAM();
	#endif
	#ifdef FLASH
    initializeFlash();
	#endif
	#ifdef SD_LOG
    initializeSD();
	#endif
}

void initializeSD() {
	if (!SD.begin(BUILTIN_SDCARD)) {
		Serial.println("Error : SD card initialization failed");
		SD_enabled = false;
		/*
		while(true) {
			biiip(2, 500);
			delay(1000);
		}
		*/
	}
	Serial.println("Success : SD card initialized");

	File dataFile = SD.open("datalog.csv", FILE_WRITE);
	if (dataFile) {
		dataFile.println("==================== New fly ====================");
		dataFile.close();
	}
}

LittleFS_QSPIFlash myfs;

void initializeFlash() {
    if (!myfs.begin()) {
        Serial.printf("Error : FLASH initialization failed");
        while (true) {
			biiip(3, 500);
			delay(1000);
        }
    }
    Serial.println("Success : FLASH initialized");
}


#ifdef PSRAM
	#define PSRAM_BASE_ADDRESS 0x70000000
	#define PSRAM_SIZE (8 * 1024 * 1024)

void initializePSRAM() {
    volatile uint32_t* psram_address = (uint32_t*)PSRAM_BASE_ADDRESS;
    const uint32_t test_pattern = 0xA5A5A5A5; // Pattern for testing

    // Write the pattern to the PSRAM
    *psram_address = test_pattern;
    arm_dcache_flush_delete((void*)psram_address, sizeof(uint32_t));

    // Read the pattern back
    uint32_t read_back = *psram_address;

    // Check if the read value is the same as the test pattern
    if (read_back != test_pattern) {
        Serial.println("Error : PSRAM initialization failed");
        while(true) {
			biiip(4, 500);
			delay(1000);
		}
    }

    Serial.println("Success : PSRAM initialized");
}

#endif // PSRAM

void dataLog(unsigned long* time, float* altitude, float* baro_apogee, float* raw_acceleration_x, float* raw_acceleration_y, float* raw_acceleration_z, float* raw_angular_velocity_x, float* raw_angular_velocity_y, float* raw_angular_velocity_z, int* state) {
	if (!SD_enabled) {
		return;
	}
	
	// Log to PSRAM
	// Increment the number of logs
	// If nb > 10, write to Flash

	File dataFile = SD.open("datalog.txt", FILE_WRITE);
	if (dataFile) {
		dataFile.print(*time);
		dataFile.print(";");
		dataFile.print(*altitude);
		dataFile.print(";");
		dataFile.print(*baro_apogee);
		dataFile.print(";");
		dataFile.print(*raw_acceleration_x);
		dataFile.print(";");
		dataFile.print(*raw_acceleration_y);
		dataFile.print(";");
		dataFile.print(*raw_acceleration_z);
		dataFile.print(";");
		dataFile.print(*raw_angular_velocity_x);
		dataFile.print(";");
		dataFile.print(*raw_angular_velocity_y);
		dataFile.print(";");
		dataFile.print(*raw_angular_velocity_z);
		dataFile.print(";");
		dataFile.print(*state);
		dataFile.println();
		dataFile.close();
	}
}

// Call SD log when FC state is landed