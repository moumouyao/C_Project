#include "tim.h"
#include "ioConfig.h"

unsigned long Systick_Tick = 0;
unsigned int Led_TaskTime[2] = {0, 500};
unsigned int View_TaskTime[2] = {0, 500};

/********************* Timer0中断函数************************/
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

void Tim0_Config(void)//1毫秒@24.000MHz
{
	AUXR |= 0x80;		//定时器时钟1T模式
	TMOD &= 0xF0;		//设置定时器模式
	TL0 = 0x40;		//设置定时初始值
	TH0 = 0xA2;		//设置定时初始值
	TF0 = 0;		//清除TF0标志
	TR0 = 1;		//定时器0开始计时
	ET0 = 1;				//使能定时器0中断
}




