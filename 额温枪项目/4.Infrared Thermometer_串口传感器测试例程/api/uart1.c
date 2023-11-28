#include "uart1.h"
#include "tim.h"


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






//����2���ڲ���������ʹ��

void Uart2Init(void)	//9600bps@24.000MHz
{
	//��������
	P1M0 &= ~(0x3<<0);
	P1M1 &= ~(0x3<<0);
	
	S2CON = 0x50;		//8λ����,�ɱ䲨���ʣ��������
	AUXR |= 0x04;		//��ʱ��ʱ��1Tģʽ
	T2L = 0x8F;			//���ö�ʱ��ʼֵ
	T2H = 0xFD;			//���ö�ʱ��ʼֵ
	AUXR |= 0x10;		//��ʱ��2��ʼ��ʱ
	S2CON &=  ~(1<<1);	//�رշ����ж�
	S2CON |=  (1<<0);	//�򿪽����ж�
	IE2   |=  (1<<0);	//����UART2�ж�
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
	if(RI2)//�����ж�
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
	//���²���
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


