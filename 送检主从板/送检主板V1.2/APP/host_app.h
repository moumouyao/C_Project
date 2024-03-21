/**
  ******************************************************************************
  * �ļ��� : host_app.h
  * ��  �� : �¹�
  * ��  �� ��V1.0
  * ʱ  �� : 2014��7��30��
  * ��  ˾ ��������缼�����޹�˾
  * ժ  Ҫ �����ļ�����������������ӿ������������Ӧ�����������
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

extern void TranSlaveDataHandle (INT8U num,INT8U no);//֡������֡��	 
extern INT8U * RS485TranSlaves (INT8U *p,INT16U *slaves_b);

void GetSlaveErrData(void);//��ȡ������Ϣ
void GetSlaveLedErrData(uint8_t num);//�ӻ���ȡ������Ϣ


//extern char FILE_Write(UINT *size,const TCHAR *path,void *data_ptr);
	 
void RS485_Send_Data2(u8 *buf,u8 len);
#ifdef __cplusplus
}
#endif


#endif

