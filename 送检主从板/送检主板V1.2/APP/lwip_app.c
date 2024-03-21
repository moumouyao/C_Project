#include "lwip_app.h"
#include "lwip_comm.h" 
#include "tcp_server_demo.h"
#include "delay.h"
#include "usart.h"
#include "ucos_ii.h"
#include "err.h"
#include "sw_controlcenter.h"
#include "sw_global.h"
#include "lwip/opt.h"
#include "udp_demo.h" 
#include "httpd.h"
#include "app.h"


void lwip_task(void *pdata)
{
	err_t err_tcp,err_udp;
	uint32_t i = 0;
  INT8U *memp_buf;//,*data,*backup;
  StrFrameHeader *frame_head,*p = NULL;
	struct tcp_pcb *tcppcbnew;  	//定义一个TCP服务器控制块
	struct tcp_pcb *tcppcbconn;  	//定义一个TCP服务器控制块
  struct udp_pcb *udppcb;  	//定义一个udp服务器控制块
	u8 res=0,escap_flag=0;		
	u8 connflag=0;		     //连接标记
	INT8U err;
	httpd_init();//建立web服务器
 if(TCPorUDPFlag == 1)
 {
  tcppcbnew=tcp_new();	//创建一个新的pcb
	if(tcppcbnew)			    //创建成功
	{
		err_tcp = tcp_bind(tcppcbnew,IP_ADDR_ANY,TCP_SERVER_PORT);	//将本地IP与指定的端口号绑定在一起,IP_ADDR_ANY为绑定本地所有的IP地址
		if(err_tcp == ERR_OK)	//绑定完成
		{
			tcppcbconn=tcp_listen(tcppcbnew); 			     //设置tcppcb进入监听状态
			tcp_accept(tcppcbconn,tcp_server_accept); 	//初始化LWIP的tcp_accept的回调函数
		}else res=1;  
	}else res=1;
	while(1)
	{
    //创建成功
    if(res == 0)
    {
      if(tcp_server_flag&1<<6)//是否收到数据?
      {
        memp_buf = memp_malloc(MEMP_PBUF_POOL);//申请一块内存pool
				if(memp_buf != NULL)
				{
				  frame_head = (StrFrameHeader *)memp_buf;
          frame_head->type = HEAD_ETH_TYPE;
          frame_head->pdata = memp_buf + sizeof(StrFrameHeader);
          memp_buf =frame_head->pdata;         //定位接收数据指针
          for(i=0;i<tcp_server_recv_len;i++)
          {
            if(tcp_server_recvbuf[i] != 0x1B)     //非转义
            {
              if(escap_flag == 1)    //上次是否为转义
              {
                *memp_buf++ = tcp_server_recvbuf[i] + 0x1B;
                escap_flag = 0;
              }
              else
                *memp_buf++ = tcp_server_recvbuf[i];   
            }
            else
              escap_flag = 1;        //转义标志位置1
          }
           frame_head->len =  memp_buf-frame_head->pdata; 
           printf("frame_head->len ==%d\n",frame_head->len );
        }
        do
				{
					#if 0
							err = OSQPost(TestQueue,(void *)frame_head);//把拷贝的数据发送给控制处理任务
					#else
						err = OSQPost(CtrlCentRecMsgQeue,(void *)frame_head);//把拷贝的数据发送给控制处理任务
					#endif
        }
				while(err != OS_ERR_NONE);
				p = OSQPend(UdpTranMsgQeue,200,&err);   //等待控制处理任务返回的数据 
        if(p !=NULL && p->type == HEAD_ETH_TYPE)
        {
          Tcpdatalen = p->len;
          memset(tcp_server_recvbuf,0,TCP_SERVER_RX_BUFSIZE);  //数据接收缓冲区清零
          for(i=0;i<Tcpdatalen;i++)
          {
//            if(p->pdata[i] == 0x02 || p->pdata[i] == 0x03 || p->pdata[i] == 0x1B)//需要转义字符
//            {
//              tcp_server_recvbuf[i] = 0x1B;
//              tcp_server_recvbuf[i+1] = p->pdata[i] - 0x1B;
//            }
//            else
              tcp_server_recvbuf[i] = p->pdata[i];
          }
        }
       tcp_server_flag&=~(1<<6);//标记数据已经被处理了.
       tcp_server_flag|=1<<7;//标记要发送数据
       memp_free(MEMP_PBUF_POOL,(void *)p);	 //释放POOL内存	
      }
      if(tcp_server_flag&1<<5)//是否连接上?
      {
        if(connflag==0)
        {
          printf("Client IP:%d.%d.%d.%d\n",lwipdev.remoteip[0],lwipdev.remoteip[1],lwipdev.remoteip[2],lwipdev.remoteip[3]);//客户端IP
          printf("Receive Data:\n");//提示消息
          connflag=1;//标记连接了
        }
      }
			else if(connflag)
      {
        printf("disconnect\n");
        connflag=0;	//标记连接断开了
      }
      //LWIP轮询任务
      lwip_periodic_handle();  //执行发送
//      IWDG_ReloadCounter();
			//if(g_cHttpFlag == 0)//没有发送任务
			{
				OSTimeDly(1);
			}
    }
		else
		{
      tcp_server_connection_close(tcppcbnew,0);//关闭TCP Server连接
      tcp_server_connection_close(tcppcbconn,0);//关闭TCP Server连接 
      tcp_server_remove_timewait(); 
      memset(tcppcbnew,0,sizeof(struct tcp_pcb));
      memset(tcppcbconn,0,sizeof(struct tcp_pcb));
    }
   }
 }
 else   //UDP通信
 {
  udppcb=udp_new();
	if(udppcb)//创建成功
	{
    err_udp=udp_bind(udppcb,IP_ADDR_ANY,UDP_DEMO_PORT);//绑定本地IP地址与端口号
    if(err_udp==ERR_OK)	//绑定完成
    {
      udp_recv(udppcb,udp_demo_recv,NULL);//注册接收回调函数 
    }else res=1;
	}else res=1;
  
	while(res==0)
	{
		if(udp_demo_flag&1<<6)//是否收到数据
		{
      //返回给上位机数据 
      p = OSQPend(UdpTranMsgQeue,200,&err);  
      if(p !=NULL && p->type == HEAD_ETH_TYPE)
      {
        p = SW_Escape(p);        
//        printf("rec_len=%d\n",p->len);
      }
			udp_demo_flag&=~(1<<6);//标记数据已经被处理了.
      udp_demo_senddata(udppcb,p);
		}
		OSTimeDly(1);
	}
	udp_demo_connection_close(udppcb); 
 }
}
	

