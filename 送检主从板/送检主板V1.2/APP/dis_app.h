/**
  ******************************************************************************
  * �ļ��� : dis_app.h
  * ��  �� : �¹�
  * ��  �� ��V1.0
  * ʱ  �� : 2014��7��30��
  * ��  ˾ ��������缼�����޹�˾
  * ժ  Ҫ �����ļ������������������ƴӿ�������ʾӦ������
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
extern void PlayHandle(ItemList *itemp,uint8_t frame_no);//��ʾ�ڼ�֡����
extern void TranSlavesDis(uint8_t frame_no,uint16_t step);
extern uint8_t UpdateItemToSlaves (void);
////���Ͳ������õ��ӻ�	 
extern void TranDataSetToSlaves(void);
////�ȴ��ӻ����غ�����
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

