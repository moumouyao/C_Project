#ifndef __USART_H_
#define __USART_H_

#include <STC8.H>

#define		UART2_VECTOR	8	//����2�ж����к�
#define		USART2_RECV_INTERRUPT					(S2CON & 1) 		// �ж�RI2�Ƿ�������
#define		CLEAN_USART2_INTERRUPT()			S2CON &= ~1			// ���RI2
#define		SEND_SUCCESS					(S2CON & 2) 		// �ж�TI2�Ƿ������
#define		CLEAN_USART2_SEND()			S2CON &= ~(1<<1)	// ���TI2��

extern unsigned int  xdata UART2_RecvTick;
extern unsigned char xdata UART2_RecvOver;
extern unsigned char xdata byte;
extern unsigned char xdata UART2_RecvCnt;
extern unsigned char xdata UART2_RecvBuff[32];

void UartInit(void);
void Uart_SendString(unsigned char * str, unsigned int len);
void Uart2Init(void);
void Uart2_SendData(unsigned char byte);
void Uart2_SendString(unsigned char * str, unsigned int len);
void usart2_tick(void);
#endif
