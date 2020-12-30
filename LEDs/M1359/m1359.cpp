
#include "m1359.h"

M1359Strip::M1359Strip(pin vcc_pin) {
  _vcc_pin = vcc_pin;
  _brightness = 255;
  _color = 1;
  
  // init our output pings
  pinMode(_vcc_pin, OUTPUT);
  
  // set power on
  digitalWrite(_vcc_pin, HIGH);

  // this sets us one past white (default 'on'), too "black"
  // without this, other offsets are weird
  pulseOnce();
}

void M1359Strip::pulseOnce() {
  delayMicroseconds(M1359_PACKET_DELAY);
  digitalWrite(_vcc_pin, LOW);
  delayMicroseconds(M1359_PACKET_DELAY);
  digitalWrite(_vcc_pin, HIGH);
}

void M1359Strip::pulse(int count) {
  for(int i=0; i<count; i++) {
    pulseOnce();
  }
}

// deprecated?
void M1359Strip::pulse8(int pattern[]) {
  
  for (int i=0; i<8; i++) {
    delayMicroseconds(M1359_PACKET_DELAY);
    if (pattern[i] == 1) {
      digitalWrite(_vcc_pin, LOW);
    }
    digitalWrite(_vcc_pin, HIGH);
  }
  digitalWrite(_vcc_pin, HIGH);
}

void M1359Strip::setColor(M1359Color value) {
  _color = value;
}

void M1359Strip::setBrightness(uint8_t value) {
  _brightness = value;
}
void M1359Strip::display() {
  //uses PWM to set brightness (this may be "too fast")
//  analogWrite(_vcc_pin, value); 

  //guessing at the muSec to display for
  int brightnessVal = map( _brightness, 0, 255, 1000, 15000);

  pulse(_color);
  delayMicroseconds(brightnessVal);
  pulse(8-_color);
  delayMicroseconds(15000 - brightnessVal);

}
