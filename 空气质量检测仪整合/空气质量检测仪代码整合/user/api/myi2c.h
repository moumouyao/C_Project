#ifndef __MYI2C_H_
#define __MYI2C_H_

#include "stm32f10x.h"                  // Device header
#include "delay.h"

	void myi2c_w_scl(uint8_t bitval);
	void myi2c_w_sda(uint8_t bitval);
	uint8_t myi2c_r_sda(void);
	void myi2c_init(void);
	void i2c_start(void);
	void i2c_stop(void);
	void myi2c_sendbyte(uint8_t byte);
	uint8_t myi2c_recv_byte(void);
	uint8_t myi2c_recv_ack(void);
	void myi2c_send_ack(uint8_t ackbit);

#endif
