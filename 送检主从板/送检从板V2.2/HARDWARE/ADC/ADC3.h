#ifndef __ADC3_H__
#define __ADC3_H__

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "sys.h"
#include "delay.h"

void Adc3_Init(void);
uint16_t Get_ADC3_val(uint8_t ch);
uint16_t Get_ADC3_Average_val(uint8_t ch, uint8_t time);

#ifdef __cplusplus
}
#endif


#endif

