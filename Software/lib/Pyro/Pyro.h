#ifndef PYRO_H
#define PYRO_H

#include "Pins.h"

class Pyro {
    public:
        Pyro(int pyro_pin, int cty_pin) : pyro_pin(pyro_pin), cty_pin(cty_pin) {
            pinMode(pyro_pin, OUTPUT);
            pinMode(cty_pin, INPUT);
            digitalWrite(pyro_pin, LOW);
            continuity = false;
            firing = false;
            this->checkContinuity();
            Serial.print("Pyro Added ; ");
            pyros.push_back(this);
        }
        void checkContinuity();
        void fire(float wait = 0, float duration = 1);
        static void manager();
        //static const std::vector<Pyro*>& getPyros() {return pyros;}
    
    private:
        int pyro_pin;
        int cty_pin;
        unsigned long startTime;
        unsigned long endTime;
        bool continuity;
        bool firing;
        static std::vector<Pyro*> pyros;
};

#endif // PYRO_H