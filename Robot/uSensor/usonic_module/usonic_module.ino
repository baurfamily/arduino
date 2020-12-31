/*
 * Ultra Sonic Sensor Module
 */

// Ultrasonic Module pins
const int trigPin = 13; // 10 microsecond high pulse causes chirp , wait 50 us between pulses
const int echoPin = 12; // Width of high pulse indicates distance

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

  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  delayMicroseconds(15);
  
  // This function blocks waiting for the echoPin to go high, then wait for 1s by default,
  // before timing out.  Return value is in uS.
  unsigned long period = pulseIn (echoPin, HIGH, 180000UL);
//  Serial.println(period);
  if((period > 0) && (period < 118000)) {
    // since the period is in uS, and the speed of sound is in m/s, the result is is meters, which we then
    // need to adjust to somethng more reasonable, so deviding by 1000, results in mm.  Also, since we are 
    // using integer math, we loose precision, so these will be rounded.
    distance = 343 * period / 2000;
  }

  return distance;
}


void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(trigPin, OUTPUT);
  digitalWrite(trigPin, LOW);
  
  pinMode(echoPin, INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println(readDistance());
  delay(250);
}
