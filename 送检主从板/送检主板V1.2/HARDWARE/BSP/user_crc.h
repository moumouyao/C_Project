/**
  ******************************************************************************
  * 文件名 : user_crc.h
  * 作  者 : 陈光
  * 版  本 ：V1.0
  * 时  间 : 2014年5月31日
  * 公  司 ：汉威光电技术有限公司
  * 摘  要 ：本文件包含了CRC模块的函数原型声明
  ******************************************************************************
  */

#ifndef __USER_CRC_H__
#define __USER_CRC_H__

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"
// #include  "sunwei_led.h"




extern  void User_CRCConfig (void);
extern  uint32_t UserHWCRC32(uint8_t pBuffer[], uint32_t NumOfByte);


#ifdef __cplusplus
}
#endif


#endif

