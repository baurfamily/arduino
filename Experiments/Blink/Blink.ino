
// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(115200);

}

void pulseStrip(int count, int muSec) {
  for (int i=0; i<count; i++) {
    digitalWrite(LED_BUILTIN, HIGH);
    delayMicroseconds(muSec);
    digitalWrite(LED_BUILTIN, LOW);
    delayMicroseconds(muSec);
  }
  digitalWrite(LED_BUILTIN, HIGH);
}

void resetStrip() {
  digitalWrite(LED_BUILTIN, LOW);
  delay(1);
  digitalWrite(LED_BUILTIN, HIGH);
  pulseStrip(1,10);
}

// the loop function runs over and over again forever
void loop() {
  static int pulseMuSec = 5;
  
//  pulseMuSec += 5;

  Serial.print(F("Loop muSec: "));
  Serial.println(pulseMuSec);

  pulseStrip(1, pulseMuSec);

  // wait a second...
  delay(1000);
  resetStrip();
  
}
