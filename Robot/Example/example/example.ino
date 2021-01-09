#include <Servo.h>

// defines for the servo
#define SERVO_PIN 11
#define SERVO_HOME 90
#define NUM_ANGLES 5
#define TOO_CLOSE 400 // distance something is too close in mm

//defines for the usensor
#define TRIG_PIN 13 // 10 microsecond high pulse causes chirp , wait 50 us between pulses
#define ECHO_PIN 12 // Width of high pulse indicates distance

//defines for the motors
#define LEFT_PWM_CNTL 6
#define LEFT_D0 7
#define LEFT_D1 5
#define RIGHT_PWM_CNTL 3
#define RIGHT_D0 4
#define RIGHT_D1 2

#define LEFT_FULL_SPEED 155
#define RIGHT_FULL_SPEED 95

#define TRUE 1
#define FALSE 0

enum motorState {
  STOP,
  FORWARD,
  REVERSE
};

enum motorNum {
  LEFT,
  RIGHT
};

struct motor_t {
  int iMotor = 0;
  int iMotorPWM = 0;
  int iDirection = STOP;
  int iMotorD0 = 0;
  int iMotorD1 = 0;
};

static struct motor_t left;
static struct motor_t right;

struct scan_t {
  int iAngle;
  int iDistance;
  bool bTooClose;
};

struct scan_t scan_data[NUM_ANGLES];

Servo head;  // create servo object to control a servo

void changeMotorDirection(struct motor_t *motor, int iMotorState) {
  switch (iMotorState) {
    case FORWARD :
      motor->iDirection = iMotorState;
      motor->iMotorD0 = HIGH;
      motor->iMotorD1 = LOW;
      break;
     case REVERSE:
      motor->iDirection = iMotorState;
      motor->iMotorD0 = LOW;
      motor->iMotorD1 = HIGH;
      break;
     case STOP:
      motor->iDirection = iMotorState;
      motor->iMotorD0 = LOW;
      motor->iMotorD1 = LOW;
      break;
     default:
      motor->iDirection = STOP;
      motor->iMotorD0 = LOW;
      motor->iMotorD1 = LOW;
  }
}

void setMotorDirection(struct motor_t *motor) {
  switch (motor->iMotor) {
    case LEFT :
      digitalWrite(LEFT_D0, motor->iMotorD0);
      digitalWrite(LEFT_D1, motor->iMotorD1);
      break;
    case RIGHT :
      digitalWrite(RIGHT_D0, motor->iMotorD0);
      digitalWrite(RIGHT_D1, motor->iMotorD1);
      break;
    default:
      digitalWrite(LEFT_D0, LOW);
      digitalWrite(LEFT_D1, LOW);
      digitalWrite(RIGHT_D0, LOW);
      digitalWrite(RIGHT_D1, LOW);
  }
}

void motorBreak(struct motor_t *motor) {
  analogWrite(motor->iMotorPWM, 255);  // If the PWM is high, and the direction is set either H, H or L, L, the motor will break stop
  changeMotorDirection(motor, STOP);
  setMotorDirection(motor);  
}

void motorFreeStop(struct motor_t *motor) {
  analogWrite(motor->iMotorPWM, 0);  // If the PWM is low, and the direction is set either H, H or L, L, the motor will break stop
  changeMotorDirection(motor, STOP);
  setMotorDirection(motor);  
}

void initMotor(struct motor_t *motor, int iMotor, int iMotorPWM) {
  motor->iMotor = iMotor;
  motor->iMotorPWM = iMotorPWM;
  motorBreak(motor);
}

void setMotorSpeed(struct motor_t *motor, int iSpeed) {
  analogWrite(motor->iMotorPWM, iSpeed);
}

void goLeft(int period) {
  goStop();
  changeMotorDirection(&left, REVERSE);
  changeMotorDirection(&right, FORWARD);
  setMotorDirection(&left);
  setMotorDirection(&right);
  setMotorSpeed(&left, LEFT_FULL_SPEED+50);
  setMotorSpeed(&right, RIGHT_FULL_SPEED+50);
  delay(period);
  goStop();
}

