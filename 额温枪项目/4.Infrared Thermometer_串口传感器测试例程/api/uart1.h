#ifndef __UART1_H
#define __UART1_H

#include "config.h"


void UartInit(void);
void Uart_SendString(unsigned char * str, unsigned int len);
void Uart_SendData(unsigned char byte);
//unsigned char Uart_RecvData(void);

void Uart2Init(void);
void Uart2_SendData(unsigned char byte);
void Uart2_SendString(unsigned char * str, unsigned int len);
void UART2_Tick(void);
unsigned char Lu90614_ReadData(float *value);
#endif
