#include <FairyLights.h>

#define VCC_PIN 6

pin pins[1] = { VCC_PIN };
FairyLights *strip = FairyLightsFactory( TwoWire, pins);

void setup() {
  strip->setColor(Blue);
}

void loop() {
  static int brightness = 0;
  static bool up = true;

  static int color = 1;

  if (up) { brightness++; } else { brightness--; }
  if (brightness == 0 || brightness == 255) { up = !up; }
  if (brightness == 0) {
    color = (color+1)%8;
    strip->setColor( color );
   }

  strip->setBrightness(brightness);
  strip->display();
}
