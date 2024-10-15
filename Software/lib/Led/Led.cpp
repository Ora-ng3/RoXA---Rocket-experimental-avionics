#include <Arduino.h>
#include "Led.h"

void initializeLed() {
    pinMode(LED_R, OUTPUT);
    pinMode(LED_G, OUTPUT);
    pinMode(LED_B, OUTPUT);
    digitalWrite(LED_R, LOW);
    digitalWrite(LED_G, LOW);
    digitalWrite(LED_B, LOW);
}

void led(float r, float g, float b){
    analogWrite(LED_R, r*256*LED_BRIGHTNESS);
    analogWrite(LED_G, g*256*LED_BRIGHTNESS);
    analogWrite(LED_B, b*256*LED_BRIGHTNESS);
}