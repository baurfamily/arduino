/*
 * Simon Says
 * 
 * Based on the classic electronic game of Simon, this is a replication
 * of that game.  Currently there is only one game mode, pressing the
 * blue button will start the standard mode, and it supports up to 256
 * sequences.
 * 
 * The challenge for this project was to use as few of pins as possible.
 * To do this, I wired the circuit such that the pin is used for both an
 * input and an outout, and the pin mode is switched based on what the
 * current need for the pin is.
 */
#include "pitches.h"

#define AUDIO_OUT_PIN 5

#define BLUE_PIN    9
#define RED_PIN     10
#define YELLOW_PIN  11
#define GREEN_PIN   12

#define BLUE_PITCH NOTE_G4
#define RED_PITCH NOTE_B4
#define YELLOW_PITCH NOTE_D4
#define GREEN_PITCH NOTE_F4
#define FAIL_PITCH NOTE_B1
#define START_TONE NOTE_B5

#define DURATION 250
#define TIMEOUT 3000    //  3s timeout waiting for a button press

enum lights {
  BLUE = 0,
  RED,
  YELLOW,
  GREEN,
  UNDEFINED
};

byte light_sequence[256];
static int entries = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  randomSeed(analogRead(0));
}

/*
 * watchButton
 * 
 * Looks for a button to be pressed, and if so, send out the audio out for 
 * that button
 * 
 * returns: the button that was pressed
 */
int watchButtons() {
  // the following variables are unsigned longs because the time, measured in
  // milliseconds, will quickly become a bigger number than can be stored in an int.
  unsigned long lastDebounceTime = 0;  // the last time the output pin was toggled
  unsigned long debounceDelay = 50;    // the debounce time; increase if the output flickers
  unsigned long timeoutTimeStart = millis();  // set the starting timeout time;
  unsigned long timeoutTime = millis() + TIMEOUT;
  bool buttonPressed = false;
  bool buttonReleased = false;

  byte lastButton = UNDEFINED;
  byte currentButton = UNDEFINED;
  byte blueButtonState = LOW;
  byte redButtonState = LOW;
  byte greenButtonState = LOW;
  byte yellowButtonState = LOW;

  int blueReading = LOW;
  int redReading = LOW;
  int greenReading = LOW;
  int yellowReading = LOW;

  Serial.println("watchButton: Entry");

  while(millis() < timeoutTime) {

    // read all four button inputs
    blueReading = digitalRead(BLUE_PIN);
    redReading = digitalRead(RED_PIN);
    greenReading = digitalRead(GREEN_PIN);
    yellowReading = digitalRead(YELLOW_PIN);

    // check if any of the button inputs changes state
    if(blueReading != blueButtonState) {  // Blue button state changed
      Serial.println("pressed blue");
      Serial.println(blueReading);
      blueButtonState = blueReading;
      timeoutTime = millis() + TIMEOUT;
      lastDebounceTime = millis();        // resent the debounce timer
      if(blueReading) // button pressed   // if the blue button is high, then set it as pressed
        currentButton = BLUE;
    }

    if(redReading != redButtonState) {  // Red button state changed
      Serial.println("pressed red");
      Serial.println(redReading);
      redButtonState = redReading;
      timeoutTime = millis() + TIMEOUT;
      lastDebounceTime = millis();        // resent the debounce timer
      if(redReading) // button pressed   // if the red button is high, then set it as pressed
        currentButton = RED;
    }

    if(greenReading != greenButtonState) {  // Red button state changed
      Serial.println("pressed green");
      Serial.println(greenReading);
      greenButtonState = greenReading;
      timeoutTime = millis() + TIMEOUT;
      lastDebounceTime = millis();        // resent the debounce timer
      if(greenReading) // button pressed   // if the red button is high, then set it as pressed
        currentButton = GREEN;
    }

    if(yellowReading != yellowButtonState) {  // Red button state changed
      Serial.println("pressed yellow");
      Serial.println(yellowReading);
      yellowButtonState = yellowReading;
      timeoutTime = millis() + TIMEOUT;
      lastDebounceTime = millis();        // resent the debounce timer
      if(yellowReading) // button pressed   // if the red button is high, then set it as pressed
        currentButton = YELLOW;
    }

    // check to see if the debounce time has been reached
    if ((millis() - lastDebounceTime) > debounceDelay) {
        // treat the button as a state change
        switch(currentButton) {
          case BLUE :
            if (blueReading) {  // blue button being pressed, so tone the right tone
              tone(AUDIO_OUT_PIN, BLUE_PITCH);
              buttonPressed = true;
            } else { // BLUE button had been pressed, and now is released
              noTone(AUDIO_OUT_PIN);
              buttonReleased = true;
            }
            break;
          case RED :
            if (redReading) {  // blue button being pressed, so tone the right tone
              tone(AUDIO_OUT_PIN, RED_PITCH);
              buttonPressed = true;
            } else { // RED button had been pressed, and now is released
              noTone(AUDIO_OUT_PIN);
              buttonReleased = true;
            }
            break;
          case GREEN :
            if (greenReading) {  // blue button being pressed, so tone the right tone
              tone(AUDIO_OUT_PIN, GREEN_PITCH);
              buttonPressed = true;
            } else { // GREEN button had been pressed, and now is released
              noTone(AUDIO_OUT_PIN);
              buttonReleased = true;
            }
            break;
          case YELLOW :
            if (yellowReading) {  // blue button being pressed, so tone the right tone
              tone(AUDIO_OUT_PIN, YELLOW_PITCH);
              buttonPressed = true;
            } else { // YELLOW button had been pressed, and now is released
              noTone(AUDIO_OUT_PIN);
              buttonReleased = true;
            }
            break;
          default :
            noTone(AUDIO_OUT_PIN);
        }
    }

    if(buttonPressed && buttonReleased) {
      break;  // get out of the while loop waiting for the timout as a button has been cycled and debounce
    }

    lastButton = currentButton;  
  }

  Serial.println("watchButton: Exit");
  return currentButton;
}


