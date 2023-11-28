#ifndef __AT24_H_
#define __AT24_H_

#include "stm32f4xx.h"                  // Device header
#include "myi2c.h"
#include "usart.h"
#include "delay.h"

void AT24_init(void);
uint8_t AT24_write_byte(uint16_t addr, uint8_t data);
uint8_t AT24_read_byte(uint16_t addr);
uint8_t AT24_write_page(uint16_t addr, uint8_t num, uint8_t *data);
void AT24_WriteBuffer(uint8_t *pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite);
uint8_t AT24_readBuffer(uint16_t addr, uint16_t num, uint8_t *recvdata);
#endif
