
void setup() {
//  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(12, OUTPUT);
  pinMode(13, OUTPUT);

//  digitalWrite(9, LOW);
  digitalWrite(10, LOW);
  digitalWrite(11, LOW);
  digitalWrite(12, LOW);
  digitalWrite(13, LOW);

  Serial.begin(115200);
  Serial.println("Setup complete");
}

void loop() {
  //cyan
  setBlue1();
  setGreen1();

  //purple
  setRed2();
  setBlue2();
}


void hgh(int pin) {
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

void setRed1() { hgh(10); off(11); off(12); low(13); }
void setRed2() { low(10); off(11); off(12); hgh(13); }

void setBlue1() { off(10); low(11); hgh(12); off(13); }
void setBlue2() { off(10); hgh(11); low(12); off(13); }

void setGreen1() { off(10); off(11); hgh(12); low(13); }
void setGreen2() { off(10); off(11); low(12); hgh(13); }


void loopByteCycle() {
  for(int i=0; i<8; i++) {
    Serial.println(i);
    digitalWrite(10, (i & 1));
    digitalWrite(11, (i & 2));
    digitalWrite(12, (i & 4));
    digitalWrite(13, (i & 8));
    delay(1000);
  }
}
