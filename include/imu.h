#ifndef _IMU_H_
#define _IMU_H_
#include "main.h"
typedef struct //陀螺仪有关数据结构体
{
    float x;
    float y;
    float z;
    int16_t adc_x;
    int16_t adc_y;
    int16_t adc_z;
} IMU_data_Typedef;
extern IMU_data_Typedef acceldata, gyrodata;
// float Gyro_x_Data_Filter();  

#endif
