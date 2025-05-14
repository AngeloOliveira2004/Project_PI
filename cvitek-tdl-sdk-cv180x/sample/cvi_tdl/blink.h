#ifndef BLINK_H
#define BLINK_H

#include <stdio.h>
#include <unistd.h>
// #include <wiringx.h>

// Frequencies in Hz
#define NOTE_C4  262
#define NOTE_D4  294
#define NOTE_E4  330
#define NOTE_G4  392
#define NOTE_C5  523
#define REST       0

typedef struct {
    int freq;
    int duration_ms;
    
} Note;

void play_note(int gpio, int freq, int duration_ms);
void setup_buzzer(int buzzer_pin);
void play_melody();
void clean_up_buzzer();

#endif
