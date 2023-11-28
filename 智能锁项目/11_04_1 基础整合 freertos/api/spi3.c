/*
****************************************************************************************
* Copyright: 2008-2019����ӯ��Ƽ����޹�˾��All rights reserved
* File name: spi3.c
* Description: spiЭ��
* Dependent file: spi3.h
* Author: weihaoMo
* Version: V1.0
* Date: 2019-11-29
* History: None
* Company web site: http://www.edu118.com/
* Company net schools: http://www.edu118.cn/
* Notice: ������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;�� 
*********************************��Ȩ���У�����ؾ�*********************************
****************************************************************************************
*/

/*
****************************************************************************************
* INCLUDES (ͷ�ļ�����)
****************************************************************************************
*/
#include "spi3.h"

/*
****************************************************************************************
* CONSTANTS (��������)
****************************************************************************************
*/


/*
****************************************************************************************
* TYPEDEFS (���Ͷ���)
****************************************************************************************
*/


/*
****************************************************************************************
* LOCAL VARIABLES (��̬����)
****************************************************************************************
*/


/*
****************************************************************************************
* LOCAL FUNCTIONS DECLARE (��̬��������)
****************************************************************************************
*/


/*
****************************************************************************************
* LOCAL FUNCTIONS (��̬����)
****************************************************************************************
*/


/*
****************************************************************************************
* PUBLIC FUNCTIONS (ȫ�ֺ���)
****************************************************************************************
*/

/*
****************************************************************************************
* Function: SPI3PinInit
* Description: SPI3�ӿڳ�ʼ��
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


	//IO�ڳ�ʼ��
	RCC->AHB1ENR |=0x01<<2;//�˿�ʱ��ʹ��
	
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
	
	//SPI3��������ʼ��
	RCC->APB1ENR |=0x01<<15;//ģ��ʱ��ʹ��
	SPI3->CR1 &=~(0x01<<15);//ѡ��˫�ߵ���ͨ������ģʽ
	SPI3->CR1 &=~(0x01<<11);//Ϊ����/����ѡ�� 8 λ����֡��ʽ
	SPI3->CR1 &=~(0x01<<10);//ȫ˫�������ͺͽ��գ�
	//���NSS����
	SPI3->CR1 |=(0x03<<8);
	SPI3->CR1 &=~(0x01<<7);//��λ��ǰ
	
	SPI3->CR1 &=~(0x07<<3);//SPI�ٶ� 42M/2=21M
	SPI3->CR1 |=(0x02<<3);
	
	SPI3->CR1 |=(0x01<<2);//��ģʽ
	SPI3->CR1 |=(0x03<<0);//MODE3
	SPI3->CR2 &=~(0x01<<4);//Ħ��������ʽ
	
	SPI3->CR1 |=(0x01<<6);//SPI3ʹ��
	
	SPI3TransferByte(0xff);
}

/*
****************************************************************************************
* Function: SPI3TransferByte
* Description: SPI3��д����
* Input: data�����͵�һ���ֽ�����
* Output: None
* Return: ���յ�һ���ֽ�����
* Author: weihaoMo
* Others: None
* Date of completion: 2019-11-29
* Date of last modify: 2019-11-29
****************************************************************************************
*/
uint8_t SPI3TransferByte(uint8_t data)
{
	/*���ͻ�����Ϊ��*/
	while(!(SPI3->SR&0x01<<1));
	SPI3->DR=data;
	/*���ܻ�����Ϊ�ǿ�*/
	while(!(SPI3->SR&0x01<<0));
	data=SPI3->DR;
	
	return data;
}







