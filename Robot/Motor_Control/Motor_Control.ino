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
  int iDirection = FORWARD;
  int iMotorD0 = 0;
  int iMotorD1 = 0;
};

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
     default:
      motor->iDirection = STOP;
      motor->iMotorD0 = LOW;
      motor->iMotorD1 = LOW;
  }
}

void setMotorDirection(struct motor_t *motor, int iMotor) {
  switch (iMotor) {
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

void setup() {
  // put your setup code here, to run once:
  pinMode(M1_PWM_CNTL, OUTPUT);
  pinMode(M1_D0, OUTPUT);
  pinMode(M1_D1, OUTPUT);

  digitalWrite(M1_D0, LOW);
  digitalWrite(M1_D1, LOW);
  analogWrite(M1_PWM_CNTL, 0);
}


void loop() {
  // put your main code here, to run repeatedly:
  struct motor_t motor1;

  analogWrite(M1_PWM_CNTL, 255);
  changeMotorDirection(&motor1, STOP);
  setMotorDirection(&motor1, MOTOR1);
  delay(1000);

  changeMotorDirection(&motor1, FORWARD);
  setMotorDirection(&motor1, MOTOR1);
  delay(5000);

  changeMotorDirection(&motor1, STOP);
  setMotorDirection(&motor1, MOTOR1);
  delay(1000);
  
  changeMotorDirection(&motor1, REVERSE);
  setMotorDirection(&motor1, MOTOR1);
  delay(5000);


}
