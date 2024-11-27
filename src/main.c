#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> // usleep için
#include "RotateCube.h"
#include "SensorFusion.h"

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: %s <sensor_bitmap>\n", argv[0]);
        return 1;
    }

    int sensor_bitmap = strtol(argv[1], NULL, 2); // 9 bitlik sensör bitmapini al
    if (sensor_bitmap < 0 || sensor_bitmap > 0b111111111) {
        printf("Error: Sensor bitmap must be a 9-bit binary value (0-0b111111111).\n");
        return 1;
    }

    FILE *file = fopen("./Data/GY521_SensorDataSampleFormatted.csv", "r");
    if (file == NULL) {
        printf("Error: Unable to open the file.\n");
        return 1;
    }

    char line[1024]; // Daha büyük buffer çünkü satır uzun
    float gyro[3][3], accel[3][3], mag[3][3]; // Maksimum 3 sensör için dizi
    CubeRotation rotation = {0.0f, 0.0f, 0.0f}; // Tek birleşik dönüşüm hesaplanacak

    init_rotate_cube(argc, argv, &rotation);

    while (fgets(line, sizeof(line), file)) {
        // Tüm sensörlerin verilerini ayrıştır
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

        // Roll, pitch ve yaw hesapla
        calculate_roll_pitch_yaw(gyro, accel, mag, sensor_bitmap, &rotation.pitch, &rotation.roll, &rotation.yaw);

        // Roll, pitch, yaw değerlerini yazdır
        printf("Roll: %.2f, Pitch: %.2f, Yaw: %.2f\n", rotation.roll, rotation.pitch, rotation.yaw);

        // Roll, pitch ve yaw değerlerini güncelle
        update_rotation(rotation);

        // Bekleme süresi (100 ms)
        usleep(30000);
    }

    fclose(file);
    return 0;
}
