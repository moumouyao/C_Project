#include "delay.h"



static __IO uint32_t uwTimingDelay;
static __IO uint32_t uwTimingDelayus;


//����һ��1us����ʱ����
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
  * @brief  ΢�뼶��ʱ
  * @param  xus ��ʱʱ������Χ��0~233015
  * @retval ��
  */
void Delay_us(uint32_t xus)
{
	 while(xus--)
	{
	  delay_1us();
	}
}

/**
  * @brief  ϵͳ��ʱ��ʵ��1ms��ʱ
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
  * @brief  ϵͳ��ʱ��ʵ��1us��ʱ
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
  * @brief  ϵͳ��ʱ���жϷ�������
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
  * @brief  ϵͳ��ʱ���жϷ�������
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




////�������ܣ�ϵͳ�δ��ж�-ms
////����˵����nms��ʱʱ��
////����ֵ����
////ע������:nms���ֵ798.915
////ʱ�䣺2018/11/20 
//void Systick_Interrupt(u16 nms)
//{
//	SysTick->CTRL &=~(0x01<<2);//ѡ��ʱ��Դ��21M��
//	SysTick->LOAD=21*1000*nms;//������װ��ֵ��LOAD��
//	SysTick->VAL=0;//дVAL������VAL��COUNTFLAG���㣬����LOADֵ��VAL��
//	SysTick->CTRL |=0x01<<1;//���δ��ж�-ģ�鼶
//		//����NVIC���ȼ�
//	NVIC_SetPriority(SysTick_IRQn, NVIC_EncodePriority (7-2, 2, 2));
//	//ϵͳ���ж�ʹ��--�ں��жϲ���ҪNVICʹ��
////	NVIC_EnableIRQ(SysTick_IRQn);
//	
//	SysTick->CTRL |=0x01<<0;//������ʱ��
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





