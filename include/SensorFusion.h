#ifndef SENSORFUSION_H
#define SENSORFUSION_H

#define ALPHA 0.98  // Complementary filter coefficient
#define SAMPLE_RATE 0.01 // 10 ms

// Function prototype
void calculate_roll_pitch_yaw(float accel[], float gyro[], float mag[], float *roll, float *pitch, float *yaw);

#endif // SENSORFUSION_H
