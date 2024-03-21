#ifndef __DELAY_H
#define __DELAY_H 			   

#include <sys.h>	  
 
void delay_init(uint8_t SYSCLK);
void delay_us(uint32_t nus);
void delay_ms(uint16_t nms);
void delay_sys_us (uint32_t time);
void delay_sys_ms(uint32_t time);

#endif





























