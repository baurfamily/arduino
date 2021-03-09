#include "hex.h"

Hex::Hex()
{
  setColor( CRGB::White );  
}

void Hex::setColor(CRGB color)
{
  for (int i=0; i<HEX_LED_TOTAL_COUNT; i++)
  {
    _leds[i] = color;  
  }
}

CRGB* Hex::state()
{
  return _leds;
}
