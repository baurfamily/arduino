
#define VCC_PIN 13

#include "m1359.h"

M1359Strip strip(VCC_PIN);

void setup() {
  strip.setColor(M1359_BLUE);
}

void loop() {
  static int brightness = 0;
  static bool up = true;

  static int color = 1;

  if (up) { brightness++; } else { brightness--; }
  if (brightness == 0 || brightness == 255) { up = !up; }
  if (brightness == 0) {
    color = (color+1)%8;
    strip.setColor( color );
   }

  strip.setBrightness(brightness);
  strip.display();
}
