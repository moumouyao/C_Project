#ifndef __USER_CRC_H__
#define __USER_CRC_H__

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"

extern  void User_CRCConfig (void);
extern  uint32_t UserHWCRC32(uint8_t pBuffer[], uint32_t NumOfByte);

#ifdef __cplusplus
}
#endif

#endif

