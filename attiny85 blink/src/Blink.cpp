/*
 * Blink
 * Turns on an LED on for one second,
 * then off for one second, repeatedly.
 */

#include <Arduino.h>

void setup()
{
  pinMode(0, OUTPUT);
}

void loop()
{
  for (int i=0; i<3; ++i) {
    digitalWrite(0, HIGH);
    delay(100);
    digitalWrite(0, LOW);
    delay(100);
  }
  delay(1000);
}
