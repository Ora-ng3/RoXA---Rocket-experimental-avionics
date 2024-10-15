#include <Arduino.h>
#include "Pyro.h"
#include "CanSat.h"

std::vector<Pyro*> Pyro::pyros;

void Pyro::manager() {
    Serial.print("Number of pyros: ");
    Serial.println(pyros.size());
    for (Pyro* pyro : pyros) {
        Serial.print("Checking ");
        Serial.print(pyro->pyro_pin);
        Serial.print(";");
        pyro->checkContinuity();
        if (pyro->firing) {
            if (millis() > pyro->startTime) {
                digitalWrite(pyro->pyro_pin, HIGH);
                Serial.println("#######################");
                Serial.print(pyro->pyro_pin);
                Serial.println(" fired");
                Serial.println("#######################");
            }
            if (millis() > pyro->endTime) {
                digitalWrite(pyro->pyro_pin, LOW);
                pyro->firing = false;
            }
        }
    }
    
    if (getCanSatFiring()) {
        if (millis() > getCanSatStartTime()) {
            openCanSat();
            setCansatFiring(false);
        }
    }
}

void Pyro::checkContinuity() {
    continuity = digitalRead(cty_pin); // or analog read with threshold ?
}

void Pyro::fire(float wait, float duration) {
    firing = true;
    startTime = millis() + wait * 1000;
    endTime = startTime + duration * 1000;
    Serial.print("Firing pyro called ; ");
}