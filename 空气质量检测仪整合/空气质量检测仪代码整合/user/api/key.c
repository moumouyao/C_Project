#include "key.h"
#include "Delay.h"
/**
  * @brief  ��ʼ������GPIO��
  * @param  None
  * @retval None
  */
void key_init(void){
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOC, ENABLE);
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	//��PA0��ʼ��λ��������ģʽ 
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	//��PC4~PC6��ʼ��λ��������ģʽ
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6;
		GPIO_Init(GPIOC, &GPIO_InitStructure);
}

/**
  * @brief  �������
  * @param  None
  * @retval �������·��ض�Ӧ�ļ�ֵ��û�а������·���0
  */
uint8_t key_get_num(void){
	uint8_t keynum = 0;
	//����1����
	if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0) == 1){
		Delay_ms(5);	//����
		if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0) != 1){
			return 0;
		}
		while(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0) == 1);
		Delay_ms(5);
		keynum = 1;
	}
	//����2����
	if(GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_4) == 0){
		Delay_ms(5);	//����
		if(GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_4) != 0){
			return 0;
		}
		while(GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_4) == 0);
		Delay_ms(5);
		keynum = 2;
	}
		//����3����
	if(GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_5) == 0){
		Delay_ms(20);	//����
		if(GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_5) != 0){
			return 0;
		}
		while(GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_5) == 0);
		Delay_ms(5);
		keynum = 3;
	}
			//����4����
	if(GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_6) == 0){
		Delay_ms(5);	//����
		if(GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_6) != 0){
			return 0;
		}
		while(GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_6) == 0);
		Delay_ms(5);
		keynum = 4;
	}
	return keynum;
}

/**
  * @brief  ����1�����ⲿ�ж� �ж���Ӧ �����ش���
	*						��ռ���ȼ� 1		��Ӧ���ȼ� 1
  * @param  None
  * @retval None
  */
void key1_interrupt_init(void){
    //����AFIO��ʱ��
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	// ����AFIO������ѡ������ѡ���ж�����
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource0);
	//��ʼ��EXIT
	EXTI_InitTypeDef EXTI_InitStruct;
	EXTI_InitStruct.EXTI_Line = EXTI_Line0;
	EXTI_InitStruct.EXTI_LineCmd = ENABLE;
	EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Rising;
	EXTI_Init(&EXTI_InitStruct);
	//�жϷ���
	//NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	//��ʼ��NVIC
	NVIC_InitTypeDef NVIC_InitStruct;
	NVIC_InitStruct.NVIC_IRQChannel = EXTI0_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 4;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
	NVIC_Init(&NVIC_InitStruct);
}

/**
  * @brief  ����2�����ⲿ�ж�	�ж���Ӧ �����ش���
	*						��ռ���ȼ� 1		��Ӧ���ȼ� 1
  * @param  None
  * @retval None
  */
void key2_interrupt_init(void){
    //����AFIO��ʱ��
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	// ����AFIO������ѡ������ѡ���ж�����
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource4);
	//��ʼ��EXIT
	EXTI_InitTypeDef EXTI_InitStruct;
	EXTI_InitStruct.EXTI_Line = EXTI_Line4;
	EXTI_InitStruct.EXTI_LineCmd = ENABLE;
	EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Rising;
	EXTI_Init(&EXTI_InitStruct);
	//�жϷ���
	//NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	//��ʼ��NVIC
	NVIC_InitTypeDef NVIC_InitStruct;
	NVIC_InitStruct.NVIC_IRQChannel = EXTI4_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 4;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
	NVIC_Init(&NVIC_InitStruct);
}

/**
  * @brief  ����3�����ⲿ�ж�	�ж���Ӧ �����ش���
	*						��ռ���ȼ� 1		��Ӧ���ȼ� 1
  * @param  None
  * @retval None
  */
