#ifndef __MY_LCD_H_
#define __MY_LCD_H_

#include "stm32f10x.h"                  // Device header
#include "bsp_lcd.h"
#include "RTC.h"
#include "kqm.h"
#include "string.h"
#include "kqm.h"
#include "dht11.h"
#include "ADC.h"
#include "Delay.h"

void show_time2LCD(void);
void show_index2LCD(void);
void show_data2LCD(void);
void lvgl_show_data_init(void);
//void show_cartoon2LCD(void);


#endif
