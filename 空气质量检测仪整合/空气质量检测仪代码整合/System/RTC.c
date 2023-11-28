#include "RTC.h"

void rtc_init(void){
	
	//������ʾRTC�Ѿ����ù�������Ҫ�ظ�����
	if (BKP_ReadBackupRegister(BKP_DR1) != 0xA5A5)
  {
    /* Backup data register value is not correct or not yet programmed (when
       the first time the program is executed) */

    printf("RTC not yet configured....\r\n\n");

    /* RTC Configuration */
    RTC_Configuration();

    printf("RTC configured....\r\n");

    /* Adjust time by values entered by the user on the hyperterminal */
    Time_Adjust();

    BKP_WriteBackupRegister(BKP_DR1, 0xA5A5);
  }
  else
  {
		//��鸴λԭ��
    /* Check if the Power On Reset flag is set */
    if (RCC_GetFlagStatus(RCC_FLAG_PORRST) != RESET)
    {
      printf("Power On Reset occurred....\r\n\n");
    }
    /* Check if the Pin Reset flag is set */
    else if (RCC_GetFlagStatus(RCC_FLAG_PINRST) != RESET)
    {
      printf("External Reset occurred....\r\n\n");
    }

    printf("No need to configure RTC....\r\n");
    /* Wait for RTC registers synchronization */
    RTC_WaitForSynchro();	//�ȴ��Ĵ���ͬ��

    /* Enable the RTC Second */
		NVIC_Configuration();
    RTC_ITConfig(RTC_IT_SEC, DISABLE);	//�������ж�
    /* Wait until last write operation on RTC registers has finished */
    RTC_WaitForLastTask();
  }
}

/**
  * @brief  Configures the RTC.
  * @param  None
  * @retval None
  */
void RTC_Configuration(void)
{
	//ʹ�ܵ�Դ��BKPʱ��
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);

	//ʹ�� RTC �ͺ󱸼Ĵ�������
  PWR_BackupAccessCmd(ENABLE);

	//������ BKP ��ȫ���Ĵ�������Ϊȱʡֵ
  BKP_DeInit();

	//�����ⲿ���پ���LSE��
  RCC_LSEConfig(RCC_LSE_ON);
	//�ȴ�LSE׼�����
  while (RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET)
  {}

		//���� RTC ʱ�ӣ�RTCCLK�� 
  RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);

	//ʹ��RTCʱ��
  RCC_RTCCLKCmd(ENABLE);

		//�ȴ�RTC�Ĵ���ͬ��
  RTC_WaitForSynchro();

		//�ȴ���һ��д�������
  RTC_WaitForLastTask();

		//�������ж�
  RTC_ITConfig(RTC_IT_SEC, DISABLE);

		//�ȴ���һ�β������
  RTC_WaitForLastTask();

	//����ʱ�ӷ�Ƶ
  RTC_SetPrescaler(32767); /* RTC period = RTCCLK/RTC_PR = (32.768 KHz)/(32767+1) */

	//�ȴ���һ�β������
  RTC_WaitForLastTask();
}

/**
  * @brief  Adjusts time.
  * @param  None
  * @retval None
  */
void Time_Adjust(void)
{
  /* Wait until last write operation on RTC registers has finished */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);//������Դ��BKPʱ��
	PWR_BackupAccessCmd(ENABLE);
  RTC_WaitForLastTask();
	
	//���� RTC ��������ֵ
  /* Change the current time */
		//RTC_SetCounter(Time_Regulate());
	RTC_SetCounter(get_server_time() + 8 * 60 * 60);
	// RTC_SetCounter(0);
  /* Wait until last write operation on RTC registers has finished */
  RTC_WaitForLastTask();
	PWR_BackupAccessCmd(DISABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, DISABLE);//������Դ��BKPʱ��
	
}



/**
  * @brief  Configures the nested vectored interrupt controller.
  * @param  None
  * @retval None
  */
void NVIC_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;

  /* Configure one bit for preemption priority */
  //NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

  /* Enable the RTC Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = RTC_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}

/**
  * @brief  Returns the time entered by user, using Hyperterminal.
  * @param  None
  * @retval Current time RTC counter value
  */
uint32_t Time_Regulate(void){
	
	struct tm current_time = {0, 30, 15, 25, 9 - 1, 2023 - 1900};
	return mktime(&current_time);
}



//������ʾʱ��
void show_time(void){
	time_t current_sec = 0;
	current_sec = RTC_GetCounter();
	struct tm *time_info = NULL;
	time_info = localtime(&current_sec);
	printf("%d-%d-%d %d:%d:%d\r\n", time_info->tm_year + 1900,
																	time_info->tm_mon + 1, 
																	time_info->tm_mday, 
																	time_info->tm_hour, 
																	time_info->tm_min, 
																	time_info->tm_sec);	
}


