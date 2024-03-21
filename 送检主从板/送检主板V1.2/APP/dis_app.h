/**
  ******************************************************************************
  * 文件名 : dis_app.h
  * 作  者 : 陈光
  * 版  本 ：V1.0
  * 时  间 : 2014年7月30日
  * 公  司 ：汉威光电技术有限公司
  * 摘  要 ：本文件包含了主控制器控制从控制器显示应用任务
  ******************************************************************************
  */
	
#ifndef __DIS_APP_H__
#define __DIS_APP_H__


#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "sw_controlcenter.h"
#include "os_cpu.h"




extern void Task_DisplayCtrl(void *p_arg);
extern void PlayHandle(ItemList *itemp,uint8_t frame_no);//显示第几帧数据
extern void TranSlavesDis(uint8_t frame_no,uint16_t step);
extern uint8_t UpdateItemToSlaves (void);
////发送参数设置到从机	 
extern void TranDataSetToSlaves(void);
////等待从机加载好数据
extern void WaitSlaveLoadOK (uint8_t slave_no,uint8_t *ack);

//	 



////extern void WaitErrSlaves(uint8_t slave_no,uint8_t *ack); 
//extern void ErrSlaves(void);
//extern void ErrSlaves_2(void);
//void Errcheck_Acknum(uint8_t * p,uint16_t *len);
//INT8U * WaitErrSlaves(uint8_t slave_no,uint8_t *ack);
 #ifdef __cplusplus
}
#endif


#endif

