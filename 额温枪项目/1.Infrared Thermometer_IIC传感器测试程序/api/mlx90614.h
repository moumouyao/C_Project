#ifndef _MLX90614_H_
#define _MLX90614_H_

#include "config.h"
#include <intrins.h>

#define  u16  unsigned int 
#define  u8 unsigned char 

void SMBus_StartBit(void);
void SMBus_StopBit(void);
void SMBus_SendBit(u8);
u8 SMBus_SendByte(u8);
u8 SMBus_ReceiveBit(void);
u8 SMBus_ReceiveByte(u8);
//void SMBus_Delay(u16);
void SMBus_Init(void);
u16 SMBus_ReadMemory(u8, u8);
u8 PEC_Calculation(u8*);
float SMBus_ReadTemp(void);    //��ȡ�¶�ֵ
void SMBus_DisplayTemperature(void);    //��LCD��5��6ҳ��ʾ�¶�

#endif
