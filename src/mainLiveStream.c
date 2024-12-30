#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> // usleep için
#include <fcntl.h>  // open, O_RDWR, O_NOCTTY, O_SYNC
#include <termios.h> // Seri port ayarları için
#include <string.h>
#include "RotateCube.h"
#include "SensorFusion.h"

#define SERIAL_PORT "/dev/ttyUSB0" // Arduino'nun bağlı olduğu port
#define BAUD_RATE B115200
#define SLEEP_TIME_US 30000 // 30 ms uyku süresi
#define MAX_DATA_LENGTH 256

int main(int argc, char *argv[]) {
    // Girdi doğrulaması
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <sensor_bitmap>\n", argv[0]);
        return EXIT_FAILURE;
    }

    int sensor_bitmap = strtol(argv[1], NULL, 2); // 9 bitlik sensör bitmapini al
    if (sensor_bitmap < 0 || sensor_bitmap > 0b111111111) {
        fprintf(stderr, "Error: Sensor bitmap must be a 9-bit binary value (0-0b111111111).\n");
        return EXIT_FAILURE;
    }

    // Seri port ayarları
    int serial_fd = open(SERIAL_PORT, O_RDWR | O_NOCTTY | O_SYNC);
    if (serial_fd == -1) {
        perror("Seri port açılamadı");
        return EXIT_FAILURE;
    }

    struct termios tty;
    if (tcgetattr(serial_fd, &tty) != 0) {
        perror("Seri port ayarları okunamadı");
        return EXIT_FAILURE;
    }

    cfsetospeed(&tty, BAUD_RATE);
    cfsetispeed(&tty, BAUD_RATE);
    tty.c_cflag = (tty.c_cflag & ~CSIZE) | CS8; // 8-bit karakter
    tty.c_iflag &= ~IGNBRK; // Break işaretlerini işleme
    tty.c_lflag = 0;        // Canonical olmayan mod
    tty.c_oflag = 0;        // Raw output
    tty.c_cc[VMIN] = 0;     // Minimum 0 karakter oku
    tty.c_cc[VTIME] = 1;    // Timeout: 0.1 saniye

    tcsetattr(serial_fd, TCSANOW, &tty);

    char buffer[MAX_DATA_LENGTH + 1]; // Gelen veri için tampon
    char *line;
    float gyro[3][3] = {0}, accel[3][3] = {0}, mag[3][3] = {0}; // Maksimum 3 sensör için
    CubeRotation rotation = {0.0f, 0.0f, 0.0f};

    init_sensor_fusion();
    init_rotate_cube(argc, argv, &rotation);

    printf("Canlı Veri Gösterimi Başlıyor...\n");

    while (1) {
        int n = read(serial_fd, buffer, MAX_DATA_LENGTH);
        if (n > 0) {
            buffer[n] = '\0'; // Gelen veriyi sonlandır

            line = strtok(buffer, "\n");
            while (line != NULL) {
                // Verileri ayrıştır
                sscanf(line,
                       "%f,%f,%f,%f,%f,%f,%f,%f,%f,"  // Sensor 1
                       "%f,%f,%f,%f,%f,%f,%f,%f,%f,"  // Sensor 2
                       "%f,%f,%f,%f,%f,%f,%f,%f,%f",  // Sensor 3
                       &gyro[0][0], &gyro[0][1], &gyro[0][2],
                       &accel[0][0], &accel[0][1], &accel[0][2],
                       &mag[0][0], &mag[0][1], &mag[0][2],
                       &gyro[1][0], &gyro[1][1], &gyro[1][2],
                       &accel[1][0], &accel[1][1], &accel[1][2],
                       &mag[1][0], &mag[1][1], &mag[1][2],
                       &gyro[2][0], &gyro[2][1], &gyro[2][2],
                       &accel[2][0], &accel[2][1], &accel[2][2],
                       &mag[2][0], &mag[2][1], &mag[2][2]);

                calculate_roll_pitch_yaw(gyro, accel, mag, sensor_bitmap, &rotation.pitch, &rotation.roll, &rotation.yaw);

                printf("Roll: %.2f, Pitch: %.2f, Yaw: %.2f\n", rotation.roll, rotation.pitch, rotation.yaw);

                update_rotation(rotation);

                line = strtok(NULL, "\n");
            }
        }
        usleep(SLEEP_TIME_US);
    }

    close(serial_fd);
    return EXIT_SUCCESS;
}
