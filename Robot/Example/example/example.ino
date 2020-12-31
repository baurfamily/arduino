#include <Servo.h>

// defines for the servo
#define SERVO_PIN 9
#define SERVO_HOME 90
#define NUM_ANGLES 5
#define TOO_CLOSE 300 // distance something is too close in mm

//defines for the usensor
#define TRIG_PIN 13 // 10 microsecond high pulse causes chirp , wait 50 us between pulses
#define ECHO_PIN 12 // Width of high pulse indicates distance

#define TRUE 1
#define FALSE 0

struct scan_t {
  int iAngle;
  int iDistance;
  bool bTooClose;
};

struct scan_t scan_data[NUM_ANGLES];

Servo head;  // create servo object to control a servo

/*
 * readDistance
 * Read distance from the ultrasonic sensor , return distance in mm
 * 
 * NOTE: The speed of sound in dry air @ 20'C is 343 m/s.  To know the distance from
 *       an object, we have to calculate the travel time using the the formulat D = R*T.
 *       Since we know that the R = 343m/s, and we will get the T, time, then we solve
 *       for distance, by multiplying the two.  Also, the sensor, will send the pulse
 *       and will bounce off the object, and then return to to the sensor, so the distance is
 *       twice wht we need it to be, to we have to half the result.
 */

unsigned int readDistance() {
  unsigned int distance = 0;

  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(15);
  
  // This function blocks waiting for the echoPin to go high, then wait for 1s by default,
  // before timing out.  Return value is in uS.
  unsigned long period = pulseIn (ECHO_PIN, HIGH, 180000UL);
//  Serial.println(period);
  if((period > 0)) { // && (period < 118000)) {
    // since the period is in uS, and the speed of sound is in m/s, the result is is meters, which we then
    // need to adjust to somethng more reasonable, so deviding by 1000, results in mm.  Also, since we are 
    // using integer math, we loose precision, so these will be rounded.
    distance = 343 * period / 2000;
  }

  return distance;
}

bool scan() {
  bool bGoodScan = TRUE;
  
  for(int positions = 0; positions < NUM_ANGLES; positions++) {
    scan_data[positions].iAngle = (180/(NUM_ANGLES-1))*positions;
    head.write(scan_data[positions].iAngle);
    scan_data[positions].iDistance = readDistance();

    // If we received a distance of 0, then we got a bad read, so return this info
    if(scan_data[positions].iDistance == 0)
      bGoodScan = FALSE;
      
    if(scan_data[positions].iDistance < TOO_CLOSE) { // something is too close, or we didn't get a good distance set flag
      scan_data[positions].bTooClose = TRUE;
    } else {
      scan_data[positions].bTooClose = FALSE;
    }
    
    delay(100);
  }

  head.write(SERVO_HOME);

  return bGoodScan;
}

void setup() {
  // put your setup code here, to run once:

  //Setup Serial output
  Serial.begin(115200);

  //Setup for Head Servo
  head.attach(SERVO_PIN);
  head.write(SERVO_HOME);

  //Setup for Ultra Sonic Sensor
  pinMode(TRIG_PIN, OUTPUT);
  digitalWrite(TRIG_PIN, LOW);
  pinMode(ECHO_PIN, INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:

  // wait for a good scan
  while(!scan()) {
    delay(500);
  }

  // look to see if any of the angles we found are too close
  for(int positions = 0; positions < NUM_ANGLES; positions++) {
    Serial.print("Angle: ");
    Serial.println(scan_data[positions].iAngle);
    Serial.print("Distance: ");
    Serial.println(scan_data[positions].iDistance);
    if(scan_data[positions].bTooClose) {
      
    }
  }

  delay(5000);
}
