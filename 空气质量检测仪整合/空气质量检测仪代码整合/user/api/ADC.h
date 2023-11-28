#ifndef __ADC_H_
#define __ADC_H_

#include "stm32f10x.h"                  // Device header
#include "usart.h"
#include "buzzer.h"
#include "led.h"

extern uint16_t ADC1_val[30];
extern float ADC1_value[3];
extern float ADC2V[3];

#define MED_FIL_VAL 11	//采样个数

void AD_light_init(void);
uint16_t ADC1_get_val(uint8_t ADC_Channel);
uint16_t AD_light_Median_filter(void);
uint16_t AD_smoke_Median_filter(void);
uint16_t ADC2_get_val(uint8_t ADC_Channel);
void AD_smoke_init(void);
float AD_temp_Median_filter(void);

void ADC1_DMA_get_val(void);
void ADC1_DMA_init(void);

void ADC2VOL(void);

#endif
