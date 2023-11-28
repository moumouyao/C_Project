#include "main.h"

void motor_init(void){
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; 
	GPIO_Init(GPIOB, &GPIO_InitStructure);
}

void open_door(uint8_t time){
	printf("电机运行\r\n");
	GPIO_SetBits(GPIOB, GPIO_Pin_3);
	GPIO_ResetBits(GPIOB, GPIO_Pin_4);
	Delay_ms(150);
	printf("锁已开\r\n");
	
	GPIO_ResetBits(GPIOB, GPIO_Pin_3);
	GPIO_ResetBits(GPIOB, GPIO_Pin_4);
	Delay_ms(time * 1000);
	
	GPIO_SetBits(GPIOB, GPIO_Pin_4);
	GPIO_ResetBits(GPIOB, GPIO_Pin_3);
	Delay_ms(150);
	printf("锁已关\r\n");
	
	GPIO_ResetBits(GPIOB, GPIO_Pin_3);
	GPIO_ResetBits(GPIOB, GPIO_Pin_4);
}