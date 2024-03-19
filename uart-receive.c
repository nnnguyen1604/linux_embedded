#include <stdio.h>
#include <string.h>
#include <errno.h>

#include <wiringPi.h>
#include <wiringSerial.h>

int main() {
    int serial_port;
    char dat;
    FILE *file_ptr; // File pointer for writing data
    
    // Open serial port
    if ((serial_port = serialOpen("/dev/ttyS0", 9600)) < 0) {
        fprintf(stderr, "Unable to open serial device: %s\n", strerror(errno));
        return 1;
    }

    // Initialize wiringPi
    if (wiringPiSetup() == -1) {
        fprintf(stdout, "Unable to start wiringPi: %s\n", strerror(errno));
        return 1;
    }

    // Open file for writing
    file_ptr = fopen("data.txt", "w");
    if (file_ptr == NULL) {
        fprintf(stderr, "Unable to open file: %s\n", strerror(errno));
        return 1;
    }

    // Main loop to read from serial port
    while (1) {
        if (serialDataAvail(serial_port)) {
            dat = serialGetchar(serial_port); // Receive character serially
            printf("%c", dat);
            fflush(stdout);
            serialPutchar(serial_port, dat); // Transmit character serially on port
            
            // Write received character to file
            fprintf(file_ptr, "%c", dat);
            fflush(file_ptr); // Flush buffer to ensure data is written immediately
        }
    }

    fclose(file_ptr); // Close file
    return 0;
}
