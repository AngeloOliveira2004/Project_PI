#include "blink.h"

static int buzzer_pin_static = 0;

Note melody[] = {
    { NOTE_C4, 300 },
    { NOTE_D4, 300 },
    { NOTE_E4, 300 },
    { REST,    150 },
    { NOTE_C5, 500 },
    { NOTE_G4, 300 },
    { NOTE_C4, 300 },
};

#define NUM_NOTES (sizeof(melody) / sizeof(Note))

void play_note(int gpio, int freq, int duration_ms) {
    if (freq == 0) {
        usleep(duration_ms * 1000);
        return;
    }

    int period_us = 1000000 / freq;
    int cycles = (duration_ms * 1000) / period_us;

    for (int i = 0; i < cycles; i++) {
        digitalWrite(gpio, HIGH);
        usleep(period_us / 2);
        digitalWrite(gpio, LOW);
        usleep(period_us / 2);
    }
}

void setup_buzzer(int buzzer_pin) {
    wiringXSetup("milkv_duo", NULL);
    pinMode(buzzer_pin, PINMODE_OUTPUT);
    digitalWrite(buzzer_pin, LOW);
    buzzer_pin_static = buzzer_pin;
}

void play_melody() {
    for (int i = 0; i < NUM_NOTES; i++) {
        play_note(buzzer_pin_static, melody[i].freq, melody[i].duration_ms);
        usleep(100000);
    }
}

void clean_up_buzzer(){
    wiringXGC();
}
