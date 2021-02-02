
void setup() {
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(12, OUTPUT);
  pinMode(13, OUTPUT);

  digitalWrite(9, LOW);
  digitalWrite(10, LOW);
  digitalWrite(11, LOW);
  digitalWrite(12, LOW);
  digitalWrite(13, LOW);

  Serial.begin(115200);
  Serial.println("Setup complete");
}

void loop() {
  loopByteCycle();
}


void high(int pin) {
  pinMode(pin, OUTPUT);
  digitalWrite(pin, HIGH); 
}

void low(int pin) {
  pinMode(pin, OUTPUT);
  digitalWrite(pin, LOW);
}

void off(int pin) {
  digitalWrite(pin, LOW);
  pinMode(pin, INPUT);
}


void loopByteCycle() {
  for(int i=0; i<8; i++) {
    Serial.println(i);
    digitalWrite( 9, !(i &  1));
    digitalWrite(10, !(i &  2));
    digitalWrite(11, !(i &  4));
//    digitalWrite(12, !(i &  8));
//    digitalWrite(13, !(i & 16));
    delay(2000);
  }
}
