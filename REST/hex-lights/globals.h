#define HEX_COUNT 1
#define LEDS_PER_HEX 48

#include <FastLED.h>

WebServer server(80);

CRGB leds[HEX_COUNT * LEDS_PER_HEX];
