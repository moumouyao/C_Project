#ifndef __TCP_SERVER_DEMO_H
#define __TCP_SERVER_DEMO_H
#include "sys.h"
#include "includes.h"
#include "lwip/tcp.h"
#include "lwip/memp.h"
#include "lwip/mem.h"
#include "lwip_comm.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32F407������
//NETCONN API��̷�ʽ��TCP���������Դ���	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2014/8/15
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved									  
//*******************************************************************************
//�޸���Ϣ
//��
////////////////////////////////////////////////////////////////////////////////// 	   

 
#define TCP_SERVER_RX_BUFSIZE	2048		//����tcp server���������ݳ���
#define TCP_SERVER_PORT			8088	//����tcp server�Ķ˿�
#define LWIP_SEND_DATA			0X80	//���������ݷ���
extern unsigned char *tcp_server_recvbuf;	//TCP�ͻ��˽������ݻ�����
extern unsigned char tcp_server_flag;			//TCP���������ݷ��ͱ�־λ

void init_WhiteList(void);//��ʼ��������
unsigned char ContrastWhiteList(unsigned char *cOriginal);//�ԱȰ�����
INT8U tcp_server_init(void);		//TCP��������ʼ��(����TCP�������߳�)
#endif

