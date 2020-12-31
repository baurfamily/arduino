/*
 * Servo Module
 */

#include <Servo.h>

#define SERVO_PIN 9
#define SERVO_HOME 90

Servo myservo;  // create servo object to control a servo

int pos = 0;    // variable to store the servo position

void setup() {
  // put your setup code here, to run once:
  myservo.attach(SERVO_PIN);
  myservo.write(SERVO_HOME);
}

void loop() {
  // put your main code here, to run repeatedly:

  for (pos = 0; pos <= 180; pos = pos + 5) { // goes from 0 degrees to 180 degrees
    // in steps of 15 degree
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(50);                       // waits 15ms for the servo to reach the position
  }
  for (pos = 180; pos >= 0; pos = pos - 5) { // goes from 0 degrees to 180 degrees
    // in steps of 15 degree
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(50);                       // waits 15ms for the servo to reach the position
  }
//  myservo.write(180);
//  delay(1000);
//  myservo.write(90);
//  delay(1000);
//  myservo.write(0);
//  delay(1000);
//  myservo.write(90);
//  delay(1000);
}
