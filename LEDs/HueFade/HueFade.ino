#include <FastLED.h>

#define LED_COUNT 300

CRGB leds[LED_COUNT];

void setup() {
  FastLED.addLeds<LPD1886, 6>(leds,  LED_COUNT);
}

void loop() { 
  static uint8_t hue = 0;
  FastLED.showColor(CHSV(hue++, 100, 100)); 
  delay(10);
}
