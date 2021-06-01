#include <SPI.h>
#include <nRF24L01.h>
#include <RF24_config.h>
#include <RF24.h>

#define SEVEN_SEG_D0 5
#define SEVEN_SEG_D1 4
#define SEVEN_SEG_D2 3
#define SEVEN_SEG_D3 2
#define SEVEN_SEG_LEFT_DIGIT_LE_N 14 // A0
#define SEVEN_SEG_RIGHT_DIGIT_LE_N 15 // A1

#define LS_ARM 20 // A6
#define LS_LAUNCH 21 // A7
#define LS_SELECT 16 // A2

#define LS_CONT_1 17 // A3
#define LS_ARMED 18 // A4
#define LS_CONT_0 19 // A5
#define LS_READY 6
#define LS_UNLOCKED 7

#define LS_SPI_CE   9
#define LS_SPI_CSN  10

#define LEFT_DIGIT 0
#define RIGHT_DIGIT 1

#define LAUNCH_PRESSED 0
#define PAD_SELECT_0 0
#define PAD_SELECT_1 1
#define ARM_SELECTED 0

#define TRANSMITRETRY 5 // will attempt to retry 5 times before failing to be ready
#define COUNTDOWNTIMERINTERVAL 100 // millis
#define BLINKINTERVAL 500 // millis
#define TRANSMITINTERVAL 250 //millis
#define DEBOUNCEDELAY 50 // millis

struct base_state_t {
  boolean unlocked = 0; // This is set if the base has been unlocked
  boolean cont_0 = 0; // This is set if there is continuity on pad 0
  boolean cont_1 = 0; // This is set if there is continuity on pad 1 
  byte sequence = 0; // base sequence number
};

struct controller_state_t {
  boolean armed = 0; // This is set if the control as been armed
  boolean readyToLaunch = 0; // count down complete, ready to launch
  boolean select = 0; // zero if pad 0, one if pad 1 is selected
  boolean launch = 0; // set when a request to launch has been sent
  byte sequence = 0; // control sequence number
};

struct base_state_t base_state;
struct controller_state_t controller_state;
static int countReady = 0;
static boolean countingDown = false;
static byte secondsDigit = 0;
static byte milliSecDigit = 0;
boolean readyToLaunch = ((secondsDigit == 0) && (milliSecDigit == 0));
static boolean padWhenArmed = 0;
static boolean isArmed = false;
static boolean canArm = true;
static byte secondsDigitDefault = 6;
static byte milliSecDigitDefault = 0;

RF24 radio(LS_SPI_CE, LS_SPI_CSN);
const byte address[][6] = {"Contl", "Base"};  // Address of the pipes we will be using

enum TX_RX {
  CONTROL = 0,
  BASE = 1
};

boolean debouncedArmSwitchRead() {
  // read the state of the switch into a local variable:
  static boolean lastState = LOW;
  static boolean currentState = LOW;
  static unsigned long lastDebounceTime = 0;
  boolean reading = readArm();

  if (reading != lastState) {
    // reset the debouncing timer
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > DEBOUNCEDELAY) {
    // whatever the reading is at, it's been there for longer than the debounce
    // delay, so take it as the actual current state:
    currentState = reading;

    // if the button state has changed:
  }
  lastState = reading;

  return currentState;
}

boolean debouncedSelectSwitchRead() {
  // read the state of the switch into a local variable:
  static boolean lastState = LOW;
  static boolean currentState = LOW;
  static unsigned long lastDebounceTime = 0;
  boolean reading = digitalRead(LS_SELECT);

  if (reading != lastState) {
    // reset the debouncing timer
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > DEBOUNCEDELAY) {
    // whatever the reading is at, it's been there for longer than the debounce
    // delay, so take it as the actual current state:
    currentState = reading;

    // if the button state has changed:
  }
  lastState = reading;

  return currentState;
}

boolean debouncedLaunchSwitchRead() {
  // read the state of the switch into a local variable:
  static boolean lastState = LOW;
  static boolean currentState = LOW;
  static unsigned long lastDebounceTime = 0;
  boolean reading = readLaunch();

  if (reading != lastState) {
    // reset the debouncing timer
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > DEBOUNCEDELAY) {
    // whatever the reading is at, it's been there for longer than the debounce
    // delay, so take it as the actual current state:
    currentState = reading;

    // if the button state has changed:
  }
  lastState = reading;

  return currentState;
}
void countDown() {
  
  if(!countingDown || (secondsDigit == 0) && (milliSecDigit == 0)) {
    return;
  }
  
  if(milliSecDigit == 0) {
    milliSecDigit = 9;
    if(secondsDigit > 0) {
      secondsDigit--;
    }
  } else {
    milliSecDigit--;
  }
  setDigit(LEFT_DIGIT, secondsDigit);
  setDigit(RIGHT_DIGIT, milliSecDigit);
   
}

void checkReady() {
  static boolean blinkState = LOW;

  if(!countReady) {
   blinkState = !blinkState;
   digitalWrite(LS_READY, blinkState); 
  } else {
    blinkState = HIGH;
    digitalWrite(LS_READY, blinkState); 
  }   
}

