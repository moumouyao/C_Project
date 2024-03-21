#include "udp_demo.h"
#include "lwip_comm.h"
#include "usart.h"
#include "led.h"
#include "includes.h"
#include "lwip/api.h"
#include "lwip/lwip_sys.h"
#include "string.h"   
#include "sw_controlcenter.h"
#include "sw_global.h"
#include "malloc.h"
#include "memp.h"
#include "tcp_server_demo.h"
#include "app.h"
extern unsigned char *g_cRemot_addr;//Զ��IP��ַ
extern uint8_t hear_beat_flag;//������־λ
#define UDP_PRIO		6
//�����ջ��С
#define UDP_STK_SIZE	1024
//�����ջ
OS_STK UDP_TASK_STK[UDP_STK_SIZE];
u8 *udp_demo_recvbuf;	//UDP�������ݻ�����
//UDP������������
u8 udp_flag;							//UDP���ݷ��ͱ�־λ

//udp������
extern uint16_t read_over_flag;
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
	
	
	StrFrameHeader *frame_head,*p=NULL;//, *p2 = NULL;//���ڶ���
	INT8U *memp_buf;//��������
	unsigned int iTcpTxdLen;//���ͱ��ĵĳ���
	unsigned int iTcpServerLen;//���ձ��ĵĳ���
	unsigned int i;
	
	
	static StrFrameHeader *ret;
	static INT8U * pot=NULL;
	pot = memp_malloc(MEMP_PBUF_POOL);
		if(pot == NULL)
		{
//			memp_free(MEMP_PBUF_POOL,(void *)frame_head);//�ͷ�POOL�ڴ�	
			NVIC_SystemReset();
		}
		ret = (StrFrameHeader *)pot;
		ret->type = 1;//ETH_TYPE;
		ret->pdata = pot + sizeof(StrFrameHeader);
		cJSON *cjson_test;
		cjson_test=cJSON_CreateObject();
		cJSON_AddStringToObject(cjson_test,"id","heart");//����ַ����͵�
		cJSON_AddStringToObject(cjson_test,"msgType","messageBeat");
		char *cjson_data=cJSON_PrintUnformatted(cjson_test);
		memcpy(ret->pdata,cjson_data,strlen(cjson_data));
		ret->len=strlen(cjson_data);
		cJSON_Delete(cjson_test);
		ret->pdata = pot + sizeof(StrFrameHeader);
	
	
	
	LWIP_UNUSED_ARG(arg);
	while(1)
	{
		udpconn = netconn_new(NETCONN_UDP);  //����һ��UDP����
		udpconn->recv_timeout = 10;
		udp_demo_recvbuf = mymalloc(SRAMEX,UDP_DEMO_RX_BUFSIZE);//��UDP��������ڴ�
		if(udpconn != NULL)  //����UDP���ӳɹ�
		{	
			err = netconn_bind(udpconn,IP_ADDR_ANY,g_cPort);
			if(err == ERR_OK)//�����
			{
				while(1)
				{
					//������������Ϣ����
//					p2 = OSQPend(UdpHearbeatQeue,1,&error);
//						p2 = OSQPend(UdpTranMsgQeue,1,&error);
//					if(error == OS_ERR_NONE)
//							{
//								IP4_ADDR(&destipaddr,g_cRemot_addr[0],g_cRemot_addr[1],g_cRemot_addr[2],g_cRemot_addr[3]);
//								netconn_connect(udpconn,&destipaddr,port);
//								for(i=0;i<p2->len;i++)//д��Ҫ���͵�����(������λ��)
//								{
//									udp_demo_recvbuf[i] = p2->pdata[i];
//								}
//								iTcpTxdLen = p2->len;//д�뷢�͵ĳ���
//								udp_flag |= LWIP_SEND_DATA; //���LWIP������Ҫ����
//								memp_free(MEMP_PBUF_POOL,(void *)p2);	 //�ͷ�POOL�ڴ�	(���ڷ�����λ�������ݰ�)
//								p2 = NULL;
//							}
//							else{
//								p2 = NULL;
//							}
	if(hear_beat_flag == 1){
		hear_beat_flag = 0;
								IP4_ADDR(&destipaddr,g_cRemot_addr[0],g_cRemot_addr[1],g_cRemot_addr[2],g_cRemot_addr[3]);
								netconn_connect(udpconn,&destipaddr,port);
								for(i=0;i<ret->len;i++)//д��Ҫ���͵�����(������λ��)
								{
									udp_demo_recvbuf[i] = ret->pdata[i];
								}
								iTcpTxdLen = ret->len;//д�뷢�͵ĳ���
								udp_flag |= LWIP_SEND_DATA; //���LWIP������Ҫ����
	}
					
					
					//��������
					if((udp_flag & LWIP_SEND_DATA) == LWIP_SEND_DATA) //===============================������Ҫ����
					{
						sentbuf = netbuf_new();
						netbuf_alloc(sentbuf,iTcpTxdLen);
						memcpy(sentbuf->p->payload,(void*)udp_demo_recvbuf,iTcpTxdLen);
						err = netconn_send(udpconn,sentbuf);  	//��netbuf�е����ݷ��ͳ�ȥ
						if(err != ERR_OK)
						{
							printf("UdP����ʧ��\r\n");
						}
						udp_flag &= ~LWIP_SEND_DATA;	//������ݷ��ͱ�־
							netconn_disconnect(udpconn); 	//�Ͽ�Զ������
						netbuf_delete(sentbuf);      	//ɾ��buf
					}
					netconn_recv(udpconn,&recvbuf); //=================================================��������
					if(recvbuf != NULL || read_over_flag > 0)          //���յ�����
					{
						if(read_over_flag == 0 && recvbuf != NULL){	//���յ�����Ϣ
						ipaddr = recvbuf->addr;//���Զ��IP
						port = recvbuf->port;//���Զ�̶˿�
						remot_addr[3] = (uint8_t)(ipaddr.addr >> 24); 
						remot_addr[2] = (uint8_t)(ipaddr.addr>> 16);
						remot_addr[1] = (uint8_t)(ipaddr.addr >> 8);
						remot_addr[0] = (uint8_t)(ipaddr.addr);
						g_cRemot_addr = remot_addr;//��¼Զ��IP��ַ
#if EnableWhiteList
						g_cTcpClientState = ContrastWhiteList(remot_addr);//�ԱȰ�����IP
#endif
						IP4_ADDR(&destipaddr,remot_addr[0],remot_addr[1],remot_addr[2],remot_addr[3]); //����Ŀ��IP��ַ
						
						//�����Ϣ����
						while(OSQPend(UdpTranMsgQeue,10,&error) != NULL);
						}
						
						netconn_connect(udpconn,&destipaddr,port); 	//���ӵ�Զ������
						if(read_over_flag > 0 &&  recvbuf == NULL){
goto label;
						}
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
						
						memset(memp_buf,0,UDP_DEMO_RX_BUFSIZE);  //���ݽ��ջ���������
						memcpy(memp_buf,udp_demo_recvbuf,iTcpServerLen);
						frame_head->len=iTcpServerLen;	
						
						if((frame_head->len!=0) && (frame_head->len<=3000))//���յ������ݳ��Ȳ���Ϊ0
						{
							memset(udp_demo_recvbuf,0,UDP_DEMO_RX_BUFSIZE);  //���ݽ��ջ���������
							netbuf_delete(recvbuf);      //ɾ��buf
							do
							{
#if EnableCode
							err = OSQPost(En_Dis_CodeQeue,(void *)frame_head);//�ѿ��������ݷ��͸����ܽ�������
#else
							err = OSQPost(CtrlCentRecMsgQeue,(void *)frame_head);//�ѿ��������ݷ��͸����ƴ�������
#endif
							}
							while(err != OS_ERR_NONE);
label:
#if EnableCode
							p = OSQPend(Dis_En_CodeQeue,1000,&error);   //�ȴ��������񷵻�
#else
							p = OSQPend(UdpTranMsgQeue,1000,&error);   //�ȴ����ƴ������񷵻ص�����
#endif
							if(error == OS_ERR_NONE)
							{
								for(i=0;i<p->len;i++)//д��Ҫ���͵�����(������λ��)
								{
									udp_demo_recvbuf[i] = p->pdata[i];
								}
								iTcpTxdLen = p->len;//д�뷢�͵ĳ���
								udp_flag |= LWIP_SEND_DATA; //���LWIP������Ҫ����
								memp_free(MEMP_PBUF_POOL,(void *)p);	 //�ͷ�POOL�ڴ�	(���ڷ�����λ�������ݰ�)
								
							}
							else
							{
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
			else 
			{
			
			}

		}
		else 
		{
		
		}

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
	init_WhiteList();//��ʼ��������
	res = OSTaskCreate(udp_thread,(void*)0,(OS_STK*)&UDP_TASK_STK[UDP_STK_SIZE-1],UDP_PRIO); //����UDP�߳�
	OS_EXIT_CRITICAL();		//���ж�
	
	return res;
}

