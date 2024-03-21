#ifndef __Code_H__
#define __Code_H__
#include "os_cpu.h"
#include "sw_controlcenter.h"
#include "delay.h"

#define Code_CS    PGout(15) //Ƭѡ�ź�
#define Code_Rst   PBout(7)  //��λ����
#define Code_Busy  PBin(6)	 //æµ������� ������Ϊ�͵�ƽ������Զ�����

extern void init_SPI(void);//��ʼ��SPI
void SPI1_Init(void);
void SPI1_SetSpeed(u8 SPI_BaudRatePrescaler);
extern u8 SPI1_ReadWriteByte(u8 TxData);
extern void Rst_Code(void);//��λ����IC
extern unsigned char WriteCode(unsigned char *cData,unsigned short iLong,unsigned char Block);
extern unsigned char ReadCode(unsigned char *cData,unsigned short *iLong,unsigned char Block);
extern unsigned char* EncryptData(unsigned char *cOriginal,unsigned int iOlong,unsigned int *iDlong,unsigned char Block);
extern unsigned char* DecryptData(unsigned char *cOriginal,unsigned int iOlong,unsigned int *iDlong,unsigned char Block);
extern void TestCode(void *p_arg);
extern void GetCodeKeyAck(StrFrameHeader *frame_head,unsigned char* p,unsigned short*len);//������Կ
extern void AlterCodeKey(unsigned char com);//�޸���Կ

#endif


