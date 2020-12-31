/*
 * Motor Control Module
 */

#define M1_PWM_CNTL 6
#define M0_D0 9
#define M0_D1 10
#define M1_D0 11
#define M1_D1 12

enum motorState {
  STOP,
  FORWARD,
  REVERSE
};

enum motorNum {
  MOTOR0,
  MOTOR1
};

struct motor_t {
  int iMotor = 0;
  int iMotorPWM = 0;
  int iDirection = STOP;
  int iMotorD0 = 0;
  int iMotorD1 = 0;
};

struct motor_t motor1;

static struct motor_t Motor0;

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
    case MOTOR0 :
      digitalWrite(M0_D0, motor->iMotorD0);
      digitalWrite(M0_D1, motor->iMotorD1);
      break;
    case MOTOR1 :
      digitalWrite(M1_D0, motor->iMotorD0);
      digitalWrite(M1_D1, motor->iMotorD1);
      break;
    default:
      digitalWrite(M0_D0, LOW);
      digitalWrite(M0_D1, LOW);
      digitalWrite(M1_D0, LOW);
      digitalWrite(M1_D1, LOW);
  }
}

void motorBreak(struct motor_t *motor) {
  analogWrite(motor->iMotorPWM, HIGH);  // If the PWM is high, and the direction is set either H, H or L, L, the motor will break stop
  changeMotorDirection(motor, STOP);
  setMotorDirection(motor);  
}

void motorFreeStop(struct motor_t *motor) {
  analogWrite(motor->iMotorPWM, LOW);  // If the PWM is low, and the direction is set either H, H or L, L, the motor will break stop
  changeMotorDirection(motor, STOP);
  setMotorDirection(motor);  
}

void initMotor(struct motor_t *motor, int iMotor, int iMotorPWM) {
  motor->iMotor = iMotor;
  motor->iMotorPWM = iMotorPWM;
  motorBreak(motor);
}

void setup() {
  // put your setup code here, to run once:
  pinMode(M1_PWM_CNTL, OUTPUT);
  pinMode(M1_D0, OUTPUT);
  pinMode(M1_D1, OUTPUT);

  digitalWrite(M1_D0, LOW);
  digitalWrite(M1_D1, LOW);
  analogWrite(M1_PWM_CNTL, 0);

  initMotor(&motor1, MOTOR1, M1_PWM_CNTL);
}


void loop() {
  // put your main code here, to run repeatedly:

  motorBreak(&motor1);
  delay(1000);

  changeMotorDirection(&motor1, FORWARD);
  setMotorDirection(&motor1);
  analogWrite(motor1.iMotorPWM, HIGH);
  delay(5000);

  motorFreeStop(&motor1);
  delay(1000);
  
  changeMotorDirection(&motor1, REVERSE);
  setMotorDirection(&motor1);
  analogWrite(motor1.iMotorPWM, HIGH);
  delay(5000);
}
