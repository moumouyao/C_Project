#ifndef __ioConfig_H
#define __ioConfig_H

#include "config.h"

sbit LED=P0^4;
sbit BEEP=P0^5;


void IO_Config(void);

#endif
