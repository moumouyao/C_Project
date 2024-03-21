#include "app.h"
#include "lwip_comm.h"
#include "delay.h"
#include "led.h"
#include "ucos_ii.h"
#include "app_cfg.h"
#include "sw_global.h"
#include "user_tim3_pwm.h"
#include "AD.H"
#include "lsens.h"
#include "i2c.h"
#include "dispose.h"
#include "RTC.h"
#include "memp.h"
#include "key.h"
#include "wdg.h"
#include "sw_controlcenter.h"
#include "Cjson.h"
#include "lwip/memp.h"

uint8_t hear_beat_flag = 0;

void Task_LED1(void *p_arg)
{ 
	uint8_t i=0, heart_num = 0;//��������
	unsigned char j=0;//���ڼ�ⰴ��
  (void)p_arg;// 'p_arg' ��û���õ�����ֹ��������ʾ����
//	INT8U err;
//	
//	static StrFrameHeader *ret;
//	static INT8U * pot=NULL;
//	pot = memp_malloc(MEMP_PBUF_POOL);
//		if(pot == NULL)
//		{
////			memp_free(MEMP_PBUF_POOL,(void *)frame_head);//�ͷ�POOL�ڴ�	
//			NVIC_SystemReset();
//		}
//		ret = (StrFrameHeader *)pot;
//		ret->type = 1;//ETH_TYPE;
//		ret->pdata = pot + sizeof(StrFrameHeader);
//		cJSON *cjson_test;
//		cjson_test=cJSON_CreateObject();
//		cJSON_AddStringToObject(cjson_test,"id","heart");//����ַ����͵�
//		cJSON_AddStringToObject(cjson_test,"msgType","messageBeat");
//		char *cjson_data=cJSON_PrintUnformatted(cjson_test);
//		memcpy(ret->pdata,cjson_data,strlen(cjson_data));
//		ret->len=strlen(cjson_data);
//		cJSON_Delete(cjson_test);
//		ret->pdata = pot + sizeof(StrFrameHeader);
	
	while (1)
	{
    IWDG_ReloadCounter();//ι��
    OSTimeDly(100);
		//===============�ƹ�-����-ʱ���ȡ==========//
		if(i==10) //100ms�Ʒ�תһ��
		{
			heart_num++;
			BrightRefresh();	
			i=0;
			LED1=!LED1;
//			printf("hello world\r\n");
			getTime();//��ȡһ��ʱ��
		}
		else 
		{
			i++;
		}
		
		//������������ÿ����һ��
		if(heart_num >= 60){
			hear_beat_flag = 1;
			heart_num = 0;
//			Json_playlist();
////			err = OSQPost(UdpHearbeatQeue,(void *)ret);//����Udp��Ϣ����
//			err = OSQPost(UdpTranMsgQeue,(void *)ret);//����Udp��Ϣ����
//			err=err;
		}
		
		//================��ַ��λ�������=============//
		if(PAin(11) == 0)//��������
		{
			j++;
		}
		else//����δ����
		{
			j = 0;
		}
		if(j>=50)//
		{
			init_IP();//����IP��ַ
			LED1 = 0;
			LED0 = 0;
			while(PAin(11) == 0);//�ȴ�����
		}
		//================��λ��־λ���===============//
		if(g_cSysRst == 1)//ϵͳ��Ҫ��λ
		{
			NVIC_SystemReset();//��λ
		}

   }
}

/*
//
//������:init_IP
//���ܣ����ڳ�ʼ��IP��ַ
*/
void init_IP()
{
	unsigned char i;
	//==========����IP��ַ=========//
	g_cIp[0] = 192;
	g_cIp[1] = 168;
	g_cIp[2] = 1;
	g_cIp[3] = 5;
	
	//==========��������=========//
	g_cGateway[0] = 192;
	g_cGateway[1] = 168;
	g_cGateway[2] = 1;
	g_cGateway[3] = 1;
	
	//======���ö˿ں�============//
	g_cPort = 8001;
	
	//============д��EEPROM===========//
	//д��˿�
	E2promWriteByte(EEROM_Port_ADDR,g_cPort>>8);//�����λ
	E2promWriteByte(EEROM_Port_ADDR+1,g_cPort);//�����λ
	//д������ �� IP
	for(i=0;i<4;i++)
	{
		E2promWriteByte(EEROM_Gateway_ADDR+i,g_cGateway[i]);
		E2promWriteByte(EEROM_Local_IP_ADDR+i,g_cIp[i]);
	}
 	g_cSysRst = 1;//����

}


void Task_KEY(void *p_arg)
{ 
  (void)p_arg;                				// 'p_arg' ��û���õ�����ֹ��������ʾ����
	uint8_t keytemp=0,lastkey=0;
	uint8_t i=0;
	while (1)
	{		
			i=0;
			keytemp=0;
			keytemp=Key_Value();
			OSTimeDly(100);
			i=Key_Value();
			if(keytemp!=0&&i==keytemp)
      {
        if(keytemp !=lastkey)  //ANJIAN1  PA0  ANJIAN2  PC3   ANJIAN3  PC2
				{
         Key_UpdateItemToSlaves(keytemp); //��ⰴ������Ԥ�ò��ű�
         lastkey=keytemp;//����keyֵ				
				}
      }
			OSTimeDly(100);
   }
}
