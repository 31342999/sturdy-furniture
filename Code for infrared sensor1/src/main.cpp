
#include "Arduino.h"

  int AnalogPin = A0;
  int ledpin = 13;
  int sensorvalue = 0;

void setup()
{
  Serial.begin()
  pinMode(ledpin, OUTPUT);
  pinMode(AnalogPin, INPUT);
}

void loop()
{
  
}