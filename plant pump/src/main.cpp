#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_DotStar.h>

static const float pi = 3.14159;

Adafruit_DotStar strip(1, INTERNAL_DS_DATA, INTERNAL_DS_CLK, DOTSTAR_BGR);

int blink = 0;

void setup() {
  pinMode(A0, INPUT);
  pinMode(A3, INPUT);
  pinMode(13, OUTPUT);

  Serial.begin(9600);

  strip.begin(); // Initialize pins for output
  strip.setBrightness(40);
  strip.show();  // Turn all LEDs off ASAP
}

int tol = 50;  // 5.0%

void loop() {

  for (int i = 0; 0; ++i) {
    switch(i%5){
      case 1:
        strip.setPixelColor(0, strip.Color(0x88, 0, 0));
        break;
      case 2:
        strip.setPixelColor(0, strip.Color(0, 0x88, 0));
        break;
      case 3:
        strip.setPixelColor(0, strip.Color(0, 0, 0x88));
        break;
      default:
        strip.setPixelColor(0, strip.Color(0, 0, 0));
        break;

    }
      strip.show();
      delay(1000);
  }


  {
    uint32_t b = LOW;
    if (++blink>=3) {
      b = HIGH;
      blink = 0;
    }
    digitalWrite(13, b);
  }

  int wet = map(analogRead(A0), 0, 1024, 1000, 0);
  int pot = map(analogRead(A3), 0, 1024, 0, 1000);
  int err = pot - wet;
  if (err > tol) {
      // too dry
      strip.setPixelColor(0, strip.Color(0x44, 0, 0));
  } else if (err < -tol) {
    // too wet
    strip.setPixelColor(0, strip.Color(0, 0, 0x44));
  } else if (err > tol/2) {
    // almost too dry: yellow alert
    strip.setPixelColor(0, strip.Color(0x44, 0x44, 0));
  } else {
    // ok
    strip.setPixelColor(0, strip.Color(0, 0x44, 0));
  }
  strip.show();
  Serial.printf("(wet=.%03d, pot=.%03d) err=%d\n", wet, pot, err);
  delay(1000);
}