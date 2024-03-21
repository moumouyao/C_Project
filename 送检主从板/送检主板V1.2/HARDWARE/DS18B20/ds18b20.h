#ifndef __DS18B20_H
#define __DS18B20_H 

#include "stm32f4xx.h"
  
//SC100������
//DS18B20 �������� 
//IO��������


#define DS18B20_PIN                         GPIO_Pin_8
#define DS18B20_GPIO_PORT                   GPIOA
#define DS18B20_GPIO_CLK                    RCC_AHB1Periph_GPIOA  

#define DS18B20_DATA_IN  GPIO_ReadInputDataBit(DS18B20_GPIO_PORT, DS18B20_PIN)
#define DS18B20_OUT_HIGH GPIO_SetBits(DS18B20_GPIO_PORT, DS18B20_PIN);
#define DS18B20_OUT_LOW  GPIO_ResetBits(DS18B20_GPIO_PORT, DS18B20_PIN);

void DS18B20_Set_Input(void);
u8 DS18B20_Init(void);//��ʼ��DS18B20
short DS18B20_Get_Temp(void);//��ȡ�¶�
void DS18B20_Write_Byte(u8 dat);//д��һ���ֽ�
u8 DS18B20_Read_Byte(void);//����һ���ֽ�
u8 DS18B20_Read_Bit(void);//����һ��λ
u8 DS18B20_Check(void);//����Ƿ����DS18B20
void DS18B20_Reset(void);       //��λDS18B20    
u8 DS18B20_Get_Serial(u8 *serial);
u8 GetCRC(u8 *str, u32 length);

extern short temperature;
#endif















