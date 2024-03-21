/**
  ******************************************************************************
  * 文件名 : ctrlcenter_app.h
  * 作  者 : 陈光
  * 版  本 ：V1.0
  * 时  间 : 2014年7月30日
  * 公  司 ：汉威光电技术有限公司
  * 摘  要 ：本文件包含了与控制中心处理相关的应用任务的声明
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
////////////////////////////新国标解析json数据///////////////////////////////////////
extern int cjson_get_number(cJSON *cjson,char *key);//获取cjson整数型数字
extern uint8_t cjson_item_srtcmp(cJSON *cjson,char *key,char *value);//比较json字符串
extern char *cjson_get_string(cJSON *cjson,char *key);//获取json字符串
extern uint8_t cjson_has_key(cJSON *cjson,char *key);//检测json数据中是否是key
extern uint8_t cjson_get_type(cJSON *cjson);//根据msgTyep的值来判断是哪个命令


extern void SYS_Reset(StrFrameHeader *ret,char *id);//复位单片机
extern void PlaySpecifyPlaylist (cJSON *json);//解析播放预制表
extern uint8_t LightControlType (cJSON *json);//调整亮度控制方式和亮度
extern void QueryLightset(uint8_t * p,uint16_t *len,char *id);//查询亮度控制方式和亮度
#ifdef __cplusplus
}
#endif


#endif