int readSequence () {
  int button;
  static int lastBlue, lastRed, lastYellow, lastGreen;
  bool okay = true;
  int index = 0;

  Serial.println("readSequence");

  pinMode(BLUE_PIN, INPUT);
  pinMode(RED_PIN, INPUT);
  pinMode(YELLOW_PIN, INPUT);
  pinMode(GREEN_PIN, INPUT);

  while (okay && (index < entries)) {
    // watch for a button press
    button = watchButtons();

    // check to see if the button that was pressed is the right one for the current index
    if(button != light_sequence[index++]) {
      okay = false;
      break;
    }
  }

  return okay;
}

void outputSequence (int duration) {
  pinMode(BLUE_PIN, OUTPUT);
  pinMode(RED_PIN, OUTPUT);
  pinMode(YELLOW_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);

  for (int i = 0; i < entries; i++) {
    delay(duration);
    switch (light_sequence[i]) {
      case BLUE :
        Serial.println("Blue");
        digitalWrite(BLUE_PIN, HIGH);
        tone(AUDIO_OUT_PIN, BLUE_PITCH);
        delay(duration);
        noTone(AUDIO_OUT_PIN);
        digitalWrite(BLUE_PIN, LOW);
        break;
      case RED :
        Serial.println("Red");
        digitalWrite(RED_PIN, HIGH);
        tone(AUDIO_OUT_PIN, RED_PITCH);
        delay(duration);
        noTone(AUDIO_OUT_PIN);
        digitalWrite(RED_PIN, LOW);
        break;
      case YELLOW :
        Serial.println("Yellow");
        digitalWrite(YELLOW_PIN, HIGH);
        tone(AUDIO_OUT_PIN, YELLOW_PITCH);
        delay(duration);
        noTone(AUDIO_OUT_PIN);
        digitalWrite(YELLOW_PIN, LOW);
        break;
      case GREEN :
        Serial.println("Green");
        digitalWrite(GREEN_PIN, HIGH);
        tone(AUDIO_OUT_PIN, GREEN_PITCH);
        delay(duration);
        noTone(AUDIO_OUT_PIN);
        digitalWrite(GREEN_PIN, LOW);
        break;
    }
  }
}

void resetSequence() {
  entries = 0;
}

void addEntry() {
  // picking a new entry is simple just picking a new random number between 0 and 3
  light_sequence[entries++] = random(4);
  Serial.println(light_sequence[entries-1]);
}

void failed() {
  Serial.println("Failed");
  pinMode(BLUE_PIN, OUTPUT);
  pinMode(RED_PIN, OUTPUT);
  pinMode(YELLOW_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  
  tone(AUDIO_OUT_PIN, FAIL_PITCH);
  for(int i = 0; i < 8; i++) {
    digitalWrite(BLUE_PIN, HIGH);
    digitalWrite(RED_PIN, HIGH);
    digitalWrite(GREEN_PIN, HIGH);
    digitalWrite(YELLOW_PIN, HIGH);
    delay(125);
    digitalWrite(BLUE_PIN, LOW);
    digitalWrite(RED_PIN, LOW);
    digitalWrite(GREEN_PIN, LOW);
    digitalWrite(YELLOW_PIN, LOW);
    delay(125);
  }
  noTone(AUDIO_OUT_PIN);
  outputSequence(250);  
}

void startWait() {
  int wait = true;
  unsigned long timeoutTime;

  for (int i = 0; i < 5; i++) {
    pinMode(GREEN_PIN, OUTPUT);
    digitalWrite(GREEN_PIN, HIGH);
    tone(AUDIO_OUT_PIN, START_TONE);
    delay(125);
    digitalWrite(GREEN_PIN, LOW);
    noTone(AUDIO_OUT_PIN);
    delay(125);    
  }
    
  while(wait) {
    pinMode(GREEN_PIN, OUTPUT);
    pinMode(BLUE_PIN, INPUT);
    
    digitalWrite(GREEN_PIN, HIGH);
    timeoutTime = millis() + 500;
    while(millis() < timeoutTime) {
      if(digitalRead(BLUE_PIN)) {
        wait = false;
        break;
      }
    }
    
    digitalWrite(GREEN_PIN, LOW);
    
    if(!wait)
      break;
      
    timeoutTime = millis() + 500;
    while(millis() < timeoutTime) {
      if(digitalRead(BLUE_PIN)) {
        wait = false;
        break;
      }
    }
  }
  while(digitalRead(BLUE_PIN)) {  // wait for button to be released to start
//    Serial.println("Waiting");
    tone(AUDIO_OUT_PIN, START_TONE);
  }
  noTone(AUDIO_OUT_PIN);
}

void loop() {
  int okay;
  int duration;
  // put your main code here, to run repeatedly:
  startWait();
  
  resetSequence();
  okay = true;
  duration = DURATION;
  
  while(okay) {
    addEntry();
    delay(500);
    outputSequence(duration);
//    delay(2000);
    okay = readSequence();
//    delay(1000);
    if((entries % 5) == 0) {
      duration -= 25;
    }
  }
  // fell out of loop, do fail
  failed();
  delay(250);
}
