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

