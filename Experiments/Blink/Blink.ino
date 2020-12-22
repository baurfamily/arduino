
// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(115200);

}

void pulse(int count, int muSec) {
  for (i=0; i<count; i++) {
    digitalWrite(LED_BUILTIN, HIGH);
    delayMicroseconds(muSec);
    digitalWrite(LED_BUILTIN, LOW);
    delayMicroseconds(muSec);
  }
}

// the loop function runs over and over again forever
void loop() {
  static int pulseMuSec = 5;
  
  pulseMuSec += 5;

  Serial.print(F("Loop muSec: "));
  Serial.println(pulseMuSec);

  pulse(1, pulseMuSec);

  // wait a second...
  delay(1000);
  
}