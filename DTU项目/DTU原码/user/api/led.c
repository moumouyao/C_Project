#include "led.h"
#include "Delay.h"
#include "interrupt.h"

	/**
  * @brief  初始化LED灯
  * @param  uint32_t t	LED灯序号
  * @retval None
  */
void led_init(uint32_t t){
	//开启时钟
	RCC->APB2ENR |= 1 << 6; //使能开启GPIO_E的时钟 1000000
	//把 CLR 8 到 11 位设置成 0011
	GPIOE->CRL &= ~(0xF << (4 + t * 4));	//清零
	GPIOE->CRL |= 3 << (4 + t * 4);				//赋值
	GPIOE->ODR |= (1 << (t + 1));			//灭灯
}

	/**
  * @brief  初始化RGB灯
  * @param  None
  * @retval None
  */
void RGB_init(void){
	//开启时钟
	RCC->APB2ENR |= 1 << 2; //使能开启GPIO_A的时钟 1000000
	//把 CLR 8 到 11 位设置成 0011
	GPIOA->CRL &= ~(0xFF << 24);	//清零
	GPIOA->CRH &= ~(0XF << 0);
	GPIOA->CRL |= 0X33 << 24;				//赋值
	GPIOA->CRH |= 0X3 << 0;
	GPIOA->ODR |= 0X7 << 6;			//灭灯
}

	/**
  * @brief  点亮RGB灯
  * @param  uint32_t t	RGB灯序号
  * @retval None
  */
void RGB_enable(uint32_t t){
	 GPIOA->ODR &= ~(1 << (t + 5));	//输出低电平
}

	/**
  * @brief  熄灭RGB灯
  * @param  uint32_t t	RGB灯序号
  * @retval None
  */
void RGB_disable(uint32_t t){
	GPIOA->ODR |= (1 << (t + 5));	//输出高电平
}

	/**
  * @brief  点亮LED灯
  * @param  uint32_t t	LED灯序号
  * @retval None
  */
void led_enable(uint32_t t){
	GPIOE->ODR &= ~(1 << (t + 1));	//输出低电平
}

	/**
  * @brief  熄灭LED灯
  * @param  uint32_t t	LED灯序号
  * @retval None
  */
void led_disable(uint32_t t){
	GPIOE->ODR |= (1 << (t + 1));	//输出高电平
}

/**
  * @brief  LED灯RGB灯反转
  * @param  None
  * @retval None
  */
void led_turn(GPIO_TypeDef *led_port,uint16_t ledpin)
{
  led_port->ODR ^= (ledpin);
}

/**
  * @brief  流水灯
  * @param  None
  * @retval None
  */
void waterfall_light(void){
	led_init(LED1);
	led_init(LED2);
	led_init(LED3);
	led_init(LED4);
	while(1){
			led_enable(LED1);
			Delay_ms(200);
			led_disable(LED1);
			Delay_ms(20);
			led_enable(LED2);
			Delay_ms(200);
			led_disable(LED2);
			Delay_ms(20);
			led_enable(LED3);
			Delay_ms(200);
			led_disable(LED3);
			Delay_ms(20);
			led_enable(LED4);
			Delay_ms(200);
			led_disable(LED4);
			Delay_ms(20);
		}
}













