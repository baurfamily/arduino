// This sketch is used to for the Launch System Base
#include <SPI.h>
#include <printf.h>
#include <nRF24L01.h>
#include <RF24_config.h>
#include <RF24.h>

#define LS_SPI_CE   9
#define LS_SPI_CSN  10

#define LS_BUZZER_EN 2
#define LS_SELECT 5
#define LS_READY 6
#define LS_UNLOCK 7
#define LS_CONT_0 14
#define LS_CONT_1 15
#define LS_FIRE_0 17
#define LS_FIRE_1 16

#define TRANSMITRETRY 10 // will attempt to retry 5 times before failing to be ready
#define COUNTDOWNTIMERINTERVAL 100 // millis
#define BLINKINTERVAL 500 // millis
#define TRANSMITINTERVAL 250 //millis
#define DEBOUNCEDELAY 50 // millis
#define BUZZINTERVAL 500 // milliseconds
#define FIREINTERVAL 1000 // millis

// instantiate an object for the nRF24L01 transceiver
RF24 radio(LS_SPI_CE, LS_SPI_CSN);

// Let these addresses be used for the pair
const byte address[][6] = {"Contl", "Base"};  // Address of the pipes we will be using

enum TX_RX {
  CONTROL = 0,
  BASE = 1
};

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
static boolean canLaunch = true;

