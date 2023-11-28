#include "delay.h"



static __IO uint32_t uwTimingDelay;
static __IO uint32_t uwTimingDelayus;


//构造一个1us的延时函数
void delay_1us()
{
		__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();
		__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();
		__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();
		__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();
		__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();
		__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();
		__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();
}


/**
  * @brief  微秒级延时
  * @param  xus 延时时长，范围：0~233015
  * @retval 无
  */
void Delay_us(uint32_t xus)
{
	 while(xus--)
	{
	  delay_1us();
	}
}

/**
  * @brief  系统定时器实现1ms延时
  * @param  nTime: specifies the delay time length, in milliseconds.
  * @retval None
  */
void Delay_ms(__IO uint32_t nTime)
{ 
	nTime = nTime * 1000;
  while(nTime--)
	{
	  delay_1us();
	}
}

/**
  * @brief  系统定时器实现1us延时
  * @param  nTime: specifies the delay time length, in milliseconds.
  * @retval None
  */
void delay_us(__IO uint32_t nTime)
{ 
  while(nTime--)
	{
	  delay_1us();
	}
}


/**
  * @brief  系统定时器中断服务函数中
  * @param  None
  * @retval None
  */
void TimingDelay_Decrement(void)
{
  if (uwTimingDelay != 0x00)
  { 
    uwTimingDelay--;
  }
}

/**
  * @brief  系统定时器中断服务函数中
  * @param  None
  * @retval None
  */
void TimingDelay_Decrementus(void)
{
  if (uwTimingDelayus != 0x00)
  { 
    uwTimingDelayus--;
  }
}




////函数功能：系统滴答中断-ms
////参数说明：nms延时时长
////返回值：无
////注意事项:nms最大值798.915
////时间：2018/11/20 
//void Systick_Interrupt(u16 nms)
//{
//	SysTick->CTRL &=~(0x01<<2);//选择时钟源（21M）
//	SysTick->LOAD=21*1000*nms;//设置重装载值（LOAD）
//	SysTick->VAL=0;//写VAL（清零VAL，COUNTFLAG清零，加载LOAD值到VAL）
//	SysTick->CTRL |=0x01<<1;//开滴答中断-模块级
//		//配置NVIC优先级
//	NVIC_SetPriority(SysTick_IRQn, NVIC_EncodePriority (7-2, 2, 2));
//	//系统级中断使能--内核中断不需要NVIC使能
////	NVIC_EnableIRQ(SysTick_IRQn);
//	
//	SysTick->CTRL |=0x01<<0;//开启定时器
//}

//void SysTick_Handler(void)
//{
//	OSIntEnter();
//	
//	SysTick->CTRL &=~(0x01<<16);
//	while(SysTick->CTRL&(0x01<<16));
//	OSTimeTick( );///
//	
//	OSIntExit();
//}





