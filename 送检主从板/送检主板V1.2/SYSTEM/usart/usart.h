#ifndef __USART_H
#define __USART_H
#include "stdio.h"	
#include "stm32f4xx_conf.h"
#include "sys.h" 
	
#define RS485_TX_EN		PGout(8)	 //485模式控制.0,接收;1,发送.

extern u16 USART_RX_STA;         		//接收状态标记	
//如果想串口中断接收，请不要注释以下宏定义
void uart_init(u32 bound);


void RS485_Init(uint8_t addr);
void User_USART6Config(u32 bound);
void uart3_send_char(u8 temp);
extern  void User_USART1Config(void);
extern  void USART1TranConfig (uint8_t *p,uint16_t num);  //uart1发送配置	
extern  void User_USART2Config(uint8_t addr);
extern  void User_USART3Config(void);
//extern  void User_USART1SendByte(int8_t);
extern  void User_USART4Config (void);


#define USART_REC_LEN  			200  	//定义最大接收字节数 200
#define EN_USART1_RX 			1		//使能（1）/禁止（0）串口1接收

#endif


