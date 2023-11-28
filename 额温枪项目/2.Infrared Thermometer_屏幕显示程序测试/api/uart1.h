#ifndef __UART_H
#define __UART_H

#include "config.h"


void UartInit(void);
void Uart_SendString(unsigned char * str, unsigned int len);
void Uart_SendData(unsigned char byte);
//unsigned char Uart_RecvData(void);
#endif
