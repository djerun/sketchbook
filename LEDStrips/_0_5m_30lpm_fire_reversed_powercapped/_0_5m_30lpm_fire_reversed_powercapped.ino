
#include "FastLED.h"

#define LED_TYPE WS2812B
#define COLOR_ORDER GRB

#define NUM_STRIPS 1
#define NUM_LEDS_PER_STRIP 15
#define NUM_LEDS (((NUM_STRIPS * NUM_LEDS_PER_STRIP)))

unsigned char BRIGHTNESS = 0xff;

CRGB leds[NUM_LEDS];

typedef union t_FieldData {
  unsigned char fireData[2*NUM_LEDS]; // temperature for all cells, frontbuffer+backbuffer
} 
FieldData;

FieldData fieldData;

#define DEFAULT_SPEED 0x3f
int speed = DEFAULT_SPEED;

float speedFactor() {
  return ((float)speed) / ((float)DEFAULT_SPEED);
}

void fire(void);
void fireInit(void);

#define NUM_MODES 1
int mode = 0;

void (*modes[])(void) = {
  &fire // 0
};

void (*modeInits[])(void) = {
  &fireInit, // 0
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
  
  FastLED.addLeds<LED_TYPE, 2, COLOR_ORDER>(leds, 0*NUM_LEDS_PER_STRIP, NUM_LEDS_PER_STRIP);
          
  FastLED.setBrightness(BRIGHTNESS);
  FastLED.setMaxPowerInVoltsAndMilliamps(5,500);

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

//////////////////////// FIRE ////////////////////////

#define FIRE__DATA ((fieldData.fireData))

boolean firstBufferIsFrontBuffer = false;

unsigned char fireDataGet(unsigned int index) {
  if (firstBufferIsFrontBuffer) {
    return FIRE__DATA[index];
  } else {
    return FIRE__DATA[NUM_LEDS+index];
  }
}

void fireDataSet(unsigned int index, unsigned char value) {
  if (firstBufferIsFrontBuffer) {
    FIRE__DATA[NUM_LEDS+index] = value;
  } else {
    FIRE__DATA[index] = value;
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
  &fireColorHell, // 0
  &fireColorRegular, // 1
  &fireColorSun, // 2
  &fireColorPoison, // 3
  &fireColorSky, // 4
  &fireColorIce, // 5
  &fireColorSoul, // 6
  &fireColorCrimson // 7
};

void fire() {
  // put some coal on the ground which should be burned:
  // Fill bottom row with  random values between 80 and 239
  for (int i = 0; i < NUM_STRIPS; ++i) {
    fireDataSet(i * NUM_LEDS_PER_STRIP, random(160) + 80);
  }
    
  // move up the old fire, lower its temperature
  // since the resolution is so low no smoothing is done
  for (int j = 0; j < NUM_STRIPS; ++j) {
    for (int i = 0; i < NUM_LEDS_PER_STRIP-1; ++i) {
      unsigned char value = fireDataGet(j*NUM_LEDS_PER_STRIP+i);
      value = (value >= 16) ? value - 16 : 0;
      fireDataSet(j*NUM_LEDS_PER_STRIP+i+1, value);
    }
  }
  
  for (int i = 0; i < NUM_LEDS; ++i) {
    leds[NUM_LEDS-1-i] = (*fireColor[color%NUM_FIRE_COLORS])(fireDataGet(i));
  }
  
  FastLED.show();
  firstBufferIsFrontBuffer = !firstBufferIsFrontBuffer;
  Serial.println("");
  delay(20);
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

  for (int i = 0; i < NUM_LEDS_PER_STRIP; ++i) {
    float y = ((float)i)/((float)NUM_LEDS_PER_STRIP);
    float e = min(1.0, abs(y - t) / SPREAD);

    leds[i] = primaryColor;
    leds[i].fadeLightBy(((char)(e*255.0)));
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
 
    float t = abs(x * 2.0 - 1.0) ;
    float ts = min(1.0, max(0.0, t + s));

    leds[i] = primaryColor;
    leds[i].fadeLightBy(((char)(ts*255.0)));
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

