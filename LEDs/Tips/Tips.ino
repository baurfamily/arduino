#include <FastLED.h>
#define LED_COUNT 100

CRGB leds[LED_COUNT];

void setup() {
  FastLED.addLeds<NEOPIXEL, 6>(leds,  LED_COUNT);
}

void loop() {
  static int len = 0;
  len = (len + 1)%11;
  leds[7] = CRGB(255, 0, 0);
  leds[8] = CRGB(255, 0, 0);
  leds[9] = CRGB(255, 0, 0);
  leds[10] = CRGB(255, 0, 0);
  leds[11] = CRGB(255, 0, 0);
  leds[12] = CRGB(255, 0, 0);
  
  for(int i = 0; i<5; i++) {
    int pos = 20*i+10-len;
    for(int j = 0; j<len*2; j++){
      leds[pos + j] = CRGB(255, 0, 0);
    }
  }


    FastLED.show();
      delay(100);
}
