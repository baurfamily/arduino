
#ifndef M1359_h
#define M1359_h

#define PACKET_SIZE 8
#define PACKET_DELAY 20

#include "Arduino.h"

typedef int pin;

class M1359Strip
{
  public:
    M1359Strip(pin vcc);
    void pulseOnce();
    void pulseStrip(int count);
    
    // deprecated?
    // ex:
    //  static int redFromReset[PACKET_SIZE] = { 1, 1, 0, 0, 0, 0, 0, 0 };
    //  static int resetFromRed[PACKET_SIZE] = { 1, 1, 1, 1, 1, 1, 0, 0 };
    void pulse8(int pattern[]);

  private:
    int _vcc_pin;
};

#endif
