#include <Arduino.h>
#include "FC.h"

void setup() {
    Serial.begin(9600);
	Serial.println("Starting setup");

	FC = new FlightComputer();
	FC->initialize();

	setupComplete(); // show that setup is complete
	FC->set_state(IDLE);
	// timeStepUpdate();
}

void loop() {
	FC->readSensors();
	//massManager();

	FC->navigate(); // where is the rocket ? (position, velocity, attitude)
	//guidance(); // where do we want to go ?
	FC->control(); // how do we get there ?

	//inFrameComms(); // sends and receives data so next events can be trigerred

	FC->eventManager(); // checking trigger functions and time constraints
	Pyro::manager(); // manages PYRO objects, checks for continuity and, if firing
					 // checks durations
	FC->memoryManager(); // sometimes writes data to flash memory (no PSRAM ?)
	timeStepUpdate(); // ensures 100Hz rate
}