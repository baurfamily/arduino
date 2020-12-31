
#define VCC_PIN 13

#include <M1359.h>

M1359Strip strip(VCC_PIN);

void setup() {
  strip.reset();
  strip.on();
  strip.setColor(M1359_BLUE);
  Serial.begin(115200);
  Serial.println("Setup complete");
}

void loop() {
  loopCycle();
}

void loopColorFade() {
  static M1359Color to = random(8);

  M1359Color from = to;
  to = random(8);

  strip.fade(from, to);
}

void loopReset() {
  for(int i=0; i<100; i++) {
    strip.reset();
  }

  delay(1000);
}

void loopCycle() {
  static int color = M1359_BLUE;
  strip.setBrightness(0);
  strip.setColor((++color)%8);
  strip.display(1000);

  // strip.on();
  // delay(1000);
}

void loopFade() {
  static int brightness = 0;
  static int color = 1;
  static int increment = 1;

  brightness += increment;
  
  if (brightness == 0 || brightness == 255) {
    increment = -increment;
  }

  if (brightness == 0) {
    color = (color+1)%8;
    strip.setColor( color );
    Serial.print("Setting color: ");
    Serial.println(color);
   }

  strip.setBrightness(brightness);
  strip.display();
}
