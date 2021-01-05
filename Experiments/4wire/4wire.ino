
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
  loopPurple();
}

void loopCycle() {
  delay(5000);
  Serial.println("0  - 0000 - O/O");
  digitalWrite(10, LOW);
  digitalWrite(11, LOW);
  digitalWrite(12, LOW);
  digitalWrite(13, LOW);
  
  delay(5000);
  Serial.println("1  - 1000 - O/R");
  digitalWrite(10, HIGH);
  digitalWrite(11, LOW);
  digitalWrite(12, LOW);
  digitalWrite(13, LOW);
  
  delay(5000);
  Serial.println("2  - 0100 - O/R");
  digitalWrite(10, LOW);
  digitalWrite(11, HIGH);
  digitalWrite(12, LOW);
  digitalWrite(13, LOW);
  
  delay(5000);
  Serial.println("3  - 1100 - B/R");
  digitalWrite(10, HIGH);
  digitalWrite(11, HIGH);
  digitalWrite(12, LOW);
  digitalWrite(13, LOW);
  
  delay(5000);
  Serial.println("4  - 0010 - O/C");
  digitalWrite(10, LOW);
  digitalWrite(11, LOW);
  digitalWrite(12, HIGH);
  digitalWrite(13, LOW);
  
  delay(5000);
  Serial.println("5  - 1010 - O/G");
  digitalWrite(10, HIGH);
  digitalWrite(11, LOW);
  digitalWrite(12, HIGH);
  digitalWrite(13, LOW);
  
  delay(5000);
  Serial.println("6  - 0110 - O/G");
  digitalWrite(10, LOW);
  digitalWrite(11, HIGH);
  digitalWrite(12, HIGH);
  digitalWrite(13, LOW);
  
  delay(5000);
  Serial.println("7  - 1110 - O/Y");
  digitalWrite(10, HIGH);
  digitalWrite(11, HIGH);
  digitalWrite(12, HIGH);
  digitalWrite(13, LOW);
  
  delay(5000);
  Serial.println("8  - 0001 - Y/O");
  digitalWrite(10, LOW);
  digitalWrite(11, LOW);
  digitalWrite(12, LOW);
  digitalWrite(13, HIGH);
  
  delay(5000);
  Serial.println("9  - 1001 - G/O");
  digitalWrite(10, HIGH);
  digitalWrite(11, LOW);
  digitalWrite(12, LOW);
  digitalWrite(13, HIGH);
  
  delay(5000);
  Serial.println("10 - 0101 - G/O");
  digitalWrite(10, LOW);
  digitalWrite(11, HIGH);
  digitalWrite(12, LOW);
  digitalWrite(13, HIGH);
  
  delay(5000);
  Serial.println("11 - 1101 - C/O");
  digitalWrite(10, HIGH);
  digitalWrite(11, HIGH);
  digitalWrite(12, LOW);
  digitalWrite(13, HIGH);
  
  delay(5000);
  Serial.println("12 - 0011 - R/B");
  digitalWrite(10, LOW);
  digitalWrite(11, LOW);
  digitalWrite(12, HIGH);
  digitalWrite(13, HIGH);
  
  delay(5000);
  Serial.println("13 - 1011 - R/O");
  digitalWrite(10, HIGH);
  digitalWrite(11, LOW);
  digitalWrite(12, HIGH);
  digitalWrite(13, HIGH);
  
  delay(5000);
  Serial.println("14 - 0111 - R/O");
  digitalWrite(10, LOW);
  digitalWrite(11, HIGH);
  digitalWrite(12, HIGH);
  digitalWrite(13, HIGH);
  
  delay(5000);
  Serial.println("15 - 1111 - O/O");
  digitalWrite(10, HIGH);
  digitalWrite(11, HIGH);
  digitalWrite(12, HIGH);
  digitalWrite(13, HIGH);
} 

void loopYellow() {
  digitalWrite(10, HIGH);
  digitalWrite(11, HIGH);
  digitalWrite(12, HIGH);
  digitalWrite(13, LOW);
  delay(10);

  digitalWrite(10, LOW);
  digitalWrite(11, LOW);
  digitalWrite(12, LOW);
  digitalWrite(13, HIGH);
  delay(10);
}

void loopPurple() {
  
  digitalWrite(10, LOW);
  digitalWrite(11, LOW);
  digitalWrite(12, HIGH);
  digitalWrite(13, HIGH);
  delay(10);
  
  digitalWrite(10, HIGH);
  digitalWrite(11, HIGH);
  digitalWrite(12, LOW);
  digitalWrite(13, LOW);
  delay(10);
}
