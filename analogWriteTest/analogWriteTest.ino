#include <Wire.h>

void setup() {
  Wire.begin();
  pinMode(3, OUTPUT);
}

void loop() {
  Wire.beginTransmission(8);
  Wire.write(1);
  Wire.endTransmission();
  analogWrite(3, 127);
  delay(100);
  Wire.beginTransmission(8);
  Wire.write(0);
  Wire.endTransmission();
  analogWrite(3, 0);
  delay(900);
}


