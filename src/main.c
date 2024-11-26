#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> // usleep için
#include "RotateCube.h"
#include "SensorFusion.h"

int main() {
    FILE *file = fopen("./Data/SensorDataSample_2.csv", "r");
    if (file == NULL) {
        printf("Error: Unable to open the file.\n");
        return 1;
    }

    char line[256];
    float gyro[3], accel[3], mag[3];
    CubeRotation rotation = {0.0f, 0.0f, 0.0f};

    int argc = 1;
    char *argv[1] = {(char *)"OpenGL"};
    init_rotate_cube(argc, argv, &rotation);
    // Veri satırlarını okuyarak işleme başla
    while (fgets(line, sizeof(line), file)) {
        // Satırı ayrıştır
        sscanf(line, "%f,%f,%f,%f,%f,%f,%f,%f,%f",
               &gyro[0], &gyro[1], &gyro[2],
               &accel[0], &accel[1], &accel[2],
               &mag[0], &mag[1], &mag[2]);

        // Roll, pitch ve yaw hesapla
        calculate_roll_pitch_yaw(accel, gyro, mag, &rotation.roll, &rotation.pitch, &rotation.yaw);

        // Roll, pitch, yaw değerlerini yazdır
        printf("Roll: %.2f, Pitch: %.2f, Yaw: %.2f\n", rotation.roll, rotation.pitch, rotation.yaw);

        // Roll, pitch ve yaw değerlerini güncelle
        update_rotation(rotation);

        // Bekleme süresi (100 ms)
        usleep(10000);
    }

    fclose(file);
    return 0;
}
