#include <FastLED.h>
#define LED_COUNT 576

CRGB leds[LED_COUNT];

void setup() {
  FastLED.addLeds<NEOPIXEL, 6>(leds,  LED_COUNT);
  //Serial.begin(115200);
}

void loop() {
  static int len = 0;
  static int hue = 0;
  static bool forward = true;
  
  if (forward) {
    len++;
  } else {
    len--;
  }
  //Serial.println(len);

  if (len == 10){
    forward = false;
    hue = (hue + 32)%256;
  } else if (len == 0) {
    forward = true;
    hue = (hue + 32)%256;
  }
  for(int i = 0; i<29; i++) {
    int center = 20*i+10;
    int pos = center-len;
    
    if (forward) {
      for(int j = 0; j<len*2+1; j++){
        leds[pos + j] = CHSV(hue, 255, 255);
      }
    } else {
      int start = center-10;
      for (int j = start; j<pos; j++) {
        leds[j] = CHSV(hue, 255, 255);
      }
      int endCap = center + 10;
      int endPos = center+len;
      for (int j = endPos; j<endCap; j++) {
        leds[j] = CHSV(hue, 255, 255);
      }
    }
  }


    FastLED.show();
    delay(100);
}
