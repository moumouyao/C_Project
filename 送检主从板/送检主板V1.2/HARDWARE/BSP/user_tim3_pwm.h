/**
  ******************************************************************************
  * 文件名 : user_tim3_pwm.h
  * 作  者 : 陈光
  * 版  本 ：V1.0
  * 时  间 : 2014年4月13日
  * 公  司 ：汉威光电技术有限公司
  * 摘  要 ：本文件包含了定时器3产生PWM模块的函数原型声明
  ******************************************************************************
  */

#ifndef __USER_TIM3_PWM_H__
#define __USER_TIM3_PWM_H__

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h" 
#include "sw_controlcenter.h"	 
#include "dis_app.h"
#include "sw_global.h"
//#include "i2c.h"

//#define MAX_BRIGHT		64

	 
extern uint8_t bright_level_flag;
extern uint16_t bright_level;
extern uint16_t last_bright_level_flag;	
extern uint8_t bright_calculate;	 
	 
extern void TIM9_CH1_PWM_Init(uint16_t Period,uint16_t Prescaler,uint16_t CCR4_Val);
extern void PwmSet(uint16_t value);
extern void BRIGHT_Level_Set(uint8_t pdata);
extern void BrightRefresh(void);

unsigned char LowOut(unsigned char cmd);
#ifdef __cplusplus
}
#endif


#endif

