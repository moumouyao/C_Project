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
	TIM_TimeBaseInitStruct.TIM_Period = 10000 - 1;	//��ʱ1s
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
  	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 4;
  	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
  	NVIC_Init(&NVIC_InitStruct);
		
		//������ʱ��
		TIM_Cmd(TIM2, ENABLE);
}






