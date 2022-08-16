
#include "show.h"

void Show_Oled(void *point)
{
    int8_t message = 0;
    for (;;)
    {
        if (xQueueReceive(IMU_Btn_Queue, &message, 10) != pdPASS)
        {
            Serial.printf("queue_Empty!");
            message = 0;
        }
        Serial.printf("message = %c", message);
        vTaskDelay(100);
    }
}
