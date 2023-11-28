#include "uart.h"



void UartInit(void)		//9600bps@24.000MHz
{
	SCON = 0x50;		//8位数据,可变波特率
	AUXR |= 0x40;		//定时器时钟1T模式
	AUXR &= 0xFE;		//串口1选择定时器1为波特率发生器
	TMOD &= 0x0F;		//设置定时器模式
	TL1 = 0x8F;		//设置定时初始值
	TH1 = 0xFD;		//设置定时初始值
	ET1 = 0;		//禁止定时器%d中断
	TR1 = 1;		//定时器1开始计时
//	ES = 1;				//打开串口中断
	ES = 0;					//打开串口中断
}


//发送单个字节  不开中断使用
void Uart_SendData(unsigned char byte)
{
	SBUF = byte;
	while(TI == 0);
	TI = 0;
}

//构造printf用的
char putchar(char c)
{
 	SBUF = c;
	while(!TI);
	TI = 0;
  return c;
}

//发送字符串  不开中断使用
void Uart_SendString(unsigned char * str, unsigned int len)
{
	while(len--)
		Uart_SendData(*str++);
}

////接收单个字节  不开中断使用
//unsigned char Uart_RecvData(void)
//{
//	while(RI == 0);
//	RI = 0;
//	return SBUF;
//}

