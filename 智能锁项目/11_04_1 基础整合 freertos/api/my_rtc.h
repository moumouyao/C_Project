#ifndef __MY_RTC_H_
#define __MY_RTC_H_

#include "stm32f4xx.h"                  // Device header
#include "usart.h"
void RTC_Set_Time(uint8_t hour, uint8_t min, uint8_t sec, uint8_t RTC_H12);
void RTC_Set_Date(uint8_t RTC_Year, int8_t RTC_Month, int8_t RTC_Date, int8_t RTC_WeekDay);
void my_rtc_init(void);

#endif
