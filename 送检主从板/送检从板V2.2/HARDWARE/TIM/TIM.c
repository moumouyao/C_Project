#include "tim.h"

void TIM1_PWM_Init(uint16_t Period,uint16_t Prescaler,uint16_t CCR4_Val)
{
  /* Tim1 PWM的配置 */ 
  GPIO_InitTypeDef        GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_OCInitTypeDef       TIM_OCInitStructure;
	/* 打开GPIOE,TIM1的时钟 */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE ,ENABLE);//使能E时钟  
  
  GPIO_PinAFConfig(GPIOE,GPIO_PinSource11,GPIO_AF_TIM1);//GPIOE11复用为定时器1
	GPIO_PinAFConfig(GPIOE,GPIO_PinSource13,GPIO_AF_TIM1);//GPIOE13复用为定时器1
	GPIO_PinAFConfig(GPIOE,GPIO_PinSource14,GPIO_AF_TIM1);//GPIOE14复用为定时器1
  
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11|GPIO_Pin_13|GPIO_Pin_14;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽复用输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        //上拉
	GPIO_Init(GPIOE,&GPIO_InitStructure);
	
	/* Time base configuration */
	/* 定时器0频率为36M/Prescaler/(Period+1) */
	/* 当定时器从0计数到127，即为128次，为一个定时周期 */
	TIM_TimeBaseStructure.TIM_Period = Period;	//自动重装载值
	TIM_TimeBaseStructure.TIM_Prescaler = Prescaler;//定时器分频
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分频系数：不分频
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //向上计数

	TIM_TimeBaseInit(TIM1,&TIM_TimeBaseStructure); //初始化定时器1

  
	//初始化TIM9 Channel 1 PWM模式
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //配置为PWM模式1
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;//比较输出使能
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;//输出极性:TIM输出比较极性低
	
	TIM_OC2Init(TIM1,&TIM_OCInitStructure);  //根据T指定的参数初始化外设TIM3OC1
	TIM_OC3Init(TIM1,&TIM_OCInitStructure);  //根据T指定的参数初始化外设TIM3OC1
	TIM_OC4Init(TIM1,&TIM_OCInitStructure);  //根据T指定的参数初始化外设TIM3OC1
  /* 设置通道4的电平跳变值，输出另外一个占空比的PWM */
	TIM_OCInitStructure.TIM_Pulse = CCR4_Val;

	TIM_OC1PreloadConfig(TIM1,TIM_OCPreload_Enable);//使能TIM3在CCR1上的预装载寄存器

  TIM_ITConfig(TIM1,TIM_IT_Update,ENABLE); //允许定时器3更新中断
	TIM_ARRPreloadConfig(TIM1,ENABLE);   //ARPE使能 
	/* TIM3 enable counter */
	TIM_Cmd(TIM1,ENABLE);   //使能TIM3
	
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
