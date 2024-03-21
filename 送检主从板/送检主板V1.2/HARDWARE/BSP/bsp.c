#include "bsp.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "timer.h"
#include "sram.h"
#include "malloc.h"
#include "tcp_server_demo.h"
#include "sdio_sdcard.h"
#include "sw_host.h"
#include "user_tim3_pwm.h"
#include "user_usart.h"
#include "nvic.h"
#include "24cxx.h"
#include  "user_crc.h"
#include "AD.H"
#include "lsens.h"
#include "key.h"
#include "i2c.h"
#include "Code.h"
#include "RTC.h"
#include "wdg.h"
#include "dma.h"

extern uint32_t bound1;//���ڲ�����
void BSP_Init(void)
{
#if EnableCode
	static unsigned char cCodeData[20] = {0x00,0x05,0x00,0x84,0x00,0x00,0x08};//Code����������ָ��
	static unsigned char cCodeData1[20] = {0x3E,0x46,0x14,0xA8,0x8B,0x3A,0x90,0x87,0x87,0x7D,0x49,0x8E,0xB8,0x8D,0x83,0x44};//���Խ���
	static unsigned char cCodeData2[20] = {0x11,0x22,0x33,0x44,0x55,0x66,0x77,0x88,0x11,0x22,0x33,0x44,0x55,0x66,0x77,0x88};//���ܲ���
	static unsigned short iCodeLong;//�������ݵĳ���
//	static unsigned char cCodeData1[20] = {0x00,0x13,0x80,0xE0,0x3F,0x00,0x0D,0x38,0xFF,0xFF,0xFA,0xF0,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};//�������ļ�
//	static unsigned char cCodeData2[20] = {0x00,0x0C,0x80,0xE0,0x00,0x00,0x07,0x3F,0x03,0x00,0xFF,0xF0,0xFF,0xFF};//����KEY�ļ�
//	static unsigned char cCodeData3[30] = {0x00,0x1A,0x80,0xD4,0x01,0x02,0x15,0x30,0xF0,0xFA,0x01,0x04,\
//	0x01,0x23,0x45,0x67,0x89,0xab,0xcd,0xef,0xfe,0xdc,0xba,0x98,0x76,0x54,0x32,0x10};//д��SM4�����ܳ�
//	static unsigned char cCodeData4[30] = {0x00,0x1A,0x80,0xD4,0x01,0x02,0x15,0x31,0xF0,0xFA,0x01,0x04,\
//	0x01,0x23,0x45,0x67,0x89,0xab,0xcd,0xef,0xfe,0xdc,0xba,0x98,0x76,0x54,0x32,0x10};//д��SM4�����ܳ�
#endif
	delay_init(168);       	  //��ʱ��ʼ��
  NVIC_Configuration(); 
  User_USART6Config(9600);  //������ͨ��
	RS485_Init(G_IDHost);

	g_cSEND_BUF_SIZE = mymalloc(SRAMEX,SEND_BUF_SIZE);//�����ڴ�
	MYDMA_Config(DMA2_Stream2,DMA_Channel_4,(u32)&USART1->DR,(u32)g_cSEND_BUF_SIZE,SEND_BUF_SIZE);//��ʼ������DMA
	//DMA2,STEAM7,CH4,����Ϊ����1,�洢��ΪSendBuff,����Ϊ:SEND_BUF_SIZE.
  LED_Init();  			      //LED��ʼ��
  I2C_GPIOConf();          //EEPROM��ʼ��   			
	FSMC_SRAM_Init();		    //��ʼ���ⲿSRAM
	
//	TIM3_Int_Init(999,999); //100khz��Ƶ��,����1000Ϊ10ms
 	
	mymem_init(SRAMEX);		  //��ʼ���ⲿ�ڴ��
	mymem_init(SRAMIN);  	//��ʼ���ڲ��ڴ��
  
  SD_Init();
  TIM9_CH1_PWM_Init(127,128,0); 
	delay_ms(10);
  SW_HostGPIOConfig();  //LCP
	GetE2PROMSet();     //��ȡE2PROM�в���,���δ������ʹ��Ĭ�ϲ���
	EXTI_UserKey_Config();
	
  User_CRCConfig();      //32λCRC

//  PwmSet(90);
  
  User_ADConfig();
	OSTimeDly(5);
  uart_init(G_BaudRate);   	    //���ڲ���������   
	SLAVERSTSet;              //�ӻ���λ������
	RS485_TX_EN=1;;          //Ĭ������Ϊ���
	//------------2022/07/07�¼�----------------------//
#if EnableCode
	init_SPI();
	while(1)//��ʼ����֤Code
	{
		Rst_Code();//��λ����IC
		Code_CS=0;//ʹ��Code
		WriteCode(cCodeData,7,1);//����д��
		Code_CS=1;//ʧ��Code
		ReadCode(cCodeData,&iCodeLong,1);//������ȡ
		if(iCodeLong == 0x0a)
		{
			break;//��ʼ��Code�ɹ�
		}
	}
//	AlterCodeKey(1);//��ʼ����Կ(���ڵ���)
	EncryptData(cCodeData2,16,(unsigned int*)&iCodeLong,1);//���Լ���
	DecryptData(cCodeData1,16,(unsigned int*)&iCodeLong,1);//���ܲ���
//---------------д���ܳף�дһ�μ��ɣ�----------------//
//	WriteCode(cCodeData1,21,1);//����д��
//	ReadCode(cCodeData,&iCodeLong,1);//������ȡ
//	WriteCode(cCodeData2,14,1);//����д��
//	ReadCode(cCodeData,&iCodeLong,1);//������ȡ
//	WriteCode(cCodeData3,28,1);//����д��
//	ReadCode(cCodeData,&iCodeLong,1);//������ȡ
//	WriteCode(cCodeData4,28,1);//����д��
//	ReadCode(cCodeData,&iCodeLong,1);//������ȡ
#endif
	
	init_RTC();//��ʼ��RTCʱ��
	IWDG_Init();//ʹ�ܿ��Ź�
}

