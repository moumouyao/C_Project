#include "tim.h"

void TIM1_PWM_Init(uint16_t Period,uint16_t Prescaler,uint16_t CCR4_Val)
{
  /* Tim1 PWM������ */ 
  GPIO_InitTypeDef        GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_OCInitTypeDef       TIM_OCInitStructure;
	/* ��GPIOE,TIM1��ʱ�� */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE ,ENABLE);//ʹ��Eʱ��  
  
  GPIO_PinAFConfig(GPIOE,GPIO_PinSource11,GPIO_AF_TIM1);//GPIOE11����Ϊ��ʱ��1
	GPIO_PinAFConfig(GPIOE,GPIO_PinSource13,GPIO_AF_TIM1);//GPIOE13����Ϊ��ʱ��1
	GPIO_PinAFConfig(GPIOE,GPIO_PinSource14,GPIO_AF_TIM1);//GPIOE14����Ϊ��ʱ��1
  
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11|GPIO_Pin_13|GPIO_Pin_14;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//���츴�����
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        //����
	GPIO_Init(GPIOE,&GPIO_InitStructure);
	
	/* Time base configuration */
	/* ��ʱ��0Ƶ��Ϊ36M/Prescaler/(Period+1) */
	/* ����ʱ����0������127����Ϊ128�Σ�Ϊһ����ʱ���� */
	TIM_TimeBaseStructure.TIM_Period = Period;	//�Զ���װ��ֵ
	TIM_TimeBaseStructure.TIM_Prescaler = Prescaler;//��ʱ����Ƶ
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //����ʱ�ӷ�Ƶϵ��������Ƶ
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //���ϼ���

	TIM_TimeBaseInit(TIM1,&TIM_TimeBaseStructure); //��ʼ����ʱ��1

  
	//��ʼ��TIM9 Channel 1 PWMģʽ
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //����ΪPWMģʽ1
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;//�Ƚ����ʹ��
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;//�������:TIM����Ƚϼ��Ե�
	
	TIM_OC2Init(TIM1,&TIM_OCInitStructure);  //����Tָ���Ĳ�����ʼ������TIM3OC1
	TIM_OC3Init(TIM1,&TIM_OCInitStructure);  //����Tָ���Ĳ�����ʼ������TIM3OC1
	TIM_OC4Init(TIM1,&TIM_OCInitStructure);  //����Tָ���Ĳ�����ʼ������TIM3OC1
  /* ����ͨ��4�ĵ�ƽ����ֵ���������һ��ռ�ձȵ�PWM */
	TIM_OCInitStructure.TIM_Pulse = CCR4_Val;

	TIM_OC1PreloadConfig(TIM1,TIM_OCPreload_Enable);//ʹ��TIM3��CCR1�ϵ�Ԥװ�ؼĴ���

  TIM_ITConfig(TIM1,TIM_IT_Update,ENABLE); //����ʱ��3�����ж�
	TIM_ARRPreloadConfig(TIM1,ENABLE);   //ARPEʹ�� 
	/* TIM3 enable counter */
	TIM_Cmd(TIM1,ENABLE);   //ʹ��TIM3
	
	TIM_CtrlPWMOutputs(TIM1,ENABLE);

}

void PwmSet_R(uint16_t value)
{
	TIM_SetCompare2(TIM1,value);
}

void PwmSet_G(uint16_t value)
{
	TIM_SetCompare3(TIM1,value);
}

void PwmSet_B(uint16_t value)
{
	TIM_SetCompare4(TIM1,value);
}
