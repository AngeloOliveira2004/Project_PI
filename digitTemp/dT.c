#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

// Define the GPIO number
#define TEMP_SENSOR_GPIO 508  // Change this if needed

// Function to read GPIO value
int read_gpio(int pin) {
    char path[50];
    snprintf(path, 50, "/sys/class/gpio/gpio%d/value", pin);
    int fd = open(path, O_RDONLY);
    if (fd < 0) {
        perror("Read GPIO");
        return -1;
    }
    
    char value;
    if (read(fd, &value, 1) != 1) {
        perror("Read error");
        close(fd);
        return -1;
    }
    
    close(fd);
    return value == '1' ? 1 : 0;
}

int main() {
    while (1) {
        int temp_value = read_gpio(TEMP_SENSOR_GPIO);
        if (temp_value < 0) {
            printf("Error reading temperature sensor\n");
        } else {
            printf("Temperature Sensor Value: %d\n", temp_value);
        }
        sleep(1);
    }
    return 0;
}
