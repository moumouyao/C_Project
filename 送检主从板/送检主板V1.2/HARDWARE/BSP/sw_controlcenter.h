///**
//  ******************************************************************************
//  * 文件名 : sw_controlcenter.h
//  * 作  者 : 陈光
//  * 版  本 ：V1.0
//  * 时  间 : 2014年5月13日
//  * 公  司 ：汉威光电技术有限公司
//  * 摘  要 ：本文件包含了主控制器与控制中心通信模块的函数原型声明
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



//用于标识数据的来源/长度/位置	 
typedef struct frame_data_header
{
	uint8_t type;    //0:UART_TYPE串口来的数据,1:ETH_TYPE网口来的数据
	uint16_t len;    //数据长度
	uint8_t *pdata;  //数据指针
}StrFrameHeader;	 
	 

typedef struct S_Str
{
  uint8_t  str_type;     //字符类型
  uint16_t posx;         //字符位置x
  uint16_t posy;         //字符位置y

  uint8_t  front_type;   //字体类型-仿宋、黑体、楷体、
  uint8_t  front_size;   //字体大小-16、24、32、48
  int8_t  space;         //字符间距
  uint8_t colr; 	       //字符颜色-红
  uint8_t colg; 	       //字符颜色-绿
  uint8_t colb; 	       //字符颜色-蓝
  uint8_t colh;          //字符颜色-琥珀
  uint8_t bmp_num;       //字符串  --在类型为位图时，地址表示BMP名，bmp文件
	uint8_t bmp_head;			 //字符串  --bmp文件名头字母
}StrList;


#pragma pack(1)//对齐方式
typedef struct S_ItemList 
{
  uint8_t	itemyyy;	  //动作条编号0-500--第几条动作
  uint8_t 	delay;      //停留时间S
  uint8_t	mod;       //显示方式
  uint8_t	param;     //移动速度(0-49)*20ms
}ItemList;
#pragma pack()


 typedef struct S_PlayList
 {
   uint8_t itemyyy;	  //动作条编号0-500--第几条动作
   uint8_t delay;      //停留时间S
   uint8_t  mod;       //显示方式
   uint8_t  param;     //移动速度(0-49)*20ms
   uint16_t step;      //步数  
 }PlayList;



//解析帧数据结构
typedef struct S_ItemStruct
{
  ItemList  itemlist;
  uint8_t   rgb_buf[DIS_BUF_CLR][DIS_BUF_LEN];//在sw_conf.h中预定义
}ITEM_STRUCT ;

////////////////////////////////////////2023年新国标解析json数据/////////////////////////////////////////////

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
extern uint8_t FileTypeJudgment (uint8_t *buffer);//文件类型判断1:BMP/2:PLAY.LIST
extern uint16_t InDex(uint8_t *S,uint16_t Snum,uint8_t *T,uint8_t Tnum,uint16_t pos);
extern uint16_t ReadNData(uint8_t *buffer,uint8_t Rnum);   //读取起始地址、要读取数据的最大位长
extern uint8_t MoveNBit (uint16_t temp);
extern uint16_t ReadItemNo(uint8_t *p,uint16_t num);//缓存指针，数据个数
extern uint16_t ReadOneItem(uint8_t *p,uint16_t num);//读取一帧数据填充RGB，返回偏移量
extern uint8_t * ReadNString(uint8_t *p,uint16_t num);
extern void TC_RGBdata (StrList *strlist,uint8_t *buf,uint16_t num);//填充RGB数据
extern void ClrItemBuffer(void);
void TC_BmpItemOnebyOne(StrList *strlist);

void InfmBoard_RetDisplayInfm (uint8_t *p,uint16_t *len, char* id);//返回当前情报板显示的内容
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

///////////////////////////////////////////cjson回应函数//////////////////////////////////////
extern void Control_AckOK(uint8_t * p,uint16_t *len,char *id);
extern void Control_AckERR(uint8_t * p,uint16_t *len,char *id);
#endif

