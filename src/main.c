#include <stdio.h>
#include <stdlib.h>
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

    // Skip the header line
    fgets(line, sizeof(line), file);

    // Read the data line by line
    while (fgets(line, sizeof(line), file)) {
        // Parse the line
        sscanf(line, "%f,%f,%f,%f,%f,%f,%f,%f,%f",
               &gyro[0], &gyro[1], &gyro[2],
               &accel[0], &accel[1], &accel[2],
               &mag[0], &mag[1], &mag[2]);

        // Calculate roll, pitch, and yaw
        calculate_roll_pitch_yaw(accel, gyro, mag, &roll, &pitch, &yaw);

        // Print the results
        printf("Roll: %.2f, Pitch: %.2f, Yaw: %.2f\n", roll, pitch, yaw);
    }

    fclose(file);
    return 0;
}
