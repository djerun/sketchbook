// IMPORTANT: Adafruit_TFTLCD LIBRARY MUST BE SPECIFICALLY
// CONFIGURED FOR EITHER THE TFT SHIELD OR THE BREAKOUT BOARD.
// SEE RELEVANT COMMENTS IN Adafruit_TFTLCD.h FOR SETUP.

#include <stdint.h>
#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_TFTLCD.h> // Hardware-specific library
#include <TouchScreen.h>

#if defined(__SAM3X8E__)
    #undef __FlashStringHelper::F(string_literal)
    #define F(string_literal) string_literal
#endif

// The control pins for the LCD can be assigned to any digital or
// analog pins...but we'll use the analog pins as this allows us to
// double up the pins with the touch screen (see the TFT paint example).
#define LCD_CS A3 // Chip Select goes to Analog 3
#define LCD_CD A2 // Command/Data goes to Analog 2
#define LCD_WR A1 // LCD Write goes to Analog 1
#define LCD_RD A0 // LCD Read goes to Analog 0

#define LCD_RESET A4 // Can alternately just connect to Arduino's reset pin

// When using the BREAKOUT BOARD only, use these 8 data lines to the LCD:
// For the Arduino Uno, Duemilanove, Diecimila, etc.:
//   D0 connects to digital pin 8  (Notice these are
//   D1 connects to digital pin 9   NOT in order!)
//   D2 connects to digital pin 2
//   D3 connects to digital pin 3
//   D4 connects to digital pin 4
//   D5 connects to digital pin 5
//   D6 connects to digital pin 6
//   D7 connects to digital pin 7
// For the Arduino Mega, use digital pins 22 through 29
// (on the 2-row header at the end of the board).

// Assign human-readable names to some common 16-bit color values:
#define  BLACK   0x0000
#define BLUE    0x001F
#define RED     0xF800
#define GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF

//Touch
#define MAXPRESSURE 1000
#define MINPRESSURE 10

#define YP A2
#define XM A3
#define YM 8
#define XP 9

#define TS_MINX 150
#define TS_MINY 120
#define TS_MAXX 920
#define TS_MAXY 940

TouchScreen touch = TouchScreen(XP, YP, XM, YM, 300);

typedef struct {
  char label1[8];
  char label2[8];
  char label3[8];
  char id;
} menuItem;

menuItem menu[] = {
  {"       ","       ","       ",  0x40 }
, {"       ","brightr","       ",  0x3f }
, {"       ","       ","       ",  0x45 }
, {"       ","  prev ","       ",  0x20 }
, {"  \\|/  ","-> O <- ","  /|\\  ",  0x5a }
, {"       "," next  ","       ",  0x2f }
, {"       ","       ","       ",  0x4a }
, {"       ","darker ","       ",  0x30 }
, {"       ","       ","       ",  0x4f }
};

Adafruit_TFTLCD tft(LCD_CS, LCD_CD, LCD_WR, LCD_RD, LCD_RESET);
// If using the shield, all control and data lines are fixed, and
// a simpler declaration can optionally be used:
// Adafruit_TFTLCD tft;

void setup(void) {
  tft.reset();

  uint16_t identifier = tft.readID();

  tft.begin(identifier);

tft.fillScreen(BLACK);

  Serial.begin(9600);

  pinMode(13, OUTPUT);
}

bool refresh = true;

void loop(void) {
  if (refresh) {
    drawMenu();
    refresh = false;
  }

  handleTouch();
  
  delay(10);
}

void handleMenu(TSPoint p) {
  int item = ((p.x*3) / tft.width()) + ((p.y*3)/tft.height())*3;
  Serial.println(menu[item].id);
}

void handleTouch() {
  digitalWrite(13, HIGH);
  TSPoint p = touch.getPoint();
  digitalWrite(13, LOW);

  //pinMode(XP, OUTPUT);
  pinMode(XM, OUTPUT);
  pinMode(YP, OUTPUT);
  //pinMode(YM, OUTPUT);

  if (p.z > MINPRESSURE && p.z < MAXPRESSURE) {
    //Serial.print("pressure:");Serial.println(p.z);
    p.x = map(p.x, TS_MINX, TS_MAXX, tft.width(), 0);
    p.y = map(p.y, TS_MINY, TS_MAXY, tft.height(), 0);
    //Serial.print("x:");Serial.print(p.x);Serial.print(",y:");Serial.println(p.y);
    handleMenu(p);
    //tft.fillCircle(p.x, p.y, 10, GREEN);
    //tft.fillScreen(GREEN);
    //delay(2000);
    refresh = true;
  }
}

void drawMenu() {
  tft.setRotation(1);
  //tft.fillScreen(BLACK);
  for (int i = 0; i < 3; ++i) {
    for (int j = 0; j < 3; ++j) {
      tft.fillRect(
        tft.width()/3*i+5
      , tft.height()/3*j+5
      , tft.width()/3-10
      , tft.height()/3-10
      , YELLOW
      );
      tft.setTextColor(BLUE);
      tft.setTextSize(2);
      tft.setCursor(
        tft.width()/3*i+10
      , tft.height()/3*j+12
      );
      tft.println(menu[i+3*j].label1);
      tft.setCursor(
        tft.width()/3*i+10
      , tft.height()/3*j+32  
      );
      tft.println(menu[i+3*j].label2);
      tft.setCursor(
        tft.width()/3*i+10
      , tft.height()/3*j+52  
      );
      tft.println(menu[i+3*j].label3);
    }
  }
}

