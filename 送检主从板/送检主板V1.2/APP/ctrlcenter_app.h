/**
  ******************************************************************************
  * �ļ��� : ctrlcenter_app.h
  * ��  �� : �¹�
  * ��  �� ��V1.0
  * ʱ  �� : 2014��7��30��
  * ��  ˾ ��������缼�����޹�˾
  * ժ  Ҫ �����ļ���������������Ĵ�����ص�Ӧ�����������
  ******************************************************************************
  */
	
#ifndef __CTRLCENTER_APP_H__
#define __CTRLCENTER_APP_H__


#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "os_cpu.h"
#include "sw_controlcenter.h"
#include "cJSON.h"
#include "host_app.h"
	 
	 
extern void Task_CtrlCenterHandle(void *p_arg);
extern void Task_FileStore(StrFrameHeader *frame_head);
extern void CtrlCenterFrameHandle (StrFrameHeader * frame_head);
extern void FileStoreHandle (INT8U stype,StrFrameHeader *frame_head);
extern void CheckSelfDeal(StrFrameHeader *frame_head);
extern uint16_t GetOffSet(StrFrameHeader *frame);
extern void Task_UsartCommunication(void *p_arg);
////////////////////////////�¹������json����///////////////////////////////////////
extern int cjson_get_number(cJSON *cjson,char *key);//��ȡcjson����������
extern uint8_t cjson_item_srtcmp(cJSON *cjson,char *key,char *value);//�Ƚ�json�ַ���
extern char *cjson_get_string(cJSON *cjson,char *key);//��ȡjson�ַ���
extern uint8_t cjson_has_key(cJSON *cjson,char *key);//���json�������Ƿ���key
extern uint8_t cjson_get_type(cJSON *cjson);//����msgTyep��ֵ���ж����ĸ�����


extern void SYS_Reset(StrFrameHeader *ret,char *id);//��λ��Ƭ��
extern void PlaySpecifyPlaylist (cJSON *json);//��������Ԥ�Ʊ�
extern uint8_t LightControlType (cJSON *json);//�������ȿ��Ʒ�ʽ������
extern void QueryLightset(uint8_t * p,uint16_t *len,char *id);//��ѯ���ȿ��Ʒ�ʽ������
#ifdef __cplusplus
}
#endif


#endif

