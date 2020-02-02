#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_DotStar.h>

Adafruit_DotStar strip(1, INTERNAL_DS_DATA, INTERNAL_DS_CLK, DOTSTAR_BGR);

static const auto red = strip.Color(0x44, 0, 0);
static const auto green = strip.Color(0, 0x44, 0);
static const auto blue = strip.Color(0, 0, 0x44);
static const auto yellow = strip.Color(0x44, 0x44, 0);


int blink = 0;

void setup() {
  pinMode(A0, INPUT);
  pinMode(A3, INPUT);
  pinMode(13, OUTPUT);
  pinMode(4, OUTPUT);

  Serial.begin(9600);

  strip.begin(); // Initialize pins for output
  strip.setBrightness(40);
  strip.show();  // Turn all LEDs off ASAP
}

int kTol = 50;  // 5.0%
static const unsigned long kDispenseSec = 10;
static const unsigned long kDispenseWait = 30;

// calibration:
static const uint32_t kDryAir = 885;
static const uint32_t kWater = 466;


ulong lastDispensed = 0;

void setColor(uint32_t c) {
  strip.setPixelColor(0, c);
}

void dispenseWater() {
  unsigned long dur = kDispenseSec * 1000;
  Serial.printf("(dispensing water for %lu msec)\n", dur);

  // pump is on pin4.  10sec of watering.
  digitalWrite(4, HIGH);
  delay(dur);
  digitalWrite(4, LOW);
}

void loop() {
  if (0) {
    uint32_t b = LOW;
    if (++blink>=3) {
      b = HIGH;
      blink = 0;
    }
    digitalWrite(13, b);
  }

  uint32_t rawWet = analogRead(A0);
  uint32_t rawPot = analogRead(A3);
  Serial.printf("(rawWet=%" PRIu32 ", rawPot=%" PRIu32 ")\n", rawWet, rawPot);



  int wet = map(rawWet, kWater, kDryAir, 1000, 0);
  int pot = map(rawPot, 0, 1024, 0, 1000);
  int err = pot - wet;
  if (err > kTol) {
      // too dry
      setColor(red);
  } else if (err < -kTol) {
    // too wet
    setColor(blue);
  } else if (err > kTol/2) {
    // almost too dry: yellow alert
    setColor(yellow);
  } else {
    // ok
    setColor(green);
  }
  strip.show();
  Serial.printf("(wet=.%03d, pot=.%03d) err=%d\n", wet, pot, err);

  if (err > kTol) {
    // too dry! Start the pump if we haven't done so too recently.
    auto t = millis();
    if (t - lastDispensed > kDispenseWait * 1000) {
      dispenseWater();
      lastDispensed = t;
    }
  }

  delay(10*1000);
}