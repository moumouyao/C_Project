#ifndef __KQM_H_
#define __KQM_H_

#include "stm32f10x.h"                  // Device header
#include "usart.h"
#include "string.h"

extern uint8_t usart_kqm_RXpacket[8];
typedef struct{
	float VOC;
	float CHO;
	uint8_t CO2;
}air_data;
extern air_data kqm_data;
void kqm_init(void);
uint8_t usart_get_kqm__RXflag(void);
void air_analysis(void);

#endif
