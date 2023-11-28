#include "uart.h"



void UartInit(void)		//9600bps@24.000MHz
{
	SCON = 0x50;		//8λ����,�ɱ䲨����
	AUXR |= 0x40;		//��ʱ��ʱ��1Tģʽ
	AUXR &= 0xFE;		//����1ѡ��ʱ��1Ϊ�����ʷ�����
	TMOD &= 0x0F;		//���ö�ʱ��ģʽ
	TL1 = 0x8F;		//���ö�ʱ��ʼֵ
	TH1 = 0xFD;		//���ö�ʱ��ʼֵ
	ET1 = 0;		//��ֹ��ʱ��%d�ж�
	TR1 = 1;		//��ʱ��1��ʼ��ʱ
//	ES = 1;				//�򿪴����ж�
	ES = 0;					//�򿪴����ж�
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

////���յ����ֽ�  �����ж�ʹ��
//unsigned char Uart_RecvData(void)
//{
//	while(RI == 0);
//	RI = 0;
//	return SBUF;
//}

