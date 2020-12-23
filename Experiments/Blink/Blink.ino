// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
  Serial.begin(115200);

}

void pulseOnce(int muSec) {
  Serial.println(" - pulse");
  delayMicroseconds(muSec);
  digitalWrite(LED_BUILTIN, LOW);
  delayMicroseconds(muSec);
  digitalWrite(LED_BUILTIN, HIGH);
}

void pulseStrip(int count, int muSec) {
  Serial.print(F("pulseStrip( "));
  Serial.print(count);
  Serial.print(F(", "));
  Serial.print(muSec);
  Serial.println(" )");

  for (int i=0; i<count; i++) {
    pulseOnce(muSec);
  }
}

void resetStrip(int muSec) {
  Serial.println("Resetting");
  pulseOnce(10000);
  pulseOnce(muSec);
}

// the loop function runs over and over again forever
void loop() {
  static int pulseMuSec = 100;
  static int pulses = 2;
    digitalWrite(LED_BUILTIN, HIGH);

//  pulseMuSec += 5;

  pulseStrip(2, 10);
  pulseStrip(2, 100);

  // wait a second...
  delay(5000);
  
  resetStrip(pulseMuSec);
  delay(1000);
  
}
