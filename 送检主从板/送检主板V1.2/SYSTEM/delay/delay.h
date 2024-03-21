//#ifndef __DELAY_H
//#define __DELAY_H 			   
//#include <sys.h>	  
////////////////////////////////////////////////////////////////////////////////////  
////本程序只供学习使用，未经作者许可，不得用于其它任何用途
////ALIENTEK STM32F407开发板
////使用SysTick的普通计数模式对延迟进行管理(支持ucosii)
////包括delay_us,delay_ms
////正点原子@ALIENTEK
////技术论坛:www.openedv.com
////修改日期:2014/5/2
////版本：V1.0
////版权所有，盗版必究。
////Copyright(C) 广州市星翼电子科技有限公司 2014-2024
////All rights reserved
////********************************************************************************
////修改说明
////无
//////////////////////////////////////////////////////////////////////////////////// 	 
//void delay_init(u8 SYSCLK);
//void delay_ms(u16 nms);
//void delay_us(u32 nus);
////void delay_ms (uint16_t time);
//#endif


/**
  ******************************************************************************
  * 文件名 : user_delay.h
  * 作  者 : 陈光
  * 版  本 ：V1.0
  * 时  间 : 2014年7月31日
  * 公  司 ：汉威光电技术有限公司
  * 摘  要 ：本文件包含了软件延时函数声明
  ******************************************************************************
  */

#ifndef __USER_DELAY_H__
#define __USER_DELAY_H__

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"
#include <sys.h>	
	 
void delay_init(u8 SYSCLK);
extern void Delay(uint32_t nCount);
extern void delay_us (uint32_t time);
extern void delay_ms (uint16_t time);






#ifdef __cplusplus
}
#endif


#endif
	



























