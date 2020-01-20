#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_DotStar.h>


Adafruit_SSD1306 display(128, 32, &Wire, -1);
Adafruit_DotStar strip(1, INTERNAL_DS_DATA, INTERNAL_DS_CLK, DOTSTAR_BRG);

void setup() {
  pinMode(A0, INPUT);
  pinMode(A3, INPUT);
  pinMode(13, OUTPUT);

  display.begin(SSD1306_SWITCHCAPVCC, 0x3C); // Address 0x3C for 128x32
  strip.begin(); // Initialize pins for output
  strip.setBrightness(40);
  display.display();
  strip.show();  // Turn all LEDs off ASAP
}

int blink = 0;

void loop() {
  if (blink++ >= 10) {
    digitalWrite(13, 1);
    blink = 0;
  } {
    digitalWrite(13, 0);
  }

  // put your main code here, to run repeatedly:
  display.clearDisplay();
  display.setTextSize(1);      // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE); // Draw white text
  display.setCursor(0, 0);     // Start at top-left corner

  int x = analogRead(A0);
  int y = analogRead(A3);

  int xd = map(x, 0, 1023, 64 + 1, 64 + 32 - 1);
  int yd = map(y, 0, 1023, 31, 1);
  const int cross[9][2] = {
    {0, 0},
    {-2, 0}, {-1, 0}, {1, 0}, {2, 0},
    {0, -2}, {0, -1}, {0, 1}, {0, 2},
  };
  for (auto&& c : cross)
    display.drawPixel(xd + c[0], yd + c[1], SSD1306_WHITE);

  display.drawRect(64, 0, 32, 32, SSD1306_WHITE);

  float xf = x/512. - .5;
  float yf = y/512. - .5;
  float r = hypotf(xf, yf);
  float th = atan2f(yf, xf);
  float pi = 3.14159;

  if (th < 0)
    th = 2 * pi + th;

  strip.setPixelColor(0, strip.ColorHSV(th * 65536. / (2*pi), r * 256, 128));
  strip.show();

  display.printf("xf=%3d\nyf=%3d\n", (int)(xf*100), (int)(yf*100));
  display.printf("r=%3d\nth=%3d", (int)(r*100), (int)(th*180/pi));
  display.display();
  
  delay(50);
}
