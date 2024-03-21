#ifndef __SW_CONFIG_H__
#define __SW_CONFIG_H__

#include "stm32f4xx.h" 
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"//互斥量


/**
  ******************************************************************************
  *                           配置箱体屏体参数
  ******************************************************************************
  */

#define SCREEN_YANSE	       G_ScreenSet.YANSE	    //屏体LED颜色种类
#define SCREEN_HANG     		 G_ScreenSet.HANG	      //屏体一行有多少个点
#define SCREEN_LIE					 G_ScreenSet.LIE		    //屏体一列有多少个点

#define SCREEN_HBYTE    		 G_ScreenSet.HBYTE	    //屏体一行有多少字节
#define SCREEN_FANHUI				 G_ScreenSet.FANHUI		  //屏体一列有多少字节
#define SCREEN_BYTE_NUM			 G_ScreenSet.BYTENUM	  //屏体一共有多少字节


#define BOX_YANSE    	       G_BoxSet.YANSE	        //屏体LED颜色种类
#define BOX_HANG					   G_BoxSet.HANG		      //箱体一行有多少个点
#define BOX_LIE						   G_BoxSet.LIE		        //箱体一列有多少个点

#define BOX_HBYTE					   G_BoxSet.HBYTE		      //箱体一行有多少字节
#define BOX_FANHUI				   G_BoxSet.FANHUI	      //箱体一行有多少字节
#define BOX_BYTE_NUM         G_BoxSet.BYTENUM       //箱体一行有多少字节


#define DIS_BUF_CLR         	3        	//RGB数据缓冲区颜色种类
#define DIS_BUF_LEN          	4096   	 	//RGB数据缓冲区长度--根据实际应用而定  32*36个箱体  



#define Debug_printf 					1	

#define MZ8X4                	0 			 	//模组类别8X4
#define MZ8X8                	1  				//模组类别8X8
#define MZ8X16               	2 	 			//模组类别8X16


#define  OK     							0         //给主机应答 0-成功
#define  NO     							1         //           1-失败

#define USART3_DMA_TX_BUFFER_MAX_LENGTH 2048
#define USART3_DMA_RX_BUFFER_MAX_LENGTH 2048

//EEROM 内存管理
#define EEROM_FRAME_FLAG_ADDR     0    //FLASH中存有完整帧数据标志位地址	 
#define EEROM_FRAME_CRC32_ADDR    1    //FLASH中存有完整帧数据最后一帧数据校验位地址


#define EEROM_SBOX_SET_FLAG_ADDR  2    //EEROM中是否存有箱体/屏体/模组类别参数
#define EEROM_SBOX_SET_START_ADDR 3    //EEROM中箱体/屏体/模组类别参数的起始地址-

//SPI_FLASH 内存管理	
#define FLASH_FRAME_TAB_ADDR      0     //总共的帧表起始地址，记录着帧总数、各个帧号和所在地址	 
#define FLASH_FRAME_FILE_START_ADDR     4096     //存放全部帧文件的起始地址	
#define FLASH_FRAME_FILE_START_SECTOR  	1   

#pragma pack(1)//对齐方式
/* 主机发送过来的屏/箱体参数结构体 */
typedef  struct 
{
  uint8_t   Yanse;				  //代表屏体颜色种类2:RG/3:RGB
  uint16_t  SHang;					//代表屏体一行点阵数量
  uint16_t  SLie;						//代表屏体一列点阵的像素数
  uint16_t  BHang;					//代表箱体一行点阵的像素数
  uint16_t  BLie; 					//代表箱体一列点阵的像素数
	uint8_t   MozuType;       //模组类别-1:8X4;2:8X8;3:8X16
}ScreenBoxDataStr;
#pragma pack()


#pragma pack(1)//对齐方式
/* 屏/箱体参数设置结构体 */
typedef  struct 
{
  uint16_t HANG;				    //代表一行点阵的数量
  uint8_t  HBYTE;					  //代表一行点阵的字节数
  uint16_t  LIE;						//代表一列点阵的数量
  uint8_t  FANHUI;					//代表一列点阵的字节数
  uint8_t  YANSE; 					//代表屏体颜色种类
	uint16_t BYTENUM;         //屏/箱体点阵字节数
}ScreenBoxSetStr;
#pragma pack()



typedef struct
{
  uint8_t frame_num;     //帧总数
	uint8_t frame_no;      //帧序号
	uint8_t data_num;      //数据总数
	uint8_t data_no;       //数据序号
}FrameFileHead;

typedef struct dnode
{
  uint8_t frame_no;       //帧号
	uint8_t mod;            //显示方式
	uint16_t in_step;       //移入总步数
	uint16_t out_step;      //移出总步数
	uint16_t step;          //总的步数/当前步数
	uint32_t addr;          //flash地址
}LoadDataNode;

//箱体显示参数范围
typedef struct
{
  uint16_t HangUint;        //本箱体一行有多少单元板(模组)
  uint16_t LieUint;         //本箱体一列有多少单元板(模组)
	uint16_t LieStart;        //本箱体列起始地址
  uint16_t HByteStart;      //本箱体行起始地址
  uint16_t LieEnd;          //本箱体列终止地址
  uint16_t HByteEnd;        //本箱体行终止地址
}ShowAreaTypedef;

//从机ID
extern uint8_t 	IDSlave;
//主机从机地址
extern uint8_t 	G_IDSlave0;          //从机地址
extern uint8_t 	G_IDHost;             //主机地址

extern uint8_t 	Door1;
extern uint8_t 	Door2;
extern uint8_t 	Door3;
extern uint8_t 	Door4;
extern uint8_t 	Door5;
extern uint8_t 	Door6;
extern uint8_t 	Door7;

extern uint8_t	G_FrameNum;           //播放表中总的帧数
extern uint8_t	G_NewFramesFlag;      //更新帧数据标志位
extern uint8_t	G_LoadDataFlag;       //装载数据完成标志位

extern uint8_t 	SlaveAckOKBuf[24];
extern uint8_t	SlaveAckFaultBuf[24];

extern uint8_t 	G_FrameFlag;           //flash中存有完整数据标志位
extern uint32_t	G_FrameCRC32;         //校验结果
//从机需要设置的参数如下：
extern uint8_t ScreenBoxSetFlag;
extern ScreenBoxSetStr G_ScreenSet;		//整个屏幕的参数
extern ScreenBoxSetStr G_BoxSet;  		//从板的参数
extern uint8_t G_MoZuType;           	//模组类别：0-8X4,1-8X8,2-8X16

extern QueueHandle_t RS485RecMsgQeue;
extern QueueHandle_t FrameStoreMsgQeue;
extern QueueHandle_t FrameLoadMbox;

extern SemaphoreHandle_t RS485TranSem;  //485信息量

extern LoadDataNode LoadNodeTab[50];

extern ShowAreaTypedef ShowAreaStru;

#define USART3_BUF_SIZE 10240

extern unsigned char USART3_BUF[USART3_BUF_SIZE];//DMA缓存数据指针


extern uint8_t	DisplayBuf[DIS_BUF_CLR][DIS_BUF_LEN];  


extern uint16_t     RS485TranCnt;      //RS485已经发的数据计数器
extern uint16_t     RS485TranLength;    //RS485要发送的数据长度
extern uint8_t    * RS485TranDataPot;   //RS485要发送的数据指针
#endif

