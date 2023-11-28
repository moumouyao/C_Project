#include "tim.h"
#include "ioConfig.h"

unsigned long Systick_Tick = 0;
unsigned int Led_TaskTime[2] = {0, 500};
unsigned int View_TaskTime[2] = {0, 500};

/********************* Timer0�жϺ���************************/
void timer0_int (void) interrupt TIMER0_VECTOR
{
	Systick_Tick++;
	Led_TaskTime[0]++;
	View_TaskTime[0]++;
}

void Delay_ms(unsigned int ms)
{
	unsigned long time = Systick_Tick+ms;
	while(time > Systick_Tick)
	{}
}

void Tim0_Config(void)//1����@24.000MHz
{
	AUXR |= 0x80;		//��ʱ��ʱ��1Tģʽ
	TMOD &= 0xF0;		//���ö�ʱ��ģʽ
	TL0 = 0x40;		//���ö�ʱ��ʼֵ
	TH0 = 0xA2;		//���ö�ʱ��ʼֵ
	TF0 = 0;		//���TF0��־
	TR0 = 1;		//��ʱ��0��ʼ��ʱ
	ET0 = 1;				//ʹ�ܶ�ʱ��0�ж�
}




