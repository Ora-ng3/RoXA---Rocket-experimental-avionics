#include <Arduino.h>
#include "Buzzer.h"

void initializeBuzzer() {
    pinMode(BUZZER, OUTPUT);
    digitalWrite(BUZZER, LOW);
}

void biiip(int nb, int d){
	for (int i=1; i<=nb; i++){
		digitalWrite(BUZZER, HIGH);
		delay(d);
		digitalWrite(BUZZER, LOW);
        if (i != nb) {delay(d);}
	}
}