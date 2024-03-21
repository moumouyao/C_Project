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
	uint8_t i=0, heart_num = 0;//用于闪灯
	unsigned char j=0;//用于检测按键
  (void)p_arg;// 'p_arg' 并没有用到，防止编译器提示警告
//	INT8U err;
//	
//	static StrFrameHeader *ret;
//	static INT8U * pot=NULL;
//	pot = memp_malloc(MEMP_PBUF_POOL);
//		if(pot == NULL)
//		{
////			memp_free(MEMP_PBUF_POOL,(void *)frame_head);//释放POOL内存	
//			NVIC_SystemReset();
//		}
//		ret = (StrFrameHeader *)pot;
//		ret->type = 1;//ETH_TYPE;
//		ret->pdata = pot + sizeof(StrFrameHeader);
//		cJSON *cjson_test;
//		cjson_test=cJSON_CreateObject();
//		cJSON_AddStringToObject(cjson_test,"id","heart");//添加字符类型的
//		cJSON_AddStringToObject(cjson_test,"msgType","messageBeat");
//		char *cjson_data=cJSON_PrintUnformatted(cjson_test);
//		memcpy(ret->pdata,cjson_data,strlen(cjson_data));
//		ret->len=strlen(cjson_data);
//		cJSON_Delete(cjson_test);
//		ret->pdata = pot + sizeof(StrFrameHeader);
	
	while (1)
	{
    IWDG_ReloadCounter();//喂狗
    OSTimeDly(100);
		//===============灯光-亮度-时间获取==========//
		if(i==10) //100ms灯反转一次
		{
			heart_num++;
			BrightRefresh();	
			i=0;
			LED1=!LED1;
//			printf("hello world\r\n");
			getTime();//获取一次时间
		}
		else 
		{
			i++;
		}
		
		//发送心跳包，每分钟一次
		if(heart_num >= 60){
			hear_beat_flag = 1;
			heart_num = 0;
//			Json_playlist();
////			err = OSQPost(UdpHearbeatQeue,(void *)ret);//发送Udp消息队列
//			err = OSQPost(UdpTranMsgQeue,(void *)ret);//发送Udp消息队列
//			err=err;
		}
		
		//================地址复位按键检测=============//
		if(PAin(11) == 0)//按键按下
		{
			j++;
		}
		else//按键未按下
		{
			j = 0;
		}
		if(j>=50)//
		{
			init_IP();//重置IP地址
			LED1 = 0;
			LED0 = 0;
			while(PAin(11) == 0);//等待松手
		}
		//================复位标志位检测===============//
		if(g_cSysRst == 1)//系统需要复位
		{
			NVIC_SystemReset();//复位
		}

   }
}

/*
//
//函数名:init_IP
//功能：用于初始化IP地址
*/
void init_IP()
{
	unsigned char i;
	//==========设置IP地址=========//
	g_cIp[0] = 192;
	g_cIp[1] = 168;
	g_cIp[2] = 1;
	g_cIp[3] = 5;
	
	//==========设置网关=========//
	g_cGateway[0] = 192;
	g_cGateway[1] = 168;
	g_cGateway[2] = 1;
	g_cGateway[3] = 1;
	
	//======设置端口号============//
	g_cPort = 8001;
	
	//============写入EEPROM===========//
	//写入端口
	E2promWriteByte(EEROM_Port_ADDR,g_cPort>>8);//保存高位
	E2promWriteByte(EEROM_Port_ADDR+1,g_cPort);//保存地位
	//写入网关 和 IP
	for(i=0;i<4;i++)
	{
		E2promWriteByte(EEROM_Gateway_ADDR+i,g_cGateway[i]);
		E2promWriteByte(EEROM_Local_IP_ADDR+i,g_cIp[i]);
	}
 	g_cSysRst = 1;//重启

}


void Task_KEY(void *p_arg)
{ 
  (void)p_arg;                				// 'p_arg' 并没有用到，防止编译器提示警告
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
         Key_UpdateItemToSlaves(keytemp); //检测按键设置预置播放表
         lastkey=keytemp;//保存key值				
				}
      }
			OSTimeDly(100);
   }
}
