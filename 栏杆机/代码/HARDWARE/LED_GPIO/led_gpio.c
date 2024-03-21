#include "led_gpio.h"

void LED_GPIO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE); //使能GPIOE的时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE); //使能GPIOE的时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC,ENABLE); //使能GPIOE的时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD,ENABLE); //使能GPIOE的时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE,ENABLE); //使能GPIOE的时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG,ENABLE); //使能GPIOE的时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF,ENABLE); //使能GPIOE的时钟
	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_14|GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_OUT;//输出
	GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;  //推挽输出
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_NOPULL;  
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_100MHz; //高速GPIO
	GPIO_Init(GPIOE,&GPIO_InitStructure);
	
	GPIO_ResetBits(GPIOE,GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_14|GPIO_Pin_15); 
	R11=0;	R12=0;	G11=0;	G12=0;	B11=0;	B12=0;

	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_OUT;//输出
	GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;  //推挽输出
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_NOPULL;  
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_100MHz; //高速GPIO
	GPIO_Init(GPIOC,&GPIO_InitStructure);
	
	GPIO_ResetBits(GPIOC,GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9); //GPIOF9,10高电平
	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_OUT;//输出
	GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;  //推挽输出
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_NOPULL;  
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_100MHz; //高速GPIO
	GPIO_Init(GPIOG,&GPIO_InitStructure);
	
	GPIO_ResetBits(GPIOG,GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8); //GPIOF9,10高电平
	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_OUT;//输出
	GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;  //推挽输出
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_NOPULL;  
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_100MHz; //高速GPIO
	GPIO_Init(GPIOD,&GPIO_InitStructure);
	
	GPIO_ResetBits(GPIOD,GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15); //GPIOF9,10高电平
	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_OUT;//输出
	GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;  //推挽输出
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_NOPULL;  
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_100MHz; //高速GPIO
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	GPIO_ResetBits(GPIOA,GPIO_Pin_8); //GPIOF9,10高电平

	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_OUT;//输出
	GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;  //推挽输出
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_NOPULL;  
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_100MHz; //高速GPIO
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	
	GPIO_ResetBits(GPIOB,GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15); //GPIOF9,10高电平

	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_OUT;//输出
	GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;  //推挽输出
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_NOPULL;  
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_100MHz; //高速GPIO
	GPIO_Init(GPIOF,&GPIO_InitStructure);
	
	GPIO_ResetBits(GPIOF,GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15); //GPIOF9,10高电平
	
//	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_14;
//	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN;//输出
//	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_UP;  
//	GPIO_Init(GPIOC,&GPIO_InitStructure);	
	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN;//输出
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_UP;  
	GPIO_Init(GPIOF,&GPIO_InitStructure);	
}
