#include "SensorFusion.h"
#include <math.h>

// Updated function to calculate roll, pitch, and yaw
void calculate_roll_pitch_yaw(float gyro[3][3], float accel[3][3], float mag[3][3], int sensor_bitmap,
                              float *roll, float *pitch, float *yaw) {
    float fused_gyro[3] = {0.0f, 0.0f, 0.0f};
    float fused_accel[3] = {0.0f, 0.0f, 0.0f};
    float fused_mag[3] = {0.0f, 0.0f, 0.0f};
    int active_gyro_count = 0, active_accel_count = 0, active_mag_count = 0;

    // Fuse gyro data
    for (int i = 0; i < 3; i++) {
        if (sensor_bitmap & (1 << (8 - i))) { // Gyro bitmap kontrolü (ilk 3 bit)
            for (int j = 0; j < 3; j++) {
                fused_gyro[j] += gyro[i][j];
            }
            active_gyro_count++;
        }
    }
    if (active_gyro_count > 0) {
        for (int j = 0; j < 3; j++) {
            fused_gyro[j] /= active_gyro_count;
        }
    }

    // Fuse accel data
    for (int i = 0; i < 3; i++) {
        if (sensor_bitmap & (1 << (5 - i))) { // Accel bitmap kontrolü (orta 3 bit)
            for (int j = 0; j < 3; j++) {
                fused_accel[j] += accel[i][j];
            }
            active_accel_count++;
        }
    }
    if (active_accel_count > 0) {
        for (int j = 0; j < 3; j++) {
            fused_accel[j] /= active_accel_count;
        }
    }

    // Fuse mag data
    for (int i = 0; i < 3; i++) {
        if (sensor_bitmap & (1 << (2 - i))) { // Mag bitmap kontrolü (son 3 bit)
            for (int j = 0; j < 3; j++) {
                fused_mag[j] += mag[i][j];
            }
            active_mag_count++;
        }
    }
    if (active_mag_count > 0) {
        for (int j = 0; j < 3; j++) {
            fused_mag[j] /= active_mag_count;
        }
    }

    // Roll and pitch calculations
    float accel_roll = atan2(fused_accel[1], sqrt(fused_accel[0] * fused_accel[0] + fused_accel[2] * fused_accel[2])) * (180.0 / M_PI);
    float accel_pitch = atan2(-fused_accel[0], sqrt(fused_accel[1] * fused_accel[1] + fused_accel[2] * fused_accel[2])) * (180.0 / M_PI);

    *roll = accel_roll;
    *pitch = accel_pitch;

    // Yaw calculation using fused mag data
    if (active_mag_count > 0) {
        float mag_x = fused_mag[0] * cos(*pitch * M_PI / 180) + fused_mag[2] * sin(*pitch * M_PI / 180);
        float mag_y = fused_mag[0] * sin(*roll * M_PI / 180) * sin(*pitch * M_PI / 180) +
        fused_mag[1] * cos(*roll * M_PI / 180) -
        fused_mag[2] * sin(*roll * M_PI / 180) * cos(*pitch * M_PI / 180);
        *yaw = atan2(-mag_y, mag_x) * (180.0 / M_PI);
    } else {
        *yaw = 0.0f; // Default if no mag data
    }
                              }
