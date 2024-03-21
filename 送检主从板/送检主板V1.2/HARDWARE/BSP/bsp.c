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

extern uint32_t bound1;//串口波特率
void BSP_Init(void)
{
#if EnableCode
	static unsigned char cCodeData[20] = {0x00,0x05,0x00,0x84,0x00,0x00,0x08};//Code获得随机数的指令
	static unsigned char cCodeData1[20] = {0x3E,0x46,0x14,0xA8,0x8B,0x3A,0x90,0x87,0x87,0x7D,0x49,0x8E,0xB8,0x8D,0x83,0x44};//测试解密
	static unsigned char cCodeData2[20] = {0x11,0x22,0x33,0x44,0x55,0x66,0x77,0x88,0x11,0x22,0x33,0x44,0x55,0x66,0x77,0x88};//加密测试
	static unsigned short iCodeLong;//读出数据的长度
//	static unsigned char cCodeData1[20] = {0x00,0x13,0x80,0xE0,0x3F,0x00,0x0D,0x38,0xFF,0xFF,0xFA,0xF0,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};//创建主文件
//	static unsigned char cCodeData2[20] = {0x00,0x0C,0x80,0xE0,0x00,0x00,0x07,0x3F,0x03,0x00,0xFF,0xF0,0xFF,0xFF};//创建KEY文件
//	static unsigned char cCodeData3[30] = {0x00,0x1A,0x80,0xD4,0x01,0x02,0x15,0x30,0xF0,0xFA,0x01,0x04,\
//	0x01,0x23,0x45,0x67,0x89,0xab,0xcd,0xef,0xfe,0xdc,0xba,0x98,0x76,0x54,0x32,0x10};//写入SM4加密密匙
//	static unsigned char cCodeData4[30] = {0x00,0x1A,0x80,0xD4,0x01,0x02,0x15,0x31,0xF0,0xFA,0x01,0x04,\
//	0x01,0x23,0x45,0x67,0x89,0xab,0xcd,0xef,0xfe,0xdc,0xba,0x98,0x76,0x54,0x32,0x10};//写入SM4解密密匙
#endif
	delay_init(168);       	  //延时初始化
  NVIC_Configuration(); 
  User_USART6Config(9600);  //串口屏通信
	RS485_Init(G_IDHost);

	g_cSEND_BUF_SIZE = mymalloc(SRAMEX,SEND_BUF_SIZE);//申请内存
	MYDMA_Config(DMA2_Stream2,DMA_Channel_4,(u32)&USART1->DR,(u32)g_cSEND_BUF_SIZE,SEND_BUF_SIZE);//初始化接收DMA
	//DMA2,STEAM7,CH4,外设为串口1,存储器为SendBuff,长度为:SEND_BUF_SIZE.
  LED_Init();  			      //LED初始化
  I2C_GPIOConf();          //EEPROM初始化   			
	FSMC_SRAM_Init();		    //初始化外部SRAM
	
//	TIM3_Int_Init(999,999); //100khz的频率,计数1000为10ms
 	
	mymem_init(SRAMEX);		  //初始化外部内存池
	mymem_init(SRAMIN);  	//初始化内部内存池
  
  SD_Init();
  TIM9_CH1_PWM_Init(127,128,0); 
	delay_ms(10);
  SW_HostGPIOConfig();  //LCP
	GetE2PROMSet();     //读取E2PROM中参数,如果未设置则使用默认参数
	EXTI_UserKey_Config();
	
  User_CRCConfig();      //32位CRC

//  PwmSet(90);
  
  User_ADConfig();
	OSTimeDly(5);
  uart_init(G_BaudRate);   	    //串口波特率设置   
	SLAVERSTSet;              //从机复位端拉高
	RS485_TX_EN=1;;          //默认配置为输出
	//------------2022/07/07新加----------------------//
#if EnableCode
	init_SPI();
	while(1)//初始化验证Code
	{
		Rst_Code();//复位解码IC
		Code_CS=0;//使能Code
		WriteCode(cCodeData,7,1);//阻塞写入
		Code_CS=1;//失能Code
		ReadCode(cCodeData,&iCodeLong,1);//阻塞读取
		if(iCodeLong == 0x0a)
		{
			break;//初始化Code成功
		}
	}
//	AlterCodeKey(1);//初始化密钥(用于调试)
	EncryptData(cCodeData2,16,(unsigned int*)&iCodeLong,1);//测试加密
	DecryptData(cCodeData1,16,(unsigned int*)&iCodeLong,1);//解密测试
//---------------写入密匙（写一次即可）----------------//
//	WriteCode(cCodeData1,21,1);//阻塞写入
//	ReadCode(cCodeData,&iCodeLong,1);//阻塞读取
//	WriteCode(cCodeData2,14,1);//阻塞写入
//	ReadCode(cCodeData,&iCodeLong,1);//阻塞读取
//	WriteCode(cCodeData3,28,1);//阻塞写入
//	ReadCode(cCodeData,&iCodeLong,1);//阻塞读取
//	WriteCode(cCodeData4,28,1);//阻塞写入
//	ReadCode(cCodeData,&iCodeLong,1);//阻塞读取
#endif
	
	init_RTC();//初始化RTC时钟
	IWDG_Init();//使能看门狗
}

