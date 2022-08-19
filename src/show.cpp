#include "show.h"
#include "stdlib.h"
#include "math.h"
#define space 60
#define obstruct_value 40
#define gravity

uint8_t obstruct[obstruct_value];
uint8_t space_arr[obstruct_value];

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
    uint8_t start_rand_obstruct = 1;
    obstruct_typedef obstruct[2];
    obstruct[0].delay = 0;               //第一个障碍物不延迟直接生成
    obstruct[0].height = random(40, 80); //高度自定义
    obstruct[0].enable = 1;
    obstruct[1].start_time = 0;
    m5.Lcd.begin();
    obstruct[0].start_time = xTaskGetTickCount(); //记录一个开始时间
    for (;;)
    {
        //障碍物移动
        if (xTaskGetTickCount() - obstruct[0].start_time >= obstruct[0].delay || obstruct[0].enable == 1)
        {
            obstruct[0].posi+=2;
            obstruct[0].enable = 1;
        }
        if (obstruct[0].posi > 200&& obstruct[1].start_time == 0)
        {
            obstruct[1].delay = random(0, 2000);
            obstruct[1].height = random(40, 80);
            obstruct[1].posi = 0;
            obstruct[1].start_time = xTaskGetTickCount();
        }
        if (obstruct[0].posi > 240)
        {
            obstruct[0].enable = 0;
            obstruct[0].posi = 0;
            obstruct[0].start_time = 0;
        }
        if (xTaskGetTickCount() - obstruct[1].start_time >= obstruct[1].delay || obstruct[1].enable == 1)
        {
            obstruct[1].posi+=2;
            obstruct[1].enable = 1;
        }
        if (obstruct[1].posi > 200 && obstruct[0].start_time == 0)
        {
            obstruct[0].delay = random(0, 2000);
            obstruct[0].height = random(20, 40);
            obstruct[0].start_time = xTaskGetTickCount();
        }
        if (obstruct[1].posi > 240)
        {
            obstruct[1].enable = 0;
            obstruct[1].posi = 0;
            obstruct[1].start_time = 0;
        }

        if (obstruct[0].enable == 1)
        {
            m5.Lcd.drawLine(100-obstruct[0].height, obstruct[0].posi, 100, obstruct[0].posi, PINK);
            m5.Lcd.drawLine(100-obstruct[0].height, obstruct[0].posi - 1, 100, obstruct[0].posi - 1, BLACK);
        }
    if(obstruct[1].enable == 1)
    {
                m5.Lcd.drawLine(100-obstruct[1].height, obstruct[1].posi, 100, obstruct[1].posi, PINK);
        m5.Lcd.drawLine(100-obstruct[1].height, obstruct[1].posi - 1, 100, obstruct[1].posi - 1, BLACK);
    }

        vTaskDelay(30);
    }
}
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
        if (message == 'R' && movement == 0)
        {
            movement = 'D'; //标志位，标志小球处于向下状态
            start_time = 5;
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
void ball_fall_timer_callback(TimerHandle_t xtimer)
{
    uint8_t data_send = 'R';
    xQueueSend(IMU_Btn_Queue, &data_send, 10);
}
