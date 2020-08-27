#include "FastLED.h"

#define LED_TYPE WS2812B
#define COLOR_ORDER GRB

#define NUM_LEDS (((1+8+12+16+24+32)))

unsigned int leds_per_ring(unsigned int n) {
  switch (n) {
    case 0: return 1;
    case 1: return 8;
    case 2: return 12;
    case 3: return 16;
    case 4: return 24;
    case 5: return 32;
    default: return 0;
  }
}

unsigned int ring_index_offset(unsigned int n) {
  switch (n) {
    case 0: return 0;
    case 1: return 1;
    case 2: return 1+8;
    case 3: return 1+8+12;
    case 4: return 1+8+12+16;
    case 5: return 1+8+12+16+24;
    default: return 0;
  }
}

unsigned char Brightness = 0xff;
unsigned char MaxSaturation = 0xbf;

CRGB Leds[NUM_LEDS];

typedef union t_FieldData {
  unsigned char starfireData[NUM_LEDS]; // temperature for all cells
} 
FieldData;

FieldData fieldData;

#define DEFAULT_SPEED 0xfff
int Speed = 0xff;

float speedFactor() {
  return ((float)Speed) / ((float)DEFAULT_SPEED);
}

unsigned char Special = 0;

#define NUM_MODES 4
int Mode = 1;

void (*Modes[])(void) = {
  &hueWalk, // 0
  &sineValueWalk, // 1
  &sineHueSaturationWalk, // 2
  &sineHueValueWalk // 3
};

void (*ModeInits[])(void) = {
  &hueWalkInit, // 0
  &sineValueWalkInit, // 1
  &sineHueSaturationWalkInit, // 2
  &sineHueValueWalkInit // 3
};

unsigned char PrimaryHue = 0;

CRGB PrimaryColor = CHSV(PrimaryHue, 255, 255);

void increaseBrightness() {
  Brightness = min(0xff, Brightness+0x10);
  Serial.print(Brightness, HEX);
}

void decreaseBrightness() {
  Brightness = max(0x0f, Brightness-0x10);
  Serial.print(Brightness, HEX);
}

void nextMode() {
  Mode = (Mode+1)%NUM_MODES;
  Serial.println(Mode, HEX);
  (*ModeInits[Mode])();
}

void previousMode() {
  Mode = (Mode+NUM_MODES-1)%NUM_MODES;
  Serial.println(Mode, HEX);
  (*ModeInits[Mode])();
}

void nextColor() {
  PrimaryHue += 0x10;
  Serial.println(PrimaryHue, HEX);
  PrimaryColor = CHSV(PrimaryHue, 255, 255);
}

void previousColor() {
  PrimaryHue -= 0x10;
  Serial.println(PrimaryHue, HEX);
  PrimaryColor = CHSV(PrimaryHue, 255, 255);
}

void increaseSaturation() {
  MaxSaturation = min(0xff, MaxSaturation + 0x10);
  Serial.println(MaxSaturation, HEX);
}

void decreaseSaturation() {
  MaxSaturation = max(0x0f, MaxSaturation - 0x10);
}

void speedUp() {
  Speed = min(0xff, Speed + 0x10);
  Serial.println(Speed, HEX);
}

void speedDown() {
  Speed = max(0x0f, Speed - 0x10);
  Serial.println(Speed, HEX);
}

void specialPlus() {
  Special = min(0xff, Special + 1);
  Serial.println(Special, HEX);
}

void specialMinus() {
  Special = max(0, Special - 1);
  Serial.println(Special, HEX);
}

