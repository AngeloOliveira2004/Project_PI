#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>
#include <sys/time.h>

#define TRIG_PIN 509  // Replace with correct GPIO number for your Milk-V Duo
#define ECHO_PIN 508  // Replace with correct GPIO number

// Function to write GPIO value
void write_gpio(int pin, int value) {
    char path[50];
    snprintf(path, 50, "/sys/class/gpio/gpio%d/value", pin);
    int fd = open(path, O_WRONLY);
    if (fd < 0) {
        perror("Write GPIO");
        return;
    }
    dprintf(fd, "%d", value);
    close(fd);
}

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
    read(fd, &value, 1);
    close(fd);
    return value == '1' ? 1 : 0;
}

// Function to get the current time in microseconds
long get_microseconds() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (tv.tv_sec * 1000000) + tv.tv_usec;
}

// Function to measure distance
double measure_distance() {
    // Send 10µs pulse to TRIG
    write_gpio(TRIG_PIN, 0);
    usleep(2);
    write_gpio(TRIG_PIN, 1);
    usleep(10);
    write_gpio(TRIG_PIN, 0);

    long start_time = 0, end_time = 0;
    
    // Wait for ECHO to go HIGH (start timing)
    long timeout = get_microseconds() + 1000000; // 1s timeout
    while (read_gpio(ECHO_PIN) == 0) {
        if (get_microseconds() > timeout) {
            printf("Timeout waiting for ECHO HIGH\n");
            return -1; // Timeout occurred
        }
    }
    start_time = get_microseconds();

    // Wait for ECHO to go LOW (stop timing)
    timeout = get_microseconds() + 1000000; // 1s timeout
    while (read_gpio(ECHO_PIN) == 1) {
        if (get_microseconds() > timeout) {
            printf("Timeout waiting for ECHO LOW\n");
            return -1; // Timeout occurred
        }
    }
    end_time = get_microseconds();

    // Calculate distance (speed of sound = 34300 cm/s)
    double duration = (double)(end_time - start_time);
    if (duration <= 0) {
        return -1; // Invalid reading
    }
    double distance = (duration * 0.0343) / 2;
    return distance;
}

int main() {
    while (1) {
        double distance = measure_distance();
        if (distance > 0) {
            printf("Distance: %.2f cm\n", distance);
        } else {
            printf("Error measuring distance\n");
        }
        sleep(1);
    }
    return 0;
}
