#include "rtc.h"

TIME g_tTime;//ʱ������
RTC_TimeTypeDef RTC_TimeStruct;
RTC_DateTypeDef RTC_DateStruct;

unsigned char init_RTC(void)
{
	RTC_InitTypeDef RTC_InitStructure;
	u16 retry=0X1FFF; 
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);//ʹ��PWRʱ��
	PWR_BackupAccessCmd(ENABLE);	//ʹ�ܺ󱸼Ĵ������� 
	
		RCC_LSEConfig(RCC_LSE_ON);//LSE ����    
		while (RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET)	//���ָ����RCC��־λ�������,�ȴ����پ������
			{
			retry++;
			delay_ms(10);
			}
		if(retry==0)return 1;		//LSE ����ʧ��. 
			
//		RCC_RTCCLKConfig(RCC_RTCCLKSource_HSE_Div25);		//����RTCʱ��(RTCCLK),����ʱ�ӵ�25��Ƶ = 320k
		RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);		//����RTCʱ��(RTCCLK),ѡ��LSE��ΪRTCʱ��    
		RCC_RTCCLKCmd(ENABLE);	//ʹ��RTCʱ�� 

    RTC_InitStructure.RTC_AsynchPrediv = 0x7F;//127;//RTC�첽��Ƶϵ��(1~0X7F) 0x7f
    RTC_InitStructure.RTC_SynchPrediv  = 0xff;//2499;//RTCͬ����Ƶϵ��(0~7FFF) 0xff
    RTC_InitStructure.RTC_HourFormat   = RTC_HourFormat_24;//RTC����Ϊ,24Сʱ��ʽ
    RTC_Init(&RTC_InitStructure);
 	if(RTC_ReadBackupRegister(RTC_BKP_DR0)!=0x5050)		//�Ƿ��һ������?
	{
		RTC_Set_Time(23,59,56,RTC_H12_AM);	//����ʱ��
		RTC_Set_Date(22,8,16,2);		//��������
		RTC_WriteBackupRegister(RTC_BKP_DR0,0x5050);	//����Ѿ���ʼ������
	}
	return 0;
}

//RTCʱ������
//hour,min,sec:Сʱ,����,����
//ampm:@RTC_AM_PM_Definitions  :RTC_H12_AM/RTC_H12_PM
//����ֵ:SUCEE(1),�ɹ�
//       ERROR(0),�����ʼ��ģʽʧ�� 
ErrorStatus RTC_Set_Time(u8 hour,u8 min,u8 sec,u8 ampm)
{
	RTC_TimeTypeDef RTC_TimeTypeInitStructure;
	
	RTC_TimeTypeInitStructure.RTC_Hours=hour;
	RTC_TimeTypeInitStructure.RTC_Minutes=min;
	RTC_TimeTypeInitStructure.RTC_Seconds=sec;
	RTC_TimeTypeInitStructure.RTC_H12=ampm;
	
	return RTC_SetTime(RTC_Format_BIN,&RTC_TimeTypeInitStructure);
	
}
//RTC��������
//year,month,date:��(0~99),��(1~12),��(0~31)
//week:����(1~7,0,�Ƿ�!)
//����ֵ:SUCEE(1),�ɹ�
//       ERROR(0),�����ʼ��ģʽʧ�� 
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
	RTC_GetTime(RTC_Format_BIN,&RTC_TimeStruct);//���ʱ��
	RTC_GetDate(RTC_Format_BIN, &RTC_DateStruct);//�������
	g_tTime.year =  RTC_DateStruct.RTC_Year;
	g_tTime.month = RTC_DateStruct.RTC_Month;
	g_tTime.day = RTC_DateStruct.RTC_Date;
	g_tTime.week = RTC_DateStruct.RTC_WeekDay;
	g_tTime.hour = RTC_TimeStruct.RTC_Hours;
	g_tTime.min = RTC_TimeStruct.RTC_Minutes;
	g_tTime.sec = RTC_TimeStruct.RTC_Seconds;
}

//��ȡ����ʱ��
void HOST_Get_SetTime_Ack(uint8_t * p,uint16_t *len, char* id)
{
	cJSON* cjson_test=NULL;
	char data_time[30];
	
	cjson_test=cJSON_CreateObject();
	
	RTC_GetTime(RTC_Format_BIN,&RTC_TimeStruct);//���ʱ��
	RTC_GetDate(RTC_Format_BIN, &RTC_DateStruct);//�������
	
	sprintf(data_time, "20%d-%d-%dT%d:%d:%dZ+0:0", 
					RTC_DateStruct.RTC_Year, RTC_DateStruct.RTC_Month, RTC_DateStruct.RTC_Date, 
					RTC_TimeStruct.RTC_Hours, RTC_TimeStruct.RTC_Minutes, RTC_TimeStruct.RTC_Seconds);
	
#if 1
	printf("��ѯϵͳʱ��%s\r\n",data_time);
#endif
	
	cJSON_AddStringToObject(cjson_test,"id",id);//����ַ����͵�
	cJSON_AddStringToObject(cjson_test,"dataTime",data_time);//����ַ����͵�
	
  char *cjson_data=cJSON_PrintUnformatted(cjson_test);
	memcpy(p,cjson_data,(unsigned int)strlen(cjson_data));
	*len=strlen(cjson_data);
	cJSON_Delete(cjson_test);
	
}


//��������ϵͳʱ��
void HOST_SetTime(cJSON *cjson)
{
	char * data_Time = NULL;
		TIME tTime;
	data_Time=cjson_get_string(cjson,"dateTime");
#if cJSON_Dbug
					printf("�������ں�ʱ��%s\r\n",data_Time);
#endif

	
	//2023-03-07T12:13:14Z+08:00
	tTime.year  = (data_Time[2] -48)*10+(data_Time[3]- 48);
	tTime.month = (data_Time[5] -48)*10+(data_Time[6]- 48);
	tTime.day   = (data_Time[8] -48)*10+(data_Time[9]- 48);
	tTime.hour  = (data_Time[11] -48)*10+(data_Time[12]- 48) + (data_Time[21] -48)*10+(data_Time[22]- 48);
	tTime.min   = (data_Time[14] -48)*10+(data_Time[15]- 48);
	tTime.sec   = (data_Time[17] -48)*10+(data_Time[18]- 48); 
	RTC_Set_Date(tTime.year,tTime.month,tTime.day,1);//��������
	RTC_Set_Time(tTime.hour,tTime.min,tTime.sec,RTC_H12_AM);//����ʱ��
}



