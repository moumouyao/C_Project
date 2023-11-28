#ifndef __KEY_H_
#define __KEY_H_

#include "stm32f4xx.h"                  // Device header
#include "delay.h"

void key_init(void);
uint8_t key_get_num(void);
void TIM3_Int_Init(u16 arr,u16 psc);
uint8_t keyboard_input(void);

#endif
