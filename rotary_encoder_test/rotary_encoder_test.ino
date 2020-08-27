

unsigned int lastValue = 49;
unsigned int lastValue2 = 440;
volatile unsigned int value = lastValue;
volatile unsigned int value2 = lastValue2;
volatile boolean rotating = false;
volatile boolean a = false;
volatile boolean b = false;

unsigned int value2_modifier(unsigned int v) {
  if ((v / 16) > 1) {
    return v / 16;
  } else {
    return 1;
  }
}

void i2() {
  if (rotating) delay(1);
  
  if (digitalRead(2) != b) {
    b = !b;
    
    if (b && !a) {
      --value;
      value2 -= value2_modifier(value2);
      
      if (value < 13) value = 13;
      if (value2 < 50) value2 = 50;
    }
    rotating = false;
  }
}

void i3() {
  if (rotating) delay(1);
  
  if (digitalRead(3) != a) {
    a = !a;
    
    if (a && !b) {
      ++value;
      value2 += value2_modifier(value2);
      
      if (value > 88) value = 88;
      if (value2 > 20000) value2 = 20000;
    }
    rotating = false;
  }
  
}

float freq(int n) {
  return pow(2.0f, float(n - 49) / 12.0f) * 440.0f;
}

void setup() {
  pinMode(2, INPUT_PULLUP);
  pinMode(3, INPUT_PULLUP);
  pinMode(6, INPUT_PULLUP);
  pinMode(7, INPUT_PULLUP);
  attachInterrupt(0, i2, CHANGE);
  attachInterrupt(1, i3, CHANGE);
  Serial.begin(9600);
}

void loop() {
  rotating = true;
  digitalWrite(13, digitalRead(6));
  
  if (lastValue != value) {
    Serial.print(value, DEC);
    Serial.print(": ");
    Serial.println(freq(value));
    lastValue = value;
  }

  if (lastValue2 != value2) {
    Serial.println(value2);
    lastValue2 = value2;
  }

  int r6 = digitalRead(6);
  int r7 = digitalRead(7);
  if (r6 && r7) {
    noTone(11);
  } else {
    if (!r6) {
      tone(11, freq(value));
    } else {
      tone(11, value2);
    }
  }
}
