#ifndef __TIM_H
#define	__TIM_H
#include "stm32f4xx.h"


extern void TIM1_PWM_Init(uint16_t Period,uint16_t Prescaler,uint16_t CCR4_Val);
extern void PwmSet_R(uint16_t value);
extern void PwmSet_G(uint16_t value);
extern void PwmSet_B(uint16_t value);

#endif           
