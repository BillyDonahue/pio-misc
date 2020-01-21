#include <Arduino.h>
//#include <Serial.h>

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.print("hello\n");
  delay(1000);
}