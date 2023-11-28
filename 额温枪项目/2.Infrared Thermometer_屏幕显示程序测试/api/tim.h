#ifndef __TIM_H
#define __TIM_H

#include "config.h"

extern unsigned long Systick_Tick;
extern unsigned int Led_TaskTime[2];
extern unsigned int View_TaskTime[2];

void Tim0_Config(void);
void Delay_ms(unsigned int ms);
#endif
