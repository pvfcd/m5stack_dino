#ifndef __SHOW_H_
#define __SHOW_H_
#include "main.h"
void Show_Oled(void *point);
void show_ball(void *point);
void ball_fall_timer_callback(TimerHandle_t xtimer);
void obstruct_calib(void *point);
void crush_detect(void *point);
typedef struct
{
    uint16_t obstruct_posi;
    uint8_t obstruct_height;
    uint8_t ball_posi;
} crush_detect_typedef;
typedef struct
{
    uint16_t posi;
    uint8_t height;
    uint8_t delay;//延迟生成的时间，以达成随机间距效果
    TickType_t start_time;//记录生成这个障碍物的时间戳，利于delay判定
    uint8_t enable;
}obstruct_typedef;
#endif