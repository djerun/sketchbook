

unsigned char lastValue = 49;
volatile unsigned char value = lastValue;
volatile boolean rotating = false;
volatile boolean a = false;
volatile boolean b = false;

void i2() {
  if (rotating) delay(1);
  
  if (digitalRead(2) != b) {
    b = !b;
    
    if (b && !a) {
      --value;
      
      if (value < 13) value = 13;
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
      
      if (value > 88) value = 88;
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
  pinMode(4, INPUT_PULLUP);
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
  
  if (digitalRead(4)) {
    noTone(5);
  } else {
    tone(5, freq(value));
  }
}

