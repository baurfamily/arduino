#include <FastLED.h>
#define LED_COUNT 100

CRGB leds[LED_COUNT];

void setup() {
  FastLED.addLeds<NEOPIXEL, 6>(leds,  LED_COUNT);
}

void loop() {
  static uint8_t hue = 0;
  static uint8_t pos = 0;

  // set last positin to off
  leds[pos] = CRGB::Black;

  // set new position
  hue++;
  pos = (pos+1) % LED_COUNT;
  leds[pos] = CHSV(hue++, 255, 255);

  FastLED.show();
  delay(10);
}
