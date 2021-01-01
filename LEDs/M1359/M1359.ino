#define VCC_PIN 13

#include <FairyLights.h>

FairyLights strip(M1359L, VCC_PIN);

void setup() {
  pinMode(12, INPUT);

  Serial.begin(115200);
  Serial.println("Setup complete");
 
  strip.setColor(Purple);
  strip.setBrightness(255);
}

void loop() {
  loopNext();
}

void loopNext() {
  strip.setBrightness(10);
  strip.nextColor();
  strip.display(1000);
}

#define SIZE 128
void loopRandomArray() {
//  static byte pattern[SIZE] = {
//    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1
//  };

  //red-green-red
  // static byte pattern[SIZE] = {
  //   0,0,1,0,1,1,1,1,1,1,1,1,1,0,0,0,1,1,1,1,1,1,0,1,1,1,0,0,1,1,1,1,1,1,1,1,1,1,1,1,0,0,1,1,1,1,1,1,1,0,1,0,1,0,1,1,1,0,1,0,1,1,0,0,0,0,1,1,1,0,0,1,0,1,1,1,0,1,0,0,0,0,0,1,0,1,1,1,1,1,0,1,1,1,1,0,0,1,1,0,0,0,0,0,1,1,1,1,1,0,1,1,1,1,1,1,1,0,1,1,1,1,1,0,0,1,0,1
  // };

  //multi-color?
  // static byte pattern[SIZE] = {
  //   1,1,1,1,1,1,1,1,1,1,1,0,1,0,1,1,0,0,1,1,1,1,1,1,1,1,1,1,1,0,1,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,1,0,1,1,1,1,1,1,1,0,0,0,1,1,0,0,1,1,0,1,1,1,1,1,1,1,0,1,1,0,1,1,1,0,1,1,1,1,0,0,0,0,1,1,1,1,0,1,1,1,1,1,0,1,1,1,1,0,0,0,1,1,1,1,0,1,1,1,1,0,1,1,1,1,1,0,0,1,1
  // };

  //three color
  static byte pattern[SIZE] = {
    1,1,1,1,1,0,1,1,0,1,1,0,0,1,1,1,1,0,1,0,0,1,0,1,0,1,1,1,1,1,1,0,1,1,0,1,1,1,1,1,1,1,1,1,0,1,1,1,1,1,1,0,1,1,1,1,1,0,1,0,0,0,0,1,1,1,1,0,0,0,1,1,0,1,1,1,1,1,0,0,1,1,1,1,1,1,1,0,1,1,0,0,0,0,1,0,1,1,1,1,0,1,0,1,1,1,0,0,1,1,0,0,0,0,0,1,1,1,0,1,0,1,0,0,1,1,0,1
  };

  // for(int i=0; i<SIZE; i++) {
  //   pattern[i] = (i%4 == 1 ? 1 : 0);
  // }
  for(int i=4; i<SIZE; i++) {
    if (random(10) > 2) {
      pattern[i] = 1;
    } else {
      pattern[i] = 0;
    }
    // pattern[i] = random(2);
  }

  Serial.print("pattern: ");
  for(int i=0; i<SIZE; i++) {
    Serial.print(pattern[i]);
  }
  Serial.println("");

  // for(int i=500; i<750; i += 10) {
    strip.quickPulse(pattern, SIZE, 750);
  // }

  delay(5000);
}

void loopChaos() {  
  // strip.display();

  analogWrite(11, random(64, 200));

  delayMicroseconds(750);

  digitalWrite(11, HIGH);

  delay(1000);

  // digitalWrite(13, LOW);
  // delay(20);
  // digitalWrite(13, HIGH);

  // pinMode(12, OUTPUT);
  // digitalWrite(12, LOW);
  // delay(500);
  // pinMode(12, INPUT);

  delay(1000);
}

void loopColorFade() {
  static fl_color to = random(8);

  fl_color from = to;
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
  static int color = FL_BLUE;
  strip.setBrightness(0);
  strip.setColor((++color)%8);
  strip.display(1000);

  // strip.on();
  // delay(1000);
}

void loopFade() {
  static int brightness = 0;
  static int color = 1;
  static int increment = 4;

  brightness += increment;
  
  if (brightness <= 0 || brightness >= 255) {
    increment = -increment;
    brightness += increment;
  }

  if (brightness <= increment) {
    color = (color+1)%8;
    strip.setColor( color );
    Serial.print("Setting color: ");
    Serial.println(color);
   }

  strip.setBrightness(brightness);
  strip.display();
}
