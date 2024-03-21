#include "udp_demo.h"
#include "lwip_comm.h"
#include "usart.h"
#include "led.h"
#include "includes.h"
#include "lwip/api.h"
#include "lwip/lwip_sys.h"
#include "string.h"
#include "sw_conf.h"
#include "malloc.h"
#include "memp.h"
#include "sw_global.h"

extern __lwip_dev lwipdev;
unsigned char *g_cRemot_addr;//Զ��IP��ַ   
 
//TCP�ͻ�������
#define UDP_PRIO		6
//�����ջ��С
#define UDP_STK_SIZE	800
//�����ջ
OS_STK UDP_TASK_STK[UDP_STK_SIZE];


u8 *udp_demo_recvbuf;	//UDP�������ݻ�����
////UDP������������
//const u8 *udp_demo_sendbuf="Explorer STM32F407 NETCONN UDP demo send data\r\n";
u8 udp_flag;							//UDP���ݷ��ͱ�־λ

//udp������
static void udp_thread(void *arg)
{
	OS_CPU_SR cpu_sr;
	err_t err;
	u8 error;
	static struct netconn *udpconn;
	static struct netbuf  *recvbuf;
	static struct netbuf  *sentbuf;
	struct ip_addr destipaddr;
	u32 data_len = 0;
	struct pbuf *q;
	static u8 remot_addr[4];//����Զ��IP��ַ
	static ip_addr_t ipaddr;
	static u16_t 			port;//Զ�˶˿ں�
	
	
	StrFrameHeader *frame_head,*p=NULL;//���ڶ���
	INT8U *memp_buf;//��������
	unsigned int iTcpTxdLen;//���ͱ��ĵĳ���
	unsigned int iTcpServerLen;//���ձ��ĵĳ���
	unsigned int i;
	
	LWIP_UNUSED_ARG(arg);
	while(1)
	{
		udpconn = netconn_new(NETCONN_UDP);  //����һ��UDP����
		udpconn->recv_timeout = 10;
		udp_demo_recvbuf = mymalloc(SRAMIN,UDP_DEMO_RX_BUFSIZE);//��UDP��������ڴ�
		if(udpconn != NULL)  //����UDP���ӳɹ�
		{
			err = netconn_bind(udpconn,IP_ADDR_ANY,lwipdev.port); 
			if(err == ERR_OK)//�����
			{
				while(1)
				{
					if((udp_flag & LWIP_SEND_DATA) == LWIP_SEND_DATA) //===============================������Ҫ����
					{
						sentbuf = netbuf_new();
						netbuf_alloc(sentbuf,iTcpTxdLen);
						memcpy(sentbuf->p->payload,(void*)udp_demo_recvbuf,iTcpTxdLen);
						err = netconn_send(udpconn,sentbuf);  	//��netbuf�е����ݷ��ͳ�ȥ
						if(err != ERR_OK)
						{
							printf("����ʧ��\r\n");
						}
						udp_flag &= ~LWIP_SEND_DATA;	//������ݷ��ͱ�־
						netconn_disconnect(udpconn); 	//�Ͽ�Զ������
						netbuf_delete(sentbuf);      	//ɾ��buf
					}
					netconn_recv(udpconn,&recvbuf); //=================================================��������
					if(recvbuf != NULL)          //���յ�����
					{
						ipaddr = recvbuf->addr;//���Զ��IP
						port = recvbuf->port;//���Զ�̶˿�
						remot_addr[3] = (uint8_t)(ipaddr.addr >> 24); 
						remot_addr[2] = (uint8_t)(ipaddr.addr>> 16);
						remot_addr[1] = (uint8_t)(ipaddr.addr >> 8);
						remot_addr[0] = (uint8_t)(ipaddr.addr);
						g_cRemot_addr = remot_addr;//��¼Զ��IP��ַ
						
						IP4_ADDR(&destipaddr,remot_addr[0],remot_addr[1],remot_addr[2],remot_addr[3]); //����Ŀ��IP��ַ
						netconn_connect(udpconn,&destipaddr,port); 	//���ӵ�Զ������
						
						OS_ENTER_CRITICAL(); //���ж�
						memp_buf = memp_malloc(MEMP_PBUF_POOL);//����һ���ڴ�pool
						memset(udp_demo_recvbuf,0,UDP_DEMO_RX_BUFSIZE);  //���ݽ��ջ���������
						for(q=recvbuf->p;q!=NULL;q=q->next)  //����������pbuf����
						{
							//�ж�Ҫ������UDP_DEMO_RX_BUFSIZE�е������Ƿ����UDP_DEMO_RX_BUFSIZE��ʣ��ռ䣬�������
							//�Ļ���ֻ����UDP_DEMO_RX_BUFSIZE��ʣ�೤�ȵ����ݣ�����Ļ��Ϳ������е�����
							if(q->len > (UDP_DEMO_RX_BUFSIZE-data_len)) memcpy(udp_demo_recvbuf+data_len,q->payload,(UDP_DEMO_RX_BUFSIZE-data_len));//��������
							else memcpy(udp_demo_recvbuf+data_len,q->payload,q->len);
							data_len += q->len;  	
							if(data_len > UDP_DEMO_RX_BUFSIZE) break; //����TCP�ͻ��˽�������,����	
						}
						OS_EXIT_CRITICAL();  //���ж�
						iTcpServerLen = data_len;
						data_len=0;  //������ɺ�data_lenҪ���㡣
						frame_head = (StrFrameHeader *)memp_buf;
						frame_head->type = HEAD_ETH_TYPE;
						frame_head->pdata = memp_buf + sizeof(StrFrameHeader);
						memp_buf =frame_head->pdata;         //��λ��������ָ��
						for(i=0;i<iTcpServerLen;i++)//=======ת�뱨��
						{
							*memp_buf++ = udp_demo_recvbuf[i];   
						}
						frame_head->len = memp_buf-frame_head->pdata;//�����
						if((frame_head->len!=0) && (frame_head->len<=3000))//���յ������ݳ��Ȳ���Ϊ0
						{
							memset(udp_demo_recvbuf,0,UDP_DEMO_RX_BUFSIZE);  //���ݽ��ջ���������
							netbuf_delete(recvbuf);      //ɾ��buf
							printf("���ĳ���Ϊ%d\r\n",frame_head->len);
							do
							{
							err = OSQPost(En_Dis_CodeQeue,(void *)frame_head);//�ѿ��������ݷ��͸����ܽ�������
							}
							while(err != OS_ERR_NONE);

							
/**********************************************************************************************************************/
/*
 *����Ϊ�������񷵻ص�UDP������					 ���в��� ��Ҫ����	
 */							
							p = OSQPend(En_Udp_CodeQeue,1500,&error);   //�ȴ��������񷵻�
							if(error == OS_ERR_NONE)
							{
								for(i=0;i<p->len;i++)//д��Ҫ���͵�����(������λ��)
								{
									udp_demo_recvbuf[i] = p->pdata[i];
								}
								iTcpTxdLen = p->len;//д�뷢�͵ĳ���
								udp_flag |= LWIP_SEND_DATA; //���LWIP������Ҫ����
								memp_free(MEMP_PBUF_POOL,(void *)p);	 //�ͷ�POOL�ڴ�	(���ڷ�����λ�������ݰ�)
								printf("���سɹ�\r\n");
								printf("���ص�����Ϊ\r\n%s\r\n",udp_demo_recvbuf);
							}
							else
							{
								printf("���������޷���\r\n");
								netconn_disconnect(udpconn); 	//�Ͽ�Զ������
							}
						}
						else
						{
							netbuf_delete(recvbuf);      //ɾ��buf
							netconn_disconnect(udpconn); 	//�Ͽ�Զ������
						}
					}
					else OSTimeDlyHMSM(0,0,0,5);  //��ʱ5ms
				}
			}
			else printf("UDP��ʧ��\r\n");
		}
		else printf("UDP���Ӵ���ʧ��\r\n");
	}
}

//����UDP�߳�
//����ֵ:0 UDP�����ɹ�
//		���� UDP����ʧ��
INT8U udp_demo_init(void)
{
	INT8U res;
	OS_CPU_SR cpu_sr;
	
	OS_ENTER_CRITICAL();	//���ж�
	res = OSTaskCreate(udp_thread,(void*)0,(OS_STK*)&UDP_TASK_STK[UDP_STK_SIZE-1],UDP_PRIO); //����UDP�߳�
	OS_EXIT_CRITICAL();		//���ж�
	
	return res;
}

