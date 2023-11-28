/*
****************************************************************************************
* Copyright: 2008-2019，信盈达科技有限公司，All rights reserved
* File name: spi3.c
* Description: spi协议
* Dependent file: spi3.h
* Author: weihaoMo
* Version: V1.0
* Date: 2019-11-29
* History: None
* Company web site: http://www.edu118.com/
* Company net schools: http://www.edu118.cn/
* Notice: 本程序只供学习使用，未经作者许可，不得用于其它任何用途。 
*********************************版权所有，盗版必究*********************************
****************************************************************************************
*/

/*
****************************************************************************************
* INCLUDES (头文件包含)
****************************************************************************************
*/
#include "spi3.h"

/*
****************************************************************************************
* CONSTANTS (常量定义)
****************************************************************************************
*/


/*
****************************************************************************************
* TYPEDEFS (类型定义)
****************************************************************************************
*/


/*
****************************************************************************************
* LOCAL VARIABLES (静态变量)
****************************************************************************************
*/


/*
****************************************************************************************
* LOCAL FUNCTIONS DECLARE (静态函数声明)
****************************************************************************************
*/


/*
****************************************************************************************
* LOCAL FUNCTIONS (静态函数)
****************************************************************************************
*/


/*
****************************************************************************************
* PUBLIC FUNCTIONS (全局函数)
****************************************************************************************
*/

/*
****************************************************************************************
* Function: SPI3PinInit
* Description: SPI3接口初始化
* Input: None
* Output: None
* Return: None
* Author: weihaoMo
* Others: None
* Date of completion: 2019-11-29
* Date of last modify: 2019-11-29
****************************************************************************************
*/
void SPI3PinInit(void)
{


	//IO口初始化
	RCC->AHB1ENR |=0x01<<2;//端口时钟使能
	
	RFID_SPI3_SCLK_GPIO->MODER &=~(3<<(RFID_SPI3_SCLK_PIN*2));
	RFID_SPI3_SCLK_GPIO->MODER |= (2<<(RFID_SPI3_SCLK_PIN*2));
	RFID_SPI3_SCLK_GPIO->OTYPER &=~(1<<RFID_SPI3_SCLK_PIN);
	RFID_SPI3_SCLK_GPIO->OSPEEDR |= (3<<(RFID_SPI3_SCLK_PIN*2));
	RFID_SPI3_SCLK_GPIO->PUPDR &=~(3<<(RFID_SPI3_SCLK_PIN*2));
	if(RFID_SPI3_SCLK_PIN<8)
	{
		RFID_SPI3_SCLK_GPIO->AFR[0] &=~(0xF<<(RFID_SPI3_SCLK_AFIO_PIN*4));
		RFID_SPI3_SCLK_GPIO->AFR[0] |= (RFID_SPI3_SCLK_AFIO<<(RFID_SPI3_SCLK_AFIO_PIN*4));
	}
	else
	{
		RFID_SPI3_SCLK_GPIO->AFR[1] &=~(0xF<<(RFID_SPI3_SCLK_AFIO_PIN*4));
		RFID_SPI3_SCLK_GPIO->AFR[1] |= (RFID_SPI3_SCLK_AFIO<<(RFID_SPI3_SCLK_AFIO_PIN*4));
	}
	
	RFID_SPI3_MISO_GPIO->MODER &=~(3<<(RFID_SPI3_MISO_PIN*2));
	RFID_SPI3_MISO_GPIO->MODER |= (2<<(RFID_SPI3_MISO_PIN*2));
	RFID_SPI3_MISO_GPIO->PUPDR &=~(3<<(RFID_SPI3_MISO_PIN*2));
	if(RFID_SPI3_MISO_PIN<8)
	{
		RFID_SPI3_MISO_GPIO->AFR[0] &=~(0xF<<(RFID_SPI3_MISO_AFIO_PIN*4));
		RFID_SPI3_MISO_GPIO->AFR[0] |= (RFID_SPI3_MISO_AFIO<<(RFID_SPI3_MISO_AFIO_PIN*4));
	}
	else
	{
		RFID_SPI3_MISO_GPIO->AFR[1] &=~(0xF<<(RFID_SPI3_MISO_AFIO_PIN*4));
		RFID_SPI3_MISO_GPIO->AFR[1] |= (RFID_SPI3_MISO_AFIO<<(RFID_SPI3_MISO_AFIO_PIN*4));
	}
	
	RFID_SPI3_MOSI_GPIO->MODER &=~(3<<(RFID_SPI3_MOSI_PIN*2));
	RFID_SPI3_MOSI_GPIO->MODER |= (2<<(RFID_SPI3_MOSI_PIN*2));
	RFID_SPI3_MOSI_GPIO->OTYPER &=~(1<<RFID_SPI3_MOSI_PIN);
	RFID_SPI3_MOSI_GPIO->OSPEEDR |= (3<<(RFID_SPI3_MOSI_PIN*2));
	RFID_SPI3_MOSI_GPIO->PUPDR &=~(3<<(RFID_SPI3_MOSI_PIN*2));
	if(RFID_SPI3_MOSI_PIN<8)
	{
		RFID_SPI3_MOSI_GPIO->AFR[0] &=~(0xF<<(RFID_SPI3_MOSI_AFIO_PIN*4));
		RFID_SPI3_MOSI_GPIO->AFR[0] |= (RFID_SPI3_MOSI_AFIO<<(RFID_SPI3_MOSI_AFIO_PIN*4));
	}
	else
	{
		RFID_SPI3_MOSI_GPIO->AFR[1] &=~(0xF<<(RFID_SPI3_MOSI_AFIO_PIN*4));
		RFID_SPI3_MOSI_GPIO->AFR[1] |= (RFID_SPI3_MOSI_AFIO<<(RFID_SPI3_MOSI_AFIO_PIN*4));
	}
	
	//SPI3控制器初始化
	RCC->APB1ENR |=0x01<<15;//模块时钟使能
	SPI3->CR1 &=~(0x01<<15);//选择双线单向通信数据模式
	SPI3->CR1 &=~(0x01<<11);//为发送/接收选择 8 位数据帧格式
	SPI3->CR1 &=~(0x01<<10);//全双工（发送和接收）
	//软件NSS管理
	SPI3->CR1 |=(0x03<<8);
	SPI3->CR1 &=~(0x01<<7);//高位在前
	
	SPI3->CR1 &=~(0x07<<3);//SPI速度 42M/2=21M
	SPI3->CR1 |=(0x02<<3);
	
	SPI3->CR1 |=(0x01<<2);//主模式
	SPI3->CR1 |=(0x03<<0);//MODE3
	SPI3->CR2 &=~(0x01<<4);//摩托罗拉格式
	
	SPI3->CR1 |=(0x01<<6);//SPI3使能
	
	SPI3TransferByte(0xff);
}

/*
****************************************************************************************
* Function: SPI3TransferByte
* Description: SPI3读写数据
* Input: data待发送的一个字节数据
* Output: None
* Return: 接收到一个字节数据
* Author: weihaoMo
* Others: None
* Date of completion: 2019-11-29
* Date of last modify: 2019-11-29
****************************************************************************************
*/
uint8_t SPI3TransferByte(uint8_t data)
{
	/*发送缓冲区为空*/
	while(!(SPI3->SR&0x01<<1));
	SPI3->DR=data;
	/*接受缓冲区为非空*/
	while(!(SPI3->SR&0x01<<0));
	data=SPI3->DR;
	
	return data;
}







