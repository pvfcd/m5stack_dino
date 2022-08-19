#include "imu.h"
IMU_data_Typedef acceldata, gyrodata;

#define rate 100
/**
 * @brief 读取陀螺仪线程
 * 
 * @param point 
 */
void IMU_ReadPin(void *point)
{
    if (m5.Imu.Init() != 0)
    {
        while (1)
        {
            Serial.println("IMU_INIT_ERROR!\n");
        }
    }
    uint8_t message;
    for(;;)
    {
        M5.Imu.getGyroData(&acceldata.x,&acceldata.y,&acceldata.z);//读取加速度计
        message = 'E';
        if(acceldata.x > rate)
        {
            // Serial.printf("up\n");
            message = 'U';
        }
        if(acceldata.x < -rate)
        {
            // Serial.printf("down\n");
            message = 'D';
        }
        if(acceldata.z > rate)
        {
            // Serial.printf("left\n");
            message = 'L';
        }
        if(acceldata.z < -rate)
        {
            // Serial.printf("right\n");
            // message = 'R';
        }
        // Serial.printf("{acceldatax:%f},{acceldatay:%f},{acceldataz:%f}",acceldata.x,acceldata.y,acceldata.z);
        if(message != 'E')
        {
            xQueueSend(IMU_Btn_Queue,&message,10);
        }
        
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}
