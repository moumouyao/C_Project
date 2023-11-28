#include "pwm.h"

/**
  * @brief  初始化定时器3，实现输出PWM波 通道1 通道2
  * @param  None
  * @retval None
  */
void pwm_tim3_init(void){
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);	//开启定时器7时钟
	
		//初始化PA6 PA7 复用推挽输出
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
  	GPIO_InitTypeDef GPIO_InitStructure;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	//选择内部单元时钟
	TIM_InternalClockConfig(TIM3);
	
	//配置时基单元 
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;	//不分频
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;	//向上计数
	TIM_TimeBaseInitStruct.TIM_Period = 100 - 1;	
	TIM_TimeBaseInitStruct.TIM_Prescaler = 7200 - 1;
	TIM_TimeBaseInitStruct.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStruct);
	
	//初始化输出比较单元
	TIM_OCInitTypeDef TIM_OCInitStruct;
	TIM_OCStructInit(&TIM_OCInitStruct);	//结构体初始化
	TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM1;	//PWM模式1 向上计数
	TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_High;	//设置高电平有效
	TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable;	//输出使能
	TIM_OCInitStruct.TIM_Pulse = 100;	//设置比较值
	TIM_OC1Init(TIM3, &TIM_OCInitStruct); //初始化通道1
	TIM_OC2Init(TIM3, &TIM_OCInitStruct);	//初始化通道2
	
	//使能定时器自动预装载寄存器
	TIM_ARRPreloadConfig(TIM3, ENABLE);
	//使能定时器
	TIM_Cmd(TIM3, ENABLE);

}

/**
  * @brief  初始化定时器1输出PWM波
  * @param  None
  * @retval None
  */
void pwm_tim1_init(void){
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);	//开启定时器1时钟
	
		//初始化PA8 复用推挽输出
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
  	GPIO_InitTypeDef GPIO_InitStructure;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	//选择内部单元时钟
	TIM_InternalClockConfig(TIM1);
	//配置时基单元 
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;	//不分频
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;	//向上计数
	TIM_TimeBaseInitStruct.TIM_Period = 100 - 1;	
	TIM_TimeBaseInitStruct.TIM_Prescaler = 7200 - 1;
	TIM_TimeBaseInitStruct.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseInitStruct);
	
	//初始化输出比较单元
	TIM_OCInitTypeDef TIM_OCInitStruct;
	TIM_OCStructInit(&TIM_OCInitStruct);	//结构体初始化
	TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM1;	//PWM模式1 向上计数
	TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_High;	//设置高电平有效
	TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable;	//输出使能
	TIM_OCInitStruct.TIM_Pulse = 100;	//设置比较值
	TIM_OCInitStruct.TIM_OCNIdleState = TIM_OCIdleState_Reset;
	TIM_OCInitStruct.TIM_OCNPolarity = TIM_OCNPolarity_High;
	TIM_OCInitStruct.TIM_OutputNState = TIM_OutputNState_Enable;
	TIM_OCInitStruct.TIM_OCIdleState = TIM_OCIdleState_Set;
	TIM_OC1Init(TIM1, &TIM_OCInitStruct); //初始化通道1
	
	//使能定时器自动预装载寄存器
	TIM_ARRPreloadConfig(TIM1, ENABLE);
	//使能定时器
	TIM_Cmd(TIM1, ENABLE);
	//启动TIM1的外部主输出
	TIM_CtrlPWMOutputs(TIM1, ENABLE);
	
}

/**
  * @brief  改变通道1 pwm波比较值
  * @param  TIM_TypeDef * TIM 定时器
  * @param  uint16_t compare 新的比较值
  * @retval None
  */
void pwm_set_compare1(TIM_TypeDef * TIM, uint16_t compare){
	TIM_SetCompare1(TIM, compare);
}

/**
  * @brief  改变通道2 pwm波比较值
  * @param  TIM_TypeDef * TIM 定时器
  * @param  uint16_t compare 新的比较值
  * @retval None
  */
void pwm_set_compare2(TIM_TypeDef * TIM, uint16_t compare){
	TIM_SetCompare2(TIM, compare);
}




