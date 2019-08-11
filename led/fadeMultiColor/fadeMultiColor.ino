/*
 Fade

 This example shows how to fade an LED on pin 9
 using the analogWrite() function.

 The analogWrite() function uses PWM, so if
 you want to change the pin you're using, be
 sure to use another PWM capable pin. On most
 Arduino, the PWM pins are identified with 
 a "~" sign, like ~3, ~5, ~6, ~9, ~10 and ~11.

 This example code is in the public domain.
 */

int g = 9;           // the PWM pin the LED is attached to
int ga = 0;    // how bright the LED is
int gfa = 5;    // how many points to fade the LED by

int r = 8;
int ra = 0;
int rfa = 3;

int b = 10;
int ba = 0;
int bfa = 1;

// the setup routine runs once when you press reset:
void setup() {
  // declare pin 9 to be an output:
  pinMode(g, OUTPUT);
  pinMode(r, OUTPUT);
  pinMode(b, OUTPUT);
}

// the loop routine runs over and over again forever:
void loop() {
  // set the brightness of pin 9:
  analogWrite(g, ga);
  analogWrite(r, ra);
  analogWrite(b, ba);

  // change the brightness for next time through the loop:
  ga += gfa;
  ra += rfa;
  ba += bfa;

  // reverse the direction of the fading at the ends of the fade:
  if (ga <= 0 || ga >= 255) {
    gfa = -gfa;
  }
  if (ra <= 0 || ra >= 255) {
    rfa = -rfa;
  }
  if (ba <= 0 || ba >= 255) {
    bfa = -bfa;
  }
  
  // wait for 30 milliseconds to see the dimming effect
  delay(30);
}


