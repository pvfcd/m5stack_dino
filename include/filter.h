#ifndef __FILTER_H_
#define __FILTER_H_

typedef struct //与卡尔曼滤波有关的结构体
{
    float x_mea; // 测量值
    float x_est; // 估计值
    float e_mea; // 测量误差，假设测量工具量程是2000，误差2%，测量误差就是2000*2%=40
    float e_est; // 估计误差，每次进行估计后需要更新
    float Kk;    // 卡尔曼增益
}KalmanFilter;
void Kalman_Update(KalmanFilter *kalmanFilter, float newMeaValue);
void Kalman_Init(KalmanFilter *kalmanFilter, float FirstMeaValue, float E_mea, float FirstEstValue, float E_est);

#endif