#ifndef __I2C2_H__
#define __I2C2_H__

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "sys.h"

#define SHT20_CMD_MEASURE_TEMP 0xe3
#define SHT20_CMD_MEASURE_HUMI 0xe5

void User_I2C2Config (void);
extern void I2C2_ByteWrite_SHT20(uint8_t addr,uint8_t dat);
extern void I2C_ReadS_24C(uint8_t addr ,uint8_t* pBuffer,uint16_t no);

uint16_t SHT20_Get_Data(uint8_t cmd);
uint8_t CRC_Check(uint8_t *ptr, uint8_t len, uint8_t checksum);
#ifdef __cplusplus
}
#endif


#endif


