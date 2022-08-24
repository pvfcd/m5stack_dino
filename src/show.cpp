#include "show.h"
#include "stdlib.h"
#include "math.h"

#define speed 4
obstruct_typedef obstruct[2];

static uint8_t gravity_movement[] = {19, 19, 36, 51, 64, 75, 84, 91, 96, 99, 100, 99, 96, 91, 84, 75, 64, 51, 36, 19, 19}; //符合重力加速度的运动轨迹

crush_detect_typedef crush_detect; //碰撞检测有关变量
/**
 * @brief 刷新显示屏线程
 *
 * @param point 传入参数
 */
void Show_Oled(void *point)
{
    pinMode(26, INPUT);
    randomSeed(analogRead(26));
    obstruct[0].delay = 0;               //第一个障碍物不延迟直接生成
    obstruct[0].height = random(60, 80); //高度自定义
    obstruct[0].posi = 0;
    obstruct[0].enable = 1;
    for (;;)
    {
        //障碍物移动
        if(obstruct[0].enable == 1 )
        {

            Serial.printf("Obs0posi = %d,hei = %d\n",obstruct[0].posi,obstruct[0].height);
            m5.Lcd.drawLine(tft_x-30,obstruct[0].posi-speed-obstruct[0].delay,tft_x-obstruct[0].height,obstruct[0].posi-speed-obstruct[0].delay,BLACK);
           m5.Lcd.drawLine(tft_x-30,obstruct[0].posi-obstruct[0].delay,tft_x-obstruct[0].height,obstruct[0].posi-obstruct[0].delay,PINK);
           
           obstruct[0].posi += speed;  

           if(obstruct[0].posi-obstruct[0].delay > 240)
           {
            obstruct[0].posi = 0;
            obstruct[0].enable = 0;
            xEventGroupSetBits(obstruct_refresh_event,0x1<<1);
            xEventGroupSetBits(obstruct_refresh_event,0x1<<0);
           }
        }
        if(obstruct[1].enable == 1 )
        {

            Serial.printf("Obs1posi = %d,hei = %d\n",obstruct[1].posi,obstruct[1].height);
           m5.Lcd.drawLine(tft_x-30,obstruct[1].posi-speed-obstruct[1].delay,tft_x-obstruct[1].height,obstruct[1].posi-speed-obstruct[1].delay,BLACK);
           m5.Lcd.drawLine(tft_x-30,obstruct[1].posi-obstruct[1].delay,tft_x-obstruct[1].height,obstruct[1].posi-obstruct[1].delay,PINK);

           obstruct[1].posi += speed;  

           if(obstruct[1].posi-obstruct[1].delay > 240)
           {
            obstruct[1].posi = 0;
            obstruct[1].enable = 0;
            xEventGroupSetBits(obstruct_refresh_event,0x1<<2);
            xEventGroupSetBits(obstruct_refresh_event,0x1<<0);
           }
        }   
        vTaskDelay(30);
    }
}
/**
 * @brief 刷新球线程
 * 
 * @param point 
 */
void show_ball(void *point)
{
    // x= v0t+1/2at^2
    int8_t message = 0;
    uint8_t movement = 0;      //动作标志位
    TickType_t start_time = 0; //开始物理学计算的起始时间
    for (;;)
    {
        //读取按键电平位置
        message = 0;
        if (xQueueReceive(IMU_Btn_Queue, &message, 10) == pdPASS)
        {
            Serial.printf("message = %c", message);
        }
        //绘制球

        if (message == 'L' && movement == 0)
        {
            movement = 'U'; //标志位，标志小球处于向上状态
            start_time = 1;
        }
        if (movement == 'U')
        {
            m5.Lcd.drawCircle(100 - gravity_movement[start_time - 1], 200, 10, BLACK);
            m5.Lcd.drawCircle(100 - gravity_movement[start_time], 200, 10, RED);
            start_time++;
            if (start_time > 20)
            {
                movement = 0;
            }
        }

        vTaskDelay(10);
    }
}
/**
 * @brief 随机生成障碍物
 * 
 * @param point 
 */
void obstruct_calib(void *point)
{
    EventBits_t obstruct_bit;//存储读取的事件组
    for(;;)
    {
        obstruct_bit = xEventGroupWaitBits(obstruct_refresh_event, // Event Group Handler
                                           0x1,              //等待1位
                                           pdFALSE,                //执行后，对应的Bits是否重置为 0
                                           pdTRUE,                 //等待的Bits判断关系 True为 AND, False为 OR
                                           portMAX_DELAY);         //一直等待
        printf("obstruct_event = %x\n",obstruct_bit);
        if(obstruct_bit&0x1<<1)
        {
            obstruct[1].delay = random(40,80);
            obstruct[1].height = random(50, 100); //高度自定义
            obstruct[1].posi = 0;
            obstruct[1].enable = 1;
        }
        if(obstruct_bit&0x1<<2)
        {
            obstruct[0].delay = random(40,80);
            obstruct[0].height = random(50, 100); //高度自定义
            obstruct[0].posi = 0;
            obstruct[0].enable = 1;
        }
        xEventGroupClearBits(obstruct_refresh_event,ALLBITS);
    }
}
/**
 * @brief 
 * 
 * @param xtimer 
 */
void ball_fall_timer_callback(TimerHandle_t xtimer)
{
    uint8_t data_send = 'R';
    xQueueSend(IMU_Btn_Queue, &data_send, 10);
}
