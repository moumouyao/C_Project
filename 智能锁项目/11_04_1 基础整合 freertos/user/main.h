
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"
#include "delay.h"
#include "key.h"
#include "usart.h"
#include <stdio.h>
#include "AT24.h"
#include "bs8116.h"
#include "my_spi_flash.h"
#include "my_lcd.h"
#include "mg200.h"
#include "voice.h"
#include "my_rtc.h"
#include "esp32.h"
#include "FreeRTOS.h"
#include "task.h"
#include "myrtos.h"
#include "rfid.h"
#include "spi3.h"
#include "motor.h"



/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/

/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void TimingDelay_Decrement(void);
void TimingDelay_Decrementus(void);

#endif /* __MAIN_H */

