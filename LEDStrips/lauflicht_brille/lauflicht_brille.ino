#include "FastLED.h"

boolean KEEP_AMBIENT = true;
boolean COLOR_CYCLE = true;

#define LED_TYPE WS2812B
#define COLOR_ORDER GRB

// multiple strips of LEDs are interpreted to be assembled as a single long strip
#define NUM_STRIPS 1
#define NUM_LEDS_PER_STRIP 9
#define NUM_LEDS (((NUM_STRIPS * NUM_LEDS_PER_STRIP)))


CRGB leds[NUM_LEDS];

byte BRIGHTNESS = 0xff;

#define DEFAULT_SPEED (0x3f)
int speed = 0x1f;

float speedFactor() {
  return ((float)speed) / ((float)DEFAULT_SPEED);
}

int mode = 2;

void fade();
void fadeInit();
void pulse();
void pulseInit();
void pulseCircle();
void pulseCircleInit();
void breathe();
void breatheInit();
void wobble();
void wobbleInit();
void knightRider();
void knightRiderInit();
void rainbow();
void rainbowInit();

void randomize();

#define NUM_MODES 7

void (*modes[])(void) = {
  &fade,
  &pulse,
  &breathe,
  &wobble,
  &knightRider,
  &pulseCircle,
  &rainbow
};

void (*modeInits[])(void) = {
  &fadeInit,
  &pulseInit,
  &breatheInit,
  &wobbleInit,
  &knightRiderInit,
  &pulseCircleInit,
  &rainbowInit
};

CRGB primaryColors[] = {
  CRGB::Red,
  CRGB::OrangeRed,
  CRGB::Yellow,
  CRGB::Green,
  CRGB::Cyan,
  CRGB::Blue,
  CRGB::Purple,
  CRGB::Crimson
};

int color = 1;

#define NUM_COLORS 8

CRGB primaryColor = primaryColors[color];

void increaseBrightness() {
  BRIGHTNESS = min(0xff, BRIGHTNESS+0x10);
  Serial.println(BRIGHTNESS, HEX);
  FastLED.setBrightness(BRIGHTNESS);
}

void decreaseBrightness() {
  BRIGHTNESS = max(0x0f, BRIGHTNESS-0x10);
  Serial.println(BRIGHTNESS, HEX);
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
        randomize();
        break;
      default:
        break;
    }
  }
}

#define NUM_BUTTONS 1

int buttonPins[NUM_BUTTONS] {
  /*12, 11, 10, 9, 8, 7, 6, 5,*/ 4
};

int buttonStates[NUM_BUTTONS] {
  /*LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW,*/ LOW
};

void (*buttonFunctions[NUM_BUTTONS])(void) = {
  /*&increaseBrightness,
  &decreaseBrightness,
  &nextMode,
  &previousMode,
  &nextColor,
  &previousColor,
  &speedUp,
  &speedDown,*/
  &randomize
};

void handleButtons() {
  /*for (int i = 0; i < NUM_BUTTONS; ++i) {
    boolean buttonState = digitalRead(buttonPins[i]);
    
    if (buttonState && buttonState != buttonStates[i]) {
      buttonFunctions[i]();
    }
    
    buttonStates[i] = buttonState;
  }*/
}

void randomize() {
  int modeTable[] = {
    1, 2, 3, 4, 4, 2, 1, 4, 4
  };
  
  mode = modeTable[random(9)];
  color = random(NUM_COLORS);
  mode = 6;
  primaryColor = primaryColors[color];
  (*modeInits[mode])();
}

void setup() {
  randomSeed(analogRead(0) ^ analogRead(1) ^ analogRead(2) ^ analogRead(3) ^ analogRead(4) ^ analogRead(5));
  Serial.begin(9600);
  
  FastLED.addLeds<LED_TYPE, 12, COLOR_ORDER>(leds, 0 * NUM_LEDS_PER_STRIP, NUM_LEDS_PER_STRIP);
  FastLED.addLeds<LED_TYPE, 11, COLOR_ORDER>(leds, 0 * NUM_LEDS_PER_STRIP, NUM_LEDS_PER_STRIP);
  FastLED.addLeds<LED_TYPE, 10, COLOR_ORDER>(leds, 0 * NUM_LEDS_PER_STRIP, NUM_LEDS_PER_STRIP);
  FastLED.addLeds<LED_TYPE,  9, COLOR_ORDER>(leds, 0 * NUM_LEDS_PER_STRIP, NUM_LEDS_PER_STRIP);
  FastLED.addLeds<LED_TYPE,  8, COLOR_ORDER>(leds, 0 * NUM_LEDS_PER_STRIP, NUM_LEDS_PER_STRIP);
  FastLED.addLeds<LED_TYPE,  7, COLOR_ORDER>(leds, 0 * NUM_LEDS_PER_STRIP, NUM_LEDS_PER_STRIP);
  FastLED.addLeds<LED_TYPE,  6, COLOR_ORDER>(leds, 0 * NUM_LEDS_PER_STRIP, NUM_LEDS_PER_STRIP);
  FastLED.addLeds<LED_TYPE,  5, COLOR_ORDER>(leds, 0 * NUM_LEDS_PER_STRIP, NUM_LEDS_PER_STRIP);
  FastLED.addLeds<LED_TYPE,  4, COLOR_ORDER>(leds, 0 * NUM_LEDS_PER_STRIP, NUM_LEDS_PER_STRIP);
  FastLED.addLeds<LED_TYPE,  3, COLOR_ORDER>(leds, 0 * NUM_LEDS_PER_STRIP, NUM_LEDS_PER_STRIP);
  FastLED.addLeds<LED_TYPE,  2, COLOR_ORDER>(leds, 0 * NUM_LEDS_PER_STRIP, NUM_LEDS_PER_STRIP);
  FastLED.setBrightness(BRIGHTNESS);

  for (int i = 0; i < NUM_BUTTONS; ++i) {
    pinMode(buttonPins[i], INPUT);
  }

  randomize();
}