boolean debounceUnlockRead() {
  // read the state of the unlock into a local variable:
  static boolean lastState = LOW;
  static boolean currentState = LOW;
  static unsigned long lastDebounceTime = 0;
  boolean reading = digitalRead(LS_UNLOCK);

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

void lampTest() {
  for(int i = 0; i < 4; i++) {
//    digitalWrite(LS_UNLOCK, HIGH);
    digitalWrite(LS_READY, HIGH);
//    digitalWrite(LS_CONT_0, HIGH);
//    digitalWrite(LS_CONT_1, HIGH);
    digitalWrite(LS_SELECT, HIGH);
    digitalWrite(LS_BUZZER_EN, HIGH); 
    delay(125);   
//    digitalWrite(LS_UNLOCK, LOW);
    digitalWrite(LS_READY, LOW);
//    digitalWrite(LS_CONT_0, LOW);
//    digitalWrite(LS_CONT_1, LOW);
    digitalWrite(LS_SELECT, LOW);
    digitalWrite(LS_BUZZER_EN, LOW);  
    delay(125);   
  }  
}

void buzzBuzzer() {
  static boolean buzzState = LOW;

  if(!canLaunch) {
    buzzState = LOW;
    digitalWrite(LS_BUZZER_EN, buzzState); 
    return;    
  }

// . Serial.print("Ready To Launch: "); Serial.println(controller_state.readyToLaunch);
  if(controller_state.readyToLaunch) {
    buzzState = HIGH;
    digitalWrite(LS_BUZZER_EN, buzzState);  
    return;   
  }
  
  if(controller_state.armed) {
   buzzState = !buzzState;
   digitalWrite(LS_BUZZER_EN, buzzState); 
  } else {
    buzzState = LOW;
    digitalWrite(LS_BUZZER_EN, buzzState); 
  }
}

void checkReady() {
  static boolean blinkState = LOW;

  if(!countReady) {
   blinkState = !blinkState;
   digitalWrite(LS_READY, blinkState); 
   digitalWrite(LS_SELECT, !blinkState);
  } else {
    blinkState = HIGH;
    digitalWrite(LS_READY, blinkState); 
  }   
}

void setAsTransmitter() {
  radio.stopListening();  // put radio in TX mode
}


void setAsReceiver() {
  radio.startListening();  // put radio in RX mode
}

void setup() {
  // put your setup code here, to run once:
  pinMode(LS_BUZZER_EN, OUTPUT);
  pinMode(LS_SELECT, OUTPUT);
  pinMode(LS_READY, OUTPUT);
  pinMode(LS_UNLOCK, INPUT);
  pinMode(LS_CONT_0, INPUT);
  pinMode(LS_CONT_1, INPUT);
  pinMode(LS_FIRE_0, OUTPUT);
  pinMode(LS_FIRE_1, OUTPUT);

  lampTest();

  Serial.begin(9600);
//  while (!Serial) {
//    // some boards need to wait to ensure access to serial over USB
//  }

  SPI.begin();

  if (!radio.begin()) {
    Serial.println(F("radio hardware not responding!"));
    while (1) {
        digitalWrite(LS_READY, HIGH);
        digitalWrite(LS_SELECT, HIGH);
        delay(250);
        digitalWrite(LS_READY, LOW);
        digitalWrite(LS_SELECT, LOW);
        delay(250);
      } // hold program in infinite loop to prevent subsequent errors
  }
  
  radio.setPALevel(RF24_PA_MAX);         // RF24_PA_MIN / RF24_PA_LOW / RF24_PA_HIGH / RF24_PA_MAX is default.
  radio.setDataRate(RF24_1MBPS);
  //radio.setPayloadSize(sizeof(STATE));  // default value is the maximum 32 bytes

  // set the TX address of the RX node into the TX pipe
  radio.openWritingPipe(address[CONTROL]);
  radio.openReadingPipe(1, address[BASE]);
//  radio.stopListening();  // put radio in TX mode
}


void loop() {
  unsigned long currentTime = millis();
  static unsigned long buzzerPreviousTime = currentTime;
  static unsigned long countReadyPreviousTime = currentTime;
  static unsigned long transmitPreviousTime = currentTime;
  static unsigned long firePreviousTime = currentTime;
  boolean radioReady = (countReady > 0);

  base_state.unlocked = debounceUnlockRead();
  base_state.cont_0 = digitalRead(LS_CONT_0);
  base_state.cont_1 = digitalRead(LS_CONT_1);

  // Update ReadyLED state every BLINKINTERVAL
  if((currentTime - countReadyPreviousTime) >= BLINKINTERVAL) {
    countReadyPreviousTime += BLINKINTERVAL;
    checkReady();
//    Serial.println(countReady); 
  }

  if(radioReady) {
    canLaunch = true;
  } else {
    canLaunch = false;
  }

  // Attempt to turn on the buzzer every BUZZINTERVAL
  if((currentTime - buzzerPreviousTime) >= BUZZINTERVAL) {
    buzzerPreviousTime += BUZZINTERVAL;
    buzzBuzzer();
  }

  // Transmit every 250ms a heartbeat packet to the base
  if((currentTime - transmitPreviousTime) >= TRANSMITINTERVAL) {
    transmitPreviousTime += TRANSMITINTERVAL;
    base_state.sequence++;
    setAsTransmitter();

    // These two lines are for debug only, and are used to test firing
    // never have these lines enabled when in operation
//    base_state.cont_0 = true;
//    base_state.cont_1 = true;

    bool rslt = radio.write(&base_state, sizeof(base_state_t));
    if (rslt) {
//      Serial.println("Transmit Success!");
      countReady = TRANSMITRETRY;
    } else {
//      Serial.println("Transmit Failed!");  
      if(countReady > 0) {
        countReady--;
      }
    }
    setAsReceiver();
  }

  // always try and receive data from the controller
  if ( radio.available() ) {
//    Serial.println("Data Received");
//    uint8_t bytes = radio.getPayloadSize(); // get the size of the payload
//    Serial.print("Payload Size: ");Serial.println(bytes);
    radio.read(&controller_state, sizeof(controller_state_t));            // fetch payload from FIFO

    // Update the output for the hardware to match the selected pad
    digitalWrite(LS_SELECT, !controller_state.select);

//    Serial.print("Controller Sequence: "); Serial.println(controller_state.sequence);

//    Serial.print(controller_state.armed); Serial.println(" :ARMED STATE");
//    Serial.print(controller_state.launch); Serial.println(" :LAUNCH STATE");
//    Serial.print(controller_state.select); Serial.println(" :SELECT STATE");
//  } else {
//    Serial.println("No Data");    
  } 

  // Check Fire status every FIREINTERVAL, if fire pin set, then the fire pin will be held for
  // FIREINTERVAL before changing the pin state again. Fire pin state is set by controller state
  if((currentTime - firePreviousTime) >= FIREINTERVAL) {
    firePreviousTime += FIREINTERVAL;
    digitalWrite(LS_FIRE_0, !controller_state.select & controller_state.launch & controller_state.readyToLaunch & canLaunch);
    digitalWrite(LS_FIRE_1, controller_state.select & controller_state.launch & controller_state.readyToLaunch & canLaunch);
  }

}
