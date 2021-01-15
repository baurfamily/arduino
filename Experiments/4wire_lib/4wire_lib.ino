#include <FairyLights.h>
#include <ThreeWireStrip.h>

#define BYPASS_PIN 13
#define CENTER_PIN 12
#define RED_PIN 11
#define BLUE_PIN 10

//pin pins[4] = { BYPASS_PIN, CENTER_PIN, RED_PIN, BLUE_PIN };
ThreeWireStrip *strip = new ThreeWireStrip( CENTER_PIN, RED_PIN, BLUE_PIN );

void setup() {
  strip->setColor(Magenta);
  
  Serial.begin(115200);
  Serial.println("Setup complete");
}

void loop() {
  static bool odd = false;
  static byte brightness = 255;
  strip->setBrightness(brightness);
    
  strip->display(1000);
  
  odd = !odd;
  if (odd)
    strip->nextColorOdd();
  else
    strip->nextColorEven();
}
