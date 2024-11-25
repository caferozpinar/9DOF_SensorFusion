#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> // usleep için
#include "RotateCube.h"
#include "SensorFusion.h"

int main() {
    FILE *file = fopen("./Data/SensorDatas.csv", "r");
    if (file == NULL) {
        printf("Error: Unable to open the file.\n");
        return 1;
    }

    char line[256];
    float gyro[3], accel[3], mag[3];
    float roll = 0, pitch = 0, yaw = 0;

    // Başlık satırını atla
    fgets(line, sizeof(line), file);

    // Veri satırlarını okuyarak işleme başla
    while (fgets(line, sizeof(line), file)) {
        // Satırı ayrıştır
        sscanf(line, "%f,%f,%f,%f,%f,%f,%f,%f,%f",
               &gyro[0], &gyro[1], &gyro[2],
               &accel[0], &accel[1], &accel[2],
               &mag[0], &mag[1], &mag[2]);

        // Roll, pitch ve yaw hesapla
        calculate_roll_pitch_yaw(accel, gyro, mag, &roll, &pitch, &yaw);

        // Ekranda roll, pitch ve yaw değerlerini göster
        rotate_cube(roll, pitch, yaw);

        // Bekleme süresi (100 ms)
        usleep(100000);
    }

    fclose(file);
    return 0;
}
