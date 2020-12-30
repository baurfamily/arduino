
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
  _lit = false;
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

//confusingly, this works different than the other pulse functions
// this one will remove voltate on a 1, and return it on a 0
// repeated values will *leave* the voltage were it was at
// there is no artificial delay, which means it should be about 3-4 MHz (I think)
void M1359Strip::quickPulse(int pattern[], int len) {
  for (int i=0; i<len; i++) {
    switch (pattern[i]) {
      case 0: digitalWrite(_vcc_pin, HIGH); break;
      case 1: digitalWrite(_vcc_pin, LOW); break;
      // no default
    }
  }
  // we do restore the high, though
  digitalWrite(_vcc_pin, HIGH);
}


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
  if (_lit) {
    reset();
    pulse(_color);
  }
}

void M1359Strip::setBrightness(uint8_t value) {
  _brightness = value;
}

// may be borked - originally between 10-20 ms delay
void M1359Strip::reset() {
  digitalWrite(_vcc_pin, LOW);
  delay(20);
  digitalWrite(_vcc_pin, HIGH);
}

void M1359Strip::on() {
  if (_lit) {
    reset();
  }
  pulse(_color);
  _lit = true;
}

void M1359Strip::off() {
  // this is probably slower than incrementing to 0
  // but more predictable if we start fading between colors
  reset();
  pulseOnce();
  _lit = false;
}

int M1359Strip::brightnessVal() {
  return map( _brightness, 0, 255, M1359_PWM_MIN, M1359_PWM_MAX);
}

void M1359Strip::display() {
  //uses PWM to set brightness (this may be "too fast")
//  analogWrite(_vcc_pin, value); 

  pulse(M1359_COLOR_COUNT - _color);
  delayMicroseconds(brightnessVal());
  pulse(_color);
  delayMicroseconds(M1359_PWM_MAX - brightnessVal());
}

void M1359Strip::fade(M1359Color fromColor, M1359Color toColor) {
  reset();

  int distance = toColor - fromColor;
  if (distance < 0) {
    distance = M1359_COLOR_COUNT - distance;
  }

  //this may be backwards...
  for(int i=0; i<=255; ) {
    int fadeVal = map( i, 0, 255, M1359_PWM_MIN, M1359_PWM_MAX);

    // start at initial color
    pulse(fromColor);
    delayMicroseconds(fadeVal);
    
    // get to next color, may have to wrap
    pulse(distance);
    delayMicroseconds(M1359_PWM_MAX - brightnessVal());
    
    // back to black (ie: effective reset)s
    pulse(M1359_COLOR_COUNT - toColor);
  }
}
