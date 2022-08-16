#include "main.h"
#include "imu.h"
// #include "filter.h"


float pitch, roll, yaw;
static uint8_t lcd_height = m5.Lcd.height();
static uint8_t lcd_width = m5.Lcd.width();
uint32_t lastpixel;
void setup()
{
      // put your setup code here, to run once:
    m5.begin();
    if (m5.Imu.Init() != 0)
    {
        while (1)
        {
            Serial.println("IMU_INIT_ERROR!\n");
        }
    }

    pinMode(10, OUTPUT);    //初始化灯
    pinMode(37,INPUT_PULLUP);
    digitalWrite(10, HIGH); //设置为高电平
}

void loop()
{
      // put your main code here, to run repeatedly:
        M5.Imu.getGyroData(&acceldata.x,&acceldata.y,&acceldata.z);
        m5.Imu.getAccelAdc(&acceldata.adc_x,&acceldata.adc_y,&acceldata.adc_z);
        // Serial.printf("{afterfilterx:%f}",Gyro_x_Data_Filter());
        Serial.printf("{acceldatax:%f},{acceldatay:%f},{acceldataz:%f}",acceldata.x,acceldata.y,acceldata.z);
        Serial.printf("{adcdatax:%d},{adcdatay:%d},{adcdataz:%d}",acceldata.adc_x,acceldata.adc_y,acceldata.adc_z);
        m5.Lcd.drawPixel(lastpixel,100,BLACK);
        m5.Lcd.drawPixel((int32_t)(acceldata.x *100+10),100,RED);
        lastpixel = (int32_t)(acceldata.x *100);
        Serial.printf ("{lastpixel:%d}",lastpixel);
        // m5.Lcd.drawLine((int32_t)(acceldata.x *100+10),60,(int32_t)(acceldata.x *100-10),60,RED);
        if(digitalRead(37) == LOW)
        {
            digitalWrite(10,LOW);
        }
        delay(100);
}
