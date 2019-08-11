#include "FastLED.h"

#define LED_TYPE WS2812B
#define COLOR_ORDER GRB

#define NUM_LEDS 15

unsigned char Brightness = 0xff;

CRGB leds[NUM_LEDS];
CRGB leds2[NUM_LEDS];

void setup() {
  Serial.begin(9600);
  
  FastLED.addLeds<LED_TYPE, 3, COLOR_ORDER>(leds, 0, NUM_LEDS);
  FastLED.addLeds<LED_TYPE, 2, COLOR_ORDER>(leds2, 0, NUM_LEDS);
  
  FastLED.setBrightness(Brightness);
  FastLED.setMaxPowerInVoltsAndMilliamps(5,450);
  
  randomSeed(analogRead(0) ^ analogRead(1) ^ analogRead(2) ^ analogRead(3) ^ analogRead(4) ^ analogRead(5) ^ analogRead(6));
}

int pos = 0;
int dir = 1;
int pos2 = 0;

void loop() {
  if (pos == (NUM_LEDS-1)) dir = -1;
  else if (pos == 0) dir = 1;
  
  for (int i = 0; i < NUM_LEDS; ++i) {
    leds[i].fadeToBlackBy(32);
    leds2[i].fadeToBlackBy(32);
  }
  
  leds[pos] = CRGB(0, 255, 0);
  leds2[pos2] = CRGB(0, random(255), 0);
  
  FastLED.show();
  
  pos += dir;
  pos2 = (pos2 + 1) % NUM_LEDS;
  
  delay(100);
}


