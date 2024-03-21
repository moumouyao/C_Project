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
unsigned char *USART2_BUF;//DMA��������ָ��
unsigned char *USART3_BUF;//DMA��������ָ��
void BSP_Init(void)
{
	delay_init(168);       	//��ʱ��ʼ��
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	//�жϷ�������
	LED_Init(); 			//LED��ʼ��
	mymem_init(SRAMIN);  	//��ʼ���ⲿ�ڴ��
	mymem_init(SRAMCCM); 	//��ʼ��CCM�ڴ��
	uart_init(115200);   	//���ڲ���������  -��ӡ
	
 	TIM1_CH3_PWM_Init(100-1,168-1);	//168M/168=1Mhz�ļ���Ƶ��,��װ��ֵ1000������PWMƵ��Ϊ 1M/1000=10Khz. 		
	TIM_SetCompare3(TIM1,50);	//�޸ıȽ�ֵ���޸�ռ�ձ�
	LED_GPIO_Init();
	I2C_GPIOConf();			//24c02
//	u32 bsp=9600;
	uart_init_2(E2promReadByte(EEROM_USART_BSP_ADDR)<<16
						| E2promReadByte(EEROM_USART_BSP_ADDR+1)<<8
						| E2promReadByte(EEROM_USART_BSP_ADDR+2)); 	//������ͨѶ
//	E2promWriteByte(EEROM_USART_BSP_ADDR,bsp>>16);
//	E2promWriteByte(EEROM_USART_BSP_ADDR+1,(bsp)>>8);
//	E2promWriteByte(EEROM_USART_BSP_ADDR+2,bsp);
	
	uart_init_3(9600); 	//������ģ��ͨѶ

//	uint8_t data1[]={0x7E,0x05,0x41,0x00,0X01,0X45,0xEF}; //7E 05 41 00 01 45 EF
//	printf_usart3(data1,7);

	USART2_BUF = mymalloc(SRAMIN,USART2_BUF_SIZE);//�����ڴ�
	MYDMA_Config(DMA1_Stream5,DMA_Channel_4,(u32)&USART2->DR,(u32)USART2_BUF,USART2_BUF_SIZE);//��ʼ������DMA
	USART_DMACmd(USART2,USART_DMAReq_Rx,ENABLE); //ʹ�ܴ���2��DMA����
  MYDMA_Enable(DMA1_Stream5,USART2_BUF_SIZE);     //��ʼ��1��DMA���䣡

	
	
//  I2C_GPIOConf();          //EEPROM��ʼ�� 
//	delay_ms(10);
//	GetE2PROMSet();     //��ȡE2PROM�в���,���δ������ʹ��Ĭ�ϲ���

	Init_SPI1();
	key_Init();
	IWDG_Init();//ʹ�ܿ��Ź�
}
