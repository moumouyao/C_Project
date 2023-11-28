#include "pwm.h"

/**
  * @brief  ��ʼ����ʱ��3��ʵ�����PWM�� ͨ��1 ͨ��2
  * @param  None
  * @retval None
  */
void pwm_tim3_init(void){
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);	//������ʱ��7ʱ��
	
		//��ʼ��PA6 PA7 �����������
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
  	GPIO_InitTypeDef GPIO_InitStructure;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	//ѡ���ڲ���Ԫʱ��
	TIM_InternalClockConfig(TIM3);
	
	//����ʱ����Ԫ 
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;	//����Ƶ
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;	//���ϼ���
	TIM_TimeBaseInitStruct.TIM_Period = 100 - 1;	
	TIM_TimeBaseInitStruct.TIM_Prescaler = 7200 - 1;
	TIM_TimeBaseInitStruct.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStruct);
	
	//��ʼ������Ƚϵ�Ԫ
	TIM_OCInitTypeDef TIM_OCInitStruct;
	TIM_OCStructInit(&TIM_OCInitStruct);	//�ṹ���ʼ��
	TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM1;	//PWMģʽ1 ���ϼ���
	TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_High;	//���øߵ�ƽ��Ч
	TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable;	//���ʹ��
	TIM_OCInitStruct.TIM_Pulse = 100;	//���ñȽ�ֵ
	TIM_OC1Init(TIM3, &TIM_OCInitStruct); //��ʼ��ͨ��1
	TIM_OC2Init(TIM3, &TIM_OCInitStruct);	//��ʼ��ͨ��2
	
	//ʹ�ܶ�ʱ���Զ�Ԥװ�ؼĴ���
	TIM_ARRPreloadConfig(TIM3, ENABLE);
	//ʹ�ܶ�ʱ��
	TIM_Cmd(TIM3, ENABLE);

}

/**
  * @brief  ��ʼ����ʱ��1���PWM��
  * @param  None
  * @retval None
  */
void pwm_tim1_init(void){
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);	//������ʱ��1ʱ��
	
		//��ʼ��PA8 �����������
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
  	GPIO_InitTypeDef GPIO_InitStructure;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	//ѡ���ڲ���Ԫʱ��
	TIM_InternalClockConfig(TIM1);
	//����ʱ����Ԫ 
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;	//����Ƶ
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;	//���ϼ���
	TIM_TimeBaseInitStruct.TIM_Period = 100 - 1;	
	TIM_TimeBaseInitStruct.TIM_Prescaler = 7200 - 1;
	TIM_TimeBaseInitStruct.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseInitStruct);
	
	//��ʼ������Ƚϵ�Ԫ
	TIM_OCInitTypeDef TIM_OCInitStruct;
	TIM_OCStructInit(&TIM_OCInitStruct);	//�ṹ���ʼ��
	TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM1;	//PWMģʽ1 ���ϼ���
	TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_High;	//���øߵ�ƽ��Ч
	TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable;	//���ʹ��
	TIM_OCInitStruct.TIM_Pulse = 100;	//���ñȽ�ֵ
	TIM_OCInitStruct.TIM_OCNIdleState = TIM_OCIdleState_Reset;
	TIM_OCInitStruct.TIM_OCNPolarity = TIM_OCNPolarity_High;
	TIM_OCInitStruct.TIM_OutputNState = TIM_OutputNState_Enable;
	TIM_OCInitStruct.TIM_OCIdleState = TIM_OCIdleState_Set;
	TIM_OC1Init(TIM1, &TIM_OCInitStruct); //��ʼ��ͨ��1
	
	//ʹ�ܶ�ʱ���Զ�Ԥװ�ؼĴ���
	TIM_ARRPreloadConfig(TIM1, ENABLE);
	//ʹ�ܶ�ʱ��
	TIM_Cmd(TIM1, ENABLE);
	//����TIM1���ⲿ�����
	TIM_CtrlPWMOutputs(TIM1, ENABLE);
	
}

/**
  * @brief  �ı�ͨ��1 pwm���Ƚ�ֵ
  * @param  TIM_TypeDef * TIM ��ʱ��
  * @param  uint16_t compare �µıȽ�ֵ
  * @retval None
  */
void pwm_set_compare1(TIM_TypeDef * TIM, uint16_t compare){
	TIM_SetCompare1(TIM, compare);
}

/**
  * @brief  �ı�ͨ��2 pwm���Ƚ�ֵ
  * @param  TIM_TypeDef * TIM ��ʱ��
  * @param  uint16_t compare �µıȽ�ֵ
  * @retval None
  */
void pwm_set_compare2(TIM_TypeDef * TIM, uint16_t compare){
	TIM_SetCompare2(TIM, compare);
}




