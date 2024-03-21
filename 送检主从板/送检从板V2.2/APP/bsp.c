#include "bsp.h"
#include "delay.h"
#include "led.h"
#include "key.h"
#include "usart.h"
#include "i2c.h"
#include "i2c2.h"
#include "tim.h"
#include "sw_led.h"
#include "sw_config.h"
#include "wdg.h"
#include "sw_led.h"
#include "user_crc.h"
#include "malloc.h"
#include "flash.h"
#include "spi.h"
#include "string.h"

void BSP_Init(void)
{
	mymem_init(SRAMIN);
	
	delay_init(168); //延时函数初始化
	uart_init(115200); //初始化串口
	uart2_init(115200);
	uart6_init(115200);
	
	LED_Init(); //初始化 LED
	Box_Door_GPIO_Init();//箱体门引脚初始化	

	G_IDSlave0=Read_IDSlave();

	User_CRCConfig();//CRC校验

	memset(USART3_BUF,0xff,USART3_BUF_SIZE);
	
	User_USART3Config (G_IDSlave0);

	User_I2CConfig();          //EEPROM初始化  
	User_I2C2Config();				 //SHT20初始化
	
	Sw_Led_Out_Init();
	Sw_Led_Int_Init();
	
	SPI_FlashInit();
	
	TIM1_PWM_Init(100,128,0); 
	PwmSet_R(10);	
	PwmSet_G(10);	
	PwmSet_B(10);	
		
//	IWDG_Init();//记得喂狗
}

//箱体GPIO初始化
void Box_Door_GPIO_Init(void){

	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOB | RCC_AHB1Periph_GPIOC | RCC_AHB1Periph_GPIOH,ENABLE); //使能GPIOA时钟
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_6; //GPIOC6与GPIOC7
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//输入功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//速度50MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL; //浮空
	GPIO_Init(GPIOA,&GPIO_InitStructure); //初始化PA9，PA10
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_2; //GPIOC6与GPIOC7
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
	GPIO_Init(GPIOC,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_Init(GPIOH,&GPIO_InitStructure);
	
}


