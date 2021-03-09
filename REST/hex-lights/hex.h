#ifndef HEX_H
#define HEX_H

#define HEX_LED_SIDE_COUNT 48
#define HEX_LED_TOTAL_COUNT 48

#include <FastLED.h>

class Hex
{
  public:
    Hex();

    void setColor(CRGB color);

    CRGB* state();

  private:
    int _ledsPerSide;
    CRGB _leds[HEX_LED_TOTAL_COUNT ];
    
};

#endif