boolean readArm() {
  if(analogRead(LS_ARM) > 500) {
    return 1;
  } else {
    return 0;
  }
}

boolean readLaunch() {
  if(analogRead(LS_LAUNCH) > 500) {
    return 1;
  } else {
    return 0;
  }
}

void setDigit(boolean digit, byte value) {
  
  digitalWrite(SEVEN_SEG_D3, (value & 0x08) >> 3);
  digitalWrite(SEVEN_SEG_D2, (value & 0x04) >> 2);
  digitalWrite(SEVEN_SEG_D1, (value & 0x02) >> 1);
  digitalWrite(SEVEN_SEG_D0, (value & 0x01) >> 0);
  
  if(digit == LEFT_DIGIT) {
    digitalWrite(SEVEN_SEG_LEFT_DIGIT_LE_N, HIGH);
//    delay(5);
    digitalWrite(SEVEN_SEG_LEFT_DIGIT_LE_N, LOW);
  }
  if(digit == RIGHT_DIGIT) {
    digitalWrite(SEVEN_SEG_RIGHT_DIGIT_LE_N, HIGH);
//    delay(5);
    digitalWrite(SEVEN_SEG_RIGHT_DIGIT_LE_N, LOW);
  }
  
}

/*
 * Flashes the LEDS and timer to make sure they are all working
 */
void lampTest() {
  for(int i = 0; i < 4; i++) {
    setDigit(LEFT_DIGIT, 8);
    setDigit(RIGHT_DIGIT, 8);
    digitalWrite(LS_ARMED, HIGH);
    digitalWrite(LS_UNLOCKED, HIGH);
    digitalWrite(LS_READY, HIGH);
    digitalWrite(LS_CONT_0, HIGH);
    digitalWrite(LS_CONT_1, HIGH); 
    delay(125);   
    setDigit(LEFT_DIGIT, 10);
    setDigit(RIGHT_DIGIT,10);
    digitalWrite(LS_ARMED, LOW);
    digitalWrite(LS_UNLOCKED, LOW);
    digitalWrite(LS_READY, LOW);
    digitalWrite(LS_CONT_0, LOW);
    digitalWrite(LS_CONT_1, LOW); 
    delay(125);   
  }
}

void resetTimer() {
  secondsDigit = secondsDigitDefault;
  milliSecDigit = milliSecDigitDefault;
  setDigit(LEFT_DIGIT, secondsDigit);
  setDigit(RIGHT_DIGIT, milliSecDigit);
}

void setAsTransmitter() {
  radio.stopListening();  // put radio in TX mode
}


void setAsReceiver() {
  radio.startListening();  // put radio in RX mode
}

void setup() {
  // put your setup code here, to run once:

  Serial.begin(9600);
  SPI.begin();

  pinMode(SEVEN_SEG_D0, OUTPUT);
  pinMode(SEVEN_SEG_D1, OUTPUT);
  pinMode(SEVEN_SEG_D2, OUTPUT);
  pinMode(SEVEN_SEG_D3, OUTPUT);
  pinMode(SEVEN_SEG_LEFT_DIGIT_LE_N, OUTPUT);
  pinMode(SEVEN_SEG_RIGHT_DIGIT_LE_N, OUTPUT);

  pinMode(LS_ARM, INPUT);
  pinMode(LS_LAUNCH, INPUT);
  pinMode(LS_SELECT, INPUT);

  pinMode(LS_ARMED, OUTPUT);
  pinMode(LS_CONT_0, OUTPUT);
  pinMode(LS_CONT_1, OUTPUT);
  pinMode(LS_READY, OUTPUT);
  pinMode(LS_UNLOCKED, OUTPUT);

  digitalWrite(SEVEN_SEG_LEFT_DIGIT_LE_N, LOW);
  digitalWrite(SEVEN_SEG_RIGHT_DIGIT_LE_N, LOW);

  lampTest();
  resetTimer();

  if (!radio.begin()) {
    Serial.println(F("radio hardware not responding!"));
    while (1) {
        digitalWrite(LS_READY, HIGH);
        digitalWrite(LS_ARMED, HIGH);
        delay(250);
        digitalWrite(LS_READY, LOW);
        digitalWrite(LS_ARMED, LOW);
        delay(250);
      } // hold program in infinite loop to prevent subsequent errors
  }

  radio.setPALevel(RF24_PA_MIN);         // RF24_PA_MIN / RF24_PA_LOW / RF24_PA_HIGH / RF24_PA_MAX is default.
  radio.setDataRate(RF24_1MBPS);
  //radio.setPayloadSize(sizeof(STATE));     // default value is the maximum 32 bytes

  radio.openWritingPipe(address[BASE]);
  radio.openReadingPipe(1, address[CONTROL]);
//  radio.startListening();  // put radio in RX mode

  while(!readArm()) {  // Arm switch is set during setup, go into setting countdown delay
    setDigit(LEFT_DIGIT, 10);
    setDigit(RIGHT_DIGIT,10);
    delay(250);
    if(!readLaunch()) { // Launch button is pressed, add one to seconds digit, or if 9, return it to 0
      secondsDigitDefault++;
      if(secondsDigitDefault > 9) {
        secondsDigitDefault = 0;
      }
    }
    setDigit(LEFT_DIGIT, secondsDigitDefault);
    setDigit(RIGHT_DIGIT, milliSecDigitDefault);
    delay(250);
  }
}

