#include "SensorFusion.h"

KalmanState kf_roll_1;
KalmanState kf_pitch_1;

KalmanState kf_roll_2;
KalmanState kf_pitch_2;

KalmanState kf_roll_3;
KalmanState kf_pitch_3;

// Initialize Kalman Filter State
void Kalman_Init(KalmanState *kf) {
    kf->angle = 0.0f;  // Initial angle
    kf->bias = 0.0f;   // Initial bias
    kf->rate = 0.0f;   // Initial rate

    // Initial covariance matrix
    kf->P[0][0] = 1.0f;
    kf->P[0][1] = 0.0f;
    kf->P[1][0] = 0.0f;
    kf->P[1][1] = 1.0f;
}

float Kalman_Update(KalmanState *kf, float newRate, float newAngle) {
    float rate = newRate - kf->bias;  // Unbiased rate
    kf->angle += DT * rate;          // Predict the new angle

    // Update error covariance matrix
    kf->P[0][0] += DT * (DT * kf->P[1][1] - kf->P[0][1] - kf->P[1][0] + Q_ANGLE);
    kf->P[0][1] -= DT * kf->P[1][1];
    kf->P[1][0] -= DT * kf->P[1][1];
    kf->P[1][1] += Q_BIAS * DT;

    // Kalman Gain
    float S = kf->P[0][0] + R_MEASURE;  // Estimate error
    float K[2];                         // Kalman gain
    K[0] = kf->P[0][0] / S;
    K[1] = kf->P[1][0] / S;

    // Measurement Update
    float y = newAngle - kf->angle;  // Angle difference
    kf->angle += K[0] * y;
    kf->bias += K[1] * y;

    // Update error covariance
    float P00_temp = kf->P[0][0];
    float P01_temp = kf->P[0][1];

    kf->P[0][0] -= K[0] * P00_temp;
    kf->P[0][1] -= K[0] * P01_temp;
    kf->P[1][0] -= K[1] * P00_temp;
    kf->P[1][1] -= K[1] * P01_temp;

    return kf->angle;  // Return the updated angle
}

void init_sensor_fusion(){
    Kalman_Init(&kf_roll_1);
    Kalman_Init(&kf_pitch_1);
    Kalman_Init(&kf_roll_2);
    Kalman_Init(&kf_pitch_2);
    Kalman_Init(&kf_roll_3);
    Kalman_Init(&kf_pitch_3);
}

// Updated function to calculate roll, pitch, and yaw
void calculate_roll_pitch_yaw(float gyro[3][3], float accel[3][3], float mag[3][3], int sensor_bitmap,
                              float *roll, float *pitch, float *yaw) {

    float ax_1 = 0, ay_1 = 0, az_1 = 0;
    float gx_1 = 0, gy_1 = 0, gz_1 = 0;
    float ax_2 = 0, ay_2 = 0, az_2 = 0;
    float gx_2 = 0, gy_2 = 0, gz_2 = 0;

    if((sensor_bitmap & 0b100000000) == 0b100000000){
        gx_1 = gyro[0][0] / 131.0 * (M_PI / 180.0);
        gy_1 = gyro[0][1] / 131.0 * (M_PI / 180.0);
        gz_1 = gyro[0][2] / 131.0 * (M_PI / 180.0);
    }

    if((sensor_bitmap & 0b010000000) == 0b010000000){
        ax_1 = accel[0][0] / 16384.0 * G;
        ay_1 = accel[0][1] / 16384.0 * G;
        az_1 = accel[0][2] / 16384.0 * G;
    }

    if((sensor_bitmap & 0b000100000) == 0b000100000){
        gx_2 = gyro[1][0] / 131.0 * (M_PI / 180.0);
        gy_2 = gyro[1][1] / 131.0 * (M_PI / 180.0);
        gz_2 = gyro[1][2] / 131.0 * (M_PI / 180.0);
    }

    if((sensor_bitmap & 0b000010000) == 0b000010000){
        ax_2 = accel[1][0] / 16384.0 * G;
        ay_2 = accel[1][1] / 16384.0 * G;
        az_2 = accel[1][2] / 16384.0 * G;
    }

    float roll_acc_1 = atan2(ay_1, sqrt(ax_1 * ax_1 + az_1 * az_1));
    float pitch_acc_1 = atan2(-ax_1, sqrt(ay_1 * ay_1 + az_1 * az_1));

    float roll_acc_2 = atan2(ay_2, sqrt(ax_2 * ax_2 + az_2 * az_2));
    float pitch_acc_2 = atan2(-ax_2, sqrt(ay_2 * ay_2 + az_2 * az_2));

    float roll_1 = Kalman_Update(&kf_roll_1, gx_1, roll_acc_1);
    float pitch_1 = Kalman_Update(&kf_pitch_1, gy_1, pitch_acc_1);

    float roll_2 = Kalman_Update(&kf_roll_1, gx_1, roll_acc_2);
    float pitch_2 = Kalman_Update(&kf_pitch_1, gy_1, pitch_acc_2);

    roll_1 *= RAD_TO_DEG;
    pitch_1 *= RAD_TO_DEG;
    roll_2 *= RAD_TO_DEG;
    pitch_2 *= RAD_TO_DEG;

    *roll = roll_1;
    *pitch = pitch_1;


}
