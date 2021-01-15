
#define BYPASS 13
#define CENTER 12
#define RED 11
#define BLUE 10

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
  Serial.println("Reds");
  setRed1();
  delay(2000);
  setRed2();
  delay(2000);
  
  Serial.println("Greens");
  setGreen1();
  delay(2000);
  setGreen2();
  delay(2000);

  Serial.println("Blues");
  setBlue1();
  delay(2000);
  setBlue2();
  delay(2000);
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

void setBlue1() { hgh(BLUE); off(RED); low(CENTER); off(BYPASS); }
void setBlue2() { low(BLUE); off(RED); hgh(CENTER); off(BYPASS); }

void setRed1() { off(BLUE); low(RED); hgh(CENTER); off(BYPASS); }
void setRed2() { off(BLUE); hgh(RED); low(CENTER); off(BYPASS); }

void setGreen1() { hgh(BLUE); low(RED); off(CENTER); off(BYPASS); }
void setGreen2() { low(BLUE); hgh(RED); off(CENTER); off(BYPASS); }


//Secondary
void setCyan1() { hgh(BLUE); low(RED); off(CENTER); low(BYPASS); }
void setCyan2() { low(BLUE); hgh(RED); off(CENTER); hgh(BYPASS); }

void setRedBlue() { hgh(BLUE); hgh(RED); low(CENTER); off(BYPASS); }
void setBlueRed() { low(BLUE); low(RED); hgh(CENTER); off(BYPASS); }


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
