// MultipleStripsInOneArray - see https://github.com/FastLED/FastLED/wiki/Multiple-Controller-Examples for more info on
// using multiple controllers.  In this example, we're going to set up four NEOPIXEL strips on three
// different pins, each strip will be referring to a different part of the single led array

#include "FastLED.h"

#define LED_TYPE WS2812B
#define COLOR_ORDER GRB

#define NUM_STRIPS 8
#define NUM_LEDS_PER_STRIP 36
#define NUM_LEDS (((NUM_STRIPS * NUM_LEDS_PER_STRIP)))

#define NUM_SNAKES 26

CRGB leds[NUM_LEDS];

byte BRIGHTNESS = 0x1f;

typedef union t_FieldData {
  unsigned char gameOfLifeData[NUM_LEDS*2/4]; // alive flags for all cells, frontbuffer+backbuffer interleaved
  unsigned char fireData[NUM_LEDS]; // temperature for all cells, frontbuffer+backbuffer
  //  unsigned char audioSpectrumData[NUM_LEDS_PER_STRIP]; //averaged amplitude per frequency band
  unsigned char snakeData[NUM_SNAKES*6];
  unsigned char cyberData[1];
  // midiPlayerData
} 
FieldData;

FieldData fieldData;

#define DEFAULT_SPEED 0x3f
int speed = DEFAULT_SPEED;

float speedFactor() {
  return ((float)speed) / ((float)DEFAULT_SPEED);
}

void snakes(void);
void snakesInit(void);
void gameOfLife(void);
void gameOfLifeInit(void);
void rainbowSwipe(void);
void rainbowSwipeInit(void);
void spinner(void);
void spinnerInit(void);
void party(void);
void partyInit(void);
void fade();
void fadeInit();
void pulse();
void pulseInit();
void breathe();
void breatheInit();
void wobble();
void wobbleInit();
void fire(void);
void fireInit(void);
void usPolice(void);
void usPoliceInit(void);
void cyber(void);
void cyberInit(void);
//void audioSpectrum(void);
//void audioSpectrumInit(void);
//void midiPlayer();
//void midiPlaterInit();

#define NUM_MODES 14
int mode = 0;

void (*modes[])(void) = {
  &snakes, // 0
  &gameOfLife, // 1
  &rainbowSwipe, // 2
  &spinner, // 3
  &party, // 4
  &fade, // 5
  &pulse, // 6
  &breathe, // 7
  &wobble, // 8
  &rainbowWalk, // 9
  &rainbowSwap, // 10
  &fire, // 11
  &usPolice, // 12
  &cyber // 13
};

