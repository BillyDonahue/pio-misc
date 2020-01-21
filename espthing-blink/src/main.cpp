#include <Arduino.h>
//#include <Serial.h>

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.printf("Hello, world! (%zu sec)\n", static_cast<size_t>(millis()));
  delay(1000);
}