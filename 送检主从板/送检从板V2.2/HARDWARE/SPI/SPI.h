#ifndef _SPI_H
#define _SPI_H

#include "stm32f4xx.h"

#define SPI_CS    PIout(0) //Æ¬Ñ¡ÐÅºÅ

extern void Init_SPI2(void);
extern void SPI2_SetSpeed(uint8_t SPI_BaudRatePrescaler);
extern void SPI2_Init(void);
extern uint8_t SPI2_ReadWriteByte(uint8_t TxData);

#endif

