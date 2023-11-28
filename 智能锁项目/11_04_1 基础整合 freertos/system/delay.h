#ifndef __DELAY_H_
#define __DELAY_H_

#include "stm32f4xx.h"                  // Device header
#include "main.h"

void Delay_ms(__IO uint32_t nTime);
void Delay_us(uint32_t xus);
void delay_us(__IO uint32_t nTime);
void Systick_Interrupt(u16 nms);
#endif
