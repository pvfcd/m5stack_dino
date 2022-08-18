#include "show.h"
#include "stdlib.h"
#include "math.h"
#define space 60
#define obstruct_value 40

uint8_t obstruct[obstruct_value];
uint8_t space_arr[obstruct_value];
static uint8_t gravity_movement[] = {100, 99, 93, 83, 69, 51,29,3}; //用excel根据公式x = 1/2at^2计算得出计算出的符合重力加速度的运动轨迹
typedef struct
{
    uint8_t obstruct_posi;
} crush_detect_typedef;

crush_detect_typedef crush_detect; //碰撞检测有关变量
/**
 * @brief 刷新显示屏线程
 *
 * @param point 传入参数
 */
void Show_Oled(void *point)
{

    uint16_t background_y_posi = 0;
    randomSeed(analogRead(26));
    for (uint8_t i = 0; i < obstruct_value; i++)
    {
        obstruct[i] = random(50, 100);
        space_arr[i] = random(80, 100);
    }
    m5.Lcd.begin();
    for (;;)
    {
        //障碍物移动
        background_y_posi++; //控制障碍物左移的变量
        for (uint8_t i = 0; i < obstruct_value; i++)
        {
            m5.Lcd.drawLine(tft_x, background_y_posi - i * space_arr[i], obstruct[i], background_y_posi - i * space_arr[i], PURPLE);
            m5.Lcd.drawLine(tft_x, background_y_posi - i * space_arr[i] - 1, obstruct[i], background_y_posi - i * space_arr[i] - 1, BLACK);
            if ((background_y_posi - i * space_arr[i]) > 200)
            {
                crush_detect.obstruct_posi = background_y_posi - i * space_arr[i];
            }
        }
        //重新生成地图
        if (background_y_posi > (space + obstruct_value) * 10)
        {
            background_y_posi = 0;
            for (uint8_t i = 0; i < obstruct_value; i++)
            {
                obstruct[i] = random(50, 100);
                space_arr[obstruct_value] = random(60, 100);
            }
        }

        vTaskDelay(30);
    }
}
void show_ball(void *point)
{
    // x= v0t+1/2at^2
    int8_t message = 0;
    uint8_t movement = 0;//动作标志位
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
            m5.Lcd.drawCircle(gravity_movement[start_time], 200, 10, RED);
            m5.Lcd.drawCircle(gravity_movement[start_time - 1], 200, 10, BLACK);
            start_time++;
            if (start_time > 6)
            {
                movement = 0;
            }
        }
        if (movement == 'D')
        {
            m5.Lcd.drawCircle(gravity_movement[start_time], 200, 10, RED);
            m5.Lcd.drawCircle(gravity_movement[start_time+1], 200, 10, BLACK);
            start_time--;
            if (start_time <=0)
            {
                movement = 0;
            }
        }

        vTaskDelay(10);
    }
}
void ball_fall_timer_callback(TimerHandle_t xtimer)
{
    
}
