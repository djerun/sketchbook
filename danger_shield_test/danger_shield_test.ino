
#define LED1 6
#define LED2 5

#define FADER3_LED 9
#define FADER2_LED 10
#define FADER1_LED 11

#define FADER3 A0
#define FADER2 A1
#define FADER1 A2

#define BUZZER 3

#define P 9

void setup() {
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(A0, INPUT);
  pinMode(FADER1_LED, OUTPUT);
  pinMode(FADER2_LED, OUTPUT);
  pinMode(FADER3_LED, OUTPUT);
}

void loop() {
  int v = analogRead(FADER2);

  analogWrite(LED1, analogRead(FADER1)/4);
  analogWrite(LED2, analogRead(FADER3)/4);
  digitalWrite(FADER1_LED, 255);
  digitalWrite(FADER2_LED, 255);
  digitalWrite(FADER3_LED, 255);
  
  if (v > 0) {
    delay(v);
    
    analogWrite(LED1, 0);
    analogWrite(LED2, 0);
    digitalWrite(FADER1_LED, 0);
    digitalWrite(FADER2_LED, 0);
    digitalWrite(FADER3_LED, 0);

   
    delay(v);
  }
}
