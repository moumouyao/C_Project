#include "timer.h"

unsigned int delay_ms[2] = {0};


void Delay_ms(unsigned int xus)
{
	delay_ms[1] = xus;
	delay_ms[0] = 0;
	while(delay_ms[0] < delay_ms[1]);

}



//配置定时器0，周期 1ms
void Timer0Init(void)		//1毫秒@24.000MHz
{
	AUXR |= 0x80;		//定时器时钟1T模式
	TMOD &= 0xF0;		//设置定时器模式
	TL0 = 0x40;		//设置定时初始值
	TH0 = 0xA2;		//设置定时初始值
	TF0 = 0;		//清除TF0标志
	ET0 = 1;		//开启定时器1中断
	TR0 = 1;		//定时器0开始计时

}

//定时器0，中断服务函数

void timer0_int(void) interrupt TIMER0_VECTOR
{
	TF0 = 0;
	delay_ms[0]++;
	usart2_tick();
}
