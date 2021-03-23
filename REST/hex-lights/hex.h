#ifndef HEX_H
#define HEX_H

#include <FastLED.h>

struct side {
  CRGB leds[8];
};

struct hex {
  side sides[8];
};

class Hex
{
  public:
    Hex(hex leds);
    void setColor(CRGB color);

    CRGB* state();

  private:
    int _ledsPerSide;
    hex _hex;
};

struct HexSide {
  Hex *owningHex;
  Hex *neighborHex;
  HexSide *prevSide;
  HexSide *nextSide;
  HexSide *neighborSide;
  CRGB *leds;
};

#endif
