#include "led.h"
#include "Delay.h"
#include "interrupt.h"

	/**
  * @brief  ��ʼ��LED��
  * @param  uint32_t t	LED�����
  * @retval None
  */
void led_init(uint32_t t){
	//����ʱ��
	RCC->APB2ENR |= 1 << 6; //ʹ�ܿ���GPIO_E��ʱ�� 1000000
	//�� CLR 8 �� 11 λ���ó� 0011
	GPIOE->CRL &= ~(0xF << (4 + t * 4));	//����
	GPIOE->CRL |= 3 << (4 + t * 4);				//��ֵ
	GPIOE->ODR |= (1 << (t + 1));			//���
}

	/**
  * @brief  ��ʼ��RGB��
  * @param  None
  * @retval None
  */
void RGB_init(void){
	//����ʱ��
	RCC->APB2ENR |= 1 << 2; //ʹ�ܿ���GPIO_A��ʱ�� 1000000
	//�� CLR 8 �� 11 λ���ó� 0011
	GPIOA->CRL &= ~(0xFF << 24);	//����
	GPIOA->CRH &= ~(0XF << 0);
	GPIOA->CRL |= 0X33 << 24;				//��ֵ
	GPIOA->CRH |= 0X3 << 0;
	GPIOA->ODR |= 0X7 << 6;			//���
}

	/**
  * @brief  ����RGB��
  * @param  uint32_t t	RGB�����
  * @retval None
  */
void RGB_enable(uint32_t t){
	 GPIOA->ODR &= ~(1 << (t + 5));	//����͵�ƽ
}

	/**
  * @brief  Ϩ��RGB��
  * @param  uint32_t t	RGB�����
  * @retval None
  */
void RGB_disable(uint32_t t){
	GPIOA->ODR |= (1 << (t + 5));	//����ߵ�ƽ
}

	/**
  * @brief  ����LED��
  * @param  uint32_t t	LED�����
  * @retval None
  */
void led_enable(uint32_t t){
	GPIOE->ODR &= ~(1 << (t + 1));	//����͵�ƽ
}

	/**
  * @brief  Ϩ��LED��
  * @param  uint32_t t	LED�����
  * @retval None
  */
void led_disable(uint32_t t){
	GPIOE->ODR |= (1 << (t + 1));	//����ߵ�ƽ
}

/**
  * @brief  LED��RGB�Ʒ�ת
  * @param  None
  * @retval None
  */
void led_turn(GPIO_TypeDef *led_port,uint16_t ledpin)
{
  led_port->ODR ^= (ledpin);
}

/**
  * @brief  ��ˮ��
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













