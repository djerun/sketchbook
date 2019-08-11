

#define B0 31
#define C1 33
#define c1 35
#define D1 37
#define d1 39
#define E1 41
#define F1 44
#define f1 46
#define G1 49
#define g1 52
#define A1 55
#define a1 58
#define B1 62
#define C2 65
#define c2 69
#define D2 73
#define d2 78
#define E2 82
#define F2 87
#define f2 93
#define G2 98
#define g2 104
#define A2 110
#define a2 117
#define B2 123
#define C3 131
#define c3 139
#define D3 147
#define d3 156
#define E3 165
#define F3 175
#define f3 185
#define G3 196
#define g3 208
#define A3 220
#define a3 233
#define B3 247
#define C4 262
#define c4 277
#define D4 294
#define d4 311
#define E4 330
#define F4 349
#define f4 370
#define G4 392
#define g4 415
#define A4 440
#define a4 466
#define B4 494
#define C5 523
#define c5 554
#define D5 587
#define d5 622
#define E5 659
#define F5 698
#define f5 740
#define G5 784
#define g5 831
#define A5 880
#define a5 932
#define B5 988
#define C6 1047
#define c6 1109
#define D6 1175
#define d6 1245
#define E6 1319
#define F6 1397
#define f6 1480
#define G6 1568
#define g6 1661
#define A6 1760
#define a6 1865
#define B6 1976
#define C7 2093
#define c7 2217
#define D7 2349
#define d7 2489
#define E7 2637
#define F7 2794
#define f7 2960
#define G7 3136
#define g7 3322
#define A7 3520
#define a7 3729
#define B7 3951
#define C8 4186
#define c8 4435
#define D8 4699
#define d8 4978

#define NUM_CHANNELS 4

bool ChannelActive[NUM_CHANNELS];
bool ChannelState[NUM_CHANNELS];
float ChannelWavelength[NUM_CHANNELS];
float ChannelTimer[NUM_CHANNELS];

struct MidiInfo {
  int fileInfo;
  int numChannels;
  int deltaTimeQuarter;
};

bool readMidiHeader() {
  if ((Serial.read() == 'M') && (Serial.read() == 'T') && (Serial.read() == 'h') && (Serial.read() == 'd')
  && (Serial.read() == 0x0) && (Serial.read() == 0x0) && (Serial.read() == 0x0) && (Serial.read() == 0x6)
  ) {
    
  } else {
    return false;
  }
}

void setup() {
  // put your setup code here, to run once:
  for (int i = 0; i < NUM_CHANNELS; ++i) {
    ChannelActive[i] = false;
    ChannelState[i] = false;
    ChannelWavelength[i] = 0.0f;
    ChannelTimer[i] = 0.0f;
  }
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:

}
