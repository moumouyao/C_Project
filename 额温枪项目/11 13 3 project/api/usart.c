#include "usart.h"

void UartInit(void)		//9600bps@24.000MHz
{
	SCON = 0x50;		//8λ����,�ɱ䲨����
	AUXR |= 0x40;		//��ʱ��ʱ��1Tģʽ
	AUXR &= 0xFE;		//����1ѡ��ʱ��1Ϊ�����ʷ�����
	TMOD &= 0x0F;		//���ö�ʱ��ģʽ
	TL1 = 0x8F;		//���ö�ʱ��ʼ	ֵ
	TH1 = 0xFD;		//���ö�ʱ��ʼֵ
	ET1 = 0;		//��ֹ��ʱ��%d�ж�
	TR1 = 1;		//��ʱ��1��ʼ��ʱ
}








//���͵����ֽ�  �����ж�ʹ��
void Uart_SendData(unsigned char byte)
{
	SBUF = byte;
	while(TI == 0);
	TI = 0;
}

//����printf�õ�
char putchar(char c)
{
 	SBUF = c;
	while(!TI);
	TI = 0;
  return c;
}

//�����ַ���  �����ж�ʹ��
void Uart_SendString(unsigned char * str, unsigned int len)
{
	while(len--)
		Uart_SendData(*str++);
}


//����2����
void Uart2Init(void)	//9600bps@24.000MHz
{
	S2CON = 0x50;		//���ô���2���ƼĴ�������ʾʹ��ģʽ1�������������
	AUXR |= 0x04;		//��AUXR�Ĵ����ĵ�2λ����Ϊ1����ʾ��ʱ��ʱ��1Tģʽ
	T2L = 0x8F;			//���ö�ʱ��2��ʼֵ�ĵ�8λ
	T2H = 0xFD;			//���ö�ʱ��2��ʼֵ�ĸ�8λ
	AUXR |= 0x10;		//��AUXR�Ĵ����ĵ�4λ����Ϊ1����ʾ��ʱ��2��ʼ��ʱ
	S2CON &=  ~(1<<1);	//�ر�UART2�����ж�
	S2CON |=  (1<<0);	//��UART2�����ж�
	IE2   |=  (1<<0);	//����UART2�ж�
}



//����2���պ���
void Uart2_SendData(unsigned char byte)
{
	S2BUF = byte;
	while(SEND_SUCCESS == 0);
	CLEAN_USART2_SEND();
}

//����2���ͺ���
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

//����2�жϷ����������մ�������������
void usart2_handler (void) interrupt UART2_VECTOR{
	if(USART2_RECV_INTERRUPT){	//����2�����ж���1
		CLEAN_USART2_INTERRUPT();
		byte = S2BUF;
		UART2_RecvTick = 1;
		if(UART2_RecvOver == 0) {
			UART2_RecvBuff[UART2_RecvCnt++] = byte;
			UART2_RecvCnt %= 32;
		}
	}
}


//����2����������������
void usart2_tick(void){
	if(UART2_RecvTick>0) {
		UART2_RecvTick++;
		if(UART2_RecvTick>100){
			UART2_RecvTick = 0;
			UART2_RecvOver = 1;
		}
	}
}






