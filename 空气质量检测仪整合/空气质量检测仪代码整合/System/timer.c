#include "timer.h"

void timer2_init(void){
	//开启定时器2的时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	//选择内部单元时钟
	TIM_InternalClockConfig(TIM2);
	//配置时基单元
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;	//不分频
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;	//向上计数
	TIM_TimeBaseInitStruct.TIM_Period = 10000 - 1;	//定时1s
	TIM_TimeBaseInitStruct.TIM_Prescaler = 7200 - 1;
	TIM_TimeBaseInitStruct.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStruct);
	
	//清除更新中断
	TIM_ClearFlag(TIM2, TIM_IT_Update);
	//开启更新中断
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
	
		//初始化NVIC
  	NVIC_InitTypeDef NVIC_InitStruct;
  	NVIC_InitStruct.NVIC_IRQChannel = TIM2_IRQn;
  	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
  	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 4;
  	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
  	NVIC_Init(&NVIC_InitStruct);
		
		//启动定时器
		TIM_Cmd(TIM2, ENABLE);
}






