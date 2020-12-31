// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
}

void pulseOnce(int muSec) {
  delayMicroseconds(muSec);
  digitalWrite(LED_BUILTIN, LOW);
  analogWrite(9, random(120,130));
//  analogWrite(9, 0);
  delayMicroseconds(muSec);
  digitalWrite(LED_BUILTIN, HIGH);
  analogWrite(9, 255);
}

void pulseStrip(int count, int muSec) {
  for (int i=0; i<count; i++) {
    pulseOnce(muSec);
  }
  analogWrite(9, 255);
}

void resetStrip(int muSec) {
//  Serial.println("Resetting");
  pulseOnce(20000);
  pulseOnce(muSec);
}

// the loop function runs over and over again forever
void loop() {
  static int pulseMuSec = 20;

  analogWrite(9, 0);
  digitalWrite(LED_BUILTIN, HIGH);

  for( int i=0; i<50; i++) {
//    pulseStrip(random(0,7), random(2000,15000));
    pulseStrip(random(0,7), 2);
    delayMicroseconds(random(0,125));
    delay(random(2,20));
  }
  delay(1000);
  
//  for( int i=0; i<50; i++) {
//    resetStrip(pulseMuSec);
//    delay(random(20,100));
//  }
  
  digitalWrite(LED_BUILTIN, HIGH);

  // wait a couple seconds...
  delay(5000);

  digitalWrite(LED_BUILTIN, LOW);
  delay(1000);
}
