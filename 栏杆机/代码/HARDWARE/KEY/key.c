#include "key.h"
//E6 C13
void key_Init()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE,ENABLE); //使能GPIOE的时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC,ENABLE); //使能GPIOE的时钟
	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN;//输入
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_UP;  
	GPIO_Init(GPIOE,&GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN;//输入
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_UP;  
	GPIO_Init(GPIOC,&GPIO_InitStructure);
}
