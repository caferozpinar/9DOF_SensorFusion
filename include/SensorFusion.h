#ifndef SENSORFUSION_H
#define SENSORFUSION_H

#include <math.h>

void calculate_roll_pitch_yaw(float gyro[3][3], float accel[3][3], float mag[3][3], int sensor_bitmap, float *roll, float *pitch, float *yaw);

#endif