void (*modeInits[])(void) = {
  &snakesInit, // 0
  &gameOfLifeInit, // 1
  &rainbowSwipeInit, // 2
  &spinnerInit, // 3
  &partyInit, // 4
  &fadeInit, // 5
  &pulseInit, // 6
  &breatheInit, // 7
  &wobbleInit, // 8
  &rainbowWalkInit, // 9
  &rainbowSwapInit, // 10
  &fireInit, // 11
  &usPoliceInit, // 12
  &cyberInit // 13
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
int color = 4;

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

/*
#define NUM_BUTTONS 8
 
 int buttonPins[NUM_BUTTONS] {
 12, 11, 10, 9, 8, 7, 6, 5
 };
 
 int buttonStates[NUM_BUTTONS] {
 LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW
 };
 
 void (*buttonFunctions[NUM_BUTTONS])(void) = {
 &increaseBrightness,
 &decreaseBrightness,
 &nextMode,
 &previousMode,
 &nextColor,
 &previousColor,
 &speedUp,
 &speedDown
 };
 
 void handleButtons() {
 for (int i = 0; i < NUM_BUTTONS; ++i) {
 boolean buttonState = digitalRead(buttonPins[i]);
 
 if (buttonState && buttonState != buttonStates[i]) {
 buttonFunctions[i]();
 }
 
 buttonStates[i] = buttonState;
 }
 }
 */

void randomizeMode() {
  //  0 snakes,
  //  1 gameOfLife,
  //  2 rainbowSwipe,
  //  3 spinner,
  //  4 party,
  //  5 fade,
  //  6 pulse,
  //  7 breathe,
  //  8 wobble,
  //  9 rainbowWalk
  // 10 rainbowSwap
  // 11 fire
  // 12 usPolice
  // 13 cyber
  int modeTable[] = {
     0,  0, 10, 10,
     2,  2,  3,  4,
     9,  9,  9,  9,
    11, 11, 11, 11,
    13, 13, 13, 13
  };
  int r = random(20);
  Serial.println(r);
  mode = modeTable[r];
  color = random(NUM_COLORS);

  primaryColor = primaryColors[color];
  (*modeInits[mode])();
}

void setup() {
  Serial.begin(9600);
  
  FastLED.addLeds<LED_TYPE, 12, COLOR_ORDER>(leds, 0 * NUM_LEDS_PER_STRIP, NUM_LEDS_PER_STRIP);
  FastLED.addLeds<LED_TYPE, 11, COLOR_ORDER>(leds, 1 * NUM_LEDS_PER_STRIP, NUM_LEDS_PER_STRIP);
  FastLED.addLeds<LED_TYPE, 10, COLOR_ORDER>(leds, 2 * NUM_LEDS_PER_STRIP, NUM_LEDS_PER_STRIP);
  FastLED.addLeds<LED_TYPE,  9, COLOR_ORDER>(leds, 3 * NUM_LEDS_PER_STRIP, NUM_LEDS_PER_STRIP);
  FastLED.addLeds<LED_TYPE,  8, COLOR_ORDER>(leds, 4 * NUM_LEDS_PER_STRIP, NUM_LEDS_PER_STRIP);
  FastLED.addLeds<LED_TYPE,  7, COLOR_ORDER>(leds, 5 * NUM_LEDS_PER_STRIP, NUM_LEDS_PER_STRIP);
  FastLED.addLeds<LED_TYPE,  6, COLOR_ORDER>(leds, 6 * NUM_LEDS_PER_STRIP, NUM_LEDS_PER_STRIP);
  FastLED.addLeds<LED_TYPE,  5, COLOR_ORDER>(leds, 7 * NUM_LEDS_PER_STRIP, NUM_LEDS_PER_STRIP);
  FastLED.setBrightness(BRIGHTNESS);

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

void dimScreenByAmount(byte amount) {
  for (int i = 0; i < NUM_LEDS; ++i) {
    leds[i].r = max(0, (int)leds[i].r-amount);
    leds[i].g = max(0, (int)leds[i].g-amount);
    leds[i].b = max(0, (int)leds[i].b-amount);
  }
}

void dimScreenByFactor(float factor) {
  for (int i = 0; i < NUM_LEDS; ++i) {
    leds[i].r *= factor;
    leds[i].g *= factor;
    leds[i].b *= factor;
    leds[i].r = (leds[i].r >= 0xf) ? leds[i].r : 0;
    leds[i].g = (leds[i].g >= 0xf) ? leds[i].g : 0;
    leds[i].b = (leds[i].b >= 0xf) ? leds[i].b : 0;
  }
}

void rgb2hsv(const unsigned char &src_r, const unsigned char &src_g, const unsigned char &src_b, unsigned char &dst_h, unsigned char &dst_s, unsigned char &dst_v)
{
  float r = src_r / 255.0f;
  float g = src_g / 255.0f;
  float b = src_b / 255.0f;

  float h, s, v; // h:0-360.0, s:0.0-1.0, v:0.0-1.0

  float maxv = max(max(r, g), b);
  float minv = min(min(r, g), b);

  v = maxv;

  if (maxv == 0.0f) {
    s = 0;
    h = 0;
  }
  else if (maxv - minv == 0.0f) {
    s = 0;
    h = 0;
  }
  else {
    s = (maxv - minv) / maxv;

    if (maxv == r) {
      h = 60 * ((g - b) / (maxv - minv)) + 0;
    }
    else if (maxv == g) {
      h = 60 * ((b - r) / (maxv - minv)) + 120;
    }
    else {
      h = 60 * ((r - g) / (maxv - minv)) + 240;
    }
  }

  if (h < 0) h += 360.0f;

  dst_h = (unsigned char)(h / 2);   // dst_h : 0-180
  dst_s = (unsigned char)(s * 255); // dst_s : 0-255
  dst_v = (unsigned char)(v * 255); // dst_v : 0-255
}

void hsv2rgb(const unsigned char &src_h, const unsigned char &src_s, const unsigned char &src_v, unsigned char &dst_r, unsigned char &dst_g, unsigned char &dst_b)
{
  float h = src_h *   2.0f; // 0-360
  float s = src_s / 255.0f; // 0.0-1.0
  float v = src_v / 255.0f; // 0.0-1.0

  float r, g, b; // 0.0-1.0

  int   hi = (int)(h / 60.0f) % 6;
  float f  = (h / 60.0f) - hi;
  float p  = v * (1.0f - s);
  float q  = v * (1.0f - s * f);
  float t  = v * (1.0f - s * (1.0f - f));

  switch(hi) {
  case 0: 
    r = v, g = t, b = p; 
    break;
  case 1: 
    r = q, g = v, b = p; 
    break;
  case 2: 
    r = p, g = v, b = t; 
    break;
  case 3: 
    r = p, g = q, b = v; 
    break;
  case 4: 
    r = t, g = p, b = v; 
    break;
  case 5: 
    r = v, g = p, b = q; 
    break;
  }

  dst_r = (unsigned char)(r * 255); // dst_r : 0-255
  dst_g = (unsigned char)(g * 255); // dst_r : 0-255
  dst_b = (unsigned char)(b * 255); // dst_r : 0-255
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

//////////////////////// SNAKES ////////////////////////

CRGB SNAKE_BLACK = CRGB::Black;

#define SNAKE_DATA ((fieldData.snakeData))

void drawSnakes() {
  for (int i = 0; i < NUM_SNAKES; i++) {
    byte r = SNAKE_DATA[i*6  ];
    byte g = SNAKE_DATA[i*6+1];
    byte b = SNAKE_DATA[i*6+2];
    byte x = SNAKE_DATA[i*6+3];
    byte y = SNAKE_DATA[i*6+4];

    leds[x+y*NUM_LEDS_PER_STRIP] = ((((unsigned long)r)<<16) + (((unsigned long)g)<<8) + ((unsigned long)b));
  }
}

void snakesInit() {
  FastLED.setBrightness(BRIGHTNESS);
  
  for (int i = 0; i < NUM_SNAKES; ++i) {
    byte h = random(0x100);
    byte s = 0xFF;
    byte v = 0xFF;
    byte r, g, b;

    hsv2rgb(h, s, v, r, g, b);

    SNAKE_DATA[i*6  ] = r; //R
    SNAKE_DATA[i*6+1] = g; //G 
    SNAKE_DATA[i*6+2] = b; //B
    SNAKE_DATA[i*6+3] = random(NUM_LEDS_PER_STRIP); //X
    SNAKE_DATA[i*6+4] = random(NUM_STRIPS); //Y
    SNAKE_DATA[i*6+5] = random(4); //D
  }

  clearScreen();

  drawSnakes();
  FastLED.show();
}

void snakes() {
  for (int i = 0; i < NUM_SNAKES; ++i) {
    byte x = SNAKE_DATA[i*6+3];
    byte y = SNAKE_DATA[i*6+4];

    switch (SNAKE_DATA[i*6+5]) {
    case 0:
      x = (x+NUM_LEDS_PER_STRIP-1)%NUM_LEDS_PER_STRIP;
      if (leds[x+y*NUM_LEDS_PER_STRIP] == SNAKE_BLACK)
        SNAKE_DATA[i*6+3] = x;
     break;
    case 1:
      if (y == 0) break;
      y = (y+NUM_STRIPS-1)%NUM_STRIPS;
      if (leds[x+y*NUM_LEDS_PER_STRIP] == SNAKE_BLACK)
        SNAKE_DATA[i*6+4] = y;
      break;
    case 2:
      x = (x+1)%NUM_LEDS_PER_STRIP;
      if (leds[x+y*NUM_LEDS_PER_STRIP] == SNAKE_BLACK)
        SNAKE_DATA[i*6+3] = x;
      break;
    case 3:
      if (y == NUM_STRIPS-1) break;
      y = (y+1)%NUM_STRIPS;
      if (leds[x+y*NUM_LEDS_PER_STRIP] == SNAKE_BLACK)
        SNAKE_DATA[i*6+4] = y;
      break;
    default:
      break;
    }

    SNAKE_DATA[i*6+5] = (SNAKE_DATA[i*6+5]+4+random(3)-1)%4;
  }

  dimScreenByFactor(0.75f);
  drawSnakes();
  FastLED.show();
  delay(((int)(100.0*speedFactor())));
}

//////////////////////// GAME OF LIFE ////////////////////////

//#define GOL_DATA ((fieldData.gameOfLifeData))
//#define GOL_RESET_CYCLE_COUNT 250

//#define GOL_DIE_WHEN_FEWER 2
//#define GOL_DIE_WHEN_ABOVE 3
//#define GOL_BIRTH_WHEN_EQUAL 3

//unsigned int cycleCount = 0;
//boolean firstIsFrontbuffer = true;

//int bytePos(int x, int y, boolean currentBuffer) {
//  return (x+y*NUM_LEDS_PER_STRIP)/4;
//}

/*
cb  ff  offset
 0    0  0
 0    1  1
 1    0  1
 1    1  0
 */

//int bitPos(int x, int y, boolean currentBuffer) {
//  return (x+y*NUM_LEDS_PER_STRIP)%4 + ((currentBuffer ^ firstIsFrontbuffer) ? 1 : 0);
//}

//boolean isAlive(int x, int y) {
//  int bitP = bitPos(x, y, true);
//  int byteP = bytePos(x, y, true);
//
//  return ((GOL_DATA[byteP])&(1<<bitP));
//}

//boolean isAliveNextTurn(int x, int y) {
//  int neighborAliveCount = 0;
//
//  neighborAliveCount += (isAlive(((x+NUM_LEDS_PER_STRIP-1)%NUM_LEDS_PER_STRIP)-1,(y+NUM_STRIPS-1)%NUM_STRIPS)) ? 1 : 0;
//  neighborAliveCount += (isAlive(((x+NUM_LEDS_PER_STRIP-1)%NUM_LEDS_PER_STRIP)+0,(y+NUM_STRIPS-1)%NUM_STRIPS)) ? 1 : 0;
//  neighborAliveCount += (isAlive(((x+NUM_LEDS_PER_STRIP-1)%NUM_LEDS_PER_STRIP)+1,(y+NUM_STRIPS-1)%NUM_STRIPS)) ? 1 : 0;
//  neighborAliveCount += (isAlive(((x+NUM_LEDS_PER_STRIP+0)%NUM_LEDS_PER_STRIP)-1,(y+NUM_STRIPS-1)%NUM_STRIPS)) ? 1 : 0;
//  neighborAliveCount += (isAlive(((x+NUM_LEDS_PER_STRIP+0)%NUM_LEDS_PER_STRIP)+1,(y+NUM_STRIPS-1)%NUM_STRIPS)) ? 1 : 0;
//  neighborAliveCount += (isAlive(((x+NUM_LEDS_PER_STRIP+1)%NUM_LEDS_PER_STRIP)-1,(y+NUM_STRIPS-1)%NUM_STRIPS)) ? 1 : 0;
//  neighborAliveCount += (isAlive(((x+NUM_LEDS_PER_STRIP+1)%NUM_LEDS_PER_STRIP)+0,(y+NUM_STRIPS-1)%NUM_STRIPS)) ? 1 : 0;
//  neighborAliveCount += (isAlive(((x+NUM_LEDS_PER_STRIP+1)%NUM_LEDS_PER_STRIP)+1,(y+NUM_STRIPS-1)%NUM_STRIPS)) ? 1 : 0;
//
//  return (isAlive(x, y) && (neighborAliveCount >= GOL_DIE_WHEN_FEWER) && (neighborAliveCount <= GOL_DIE_WHEN_ABOVE)) || (neighborAliveCount == GOL_BIRTH_WHEN_EQUAL);
//}

//void setGOL(int x, int y, boolean v) {
//  int bitP = bitPos(x, y, false);
//  int byteP = bytePos(x, y, false);
//
//  if (v) {
//    GOL_DATA[byteP] |= ((char)(1<<bitP));
//  } 
//  else {
//    GOL_DATA[byteP] &= ((char)(0xff^(1<<bitP)));
//  }
//}

void gameOfLifeInit() {
  FastLED.setBrightness(BRIGHTNESS);
//  for (int i = 0; i < (NUM_LEDS*2/4); ++i) {
//    GOL_DATA[i] = ((char)(random(256)));
//  }
//
//  cycleCount = 0;
//  clearScreen();
}

void gameOfLife() {
//  for (int i = 0; i < NUM_LEDS_PER_STRIP; ++i) {
//    for (int j = 0; j < NUM_STRIPS; ++j) {
//      boolean v = isAliveNextTurn(i, j);
//      setGOL(i, j, v);
//      leds[i+j*NUM_LEDS_PER_STRIP] = (v) ? primaryColor : CRGB::Black;
//    }
//  }
//
//  FastLED.show();
//  delay(1000);
}

//////////////////////// FIRE ////////////////////////

#define FIRE__DATA ((fieldData.fireData))

boolean firstBufferIsFrontBuffer = false;

unsigned char fireDataGet(unsigned int index) {
  if (firstBufferIsFrontBuffer) {
    unsigned char c = FIRE__DATA[index];
    unsigned char mask = 0xf0;
    c &= mask;
    c >>= 4;
    return (unsigned char)(c * 16);
  } else {
    unsigned char c = FIRE__DATA[index];
    unsigned char mask = 0x0f;
    c &= mask;
    return (unsigned char)(c * 16);
  }
}

void fireDataSet(unsigned int index, unsigned char value) {
  value /= 16;
  if (firstBufferIsFrontBuffer) {
    unsigned char c = FIRE__DATA[index];
    unsigned char mask = 0xf0;
    c &= mask;
    c |= value;
    FIRE__DATA[index] = c;
  } else {
    unsigned char c = FIRE__DATA[index];
    unsigned char mask = 0x0f;
    c &= mask;
    value <<= 4;
    c |= value;
    FIRE__DATA[index] = c;
  }
}

void fireInit() {
  FastLED.setBrightness(BRIGHTNESS);
  
  firstBufferIsFrontBuffer = true;  
  for (int i = 0; i < NUM_LEDS; ++i) {
    fireDataSet(i, 0);
  }
  firstBufferIsFrontBuffer = false;
  for (int i = 0; i < NUM_LEDS; ++i) {
    fireDataSet(i, 0);
  }
  clearScreen;
}

CRGB fireColorRegular(unsigned char temperature) {
  //Serial.print((int)temperature);
  if (temperature < 64) {
    // black to red
    return CHSV(0, 255, 4 * temperature);
  } else {
    // red to yellow
    return CHSV((temperature - 64) / 4, 255, 255);
  }
}

CRGB fireColorHell(unsigned char temperature) {
  //Serial.print((int)temperature);
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

CRGB fireColorSun(unsigned char temperature) {
  //Serial.print((int)temperature);
  if (temperature < 128) {
    // black to yellow
    return CRGB(2 * temperature, 2 * temperature, 0);
  } else {
    // yellow to white
    return CRGB(255, 255, temperature - 128);
  }
}

CRGB fireColorIce(unsigned char temperature) {
  //Serial.print((int)temperature);
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

CRGB fireColorSky(unsigned char temperature) {
  //Serial.print((int)temperature);
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


CRGB fireColorPoison(unsigned char temperature) {
  //Serial.print((int)temperature);
  if (temperature < 128) {
    // black to green
    return CRGB(0, 2 * temperature, 0);
  } else {
    // green to ???
    return CRGB(temperature - 128, 255, 0);
  }
}

CRGB fireColorSoul(unsigned char temperature) {
  //Serial.print((int)temperature);
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

CRGB fireColorCrimson(unsigned char temperature) {
  //Serial.print((int)temperature);
  if (temperature < 128) {
    // black to pink
    return CRGB(2 * temperature, 0, 2 * temperature);
  } else {
    // pink to crimson
    return CRGB(255, temperature - 128, 255);
  }
}

#define NUM_FIRE_COLORS 8

CRGB (*fireColor[])(unsigned char temperature) = {
  &fireColorHell,
  &fireColorRegular,
  &fireColorSun,
  &fireColorPoison,
  &fireColorSky,
  &fireColorIce,
  &fireColorSoul,
  &fireColorCrimson
};

void fire() {
  // put some coal on the ground which should be burned:
  // Fill bottom row with  random values between 80 and 239
  for (int i = 0; i < NUM_LEDS_PER_STRIP; ++i) {
    fireDataSet(i, random(160) + 80);
  }
  
  // Add some hotspots:
  // bright sparkts at the bottom with 255
  for (int i = 0; i < 5; i++) {
    fireDataSet(random(NUM_LEDS_PER_STRIP), 255);
  }
  
  // move up the old fire, lower its temperature
  // since the resolution is so low no smoothing is done
  for (int j = 0; j < NUM_STRIPS-1; ++j) {
    for (int i = 0; i < NUM_LEDS_PER_STRIP; ++i) {
      unsigned char value = fireDataGet(j*NUM_LEDS_PER_STRIP+i);
      value = (value >= 32) ? value - 32 : 0;
      fireDataSet((j+1)*NUM_LEDS_PER_STRIP+i, value);
    }
  }
  
  for (int i = 0; i < NUM_LEDS; ++i) {
    leds[i] = (*fireColor[color%NUM_FIRE_COLORS])(fireDataGet(i));
  }
  
  FastLED.show();
  firstBufferIsFrontBuffer = !firstBufferIsFrontBuffer;
  delay(20);
}

//////////////////////// RAINBOW SWIPE ////////////////////////

void rainbowSwipeInit() {
  FastLED.setBrightness(BRIGHTNESS);
  clearScreen();
}

void rainbowSwipe() {
  int state = ((int)(frac(time())*255.0));

  for(int i = 0; i < NUM_LEDS_PER_STRIP; ++i) {
    for (int j = 0; j < NUM_STRIPS; j++) {
      leds[i+j*NUM_LEDS_PER_STRIP] = CHSV((state+(i*255/NUM_LEDS_PER_STRIP))%255, 255, 255);
    }
  }
  FastLED.show();
}

//////////////////////// SPINNER ////////////////////////

void spinnerInit() {
  FastLED.setBrightness(BRIGHTNESS);
  clearScreen();
}

void spinner() {
#define SPREAD 0.1

  float tim = time() * 0.7;
  float tim5 = tim + 0.5;

  for (int i = 0; i < NUM_LEDS_PER_STRIP; ++i) {
    float y = ((float)i)/((float)NUM_LEDS_PER_STRIP);
    float t0 = min(1.0, abs(y       - frac(tim)) / SPREAD);
    float t1 = min(1.0, abs(y + 1.0 - frac(tim)) / SPREAD);
    float t2 = min(1.0, abs(y - 1.0 - frac(tim)) / SPREAD);
    float t3 = min(1.0, abs(y       - frac(tim5)) / SPREAD);
    float t4 = min(1.0, abs(y + 1.0 - frac(tim5)) / SPREAD);
    float t5 = min(1.0, abs(y - 1.0 - frac(tim5)) / SPREAD);
    float t = min(min(min(t0, t1), min(t2, t3)), min(t4, t5));
    for (int j = 0; j < NUM_STRIPS; ++j) {
      leds[i+j*NUM_LEDS_PER_STRIP] = primaryColor;
      leds[i+j*NUM_LEDS_PER_STRIP].fadeLightBy(((char)(t*255.0)));
    }
  }

  FastLED.show();
}

//////////////////////// PARTY ////////////////////////

void partyInit(void) {
  FastLED.setBrightness(BRIGHTNESS);
  for (int i = 0; i < NUM_LEDS; ++i) {
    leds[i] = CHSV(random(256), 255, 255);
  }
}

void party(void) {
  for (int i = 0; i < 36; ++i) {
    leds[random(NUM_LEDS)] = CHSV(random(256), 255, 255);
  }
  FastLED.show();
}

//////////////////////// FADE ////////////////////////

void fadeInit() {
  FastLED.setBrightness(BRIGHTNESS);
  clearScreen();
}

void fade() {
  float f = sin(time())*0.45 + 0.45;

  for (int i = 0; i < NUM_LEDS; ++i) {
    leds[i] = primaryColor;
    leds[i].fadeLightBy(((char)(f*255.0)));
  }

  FastLED.show();
}

//////////////////////// PULSE ////////////////////////

void pulseInit() {
  FastLED.setBrightness(BRIGHTNESS);
  clearScreen();
}

void pulse() {
#define SPREAD 0.5

  float t = ftime() + 0.5;

  for (int i = 0; i < NUM_STRIPS; ++i) {
    float y = ((float)i)/((float)NUM_LEDS_PER_STRIP);
    float e = min(1.0, abs(y - t) / SPREAD);

    for (int j = 0; j < NUM_LEDS_PER_STRIP; ++j) {
      leds[j+i*NUM_LEDS_PER_STRIP] = primaryColor;
      leds[j+i*NUM_LEDS_PER_STRIP].fadeLightBy(((char)(e*255.0)));
    }
  }

  FastLED.show();
}

//////////////////////// BREATHE ////////////////////////

void breatheInit() {
  FastLED.setBrightness(BRIGHTNESS);
  clearScreen();
}

void breathe() {
  float s = sin(time()*.5) * 0.5 + 0.1;

  for (int i = 0; i < NUM_LEDS_PER_STRIP; ++i) {
    float x = ((float)i) / ((float)(NUM_LEDS_PER_STRIP-1));

    for (int j = 0; j < NUM_STRIPS; ++j) {
      float y = ((float)j) / ((float)(NUM_STRIPS-1));

      float t = abs(y * 2.0 - 1.0) ;//* abs(x * 2.0 - 1.0);
      float ts = min(1.0, max(0.0, t + s));

      leds[i+j*NUM_LEDS_PER_STRIP] = primaryColor;
      leds[i+j*NUM_LEDS_PER_STRIP].fadeLightBy(((char)(ts*255.0)));
    }
  }

  FastLED.show();
}


//////////////////////// WOBBLE ////////////////////////

void wobbleInit() {
  FastLED.setBrightness(BRIGHTNESS);
  clearScreen();
}

void wobble() {
  float f = 0.25;
  f *= (1.0 + sin(time()*10.0)*0.2);
  f *= (1.0 + sin(time()* 4.5)*0.2);

  char r = (char)(f*(float)primaryColor.r);
  char g = (char)(f*(float)primaryColor.g);
  char b = (char)(f*(float)primaryColor.b);

  for (int i = 0; i < NUM_LEDS; ++i) {
    leds[i] = CRGB(r, g, b);
  }

  FastLED.show();
}

//////////////////////// RAINBOW WALK ////////////////////////

void rainbowWalkInit() {
  FastLED.setBrightness(BRIGHTNESS);
  clearScreen(); 
}

void rainbowWalk() {
  int state = -((int)(frac(time()*0.2)*255.0));

  for(int i = 0; i < NUM_LEDS_PER_STRIP; ++i) {
    for (int j = 0; j < NUM_STRIPS; j++) {
      leds[i+j*NUM_LEDS_PER_STRIP] = CHSV(((state+(j*255/NUM_LEDS_PER_STRIP)))%255, 255, 255);
    }
  }
  FastLED.show();
}

//////////////////////// RAINBOW SWAP ////////////////////////

void rainbowSwapInit() {
  FastLED.setBrightness(BRIGHTNESS);
  clearScreen();
}

void rainbowSwap() {
  float ti = time() * 1.2;

  for (int j = 0; j < NUM_STRIPS; ++j) {
    float tim = ti + ((float)j/(float)NUM_STRIPS)*0.35;
    
    int phase = (int)tim;
    boolean transitionActive = (phase % 2) == 1;
  
    phase /= 2;
    phase %= 6;
  
    int hueCurrent = phase * 60;
    
    float t;
    if (transitionActive) {
      t = frac(tim);
    } else {
      t = 0.0;
    }
    float hC = (((float)hueCurrent) / 360.0);
    t = easeInOutExpo(t);
    float rH = (t / 6.0) + hC;
    int rowHue = (int)(rH * 256.0);
    
    for (int i = 0; i < NUM_LEDS_PER_STRIP; ++ i) {
      leds[i+j*NUM_LEDS_PER_STRIP] = CHSV(rowHue, 255, 255);
    }
  }
  
  FastLED.show();
}

//////////////////////// US POLICE ////////////////////////

void usPoliceInit() {
  FastLED.setBrightness(0xff);
}

void flashIn(float t, unsigned int x1, unsigned int x2, unsigned int y1, unsigned int y2, unsigned char h, unsigned char s) {
  unsigned char v = (unsigned char)(easeInOutExpo(t) * 256.0);
  CRGB color = CHSV(h, s, v);

  for (int j = y1; j <= y2; ++j) {
    for (int i = x1; i <= x2; ++i) {
      leds[j*NUM_LEDS_PER_STRIP+i] = color;
    }
  }
}

void flashOut(float t, unsigned int x1, unsigned int x2, unsigned int y1, unsigned int y2, unsigned char h, unsigned char s) {
  unsigned char v = (unsigned char)(easeInOutExpo(1.0 - t) * 256.0);
  CRGB color = CHSV(h, s, v);
  for (int j = y1; j <= y2; ++j) {
    for (int i = x1; i <= x2; ++i) {
      leds[j*NUM_LEDS_PER_STRIP+i] = color;
    }
  }
}

/*
0..1 black to red right
1..2 red to black right
2..3 black to white center
3..4 white to black center
4..5 black to red right
5..6 red to black right
6..7 black to blue left
7..8 blue to black left
8..9 black to white center
9..10 white to black center
10..11 black to blue left
11..12 blue to black left
*/
/*
0..3 Center back
8..15 Right
20..23 Center front
28..35 Left
*/
void usPolice() {
  clearScreen();
  
  float stage = frac(time()) * 12.0f;
  
  switch ((unsigned int)stage) {
    case 0:
      flashIn(frac(stage), 8, 15, 0, 7, 0, 255);
      break;
    case 1:
      flashOut(frac(stage), 8, 15, 0, 7, 0, 255);
      break;
    case 2:
      flashIn(frac(stage), 0, 3, 0, 7, 0, 0);
      flashIn(frac(stage), 20, 23, 0, 7, 0, 0);
      break;
    case 3:
      flashOut(frac(stage), 0, 3, 0, 7, 0, 0);
      flashOut(frac(stage), 20, 23, 0, 7, 0, 0);
      break;
    case 4:
      flashIn(frac(stage), 8, 15, 0, 7, 0, 255);
      break;
    case 5:
      flashOut(frac(stage), 8, 15, 0, 7, 0, 255);
      break;
    case 6:
      flashIn(frac(stage), 28, 35, 0, 7, 160, 255);
      break;
    case 7:
      flashOut(frac(stage), 28, 35, 0, 7, 160, 255);
      break;
    case 8:
      flashIn(frac(stage), 0, 3, 0, 7, 0, 0);
      flashIn(frac(stage), 20, 23, 0, 7, 0, 0);
      break;
    case 9:
      flashOut(frac(stage), 0, 3, 0, 7, 0, 0);
      flashOut(frac(stage), 20, 23, 0, 7, 0, 0);
      break;
    case 10:
      flashIn(frac(stage), 28, 35, 0, 7, 160, 255);
      break;
    case 11: 
      flashOut(frac(stage), 28, 35, 0, 7, 160, 255);
      break;
    default:
      break;
  }
  
  FastLED.show();
}

//////////////////////// CYBER ////////////////////////

#define CYBER_DATA ((fieldData.cyberData))
#define CYBER_COUNTER ((CYBER_DATA[0]))

void cyberInit() {
  CYBER_COUNTER = 0;
  clearScreen();
}

void cyber() {
  if (CYBER_COUNTER == NUM_LEDS_PER_STRIP) {
    delay(random(500)+250);
    for (int j = 0; j < (NUM_STRIPS-1); ++j) {
      for (int i = 0; i < NUM_LEDS_PER_STRIP; ++i) {
        leds[i+(NUM_STRIPS-j-1)*NUM_LEDS_PER_STRIP] = leds[i+(NUM_STRIPS-j-2)*NUM_LEDS_PER_STRIP];
      }
    }
    for (int i = 0; i < NUM_LEDS_PER_STRIP; ++i) {
      leds[i] = CRGB::Black;
    }
    CYBER_COUNTER = 0;
    delay(random(500)+250);
  } else {
    leds[CYBER_COUNTER] = primaryColor;
    leds[CYBER_COUNTER].fadeToBlackBy(random(255));
    CYBER_COUNTER += 1;
  }
  FastLED.show();
  delay(random(50)+25);
}

//////////////////////// AUDIO SPECTRUM VISUALIZER ////////////////////////



//////////////////////// MIDI PLAYER  ////////////////////////







