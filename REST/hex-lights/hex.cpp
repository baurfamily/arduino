#include "hex.h"

Hex::Hex(hex leds)
{
  //this should unwind itself into the structs
  this.hex = &leds;
  setColor( CRGB::White );  
}

void Hex::setColor(CRGB color)
{
  for (int i=0; i<HEX_LEDS_COUNT; i++)
  {
    this.hex[i] = color;  
  }
}

CRGB* Hex::state()
{
  return this.hex;
}
