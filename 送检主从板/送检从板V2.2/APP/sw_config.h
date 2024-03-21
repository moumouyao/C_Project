#ifndef __SW_CONFIG_H__
#define __SW_CONFIG_H__

#include "stm32f4xx.h" 
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"//������


/**
  ******************************************************************************
  *                           ���������������
  ******************************************************************************
  */

#define SCREEN_YANSE	       G_ScreenSet.YANSE	    //����LED��ɫ����
#define SCREEN_HANG     		 G_ScreenSet.HANG	      //����һ���ж��ٸ���
#define SCREEN_LIE					 G_ScreenSet.LIE		    //����һ���ж��ٸ���

#define SCREEN_HBYTE    		 G_ScreenSet.HBYTE	    //����һ���ж����ֽ�
#define SCREEN_FANHUI				 G_ScreenSet.FANHUI		  //����һ���ж����ֽ�
#define SCREEN_BYTE_NUM			 G_ScreenSet.BYTENUM	  //����һ���ж����ֽ�


#define BOX_YANSE    	       G_BoxSet.YANSE	        //����LED��ɫ����
#define BOX_HANG					   G_BoxSet.HANG		      //����һ���ж��ٸ���
#define BOX_LIE						   G_BoxSet.LIE		        //����һ���ж��ٸ���

#define BOX_HBYTE					   G_BoxSet.HBYTE		      //����һ���ж����ֽ�
#define BOX_FANHUI				   G_BoxSet.FANHUI	      //����һ���ж����ֽ�
#define BOX_BYTE_NUM         G_BoxSet.BYTENUM       //����һ���ж����ֽ�


#define DIS_BUF_CLR         	3        	//RGB���ݻ�������ɫ����
#define DIS_BUF_LEN          	4096   	 	//RGB���ݻ���������--����ʵ��Ӧ�ö���  32*36������  



#define Debug_printf 					1	

#define MZ8X4                	0 			 	//ģ�����8X4
#define MZ8X8                	1  				//ģ�����8X8
#define MZ8X16               	2 	 			//ģ�����8X16


#define  OK     							0         //������Ӧ�� 0-�ɹ�
#define  NO     							1         //           1-ʧ��

#define USART3_DMA_TX_BUFFER_MAX_LENGTH 2048
#define USART3_DMA_RX_BUFFER_MAX_LENGTH 2048

//EEROM �ڴ����
#define EEROM_FRAME_FLAG_ADDR     0    //FLASH�д�������֡���ݱ�־λ��ַ	 
#define EEROM_FRAME_CRC32_ADDR    1    //FLASH�д�������֡�������һ֡����У��λ��ַ


#define EEROM_SBOX_SET_FLAG_ADDR  2    //EEROM���Ƿ��������/����/ģ��������
#define EEROM_SBOX_SET_START_ADDR 3    //EEROM������/����/ģ������������ʼ��ַ-

//SPI_FLASH �ڴ����	
#define FLASH_FRAME_TAB_ADDR      0     //�ܹ���֡����ʼ��ַ����¼��֡����������֡�ź����ڵ�ַ	 
#define FLASH_FRAME_FILE_START_ADDR     4096     //���ȫ��֡�ļ�����ʼ��ַ	
#define FLASH_FRAME_FILE_START_SECTOR  	1   

#pragma pack(1)//���뷽ʽ
/* �������͹�������/��������ṹ�� */
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



typedef struct
{
  uint8_t frame_num;     //֡����
	uint8_t frame_no;      //֡���
	uint8_t data_num;      //��������
	uint8_t data_no;       //�������
}FrameFileHead;

typedef struct dnode
{
  uint8_t frame_no;       //֡��
	uint8_t mod;            //��ʾ��ʽ
	uint16_t in_step;       //�����ܲ���
	uint16_t out_step;      //�Ƴ��ܲ���
	uint16_t step;          //�ܵĲ���/��ǰ����
	uint32_t addr;          //flash��ַ
}LoadDataNode;

//������ʾ������Χ
typedef struct
{
  uint16_t HangUint;        //������һ���ж��ٵ�Ԫ��(ģ��)
  uint16_t LieUint;         //������һ���ж��ٵ�Ԫ��(ģ��)
	uint16_t LieStart;        //����������ʼ��ַ
  uint16_t HByteStart;      //����������ʼ��ַ
  uint16_t LieEnd;          //����������ֹ��ַ
  uint16_t HByteEnd;        //����������ֹ��ַ
}ShowAreaTypedef;

//�ӻ�ID
extern uint8_t 	IDSlave;
//�����ӻ���ַ
extern uint8_t 	G_IDSlave0;          //�ӻ���ַ
extern uint8_t 	G_IDHost;             //������ַ

extern uint8_t 	Door1;
extern uint8_t 	Door2;
extern uint8_t 	Door3;
extern uint8_t 	Door4;
extern uint8_t 	Door5;
extern uint8_t 	Door6;
extern uint8_t 	Door7;

extern uint8_t	G_FrameNum;           //���ű����ܵ�֡��
extern uint8_t	G_NewFramesFlag;      //����֡���ݱ�־λ
extern uint8_t	G_LoadDataFlag;       //װ��������ɱ�־λ

extern uint8_t 	SlaveAckOKBuf[24];
extern uint8_t	SlaveAckFaultBuf[24];

extern uint8_t 	G_FrameFlag;           //flash�д����������ݱ�־λ
extern uint32_t	G_FrameCRC32;         //У����
//�ӻ���Ҫ���õĲ������£�
extern uint8_t ScreenBoxSetFlag;
extern ScreenBoxSetStr G_ScreenSet;		//������Ļ�Ĳ���
extern ScreenBoxSetStr G_BoxSet;  		//�Ӱ�Ĳ���
extern uint8_t G_MoZuType;           	//ģ�����0-8X4,1-8X8,2-8X16

extern QueueHandle_t RS485RecMsgQeue;
extern QueueHandle_t FrameStoreMsgQeue;
extern QueueHandle_t FrameLoadMbox;

extern SemaphoreHandle_t RS485TranSem;  //485��Ϣ��

extern LoadDataNode LoadNodeTab[50];

extern ShowAreaTypedef ShowAreaStru;

#define USART3_BUF_SIZE 10240

extern unsigned char USART3_BUF[USART3_BUF_SIZE];//DMA��������ָ��


extern uint8_t	DisplayBuf[DIS_BUF_CLR][DIS_BUF_LEN];  


extern uint16_t     RS485TranCnt;      //RS485�Ѿ��������ݼ�����
extern uint16_t     RS485TranLength;    //RS485Ҫ���͵����ݳ���
extern uint8_t    * RS485TranDataPot;   //RS485Ҫ���͵�����ָ��
#endif

