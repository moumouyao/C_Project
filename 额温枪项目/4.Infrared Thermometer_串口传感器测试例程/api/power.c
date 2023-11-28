#include "power.h"


void UART3_Isr() interrupt 17
{

}

void Power_Config(void)
{
	P_SW2 &= ~(1<<1);
	IE2 |= 1<<3;
}


void Power_Off(void)
{
	PCON |= 1<<1;
}




















