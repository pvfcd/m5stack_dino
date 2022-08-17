#include "show.h"
#include "stdlib.h"

#define space 60
#define obstruct_value 40
uint8_t obstruct[obstruct_value];
uint8_t space_arr[obstruct_value];

void Show_Oled(void *point)
{
    int8_t message = 0;
    uint16_t background_y_posi = 0;
    randomSeed(analogRead(26));
    for (uint8_t i = 0; i < obstruct_value; i++)
    {
        obstruct[i] = random(50, 100);
        space_arr[i] = random(60,100);
    }
    m5.Lcd.begin();
    for (;;)
    {
        message = 0;
        if (xQueueReceive(IMU_Btn_Queue, &message, 10) == pdPASS)
        {
            Serial.printf("message = %c", message);
        }
        background_y_posi++; //控制背景左移的变量
        for (uint8_t i = 0; i < obstruct_value; i++)
        {
            m5.Lcd.drawLine(tft_x, background_y_posi - i * space_arr[i], obstruct[i], background_y_posi - i * space_arr[i], PURPLE);
            m5.Lcd.drawLine(tft_x, background_y_posi - i * space_arr[i] - 1, obstruct[i], background_y_posi - i * space_arr[i] - 1, BLACK);
        }
        if (background_y_posi > (space + obstruct_value) * 10)
        {
            background_y_posi = 0;
            for (uint8_t i = 0; i < obstruct_value; i++)
            {
                obstruct[i] = random(50, 100);
                space_arr[obstruct_value] = random(60,100);
            }
        }
        if (message == 'R')
        {
            m5.Lcd.drawCircle(1 * tft_x / 4, 200, 10, BLACK);
            m5.Lcd.drawCircle(3 * tft_x / 4, 200, 10, RED);
        }
        if (message == 'L')
        {
            m5.Lcd.drawCircle(3 * tft_x / 4, 200, 10, BLACK);
            m5.Lcd.drawCircle(1 * tft_x / 4, 200, 10, RED);
        }
        vTaskDelay(30);
    }
}
