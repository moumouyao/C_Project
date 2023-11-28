#include "key.h"
#include "tim.h"

sbit KEY1 = P0^0;
sbit KEY2 = P0^1;
sbit KEY3 = P0^2;
sbit KEY4 = P0^3;

unsigned char Get_KeyValue(void)
{
	unsigned char keyValue = 0;
	if(KEY1 == 0) {
		Delay_ms(10);
		if(KEY1 == 0) {
			keyValue = 1;
			while(KEY1 == 0)
			{}
		}
	}
	if(KEY2 == 0) {
		Delay_ms(10);
		if(KEY2 == 0) {
			keyValue = 2;
			while(KEY2 == 0)
			{}
		}
	}
	if(KEY3 == 0) {
		Delay_ms(10);
		if(KEY3 == 0) {
			keyValue = 3;
			while(KEY3 == 0)
			{}
		}
	}
	if(KEY4 == 0) {
		Delay_ms(10);
		if(KEY4 == 0) {
			keyValue = 4;
			while(KEY4 == 0)
			{}
		}
	}
	return keyValue;
}






