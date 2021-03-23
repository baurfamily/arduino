#include <FastLED.h>
#define LED_COUNT 576
#define REPEAT_BOUNDRY 48

CRGB leds[LED_COUNT];

void setup() {
  FastLED.addLeds<NEOPIXEL, 6>(leds,  LED_COUNT);
}

void loop() {
  static uint8_t hue = 0;
  static uint16_t pos = 0;
  static bool forward = true;

  
  hue++;

  int chunks = LED_COUNT / REPEAT_BOUNDRY;
  for (int i=0; i<REPEAT_BOUNDRY; i++) {
    for (int j=0; j<chunks; j++) {
      leds[i + j*REPEAT_BOUNDRY] = CHSV(hue + i*10, 255, 255);
    }
  }

  FastLED.show();
  delay(100);
}
