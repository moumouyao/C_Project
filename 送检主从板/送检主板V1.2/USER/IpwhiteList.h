#ifndef __IpwhiteList_H__
#define __IpwhiteList_H__
#include "os_cpu.h"
#include "sw_controlcenter.h"

extern void ExtractWhiteList(StrFrameHeader *frame_head);//写入白名单
extern void WirteEEROMwhite(void);//写入EEROM
extern void WhiteListACK(unsigned char* p,unsigned short*len);//写入成功回复
extern void WhiteListACKdata(unsigned char* p,unsigned short*len);//读取IP白名单













#endif

