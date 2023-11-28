#include "RS485.h"
/**
  * @brief  RS485初始化
  * @param  None
  * @retval None
  */
void RS485_init(void){
	usart2_init();
	//使能引脚初始化
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
}

/**
  * @brief  使能485控制端(启动发送)
  * @param  None
  * @retval None
  */

void RS485_TX_ENABLE(void){
	GPIO_SetBits(GPIOA, GPIO_Pin_4);
}

/**
  * @brief  失能485控制端（改为接收）
  * @param  None
  * @retval None
  */
void RS485_RX_ENABLE(void){
	GPIO_ResetBits(GPIOA, GPIO_Pin_4);
}

























