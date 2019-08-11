
#include "FastLED.h"

#define LED_TYPE WS2812B
#define COLOR_ORDER GRB

#define NUM_STRIPS 1
#define NUM_LEDS_PER_STRIP 15
#define NUM_LEDS (((NUM_STRIPS * NUM_LEDS_PER_STRIP)))

unsigned char BRIGHTNESS = 0xff;

CRGB leds[NUM_LEDS];

typedef union t_FieldData {
  unsigned char starfireData[NUM_LEDS]; // temperature for all cells
} 
FieldData;

FieldData fieldData;

#define DEFAULT_SPEED 0x3f
int speed = DEFAULT_SPEED;

float speedFactor() {
  return ((float)speed) / ((float)DEFAULT_SPEED);
}

void starfire(void);
void starfireInit(void);

#define NUM_MODES 1
int mode = 0;

void (*modes[])(void) = {
  &starfire // 0
};

void (*modeInits[])(void) = {
  &starfireInit, // 0
};

CRGB primaryColors[] = {
  CRGB::Red, // 0
  CRGB::OrangeRed, // 1
  CRGB::Yellow, // 2
  CRGB::Green, // 3
  CRGB::Cyan, // 4
  CRGB::Blue, // 5
  CRGB::Purple, // 6
  CRGB::Crimson // 7
};

#define NUM_COLORS 8
int color = 0;

CRGB primaryColor = primaryColors[color];

void increaseBrightness() {
  BRIGHTNESS = min(0xff, BRIGHTNESS+0x10);
  Serial.print(BRIGHTNESS, HEX);
  FastLED.setBrightness(BRIGHTNESS);
}

void decreaseBrightness() {
  BRIGHTNESS = max(0x0f, BRIGHTNESS-0x10);
  Serial.print(BRIGHTNESS, HEX);
  FastLED.setBrightness(BRIGHTNESS);
}

void nextMode() {
  mode = (mode+1)%NUM_MODES;
  Serial.println(mode, HEX);
  (*modeInits[mode])();
}

void previousMode() {
  mode = (mode+NUM_MODES-1)%NUM_MODES;
  Serial.println(mode, HEX);
  (*modeInits[mode])();
}

void nextColor() {
  color = (color+1)%NUM_COLORS;
  Serial.println(color, HEX);
  primaryColor = primaryColors[color];
}

void previousColor() {
  color = (color+NUM_COLORS-1)%NUM_COLORS;
  Serial.println(color, HEX);
  primaryColor = primaryColors[color];
}

void speedUp() {
  speed = min(0xff, speed + 0x10);
  Serial.println(speed, HEX);
}

void speedDown() {
  speed = max(0x0f, speed - 0x10);
  Serial.println(speed, HEX);
}

unsigned char special = 0;

void specialPlus() {
  ++special;
  Serial.println(special, HEX);
}

void specialMinus() {
  --special;
  Serial.println(special, HEX);
}

void handleSerial() {
  if (Serial.available()) {
    byte cmd = Serial.read();
    Serial.print(cmd, HEX);
    switch (cmd) {
    case '1':
      increaseBrightness();
      break;
    case '2':
      decreaseBrightness();
      break;
    case '3':
      nextMode();
      break;
    case '4':
      previousMode();
      break;
    case '5':
      nextColor();
      break;
    case '6':
      previousColor();
      break;
    case '7':
      speedUp();
      break;
    case '8':
      speedDown();
      break;
    case '9':
      specialPlus();
      break;
    case '0':
      specialMinus();
      break;
    case 'r':
      randomizeMode();
      break;
    default:
      break;
    }
    Serial.println("");
  }
}

void randomizeMode() {
  mode = random(NUM_MODES);
  color = random(NUM_COLORS);

  primaryColor = primaryColors[color];
  (*modeInits[mode])();
}

void setup() {
  Serial.begin(9600);
  
  FastLED.addLeds<LED_TYPE, 3, COLOR_ORDER>(leds, 0*NUM_LEDS_PER_STRIP, NUM_LEDS_PER_STRIP);
          
  FastLED.setBrightness(BRIGHTNESS);
  FastLED.setMaxPowerInVoltsAndMilliamps(5,450);

  randomSeed(analogRead(0) ^ analogRead(1) ^ analogRead(2) ^ analogRead(3) ^ analogRead(4) ^ analogRead(5));

  randomizeMode();
}

void loop() {
  handleSerial();
  (*modes[mode])();
}

//////////////////////// UTILITY ////////////////////////

void fillScreen(CRGB color) {
  for (int i = 0; i < NUM_LEDS; ++i) {
    leds[i] = color;
  }
  FastLED.show();
}

void clearScreen() {
  fillScreen(CRGB::Black);
}

