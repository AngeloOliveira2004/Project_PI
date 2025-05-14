#include <stdio.h>
#include <stdlib.h>  // For atoi()
#include <unistd.h>
#include <wiringx.h>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <0|1>\n", argv[0]);
        return 1;
    }

    int state = atoi(argv[1]);  // Convert argument to int
    if (state != 0 && state != 1) {
        fprintf(stderr, "Argument must be 0 or 1\n");
        return 1;
    }

    int DUO_LED = 1;

    if (wiringXSetup("milkv_duo", NULL) == -1) {
        wiringXGC();
        fprintf(stderr, "Failed to set up wiringX\n");
        return 1;
    }

    if (wiringXValidGPIO(DUO_LED) != 0) {
        fprintf(stderr, "Invalid GPIO %d\n", DUO_LED);
        return 1;
    }

    pinMode(DUO_LED, PINMODE_OUTPUT);

    if (state == 1) {
        printf("Setting GPIO %d: HIGH\n", DUO_LED);
        digitalWrite(DUO_LED, HIGH);
    } else {
        printf("Setting GPIO %d: LOW\n", DUO_LED);
        digitalWrite(DUO_LED, LOW);
    }

    return 0;
}