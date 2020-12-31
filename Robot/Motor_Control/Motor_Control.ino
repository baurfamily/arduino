/*
 * Motor Control Module
 */

#define LEFT_PWM_CNTL 6
#define LEFT_D0 7
#define LEFT_D1 5
#define RIGHT_PWM_CNTL 3
#define RIGHT_D0 4
#define RIGHT_D1 2

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

void changeMotorDirection(struct motor_t *motor, int iMotorState) {
  switch (iMotorState) {
    case FORWARD :
      motor->iDirection = iMotorState;
      motor->iMotorD0 = LOW;
      motor->iMotorD1 = HIGH;
      break;
     case REVERSE:
      motor->iDirection = iMotorState;
      motor->iMotorD0 = HIGH;
      motor->iMotorD1 = LOW;
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
  changeMotorDirection(&left, FORWARD);
  changeMotorDirection(&right, REVERSE);
  setMotorDirection(&left);
  setMotorDirection(&right);
  setMotorSpeed(&left, 128);
  setMotorSpeed(&right, 128);
  delay(period);
  goStop();
}

void goRight(int period) {
  goStop();
  changeMotorDirection(&left, REVERSE);
  changeMotorDirection(&right, FORWARD);
  setMotorDirection(&left);
  setMotorDirection(&right);
  setMotorSpeed(&left, 128);
  setMotorSpeed(&right, 128);
  delay(period);
  goStop();
}

void goForward(int period) {
  goStop();
  changeMotorDirection(&left, FORWARD);
  changeMotorDirection(&right, FORWARD);
  setMotorDirection(&left);
  setMotorDirection(&right);
  setMotorSpeed(&left, 128);
  setMotorSpeed(&right, 128);
  delay(period);
  goStop();
}

void goBackward(int period) {
  goStop();
  changeMotorDirection(&left, REVERSE);
  changeMotorDirection(&right, REVERSE);
  setMotorDirection(&left);
  setMotorDirection(&right);
  setMotorSpeed(&left, 128);
  setMotorSpeed(&right, 128);
  delay(period);
  goStop();
}

void goStop() {
  setMotorSpeed(&left, 0);
  setMotorSpeed(&right, 0);
}

void motorTest() {
  goForward(1000);
  goBackward(1000);
  goLeft(1000);
  goRight(1000);
}

void setup() {
  // put your setup code here, to run once:
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

  motorTest();
  delay(1000);
}
