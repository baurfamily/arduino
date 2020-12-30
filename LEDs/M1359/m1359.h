
#ifndef M1359_h
#define M1359_h

#define M1359_PACKET_SIZE 8
#define M1359_PACKET_DELAY 20

// these were fairly arbitrary picks
// it looked good to *our* eyes
#define M1359_PWM_MIN 1000
#define M1359_PWM_MAX 15000

#define M1359_OFF 0
#define M1359_RED 1
#define M1359_GREEN 2
#define M1359_YELLOW 3
#define M1359_BLUE 4
#define M1359_PURPLE 5
#define M1359_CYAN 6
#define M1359_WHITE 7

#define M1359_COLOR_COUNT 8

#include "Arduino.h"

typedef int pin;
typedef uint8_t M1359Color;

class M1359Strip
{
  public:
    M1359Strip(pin vcc);
    void pulseOnce();
    void pulse(int count);
    
    //  static int redFromReset[PACKET_SIZE] = { 1, 1, 0, 0, 0, 0, 0, 0 };
    //  static int resetFromRed[PACKET_SIZE] = { 1, 1, 1, 1, 1, 1, 0, 0 };
    void pulse8(int pattern[]);
    void quickPulse(int pattern[], int len);

    void setColor(M1359Color value);
    void setBrightness(uint8_t value);

    void on();
    void off();
    void reset();
    void display();

    void fade(M1359Color fromColor, M1359Color toColor);

  private:
    int _vcc_pin;
    uint8_t _brightness;
    uint8_t _color; // actually, 3 bits
    bool _lit;

    int brightnessVal();
};

#endif
