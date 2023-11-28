#ifndef __INTERRUPT_H_
#define __INTERRUPT_H_

#include "stm32f10x.h"                  // Device header
#include "usart.h"
void EXTI0_IRQHandler(void);
void EXTI4_IRQHandler(void);
void EXTI9_5_IRQHandler(void);
uint8_t get_k2_val(void);
#endif
