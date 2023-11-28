#ifndef __MG200_H_
#define __MG200_H_

#include "stm32f4xx.h"                  // Device header
#include "usart.h"
#include "delay.h"
#include <string.h>

#define START_CODE 	0x6c
#define SEND_ADDR		0x63
#define RECV_ADDR		0x62
#define RESERVE			0x00


void mg200_init(void);
uint8_t Enroll(void);
uint8_t match_fingerprints(void);
uint8_t del_fingerprints(uint8_t finger_id);
uint8_t del_all_fingerprints(void);
uint8_t get_finger_num(void);


#endif
