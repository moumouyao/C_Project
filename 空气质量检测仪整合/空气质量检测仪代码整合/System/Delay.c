#include "Delay.h"

#define xPortSysTickHandler SysTick_Handler

uint32_t led_tick[2] = {0, 500}, key_tick[2] = {0, 20};
uint32_t  d_ms[2] = {0, 0}, d_us[2] = {0, 0}, d_s[2] = {0, 0};
uint32_t su_03t[2] = {0, 15},	kqm[2] = {0, 20}, dht11[2] = {0, 500}, 
				ADC_DMA[2] = {0, 20}, lowpor[2] = {0, 20}, current_time[2] = {0, 20},
				current_time2LCD[2] = {0, 20}, show_data2lcd[2] = {0, 20};//语音模块时间片

//构造一个1us的延时函数
void delay_1us()
{
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
//	d_us[1] = xus;
//	d_us[0] = 0;
//	while(d_us[0] < d_us[1]);
	 while(xus--)
	{
	  delay_1us();
	}
}
/**
  * @brief  毫秒级延时
  * @param  xms 延时时长，范围：0~4294967295
  * @retval 无
  */
void Delay_ms(uint32_t xms)
{
//	d_ms[0] = 0;
//	d_ms[1] = xms;
//	while(d_ms[0] < d_ms[1]);
	xms=1000*xms;
	while(xms--)
	{
	  delay_1us();
	}
}
 
/**
  * @brief  秒级延时
  * @param  xs 延时时长，范围：0~4294967295
  * @retval 无
  */
void Delay_s(uint32_t xs)
{
	d_s[0] = 0;
	d_s[1] = xs  * 1000 ;
	while(d_s[0] < d_s[1]);
} 

/**
  * @brief  配置系统定时器
	*						每1us产生一次中断
  * @param  None
  * @retval None
  */
void sys_tick_init(void){
	SysTick_Config(72000000 / 1000);	//系统定时器1us产生一次中断
}

/**
  * @brief  su_03t设置时间片
  * @param  uint8_t time	唤醒时间
  * @retval None
  */
void su_time_scale(uint8_t time){
	if(su_03t[0] > time){
		vioce_analysis();
		su_03t[0] = 0;
	}
}

/**
  * @brief  su_03t设置时间片
  * @param  uint8_t time	唤醒时间
  * @retval None
  */
void kqm_time_scale(uint8_t time){
	if(kqm[0] > time){
		air_analysis();
		kqm[0] = 0;
	}
}

/**
  * @brief  dht11设置时间片
  * @param  uint8_t time	唤醒时间
  * @retval None
  */
void dht_time_scale(uint16_t time){
	if(dht11[0] > time){
		//printf("\r\n%d\r\n", dht_get_val());
		dht11[0] = 0;
	}
	
}

/**
  * @brief  ADC1 2 DMA设置时间片
  * @param  uint8_t time	唤醒时间
  * @retval None
  */
void ADC_time_scale(uint16_t time){
	if(ADC_DMA[0] > time){
		ADC2VOL();
		ADC_DMA[0] = 0;
	}
}

/**
  * @brief  低功耗时间片
  * @param  uint8_t time	唤醒时间
  * @retval None
  */
void lowpor_time_scale(uint16_t time){
	if(lowpor[0] > time){
		key_control();
		lowpor[0] = 0;
	}
}
/**
* @brief  串口显示时间时间片
  * @param  uint8_t time	唤醒时间
  * @retval None
  */
void current_time_scale(uint16_t time){
	if(current_time[0] > time){
		show_time();
		current_time[0] = 0;
	}
}
///**
//* @brief  LCD显示时间时间片
//  * @param  uint8_t time	唤醒时间
//  * @retval None
//  */
//void current_time2LCD_scale(uint16_t time){
//	if(current_time2LCD[0] > time){
//		show_time2LCD();
//		current_time2LCD[0] = 0;
//	}
//}

///**
//* @brief  LCD显示数据时间片
//  * @param  uint8_t time	唤醒时间
//  * @retval None
//  */
//void show_data2LCD_scale(uint16_t time){
//	if(show_data2lcd[0] > time){
//		show_data2LCD();
//		show_data2lcd[0] = 0;
//	}
//}



/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
//void SysTick_Handler(void)
//{
//	led_tick[0]++;
//	key_tick[0]++;
//	d_ms[0]++;
//	d_us[0]++;
//	d_s[0]++;
//	su_03t[0]++;
//	kqm[0]++;
//	dht11[0]++;
//	ADC_DMA[0]++;
//	lowpor[0]++;
//	current_time[0]++;
//	current_time2LCD[0]++;
//	show_data2lcd[0]++;
//}

void xPortSysTickHandler( void )
{
//		led_tick[0]++;
//	key_tick[0]++;
//	d_ms[0]++;
//	d_us[0]++;
//	d_s[0]++;
//	su_03t[0]++;
//	kqm[0]++;
//	dht11[0]++;
//	ADC_DMA[0]++;
//	lowpor[0]++;
//	current_time[0]++;
//	current_time2LCD[0]++;
//	show_data2lcd[0]++;
	
	/* The SysTick runs at the lowest interrupt priority, so when this interrupt
	executes all interrupts must be unmasked.  There is therefore no need to
	save and then restore the interrupt mask value as its value is already
	known - therefore the slightly faster vPortRaiseBASEPRI() function is used
	in place of portSET_INTERRUPT_MASK_FROM_ISR(). */
	vPortRaiseBASEPRI();
	{
		/* Increment the RTOS tick. */
		if( xTaskIncrementTick() != pdFALSE )
		{
			/* A context switch is required.  Context switching is performed in
			the PendSV interrupt.  Pend the PendSV interrupt. */
			portNVIC_INT_CTRL_REG = portNVIC_PENDSVSET_BIT;
		}
	}
	vPortClearBASEPRIFromISR();
}
