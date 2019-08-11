// MultipleStripsInOneArray - see https://github.com/FastLED/FastLED/wiki/Multiple-Controller-Examples for more info on
// using multiple controllers.  In this example, we're going to set up four NEOPIXEL strips on three
// different pins, each strip will be referring to a different part of the single led array

#include "FastLED.h"

#define NUM_STRIPS 8
#define NUM_LEDS_PER_STRIP 36
#define NUM_LEDS (NUM_LEDS_PER_STRIP * NUM_STRIPS)

CRGB leds[NUM_LEDS];

// For mirroring strips, all the "special" stuff happens just in setup.  We
// just addLeds multiple times, once for each strip
void setup() {
  FastLED.addLeds<NEOPIXEL, 13>(leds, 0 * NUM_LEDS_PER_STRIP, NUM_LEDS_PER_STRIP);
  FastLED.addLeds<NEOPIXEL, 12>(leds, 1 * NUM_LEDS_PER_STRIP, NUM_LEDS_PER_STRIP);
  FastLED.addLeds<NEOPIXEL, 11>(leds, 2 * NUM_LEDS_PER_STRIP, NUM_LEDS_PER_STRIP);
  FastLED.addLeds<NEOPIXEL, 10>(leds, 3 * NUM_LEDS_PER_STRIP, NUM_LEDS_PER_STRIP);
  FastLED.addLeds<NEOPIXEL,  9>(leds, 4 * NUM_LEDS_PER_STRIP, NUM_LEDS_PER_STRIP);
  FastLED.addLeds<NEOPIXEL,  8>(leds, 5 * NUM_LEDS_PER_STRIP, NUM_LEDS_PER_STRIP);
  FastLED.addLeds<NEOPIXEL,  7>(leds, 6 * NUM_LEDS_PER_STRIP, NUM_LEDS_PER_STRIP);
  FastLED.addLeds<NEOPIXEL,  6>(leds, 7 * NUM_LEDS_PER_STRIP, NUM_LEDS_PER_STRIP);
}

CRGB colors[NUM_STRIPS] = {
  CRGB::Red, CRGB::Red,
  CRGB::Orange, CRGB::Orange,
  CRGB::Yellow, CRGB::Yellow,
  CRGB::White, CRGB::White
};

void loop() {
  for(int i = 0; i < NUM_LEDS_PER_STRIP; i++) {
    fadeToBlackBy(leds, NUM_LEDS, 128);
    for (int j = 0; j < NUM_STRIPS; ++j) {
      leds[j*NUM_LEDS_PER_STRIP+i] = colors[j];
    }
    FastLED.show();
    
    delay(100);
  }
}

