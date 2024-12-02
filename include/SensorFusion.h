#ifndef SENSORFUSION_H
#define SENSORFUSION_H

#include <math.h>

// Time step (in seconds)
#define DT 0.03

// radians to degree conversion constant
#define RAD_TO_DEG (180.0 / M_PI)

// Noise characteristics
#define Q_ANGLE 0.001   // Process noise for angle
#define Q_BIAS 0.003    // Process noise for bias
#define R_MEASURE 0.03  // Measurement noise

// Gravity constant
#define G 9.81

// Kalman Filter State Variables
typedef struct {
    float angle;      // Angle estimate (roll or pitch)
    float bias;       // Gyro bias estimate
    float rate;       // Unbiased gyro rate
    float P[2][2];    // Error covariance matrix
} KalmanState;

// Function prototypes
/**
 * @brief Initializes the Kalman filter state.
 * @param kf Pointer to the Kalman filter state structure.
 */
void Kalman_Init(KalmanState *kf);

/**
 * @brief Updates the Kalman filter state with new measurements.
 * @param kf Pointer to the Kalman filter state structure.
 * @param newRate Gyroscope rate measurement.
 * @param newAngle Accelerometer angle measurement.
 * @return Updated angle estimate.
 */
float Kalman_Update(KalmanState *kf, float newRate, float newAngle);

/**
 * @brief Initializes sensor fusion by initializing multiple Kalman filters.
 */
void init_sensor_fusion();

/**
 * @brief Calculates roll, pitch, and yaw using sensor fusion.
 * @param gyro 3x3 gyroscope data array.
 * @param accel 3x3 accelerometer data array.
 * @param mag 3x3 magnetometer data array.
 * @param sensor_bitmap Bitmap indicating active sensors.
 * @param roll Pointer to store calculated roll.
 * @param pitch Pointer to store calculated pitch.
 * @param yaw Pointer to store calculated yaw.
 */
void calculate_roll_pitch_yaw(float gyro[3][3], float accel[3][3], float mag[3][3], int sensor_bitmap, float *roll, float *pitch, float *yaw);

#endif // SENSORFUSION_H
