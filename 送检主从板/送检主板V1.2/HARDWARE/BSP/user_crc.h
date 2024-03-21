/**
  ******************************************************************************
  * �ļ��� : user_crc.h
  * ��  �� : �¹�
  * ��  �� ��V1.0
  * ʱ  �� : 2014��5��31��
  * ��  ˾ ��������缼�����޹�˾
  * ժ  Ҫ �����ļ�������CRCģ��ĺ���ԭ������
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

