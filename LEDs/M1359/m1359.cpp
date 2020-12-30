
#define VCC_PIN 13
#define SIGNAL_PIN 12

#include "m1359.h"

M1359Strip::M1359Strip(pin vcc_pin) {
  _vcc_pin = vcc_pin;
  
  // init our output pings
  pinMode(_vcc_pin, OUTPUT);
  
  // set power on
  digitalWrite(_vcc_pin, HIGH);

  // this sets us one past white (default 'on'), too "black"
  // without this, other offsets are weird
  pulseOnce();
}

void M1359Strip::pulseOnce() {
  delayMicroseconds(PACKET_DELAY);
  digitalWrite(_vcc_pin, LOW);
  delayMicroseconds(PACKET_DELAY);
  digitalWrite(_vcc_pin, HIGH);
}

void M1359Strip::pulseStrip(int count) {
  for(int i=0; i<count; i++) {
    pulseOnce();
  }
}

// deprecated?
void M1359Strip::pulse8(int pattern[]) {
  
  for (int i=0; i<8; i++) {
    delayMicroseconds(PACKET_DELAY);
    if (pattern[i] == 1) {
      digitalWrite(_vcc_pin, LOW);
    }
    digitalWrite(_vcc_pin, HIGH);
  }
  digitalWrite(_vcc_pin, HIGH);
}
