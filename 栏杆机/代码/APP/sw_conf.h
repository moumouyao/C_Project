#ifndef __SW_CONF_H__
#define __SW_CONF_H__

#ifdef __cplusplus
 extern "C" {
#endif

#include "stm32f4xx.h"
#include "sw_global.h"
//用于标识数据的来源/长度/位置	 
typedef struct frame_data_header
{
	uint8_t type;
	uint16_t len;    //数据长度
	uint8_t *pdata;  //数据指针
}StrFrameHeader;	

typedef struct
{
	uint8_t  hangnum;	//存放行号(小于10有效)	
	uint16_t len; //命令长度
	uint8_t pdata[512];		//用于接受时存放显示内容
}DisPlay_Data;	

typedef struct
{
	uint8_t  HZorZF;//汉字为1，字符为2，初始为0
	uint8_t  HZ_Data[HZ_DATA_LEN];
	uint8_t  ZF_Data[ZF_DATA_LEN];
	uint8_t  ZF_Data1[ZF_DATA_LEN];
}ZM_DATA;

typedef struct
{
	uint16_t lenth;
	uint16_t cnt;
	uint8_t *pdata;
}USARTSEND;

typedef struct
{
	uint16_t integer;
	uint8_t  fen;
	uint8_t  jiao;
}MONEY;

typedef struct
{
	DisPlay_Data data;
	struct DisPlay_Data_Student* next;
}DisPlay_Data_Student;	

#ifdef __cplusplus 
}
#endif


#endif

