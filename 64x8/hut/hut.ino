// MultipleStripsInOneArray - see https://github.com/FastLED/FastLED/wiki/Multiple-Controller-Examples for more info on
// using multiple controllers.  In this example, we're going to set up four NEOPIXEL strips on three
// different pins, each strip will be referring to a different part of the single led array

#define FIX_MATH_NO_OVERFLOW

#include <ESP8266WiFi.h>

#include "FastLED.h"

#include "font.h"

#define LED_TYPE WS2812B
#define COLOR_ORDER GRB

#define NUM_ROWS 8
#define NUM_COLS 64
#define NUM_LEDS (((NUM_ROWS * NUM_COLS)))

#define NUM_SNAKES 32

#define WIFI_SSID (("pixelhut"))
#define WIFI_PASSWORD (("pixelhut"))
#define WIFI_PORT 1337

boolean WIFI_ACTIVE = false;

CRGB led[NUM_LEDS];

byte BRIGHTNESS = 0xf;

typedef union t_FieldData {
  boolean gameOfLifeData[NUM_LEDS*2]; // alive flags for all cells, frontbuffer+backbuffer
  unsigned char fireData[NUM_LEDS]; // temperature for all cells, frontbuffer+backbuffer
  //  unsigned char audioSpectrumData[NUM_COLS]; //averaged amplitude per frequency band
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

void enableWIFI(void);
void disableWIFI(void);

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
void cyber(void);
void cyberInit(void);
void pixelflut(void);
void pixelflutInit(void);
//void audioSpectrum(void);
//void audioSpectrumInit(void);
//void midiPlayer();
//void midiPlaterInit();

#define NUM_MODES 14
int mode = 11;

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
  &cyber, // 12
  &pixelflut // 13
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
  &cyberInit, // 12
  &pixelflutInit // 13
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

WiFiServer CONTROL_SERVER(7331);

WiFiClient CONTROL_CLIENT;

void handleCommandClient() {
  if (CONTROL_CLIENT.connected()) {
    if (CONTROL_CLIENT.available()) {
      Serial.println("client available");
      String line = CONTROL_CLIENT.readStringUntil('\n');
      Serial.print("got line: \"");
      Serial.print(line);
      Serial.println("\"");
      for (unsigned int i = 0; i < line.length(); ++i) {
        byte cmd = line.charAt(i);
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
      }
      CONTROL_CLIENT.flush();
    }
  } else {
    Serial.println("client disconnected");
    CONTROL_CLIENT.stop();
  }
}

void handleCommandServer() {
  if (CONTROL_CLIENT) {
    handleCommandClient();
  } else {
    CONTROL_CLIENT = CONTROL_SERVER.available();
    if (CONTROL_CLIENT) {
      handleCommandClient();
    }
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
  // 12 cyber
  int modeTable[] = {
     0,  0,  0,  0,
     1,  1,  1,  1,
     2,  2,  3,  4,
     9,  9, 10, 10,
    11, 11, 11, 11,
    12, 12, 12, 12
  };
  int r = random(24);
  Serial.println(r);
  mode = modeTable[r];
  color = random(NUM_COLORS);
  Serial.println(color);
  primaryColor = primaryColors[color];
  (*modeInits[mode])();
}

void setup() {
  Serial.begin(115200);
  Serial.println("##### EPIC HAT IS EPIC #####");
  
  FastLED.addLeds<LED_TYPE,  2, COLOR_ORDER>(led, 0, NUM_LEDS);
  FastLED.setBrightness(BRIGHTNESS);

  enableWIFI();
  CONTROL_SERVER.begin();

  randomSeed(millis());
  randomizeMode();
}

void loop() {
  handleSerial();
  handleCommandServer();
  (*modes[mode])();
}

//////////////////////// UTILITY ////////////////////////

unsigned int led_index(int row, int col) {
  if ((col % 2) == 0) {
    return (col % NUM_COLS) * NUM_ROWS + NUM_ROWS - 1 - (row % NUM_ROWS);
  } else {
    return (col % NUM_COLS) * NUM_ROWS + (row % NUM_ROWS);
  }
}

void fillScreen(CRGB color) {
  for (int i = 0; i < NUM_LEDS; ++i) {
    led[i] = color;
  }
  FastLED.show();
}

void clearScreen() {
  fillScreen(CRGB::Black);
}

void dimScreenByAmount(byte amount) {
  for (int i = 0; i < NUM_LEDS; ++i) {
    led[i].r = max(0, (int)led[i].r-amount);
    led[i].g = max(0, (int)led[i].g-amount);
    led[i].b = max(0, (int)led[i].b-amount);
  }
}

void dimScreenByFactor(float factor) {
  for (int i = 0; i < NUM_LEDS; ++i) {
    led[i].r *= factor;
    led[i].g *= factor;
    led[i].b *= factor;
    led[i].r = (led[i].r >= 0xf) ? led[i].r : 0;
    led[i].g = (led[i].g >= 0xf) ? led[i].g : 0;
    led[i].b = (led[i].b >= 0xf) ? led[i].b : 0;
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

void enableWIFI() {
  if (!WIFI_ACTIVE) {
    WiFi.softAP(WIFI_SSID, WIFI_PASSWORD);
    Serial.println(WiFi.softAPIP());
    WIFI_ACTIVE = true;
  }
}

void disableWIFI() {
  if (WIFI_ACTIVE) {
    WiFi.softAPdisconnect();
    WIFI_ACTIVE = false;
  }
}

int glyphPos(char c) {
  // "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ-=!@#$%^&*()_+[]{};'\\:\"|,./<>?"
  switch (c) {
    case '0': return 0;
    case '1': return 1;
    case '2': return 2;
    case '3': return 3;
    case '4': return 4;
    case '5': return 5;
    case '6': return 6;
    case '7': return 7;
    case '8': return 8;
    case '9': return 9;
    case 'A': return 10;
    case 'B': return 11;
    case 'C': return 12;
    case 'D': return 13;
    case 'E': return 14;
    case 'F': return 15;
    case 'G': return 16;
    case 'H': return 17;
    case 'I': return 18;
    case 'J': return 19;
    case 'K': return 20;
    case 'L': return 21;
    case 'M': return 22;
    case 'N': return 23;
    case 'O': return 24;
    case 'P': return 25;
    case 'Q': return 26;
    case 'R': return 27;
    case 'S': return 28;
    case 'T': return 29;
    case 'U': return 30;
    case 'V': return 31;
    case 'W': return 32;
    case 'X': return 33;
    case 'Y': return 34;
    case 'Z': return 35;
    case '-': return 36;
    case '=': return 37;
    case '!': return 38;
    case '@': return 39;
    case '#': return 40;
    case '$': return 41;
    case '%': return 42;
    case '^': return 43;
    case '&': return 44;
    case '*': return 45;
    case '(': return 46;
    case ')': return 47;
    case '_': return 48;
    case '+': return 49;
    case '[': return 50;
    case ']': return 51;
    case '{': return 52;
    case '}': return 53;
    case ';': return 54;
    case '\'': return 55;
    case '\\': return 56;
    case ':': return 57;
    case '"': return 58;
    case '|': return 59;
    case ',': return 60;
    case '.': return 61;
    case '/': return 62;
    case '<': return 63;
    case '>': return 64;
    case '?': return 65;
    default: return -1;
  }
}

void displayText(String s) {
  Serial.println("displayText");
  unsigned int offset = 0;
  for (int i = 0; i < s.length(); ++i) {
    Serial.print("offset: ");
    Serial.println(offset);
    Serial.print("i: ");
    Serial.println(i);
    char glyph = s.charAt(i);
    int gp = glyphPos(glyph);
    if (gp >= 0) {
      for (unsigned int row = 0; row < 7; ++row) {
        for (unsigned int col = 0; col < 5; ++col) {
          char v = FONT[gp][row][col];
          if ('#' == v) {
            led[led_index(NUM_ROWS-1-row, col+offset)] = primaryColor;
          } else {
            led[led_index(NUM_ROWS-1-row, col+offset)] = CRGB::Black;
          }
        }
      }
    }
    offset += 6;
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

    led[led_index(y, x)] = ((((unsigned long)r)<<16) + (((unsigned long)g)<<8) + ((unsigned long)b));
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
    SNAKE_DATA[i*6+3] = random(NUM_COLS); //X
    SNAKE_DATA[i*6+4] = random(NUM_ROWS); //Y
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
      x = (x+NUM_COLS-1)%NUM_COLS;
      if (led[led_index(y, x)] == SNAKE_BLACK)
        SNAKE_DATA[i*6+3] = x;
     break;
    case 1:
      if (y == 0) break;
      y = (y+NUM_ROWS-1)%NUM_ROWS;
      if (led[led_index(y, x)] == SNAKE_BLACK)
        SNAKE_DATA[i*6+4] = y;
      break;
    case 2:
      x = (x+1)%NUM_COLS;
      if (led[led_index(y, x)] == SNAKE_BLACK)
        SNAKE_DATA[i*6+3] = x;
      break;
    case 3:
      if (y == NUM_ROWS-1) break;
      y = (y+1)%NUM_ROWS;
      if (led[led_index(y, x)] == SNAKE_BLACK)
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

#define GOL_DATA ((fieldData.gameOfLifeData))
#define GOL_RESET_CYCLE_COUNT 10
#define GOL_LOW_CHANGE_COUNT 32

#define GOL_DIE_WHEN_FEWER 2
#define GOL_DIE_WHEN_ABOVE 3
#define GOL_BIRTH_WHEN_EQUAL 3

unsigned int cycleCount = 0;
boolean firstIsFrontbuffer = true;

boolean isAlive(unsigned int x, unsigned int y) {
  unsigned int offset = (firstIsFrontbuffer) ? 0 : NUM_LEDS;
  return GOL_DATA[(y*NUM_COLS)+x+offset];
}

boolean isAliveNextTurn(unsigned int x, unsigned int y) {
  int neighborAliveCount = 0;

  neighborAliveCount += (isAlive(((x+NUM_COLS-1)%NUM_COLS),(y+NUM_ROWS-1)%NUM_ROWS)) ? 1 : 0;
  neighborAliveCount += (isAlive(((x+NUM_COLS-1)%NUM_COLS),(y+NUM_ROWS+0)%NUM_ROWS)) ? 1 : 0;
  neighborAliveCount += (isAlive(((x+NUM_COLS-1)%NUM_COLS),(y+NUM_ROWS+1)%NUM_ROWS)) ? 1 : 0;
  neighborAliveCount += (isAlive(((x+NUM_COLS+0)%NUM_COLS),(y+NUM_ROWS-1)%NUM_ROWS)) ? 1 : 0;
  neighborAliveCount += (isAlive(((x+NUM_COLS+0)%NUM_COLS),(y+NUM_ROWS+1)%NUM_ROWS)) ? 1 : 0;
  neighborAliveCount += (isAlive(((x+NUM_COLS+1)%NUM_COLS),(y+NUM_ROWS-1)%NUM_ROWS)) ? 1 : 0;
  neighborAliveCount += (isAlive(((x+NUM_COLS+1)%NUM_COLS),(y+NUM_ROWS+0)%NUM_ROWS)) ? 1 : 0;
  neighborAliveCount += (isAlive(((x+NUM_COLS+1)%NUM_COLS),(y+NUM_ROWS+1)%NUM_ROWS)) ? 1 : 0;

  return (
      isAlive(x, y) && 
      (neighborAliveCount >= GOL_DIE_WHEN_FEWER) && 
      (neighborAliveCount <= GOL_DIE_WHEN_ABOVE)
    ) || (
      neighborAliveCount == GOL_BIRTH_WHEN_EQUAL
    );
}

void setGOL(unsigned int x, unsigned int y, boolean v) {
    unsigned int offset = (firstIsFrontbuffer) ? NUM_LEDS : 0;
    GOL_DATA[(y*NUM_COLS)+x+offset] = v;
}

void gameOfLifeInit() {
  FastLED.setBrightness(BRIGHTNESS);
  for (int i = 0; i < (NUM_LEDS*2); ++i) {
    GOL_DATA[i] = random(2) == 0;
  }

  cycleCount = 0;
  clearScreen();
}

void gameOfLife() {
  unsigned int changeCount = 0;
  for (unsigned int i = 0; i < NUM_COLS; ++i) {
    for (unsigned int j = 0; j < NUM_ROWS; ++j) {
      boolean now = isAlive(i, j);
      boolean next = isAliveNextTurn(i, j);
      led[led_index(j, i)] = (now) ? primaryColor : CRGB::Black;
      setGOL(i, j, next);
      if (now != next) {
        ++changeCount;
      }
    }
  }
  firstIsFrontbuffer = !firstIsFrontbuffer;
  FastLED.show();
  delay(1000);
  if (changeCount < GOL_LOW_CHANGE_COUNT) {
    ++cycleCount;
    if (cycleCount >= GOL_RESET_CYCLE_COUNT) {
      gameOfLifeInit();
    }
  } else {
    cycleCount = 0;
  }
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
  for (int i = 0; i < NUM_COLS; ++i) {
    fireDataSet(led_index(0, i), random(160) + 80);
  }
  
  // Add some hotspots:
  // bright sparkts at the bottom with 255
  for (int i = 0; i < 9; i++) {
    fireDataSet(led_index(0, random(NUM_COLS)), 255);
  }
  
  // move up the old fire, lower its temperature
  // since the resolution is so low no smoothing is done
  for (int j = 0; j < NUM_ROWS-1; ++j) {
    for (int i = 0; i < NUM_COLS; ++i) {
      unsigned char value = fireDataGet(led_index(j, i));
      value = (value >= 32) ? value - 32 : 0;
      fireDataSet(led_index(j+1, i), value);
    }
  }
  
  for (int i = 0; i < NUM_LEDS; ++i) {
    led[i] = (*fireColor[color%NUM_FIRE_COLORS])(fireDataGet(i));
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

  for(int i = 0; i < NUM_COLS; ++i) {
    for (int j = 0; j < NUM_ROWS; j++) {
      led[led_index(j, i)] = CHSV((state+(i*255/NUM_COLS))%255, 255, 255);
    }
  }
  FastLED.show();
}

//////////////////////// SPINNER ////////////////////////

unsigned int SPINNER_COUNTER = 0;

void spinnerInit() {
  FastLED.setBrightness(BRIGHTNESS);
  SPINNER_COUNTER = 0;
  clearScreen();
}

void spinner() {
  dimScreenByAmount(0x1f);

  for (int j = 0; j < NUM_ROWS; ++j) {
    led[led_index(j, SPINNER_COUNTER)] = primaryColor;
    led[led_index(j, (SPINNER_COUNTER+NUM_COLS/2) % NUM_COLS)] = primaryColor;
  }

  SPINNER_COUNTER = (SPINNER_COUNTER+1) % NUM_COLS;

  delay(10);

  FastLED.show();
}

//////////////////////// PARTY ////////////////////////

void partyInit(void) {
  FastLED.setBrightness(BRIGHTNESS);
  for (int i = 0; i < NUM_LEDS; ++i) {
    led[i] = CHSV(random(256), 255, 255);
  }
}

void party(void) {
  for (int i = 0; i < 36; ++i) {
    led[random(NUM_LEDS)] = CHSV(random(256), 255, 255);
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
    led[i] = primaryColor;
    led[i].fadeLightBy(((char)(f*255.0)));
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

  for (int i = 0; i < NUM_ROWS; ++i) {
    float y = ((float)i)/((float)NUM_COLS);
    float e = min(1.0, abs(y - t) / SPREAD);

    for (int j = 0; j < NUM_COLS; ++j) {
      led[led_index(i, j)] = primaryColor;
      led[led_index(i, j)].fadeLightBy(((char)(e*255.0)));
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

  for (int i = 0; i < NUM_COLS; ++i) {
    float x = ((float)i) / ((float)(NUM_COLS-1));

    for (int j = 0; j < NUM_ROWS; ++j) {
      float y = ((float)j) / ((float)(NUM_ROWS-1));

      float t = abs(y * 2.0 - 1.0) ;//* abs(x * 2.0 - 1.0);
      float ts = min(1.0f, max(0.0f, t + s));

      led[led_index(j, i)] = primaryColor;
      led[led_index(j, i)].fadeLightBy(((char)(ts*255.0)));
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
    led[i] = CRGB(r, g, b);
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

  for(int i = 0; i < NUM_COLS; ++i) {
    for (int j = 0; j < NUM_ROWS; j++) {
      led[led_index(j, i)] = CHSV(((state+(j*255/NUM_COLS)))%255, 255, 255);
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

  for (int j = 0; j < NUM_ROWS; ++j) {
    float tim = ti + ((float)j/(float)NUM_ROWS)*0.35;
    
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
    
    for (int i = 0; i < NUM_COLS; ++ i) {
      led[led_index(j, i)] = CHSV(rowHue, 255, 255);
    }
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
  if (CYBER_COUNTER == NUM_COLS) {
    delay(random(500)+250);
    for (int j = 0; j < (NUM_ROWS-1); ++j) {
      for (int i = 0; i < NUM_COLS; ++i) {
        led[led_index(NUM_ROWS-j-1, i)] = led[led_index(NUM_ROWS-j-2, i)];
      }
    }
    for (int i = 0; i < NUM_COLS; ++i) {
      led[led_index(0, i)] = CRGB::Black;
    }
    CYBER_COUNTER = 0;
    delay(random(500)+250);
  } else {
    led[led_index(0, CYBER_COUNTER)] = primaryColor;
    led[led_index(0, CYBER_COUNTER)].fadeToBlackBy(random(255));
    CYBER_COUNTER += 1;
  }
  FastLED.show();
  delay(random(50)+25);
}

//////////////////////// PIXELFLUT ////////////////////////

WiFiServer PIXELFLUT_SERVER(1337);

void handleCommand(String command, WiFiClient client) {
  if (command.length() > 0) {
    if (command.equals("HELP")) {
      client.write("Minimalistic Implementation of Pixelflut\nSupports a single command per connection, does not support color output, does not support alpha channel.");
      return;
    }
    if (command.equals("SIZE")) {
      client.write("SIZE 64 8\n");
      return;
    }
    int x, y, r, g, b;
    int count = sscanf(command.c_str(), "PX %u %u %2x%2x%2x", &x, &y, &r, &g, &b);
    Serial.println(count);
    Serial.println(x);
    Serial.println(y);
    Serial.println(r);
    Serial.println(g);
    Serial.println(b);
    if ((count == 5)
    && (x >= 0)
    && (x < NUM_COLS)
    && (y >= 0)
    && (y < NUM_ROWS)
    && (r >= 0)
    && (r < 256)
    && (g >= 0)
    && (g < 256)
    && (b >= 0)
    && (b < 256)
    ) {
      led[led_index(y, x)] = CRGB(r, g, b);
    }
  }
}

void pixelflutInit() {
  Serial.println("Starting Server");
  PIXELFLUT_SERVER.begin();
  Serial.println("Server up");
  clearScreen();
  displayText(String("FOOBAR"));
  FastLED.show();
}

void pixelflut() {
  WiFiClient client = PIXELFLUT_SERVER.available();
  if (!client) {
    return;
  }

  Serial.println("new client");
  while (!client.available()) delay(1);

  String line = client.readStringUntil('\n');
  Serial.println(line);
  handleCommand(line, client);
  client.flush();
  client.stop();
  FastLED.show();
}

//////////////////////// AUDIO SPECTRUM VISUALIZER ////////////////////////



//////////////////////// MIDI PLAYER  ////////////////////////
