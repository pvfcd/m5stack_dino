#include "main.h"
#include "imu.h"
#include "show.h"
// #include "filter.h"

QueueHandle_t IMU_Btn_Queue = xQueueCreate(4,sizeof(char));//创建一个队列,长度4，大小20
TimerHandle_t ball_down_timer = xTimerCreate("ball_down",500,pdFALSE,(void*)1,ball_fall_timer_callback);
void setup()
{
      // put your setup code here, to run once:
    m5.begin();
    // for(int i = 0 ; i < 240 ; i +=4)
    // {
    //     for (int j = 0 ; j < 135; j ++)
    //     {
    //         m5.Lcd.drawPixel(j,i,RED);
    //     }
    // }
    xTaskCreatePinnedToCore(IMU_ReadPin,"read_pin",4096,NULL,1024,NULL,0);
    xTaskCreatePinnedToCore(Show_Oled,"oled_show",1024*10,NULL,3,NULL,1);//使用CPU1刷屏
    xTaskCreatePinnedToCore(show_ball,"show_ball",1024*5,NULL,3,NULL,1);//刷新移动球
    
    pinMode(10, OUTPUT);    //初始化灯
    pinMode(37,INPUT_PULLUP);
    digitalWrite(10, HIGH); //设置为高电平
}

void loop()
{
      // put your main code here, to run repeatedly:

}
