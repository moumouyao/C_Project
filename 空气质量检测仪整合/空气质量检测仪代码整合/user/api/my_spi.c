#include "my_spi.h"

/**
  * @brief  初始化SPI2
  * @param  None
  * @retval None
  */
void spi_init(void){
	//开启时钟	GPIOB spi2
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);
	
	//配置	CS PB12	普通推挽		SCK PB13	复用推挽		MOSI PB15	复用推挽  MISO PB14	浮空输入
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
		
	//GPIO_Pin_12设置高电平
		GPIO_SetBits(GPIOB, GPIO_Pin_12);
		
		//spi初始化
		SPI_InitTypeDef SPI_InitStruct;
		SPI_Init(SPI1, &SPI_InitStruct);
	
}