/**
  ******************************************************************************
  * �ļ��� : sw_host.h
  * ��  �� : �¹�
  * ��  �� ��V1.0
  * ʱ  �� : 2014��6��7��
  * ��  ˾ ��������缼�����޹�˾
  * ժ  Ҫ �����ļ�����������������ӿ�����ģ��ĺ���ԭ������
  ******************************************************************************
  */

#ifndef __SW_HOST_H__
#define __SW_HOST_H__

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"
	 
	 
//#define LCP           GPIO_Pin_1   //�������ƿ�
//#define SLAVE485      GPIO_Pin_0    //485������ƶ˿�
//#define SLAVERST      GPIO_Pin_5    //�������ƿ�
//	 
//#define LCPSet         GPIO_WriteBit(GPIOC,LCP,Bit_SET)	  //�������ƿ�
//#define LCPReset       GPIO_WriteBit(GPIOC,LCP,Bit_RESET)
//	 

//#define RS485_TX_EN=1;    GPIO_WriteBit(GPIOD,SLAVE485,Bit_SET)	  
//#define RS485_TX_EN=0;     GPIO_WriteBit(GPIOD,SLAVE485,Bit_RESET)


//#define SLAVERSTSet      GPIO_WriteBit(GPIOB,SLAVERST,Bit_SET)	  //�������ƿ�
//#define SLAVERSTReset    GPIO_WriteBit(GPIOB,SLAVERST,Bit_RESET)  //�������ƿ�

#define LCP           GPIO_Pin_4    //�������ƿ�
#define SLAVE485      GPIO_Pin_8    //485������ƶ˿�


	 
#define LCPSet         GPIO_WriteBit(GPIOE,LCP,Bit_SET)	  //�������ƿ�
#define LCPReset       GPIO_WriteBit(GPIOE,LCP,Bit_RESET)
	 
//#define LCPSet         PEout(1)	  
//#define LCPReset       PEout(0)


//#define RS485_TX_EN=1;    GPIO_WriteBit(GPIOG,SLAVE485,Bit_SET)	  
//#define RS485_TX_EN=0;     GPIO_WriteBit(GPIOG,SLAVE485,Bit_RESET)


#define SLAVERSTSet      GPIO_WriteBit(GPIOE,GPIO_Pin_2,Bit_SET)	  //�������ƿ�
#define SLAVERSTReset    GPIO_WriteBit(GPIOE,GPIO_Pin_2,Bit_RESET)  //�������ƿ�

#pragma pack(1)//���뷽ʽ
/* �������͸��ӻ�����/��������ṹ�� */
typedef  struct 
{
  uint8_t   Yanse;				  //����������ɫ����2:RG/3:RGB
  uint16_t  SHang;					//��������һ�е�������
  uint16_t  SLie;						//��������һ�е����������
  uint16_t  BHang;					//��������һ�е����������
  uint16_t  BLie; 					//��������һ�е����������
	uint8_t   MozuType;       //ģ�����-1:8X4;2:8X8;3:8X16
}ScreenBoxDataStr;
#pragma pack()



#pragma pack(1)//���뷽ʽ
/* ��/����������ýṹ�� */
typedef  struct 
{
  uint16_t HANG;				    //����һ�е��������
  uint8_t  HBYTE;					  //����һ�е�����ֽ���
  uint16_t  LIE;						//����һ�е��������
  uint8_t  FANHUI;					//����һ�е�����ֽ���
  uint8_t  YANSE; 					//����������ɫ����
	uint16_t BYTENUM;         //��/��������ֽ���
}ScreenBoxSetStr;
#pragma pack()




//���ȵ��ڷ�ʽ/����ֵ����δ�洢

	 
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

