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
	struct tcp_pcb *tcppcbnew;  	//����һ��TCP���������ƿ�
	struct tcp_pcb *tcppcbconn;  	//����һ��TCP���������ƿ�
  struct udp_pcb *udppcb;  	//����һ��udp���������ƿ�
	u8 res=0,escap_flag=0;		
	u8 connflag=0;		     //���ӱ��
	INT8U err;
	httpd_init();//����web������
 if(TCPorUDPFlag == 1)
 {
  tcppcbnew=tcp_new();	//����һ���µ�pcb
	if(tcppcbnew)			    //�����ɹ�
	{
		err_tcp = tcp_bind(tcppcbnew,IP_ADDR_ANY,TCP_SERVER_PORT);	//������IP��ָ���Ķ˿ںŰ���һ��,IP_ADDR_ANYΪ�󶨱������е�IP��ַ
		if(err_tcp == ERR_OK)	//�����
		{
			tcppcbconn=tcp_listen(tcppcbnew); 			     //����tcppcb�������״̬
			tcp_accept(tcppcbconn,tcp_server_accept); 	//��ʼ��LWIP��tcp_accept�Ļص�����
		}else res=1;  
	}else res=1;
	while(1)
	{
    //�����ɹ�
    if(res == 0)
    {
      if(tcp_server_flag&1<<6)//�Ƿ��յ�����?
      {
        memp_buf = memp_malloc(MEMP_PBUF_POOL);//����һ���ڴ�pool
				if(memp_buf != NULL)
				{
				  frame_head = (StrFrameHeader *)memp_buf;
          frame_head->type = HEAD_ETH_TYPE;
          frame_head->pdata = memp_buf + sizeof(StrFrameHeader);
          memp_buf =frame_head->pdata;         //��λ��������ָ��
          for(i=0;i<tcp_server_recv_len;i++)
          {
            if(tcp_server_recvbuf[i] != 0x1B)     //��ת��
            {
              if(escap_flag == 1)    //�ϴ��Ƿ�Ϊת��
              {
                *memp_buf++ = tcp_server_recvbuf[i] + 0x1B;
                escap_flag = 0;
              }
              else
                *memp_buf++ = tcp_server_recvbuf[i];   
            }
            else
              escap_flag = 1;        //ת���־λ��1
          }
           frame_head->len =  memp_buf-frame_head->pdata; 
           printf("frame_head->len ==%d\n",frame_head->len );
        }
        do
				{
					#if 0
							err = OSQPost(TestQueue,(void *)frame_head);//�ѿ��������ݷ��͸����ƴ�������
					#else
						err = OSQPost(CtrlCentRecMsgQeue,(void *)frame_head);//�ѿ��������ݷ��͸����ƴ�������
					#endif
        }
				while(err != OS_ERR_NONE);
				p = OSQPend(UdpTranMsgQeue,200,&err);   //�ȴ����ƴ������񷵻ص����� 
        if(p !=NULL && p->type == HEAD_ETH_TYPE)
        {
          Tcpdatalen = p->len;
          memset(tcp_server_recvbuf,0,TCP_SERVER_RX_BUFSIZE);  //���ݽ��ջ���������
          for(i=0;i<Tcpdatalen;i++)
          {
//            if(p->pdata[i] == 0x02 || p->pdata[i] == 0x03 || p->pdata[i] == 0x1B)//��Ҫת���ַ�
//            {
//              tcp_server_recvbuf[i] = 0x1B;
//              tcp_server_recvbuf[i+1] = p->pdata[i] - 0x1B;
//            }
//            else
              tcp_server_recvbuf[i] = p->pdata[i];
          }
        }
       tcp_server_flag&=~(1<<6);//��������Ѿ���������.
       tcp_server_flag|=1<<7;//���Ҫ��������
       memp_free(MEMP_PBUF_POOL,(void *)p);	 //�ͷ�POOL�ڴ�	
      }
      if(tcp_server_flag&1<<5)//�Ƿ�������?
      {
        if(connflag==0)
        {
          printf("Client IP:%d.%d.%d.%d\n",lwipdev.remoteip[0],lwipdev.remoteip[1],lwipdev.remoteip[2],lwipdev.remoteip[3]);//�ͻ���IP
          printf("Receive Data:\n");//��ʾ��Ϣ
          connflag=1;//���������
        }
      }
			else if(connflag)
      {
        printf("disconnect\n");
        connflag=0;	//������ӶϿ���
      }
      //LWIP��ѯ����
      lwip_periodic_handle();  //ִ�з���
//      IWDG_ReloadCounter();
			//if(g_cHttpFlag == 0)//û�з�������
			{
				OSTimeDly(1);
			}
    }
		else
		{
      tcp_server_connection_close(tcppcbnew,0);//�ر�TCP Server����
      tcp_server_connection_close(tcppcbconn,0);//�ر�TCP Server���� 
      tcp_server_remove_timewait(); 
      memset(tcppcbnew,0,sizeof(struct tcp_pcb));
      memset(tcppcbconn,0,sizeof(struct tcp_pcb));
    }
   }
 }
 else   //UDPͨ��
 {
  udppcb=udp_new();
	if(udppcb)//�����ɹ�
	{
    err_udp=udp_bind(udppcb,IP_ADDR_ANY,UDP_DEMO_PORT);//�󶨱���IP��ַ��˿ں�
    if(err_udp==ERR_OK)	//�����
    {
      udp_recv(udppcb,udp_demo_recv,NULL);//ע����ջص����� 
    }else res=1;
	}else res=1;
  
	while(res==0)
	{
		if(udp_demo_flag&1<<6)//�Ƿ��յ�����
		{
      //���ظ���λ������ 
      p = OSQPend(UdpTranMsgQeue,200,&err);  
      if(p !=NULL && p->type == HEAD_ETH_TYPE)
      {
        p = SW_Escape(p);        
//        printf("rec_len=%d\n",p->len);
      }
			udp_demo_flag&=~(1<<6);//��������Ѿ���������.
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
	ret_head = memp_malloc(MEMP_PBUF_POOL);//����һ���ڴ�
	test = 0;
	while(ret_head == NULL)
	{
	  test++;
		if(test == 1)
			//printf("\n\r ��ȡ�ڴ�ʧ�ܣ�SW_Escape");
		OSTimeDly(5);
		ret_head = memp_malloc(MEMP_PBUF_POOL);     //����һ���ڴ��������ݷ���
	}
	ret_head->type = frame_head->type;
	ret_head->pdata = (INT8U *)ret_head + sizeof(StrFrameHeader);
	dp = ret_head->pdata;
	*dp++ = *p;  //������һ������
	for(i=1;i<frame_head->len-1;i++)
	{
		if(p[i] == 0x02 || p[i] == 0x03 || p[i] == 0x1B)//��Ҫת���ַ�
		{
      *dp++ = 0x1B;
		  *dp++ = p[i] - 0x1B;
    }
		else
			*dp++ = p[i];
  }
	*dp++ = p[frame_head->len-1];//���һ������
	
	ret_head->len = dp - ret_head->pdata;//�洢���ݸ���
	memp_free(MEMP_PBUF_POOL,(void *)frame_head);//�ͷ��ڴ�
	return ret_head;
}