void key3_interrupt_init(void){
    //����AFIO��ʱ��
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	// ����AFIO������ѡ������ѡ���ж�����
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource5);
	//��ʼ��EXIT
	EXTI_InitTypeDef EXTI_InitStruct;
	EXTI_InitStruct.EXTI_Line = EXTI_Line5;
	EXTI_InitStruct.EXTI_LineCmd = ENABLE;
	EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Rising;
	EXTI_Init(&EXTI_InitStruct);
	//�жϷ���
	//NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	//��ʼ��NVIC
	NVIC_InitTypeDef NVIC_InitStruct;
	NVIC_InitStruct.NVIC_IRQChannel = EXTI9_5_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 4;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
	NVIC_Init(&NVIC_InitStruct);
}

/**
  * @brief  ����4�����ⲿ�ж�	�ж���Ӧ �����ش���
	*						��ռ���ȼ� 1		��Ӧ���ȼ� 1
  * @param  None
  * @retval None
  */
void key4_interrupt_init(void){
    //����AFIO��ʱ��
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	// ����AFIO������ѡ������ѡ���ж�����
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource6);
	//��ʼ��EXIT
	EXTI_InitTypeDef EXTI_InitStruct;
	EXTI_InitStruct.EXTI_Line = EXTI_Line6;
	EXTI_InitStruct.EXTI_LineCmd = ENABLE;
	EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Rising;
	EXTI_Init(&EXTI_InitStruct);
	//�жϷ���
	//NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	//��ʼ��NVIC
	NVIC_InitTypeDef NVIC_InitStruct;
	NVIC_InitStruct.NVIC_IRQChannel = EXTI9_5_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 4;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
	NVIC_Init(&NVIC_InitStruct);
}


/**
  * @brief  �������Ƶ͹���ģʽ
	*						key1		˯��ģʽ
	*						key2		ͣ��ģʽ
	*						key3		����ģʽ
  * @param  None
  * @retval None
  */
void key_control(void){
	switch(key_get_num()){
		case 1:break;
		case 2:	
			printf("˯��ģʽ\r\n");	
			SysTick->CTRL &= 0xFFFE;
			__WFI();
			printf("�˳�˯��ģʽ\r\n");
			SysTick->CTRL |=1;
			break;
		case 3:	
			printf("ͣ��ģʽ\r\n");
			PWR_EnterSTOPMode(PWR_Regulator_LowPower, PWR_STOPEntry_WFI);
			SystemInit();
			printf("�˳�ͣ��ģʽ\r\n");
			break;
		case 4:
			printf("����ģʽ\r\n");
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);
			//ʹ�� RTC �ͺ󱸼Ĵ�������
			PWR_BackupAccessCmd(ENABLE);
			BKP_WriteBackupRegister(BKP_DR2, 0x01);	//д�������־λ
			PWR_BackupAccessCmd(DISABLE);
			PWR_ClearFlag(PWR_FLAG_WU);
			PWR_WakeUpPinCmd(ENABLE);
			PWR_EnterSTANDBYMode();
			break;
		default: break;
	}
}




















//�Ĵ����水������
/**
  * @brief  ��ʼ������
  * @param  None
  * @retval None
  */
//void key_init(void){
//	//PA0 ��������
//	RCC->APB2ENR |= 1 << 2; //ʹ�ܿ���GPIO_A��ʱ�� 
//	GPIOA->CRL &= ~0xF;	//���ƼĴ�������
//	GPIOA->CRL |= 0X4;				//��������
//}

/**
  * @brief  �������
  * @param  None
  * @retval key_num = 0 �ް�������
	* @retval key_num = 1 ����1����
	* @retval key_num = 2 ����2����
	* @retval key_num = 3 ����3����
	* @retval key_num = 4 ����4����
  */
//uint8_t key_check(void){
//	uint8_t key_val = 0;
//	if(GPIOA->IDR & 0X0001){		//���PA0״̬��ֻ��ע��0λ
//		Delay_ms(10);							//����
//		if(GPIOA->IDR & 0X0001){
//			while(GPIOA->IDR & 0X0001){};	
//			key_val = 1;
//		}
//		else{
//			key_val = 0;
//		}
//	}	
//	return key_val;
//}














