#define PACKET_SIZE 128


void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);
  
  digitalWrite(LED_BUILTIN, LOW);
  Serial.begin(115200);
}

void pulseOnce(int muSec) {
  delayMicroseconds(muSec);
  digitalWrite(LED_BUILTIN, LOW);
  delayMicroseconds(muSec);
  digitalWrite(LED_BUILTIN, HIGH);
}

void pulseStrip(int pattern[]) {
  #ifdef DEBUG
    Serial.print("pulseStrip: ");
  #endif
  
  digitalWrite(LED_BUILTIN, HIGH);
  for (int i=0; i<PACKET_SIZE; i++) {
    #ifdef DEBUG
      if (i%8 == 0) { Serial.print("_"); }
      Serial.print(pattern[i]);
    #endif

//    delayMicroseconds(4);
    if (pattern[i] == 1) {
      digitalWrite(LED_BUILTIN, LOW);
    }
    digitalWrite(LED_BUILTIN, HIGH);
  }
  digitalWrite(LED_BUILTIN, HIGH);
  #ifdef DEBUG
    Serial.println("");
  #endif
}

void loop() {
  digitalWrite(LED_BUILTIN, HIGH);


  int pattern[PACKET_SIZE] = { 1, 0, 0, 0, 0, 0, 0, 0,
                               0, 0, 0, 0, 0, 0, 0, 0,
                               0, 0, 0, 0, 0, 0, 0, 0,
                               0, 0, 0, 0, 0, 0, 0, 0,
                               0, 0, 0, 0, 0, 0, 0, 0,
                               0, 0, 0, 0, 0, 0, 0, 0,
                               0, 0, 0, 0, 0, 0, 0, 0,
                               0, 0, 0, 0, 0, 0, 0, 0,
                               0, 0, 0, 0, 0, 0, 0, 0,
                               0, 0, 0, 0, 0, 0, 0, 0,
                               0, 0, 0, 0, 0, 0, 0, 0,
                               0, 0, 0, 0, 0, 0, 0, 0,
                               0, 0, 0, 0, 0, 0, 0, 0,
                               0, 0, 0, 0, 0, 0, 0, 0,
                               0, 0, 0, 0, 0, 0, 0, 0,
                               0, 0, 0, 0, 0, 0, 0, 0 };
                               

  for(int i; i<50; i++) {
    for(int j=0; j<PACKET_SIZE; j++) {
      pattern[j] = random(2);
    }
    pulseStrip(pattern);
    delayMicroseconds(10);
    delay(1000);
  }

  pulseStrip(pattern);
  delay(1000);
}
