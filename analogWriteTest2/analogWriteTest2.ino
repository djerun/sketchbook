

void setup() {
  pinMode(3, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  if (Serial.available() > 0) {
    if (Serial.read()) {
      analogWrite(3, 127);
    } else {
      analogWrite(3, 0);
    }
  }
}


