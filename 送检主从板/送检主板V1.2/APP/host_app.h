/**
  ******************************************************************************
  * 文件名 : host_app.h
  * 作  者 : 陈光
  * 版  本 ：V1.0
  * 时  间 : 2014年7月30日
  * 公  司 ：汉威光电技术有限公司
  * 摘  要 ：本文件包含了主控制器与从控制器处理相关应用任务的声明
  ******************************************************************************
  */
	
#ifndef __HOST_APP_H__
#define __HOST_APP_H__


#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "os_cpu.h"
//#include "integer.h"
//#include "ff.h"
#include "sys.h"	

extern uint8_t Slave_Tem;

extern void Task_RS485Tran(void *p_arg);	
extern void Task_ReadPlayList(void *p_arg);

extern void TranSlaveDataHandle (INT8U num,INT8U no);//帧总数，帧号	 
extern INT8U * RS485TranSlaves (INT8U *p,INT16U *slaves_b);

void GetSlaveErrData(void);//获取故障信息
void GetSlaveLedErrData(uint8_t num);//从机获取坏点信息


//extern char FILE_Write(UINT *size,const TCHAR *path,void *data_ptr);
	 
void RS485_Send_Data2(u8 *buf,u8 len);
#ifdef __cplusplus
}
#endif


#endif

