#include "timer.h"

unsigned int delay_ms[2] = {0};


void Delay_ms(unsigned int xus)
{
	delay_ms[1] = xus;
	delay_ms[0] = 0;
	while(delay_ms[0] < delay_ms[1]);

}



//���ö�ʱ��0������ 1ms
void Timer0Init(void)		//1����@24.000MHz
{
	AUXR |= 0x80;		//��ʱ��ʱ��1Tģʽ
	TMOD &= 0xF0;		//���ö�ʱ��ģʽ
	TL0 = 0x40;		//���ö�ʱ��ʼֵ
	TH0 = 0xA2;		//���ö�ʱ��ʼֵ
	TF0 = 0;		//���TF0��־
	ET0 = 1;		//������ʱ��1�ж�
	TR0 = 1;		//��ʱ��0��ʼ��ʱ

}

//��ʱ��0���жϷ�����

void timer0_int(void) interrupt TIMER0_VECTOR
{
	TF0 = 0;
	delay_ms[0]++;
	usart2_tick();
}
