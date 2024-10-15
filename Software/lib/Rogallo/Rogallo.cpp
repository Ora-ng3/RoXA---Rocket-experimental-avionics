#include <Arduino.h>
#include "Rogallo.h"
#include "Pins.h"

int R = 2;
int value = LOW;
int d = 10*1000;

void initializeRogallo() {
    pinMode(STEP, OUTPUT); //Step pin as output
    pinMode(DIR,  OUTPUT); //Direcction pin as output
    digitalWrite(STEP, LOW); // Currently no stepper motor movement
    digitalWrite(DIR, LOW);
}

void rotate_clockwise(float n){
  digitalWrite(DIR, HIGH); //Rotate stepper motor in clock wise direction
          for( int i=1;i<=200*n;i++){
          digitalWrite(STEP, HIGH);
          delay(2);
          digitalWrite(STEP, LOW);
          delay(2);}
    value = HIGH;
}

void rotate_anticlockwise(float n){
  digitalWrite(DIR, LOW); //Rotate stepper motor in anti clock wise direction
            for( int i=1;i<=200*n;i++){
            digitalWrite(STEP, HIGH);
            delay(2);
            digitalWrite(STEP, LOW);
            delay(2);}
      value = LOW;
}