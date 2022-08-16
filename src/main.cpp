#include "main.h"
#include "imu.h"
#include "show.h"
// #include "filter.h"

QueueHandle_t IMU_Btn_Queue = xQueueCreate(4,sizeof(char));//创建一个队列,长度4，大小20
void setup()
{
      // put your setup code here, to run once:
    m5.begin();
    xTaskCreate(IMU_ReadPin,"read_pin",4096,NULL,1024,NULL);
    xTaskCreate(Show_Oled,"oled_show",1024,NULL,3,NULL);
    pinMode(10, OUTPUT);    //初始化灯
    pinMode(37,INPUT_PULLUP);
    digitalWrite(10, HIGH); //设置为高电平
}

void loop()
{
      // put your main code here, to run repeatedly:

}
