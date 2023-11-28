#include "myrtos.h"

void TimeTask(void *pdata)
{
	printf("finger task coming!!\r\n");
	while(1){
//        printf("\r\n时间显示任务正在运行\r\n");
    time_task();
		vTaskDelay(600);

    }
	
}

	uint8_t time_data[128];
	RTC_TimeTypeDef RTC_TimeStruct;

void time_task(void){
	RTC_GetTime(RTC_Format_BIN,&RTC_TimeStruct);
	sprintf((char*)time_data,"Time:%02d:%02d:%02d",RTC_TimeStruct.RTC_Hours,RTC_TimeStruct.RTC_Minutes,RTC_TimeStruct.RTC_Seconds);
	
	lcd_display_string(0,0,(uint8_t*)time_data,BLUE,WHITE,FONT_SIZE_32,NULL);
	lcd_display_string(0,140,(uint8_t *)"_ _ _ _ _ _ _",BLUE,WHITE,FONT_SIZE_24,NULL);
//	printf("time %s\r\n", time_data);
	memset(time_data, 0, 128);
}