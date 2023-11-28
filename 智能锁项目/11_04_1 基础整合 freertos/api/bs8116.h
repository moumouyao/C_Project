#ifndef __BS8116_H_
#define __BS8116_H_

#include "stm32f4xx.h"                  // Device header
uint8_t BS8116ReadKey(void) ;
void BS8116_init(void);
void i2c_w_scl(uint8_t bitval);
void i2c_w_sda(uint8_t bitval);
uint8_t i2c_r_sda(void);
uint8_t i2c_r_scl(void);
void i2c_init(void);
void iic_start(void);
void iic_stop(void);
void iic_nack(void);
void iic_ack(void);
uint8_t iic_read_byte(uint8_t ack);
uint8_t I2C_WriteByte(uint8_t byte);

/************/


#endif