void handleSerial() {
  if (Serial.available()) {
    byte cmd = Serial.read();
    Serial.print(cmd, HEX);
    switch (cmd) {
    case '1':
      nextMode();
      break;
    case '2':
      previousMode();
      break;
    case '3':
      nextColor();
      break;
    case '4':
      previousColor();
      break;
    case '5':
      increaseSaturation();
      break;
    case '6':
      decreaseSaturation();
      break;
    case '7':
      increaseBrightness();
      break;
    case '8':
      decreaseBrightness();    
      break;
    case '9':
      speedUp();
      break;
    case '0':
      speedDown();
      break;
    case '-':
      specialPlus();
      break;
    case '=':
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
  Mode = random(NUM_MODES);
  PrimaryHue = random(16)*0x10;

  PrimaryColor = CHSV(PrimaryHue, 255, 255);
  (*ModeInits[Mode])();
}

int ButtonStates[] = { 1, 1, 1, 1 };

void handleControls() {
  MaxSaturation = analogRead(0) >> 2;
  PrimaryHue = analogRead(1) >> 2;
  Speed = analogRead(2) >> 2;
  Brightness = analogRead(3) >> 2;
  
  int val = digitalRead(12);
  if (val != ButtonStates[0]) {
    ButtonStates[0] = val;
    if (val == 0) {
      previousMode();
    }
  }
  val = digitalRead(10);
  if (val != ButtonStates[1]) {
    ButtonStates[1] = val;
    if (val == 0) {
      specialMinus();
    }
  }
  val = digitalRead( 8);
  if (val != ButtonStates[2]) {
    ButtonStates[2] = val;
    if (val == 0) {
      specialPlus();
    }
  }
  val = digitalRead( 6);
  if (val != ButtonStates[3]) {
    ButtonStates[3] = val;
    if (val == 0) {
      nextMode();
    }
  }
}

void setup() {
  Serial.begin(9600);

  pinMode(12, INPUT_PULLUP);
  pinMode(10, INPUT_PULLUP);
  pinMode( 8, INPUT_PULLUP);
  pinMode( 6, INPUT_PULLUP);
  
/*FastLED.addLeds<LED_TYPE,  2, COLOR_ORDER>(Leds,            0, 1+8+12+16+24+32); //all  
  FastLED.addLeds<LED_TYPE, 12, COLOR_ORDER>(Leds,            0, 1); //ring 0
  FastLED.addLeds<LED_TYPE, 11, COLOR_ORDER>(Leds,            1, 8); //ring 1
  FastLED.addLeds<LED_TYPE, 10, COLOR_ORDER>(Leds,          1+8, 12); //ring 2
  FastLED.addLeds<LED_TYPE,  9, COLOR_ORDER>(Leds,       1+8+12, 16); //ring 3
  FastLED.addLeds<LED_TYPE,  8, COLOR_ORDER>(Leds,    1+8+12+16, 24); //ring 4
  FastLED.addLeds<LED_TYPE,  7, COLOR_ORDER>(Leds, 1+8+12+16+24, 32); //ring 5*/
  FastLED.addLeds<LED_TYPE,  2, COLOR_ORDER>(Leds, 1+8+12+16+24, 32);

  FastLED.setBrightness(Brightness);

  randomSeed(analogRead(0) ^ analogRead(1) ^ analogRead(2) ^ analogRead(3) ^ analogRead(4) ^ analogRead(5));

  randomizeMode();
}

void loop() {
  handleControls();
  handleSerial();
  advanceTime();
  (*Modes[Mode])();
}

//////////////////////// UTILITY ////////////////////////

void fillScreen(CRGB color) {
  for (int i = 0; i < NUM_LEDS; ++i) {
    Leds[i] = color;
  }
  FastLED.show();
}

void clearScreen() {
  fillScreen(CRGB::Black);
}

float Time = 0.0f;
unsigned long MillisLast = 0;

float advanceTime() {
  unsigned long millisNow = millis();
  Time += ((float)(millisNow - MillisLast)) / 1000.0f * speedFactor();
  MillisLast = millisNow;
}

float time() {
  return Time;
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

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void hueWalkInit() {
  clearScreen();
}

void hueWalk() {
  for (unsigned int ring = 1; ring < 6; ++ring) {
    unsigned int num_leds = leds_per_ring(ring);
    for (unsigned int i = 0; i < num_leds; ++i) {
      unsigned int offset = ring_index_offset(ring);
      float h = ((float)i) / ((float)(num_leds));
      h += frac(time());
      h *= 255.0f;
      unsigned char hue = (unsigned char)h;
      unsigned char saturation = MaxSaturation;
      if (i % (Special+1) == 0) {
      Leds[offset+i] = CHSV(hue, saturation, 255);
      Leds[offset+i] %= Brightness;
      } else {
        Leds[offset+i] = CRGB::Black;
      }
    }
  }
  FastLED.show();
}

void sineValueWalkInit() {
  clearScreen();
}

void sineValueWalk() {
  for (unsigned int ring = 1; ring < 6; ++ring) {
    unsigned int num_leds = leds_per_ring(ring);
    for (unsigned int i = 0; i < num_leds; ++i) {
      unsigned int offset = ring_index_offset(ring);
      
      float v = ((float)i) / ((float)(num_leds));
      v += frac(time());
      v *= 2.0f * PI;
      v = sin(((float)Special+1.0f) * v);
      v = abs(v);
      v = pow(v, 10.0f);
      v *= (float)Brightness;

      unsigned char hue = (unsigned char)PrimaryHue;
      unsigned char saturation = MaxSaturation;
      unsigned char value = (unsigned char)v;
      Leds[offset+i] = CHSV(hue, saturation, 255);
      Leds[offset+i] %= value;
    }
  }
  FastLED.show();
}

void sineHueSaturationWalkInit() {
  clearScreen();
}

void sineHueSaturationWalk() {
  for (unsigned int ring = 1; ring < 6; ++ring) {
    unsigned int num_leds = leds_per_ring(ring);
    for (unsigned int i = 0; i < num_leds; ++i) {
      unsigned int offset = ring_index_offset(ring);
      unsigned char h1 = PrimaryHue;
      unsigned char h2 = (h1+128)%256;
      
      float s = ((float)i) / ((float)(num_leds));
      s += frac(time());
      s *= 2.0f * PI;
      s = sin(((float)Special+1.0f) * s);

      float h = h1;
      if (s < 0.0f) h = h2;

      s = abs(s) * ((float)MaxSaturation);

      unsigned char hue = (unsigned char)h;
      unsigned char saturation = (unsigned char)s;
      Leds[offset+i] = CHSV(hue, saturation, 255);
      Leds[offset+i] %= Brightness;
    }
  }
  FastLED.show();
}

void sineHueValueWalkInit() {
  clearScreen();
}

void sineHueValueWalk() {
  for (unsigned int ring = 1; ring < 6; ++ring) {
    unsigned int num_leds = leds_per_ring(ring);
    for (unsigned int i = 0; i < num_leds; ++i) {
      unsigned int offset = ring_index_offset(ring);
      unsigned char h1 = PrimaryHue;
      unsigned char h2 = (h1+128)%256;
      
      float v = ((float)i) / ((float)(num_leds));
      v += frac(time());
      v *= 2.0f * PI;
      v = sin(((float)Special+1.0f) * v);

      float h = h1;
      if (v < 0.0f) h = h2;

      v = abs(v);
      v = pow(v, 10.0f);
      v *= (float)Brightness;
      
      unsigned char hue = (unsigned char)h;
      unsigned char saturation = MaxSaturation;
      unsigned char value = (unsigned char)v;
      Leds[offset+i] = CHSV(hue, saturation, 255);
      Leds[offset+i] %= value;
    }
  }
  FastLED.show();
}