void loop() {
  handleSerial();
  handleButtons();
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
        case 0: r = v, g = t, b = p; break;
        case 1: r = q, g = v, b = p; break;
        case 2: r = p, g = v, b = t; break;
        case 3: r = p, g = q, b = v; break;
        case 4: r = t, g = p, b = v; break;
        case 5: r = v, g = p, b = q; break;
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

//////////////////////// FADE ////////////////////////

void fadeInit() {
  clearScreen();
}

void fade() {
  float f = sin(time())*0.4 + 0.6;
  
  char r = (char)(f*(float)primaryColor.r);
  char g = (char)(f*(float)primaryColor.g);
  char b = (char)(f*(float)primaryColor.b);
  
  for (int i = 0; i < NUM_LEDS; ++i) {
    leds[i] = CRGB(r, g, b);
  }
  
  FastLED.show();
}

//////////////////////// PULSE ////////////////////////

#define PULSE_SPREAD 0.5

void pulseInit() {
  clearScreen();
}

void pulse() {
  float f = 0.25;

  if (KEEP_AMBIENT) {
    f *= (1.0 + sin(time()*20.0)*0.05);
    f *= (1.0 + sin(time()* 9.0)*0.05);
  }

  for (int i = 0; i < NUM_LEDS; ++i) {
    float y = ((float)i)/((float)NUM_LEDS);
    float t = ftime() + 0.5;
    float e = 1.0 - min(1.0, abs(y - t) / PULSE_SPREAD);

    if (KEEP_AMBIENT) {
        e = max(f, e);
    }

    char r = (char)(e*(float)primaryColor.r);
    char g = (char)(e*(float)primaryColor.g);
    char b = (char)(e*(float)primaryColor.b);
      
    leds[i] = CRGB(r, g, b);
  }

  FastLED.show();
}

//////////////////////// PULSE_CIRCLE ////////////////////////

#define PULSE_CIRCLE_SPREAD 0.3

void pulseCircleInit() {
  clearScreen();
}

void pulseCircle() {
  for (int i = 0; i < NUM_LEDS; ++i) {
    float y = ((float)i)/((float)NUM_LEDS);
    float t0 = min(1.0, y       - frac(time()) / PULSE_CIRCLE_SPREAD);
    float t1 = min(1.0, y + 1.0 - frac(time()) / PULSE_CIRCLE_SPREAD);
    float t2 = min(1.0, y - 1.0 - frac(time()) / PULSE_CIRCLE_SPREAD);
    float t = min(min(t0, t1), t2);
    
    leds[i] = primaryColor;
    leds[i].fadeLightBy(((char)(t*255.0)));
  }

  FastLED.show();
}

//////////////////////// BREATHE ////////////////////////

void breatheInit() {
  clearScreen();
}

void breathe() {
  for (int i = 0; i < NUM_LEDS; ++i) {
    float y = ((float)i) / ((float)(NUM_LEDS-1));
    float t = 1.0 - abs(y * 2.0 - 1.0);
    float s = sin(time()*2.0) * 0.75 + 0.25;
    float ts = min(1.0, max(0.0, t + s));
    
    char r = (char)((ts)*(float)primaryColor.r);
    char g = (char)((ts)*(float)primaryColor.g);
    char b = (char)((ts)*(float)primaryColor.b);
      
    leds[i] = CRGB(r, g, b);
  }

  FastLED.show();
}


//////////////////////// WOBBLE ////////////////////////

void wobbleInit() {
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

//////////////////////// KNIGHT RIDER ////////////////////////

#define KNIGHTRIDER_SPREAD 0.3

void knightRiderInit() {
  clearScreen();
}

void knightRider() {
  float f = 0.25;
  
  if (KEEP_AMBIENT) {
    f *= (1.0 + sin(time()*20.0)*0.05);
    f *= (1.0 + sin(time()* 9.0)*0.05);
  }

  for (int i = 0; i < NUM_LEDS; ++i) {
    float y = ((float)i)/((float)NUM_LEDS);
    float t = abs(ftime());
    float e = 1.0 - min(1.0, abs(y - t) / KNIGHTRIDER_SPREAD);

    if (KEEP_AMBIENT) {
      e = max(f, e);
    }
    
    char r = (char)(e*(float)primaryColor.r);
    char g = (char)(e*(float)primaryColor.g);
    char b = (char)(e*(float)primaryColor.b);
      
    leds[i] = CRGB(r, g, b);
  }

  FastLED.show();
}

//////////////////////// RAINBOW ////////////////////////

void rainbowInit() {
  clearScreen(); 
}

void rainbow() {
  int state = ((int)(frac(time())*255.0));
  
  for (int i = 0; i < NUM_LEDS; ++i) {
    leds[i] = CHSV((state+(i*255/9 /*NUM_LEDS*/))%256, 255, 255); //with NUM_LEDS == 150 dividing by little more than 64 bugges out on high values of i
  }
  FastLED.show();
}

