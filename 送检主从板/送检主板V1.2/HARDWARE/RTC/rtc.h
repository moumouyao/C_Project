#ifndef _RTC_H_
#define _RTC_H_

#include "stm32f4xx.h"
#include "stdio.h"
#include "sw_controlcenter.h"
#include "delay.h"
#include "ctrlcenter_app.h"
#include "cJSON.h"
#include "string.h"

void HOST_Get_SetTime_Ack(uint8_t * p,uint16_t *len, char* id);
extern unsigned char init_RTC(void);
extern ErrorStatus RTC_Set_Time(u8 hour,u8 min,u8 sec,u8 ampm);
extern ErrorStatus RTC_Set_Date(u8 year,u8 month,u8 date,u8 week);
extern void getTime(void);
extern void HOST_SetTime(cJSON *cjson);

typedef struct 
{
	uint8_t year;
	uint8_t month;
	uint8_t day;
	uint8_t week;
	uint8_t hour;
	uint8_t min;
	uint8_t sec;
}TIME;
extern TIME g_tTime;

#endif
