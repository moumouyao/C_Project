#include "bsp.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "malloc.h"
#include "i2c.h"
#include "wdg.h"
#include "sw_app.h"
#include "SPI.h"
#include "sw_global.h"
#include "tim.h"
#include "led_gpio.h"
#include "key.h"
unsigned char *USART2_BUF;//DMA缓存数据指针
unsigned char *USART3_BUF;//DMA缓存数据指针
void BSP_Init(void)
{
	delay_init(168);       	//延时初始化
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	//中断分组配置
	LED_Init(); 			//LED初始化
	mymem_init(SRAMIN);  	//初始化外部内存池
	mymem_init(SRAMCCM); 	//初始化CCM内存池
	uart_init(115200);   	//串口波特率设置  -打印
	
 	TIM1_CH3_PWM_Init(100-1,168-1);	//168M/168=1Mhz的计数频率,重装载值1000，所以PWM频率为 1M/1000=10Khz. 		
	TIM_SetCompare3(TIM1,50);	//修改比较值，修改占空比
	LED_GPIO_Init();
	I2C_GPIOConf();			//24c02
//	u32 bsp=9600;
	uart_init_2(E2promReadByte(EEROM_USART_BSP_ADDR)<<16
						| E2promReadByte(EEROM_USART_BSP_ADDR+1)<<8
						| E2promReadByte(EEROM_USART_BSP_ADDR+2)); 	//与中心通讯
//	E2promWriteByte(EEROM_USART_BSP_ADDR,bsp>>16);
//	E2promWriteByte(EEROM_USART_BSP_ADDR+1,(bsp)>>8);
//	E2promWriteByte(EEROM_USART_BSP_ADDR+2,bsp);
	
	uart_init_3(9600); 	//与语音模块通讯

//	uint8_t data1[]={0x7E,0x05,0x41,0x00,0X01,0X45,0xEF}; //7E 05 41 00 01 45 EF
//	printf_usart3(data1,7);

	USART2_BUF = mymalloc(SRAMIN,USART2_BUF_SIZE);//申请内存
	MYDMA_Config(DMA1_Stream5,DMA_Channel_4,(u32)&USART2->DR,(u32)USART2_BUF,USART2_BUF_SIZE);//初始化接收DMA
	USART_DMACmd(USART2,USART_DMAReq_Rx,ENABLE); //使能串口2的DMA接收
  MYDMA_Enable(DMA1_Stream5,USART2_BUF_SIZE);     //开始第1次DMA传输！

	
	
//  I2C_GPIOConf();          //EEPROM初始化 
//	delay_ms(10);
//	GetE2PROMSet();     //读取E2PROM中参数,如果未设置则使用默认参数

	Init_SPI1();
	key_Init();
	IWDG_Init();//使能看门狗
}
