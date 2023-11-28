#include "my_spi.h"

/**
  * @brief  ��ʼ��SPI2
  * @param  None
  * @retval None
  */
void spi_init(void){
	//����ʱ��	GPIOB spi2
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);
	
	//����	CS PB12	��ͨ����		SCK PB13	��������		MOSI PB15	��������  MISO PB14	��������
  	GPIO_InitTypeDef GPIO_InitStructure;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_15;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;
		GPIO_Init(GPIOB, &GPIO_InitStructure);
	
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
		GPIO_Init(GPIOB, &GPIO_InitStructure);
		
	//GPIO_Pin_12���øߵ�ƽ
		GPIO_SetBits(GPIOB, GPIO_Pin_12);
		
		//spi��ʼ��
		SPI_InitTypeDef SPI_InitStruct;
		SPI_Init(SPI1, &SPI_InitStruct);
	
}