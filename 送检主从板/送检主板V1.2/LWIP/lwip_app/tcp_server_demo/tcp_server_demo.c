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
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK STM32F407开发板
//NETCONN API编程方式的TCP服务器测试代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//创建日期:2014/8/15
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved									  
//*******************************************************************************
//修改信息
//无
////////////////////////////////////////////////////////////////////////////////// 	   
 

u8 *tcp_server_recvbuf;	//TCP客户端接收数据缓冲区
u8 tcp_server_flag;								//TCP服务器数据发送标志位

//TCP客户端任务
#define TCPSERVER_PRIO		6
//任务堆栈大小
#define TCPSERVER_STK_SIZE	800
//任务堆栈
OS_STK TCPSERVER_TASK_STK[TCPSERVER_STK_SIZE];

//tcp服务器任务
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
	StrFrameHeader *frame_head,*p=NULL;//用于队列
	INT8U *memp_buf;//报文数据
	unsigned int iTcpServerLen;//接收报文的长度
	unsigned int iTcpTxdLen;//发送报文的长度
	unsigned char cEcho=0;//需要反馈标志位
	unsigned int i;
	
	LWIP_UNUSED_ARG(arg);//未使用的参数
	tcp_server_recvbuf = mymalloc(SRAMEX,TCP_SERVER_RX_BUFSIZE);//给TCP缓存分配内存
	conn = netconn_new(NETCONN_TCP);  //创建一个TCP链接
	netconn_bind(conn,IP_ADDR_ANY,g_cPort);  //绑定端口 8号端口
	netconn_listen(conn);  		//进入监听模式
	conn->recv_timeout = 10;  	//禁止阻塞线程 等待10ms
	while (1) 
	{
		err = netconn_accept(conn,&newconn);  //接收连接请求
		if(err==ERR_OK)newconn->recv_timeout = 10;

		if (err == ERR_OK)    //处理新连接的数据
		{
			struct netbuf *recvbuf;

			netconn_getaddr(newconn,&ipaddr,&port,0); //获取远端IP地址和端口号
		
			remot_addr[3] = (uint8_t)(ipaddr.addr >> 24); 
			remot_addr[2] = (uint8_t)(ipaddr.addr>> 16);
			remot_addr[1] = (uint8_t)(ipaddr.addr >> 8);
			remot_addr[0] = (uint8_t)(ipaddr.addr);
			g_cRemot_addr = remot_addr;//远程IP地址
#if EnableWhiteList
			g_cTcpClientState = ContrastWhiteList(remot_addr);//对比白名单IP
#endif
			while(1)
			{
				if((tcp_server_flag & LWIP_SEND_DATA) == LWIP_SEND_DATA) //有数据要发送
				{
					err = netconn_write(newconn ,tcp_server_recvbuf,iTcpTxdLen,NETCONN_COPY); //发送tcp_server_sendbuf中的数据
					if(err != ERR_OK)
					{
					}
					tcp_server_flag &= ~LWIP_SEND_DATA;
				}
				if((recv_err = netconn_recv(newconn,&recvbuf)) == ERR_OK)  	//接收到数据
				{

					OS_ENTER_CRITICAL(); //关中断
					memp_buf = memp_malloc(MEMP_PBUF_POOL);//申请一块内存pool
					for(q=recvbuf->p;q!=NULL;q=q->next)  //遍历完整个pbuf链表 拷贝接收到的数据
					{
						//判断要拷贝到TCP_SERVER_RX_BUFSIZE中的数据是否大于TCP_SERVER_RX_BUFSIZE的剩余空间，如果大于
						//的话就只拷贝TCP_SERVER_RX_BUFSIZE中剩余长度的数据，否则的话就拷贝所有的数据
						if(q->len > (TCP_SERVER_RX_BUFSIZE-data_len)) 
						{
							memcpy(tcp_server_recvbuf+data_len,q->payload,(TCP_SERVER_RX_BUFSIZE-data_len));//拷贝数据
						}
						else
						{
							memcpy(tcp_server_recvbuf+data_len,q->payload,q->len);
						}
						data_len += q->len;
						if(data_len > TCP_SERVER_RX_BUFSIZE)
							break; //超出TCP客户端接收数组,跳出
					}
					netbuf_delete(recvbuf);
					OS_EXIT_CRITICAL();  //开中断
					iTcpServerLen = data_len;
					data_len=0;  //复制完成后data_len要清零。	
					//=================构建数据包，准备发往控制处理任务==================//
				  frame_head = (StrFrameHeader *)memp_buf;
          frame_head->type = HEAD_ETH_TYPE;
          frame_head->pdata = memp_buf + sizeof(StrFrameHeader);
					memp_buf =frame_head->pdata;         //定位接收数据指针

					memcpy(frame_head->pdata,tcp_server_recvbuf,iTcpServerLen);
					frame_head->len=iTcpServerLen;					

					if(frame_head->len != 0)//接收到的数据长度不可为0
					{
						memset(tcp_server_recvbuf,0,TCP_SERVER_RX_BUFSIZE);  //数据接收缓冲区清零
						do
						{
#if EnableCode
							err = OSQPost(En_Dis_CodeQeue,(void *)frame_head);//把拷贝的数据发送给加密解密任务
#else
							err = OSQPost(CtrlCentRecMsgQeue,(void *)frame_head);//把拷贝的数据发送给控制处理任务
#endif
						}
						while(err != OS_ERR_NONE);
						cEcho = 1;//等待反馈
					}
				}
				else if(cEcho)//是否需要反馈
				{
					cEcho = 0;//反馈完毕
#if EnableCode
					p = OSQPend(Dis_En_CodeQeue,1000,&error);   //等待加密任务返回
#else
					p = OSQPend(UdpTranMsgQeue,1000,&error);   //等待控制处理任务返回的数据
#endif
					if(error == OS_ERR_NONE)
					{
						p->len = p->len>1024?1024:p->len;
						for(i=0;i<p->len;i++)//写入要发送的数据(反馈上位机)
						{
							tcp_server_recvbuf[i] = p->pdata[i];
						}
						iTcpTxdLen = p->len;//写入发送的长度
						tcp_server_flag |= LWIP_SEND_DATA; //标记LWIP有数据要发送
						memp_free(MEMP_PBUF_POOL,(void *)p);	 //释放POOL内存	(用于返回上位机的数据包)

						OSQFlush(UdpTranMsgQeue);
					}
					else
					{

					}
				}
				else if(recv_err == ERR_CLSD)  //关闭连接
				{
					netconn_close(newconn);
					netconn_delete(newconn);
					break;
				}				
			OSTimeDlyHMSM(0,0,0,5);  //延时5ms
			}
		}
		OSTimeDlyHMSM(0,0,0,5);  //延时5ms
	}
}
void init_WhiteList()
{
	g_cIpWhiteList[0] = mymalloc(SRAMEX,4);//给第一个白名单申请内存
	g_cIpWhiteList[1] = mymalloc(SRAMEX,4);//给第一个白名单申请内存
	g_cIpWhiteList[2] = mymalloc(SRAMEX,4);//给第一个白名单申请内存
	g_cIpWhiteList[3] = mymalloc(SRAMEX,4);//给第一个白名单申请内存
	g_cIpWhiteList[4] = mymalloc(SRAMEX,4);//给第一个白名单申请内存
	//===============初始化白名单===============//
	g_cIpWhiteList[0][0] = E2promReadByte(EEROM_White_1);//读取IP;
	g_cIpWhiteList[0][1] = E2promReadByte(EEROM_White_1+1);//读取IP;;
	g_cIpWhiteList[0][2] = E2promReadByte(EEROM_White_1+2);//读取IP;;
	g_cIpWhiteList[0][3] = E2promReadByte(EEROM_White_1+3);//读取IP;;
	
	g_cIpWhiteList[1][0] = E2promReadByte(EEROM_White_2);//读取IP;;
	g_cIpWhiteList[1][1] = E2promReadByte(EEROM_White_2+1);//读取IP;;
	g_cIpWhiteList[1][2] = E2promReadByte(EEROM_White_2+2);//读取IP;;
	g_cIpWhiteList[1][3] = E2promReadByte(EEROM_White_2+3);//读取IP;;
	
	g_cIpWhiteList[2][0] = E2promReadByte(EEROM_White_3);//读取IP;;
	g_cIpWhiteList[2][1] = E2promReadByte(EEROM_White_3+1);//读取IP;;
	g_cIpWhiteList[2][2] = E2promReadByte(EEROM_White_3+2);//读取IP;;
	g_cIpWhiteList[2][3] = E2promReadByte(EEROM_White_3+3);//读取IP;;
	
	g_cIpWhiteList[3][0] = E2promReadByte(EEROM_White_4);//读取IP;;
	g_cIpWhiteList[3][1] = E2promReadByte(EEROM_White_4+1);//读取IP;;
	g_cIpWhiteList[3][2] = E2promReadByte(EEROM_White_4+2);//读取IP;;
	g_cIpWhiteList[3][3] = E2promReadByte(EEROM_White_4+3);//读取IP;;
	
	g_cIpWhiteList[4][0] = E2promReadByte(EEROM_White_5);//读取IP;;
	g_cIpWhiteList[4][1] = E2promReadByte(EEROM_White_5+1);//读取IP;;
	g_cIpWhiteList[4][2] = E2promReadByte(EEROM_White_5+2);//读取IP;;
	g_cIpWhiteList[4][3] = E2promReadByte(EEROM_White_5+3);//读取IP;;
}
/*
对比白名单(客户端链接后调用)
返回值：1=改客户端为白名单
0=该客户端不是白名单
*/
unsigned char ContrastWhiteList(unsigned char *cOriginal)
{
	unsigned char i,j;
	unsigned char cReturn = 0;
	for(i=0;i<WhiteListNum;i++)
	{
		for(j=0;j<4;j++)
		{
			if(cOriginal[j] != g_cIpWhiteList[i][j])//对比IP
			{
				break;
			}
		}
		cReturn = j==4?1:cReturn;//是否对比成功
	}
	return cReturn;//返回  暂时关闭白名单功能
}

//创建TCP服务器线程
//返回值:0 TCP服务器创建成功
//		其他 TCP服务器创建失败
INT8U tcp_server_init(void)
{
	INT8U res;
	OS_CPU_SR cpu_sr;
	
	OS_ENTER_CRITICAL();	//关中断
	init_WhiteList();//初始化白名单
	res = OSTaskCreate(tcp_server_thread,(void*)0,(OS_STK*)&TCPSERVER_TASK_STK[TCPSERVER_STK_SIZE-1],TCPSERVER_PRIO); //创建TCP服务器线程
	OS_EXIT_CRITICAL();		//开中断
	
	return res;
}


