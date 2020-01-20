/**
 * Blink
 *
 * Turns on an LED on for one second,
 * then off for one second, repeatedly.
 */
#include "Arduino.h"
#include "Adafruit_NeoPixel.h"


// Set LED_BUILTIN if it is not defined by Arduino framework
// #define LED_BUILTIN 13

#define PIN       40
#define NUMPIXELS 1
#define DELAYVAL 500 // Time (in milliseconds) to pause between pixels

Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

void setup()
{
  // initialize LED digital pin as an output.
  pinMode(LED_BUILTIN, OUTPUT);
  pixels.begin();
}

int c = 0;

void loop()
{
  auto hue = [](int c, int c0) {
    return uint8_t(abs((c - c0 * 33) % 100 - 50) / 5);
  };
  uint8_t rgb[] = { hue(c,0), hue(c,1), hue(c,2) };

  // pixels.clear();
  pixels.setPixelColor(0, pixels.Color(rgb[0], rgb[1], rgb[2]));
  pixels.show();
  ++c;
  delay(10);

  //pixels.setPixelColor(0, 0);
  //pixels.show();
  //delay(50);

}
