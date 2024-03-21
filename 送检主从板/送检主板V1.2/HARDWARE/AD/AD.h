/**
  ******************************************************************************
  * 文件名 : AD.h
  * 作  者 : 陈光
  * 版  本 ：V1.0
  * 时  间 : 2014年8月12日
  * 公  司 ：汉威光电技术有限公司
  * 摘  要 ：本文件包含了AD模块的函数原型声明
  ******************************************************************************
  */

#ifndef __AD_H__
#define __AD_H__

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "includes.h"




extern  void User_ADConfig (void);
	 
extern  __IO uint16_t ADC1ConvertedValue;
extern void ADC3_CH5_DMA_Config(void);



#ifdef __cplusplus
}
#endif


#endif


