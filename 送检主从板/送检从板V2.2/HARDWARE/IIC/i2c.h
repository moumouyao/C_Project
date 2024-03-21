#ifndef __I2C_H__
#define __I2C_H__

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "sys.h"

extern void User_I2CConfig (void);
extern void I2C_Standby_24C(void);
extern void I2C_ReadS_24C(uint8_t addr ,uint8_t* pBuffer,uint16_t no);
extern void I2C_WriteS_24C(uint8_t addr,uint8_t* pBuffer,  uint16_t no);
extern void I2C_ByteWrite_24C(uint8_t addr,uint8_t dat);
extern void I2C_PageWrite_24C(uint8_t addr,uint8_t* pBuffer, uint8_t no);
extern void I2C_Test(void);







#ifdef __cplusplus
}
#endif


#endif


