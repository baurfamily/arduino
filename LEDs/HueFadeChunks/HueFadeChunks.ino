#include <FastLED.h>
#define LED_COUNT 576

#define DATA_PIN 13

// "one" color, slowly change together
//#define REPEAT_BOUNDRY 48
//#define INCREMENT 1
//#define SPEED 50
//#define HEX_DIFF 2

// continuous chain
#define REPEAT_BOUNDRY 1
#define INCREMENT 1
#define SPEED 100
#define HEX_DIFF 1


// rainbow, slowly change
//#define REPEAT_BOUNDRY 48
//#define INCREMENT 1
//#define SPEED 100
//#define HEX_DIFF 21

// two color, slowly change
//#define REPEAT_BOUNDRY 48
//#define INCREMENT 1
//#define SPEED 20
//#define HEX_DIFF 128

// four color, slowly change
//#define REPEAT_BOUNDRY 48
//#define INCREMENT 1
//#define SPEED 20
//#define HEX_DIFF 64


CRGB leds[LED_COUNT];

void setup() {
  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds,  LED_COUNT);
}

void loop() {
  static uint8_t hue = 0;
  static uint16_t pos = 0;
  static bool forward = true;

  
  hue += INCREMENT;

  int chunks = LED_COUNT / REPEAT_BOUNDRY;
  for (int i=0; i<REPEAT_BOUNDRY; i++) {
    for (int j=0; j<chunks; j++) {
      leds[i + j*REPEAT_BOUNDRY] = CHSV(hue + j*HEX_DIFF, 255, 255);
    }
  }

  FastLED.show();
  delay(SPEED);
}
