#ifndef __DELAY_H
#define __DELAY_H
#include "stm32f10x.h"                  // Device header
#include "su_03t.h"
#include "kqm.h"
#include "dht11.h"
#include "ADC.h"
#include "key.h"
#include "RTC.h"
#include "my_lcd.h"
#include "FreeRTOS.h"
#include "task.h"

//定时器任务变量
extern uint32_t led_tick[2];
extern uint32_t key_tick[2];

void Delay_us(uint32_t us);
void Delay_ms(uint32_t ms);
void Delay_s(uint32_t s);
void sys_tick_init(void);
void delay_1us(void);

void dht_time_scale(uint16_t time);
void su_time_scale(uint8_t time);
void kqm_time_scale(uint8_t time);
void ADC_time_scale(uint16_t time);
void lowpor_time_scale(uint16_t time);
void current_time_scale(uint16_t time);
void current_time2LCD_scale(uint16_t time);
void show_data2LCD_scale(uint16_t time);

#endif
