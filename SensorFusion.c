#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define ALPHA 0.98  // Complementary filter coefficient
#define SAMPLE_RATE 0.01 // 10 ms

// Function prototypes
void calculate_roll_pitch_yaw(float accel[], float gyro[], float mag[], float *roll, float *pitch, float *yaw);

int main() {
    FILE *file = fopen("SensorDatasComma.csv", "r");
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

void calculate_roll_pitch_yaw(float accel[], float gyro[], float mag[], float *roll, float *pitch, float *yaw) {
    // Accelerometer-based calculations for roll and pitch
    float accel_roll = atan2(accel[1], sqrt(accel[0] * accel[0] + accel[2] * accel[2])) * (180.0 / M_PI);
    float accel_pitch = atan2(-accel[0], sqrt(accel[1] * accel[1] + accel[2] * accel[2])) * (180.0 / M_PI);

    // Integrate gyroscope data for roll and pitch
    *roll = ALPHA * (*roll + gyro[0] * SAMPLE_RATE) + (1 - ALPHA) * accel_roll;
    *pitch = ALPHA * (*pitch + gyro[1] * SAMPLE_RATE) + (1 - ALPHA) * accel_pitch;

    // Magnetometer-based yaw calculation
    float mag_x = mag[0] * cos(*pitch * M_PI / 180) + mag[2] * sin(*pitch * M_PI / 180);
    float mag_y = mag[0] * sin(*roll * M_PI / 180) * sin(*pitch * M_PI / 180) + mag[1] * cos(*roll * M_PI / 180) - mag[2] * sin(*roll * M_PI / 180) * cos(*pitch * M_PI / 180);
    *yaw = atan2(-mag_y, mag_x) * (180.0 / M_PI);
}
