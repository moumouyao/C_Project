#ifndef _LWIP_APP_H
#define _LWIP_APP_H
#include "sw_controlcenter.h"

void lwip_task(void *pdata);
StrFrameHeader* SW_Escape(StrFrameHeader *frame_head); 
//void SW_UnEscape(struct netbuf *buf,INT8U *dbuf,INT16U *len);
#endif

