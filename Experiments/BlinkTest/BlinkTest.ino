#include "Arduino.h"

#define LED_BUILTIN 13

// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);
}

// the loop function runs over and over again forever
void loop() {
  static int d = 0;

  static int pulse = 500;

  digitalWrite(LED_BUILTIN, HIGH);

  Serial.println(sprintf("Loop number: %d", d));

  digitalWrite(LED_BUILTIN, LOW);
  delayMicroseconds(pulse);
  digitalWrite(LED_BUILTIN, HIGH);
  delayMicroseconds(pulse);
  digitalWrite(LED_BUILTIN, LOW);
  delayMicroseconds(pulse);
  digitalWrite(LED_BUILTIN, HIGH);
  delayMicroseconds(pulse);
  
  delay(1000);

  
  digitalWrite(LED_BUILTIN, LOW);
  delayMicroseconds(10000);
  digitalWrite(LED_BUILTIN, HIGH);
  delayMicroseconds(pulse);
    digitalWrite(LED_BUILTIN, LOW);
  delayMicroseconds(pulse);
  digitalWrite(LED_BUILTIN, HIGH);
  delayMicroseconds(pulse);


  
  delay(1000);

  
}
