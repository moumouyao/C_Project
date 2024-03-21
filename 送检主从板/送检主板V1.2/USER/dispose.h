#include "stm32f4xx.h"
#include "stm32f4xx_rtc.h"
#include "malloc.h"
#include "app.h"
#include "sw_controlcenter.h"

extern unsigned char g_cTrieDeep;



//===========2022-07-29新增=============//
extern unsigned char *getFileName(unsigned char *cmd);
extern unsigned int getFileAddr(unsigned char *cmd);
extern void RecordLog(StrFrameHeader *frame_head,unsigned char cCommand);//记录日志
extern unsigned char SaveFile(StrFrameHeader *frame_head,unsigned char *cFileName,unsigned int iOffsetAddr);
unsigned char up_play(StrFrameHeader *frame_head);

