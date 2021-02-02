#include <FairyLights.h>
#include <ThreeWireStrip.h>

#define BYPASS_PIN 10
#define LEFT_PIN 13
#define CENTER_PIN 12
#define RIGHT_PIN 11

//pin pins[4] = { BYPASS_PIN, CENTER_PIN, RED_PIN, BLUE_PIN };
ThreeWireStrip *strip = new ThreeWireStrip( LEFT_PIN, CENTER_PIN, RIGHT_PIN);

void setup() {
  strip->setColor(Blue);
  
  Serial.begin(115200);
  Serial.println("Setup complete");
}

void loop() {
  static bool odd = false;
  static byte brightness = 255;
  strip->setBrightness(brightness);
    
  strip->display(1000);

//  strip->evenNodes()->nextColor();
//  strip->oddNodes()->nextColor();
  
//  strip->nextColor();
//  odd = !odd;
//  if (odd)
//    strip->oddNodes()->nextColor();
//  else
//    strip->evenNodes()->nextColor();
}
