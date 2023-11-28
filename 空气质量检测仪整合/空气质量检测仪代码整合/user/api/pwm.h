#ifndef __PWM_H_
#define __PWM_H_

#include "stm32f10x.h"                  // Device header

void pwm_tim3_init(void);
void pwm_tim1_init(void);
void pwm_set_compare1(TIM_TypeDef * TIM, uint16_t compare);
void pwm_set_compare2(TIM_TypeDef * TIM, uint16_t compare);

#endif
