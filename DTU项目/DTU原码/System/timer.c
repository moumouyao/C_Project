#include "timer.h"

void timer2_init(void){
	//������ʱ��2��ʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	//ѡ���ڲ���Ԫʱ��
	TIM_InternalClockConfig(TIM2);
	//����ʱ����Ԫ
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;	//����Ƶ
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;	//���ϼ���
	TIM_TimeBaseInitStruct.TIM_Period = 10 - 1;	//��ʱ1s
	TIM_TimeBaseInitStruct.TIM_Prescaler = 7200 - 1;
	TIM_TimeBaseInitStruct.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStruct);
	
	//��������ж�
	TIM_ClearFlag(TIM2, TIM_IT_Update);
	//���������ж�
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
	
		//��ʼ��NVIC
  	NVIC_InitTypeDef NVIC_InitStruct;
  	NVIC_InitStruct.NVIC_IRQChannel = TIM2_IRQn;
  	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
  	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1;
  	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1;
  	NVIC_Init(&NVIC_InitStruct);
		
		//������ʱ��
		TIM_Cmd(TIM2, ENABLE);
}



/**
  * @brief  ��ʼ����ʱ��3��ʵ�����PWM��
  * @param  None
  * @retval None
  */
void time3_init(void){
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);	//������ʱ��7ʱ��
		//��ʼ��PA6
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
  	GPIO_InitTypeDef GPIO_InitStructure;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  	GPIO_Init(GPIOA, &GPIO_InitStructure);
	//ѡ���ڲ���Ԫʱ��
	TIM_InternalClockConfig(TIM2);
	//����ʱ����Ԫ
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;	//����Ƶ
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;	//���ϼ���
	TIM_TimeBaseInitStruct.TIM_Period = 100 - 1;	//��ʱ1s
	TIM_TimeBaseInitStruct.TIM_Prescaler = 720 - 1;
	TIM_TimeBaseInitStruct.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStruct);
	
	//��ʼ������Ƚϵ�Ԫ
	TIM_OCInitTypeDef TIM_OCInitStruct;
	TIM_OCStructInit(&TIM_OCInitStruct);
	TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStruct.TIM_Pulse = 100;
	TIM_OC1Init(TIM3, &TIM_OCInitStruct);
	
	//ʹ�ܶ�ʱ��
	TIM_Cmd(TIM3, ENABLE);
}

/**
  * @brief  �ı�pwm��
  * @param  None
  * @retval None
  */
void pwm_set_compare1(uint16_t compare){
	TIM_SetCompare1(TIM3, compare);
}




