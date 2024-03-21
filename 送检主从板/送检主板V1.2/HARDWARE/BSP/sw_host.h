/**
  ******************************************************************************
  * 文件名 : sw_host.h
  * 作  者 : 陈光
  * 版  本 ：V1.0
  * 时  间 : 2014年6月7日
  * 公  司 ：汉威光电技术有限公司
  * 摘  要 ：本文件包含了主控制器与从控制器模块的函数原型声明
  ******************************************************************************
  */

#ifndef __SW_HOST_H__
#define __SW_HOST_H__

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"
	 
	 
//#define LCP           GPIO_Pin_1   //主机控制口
//#define SLAVE485      GPIO_Pin_0    //485方向控制端口
//#define SLAVERST      GPIO_Pin_5    //主机控制口
//	 
//#define LCPSet         GPIO_WriteBit(GPIOC,LCP,Bit_SET)	  //主机控制口
//#define LCPReset       GPIO_WriteBit(GPIOC,LCP,Bit_RESET)
//	 

//#define RS485_TX_EN=1;    GPIO_WriteBit(GPIOD,SLAVE485,Bit_SET)	  
//#define RS485_TX_EN=0;     GPIO_WriteBit(GPIOD,SLAVE485,Bit_RESET)


//#define SLAVERSTSet      GPIO_WriteBit(GPIOB,SLAVERST,Bit_SET)	  //主机控制口
//#define SLAVERSTReset    GPIO_WriteBit(GPIOB,SLAVERST,Bit_RESET)  //主机控制口

#define LCP           GPIO_Pin_4    //主机控制口
#define SLAVE485      GPIO_Pin_8    //485方向控制端口


	 
#define LCPSet         GPIO_WriteBit(GPIOE,LCP,Bit_SET)	  //主机控制口
#define LCPReset       GPIO_WriteBit(GPIOE,LCP,Bit_RESET)
	 
//#define LCPSet         PEout(1)	  
//#define LCPReset       PEout(0)


//#define RS485_TX_EN=1;    GPIO_WriteBit(GPIOG,SLAVE485,Bit_SET)	  
//#define RS485_TX_EN=0;     GPIO_WriteBit(GPIOG,SLAVE485,Bit_RESET)


#define SLAVERSTSet      GPIO_WriteBit(GPIOE,GPIO_Pin_2,Bit_SET)	  //主机控制口
#define SLAVERSTReset    GPIO_WriteBit(GPIOE,GPIO_Pin_2,Bit_RESET)  //主机控制口

#pragma pack(1)//对齐方式
/* 主机发送给从机的屏/箱体参数结构体 */
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




//亮度调节方式/亮度值参数未存储

	 
extern  void RS485TranConfig (uint8_t *p,uint16_t num);
extern  void SW_HostGPIOConfig (void);
extern  void GetE2PROMSet(void );//G_ItemFlag,G_ItemUpdateFlag,,G_FrameCRC32
extern  void RefreshDataSet (uint8_t *p);
extern  void StoreDataSet (uint8_t *p,uint8_t len);
extern  void Usart_StoreDataSet(uint8_t *p,uint8_t len,uint8_t type);



#ifdef __cplusplus
}
#endif


#endif

