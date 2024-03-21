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
unsigned char *g_cRemot_addr;//远程IP地址   
 
//TCP客户端任务
#define UDP_PRIO		6
//任务堆栈大小
#define UDP_STK_SIZE	800
//任务堆栈
OS_STK UDP_TASK_STK[UDP_STK_SIZE];


u8 *udp_demo_recvbuf;	//UDP接收数据缓冲区
////UDP发送数据内容
//const u8 *udp_demo_sendbuf="Explorer STM32F407 NETCONN UDP demo send data\r\n";
u8 udp_flag;							//UDP数据发送标志位

//udp任务函数
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
	static u8 remot_addr[4];//保存远端IP地址
	static ip_addr_t ipaddr;
	static u16_t 			port;//远端端口号
	
	
	StrFrameHeader *frame_head,*p=NULL;//用于队列
	INT8U *memp_buf;//报文数据
	unsigned int iTcpTxdLen;//发送报文的长度
	unsigned int iTcpServerLen;//接收报文的长度
	unsigned int i;
	
	LWIP_UNUSED_ARG(arg);
	while(1)
	{
		udpconn = netconn_new(NETCONN_UDP);  //创建一个UDP链接
		udpconn->recv_timeout = 10;
		udp_demo_recvbuf = mymalloc(SRAMIN,UDP_DEMO_RX_BUFSIZE);//给UDP缓存分配内存
		if(udpconn != NULL)  //创建UDP连接成功
		{
			err = netconn_bind(udpconn,IP_ADDR_ANY,lwipdev.port); 
			if(err == ERR_OK)//绑定完成
			{
				while(1)
				{
					if((udp_flag & LWIP_SEND_DATA) == LWIP_SEND_DATA) //===============================有数据要发送
					{
						sentbuf = netbuf_new();
						netbuf_alloc(sentbuf,iTcpTxdLen);
						memcpy(sentbuf->p->payload,(void*)udp_demo_recvbuf,iTcpTxdLen);
						err = netconn_send(udpconn,sentbuf);  	//将netbuf中的数据发送出去
						if(err != ERR_OK)
						{
							printf("发送失败\r\n");
						}
						udp_flag &= ~LWIP_SEND_DATA;	//清除数据发送标志
						netconn_disconnect(udpconn); 	//断开远程主机
						netbuf_delete(sentbuf);      	//删除buf
					}
					netconn_recv(udpconn,&recvbuf); //=================================================接收数据
					if(recvbuf != NULL)          //接收到数据
					{
						ipaddr = recvbuf->addr;//获得远程IP
						port = recvbuf->port;//获得远程端口
						remot_addr[3] = (uint8_t)(ipaddr.addr >> 24); 
						remot_addr[2] = (uint8_t)(ipaddr.addr>> 16);
						remot_addr[1] = (uint8_t)(ipaddr.addr >> 8);
						remot_addr[0] = (uint8_t)(ipaddr.addr);
						g_cRemot_addr = remot_addr;//记录远端IP地址
						
						IP4_ADDR(&destipaddr,remot_addr[0],remot_addr[1],remot_addr[2],remot_addr[3]); //构造目的IP地址
						netconn_connect(udpconn,&destipaddr,port); 	//连接到远端主机
						
						OS_ENTER_CRITICAL(); //关中断
						memp_buf = memp_malloc(MEMP_PBUF_POOL);//申请一块内存pool
						memset(udp_demo_recvbuf,0,UDP_DEMO_RX_BUFSIZE);  //数据接收缓冲区清零
						for(q=recvbuf->p;q!=NULL;q=q->next)  //遍历完整个pbuf链表
						{
							//判断要拷贝到UDP_DEMO_RX_BUFSIZE中的数据是否大于UDP_DEMO_RX_BUFSIZE的剩余空间，如果大于
							//的话就只拷贝UDP_DEMO_RX_BUFSIZE中剩余长度的数据，否则的话就拷贝所有的数据
							if(q->len > (UDP_DEMO_RX_BUFSIZE-data_len)) memcpy(udp_demo_recvbuf+data_len,q->payload,(UDP_DEMO_RX_BUFSIZE-data_len));//拷贝数据
							else memcpy(udp_demo_recvbuf+data_len,q->payload,q->len);
							data_len += q->len;  	
							if(data_len > UDP_DEMO_RX_BUFSIZE) break; //超出TCP客户端接收数组,跳出	
						}
						OS_EXIT_CRITICAL();  //开中断
						iTcpServerLen = data_len;
						data_len=0;  //复制完成后data_len要清零。
						frame_head = (StrFrameHeader *)memp_buf;
						frame_head->type = HEAD_ETH_TYPE;
						frame_head->pdata = memp_buf + sizeof(StrFrameHeader);
						memp_buf =frame_head->pdata;         //定位接收数据指针
						for(i=0;i<iTcpServerLen;i++)//=======转译报文
						{
							*memp_buf++ = udp_demo_recvbuf[i];   
						}
						frame_head->len = memp_buf-frame_head->pdata;//命令长度
						if((frame_head->len!=0) && (frame_head->len<=3000))//接收到的数据长度不可为0
						{
							memset(udp_demo_recvbuf,0,UDP_DEMO_RX_BUFSIZE);  //数据接收缓冲区清零
							netbuf_delete(recvbuf);      //删除buf
							printf("报文长度为%d\r\n",frame_head->len);
							do
							{
							err = OSQPost(En_Dis_CodeQeue,(void *)frame_head);//把拷贝的数据发送给加密解密任务
							}
							while(err != OS_ERR_NONE);

							
/**********************************************************************************************************************/
/*
 *以下为加密任务返回到UDP的数据					 队列不对 需要重新	
 */							
							p = OSQPend(En_Udp_CodeQeue,1500,&error);   //等待加密任务返回
							if(error == OS_ERR_NONE)
							{
								for(i=0;i<p->len;i++)//写入要发送的数据(反馈上位机)
								{
									udp_demo_recvbuf[i] = p->pdata[i];
								}
								iTcpTxdLen = p->len;//写入发送的长度
								udp_flag |= LWIP_SEND_DATA; //标记LWIP有数据要发送
								memp_free(MEMP_PBUF_POOL,(void *)p);	 //释放POOL内存	(用于返回上位机的数据包)
								printf("返回成功\r\n");
								printf("返回的数据为\r\n%s\r\n",udp_demo_recvbuf);
							}
							else
							{
								printf("加密任务无返回\r\n");
								netconn_disconnect(udpconn); 	//断开远程主机
							}
						}
						else
						{
							netbuf_delete(recvbuf);      //删除buf
							netconn_disconnect(udpconn); 	//断开远程主机
						}
					}
					else OSTimeDlyHMSM(0,0,0,5);  //延时5ms
				}
			}
			else printf("UDP绑定失败\r\n");
		}
		else printf("UDP连接创建失败\r\n");
	}
}

//创建UDP线程
//返回值:0 UDP创建成功
//		其他 UDP创建失败
INT8U udp_demo_init(void)
{
	INT8U res;
	OS_CPU_SR cpu_sr;
	
	OS_ENTER_CRITICAL();	//关中断
	res = OSTaskCreate(udp_thread,(void*)0,(OS_STK*)&UDP_TASK_STK[UDP_STK_SIZE-1],UDP_PRIO); //创建UDP线程
	OS_EXIT_CRITICAL();		//开中断
	
	return res;
}

