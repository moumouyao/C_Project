#include "key.h"
#include "Delay.h"
/**
  * @brief  初始化按键GPIO口
  * @param  None
  * @retval None
  */
void key_init(void){
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOC, ENABLE);
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	//将PA0初始化位下拉输入模式 
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	//将PC4~PC6初始化位上拉输入模式
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6;
		GPIO_Init(GPIOC, &GPIO_InitStructure);
}

/**
  * @brief  按键检测
  * @param  None
  * @retval 按键按下返回对应的键值，没有按键按下返回0
  */
uint8_t key_get_num(void){
	uint8_t keynum = 0;
	//按键1按下
	if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0) == 1){
		Delay_ms(5);	//消抖
		if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0) != 1){
			return 0;
		}
		while(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0) == 1);
		Delay_ms(5);
		keynum = 1;
	}
	//按键2按下
	if(GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_4) == 0){
		Delay_ms(5);	//消抖
		if(GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_4) != 0){
			return 0;
		}
		while(GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_4) == 0);
		Delay_ms(5);
		keynum = 2;
	}
		//按键3按下
	if(GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_5) == 0){
		Delay_ms(20);	//消抖
		if(GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_5) != 0){
			return 0;
		}
		while(GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_5) == 0);
		Delay_ms(5);
		keynum = 3;
	}
			//按键4按下
	if(GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_6) == 0){
		Delay_ms(5);	//消抖
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
  * @brief  按键1配置外部中断 中断响应 上升沿触发
	*						抢占优先级 1		响应优先级 1
  * @param  None
  * @retval None
  */
void key1_interrupt_init(void){
    //开启AFIO口时钟
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	// 配置AFIO的数据选择器，选择中断引脚
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource0);
	//初始化EXIT
	EXTI_InitTypeDef EXTI_InitStruct;
	EXTI_InitStruct.EXTI_Line = EXTI_Line0;
	EXTI_InitStruct.EXTI_LineCmd = ENABLE;
	EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Rising;
	EXTI_Init(&EXTI_InitStruct);
	//中断分组
	//NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	//初始化NVIC
	NVIC_InitTypeDef NVIC_InitStruct;
	NVIC_InitStruct.NVIC_IRQChannel = EXTI0_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 4;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
	NVIC_Init(&NVIC_InitStruct);
}

/**
  * @brief  按键2配置外部中断	中断响应 上升沿触发
	*						抢占优先级 1		响应优先级 1
  * @param  None
  * @retval None
  */
void key2_interrupt_init(void){
    //开启AFIO口时钟
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	// 配置AFIO的数据选择器，选择中断引脚
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource4);
	//初始化EXIT
	EXTI_InitTypeDef EXTI_InitStruct;
	EXTI_InitStruct.EXTI_Line = EXTI_Line4;
	EXTI_InitStruct.EXTI_LineCmd = ENABLE;
	EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Rising;
	EXTI_Init(&EXTI_InitStruct);
	//中断分组
	//NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	//初始化NVIC
	NVIC_InitTypeDef NVIC_InitStruct;
	NVIC_InitStruct.NVIC_IRQChannel = EXTI4_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 4;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
	NVIC_Init(&NVIC_InitStruct);
}

/**
  * @brief  按键3配置外部中断	中断响应 上升沿触发
	*						抢占优先级 1		响应优先级 1
  * @param  None
  * @retval None
  */
void key3_interrupt_init(void){
    //开启AFIO口时钟
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	// 配置AFIO的数据选择器，选择中断引脚
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource5);
	//初始化EXIT
	EXTI_InitTypeDef EXTI_InitStruct;
	EXTI_InitStruct.EXTI_Line = EXTI_Line5;
	EXTI_InitStruct.EXTI_LineCmd = ENABLE;
	EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Rising;
	EXTI_Init(&EXTI_InitStruct);
	//中断分组
	//NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	//初始化NVIC
	NVIC_InitTypeDef NVIC_InitStruct;
	NVIC_InitStruct.NVIC_IRQChannel = EXTI9_5_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 4;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
	NVIC_Init(&NVIC_InitStruct);
}

/**
  * @brief  按键4配置外部中断	中断响应 上升沿触发
	*						抢占优先级 1		响应优先级 1
  * @param  None
  * @retval None
  */
void key4_interrupt_init(void){
    //开启AFIO口时钟
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	// 配置AFIO的数据选择器，选择中断引脚
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource6);
	//初始化EXIT
	EXTI_InitTypeDef EXTI_InitStruct;
	EXTI_InitStruct.EXTI_Line = EXTI_Line6;
	EXTI_InitStruct.EXTI_LineCmd = ENABLE;
	EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Rising;
	EXTI_Init(&EXTI_InitStruct);
	//中断分组
	//NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	//初始化NVIC
	NVIC_InitTypeDef NVIC_InitStruct;
	NVIC_InitStruct.NVIC_IRQChannel = EXTI9_5_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 4;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
	NVIC_Init(&NVIC_InitStruct);
}


/**
  * @brief  按键控制低功耗模式
	*						key1		睡眠模式
	*						key2		停机模式
	*						key3		待机模式
  * @param  None
  * @retval None
  */
void key_control(void){
	switch(key_get_num()){
		case 1:break;
		case 2:	
			printf("睡眠模式\r\n");	
			SysTick->CTRL &= 0xFFFE;
			__WFI();
			printf("退出睡眠模式\r\n");
			SysTick->CTRL |=1;
			break;
		case 3:	
			printf("停机模式\r\n");
			PWR_EnterSTOPMode(PWR_Regulator_LowPower, PWR_STOPEntry_WFI);
			SystemInit();
			printf("退出停机模式\r\n");
			break;
		case 4:
			printf("待机模式\r\n");
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);
			//使能 RTC 和后备寄存器访问
			PWR_BackupAccessCmd(ENABLE);
			BKP_WriteBackupRegister(BKP_DR2, 0x01);	//写入待机标志位
			PWR_BackupAccessCmd(DISABLE);
			PWR_ClearFlag(PWR_FLAG_WU);
			PWR_WakeUpPinCmd(ENABLE);
			PWR_EnterSTANDBYMode();
			break;
		default: break;
	}
}




















//寄存器版按键控制
/**
  * @brief  初始化按键
  * @param  None
  * @retval None
  */
//void key_init(void){
//	//PA0 浮空输入
//	RCC->APB2ENR |= 1 << 2; //使能开启GPIO_A的时钟 
//	GPIOA->CRL &= ~0xF;	//控制寄存器清零
//	GPIOA->CRL |= 0X4;				//浮空输入
//}

/**
  * @brief  按键检测
  * @param  None
  * @retval key_num = 0 无按键按下
	* @retval key_num = 1 按键1按下
	* @retval key_num = 2 按键2按下
	* @retval key_num = 3 按键3按下
	* @retval key_num = 4 按键4按下
  */
//uint8_t key_check(void){
//	uint8_t key_val = 0;
//	if(GPIOA->IDR & 0X0001){		//检测PA0状态，只关注第0位
//		Delay_ms(10);							//消抖
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














