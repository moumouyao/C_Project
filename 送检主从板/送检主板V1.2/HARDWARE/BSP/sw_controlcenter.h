///**
//  ******************************************************************************
//  * �ļ��� : sw_controlcenter.h
//  * ��  �� : �¹�
//  * ��  �� ��V1.0
//  * ʱ  �� : 2014��5��13��
//  * ��  ˾ ��������缼�����޹�˾
//  * ժ  Ҫ �����ļ������������������������ͨ��ģ��ĺ���ԭ������
//  ******************************************************************************
//  */

#ifndef __SW_CONTROLCENTER_H__
#define __SW_CONTROLCENTER_H__

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"
#include "sw_conf.h"
#include "sw_host.h"
#include "base64.h"
#include "cJSON.h"
#include "memp.h"
//#include "ucos_ii.h"
//#include "os_cpu.h"



#define  BMP_TYPE   1
#define  STR_TYPE   2


#define  HEAD_UART_TYPE    2
#define  HEAD_ETH_TYPE     8	 
#define  HEAD_UART4_TYPE   4
#define  HEAD_USART1_TYPE  1
#define  HEAD_USART6_TYPE  6



//���ڱ�ʶ���ݵ���Դ/����/λ��	 
typedef struct frame_data_header
{
	uint8_t type;    //0:UART_TYPE������������,1:ETH_TYPE������������
	uint16_t len;    //���ݳ���
	uint8_t *pdata;  //����ָ��
}StrFrameHeader;	 
	 

typedef struct S_Str
{
  uint8_t  str_type;     //�ַ�����
  uint16_t posx;         //�ַ�λ��x
  uint16_t posy;         //�ַ�λ��y

  uint8_t  front_type;   //��������-���Ρ����塢���塢
  uint8_t  front_size;   //�����С-16��24��32��48
  int8_t  space;         //�ַ����
  uint8_t colr; 	       //�ַ���ɫ-��
  uint8_t colg; 	       //�ַ���ɫ-��
  uint8_t colb; 	       //�ַ���ɫ-��
  uint8_t colh;          //�ַ���ɫ-����
  uint8_t bmp_num;       //�ַ���  --������Ϊλͼʱ����ַ��ʾBMP����bmp�ļ�
	uint8_t bmp_head;			 //�ַ���  --bmp�ļ���ͷ��ĸ
}StrList;


#pragma pack(1)//���뷽ʽ
typedef struct S_ItemList 
{
  uint8_t	itemyyy;	  //���������0-500--�ڼ�������
  uint8_t 	delay;      //ͣ��ʱ��S
  uint8_t	mod;       //��ʾ��ʽ
  uint8_t	param;     //�ƶ��ٶ�(0-49)*20ms
}ItemList;
#pragma pack()


 typedef struct S_PlayList
 {
   uint8_t itemyyy;	  //���������0-500--�ڼ�������
   uint8_t delay;      //ͣ��ʱ��S
   uint8_t  mod;       //��ʾ��ʽ
   uint8_t  param;     //�ƶ��ٶ�(0-49)*20ms
   uint16_t step;      //����  
 }PlayList;



//����֡���ݽṹ
typedef struct S_ItemStruct
{
  ItemList  itemlist;
  uint8_t   rgb_buf[DIS_BUF_CLR][DIS_BUF_LEN];//��sw_conf.h��Ԥ����
}ITEM_STRUCT ;

////////////////////////////////////////2023���¹������json����/////////////////////////////////////////////

typedef struct{
	uint8_t key_size;
	char key[255][255];
} TYP_CJSON_CHECK;

typedef struct {
	uint8_t size;
	TYP_CJSON_CHECK *list[255];
} TYP_CJSON_CHECK_RULES;

typedef struct{
	uint8_t type;
	uint8_t bool_value;
	char *string_value;
	uint32_t array_size;
	double number_value;
	char string_array_value[255][255];
	double number_array_value[255];
}TYP_CJSON_RESULT;
////////////////////////////////////////////////////////////////////////////////////////////////////////////


extern void StrListInit(StrList * p);
extern uint8_t FileTypeJudgment (uint8_t *buffer);//�ļ������ж�1:BMP/2:PLAY.LIST
extern uint16_t InDex(uint8_t *S,uint16_t Snum,uint8_t *T,uint8_t Tnum,uint16_t pos);
extern uint16_t ReadNData(uint8_t *buffer,uint8_t Rnum);   //��ȡ��ʼ��ַ��Ҫ��ȡ���ݵ����λ��
extern uint8_t MoveNBit (uint16_t temp);
extern uint16_t ReadItemNo(uint8_t *p,uint16_t num);//����ָ�룬���ݸ���
extern uint16_t ReadOneItem(uint8_t *p,uint16_t num);//��ȡһ֡�������RGB������ƫ����
extern uint8_t * ReadNString(uint8_t *p,uint16_t num);
extern void TC_RGBdata (StrList *strlist,uint8_t *buf,uint16_t num);//���RGB����
extern void ClrItemBuffer(void);
void TC_BmpItemOnebyOne(StrList *strlist);

void InfmBoard_RetDisplayInfm (uint8_t *p,uint16_t *len, char* id);//���ص�ǰ�鱨����ʾ������
void InfmBoard_ErrAll(uint8_t * p,uint16_t *len, char* id);
void InfmBoard_LoadYuZhiFile(int filetype, char* filename, char * p,uint16_t len);
void upload_dev_file(cJSON *json);
void InfmBoard_Read_File_Ack(char* filename, uint8_t * p,uint16_t *len, char* filetext, uint16_t filelen, char* id);
uint16_t InfmBoard_Read_file_text(int filetype, char* filename, char * text);
//uint16_t InfmBoard_Read_file_text(int filetype, char* filename, char * text,uint16_t textsize, uint8_t * p,uint16_t *len);
uint16_t download_dev_file(cJSON *json, uint8_t * p,uint16_t *len, char* id);
uint8_t delete_dev_file(cJSON *json);
void Dev_File_Dir(cJSON *json, uint8_t * p,uint16_t *len, char* id);
void Json_playlist(char* list, uint16_t len, char* p);
void InfmBoardLed_Err_ACK(uint8_t * p,uint16_t *len, char* id);

///////////////////////////////////////////cjson��Ӧ����//////////////////////////////////////
extern void Control_AckOK(uint8_t * p,uint16_t *len,char *id);
extern void Control_AckERR(uint8_t * p,uint16_t *len,char *id);
#endif

