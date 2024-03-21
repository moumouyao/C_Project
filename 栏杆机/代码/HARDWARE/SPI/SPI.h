#ifndef _SPI_H
#define _SPI_H

#include "stm32f4xx.h"
#define SPI_CS    PBout(6) //片选信号

void Init_SPI1(void);
void SPI1_SetSpeed(u8 SPI_BaudRatePrescaler);
void SPI1_Init(void);
u8 SPI1_ReadWriteByte(u8 TxData);
void GenitopZk_Address(u32 Address);

//16号字体使用
void ASCII_8X16(u8 ASCIICode, u8 *BUF);
uint8_t GB2312_16X16(u8 MSB,u8 LSB,u8 *BUF);
//24号字体使用
void ASCII_12X24(u8 ASCIICode, u8 *BUF);
uint8_t GB2312_24X24(u8 MSB,u8 LSB,u8 *BUF);
#endif

