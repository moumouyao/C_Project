/**
  ******************************************************************************
  * 文件名 : i2c.h
  * 作  者 : 陈光
  * 版  本 ：V1.0
  * 时  间 : 2014年6月24日
  * 公  司 ：汉威光电技术有限公司
  * 摘  要 ：本文件包含了I2C模块的函数原型声明
  ******************************************************************************
  */

#ifndef __I2C_H__
#define __I2C_H__

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"

	 



extern void I2C_Test(void);
extern void I2C_GPIOConf (void);
extern unsigned char E2promWriteByte( unsigned char addr, unsigned char data );
extern unsigned char E2promReadByte( unsigned char addr );	 


#ifdef __cplusplus
}
#endif


#endif

