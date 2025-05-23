#include <wiringx.h>
#include <unistd.h>
#include <stdio.h>

int main() {
    int buzzer_pin = 1; // Replace with actual GPIO if different

    if (wiringXSetup("milkv_duo", NULL) == -1) {
        printf("Failed to setup WiringX.\n");
        return -1;
    }

    pinMode(buzzer_pin, PINMODE_OUTPUT);

    // Turn it ON (Beep)
    digitalWrite(buzzer_pin, HIGH);
    sleep(1);

    // Turn it OFF
    digitalWrite(buzzer_pin, LOW);
    sleep(1);

    // Optional: beep pattern
    for (int i = 0; i < 3; i++) {
        digitalWrite(buzzer_pin, HIGH);
        usleep(200000); // 200ms
        digitalWrite(buzzer_pin, LOW);
        usleep(200000);
    }

    wiringXGC(); // clean up
    return 0;
}
