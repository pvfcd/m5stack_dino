#ifndef __MAIN_H_
#define __MAIN_H_

#include <Arduino.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <M5StickCPlus.h>
#include "stdlib.h"
#include "stdint.h"
extern QueueHandle_t IMU_Btn_Queue;//创建一个队列,长度4，大小20
#endif