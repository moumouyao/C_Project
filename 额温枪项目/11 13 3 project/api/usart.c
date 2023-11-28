#include "usart.h"

void UartInit(void)		//9600bps@24.000MHz
{
	SCON = 0x50;		//8位数据,可变波特率
	AUXR |= 0x40;		//定时器时钟1T模式
	AUXR &= 0xFE;		//串口1选择定时器1为波特率发生器
	TMOD &= 0x0F;		//设置定时器模式
	TL1 = 0x8F;		//设置定时初始	值
	TH1 = 0xFD;		//设置定时初始值
	ET1 = 0;		//禁止定时器%d中断
	TR1 = 1;		//定时器1开始计时
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


//串口2配置
void Uart2Init(void)	//9600bps@24.000MHz
{
	S2CON = 0x50;		//设置串口2控制寄存器，表示使用模式1，允许接收数据
	AUXR |= 0x04;		//将AUXR寄存器的第2位设置为1，表示定时器时钟1T模式
	T2L = 0x8F;			//设置定时器2初始值的低8位
	T2H = 0xFD;			//设置定时器2初始值的高8位
	AUXR |= 0x10;		//将AUXR寄存器的第4位设置为1，表示定时器2开始计时
	S2CON &=  ~(1<<1);	//关闭UART2发送中断
	S2CON |=  (1<<0);	//打开UART2接收中断
	IE2   |=  (1<<0);	//允许UART2中断
}



//串口2接收函数
void Uart2_SendData(unsigned char byte)
{
	S2BUF = byte;
	while(SEND_SUCCESS == 0);
	CLEAN_USART2_SEND();
}

//串口2发送函数
void Uart2_SendString(unsigned char * str, unsigned int len)
{
	while(len--)
		Uart2_SendData(*str++);
}



unsigned int  xdata UART2_RecvTick = 0;
unsigned char xdata UART2_RecvOver = 0;
unsigned char xdata byte = 0;
unsigned char xdata UART2_RecvCnt = 0;
unsigned char xdata UART2_RecvBuff[32] = {0};

//串口2中断服务函数，接收传感器返回数据
void usart2_handler (void) interrupt UART2_VECTOR{
	if(USART2_RECV_INTERRUPT){	//串口2接收中断置1
		CLEAN_USART2_INTERRUPT();
		byte = S2BUF;
		UART2_RecvTick = 1;
		if(UART2_RecvOver == 0) {
			UART2_RecvBuff[UART2_RecvCnt++] = byte;
			UART2_RecvCnt %= 32;
		}
	}
}


//串口2接收数据心跳函数
void usart2_tick(void){
	if(UART2_RecvTick>0) {
		UART2_RecvTick++;
		if(UART2_RecvTick>100){
			UART2_RecvTick = 0;
			UART2_RecvOver = 1;
		}
	}
}






