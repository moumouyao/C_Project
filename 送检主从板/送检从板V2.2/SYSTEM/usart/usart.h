#ifndef __USART_H
#define __USART_H
#include "stdio.h"	
#include "stm32f4xx_conf.h"
#include "sys.h" 

#define USART2_RECV_BUF_SIZE 1024

struct RS485RECV{
	uint8_t data[USART2_RECV_BUF_SIZE];
	uint16_t len;
	uint8_t recv_flag;
};

#define SLAVE485Out    GPIO_WriteBit(GPIOC,GPIO_Pin_12,Bit_SET)	  
#define SLAVE485In     GPIO_WriteBit(GPIOC,GPIO_Pin_12,Bit_RESET)	 

#define U2_SLAVE485Out    GPIO_WriteBit(GPIOH,GPIO_Pin_3,Bit_SET)	  
#define U2_SLAVE485In     GPIO_WriteBit(GPIOH,GPIO_Pin_3,Bit_RESET)	

void uart6_init(u32 bound);
extern void uart_init(u32 bound);
extern void uart2_init(u32 bound);
extern void User_USART3Config (uint8_t slave_addr);
extern void RS485TranConfig (uint8_t *p,uint16_t num);
#endif


