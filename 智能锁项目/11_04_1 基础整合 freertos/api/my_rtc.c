#include "my_rtc.h"
#include "time.h"
/**
  * @brief  RTC��ʼ��
  * @param  None
  * @retval None
  */
void my_rtc_init(void){
	struct tm *time_info = NULL;
	time_t current_sec = 0;
	
	//ʹ�ܵ�Դʱ�ӣ���ʹ�� RTC �� RTC �󱸼Ĵ���д����
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);//ʹ�� PWR ʱ��
	PWR_BackupAccessCmd(ENABLE); //ʹ�ܺ󱸼Ĵ�������
	
	if(RTC_ReadBackupRegister(RTC_BKP_DR0)!=0x5050){//�Ƿ��һ������?
		//�����ⲿ����������ѡ�� RTC ʱ�ӣ���ʹ��
		RCC_LSEConfig(RCC_LSE_ON);//LSE ���� 
		/* Wait till LSI is ready */  
		while(RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET);
		RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE); //��ѡ�� LSE ��Ϊ RTC ʱ�� 
		RCC_RTCCLKCmd(ENABLE); //ʹ�� RTC ʱ��
		
		//��ʼ��RTC
		RTC_InitTypeDef RTC_InitStruct;
		RTC_InitStruct.RTC_AsynchPrediv = 0x7f;
		RTC_InitStruct.RTC_HourFormat = RTC_HourFormat_24;
		RTC_InitStruct.RTC_SynchPrediv = 0xff;
		RTC_Init(&RTC_InitStruct);
		
		current_sec = get_server_time();
		time_info = localtime(&current_sec);
		
		
		RTC_Set_Time(time_info->tm_hour + 8,time_info->tm_min,time_info->tm_sec,RTC_H12_AM); //����ʱ��
		RTC_Set_Date(14,5,5,1); //��������
		
		RTC_WriteBackupRegister(RTC_BKP_DR0,0x5050);//����Ѿ���ʼ������
	}
	else{
		printf("RTC�����ʼ��\r\n");
	}
}

//����ʱ��
void RTC_Set_Time(uint8_t hour, uint8_t min, uint8_t sec, uint8_t RTC_H12){
	
	RTC_TimeTypeDef RTC_TimeStruct;
	RTC_TimeStruct.RTC_Hours = hour;
	RTC_TimeStruct.RTC_Minutes = min;
	RTC_TimeStruct.RTC_Seconds = sec;
	RTC_TimeStruct.RTC_H12 = RTC_H12;
	RTC_SetTime(RTC_Format_BIN, &RTC_TimeStruct);
}

//����ʱ��
void RTC_Set_Date(uint8_t RTC_Year, int8_t RTC_Month, int8_t RTC_Date, int8_t RTC_WeekDay){
	
	RTC_DateTypeDef RTC_DateStruct;
	RTC_DateStruct.RTC_Date = RTC_Date;
	RTC_DateStruct.RTC_Month = RTC_Month;
	RTC_DateStruct.RTC_WeekDay = RTC_WeekDay;
	RTC_DateStruct.RTC_Year = RTC_Year;
	RTC_SetDate(RTC_Format_BIN, &RTC_DateStruct);
}

