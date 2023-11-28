#ifndef __TIMER_H_
#define __TIMER_H_

#include "stm32f10x.h"                  // Device header
void timer2_init(void);
void time3_init(void);
void pwm_set_compare1(uint16_t compare);

#endif
