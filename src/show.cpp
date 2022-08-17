#include "show.h"

void Show_Oled(void *point)
{
    int8_t message = 0;
    m5.Lcd.begin();
    for (;;)
    {
        message = 0;
        if (xQueueReceive(IMU_Btn_Queue, &message, 10) == pdPASS)
        {
            Serial.printf("message = %c", message);
            // Serial.printf("queue_Empty!");
            
        }
        if(message == 'R')
        {
            m5.Lcd.drawCircle(1*tft_x/4,200,10,BLACK);
            m5.Lcd.drawCircle(3*tft_x/4,200,10,RED);
        }
        if(message == 'L')
        {
            m5.Lcd.drawCircle(3*tft_x/4,200,10,BLACK);
            m5.Lcd.drawCircle(1*tft_x/4,200,10,RED);
        }
        vTaskDelay(100);
    }
}
