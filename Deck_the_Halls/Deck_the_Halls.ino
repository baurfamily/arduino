/*
  Melody

  Plays Deck The Halls

  circuit:
  - 8 ohm speaker on digital pin 8
  - LEDs on pins 9 - 12

*/

#include "pitches.h"

#define AUDIO_OUT_PIN 8

struct note_t {
  int pitch;    // The freqency of the pitch, defined in pitches.h
  int duration; // how long to play the pitch for, 2 = half note, 4 - quater note, so on
  int led;      // if LEDs are used, what GPIO is used, use 0 for no LED
};

#define VERSE1_LENGTH 17

struct note_t verse1[] = {
  {NOTE_G4, 3, 9}, {NOTE_F4, 8, 10}, {NOTE_E4, 4, 11}, {NOTE_D4, 4, 12},
  {NOTE_C4, 4, 9}, {NOTE_D4, 4, 10}, {NOTE_E4, 4, 11}, {NOTE_C4, 4, 12},
  {NOTE_D4, 8, 9}, {NOTE_E4, 8, 10}, {NOTE_F4, 8, 11}, {NOTE_D4, 8, 12},
  {NOTE_E4, 3, 9}, {NOTE_D4, 8, 10}, {NOTE_C4, 4, 11}, {NOTE_B3, 4, 12},
  {NOTE_C4, 2, 9}
};

#define VERSE2_LENGTH 34

struct note_t verse2[] = {
  {NOTE_D4, 3, 9}, {NOTE_E4, 8, 10}, {NOTE_F4, 4, 11}, {NOTE_D4, 4, 12},
  {NOTE_E4, 3, 9}, {NOTE_F4, 8, 10}, {NOTE_G4, 4, 11}, {NOTE_D4, 4, 12},
  
  {NOTE_E4, 8, 9}, {NOTE_F4, 8, 10}, {NOTE_G4, 4, 11}, {NOTE_A4, 8, 12},
  {NOTE_B4, 8, 9}, {NOTE_C5, 4, 10}, {NOTE_B4, 4, 11}, {NOTE_A4, 4, 12},
  
  {NOTE_G4, 2, 9}, {NOTE_G4, 3, 10}, {NOTE_F4, 8, 11}, {NOTE_E4, 4, 12},
  {NOTE_D4, 4, 9}, {NOTE_C4, 4, 10}, {NOTE_D4, 4, 11}, {NOTE_E4, 4, 12},
  
  {NOTE_C4, 4, 9}, {NOTE_A4, 8, 10}, {NOTE_A4, 8, 11}, {NOTE_A4, 8, 12},
  {NOTE_A4, 8, 9}, {NOTE_G4, 3, 10}, {NOTE_F4, 8, 11}, {NOTE_E4, 4, 12},

  {NOTE_D4, 4, 9}, {NOTE_C4, 2, 10}

};

void play_phrase(note_t *phrase, int total_notes) {
  for (int aNote = 0; aNote < total_notes; aNote++) {
    if (phrase->led > 0)
      digitalWrite(phrase->led, HIGH);
      
    // to calculate the note duration, take one second divided by the note type.
    //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
    int noteDuration = 1000 / phrase->duration;
    tone(AUDIO_OUT_PIN, phrase->pitch, noteDuration);

    // to distinguish the notes, set a minimum time between them.
    // the note's duration + 30% seems to work well:
    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);
    // stop the tone playing:
    noTone(AUDIO_OUT_PIN);
    if (phrase->led > 0)
      digitalWrite(phrase->led, LOW);
    phrase++;
  }
}

void setup() {
  pinMode(AUDIO_OUT_PIN, OUTPUT);
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
    play_phrase(verse1, VERSE1_LENGTH);

  }

  play_phrase(verse2, VERSE2_LENGTH);
}

// notes in the melody:
//int melody1[] = {
//  NOTE_G4, NOTE_F4, NOTE_E4, NOTE_D4, NOTE_C4, NOTE_D4, NOTE_E4, NOTE_C4,
//  NOTE_D4, NOTE_E4, NOTE_F4, NOTE_D4, NOTE_E4, NOTE_D4, NOTE_C4, NOTE_B3, NOTE_C4
//};
//
//int melody2[] = {
//  NOTE_D4, NOTE_E4, NOTE_F4, NOTE_D4, NOTE_E4, NOTE_F4, NOTE_G4, NOTE_D4,
//  NOTE_E4, NOTE_F4, NOTE_G4, NOTE_A4, NOTE_B4, NOTE_C5, NOTE_B4, NOTE_A4,
//  NOTE_G4, NOTE_G4, NOTE_F4, NOTE_E4, NOTE_D4, NOTE_C4, NOTE_D4, NOTE_E4,
//  NOTE_C4, NOTE_A4, NOTE_A4, NOTE_A4, NOTE_A4, NOTE_G4, NOTE_F4, NOTE_E4,
//  NOTE_D4, NOTE_C4
//};
//
//// note durations: 4 = quarter note, 8 = eighth note, etc.:
//int noteDurations1[] = {
//  3, 8, 4, 4, 4, 4, 4, 4,
//  8, 8, 8, 8, 3, 8, 4, 4,
//  2
//};
//
//int noteDurations2[] = {
//  3, 8, 4, 4, 3, 8, 4, 4,
//  8, 8, 4, 8, 8, 4, 4, 4,
//  2, 3, 8, 4, 4, 4, 4, 4,
//  4, 8, 8, 8, 8, 3, 8, 4, 4, 2
//};

//    for (int thisNote = 0; thisNote < 17; thisNote++) {
//      digitalWrite(i, HIGH);   // turn the LED on (HIGH is the voltage level)
//  
//      // to calculate the note duration, take one second divided by the note type.
//      //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
//      int noteDuration = 1000 / noteDurations1[thisNote];
//      tone(8, melody1[thisNote], noteDuration);
//  
//      // to distinguish the notes, set a minimum time between them.
//      // the note's duration + 30% seems to work well:
//      int pauseBetweenNotes = noteDuration * 1.30;
//      delay(pauseBetweenNotes);
//      // stop the tone playing:
//      noTone(8);
//  
//      digitalWrite(i, LOW);   // turn the LED on (HIGH is the voltage level) 
//      i++;
//      if(i > 12) i = 9;   
//    }
