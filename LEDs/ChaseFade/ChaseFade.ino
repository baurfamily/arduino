#include <FastLED.h>
#define LED_COUNT 576

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
  
  if (pos == LED_COUNT-1 || pos == 0) {
    forward = !forward;
  }

  for (int i=0; i<LED_COUNT; i++) {
    CRGB color = leds[i];
    uint8_t dist = abs8(pos - i);
    if (dist < 25) {
      color.fadeLightBy(dist*dist);  
    } else {
      color = CRGB::Black;
    }
    
    leds[i] = color;
  }
  
  leds[pos] = CHSV(hue++, 255, 255);

  FastLED.show();
  delay(10);
}
