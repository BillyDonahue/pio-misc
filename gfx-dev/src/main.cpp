#include <Arduino.h>
#include <Adafruit_GFX.h>

GFXcanvas1 disp(64, 64);

void setup() {
  // put your setup code here, to run once:
  disp.writeFastHLine(0, 0, 64, 1);
}

void loop() {
  // put your main code here, to run repeatedly:
}