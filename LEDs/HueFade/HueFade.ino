#include <FastLED.h>

CRGB leds[480];

void setup() {
  FastLED.addLeds<NEOPIXEL, 6>(leds,  48); 
}

void loop() { 
  static uint8_t hue = 0;
  FastLED.showColor(CHSV(hue++, 100, 255)); 
  delay(10);
}
