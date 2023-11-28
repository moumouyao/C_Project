#ifndef __MYDMA_H_
#define __MYDMA_H_

#include "stm32f10x.h"                  // Device header

	void MyDMA_init(uint32_t AddrA, uint32_t AddrB, uint16_t size);
	void MyDMA_Transfer(void);

#endif
