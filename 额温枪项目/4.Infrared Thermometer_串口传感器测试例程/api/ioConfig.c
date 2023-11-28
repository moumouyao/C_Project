#include "ioConfig.h"

void IO_Config(void)
{
	//LED BEEP
	P0M1 &= ~(0x3<<4),	P0M0 |= (0x3<<4);//ÍÆÍìÊä³ö
	//KEY
	P0M1 |=  (0xF<<0),	P0M0 &= ~(0xF<<0);
	//BAT
	P1M1 |=  (0x1<<3),	P1M0 &= ~(0x1<<3);
	//EEPROM
	P1M1 &= ~(1<<4),	P1M0 |=  (1<<4);
	P1M1 &= ~(1<<5),	P1M0 &= ~(1<<5);
	//MLX906

	//LCD
	P2M1 &= ~(1<<0),	P2M0 |=  (1<<0);
	P4M1 &= ~(0x1F<<0),	P4M0 |=  (0x1F<<0);
}
















