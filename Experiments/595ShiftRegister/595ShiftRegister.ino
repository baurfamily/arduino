//**************************************************************//
//  Name    : shiftOutCode, Hello World
//  Author  : Carlyn Maw,Tom Igoe, David A. Mellis
//  Date    : 25 Oct, 2006
//  Modified: 23 Mar 2010
//  Version : 2.0
//  Notes   : Code for using a 74HC595 Shift Register           //
//          : to count from 0 to 255
//****************************************************************

//Pin connected to ST_CP of 74HC595
int latchPin = 8;

//Pin connected to SH_CP of 74HC595
int clockPin = 12;

////Pin connected to DS of 74HC595
int dataPin = 11;

void setup() {
  //set pins to output so you can control the shift register
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
}

void loop() {
  // count from 0 to 255 and display the number
  // on the LEDs
  for (int numberToDisplay = 0; numberToDisplay < 256; numberToDisplay++) {
    // take the latchPin low so
    // the LEDs don't change while you're sending in bits:
    digitalWrite(latchPin, LOW);

    // shift out the bits:
    shiftOut(dataPin, clockPin, MSBFIRST, numberToDisplay);

    //take the latch pin high so the LEDs will light up:
    digitalWrite(latchPin, HIGH);

    // pause before next value:
    delay(500);
  }
}

/*
  Shift Register Example
  for 74HC595 shift register
  This sketch turns reads serial input and uses it to set the pins
  of a 74HC595 shift register.
  Hardware:
  * 74HC595 shift register attached to pins 8, 12, and 11 of the Arduino,
  as detailed below.
  * LEDs attached to each of the outputs of the shift register.
  Created 22 May 2009
  Created 23 Mar 2010
  by Tom Igoe
*/

void loop2() {
  if (Serial.available() > 0) {
    // ASCII '0' through '9' characters are
    // represented by the values 48 through 57.
    // so if the user types a number from 0 through 9 in ASCII,
    // you can subtract 48 to get the actual value:
    int bitToSet = Serial.read() - 48;
    // write to the shift register with the correct bit set high:
    registerWrite(bitToSet, HIGH);
  }
}

// This method sends bits to the shift register:
void registerWrite(int whichPin, int whichState) {
  // the bits you want to send
  byte bitsToSend = 0;
  
  // turn off the output so the pins don't light up
  // while you're shifting bits:
  digitalWrite(latchPin, LOW);
  
  // turn on the next highest bit in bitsToSend:
  bitWrite(bitsToSend, whichPin, whichState);
  
  // shift the bits out:
  shiftOut(dataPin, clockPin, MSBFIRST, bitsToSend);
  
  // turn on the output so the LEDs can light up:
  digitalWrite(latchPin, HIGH);
}
