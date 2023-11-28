#ifndef __DELAY_H
#define __DELAY_H
#include "stm32f10x.h"                  // Device header
#include "7s1.h"
#include "led.h"
#include "usart.h"
#include "stdio.h"
#include "Modbus.h"

//定时器任务变量
extern uint32_t led_tick[2];
extern uint32_t key_tick[2];

void Delay_us(uint32_t us);
void Delay_ms(uint32_t ms);
void Delay_s(uint32_t s);
void sys_tick_init(void);

void led_time_scale(uint16_t time);
void dataanalysis_time_scale(uint16_t time);
void getdata_time_scale(uint16_t time);
void modbus_time_scale(uint16_t time);

#endif
