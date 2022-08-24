#ifndef __MAIN_H_
#define __MAIN_H_

#include <Arduino.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <M5StickCPlus.h>
#include "stdlib.h"
#include "stdint.h"

#define tft_x 135
#define tft_y 240
#define ALLBITS 0xFFFFFF //24bits都是1

extern QueueHandle_t IMU_Btn_Queue;//创建一个队列,长度4，大小20
extern EventGroupHandle_t obstruct_refresh_event; //创建重新刷新障碍物事件组
extern TaskHandle_t oled_show_handle,show_ball_handle;
#endif