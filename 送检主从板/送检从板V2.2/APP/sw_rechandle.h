#ifndef __SW_RECHANDLE_H__
#define __SW_RECHANDLE_H__

#include "stm32f4xx.h"
#include "sys.h"
#include "sw_config.h"
extern void SlaveAckInit (uint8_t *pot,uint8_t temp);
extern void RecDataHandle (uint8_t * p);
extern void SlaveAckOK (void);
extern void SBoxSetStore (uint8_t *p);
extern void ScreenBoxSetRefresh (uint8_t *p);
extern void FrameStoreHandle (uint8_t *p);
extern void ReadFrameFileHead(uint8_t *p,FrameFileHead *head);
extern void GetLoadNodeTab(LoadDataNode *tab,uint8_t num);           //所有帧加载头表
extern void GetSetEEPROM (void);
void Open_DetectionAck(uint8_t num);	//返回坏点数据
void SlaveAckFault (void);





extern void LoadDataHandle (uint8_t p[][DIS_BUF_LEN],LoadDataNode *p_node);
extern void LoadDotaNowIn (uint8_t buf[][DIS_BUF_LEN],uint32_t addr,uint16_t step);
extern void LoadDotaLeftIn (uint8_t buf[][DIS_BUF_LEN],uint32_t addr,uint16_t step);
extern void LoadDotaLeftOut (uint8_t buf[][DIS_BUF_LEN],uint32_t addr,uint16_t step);
extern void LoadDotaRightIn (uint8_t buf[][DIS_BUF_LEN],uint32_t addr,uint16_t step);
extern void LoadDotaRightOut (uint8_t buf[][DIS_BUF_LEN],uint32_t addr,uint16_t step);
extern void LoadDotaDownIn (uint8_t buf[][DIS_BUF_LEN],uint32_t addr,uint16_t step);
extern void LoadDotaDownOut (uint8_t buf[][DIS_BUF_LEN],uint32_t addr,uint16_t step);
extern void LoadDotaUpIn (uint8_t buf[][DIS_BUF_LEN],uint32_t addr,uint16_t step);
extern void LoadDotaUpOut (uint8_t buf[][DIS_BUF_LEN],uint32_t addr,uint16_t step);
extern void LoadDotaHengIn (uint8_t buf[][DIS_BUF_LEN],uint32_t addr,uint16_t step);
extern void LoadDotaHengOut (uint8_t buf[][DIS_BUF_LEN],uint32_t addr,uint16_t step);
extern void LoadDotaShuIn (uint8_t buf[][DIS_BUF_LEN],uint32_t addr,uint16_t step);
extern void LoadDotaShuOut (uint8_t buf[][DIS_BUF_LEN],uint32_t addr,uint16_t step);
extern void ClrDisplayBuf (void);//清零显示缓冲区
#endif
