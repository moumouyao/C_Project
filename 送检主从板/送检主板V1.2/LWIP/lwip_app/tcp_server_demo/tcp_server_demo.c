#include "tcp_server_demo.h"
#include "lwip/opt.h"
#include "lwip_comm.h"
#include "led.h"
#include "lwip/lwip_sys.h"
#include "lwip/api.h"
#include "sw_controlcenter.h"
#include "sw_global.h"
#include "malloc.h"
#include "i2c.h"
#include "timer.h"

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
 

u8 *tcp_server_recvbuf;	//TCP�ͻ��˽������ݻ�����
u8 tcp_server_flag;								//TCP���������ݷ��ͱ�־λ

//TCP�ͻ�������
#define TCPSERVER_PRIO		6
//�����ջ��С
#define TCPSERVER_STK_SIZE	800
//�����ջ
OS_STK TCPSERVER_TASK_STK[TCPSERVER_STK_SIZE];

//tcp����������
static void tcp_server_thread(void *arg)
{
	OS_CPU_SR cpu_sr;
	u32 data_len = 0;
	struct pbuf *q;
	err_t err,recv_err;
	u8 error;
	static u8 remot_addr[4];
	struct netconn *conn, *newconn;
	static ip_addr_t ipaddr;
	static u16_t 			port;
	StrFrameHeader *frame_head,*p=NULL;//���ڶ���
	INT8U *memp_buf;//��������
	unsigned int iTcpServerLen;//���ձ��ĵĳ���
	unsigned int iTcpTxdLen;//���ͱ��ĵĳ���
	unsigned char cEcho=0;//��Ҫ������־λ
	unsigned int i;
	
	LWIP_UNUSED_ARG(arg);//δʹ�õĲ���
	tcp_server_recvbuf = mymalloc(SRAMEX,TCP_SERVER_RX_BUFSIZE);//��TCP��������ڴ�
	conn = netconn_new(NETCONN_TCP);  //����һ��TCP����
	netconn_bind(conn,IP_ADDR_ANY,g_cPort);  //�󶨶˿� 8�Ŷ˿�
	netconn_listen(conn);  		//�������ģʽ
	conn->recv_timeout = 10;  	//��ֹ�����߳� �ȴ�10ms
	while (1) 
	{
		err = netconn_accept(conn,&newconn);  //������������
		if(err==ERR_OK)newconn->recv_timeout = 10;

		if (err == ERR_OK)    //���������ӵ�����
		{
			struct netbuf *recvbuf;

			netconn_getaddr(newconn,&ipaddr,&port,0); //��ȡԶ��IP��ַ�Ͷ˿ں�
		
			remot_addr[3] = (uint8_t)(ipaddr.addr >> 24); 
			remot_addr[2] = (uint8_t)(ipaddr.addr>> 16);
			remot_addr[1] = (uint8_t)(ipaddr.addr >> 8);
			remot_addr[0] = (uint8_t)(ipaddr.addr);
			g_cRemot_addr = remot_addr;//Զ��IP��ַ
#if EnableWhiteList
			g_cTcpClientState = ContrastWhiteList(remot_addr);//�ԱȰ�����IP
#endif
			while(1)
			{
				if((tcp_server_flag & LWIP_SEND_DATA) == LWIP_SEND_DATA) //������Ҫ����
				{
					err = netconn_write(newconn ,tcp_server_recvbuf,iTcpTxdLen,NETCONN_COPY); //����tcp_server_sendbuf�е�����
					if(err != ERR_OK)
					{
					}
					tcp_server_flag &= ~LWIP_SEND_DATA;
				}
				if((recv_err = netconn_recv(newconn,&recvbuf)) == ERR_OK)  	//���յ�����
				{

					OS_ENTER_CRITICAL(); //���ж�
					memp_buf = memp_malloc(MEMP_PBUF_POOL);//����һ���ڴ�pool
					for(q=recvbuf->p;q!=NULL;q=q->next)  //����������pbuf���� �������յ�������
					{
						//�ж�Ҫ������TCP_SERVER_RX_BUFSIZE�е������Ƿ����TCP_SERVER_RX_BUFSIZE��ʣ��ռ䣬�������
						//�Ļ���ֻ����TCP_SERVER_RX_BUFSIZE��ʣ�೤�ȵ����ݣ�����Ļ��Ϳ������е�����
						if(q->len > (TCP_SERVER_RX_BUFSIZE-data_len)) 
						{
							memcpy(tcp_server_recvbuf+data_len,q->payload,(TCP_SERVER_RX_BUFSIZE-data_len));//��������
						}
						else
						{
							memcpy(tcp_server_recvbuf+data_len,q->payload,q->len);
						}
						data_len += q->len;
						if(data_len > TCP_SERVER_RX_BUFSIZE)
							break; //����TCP�ͻ��˽�������,����
					}
					netbuf_delete(recvbuf);
					OS_EXIT_CRITICAL();  //���ж�
					iTcpServerLen = data_len;
					data_len=0;  //������ɺ�data_lenҪ���㡣	
					//=================�������ݰ���׼���������ƴ�������==================//
				  frame_head = (StrFrameHeader *)memp_buf;
          frame_head->type = HEAD_ETH_TYPE;
          frame_head->pdata = memp_buf + sizeof(StrFrameHeader);
					memp_buf =frame_head->pdata;         //��λ��������ָ��

					memcpy(frame_head->pdata,tcp_server_recvbuf,iTcpServerLen);
					frame_head->len=iTcpServerLen;					

					if(frame_head->len != 0)//���յ������ݳ��Ȳ���Ϊ0
					{
						memset(tcp_server_recvbuf,0,TCP_SERVER_RX_BUFSIZE);  //���ݽ��ջ���������
						do
						{
#if EnableCode
							err = OSQPost(En_Dis_CodeQeue,(void *)frame_head);//�ѿ��������ݷ��͸����ܽ�������
#else
							err = OSQPost(CtrlCentRecMsgQeue,(void *)frame_head);//�ѿ��������ݷ��͸����ƴ�������
#endif
						}
						while(err != OS_ERR_NONE);
						cEcho = 1;//�ȴ�����
					}
				}
				else if(cEcho)//�Ƿ���Ҫ����
				{
					cEcho = 0;//�������
#if EnableCode
					p = OSQPend(Dis_En_CodeQeue,1000,&error);   //�ȴ��������񷵻�
#else
					p = OSQPend(UdpTranMsgQeue,1000,&error);   //�ȴ����ƴ������񷵻ص�����
#endif
					if(error == OS_ERR_NONE)
					{
						p->len = p->len>1024?1024:p->len;
						for(i=0;i<p->len;i++)//д��Ҫ���͵�����(������λ��)
						{
							tcp_server_recvbuf[i] = p->pdata[i];
						}
						iTcpTxdLen = p->len;//д�뷢�͵ĳ���
						tcp_server_flag |= LWIP_SEND_DATA; //���LWIP������Ҫ����
						memp_free(MEMP_PBUF_POOL,(void *)p);	 //�ͷ�POOL�ڴ�	(���ڷ�����λ�������ݰ�)

						OSQFlush(UdpTranMsgQeue);
					}
					else
					{

					}
				}
				else if(recv_err == ERR_CLSD)  //�ر�����
				{
					netconn_close(newconn);
					netconn_delete(newconn);
					break;
				}				
			OSTimeDlyHMSM(0,0,0,5);  //��ʱ5ms
			}
		}
		OSTimeDlyHMSM(0,0,0,5);  //��ʱ5ms
	}
}
void init_WhiteList()
{
	g_cIpWhiteList[0] = mymalloc(SRAMEX,4);//����һ�������������ڴ�
	g_cIpWhiteList[1] = mymalloc(SRAMEX,4);//����һ�������������ڴ�
	g_cIpWhiteList[2] = mymalloc(SRAMEX,4);//����һ�������������ڴ�
	g_cIpWhiteList[3] = mymalloc(SRAMEX,4);//����һ�������������ڴ�
	g_cIpWhiteList[4] = mymalloc(SRAMEX,4);//����һ�������������ڴ�
	//===============��ʼ��������===============//
	g_cIpWhiteList[0][0] = E2promReadByte(EEROM_White_1);//��ȡIP;
	g_cIpWhiteList[0][1] = E2promReadByte(EEROM_White_1+1);//��ȡIP;;
	g_cIpWhiteList[0][2] = E2promReadByte(EEROM_White_1+2);//��ȡIP;;
	g_cIpWhiteList[0][3] = E2promReadByte(EEROM_White_1+3);//��ȡIP;;
	
	g_cIpWhiteList[1][0] = E2promReadByte(EEROM_White_2);//��ȡIP;;
	g_cIpWhiteList[1][1] = E2promReadByte(EEROM_White_2+1);//��ȡIP;;
	g_cIpWhiteList[1][2] = E2promReadByte(EEROM_White_2+2);//��ȡIP;;
	g_cIpWhiteList[1][3] = E2promReadByte(EEROM_White_2+3);//��ȡIP;;
	
	g_cIpWhiteList[2][0] = E2promReadByte(EEROM_White_3);//��ȡIP;;
	g_cIpWhiteList[2][1] = E2promReadByte(EEROM_White_3+1);//��ȡIP;;
	g_cIpWhiteList[2][2] = E2promReadByte(EEROM_White_3+2);//��ȡIP;;
	g_cIpWhiteList[2][3] = E2promReadByte(EEROM_White_3+3);//��ȡIP;;
	
	g_cIpWhiteList[3][0] = E2promReadByte(EEROM_White_4);//��ȡIP;;
	g_cIpWhiteList[3][1] = E2promReadByte(EEROM_White_4+1);//��ȡIP;;
	g_cIpWhiteList[3][2] = E2promReadByte(EEROM_White_4+2);//��ȡIP;;
	g_cIpWhiteList[3][3] = E2promReadByte(EEROM_White_4+3);//��ȡIP;;
	
	g_cIpWhiteList[4][0] = E2promReadByte(EEROM_White_5);//��ȡIP;;
	g_cIpWhiteList[4][1] = E2promReadByte(EEROM_White_5+1);//��ȡIP;;
	g_cIpWhiteList[4][2] = E2promReadByte(EEROM_White_5+2);//��ȡIP;;
	g_cIpWhiteList[4][3] = E2promReadByte(EEROM_White_5+3);//��ȡIP;;
}
/*
�ԱȰ�����(�ͻ������Ӻ����)
����ֵ��1=�Ŀͻ���Ϊ������
0=�ÿͻ��˲��ǰ�����
*/
unsigned char ContrastWhiteList(unsigned char *cOriginal)
{
	unsigned char i,j;
	unsigned char cReturn = 0;
	for(i=0;i<WhiteListNum;i++)
	{
		for(j=0;j<4;j++)
		{
			if(cOriginal[j] != g_cIpWhiteList[i][j])//�Ա�IP
			{
				break;
			}
		}
		cReturn = j==4?1:cReturn;//�Ƿ�Աȳɹ�
	}
	return cReturn;//����  ��ʱ�رհ���������
}

//����TCP�������߳�
//����ֵ:0 TCP�����������ɹ�
//		���� TCP����������ʧ��
INT8U tcp_server_init(void)
{
	INT8U res;
	OS_CPU_SR cpu_sr;
	
	OS_ENTER_CRITICAL();	//���ж�
	init_WhiteList();//��ʼ��������
	res = OSTaskCreate(tcp_server_thread,(void*)0,(OS_STK*)&TCPSERVER_TASK_STK[TCPSERVER_STK_SIZE-1],TCPSERVER_PRIO); //����TCP�������߳�
	OS_EXIT_CRITICAL();		//���ж�
	
	return res;
}


