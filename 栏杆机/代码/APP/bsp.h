#ifndef _BSP_H
#define _BSP_H

#include "stm32f4xx.h"

void BSP_Init(void);
void lwip_test_ui(u8 mode);
void AlterCodeKey(unsigned char cmd);

#endif
