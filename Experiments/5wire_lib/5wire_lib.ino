#define RED 9
#define BLUE 10
#define GREEN 11

#include <FourWireStrip.h>

FourWireStrip *strip = new FourWireStrip(RED, GREEN, BLUE, 0);

void setup() {
//  pinMode(9, OUTPUT);
//  pinMode(10, OUTPUT);
//  pinMode(11, OUTPUT);
//  pinMode(12, OUTPUT);
//  pinMode(13, OUTPUT);
//
//  digitalWrite(9, LOW);
//  digitalWrite(10, LOW);
//  digitalWrite(11, LOW);
//  digitalWrite(12, LOW);
//  digitalWrite(13, LOW);

  Serial.begin(115200);
  Serial.println("Setup complete");
}

void loop() {
  loopFade();
}

void loopFade() {
  strip->setBrightness(255);

  strip->fade(Red, Green, 10000);
  strip->fade(Green, Blue, 10000);
  strip->fade(Blue, Red, 10000);
}

void loopStripCycle() {
  static byte brightness = 255;

  strip->setBrightness(brightness);
  strip->nextColor();
  strip->display(1000);

  Serial.println(brightness);
  brightness--;
}

void loopStripColor() {
  strip->setBrightness(255);

  Serial.println("Red");
  strip->setColor(Red);
  strip->display();
  delay(1000);
  strip->display(1000);

  Serial.println("Green");
  strip->setColor(Green);
  strip->display();
  delay(1000);
  strip->display(1000);

  Serial.println("Blue");
  strip->setColor(Blue);
  strip->display();
  delay(1000);
  strip->display(1000);

  Serial.println("Yellow");
  strip->setColor(Yellow);
  strip->display();
  delay(1000);
  strip->display(1000);

  Serial.println("Cyan");
  strip->setColor(Cyan);
  strip->display();
  delay(1000);
  strip->display(1000);

  Serial.println("Magenta");
  strip->setColor(Magenta);
  strip->display();
  delay(1000);
  strip->display(1000); 
}

void loopColorCycle() {

  Serial.println("all");
  analogWrite(RED, 127);
  analogWrite(GREEN, 127);
  analogWrite(BLUE, 127);
  delay(1000);
  
  Serial.println("red 255");
  redAna(255);
  delay(2000);
  Serial.println("red 128");
  redAna(128);
  delay(2000);
  Serial.println("red 0");
  redAna(0);
  delay(2000);
  Serial.println("red off");
  redOff();
  delay(2000);

  Serial.println("green 255");
  greenAna(255);
  delay(2000);
  Serial.println("green 127");
  greenAna(127);
  delay(2000);
  Serial.println("green 0");
  greenAna(0);
  delay(2000);
  Serial.println("green off");
  greenOff();
  delay(2000);

  Serial.println("blue 255");
  blueAna(255);
  delay(2000);
  Serial.println("blue 128");
  blueAna(128);
  delay(2000);
  Serial.println("blue 0");
  blueAna(0);
  delay(2000);
  Serial.println("blue off");
  blueOff();
  delay(2000);
}

void redAna(byte val) {
  ana(RED, val);
  off(BLUE);
  off(GREEN);
}

void greenAna(byte val) {
  off(RED);
  ana(GREEN, val);
  off(BLUE);
}

void blueAna(byte val) {
  off(RED);
  off(GREEN);
  ana(BLUE, val);
}

void redOff() {
  low(RED);
  off(BLUE);
  off(GREEN);
}

void blueOff() {
  off(RED);
  low(BLUE);
  off(GREEN);
}

void greenOff() {
  off(RED);
  off(BLUE);
  low(GREEN);
}

void red() {
  low(RED);
  high(BLUE);
  high(GREEN);
}

void blue() {
  high(RED);
  low(BLUE);
  high(GREEN);
}

void green() {
  high(RED);
  high(BLUE);
  low(GREEN);
}

void high(int pin) {
  pinMode(pin, OUTPUT);
  digitalWrite(pin, HIGH); 
}

void low(int pin) {
  pinMode(pin, OUTPUT);
  digitalWrite(pin, LOW);
}

void off(int pin) {
  digitalWrite(pin, LOW);
  pinMode(pin, INPUT);
}

void ana(int pin, byte val) {
  pinMode(pin, OUTPUT);
  analogWrite(pin, val);
}


void loopByteCycle() {
  for(int i=0; i<8; i++) {
    Serial.println(i);
    digitalWrite( 9, !(i &  1));
    digitalWrite(10, !(i &  2));
    digitalWrite(11, !(i &  4));
//    digitalWrite(12, !(i &  8));
//    digitalWrite(13, !(i & 16));
    delay(2000);
  }
}
