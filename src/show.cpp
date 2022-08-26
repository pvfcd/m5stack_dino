#include "show.h"
#include "stdlib.h"
#include "math.h"
#include "string.h"
#include "pic.h"
#define speed 4
obstruct_typedef obstruct[2];

static uint8_t gravity_movement[] = {19, 19, 36, 51, 64, 75, 84, 91, 96, 99, 100, 99, 96, 91, 84, 75, 64, 51, 36, 19, 19}; //符合重力加速度的运动轨迹

crush_detect_typedef obst_posi; //碰撞检测有关变量

void show_dino(uint16_t x,uint16_t y,uint16_t last_x,uint16_t last_y)
{
    if(x-last_x > 0)
    {
        m5.Lcd.fillRect(last_x+30,last_y,x-last_x + 60,last_y + 45,WHITE);
    }
    else
    {
        m5.Lcd.fillRect(last_x+30,last_y,last_x-x + 60,last_y + 45,WHITE);
    }
    m5.Lcd.drawBitmap(x+30,y,50,45,dino1);
}
/**
 * @brief 绘制障碍物
 * 
 * @param x 
 * @param y 
 * @param last_x 
 * @param last_y 
 * @param obst_value
 */
void show_obst(uint16_t x,uint16_t y,uint16_t last_x,uint16_t last_y,uint16_t obst_value)
{
    // m5.Lcd.fillRect(last_x,y-last_y,speed,50,WHITE);
    switch (obst_value)
    {
    case 1:
        m5.Lcd.drawBitmap(x,y,51,60,obst1);//画1号障碍物
        // m5.Lcd.fillRect(50,y-25,10,10,WHITE);
        break;
    case 2:
        m5.Lcd.drawBitmap(x,y,50,32,obst2);//画2号障碍物
        // m5.Lcd.fillRect(x,y-25,50,10,WHITE);
        break;
    default:
        break;
    }
    // m5.Lcd.fillRect(x,y+50,50,10,WHITE);
    
}
/**
 * @brief 刷新障碍物线程
 *
 * @param point 传入参数
 */
void Show_Oled(void *point)
{
    pinMode(26, INPUT);
    randomSeed(analogRead(26));
    obstruct[0].delay = 0;               //第一个障碍物不延迟直接生成
    obstruct[0].height = 1; //高度自定义
    obstruct[0].posi = 0;
    obstruct[0].enable = 1;
    for (;;)
    {
        //障碍物移动
        if (obstruct[0].enable == 1)
        {

            // Serial.printf("Obs0posi = %d,hei = %d\n", obstruct[0].posi, obstruct[0].height);
            obst_posi.obstruct_posi = obstruct[0].posi - obstruct[0].delay;
            show_obst(90,obstruct[0].posi,0,0,obstruct[0].height);
            obstruct[0].posi += speed;

            if (obstruct[0].posi - obstruct[0].delay > 245)
            {
                obstruct[0].posi = 0;
                obstruct[0].enable = 1;
                obstruct[0].height = random(1,3);
                Serial.printf("h = %d",obstruct[0].height);
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
    uint8_t i = 0;
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
            show_dino(100 - gravity_movement[start_time],200,100 - gravity_movement[start_time - 1],200);
            // m5.Lcd.drawCircle(100 - gravity_movement[start_time - 1], 200, 10, BLACK);
            // m5.Lcd.drawCircle(100 - gravity_movement[start_time], 200, 10, RED);
            start_time++;
            if (start_time > 20)
            {
                movement = 0;
            }
            obst_posi.ball_posi = gravity_movement[start_time];
        }
        else if(i == 1)
        {
            m5.Lcd.drawBitmap(100 - gravity_movement[1],200,50,45,dino1);
            i = 0;
        }
        else if (i == 0)
        {
            m5.Lcd.drawBitmap(100 - gravity_movement[1],200,50,45,dino2);
            i = 1;
        }
        

        vTaskDelay(20);
    }
}
/**
 * @brief 碰撞检测
 *
 * @param point
 */
void crush_detect(void *point)
{
    uint8_t message = 0;
    for (;;)
    {
        // Serial.printf("ball_posi = %d,obsposi = %d,obsheight= %d\n", obst_posi.ball_posi, obst_posi.obstruct_posi, obst_posi.obstruct_height);
        if (197 < obst_posi.obstruct_posi && obst_posi.obstruct_posi < 203)
        {
            if (70 > obst_posi.ball_posi)
            {

                if (oled_show_handle != NULL)
                {
                    vTaskSuspend(oled_show_handle);
                    oled_show_handle = NULL;
                }
                if (show_ball_handle != NULL)
                {
                    vTaskSuspend(show_ball_handle);
                    show_ball_handle = NULL;
                }
                // Serial.printf("stop!");
            }
            
        }
        if (oled_show_handle == NULL)
        {
            m5.Lcd.drawBitmap(0,0,135,240,end);
            if (xQueueReceive(IMU_Btn_Queue, &message, 10) == pdPASS)
            {
                if (message == 'U')
                {
                    abort();
                }
            }
        }
        // Serial.printf("score=%d",score/speed);
        // sprintf(score_buf,"%d",score);
        // m5.Lcd.drawRect(0,240,135,236,YELLOW);//修正残影
        vTaskDelay(20);
    }
}