void loop() {
  boolean launchButtonPressed = !debouncedLaunchSwitchRead();
  boolean armedSwitchOn = !debouncedArmSwitchRead();
  boolean selectedPad = debouncedSelectSwitchRead();
  boolean pad0Selected = !selectedPad;
  boolean pad1Selected = selectedPad;
  boolean radioReady = countReady > 0;
  unsigned long currentTime = millis();
  static unsigned long countDownPreviousTime = currentTime;
  static unsigned long countReadyPreviousTime = currentTime;
  static unsigned long transmitPreviousTime = currentTime;
  boolean readyToLaunch = ((secondsDigit == 0) && (milliSecDigit == 0));

  // Debug code to test the countdown timer
//  if(armedSwitchOn) {
//    countingDown = true;
//  } else {
//    countingDown = false;
//    resetTimer();
//  }

  // Attempt to count down every COUNTDOWNTIMERINTERVAL
  if((currentTime - countDownPreviousTime) >= COUNTDOWNTIMERINTERVAL) {
    countDownPreviousTime += COUNTDOWNTIMERINTERVAL;
    countDown();
  }

  // Update Ready LED state every BLINKINTERVAL
  if((currentTime - countReadyPreviousTime) >= BLINKINTERVAL) {
    countReadyPreviousTime += BLINKINTERVAL;
    checkReady();
//    Serial.println(countReady); 
  }

  // Transmit every 250ms a heartbeat packet to the base
  if((currentTime - transmitPreviousTime) >= TRANSMITINTERVAL) {
    transmitPreviousTime += TRANSMITINTERVAL;
    controller_state.sequence++;
    setAsTransmitter();
    bool rslt = radio.write(&controller_state, sizeof(controller_state_t));
    if (rslt) {
//      Serial.println("Transmit Success!");
      countReady = TRANSMITRETRY;
    } else {
//      Serial.println("Transmit Failed!"); 
      // If we can't transmit a packet, then decremmet the retry count 
      if(countReady > 0) {
        countReady--;
      }
    }
    setAsReceiver();
  }

  // always try and receive data from the base
  if ( radio.available() ) {
//    uint8_t bytes = radio.getPayloadSize(); // get the size of the payload
//    Serial.print("Payload Size: ");Serial.println(bytes);
    radio.read(&base_state, sizeof(base_state_t));            // fetch payload from FIFO

//    Serial.print("Data Received, Base Sequence: "); Serial.println(base_state.sequence);
//    Serial.print("Unlock: "); Serial.println(base_state.unlocked);
//    Serial.print("Continuity 0: "); Serial.println(base_state.cont_0);
//    Serial.print("Continuity 1: "); Serial.println(base_state.cont_1);

    digitalWrite(LS_UNLOCKED, base_state.unlocked);
    digitalWrite(LS_CONT_0, base_state.cont_0);
    digitalWrite(LS_CONT_1, base_state.cont_1);
//  } else {
//    Serial.println("No Data");    
  }

  if(!armedSwitchOn) {
    isArmed = false;
    canArm = true;
  }

  if(!((pad0Selected && base_state.cont_0) || (pad1Selected && base_state.cont_1))) {
    canArm = false;
  }

  if(!base_state.unlocked) {
    canArm = false;
  }

//  Serial.print("radioReady: "); Serial.println(radioReady);
  if(radioReady) {
    if(armedSwitchOn && canArm) {

      if(!isArmed) {
        padWhenArmed = selectedPad;
        isArmed = true;
      }
      
      if(isArmed && (padWhenArmed == selectedPad)) {
        controller_state.armed = true;
        countingDown = true;
        digitalWrite(LS_ARMED, HIGH); 
      } else {
        canArm = false;
      }
    } else {
        resetTimer();
        countingDown = false;
        controller_state.armed = false;
        digitalWrite(LS_ARMED, LOW);      
    }
  } else {
      resetTimer();
      countingDown = false;
      controller_state.armed = false;
      canArm = false;
      digitalWrite(LS_ARMED, LOW);
      digitalWrite(LS_UNLOCKED, LOW);
      digitalWrite(LS_CONT_0, LOW);
      digitalWrite(LS_CONT_1, LOW);
  }

  controller_state.launch = readyToLaunch && launchButtonPressed;
  controller_state.select = selectedPad;
  controller_state.readyToLaunch = readyToLaunch;

// Debug for running to count down timer
//  for(int i = 9; i >= 0; i--) {
//    setDigit(LEFT_DIGIT, i);
//    for(int j = 9; j >= 0; j--) {
//      setDigit(RIGHT_DIGIT, j);
//      delay(100);
//    }
//  }

}