void goRight(int period) {
  goStop();
  changeMotorDirection(&left, FORWARD);
  changeMotorDirection(&right, REVERSE);
  setMotorDirection(&left);
  setMotorDirection(&right);
  setMotorSpeed(&left, LEFT_FULL_SPEED+50);
  setMotorSpeed(&right, RIGHT_FULL_SPEED+50);
  delay(period);
  goStop();
}

void goForward(int period) {
  goStop();
  changeMotorDirection(&left, FORWARD);
  changeMotorDirection(&right, FORWARD);
  setMotorDirection(&left);
  setMotorDirection(&right);
  setMotorSpeed(&left, LEFT_FULL_SPEED);
  setMotorSpeed(&right, RIGHT_FULL_SPEED);
  delay(period);
  goStop();
}

void goBackward(int period) {
  goStop();
  changeMotorDirection(&left, REVERSE);
  changeMotorDirection(&right, REVERSE);
  setMotorDirection(&left);
  setMotorDirection(&right);
  setMotorSpeed(&left, LEFT_FULL_SPEED);
  setMotorSpeed(&right, RIGHT_FULL_SPEED);
  delay(period);
  goStop();
}

void goStop() {
  setMotorSpeed(&left, 0);
  setMotorSpeed(&right, 0);
}

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
  if((period > 0) && (period < 118000)) {
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
    delay(250);
    scan_data[positions].iDistance = readDistance();

    // If we received a distance of 0, then we got a bad read, so return this info
    if(scan_data[positions].iDistance == 0)
      bGoodScan = FALSE;
      
    if(scan_data[positions].iDistance < TOO_CLOSE) { // something is too close, or we didn't get a good distance set flag
      scan_data[positions].bTooClose = TRUE;
    } else {
      scan_data[positions].bTooClose = FALSE;
    }
    
    delay(250);
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

  pinMode(LEFT_PWM_CNTL, OUTPUT);
  pinMode(LEFT_D0, OUTPUT);
  pinMode(LEFT_D1, OUTPUT);

  pinMode(RIGHT_PWM_CNTL, OUTPUT);
  pinMode(RIGHT_D0, OUTPUT);
  pinMode(RIGHT_D1, OUTPUT);

  digitalWrite(LEFT_D0, LOW);
  digitalWrite(LEFT_D1, LOW);
  analogWrite(LEFT_PWM_CNTL, 0);

  digitalWrite(RIGHT_D0, LOW);
  digitalWrite(RIGHT_D1, LOW);
  analogWrite(RIGHT_PWM_CNTL, 0);

  initMotor(&left, LEFT, LEFT_PWM_CNTL);
  initMotor(&right, RIGHT, RIGHT_PWM_CNTL);
}

void loop() {
  // put your main code here, to run repeatedly:

  delay(1000);

  head.write(90);   // look strait ahead

  while(readDistance() > TOO_CLOSE) {
    switch(random(4)) {
      case 0: goLeft(random(250, 750));
      break;
      case 1: goRight(random(250, 750));
      break;
      case 2:
      case 3: goForward(random(250, 750));
      break;
    }
    delay(500);
  }

  goStop();

  switch(random(3)) {
    case 0: goLeft(random(250, 750));
    break;
    case 1: goRight(random(250, 750));
    break;
    case 2: goBackward(random(250, 750));
    break;
  }

  switch(random(3)) {
    case 0: goLeft(random(250, 750));
    break;
    case 1: goRight(random(250, 750));
    break;
    case 2: goBackward(random(250, 750));
    break;
  }
//  // wait for a good scan
//  while(!scan()) {
//    delay(500);
//  }
//
//  // look to see if any of the angles we found are too close
//  for(int positions = 0; positions < NUM_ANGLES; positions++) {
//    Serial.print("Angle: ");
//    Serial.println(scan_data[positions].iAngle);
//    Serial.print("Distance: ");
//    Serial.println(scan_data[positions].iDistance);
//    if(scan_data[positions].bTooClose) {
//      
//    }
//  }
//  Serial.println("");
//  delay(5000);
}
