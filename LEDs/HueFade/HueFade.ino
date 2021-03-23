#include <FastLED.h>

#define LED_COUNT 576

CRGB leds[LED_COUNT];

void setup() {
  FastLED.addLeds<NEOPIXEL, 6>(leds,  LED_COUNT); 
}

void loop() { 
  static uint8_t hue = 0;
  FastLED.showColor(CHSV(hue++, 255, 255)); 
  delay(100);
}
