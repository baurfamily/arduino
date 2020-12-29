/*
  Melody

  Plays Deck The Halls

  circuit:
  - 8 ohm speaker on digital pin 8
  - LEDs on pins 9 - 12

*/

#include "pitches.h"

#define AUDIO_OUT_PIN 8

// Duration of notes
#define SIXTENTH    16
#define DOTTED_S    12
#define EIGHTH      8
#define DOTTED_E    6
#define QUARTER     4
#define DOTTED_Q    3
#define HALF        2
#define WHOLE       1

struct note_t {
  int pitch;    // The freqency of the pitch, defined in pitches.h
  int duration; // how long to play the pitch for, 2 = half note, 4 - quater note, so on
  int led;      // if LEDs are used, what GPIO is used, use 0 for no LED
};

#define VERSE1_LENGTH 17

struct note_t verse1[] = {
  {NOTE_G4, DOTTED_Q, 9}, {NOTE_F4, EIGHTH, 10},  {NOTE_E4, QUARTER, 11}, {NOTE_D4, QUARTER, 12},
  {NOTE_C4, QUARTER, 9},  {NOTE_D4, QUARTER, 10}, {NOTE_E4, QUARTER, 11}, {NOTE_C4, QUARTER, 12},
  
  {NOTE_D4, EIGHTH, 9},   {NOTE_E4, EIGHTH, 10},  {NOTE_F4, EIGHTH, 11},  {NOTE_D4, EIGHTH, 12},
  {NOTE_E4, DOTTED_Q, 9}, {NOTE_D4, EIGHTH, 10},  {NOTE_C4, QUARTER, 11}, {NOTE_B3, QUARTER, 12},
  
  {NOTE_C4, HALF, 9}
};

#define VERSE2_LENGTH 34

struct note_t verse2[] = {
  {NOTE_D4, DOTTED_Q, 9}, {NOTE_E4, EIGHTH, 10}, {NOTE_F4, QUARTER, 11}, {NOTE_D4, QUARTER, 12},
  {NOTE_E4, DOTTED_Q, 9}, {NOTE_F4, EIGHTH, 10}, {NOTE_G4, QUARTER, 11}, {NOTE_D4, QUARTER, 12},
  
  {NOTE_E4, EIGHTH, 9}, {NOTE_F4, EIGHTH, 10},  {NOTE_G4, QUARTER, 11}, {NOTE_A4, EIGHTH, 12},
  {NOTE_B4, EIGHTH, 9}, {NOTE_C5, QUARTER, 10}, {NOTE_B4, QUARTER, 11}, {NOTE_A4, QUARTER, 12},
  
  {NOTE_G4, HALF, 9},    {NOTE_G4, DOTTED_Q, 10}, {NOTE_F4, EIGHTH, 11},  {NOTE_E4, QUARTER, 12},
  {NOTE_D4, QUARTER, 9}, {NOTE_C4, QUARTER, 10},  {NOTE_D4, QUARTER, 11}, {NOTE_E4, QUARTER, 12},
  
  {NOTE_C4, QUARTER, 9}, {NOTE_A4, EIGHTH, 10},   {NOTE_A4, EIGHTH, 11}, {NOTE_A4, EIGHTH, 12},
  {NOTE_A4, EIGHTH, 9},  {NOTE_G4, DOTTED_Q, 10}, {NOTE_F4, EIGHTH, 11}, {NOTE_E4, QUARTER, 12},

  {NOTE_D4, QUARTER, 9}, {NOTE_C4, HALF, 10}
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
  delay(2000);

  for (int k= 0; k < 2; k++) {
    play_phrase(verse1, VERSE1_LENGTH);

  }

  play_phrase(verse2, VERSE2_LENGTH);
}
