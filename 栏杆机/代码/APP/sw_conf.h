#ifndef __SW_CONF_H__
#define __SW_CONF_H__

#ifdef __cplusplus
 extern "C" {
#endif

#include "stm32f4xx.h"
#include "sw_global.h"
//���ڱ�ʶ���ݵ���Դ/����/λ��	 
typedef struct frame_data_header
{
	uint8_t type;
	uint16_t len;    //���ݳ���
	uint8_t *pdata;  //����ָ��
}StrFrameHeader;	

typedef struct
{
	uint8_t  hangnum;	//����к�(С��10��Ч)	
	uint16_t len; //�����
	uint8_t pdata[512];		//���ڽ���ʱ�����ʾ����
}DisPlay_Data;	

typedef struct
{
	uint8_t  HZorZF;//����Ϊ1���ַ�Ϊ2����ʼΪ0
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

