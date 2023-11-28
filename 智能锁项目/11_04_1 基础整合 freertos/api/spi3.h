#ifndef _SPI3_H_
#define _SPI3_H_

#ifdef __cplusplus
 extern "C" {
#endif


/*
****************************************************************************************
* INCLUDES (头文件包含)
****************************************************************************************
*/
#include "stm32f4xx.h"
#include "io_bit.h"

/*
****************************************************************************************
* TYPEDEFS (数据类型重定义)
****************************************************************************************
*/
 

/*
****************************************************************************************
* EXTERNAL VARIABLES (外部变量)
****************************************************************************************
*/


/*
****************************************************************************************
* CONSTANTS (常量)
****************************************************************************************
*/


/*
****************************************************************************************
* MACROS (宏定义)
****************************************************************************************
*/
//RFID_SPI3_SCLK---PC10
//RFID_SPI3_MISO---PC11
//RFID_SPI3_MOSI---PC12
#define RFID_SPI3_SCLK_GPIO GPIOC
#define RFID_SPI3_SCLK_PIN 10
#define RFID_SPI3_SCLK_AFIO 6
#define RFID_SPI3_SCLK_AFIO_PIN ( (RFID_SPI3_SCLK_PIN>7)?(RFID_SPI3_SCLK_PIN-8):(RFID_SPI3_SCLK_PIN) )

#define RFID_SPI3_MISO_GPIO GPIOC
#define RFID_SPI3_MISO_PIN 11
#define RFID_SPI3_MISO_AFIO 6
#define RFID_SPI3_MISO_AFIO_PIN ( (RFID_SPI3_MISO_PIN>7)?(RFID_SPI3_MISO_PIN-8):(RFID_SPI3_MISO_PIN) )

#define RFID_SPI3_MOSI_GPIO GPIOC
#define RFID_SPI3_MOSI_PIN 12
#define RFID_SPI3_MOSI_AFIO 6
#define RFID_SPI3_MOSI_AFIO_PIN ( (RFID_SPI3_MOSI_PIN>7)?(RFID_SPI3_MOSI_PIN-8):(RFID_SPI3_MOSI_PIN) )


#define SCLK PCout(10)
#define MISO PCin(11)
#define MOSI PCout(12)

/*
****************************************************************************************
* PUBLIC FUNCTIONS DECLARE (声明全局函数)
****************************************************************************************
*/
void SPI3PinInit(void);
uint8_t SPI3TransferByte(uint8_t data);


#ifdef __cplusplus
}
#endif

#endif







