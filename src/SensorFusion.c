#include "SensorFusion.h"
#include <math.h>

// Function to calculate roll, pitch, and yaw
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
