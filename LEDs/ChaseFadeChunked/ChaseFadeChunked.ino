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
  
  // set new position
  hue++;

  if (forward) { pos++; } else { pos--; }
//  
//  if (pos == REPEAT_BOUNDRY-1 || pos == 0) {
//    forward = !forward;
//  }
  pos = pos % REPEAT_BOUNDRY;

  int chunks = LED_COUNT / REPEAT_BOUNDRY;

  for (int i=0; i<REPEAT_BOUNDRY; i++) {
    CRGB color = leds[i];
    uint8_t dist = abs8(pos - i);
    if (dist < REPEAT_BOUNDRY) {
      color.fadeLightBy(dist);  
    } else {
      color = CRGB::Black;
    }

    for (int j=0; j<chunks; j++) {
      leds[i + j*REPEAT_BOUNDRY] = color;
    }
  }

  for (int j=0; j<chunks; j++) {
    leds[pos + j*REPEAT_BOUNDRY] = CHSV(hue++, 255, 255);
  }

  FastLED.show();
  delay(1000);
}
