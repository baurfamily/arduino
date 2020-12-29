#include <FastLED.h>

CRGB leds[100];

void setup() {
  FastLED.addLeds<NEOPIXEL, 6>(leds,  100);
}

void loop() { 
  static uint8_t hue = 0;
  FastLED.showColor(CHSV(hue++, 100, 100)); 
  delay(10);
}