StrFrameHeader* SW_Escape(StrFrameHeader *frame_head)
{
  StrFrameHeader *ret_head;
	INT16U test,i;
	INT8U *p,*dp;
	
	p = frame_head->pdata; 
	ret_head = memp_malloc(MEMP_PBUF_POOL);//申请一块内存
	test = 0;
	while(ret_head == NULL)
	{
	  test++;
		if(test == 1)
			//printf("\n\r 获取内存失败：SW_Escape");
		OSTimeDly(5);
		ret_head = memp_malloc(MEMP_PBUF_POOL);     //申请一块内存用于数据发送
	}
	ret_head->type = frame_head->type;
	ret_head->pdata = (INT8U *)ret_head + sizeof(StrFrameHeader);
	dp = ret_head->pdata;
	*dp++ = *p;  //拷贝第一个数据
	for(i=1;i<frame_head->len-1;i++)
	{
		if(p[i] == 0x02 || p[i] == 0x03 || p[i] == 0x1B)//需要转义字符
		{
      *dp++ = 0x1B;
		  *dp++ = p[i] - 0x1B;
    }
		else
			*dp++ = p[i];
  }
	*dp++ = p[frame_head->len-1];//最后一个数据
	
	ret_head->len = dp - ret_head->pdata;//存储数据个数
	memp_free(MEMP_PBUF_POOL,(void *)frame_head);//释放内存
	return ret_head;
}



