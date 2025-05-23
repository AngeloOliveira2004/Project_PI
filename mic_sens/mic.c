#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

// Define the GPIO pin for the microphone sensor
#define MIC_PIN 509  // OUT pin of the microphone sensor

// Function to read GPIO value
int read_gpio(int pin) {
    char path[50];
    snprintf(path, 50, "/sys/class/gpio/gpio%d/value", pin);
    
    int fd = open(path, O_RDONLY);
    if (fd < 0) {
        perror("Error reading GPIO");
        return -1;
    }

    char value;
    read(fd, &value, 1);
    close(fd);

    return (value == '1') ? 1 : 0;
}

// Function to initialize GPIO
void setup_gpio(int pin) {
    char path[50];
    
    // Export GPIO if not already exported
    snprintf(path, 50, "/sys/class/gpio/export");
    int fd = open(path, O_WRONLY);
    if (fd >= 0) {
        dprintf(fd, "%d", pin);
        close(fd);
    }

    // Set GPIO direction to input
    snprintf(path, 50, "/sys/class/gpio/gpio%d/direction", pin);
    fd = open(path, O_WRONLY);
    if (fd >= 0) {
        write(fd, "in", 2);
        close(fd);
    }
}

int main() {
    // Setup the microphone sensor GPIO
    setup_gpio(MIC_PIN);
    
    printf("Reading microphone sensor (GPIO %d)...\n", MIC_PIN);

    while (1) {
        int sound_detected = read_gpio(MIC_PIN);
        
        if (sound_detected == 0) {
            printf("Sound detected!\n");
        } else {
            printf("No sound detected.\n");
        }

        usleep(500000); // Wait 500ms
    }

    return 0;
}
