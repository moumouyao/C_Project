#ifndef __KEY_H_
#define __KEY_H_

#include "stm32f10x.h"                  // Device header

void key_init(void);
uint8_t key_get_num(void);
void key1_interrupt_init(void);
void key2_interrupt_init(void);
void key3_interrupt_init(void);
void key4_interrupt_init(void);
#endif

//#ifndef __KEY_H_
//#define __KEY_H_

//#include "stm32f10x.h"                  // Device header

//void key_init(void);
//uint8_t key_check(void);

//#endif
