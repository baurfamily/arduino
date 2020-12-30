#define PACKET_SIZE 8
#define PACKET_DELAY 20

#define VCC_PIN 13

#include "m1359.h"

M1359Strip strip(VCC_PIN);

void setup() {
  // just in case we have to debug
//  Serial.begin(115200);
}

void loop() {
  digitalWrite(VCC_PIN, HIGH);
  static int brightness = 0;
  static bool up = true;

  static int color = 1;


  if (up) { brightness++; } else { brightness--; }
  if (brightness == 0 || brightness == 255) { up = !up; }
  if (brightness == 0) { color = (color+1)%8; }

  int val = map( brightness, 0, 255, 1000, 15000);
  
  strip.pulseStrip(color);
  delayMicroseconds(val);

  strip.pulseStrip(8-color);
  delayMicroseconds(15000 - val);

}
