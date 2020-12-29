#define PACKET_SIZE 8
#define PACKET_DELAY 20

void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(12, OUTPUT);
  digitalWrite(12, HIGH);
  digitalWrite(LED_BUILTIN, HIGH);
  Serial.begin(115200);
  pulseStrip(1);
}

void pulseOnce(int muSec) {
  delayMicroseconds(muSec);
  digitalWrite(LED_BUILTIN, LOW);
  delayMicroseconds(muSec);
  digitalWrite(LED_BUILTIN, HIGH);
}

void pulseStrip(int count) {
  digitalWrite(LED_BUILTIN, HIGH);
//  Serial.print("pulse:");
  for(int i=0; i<count; i++) {
//    Serial.print(".");
    delayMicroseconds(PACKET_DELAY);
    digitalWrite(LED_BUILTIN, LOW);
    delayMicroseconds(PACKET_DELAY);
    digitalWrite(LED_BUILTIN, HIGH);
  }
//  Serial.println("");
}

void pulseStrip(int pattern[]) {
//  Serial.print("pulseStrip: ");
  
  digitalWrite(LED_BUILTIN, HIGH);
  for (int i=0; i<PACKET_SIZE; i++) {
//    if (i%8 == 0) { Serial.print("_"); }
//    Serial.print(pattern[i]);

    delayMicroseconds(PACKET_DELAY);
    if (pattern[i] == 1) {
      digitalWrite(LED_BUILTIN, LOW);
    }
    digitalWrite(LED_BUILTIN, HIGH);
  }
  digitalWrite(LED_BUILTIN, HIGH);
//  Serial.println("");
}

void loop() {
  digitalWrite(LED_BUILTIN, HIGH);
  static int brightness = 0;
  static bool up = true;

  static int color = 1;

//  static int redFromReset[PACKET_SIZE] = { 1, 1, 0, 0, 0, 0, 0, 0 };
//  static int resetFromRed[PACKET_SIZE] = { 1, 1, 1, 1, 1, 1, 0, 0 };

  if (up) { brightness++; } else { brightness--; }
  if (brightness == 0 || brightness == 255) { up = !up; }
  if (brightness == 0) { color = (color+1)%8; }

  int val = map( brightness, 0, 255, 1000, 15000);
  
  pulseStrip(color);
  delayMicroseconds(val);

  pulseStrip(8-color);
  delayMicroseconds(15000 - val);

}
