#ifndef __KEY_H
#define	__KEY_H
#include "stm32f4xx.h"



void EXTI_UserKey_Config(void);
uint8_t Key_Value(void);
void SPARE_Gpio(void);
void Key_UpdateItemToSlaves(uint8_t keyvalue);
uint8_t Updatekey (void);
#define KEYCLOSE  0
#define KEYOPEN   1
#define KEY1_read      GPIOC->IDR&GPIO_Pin_4 

#endif           


