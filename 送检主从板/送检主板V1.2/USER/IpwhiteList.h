#ifndef __IpwhiteList_H__
#define __IpwhiteList_H__
#include "os_cpu.h"
#include "sw_controlcenter.h"

extern void ExtractWhiteList(StrFrameHeader *frame_head);//д�������
extern void WirteEEROMwhite(void);//д��EEROM
extern void WhiteListACK(unsigned char* p,unsigned short*len);//д��ɹ��ظ�
extern void WhiteListACKdata(unsigned char* p,unsigned short*len);//��ȡIP������













#endif

