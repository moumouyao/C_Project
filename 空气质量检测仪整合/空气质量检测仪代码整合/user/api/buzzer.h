#ifndef __BUZZER_H_
#define __BUZZER_H_

#define BUZZER_PORT GPIOC
#define BUZZER_PIN 	1<<0

#include "stm32f10x.h"                  // Device header
void buzzer_init(void);
void buzzer_enable(void);
void buzzer_disable(void);
void buzzer_turn(GPIO_TypeDef *beef_port,uint16_t beef_pin);
#endif