float time() {
  return ((float)millis()) / 1000.0f * speedFactor();
}

float frac(float n) {
  return n - floor(n);
}

#define FTIME_STRETCH 1.0

float ftime() {
  return frac(time() * FTIME_STRETCH) * 2.0 - 1.0;
}

/**
 * @param t current step [0.0, 1.0]
 */
float easeInOutExpo(float t) {
  t *= 2;
  if (t < 1) {
    return 0.5 * pow( 2, 10 * (t - 1) );
  } else {
    t--;
    return 0.5 * ( -pow( 2, -10 * t) + 2 );
  }
}

//////////////////////// STARFIRE ////////////////////////

#define STARFIRE__DATA ((fieldData.starfireData))

unsigned char starfireDataGet(unsigned int index) {
  return STARFIRE__DATA[index];
}

void starfireDataSet(unsigned int index, unsigned char value) {
  STARFIRE__DATA[index] = value;
}

void starfireInit() {
  FastLED.setBrightness(BRIGHTNESS);
  
  for (int i = 0; i < NUM_LEDS; ++i) {
    starfireDataSet(i, 0);
  }
  clearScreen;
}

CRGB starfireColorRegular(unsigned char temperature) {
  if (temperature < 64) {
    // black to red
    return CHSV(0, 255, 4 * temperature);
  } else {
    // red to yellow
    return CHSV((temperature - 64) / 4, 255, 255);
  }
}

CRGB starfireColorHell(unsigned char temperature) {
  if (temperature < 64) {
    // black to crimson
    return CHSV(240, 255, 4 * temperature);
  } else if (temperature < 192) {
    // crimson to red
    return CHSV(240 + (temperature - 64) / 8, 255, 255);
  } else {
    // red to orange
    return CHSV((temperature - 192) / 4, 255, 255);
  }
}

CRGB starfireColorSun(unsigned char temperature) {
  if (temperature < 128) {
    // black to yellow
    return CRGB(2 * temperature, 2 * temperature, 0);
  } else {
    // yellow to white
    return CRGB(255, 255, temperature - 128);
  }
}

CRGB starfireColorIce(unsigned char temperature) {
  if (temperature < 64) {
    // black to blue
    return CHSV(160, 255, 4 * temperature);
  } else if (temperature < 128) {
    return CHSV(160, 255, 255);
  } else {
    // blue to white
    return CHSV(160, 255 - (2 * (temperature - 128)), 255);
  } 
}

CRGB starfireColorSky(unsigned char temperature) {
  if (temperature < 64) {
    // black to cyan
    return CRGB(0, 4 * temperature, 4 * temperature);
  } else if (temperature < 128) {
    // cyan to light blue
    return CRGB(0, 255 - (temperature - 64), 255);
  } else {
    // blue to white
    return CRGB(2 * (temperature - 128), 192 + (temperature - 128) / 2, 255);
  } 
}

CRGB starfireColorPoison(unsigned char temperature) {
  if (temperature < 128) {
    // black to green
    return CRGB(0, 2 * temperature, 0);
  } else {
    // green to ???
    return CRGB(temperature - 128, 255, 0);
  }
}

CRGB starfireColorSoul(unsigned char temperature) {
  if (temperature < 64) {
    // black to purple
    return CRGB(2 * temperature, 0, 4 * temperature);
  } else if (temperature < 192) {
    // purple to magenta
    return CRGB(128 + (temperature - 64), 0, 255);
  } else {
    // magenta to white
    return CRGB(255, 4 * (temperature - 192), 255);
  }
}

CRGB starfireColorCrimson(unsigned char temperature) {
  if (temperature < 128) {
    // black to pink
    return CRGB(2 * temperature, 0, 2 * temperature);
  } else {
    // pink to crimson
    return CRGB(255, temperature - 128, 255);
  }
}

#define NUM_STARFIRE_COLORS 8

CRGB (*starfireColor[])(unsigned char temperature) = {
  &starfireColorHell, // 0
  &starfireColorRegular, // 1
  &starfireColorSun, // 2
  &starfireColorPoison, // 3
  &starfireColorSky, // 4
  &starfireColorIce, // 5
  &starfireColorSoul, // 6
  &starfireColorCrimson // 7
};

void starfire() {
  for (int i = 0; i < NUM_LEDS_PER_STRIP; ++i) {
    unsigned char old = starfireDataGet(i)-16;
    starfireDataSet(i, max(old, random(255)));
  }
  
  for (int i = 0; i < NUM_LEDS; ++i) {
    leds[i] = (*starfireColor[color%NUM_STARFIRE_COLORS])(starfireDataGet(i));
  }
  
  FastLED.show();
  delay(10);
}

