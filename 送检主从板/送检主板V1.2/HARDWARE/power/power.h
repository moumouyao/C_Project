#ifndef  __POWER_H
#define	__POWER_H
#include "stm32f4xx.h"
#include "includes.h"
//#include "wdg.h"
#include "sw_controlcenter.h"
#include "cJSON.h"

void Power_Gpio(void);
void EXTI_Power_Config(void);
void Control_Power(cJSON *json);


#endif

