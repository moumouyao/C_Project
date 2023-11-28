#ifndef __RTC_H_
#define __RTC_H_

#include "stm32f10x.h"                  // Device header
#include "usart.h"
#include "time.h"
#include "string.h"
#include "my_mqtt.h"
#include "RTC.h"

void rtc_init(void);
void RTC_Configuration(void);
void Time_Adjust(void);
uint32_t Time_Regulate(void);
void NVIC_Configuration(void);
void show_time(void);
uint8_t get_time_str(unsigned char * str);

#endif
