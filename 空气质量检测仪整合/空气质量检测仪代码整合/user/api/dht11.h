#ifndef __DHT11_H_
#define __DHT11_H_

#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include <stdio.h>

typedef struct{
	uint8_t tmp;
	uint8_t	hum;
	uint8_t tmp_flag;	//0 Õý 1¸º
	uint8_t tmp_flo;
}DHT;

extern DHT dht_data;

uint8_t dht_get_val(void);
void dth11_init(void);

#endif
