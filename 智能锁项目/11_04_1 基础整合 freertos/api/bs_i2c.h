#ifndef __BS_I2C_H_
#define __BS_I2C_H_

#include "stm32f4xx.h"                  // Device header
#include "usart.h"

#include "delay.h"

	void bs_i2c_w_scl(uint8_t bitval);
	void bs_i2c_w_sda(uint8_t bitval);
	uint8_t bs_i2c_r_sda(void);
	void bs_i2c_init(void);
	void bs_i2c_start(void);
	void bs_i2c_stop(void);
	uint8_t bs_i2c_busy(void);
	void bs_i2c_sendbyte(uint8_t byte);
	uint8_t bs_i2c_recv_byte(void);
	uint8_t bs_i2c_recv_ack(void);
	void bs_i2c_send_ack(uint8_t ackbit);
	
#endif
