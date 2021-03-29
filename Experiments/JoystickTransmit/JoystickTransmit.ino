#include "LedControl.h"
#define joyX A0
#define joyY A1
 
int xMap, yMap, xValue, yValue;
LedControl lc=LedControl(12,11,10,1);
 
void setup() {
  Serial.begin(115200);
 
  lc.shutdown(0,false);
  /* Set the brightness to a medium values */
  lc.setIntensity(0,8);
  /* and clear the display */
  lc.clearDisplay(0);
}
 
void loop() {
  // put your main code here, to run repeatedly:
  xValue = analogRead(joyX);
  yValue = analogRead(joyY);
  xMap = map(xValue, 0,1023, 0, 7);
  yMap = map(yValue,0,1023,7,0);
  lc.setLed(0,xMap,yMap,true);
}
