#ifndef __LU90614_H_
#define __LU90614_H_

#include <STC8.H>
#include "usart.h"
#include "timer.h"
#include "stdio.h"

void LU90614_init(void);
char read_body_temperature(void);
#endif
