/*
  Melody

  Plays Deck The Halls

  circuit:
  - 8 ohm speaker on digital pin 8
  - LEDs on pins 9 - 12

*/

#include "pitches.h"

// notes in the melody:
int melody1[] = {
  NOTE_G4, NOTE_F4, NOTE_E4, NOTE_D4, NOTE_C4, NOTE_D4, NOTE_E4, NOTE_C4,
  NOTE_D4, NOTE_E4, NOTE_F4, NOTE_D4, NOTE_E4, NOTE_D4, NOTE_C4, NOTE_B3, NOTE_C4
};

int melody2[] = {
  NOTE_D4, NOTE_E4, NOTE_F4, NOTE_D4, NOTE_E4, NOTE_F4, NOTE_G4, NOTE_D4,
  NOTE_E4, NOTE_F4, NOTE_G4, NOTE_A4, NOTE_B4, NOTE_C5, NOTE_B4, NOTE_A4,
  NOTE_G4, NOTE_G4, NOTE_F4, NOTE_E4, NOTE_D4, NOTE_C4, NOTE_D4, NOTE_E4,
  NOTE_C4, NOTE_A4, NOTE_A4, NOTE_A4, NOTE_A4, NOTE_G4, NOTE_F4, NOTE_E4,
  NOTE_D4, NOTE_C4
};

// note durations: 4 = quarter note, 8 = eighth note, etc.:
int noteDurations1[] = {
  3, 8, 4, 4, 4, 4, 4, 4, 8, 8, 8, 8, 3, 8, 4, 4, 2
};

int noteDurations2[] = {
  3, 8, 4, 4, 3, 8, 4, 4,
  8, 8, 4, 8, 8, 4, 4, 4,
  2, 3, 8, 4, 4, 4, 4, 4,
  4, 8, 8, 8, 8, 3, 8, 4, 4, 2
};

void setup() {
  // iterate over the notes of the melody:
  pinMode(12, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(9, OUTPUT);
}

void loop() {
  // no need to repeat the melody.
  static int i = 9;

  delay(2000);

  for (int k= 0; k < 2; k++) {
    for (int thisNote = 0; thisNote < 17; thisNote++) {
      digitalWrite(i, HIGH);   // turn the LED on (HIGH is the voltage level)
  
      // to calculate the note duration, take one second divided by the note type.
      //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
      int noteDuration = 1000 / noteDurations1[thisNote];
      tone(8, melody1[thisNote], noteDuration);
  
      // to distinguish the notes, set a minimum time between them.
      // the note's duration + 30% seems to work well:
      int pauseBetweenNotes = noteDuration * 1.30;
      delay(pauseBetweenNotes);
      // stop the tone playing:
      noTone(8);
  
      digitalWrite(i, LOW);   // turn the LED on (HIGH is the voltage level) 
      i++;
      if(i > 12) i = 9;   
    }
  }

  for (int thisNote = 0; thisNote < 34; thisNote++) {
    digitalWrite(i, HIGH);   // turn the LED on (HIGH is the voltage level)

    // to calculate the note duration, take one second divided by the note type.
    //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
    int noteDuration = 1000 / noteDurations2[thisNote];
    tone(8, melody2[thisNote], noteDuration);

    // to distinguish the notes, set a minimum time between them.
    // the note's duration + 30% seems to work well:
    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);
    // stop the tone playing:
    noTone(8);
    
    digitalWrite(i, LOW);   // turn the LED on (HIGH is the voltage level) 
    i++;
    if(i > 12) i = 9;   
  }
}
