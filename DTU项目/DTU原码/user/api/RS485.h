#ifndef __RS485_H_
#define __RS485_H_

#include "stm32f10x.h"                  // Device header
#include "usart.h"

void RS485_init(void);
void RS485_TX_ENABLE(void);
void RS485_RX_ENABLE(void);

#endif