/**
  * @brief  ��ȡʱ���ַ���
  * @param  unsigned char * str	�������� ʱ���ַ��� 
  * @retval �ַ�������
  */
uint8_t get_time_str(unsigned char * str){
	time_t current_sec = 0;
	uint8_t flag = 0;
	current_sec = RTC_GetCounter();
	struct tm *time_info = NULL;
	time_info = localtime(&current_sec);
	if(time_info->tm_sec <= 9){
		flag |= 0x01;
	}
	if(time_info->tm_min <= 9){
		flag |= 0x02;
	}
	if(time_info->tm_hour <= 9){
		flag |= 0x04;
	}
	if(time_info->tm_mday <= 9){
		flag |= 0x08;
	}
	if((time_info->tm_mon + 1) <= 9){
		flag |= 0x10;
	}
		switch(flag){

  case 0: 
    sprintf((char*)str, "%d-%d-%d %d:%d:%d", time_info->tm_year + 1900, time_info->tm_mon + 1, time_info->tm_mday, time_info->tm_hour, time_info->tm_min, time_info->tm_sec);
    break;

  case 1:
    sprintf((char*)str, "%d-%d-%d %d:%d:0%d", time_info->tm_year + 1900, time_info->tm_mon + 1, time_info->tm_mday, time_info->tm_hour, time_info->tm_min, time_info->tm_sec);
    break;

  case 2:
    sprintf((char*)str, "%d-%d-%d %d:0%d:%d", time_info->tm_year + 1900, time_info->tm_mon + 1, time_info->tm_mday, time_info->tm_hour, time_info->tm_min, time_info->tm_sec);
    break;

  case 3:
    sprintf((char*)str, "%d-%d-%d %d:0%d:0%d", time_info->tm_year + 1900, time_info->tm_mon + 1, time_info->tm_mday, time_info->tm_hour, time_info->tm_min, time_info->tm_sec);
    break;

  case 4:
    sprintf((char*)str, "%d-%d-%d 0%d:%d:%d", time_info->tm_year + 1900, time_info->tm_mon + 1, time_info->tm_mday, time_info->tm_hour, time_info->tm_min, time_info->tm_sec);
    break;

  case 5: 
    sprintf((char*)str, "%d-%d-%d 0%d:%d:0%d", time_info->tm_year + 1900, time_info->tm_mon + 1, time_info->tm_mday, time_info->tm_hour, time_info->tm_min, time_info->tm_sec);
    break;

  case 6:
    sprintf((char*)str, "%d-%d-%d 0%d:0%d:%d", time_info->tm_year + 1900, time_info->tm_mon + 1, time_info->tm_mday, time_info->tm_hour, time_info->tm_min, time_info->tm_sec);
    break;

  case 7:
    sprintf((char*)str, "%d-%d-%d 0%d:0%d:0%d", time_info->tm_year + 1900, time_info->tm_mon + 1, time_info->tm_mday, time_info->tm_hour, time_info->tm_min, time_info->tm_sec);
    break;

  case 8:
    sprintf((char*)str, "%d-%d-0%d %d:%d:%d", time_info->tm_year + 1900, time_info->tm_mon + 1, time_info->tm_mday, time_info->tm_hour, time_info->tm_min, time_info->tm_sec);
    break;

  case 9:
    sprintf((char*)str, "%d-%d-0%d %d:%d:0%d", time_info->tm_year + 1900, time_info->tm_mon + 1, time_info->tm_mday, time_info->tm_hour, time_info->tm_min, time_info->tm_sec);
    break;

  case 10:
    sprintf((char*)str, "%d-%d-0%d %d:0%d:%d", time_info->tm_year + 1900, time_info->tm_mon + 1, time_info->tm_mday, time_info->tm_hour, time_info->tm_min, time_info->tm_sec);
    break;

  case 11:
    sprintf((char*)str, "%d-%d-0%d %d:0%d:0%d", time_info->tm_year + 1900, time_info->tm_mon + 1, time_info->tm_mday, time_info->tm_hour, time_info->tm_min, time_info->tm_sec);
    break;

  case 12:
    sprintf((char*)str, "%d-%d-0%d 0%d:%d:%d", time_info->tm_year + 1900, time_info->tm_mon + 1, time_info->tm_mday, time_info->tm_hour, time_info->tm_min, time_info->tm_sec);
    break;

  case 13:
    sprintf((char*)str, "%d-%d-0%d 0%d:%d:0%d", time_info->tm_year + 1900, time_info->tm_mon + 1, time_info->tm_mday, time_info->tm_hour, time_info->tm_min, time_info->tm_sec);
    break;

  case 14:
    sprintf((char*)str, "%d-%d-0%d 0%d:0%d:%d", time_info->tm_year + 1900, time_info->tm_mon + 1, time_info->tm_mday, time_info->tm_hour, time_info->tm_min, time_info->tm_sec); 
    break;

  case 15:
    sprintf((char*)str, "%d-%d-0%d 0%d:0%d:0%d", time_info->tm_year + 1900, time_info->tm_mon + 1, time_info->tm_mday, time_info->tm_hour, time_info->tm_min, time_info->tm_sec);
    break;

  case 16:
    sprintf((char*)str, "%d-0%d-%d %d:%d:%d", time_info->tm_year + 1900, time_info->tm_mon + 1, time_info->tm_mday, time_info->tm_hour, time_info->tm_min, time_info->tm_sec);
    break;

	case 17:
		sprintf((char*)str, "%d-0%d-%d %d:%d:0%d", time_info->tm_year + 1900, time_info->tm_mon + 1, time_info->tm_mday, time_info->tm_hour, time_info->tm_min, time_info->tm_sec);
		break;

	case 18:
		sprintf((char*)str, "%d-0%d-%d %d:0%d:%d", time_info->tm_year + 1900, time_info->tm_mon + 1, time_info->tm_mday, time_info->tm_hour, time_info->tm_min, time_info->tm_sec);
		break;

	case 19:
		sprintf((char*)str, "%d-0%d-%d %d:0%d:0%d", time_info->tm_year + 1900, time_info->tm_mon + 1, time_info->tm_mday, time_info->tm_hour, time_info->tm_min, time_info->tm_sec);
		break;  

	case 20:
		sprintf((char*)str, "%d-0%d-%d 0%d:%d:%d", time_info->tm_year + 1900, time_info->tm_mon + 1, time_info->tm_mday, time_info->tm_hour, time_info->tm_min, time_info->tm_sec);
		break;

	case 21:
		sprintf((char*)str, "%d-0%d-%d 0%d:%d:0%d", time_info->tm_year + 1900, time_info->tm_mon + 1, time_info->tm_mday, time_info->tm_hour, time_info->tm_min, time_info->tm_sec);
		break;

	case 22:
		sprintf((char*)str, "%d-0%d-%d 0%d:0%d:%d", time_info->tm_year + 1900, time_info->tm_mon + 1, time_info->tm_mday, time_info->tm_hour, time_info->tm_min, time_info->tm_sec);
		break;  

	case 23:
		sprintf((char*)str, "%d-0%d-%d 0%d:0%d:0%d", time_info->tm_year + 1900, time_info->tm_mon + 1, time_info->tm_mday, time_info->tm_hour, time_info->tm_min, time_info->tm_sec);
		break;

	case 24:
		sprintf((char*)str, "%d-0%d-0%d %d:%d:%d", time_info->tm_year + 1900, time_info->tm_mon + 1, time_info->tm_mday, time_info->tm_hour, time_info->tm_min, time_info->tm_sec);
		break;

	case 25:
		sprintf((char*)str, "%d-0%d-0%d %d:%d:0%d", time_info->tm_year + 1900, time_info->tm_mon + 1, time_info->tm_mday, time_info->tm_hour, time_info->tm_min, time_info->tm_sec);
		break;

	case 26:
		sprintf((char*)str, "%d-0%d-0%d %d:0%d:%d", time_info->tm_year + 1900, time_info->tm_mon + 1, time_info->tm_mday, time_info->tm_hour, time_info->tm_min, time_info->tm_sec);
		break;

	case 27:
		sprintf((char*)str, "%d-0%d-0%d %d:0%d:0%d", time_info->tm_year + 1900, time_info->tm_mon + 1, time_info->tm_mday, time_info->tm_hour, time_info->tm_min, time_info->tm_sec);
		break;

	case 28:
		sprintf((char*)str, "%d-0%d-0%d 0%d:%d:%d", time_info->tm_year + 1900, time_info->tm_mon + 1, time_info->tm_mday, time_info->tm_hour, time_info->tm_min, time_info->tm_sec);
		break;

	case 29:
		sprintf((char*)str, "%d-0%d-0%d 0%d:%d:0%d", time_info->tm_year + 1900, time_info->tm_mon + 1, time_info->tm_mday, time_info->tm_hour, time_info->tm_min, time_info->tm_sec);
		break;

	case 30:
		sprintf((char*)str, "%d-0%d-0%d 0%d:0%d:%d", time_info->tm_year + 1900, time_info->tm_mon + 1, time_info->tm_mday, time_info->tm_hour, time_info->tm_min, time_info->tm_sec);
		break;
	
  case 31: 
    sprintf((char*)str, "%d-0%d-0%d %d:%d:%d", time_info->tm_year + 1900, time_info->tm_mon + 1, time_info->tm_mday, time_info->tm_hour, time_info->tm_min, time_info->tm_sec);
    break;

  default:
		sprintf((char*)str, "%d-%d-%d %d:%d:%d", time_info->tm_year + 1900, time_info->tm_mon + 1, time_info->tm_mday, time_info->tm_hour, time_info->tm_min, time_info->tm_sec);
    break;
}
								
	
	return strlen((char*)str);
}


