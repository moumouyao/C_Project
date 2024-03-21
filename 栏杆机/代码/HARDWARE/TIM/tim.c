#include "tim.h"


void TIM1_CH3_PWM_Init(u16 arr,u16 psc)//PE13
{		

    GPIO_InitTypeDef GPIO_InitStructure;
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    TIM_OCInitTypeDef  TIM_OCInitStructure;
    
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);      //TIM1时钟使能    
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);     //使能PORTF时钟    
    
    GPIO_PinAFConfig(GPIOE,GPIO_PinSource13,GPIO_AF_TIM1); //GPIOA11复用为定时器1
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;           //GPIOA11
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        //复用功能
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;    //速度100MHz
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      //推挽复用输出
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;        //上拉
    GPIO_Init(GPIOE,&GPIO_InitStructure);              //初始化GPIOA11
      
    TIM_TimeBaseStructure.TIM_Prescaler=psc;  //定时器分频
    TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Down; //向上计数模式
    TIM_TimeBaseStructure.TIM_Period=arr;   //自动重装载值
    TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
    TIM_TimeBaseStructure.TIM_RepetitionCounter=0;  //20201020
    TIM_TimeBaseInit(TIM1,&TIM_TimeBaseStructure);//初始化定时器14
    
    //初始化TIM14 Channel1 PWM模式     
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //选择定时器模式:TIM脉冲宽度调制模式2
		TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCNPolarity_High; //输出极性:TIM输出比较极性高
    TIM_OC3Init(TIM1, &TIM_OCInitStructure);  //根据T指定的参数初始化外设TIM1 4OC1
 
    TIM_OC3PreloadConfig(TIM1, TIM_OCPreload_Enable);  //使能TIM1在CCR1上的预装载寄存器
 
    TIM_ARRPreloadConfig(TIM1,ENABLE);//ARPE使能 
    
    TIM_Cmd(TIM1, ENABLE);  //使能TIM1
    
		TIM_CtrlPWMOutputs(TIM1, ENABLE);//使能TIM1的PWM输出，TIM1与TIM8有效,如果没有这行会问题

} 


	
	
	
