#include "rtc.h"

TIME g_tTime;//时间数据
RTC_TimeTypeDef RTC_TimeStruct;
RTC_DateTypeDef RTC_DateStruct;

unsigned char init_RTC(void)
{
	RTC_InitTypeDef RTC_InitStructure;
	u16 retry=0X1FFF; 
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);//使能PWR时钟
	PWR_BackupAccessCmd(ENABLE);	//使能后备寄存器访问 
	
		RCC_LSEConfig(RCC_LSE_ON);//LSE 开启    
		while (RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET)	//检查指定的RCC标志位设置与否,等待低速晶振就绪
			{
			retry++;
			delay_ms(10);
			}
		if(retry==0)return 1;		//LSE 开启失败. 
			
//		RCC_RTCCLKConfig(RCC_RTCCLKSource_HSE_Div25);		//设置RTC时钟(RTCCLK),高速时钟的25分频 = 320k
		RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);		//设置RTC时钟(RTCCLK),选择LSE作为RTC时钟    
		RCC_RTCCLKCmd(ENABLE);	//使能RTC时钟 

    RTC_InitStructure.RTC_AsynchPrediv = 0x7F;//127;//RTC异步分频系数(1~0X7F) 0x7f
    RTC_InitStructure.RTC_SynchPrediv  = 0xff;//2499;//RTC同步分频系数(0~7FFF) 0xff
    RTC_InitStructure.RTC_HourFormat   = RTC_HourFormat_24;//RTC设置为,24小时格式
    RTC_Init(&RTC_InitStructure);
 	if(RTC_ReadBackupRegister(RTC_BKP_DR0)!=0x5050)		//是否第一次配置?
	{
		RTC_Set_Time(23,59,56,RTC_H12_AM);	//设置时间
		RTC_Set_Date(22,8,16,2);		//设置日期
		RTC_WriteBackupRegister(RTC_BKP_DR0,0x5050);	//标记已经初始化过了
	}
	return 0;
}

//RTC时间设置
//hour,min,sec:小时,分钟,秒钟
//ampm:@RTC_AM_PM_Definitions  :RTC_H12_AM/RTC_H12_PM
//返回值:SUCEE(1),成功
//       ERROR(0),进入初始化模式失败 
ErrorStatus RTC_Set_Time(u8 hour,u8 min,u8 sec,u8 ampm)
{
	RTC_TimeTypeDef RTC_TimeTypeInitStructure;
	
	RTC_TimeTypeInitStructure.RTC_Hours=hour;
	RTC_TimeTypeInitStructure.RTC_Minutes=min;
	RTC_TimeTypeInitStructure.RTC_Seconds=sec;
	RTC_TimeTypeInitStructure.RTC_H12=ampm;
	
	return RTC_SetTime(RTC_Format_BIN,&RTC_TimeTypeInitStructure);
	
}
//RTC日期设置
//year,month,date:年(0~99),月(1~12),日(0~31)
//week:星期(1~7,0,非法!)
//返回值:SUCEE(1),成功
//       ERROR(0),进入初始化模式失败 
ErrorStatus RTC_Set_Date(u8 year,u8 month,u8 date,u8 week)
{
	RTC_DateTypeDef RTC_DateTypeInitStructure;
	RTC_DateTypeInitStructure.RTC_Date=date;
	RTC_DateTypeInitStructure.RTC_Month=month;
	RTC_DateTypeInitStructure.RTC_WeekDay=week;
	RTC_DateTypeInitStructure.RTC_Year=year;
	return RTC_SetDate(RTC_Format_BIN,&RTC_DateTypeInitStructure);
}

void getTime()
{
	RTC_GetTime(RTC_Format_BIN,&RTC_TimeStruct);//获得时间
	RTC_GetDate(RTC_Format_BIN, &RTC_DateStruct);//获得日期
	g_tTime.year =  RTC_DateStruct.RTC_Year;
	g_tTime.month = RTC_DateStruct.RTC_Month;
	g_tTime.day = RTC_DateStruct.RTC_Date;
	g_tTime.week = RTC_DateStruct.RTC_WeekDay;
	g_tTime.hour = RTC_TimeStruct.RTC_Hours;
	g_tTime.min = RTC_TimeStruct.RTC_Minutes;
	g_tTime.sec = RTC_TimeStruct.RTC_Seconds;
}

//获取主机时间
void HOST_Get_SetTime_Ack(uint8_t * p,uint16_t *len, char* id)
{
	cJSON* cjson_test=NULL;
	char data_time[30];
	
	cjson_test=cJSON_CreateObject();
	
	RTC_GetTime(RTC_Format_BIN,&RTC_TimeStruct);//获得时间
	RTC_GetDate(RTC_Format_BIN, &RTC_DateStruct);//获得日期
	
	sprintf(data_time, "20%d-%d-%dT%d:%d:%dZ+0:0", 
					RTC_DateStruct.RTC_Year, RTC_DateStruct.RTC_Month, RTC_DateStruct.RTC_Date, 
					RTC_TimeStruct.RTC_Hours, RTC_TimeStruct.RTC_Minutes, RTC_TimeStruct.RTC_Seconds);
	
#if 1
	printf("查询系统时间%s\r\n",data_time);
#endif
	
	cJSON_AddStringToObject(cjson_test,"id",id);//添加字符类型的
	cJSON_AddStringToObject(cjson_test,"dataTime",data_time);//添加字符类型的
	
  char *cjson_data=cJSON_PrintUnformatted(cjson_test);
	memcpy(p,cjson_data,(unsigned int)strlen(cjson_data));
	*len=strlen(cjson_data);
	cJSON_Delete(cjson_test);
	
}


//设置主机系统时间
void HOST_SetTime(cJSON *cjson)
{
	char * data_Time = NULL;
		TIME tTime;
	data_Time=cjson_get_string(cjson,"dateTime");
#if cJSON_Dbug
					printf("设置日期和时间%s\r\n",data_Time);
#endif

	
	//2023-03-07T12:13:14Z+08:00
	tTime.year  = (data_Time[2] -48)*10+(data_Time[3]- 48);
	tTime.month = (data_Time[5] -48)*10+(data_Time[6]- 48);
	tTime.day   = (data_Time[8] -48)*10+(data_Time[9]- 48);
	tTime.hour  = (data_Time[11] -48)*10+(data_Time[12]- 48) + (data_Time[21] -48)*10+(data_Time[22]- 48);
	tTime.min   = (data_Time[14] -48)*10+(data_Time[15]- 48);
	tTime.sec   = (data_Time[17] -48)*10+(data_Time[18]- 48); 
	RTC_Set_Date(tTime.year,tTime.month,tTime.day,1);//设置日期
	RTC_Set_Time(tTime.hour,tTime.min,tTime.sec,RTC_H12_AM);//设置时间
}



