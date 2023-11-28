#ifndef __SU_03T_H_
#define __SU_03T_H_

#include "stm32f10x.h"                  // Device header
#include "usart.h"
#include "led.h"
#include "kqm.h"
#include "buzzer.h"
#include <stdlib.h>
#include <time.h>
#include "dht11.h"
void su_o3t_init(void);
uint8_t usart_get_su_RXflag(void);
void vioce_analysis(void);
void usart_send2su_array(uint8_t *array, uint16_t len);
void usart_send2su_bit(uint8_t data);

#endif
