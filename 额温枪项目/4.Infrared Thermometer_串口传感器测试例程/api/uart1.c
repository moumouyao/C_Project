#include "uart1.h"
#include "tim.h"


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






//串口2用于测量传感器使用

void Uart2Init(void)	//9600bps@24.000MHz
{
	//引脚配置
	P1M0 &= ~(0x3<<0);
	P1M1 &= ~(0x3<<0);
	
	S2CON = 0x50;		//8位数据,可变波特率，允许接收
	AUXR |= 0x04;		//定时器时钟1T模式
	T2L = 0x8F;			//设置定时初始值
	T2H = 0xFD;			//设置定时初始值
	AUXR |= 0x10;		//定时器2开始计时
	S2CON &=  ~(1<<1);	//关闭发送中断
	S2CON |=  (1<<0);	//打开接收中断
	IE2   |=  (1<<0);	//允许UART2中断
}

void Uart2_SendData(unsigned char byte)
{
	S2BUF = byte;
	while(TI2 == 0);
	CLR_TI2();
}


void Uart2_SendString(unsigned char * str, unsigned int len)
{
	while(len--)
		Uart2_SendData(*str++);
}

unsigned char xdata UART2_RecvBuff[32] = {0};
unsigned char xdata UART2_RecvCnt = 0;
unsigned int  xdata UART2_RecvTick = 0;
unsigned char xdata UART2_RecvOver = 0;
unsigned char xdata byte = 0;
void UART2_int (void) interrupt UART2_VECTOR
{
	if(RI2)//接收中断
	{
		CLR_RI2();
		byte = S2BUF;
		UART2_RecvTick = 1;
		if(UART2_RecvOver == 0) {
			UART2_RecvBuff[UART2_RecvCnt++] = byte;
			UART2_RecvCnt %= 32;
		}
	}
}

void UART2_Tick(void)
{
	if(UART2_RecvTick>0) {
		UART2_RecvTick++;
		if(UART2_RecvTick>100){
			UART2_RecvTick = 0;
			UART2_RecvOver = 1;
		}
	}
	

}

unsigned char Lu90614_ReadData(float *value)
{
	//体温测量
	unsigned char xdata ret = 0;
	unsigned char xdata cnt = 0;
	unsigned char xdata check = 0;
	unsigned char xdata CMD1[6] = {0xFA, 0xC5, 0xBF,0xFA, 0xCA, 0xC4};
	Uart2_SendString(CMD1, 6);
	while(UART2_RecvOver == 0) {
		Delay_ms(1); cnt++;
		if(cnt>100) {
			return ret;
		}
	}
	if(UART2_RecvOver == 1) {
		if(UART2_RecvCnt != 9) goto err;
		
		for(cnt = 0; cnt<UART2_RecvCnt-1; cnt++) {
			check += UART2_RecvBuff[cnt];
		}
		if(check != UART2_RecvBuff[UART2_RecvCnt-1]) goto err;
		*value = UART2_RecvBuff[2] + UART2_RecvBuff[3]/100.0;
		ret = 1;
	}
	err:
		UART2_RecvCnt = 0;
		UART2_RecvTick = 0;
		UART2_RecvOver = 0;
	return ret;
	
}


