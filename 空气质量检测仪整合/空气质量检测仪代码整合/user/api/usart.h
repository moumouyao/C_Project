#ifndef __USART_H_
#define __USART_H_

#include "stm32f10x.h"                  // Device header
#include <stdio.h>
#include <stdarg.h>
#include	"esp8266.h"

#define LEN 4		//定义数据包长度
#define PACKET_HEAD 0xFF	//定义包头
#define PACKET_END 0xFE //定义包尾
#define STRLEN 100	//定义文本数据包长度

extern uint8_t usart_TXpacket[];
extern uint8_t usart_RXpacket[];
extern char usart_RX_str_packet[STRLEN];

void usart_init(void);
void usart3_init(void);
void usart_sendbit(USART_TypeDef* USARTx, uint8_t data);
void usaer_sendstr(USART_TypeDef* USARTx, uint8_t str[], uint8_t len);
void usart_send_array(USART_TypeDef* USARTx, uint8_t *array, uint16_t len);
void usart_send_string(USART_TypeDef* USARTx, char *str);
void usart_send_num(USART_TypeDef* USARTx, uint32_t num, uint8_t len);
uint8_t usart_recv(USART_TypeDef* USARTx);
void usart_printf(char *format, ...);
uint8_t usart_get_RXflag(void);
uint8_t usart_get_RXdata(void);
void usart_send_hex_packet(USART_TypeDef* USARTx);
#endif
