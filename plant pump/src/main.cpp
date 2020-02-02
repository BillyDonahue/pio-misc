#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_DotStar.h>

Adafruit_DotStar strip(1, INTERNAL_DS_DATA, INTERNAL_DS_CLK, DOTSTAR_BGR);

static const auto red = strip.Color(0x44, 0, 0);
static const auto green = strip.Color(0, 0x44, 0);
static const auto blue = strip.Color(0, 0, 0x44);
static const auto yellow = strip.Color(0x44, 0x44, 0);
static const auto white = strip.Color(0x44, 0x44, 0x44);
static const auto black = strip.Color(0, 0, 0);

void setColor(uint32_t c) {
  strip.setPixelColor(0, c);
  strip.show();
}

const int kTol = 50;  // 5.0%
const unsigned long kDispenseSec = 10;
const unsigned long kDispenseWait = 30;

// Can only enter watering cycle a few times per day.
// Let's say every 4 hours.
const unsigned long kMinWateringCyclePeriod = 4*60*60;

// calibration:
const uint32_t kDryAir = 885;
const uint32_t kWater = 466;


ulong lastDispensed = 0;  // ts msec

// In watering cycle currently.
bool watering = false;
ulong lastWateringCycle = 0;  // ts msec

void dispenseWater() {
  unsigned long dur = kDispenseSec * 1000;
  Serial.printf("(dispensing water for %lu msec)\n", dur);

  // pump is on pin4.  10sec of watering.
  setColor(white);
  digitalWrite(13,HIGH);
  digitalWrite(4, HIGH);
  delay(dur);
  digitalWrite(4, LOW);
  digitalWrite(13,LOW);
  setColor(black);
}

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

void loop() {
  uint32_t rawWet = analogRead(A0);
  uint32_t rawPot = analogRead(A3);
  Serial.printf("(rawWet=%" PRIu32 ", rawPot=%" PRIu32 ")\n", rawWet, rawPot);

  int wet = map(rawWet, kWater, kDryAir, 1000, 0);
  int pot = map(rawPot, 0, 1024, 0, 1000);
  int err = pot - wet;
  if (err > kTol) {
      // too dry
      setColor(red);
      auto t = millis();
      if (!lastWateringCycle || t - lastWateringCycle >= kMinWateringCyclePeriod * 1000) {
        watering = true;
        lastWateringCycle = t;
      }
  } else if (err < -kTol) {
    // too wet
    setColor(blue);
  } else if (err > kTol/2) {
    // almost too dry: yellow alert
    setColor(yellow);
  } else {
    // ok
    setColor(green);
    watering = false;
  }
  Serial.printf("(wet=.%03d, pot=.%03d) err=%d\n", wet, pot, err);

  if (watering) {
    // too dry! Start the pump if we haven't done so too recently.
    auto t = millis();
    if (t - lastDispensed > kDispenseWait * 1000) {
      dispenseWater();
      lastDispensed = t;
    }
  }

  delay(1*1000);
}