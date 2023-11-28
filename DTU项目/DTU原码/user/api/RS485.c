#include "RS485.h"
/**
  * @brief  RS485��ʼ��
  * @param  None
  * @retval None
  */
void RS485_init(void){
	usart2_init();
	//ʹ�����ų�ʼ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
}

/**
  * @brief  ʹ��485���ƶ�(��������)
  * @param  None
  * @retval None
  */

void RS485_TX_ENABLE(void){
	GPIO_SetBits(GPIOA, GPIO_Pin_4);
}

/**
  * @brief  ʧ��485���ƶˣ���Ϊ���գ�
  * @param  None
  * @retval None
  */
void RS485_RX_ENABLE(void){
	GPIO_ResetBits(GPIOA, GPIO_Pin_4);
}

























