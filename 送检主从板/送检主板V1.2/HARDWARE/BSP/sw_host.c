/**
  ******************************************************************************
  * 文件名 : sw_host.c
  * 作  者 : 陈光
  * 版  本 ：V1.0
  * 时  间 : 2014年5月20日
  * 公  司 ：汉威光电技术有限公司
  * 摘  要 ：本文件包含了主机控制LED显示屏函数模块。
  *
  ******************************************************************************
  */



/**
  ******************************************************************************
  *                                  头文件
  ******************************************************************************
  */
	
#include "user_tim3_pwm.h"
#include "sw_host.h"
#include "includes.h"
#include "sw_conf.h"
#include "sw_global.h"
//#include "24cxx.h" 
#include "i2c.h"
/**
  ******************************************************************************
  *                              USART2TranConfig()
  *
  * 描述   : USART1发送参数配置
  *		   
  * 参数   : INT16U  num    要发送的数据长度
	*          INT8U   *P     要发送的数据指针
  *
  *
  * 返回值 : 无
  ******************************************************************************
  */


void RS485TranConfig (uint8_t *p,uint16_t num)
{
  RS485TranCnt = 0;                      //发送数据计数器清零
  RS485TranLength = num;                 //发送数据长度
	RS485TranDataPot = p;	                 //发送数据指针
  
//  printf("RS485TranLength=%d",RS485TranLength);
  

}

/**															   
  ******************************************************************************
  *                             SW_HostGPIOConfig()
  *
  * 描述   : 初始化主控制器LED的GPIO控制引脚初始化
  *		   
  * 参数   : 无
  *
  *
  * 返回值 : 无
  ******************************************************************************
  */

void SW_HostGPIOConfig (void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE , ENABLE);

//	//PG8推挽输出，485模式控制  
//  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8; //GPIOG8
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//输出
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//速度100MHz
//	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //推挽输出
//	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //上拉
//	GPIO_Init(GPIOG,&GPIO_InitStructure); //初始化PG8
   
	 
	/* PE2：SlaveRST */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT; 
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOE, &GPIO_InitStructure);  
  
  
	/* PE4：LCP */                                   
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT; 
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init(GPIOE, &GPIO_InitStructure);   
}



void GetE2PROMSet(void )//G_ItemFlag,G_ItemUpdateFlag,,G_FrameCRC32
{
	u8 temp[80],i;
	unsigned char cIpData;
	G_ItemFlag = E2promReadByte(EEROM_ITEM_FLAG_ADDR);  //flash中存有完整play.lst数据标志位
	
  TCPorUDPFlag = E2promReadByte(EEROM_TCP_OR_UDP_ADDR); //TCP和UDP通讯标志位
//  printf(" TCPorUDPFlag=%d\n",TCPorUDPFlag);
  
	bright_level_flag = E2promReadByte(EEROM_BRIGHT_LEVEL_FLAG_ADDR);//亮度级标志位	
//  printf("bright_level_flag1=%d\r\n",bright_level_flag);
	if(bright_level_flag!=0 && bright_level_flag!=1 && bright_level_flag!=2)
		bright_level_flag = 0;
    BRIGHT_Level_Set(0);
	
	max_bright_proportion = E2promReadByte(EEROM_MAX_BRIGHT_PROPORTION_ADDR);//最大亮度百分比
//   printf("max_bright_proportion= %d\r\n",max_bright_proportion);
  if(max_bright_proportion>99 || max_bright_proportion<=0)
	{
		max_bright_proportion = 60;
		E2promWriteByte(EEROM_MAX_BRIGHT_PROPORTION_ADDR,max_bright_proportion);
	}
	YuZhiFlag = E2promReadByte(EEROM_YUZHIFLAG_ADDR);//预制播放标志位
	YuZhiXuHao = E2promReadByte(EEROM_YUZHIXUHAO_ADDR); //预置文件序号
////	printf("\r\n 完整数据G_ItemFlag=%d",G_ItemFlag);
	G_ItemUpdateFlag = E2promReadByte(EEROM_ITEM_UPDATE_FLAG_ADDR); //帧数据更新到从机完毕标志位
// 	I2C_ReadS_24C(EEROM_ITEM_UPDATE_FLAG_ADDR,&G_ItemUpdateFlag,1);
	;//printf("\r\n 完整数据更新标志位G_ItemUpdateFlag=%d",G_ItemUpdateFlag);
	
	//屏体参数/从控制区域参数/模组/IP参数设置完成标志位
	G_DataSetFlag = E2promReadByte(EEROM_DATA_SET_FLAG_ADDR);
	;//printf("\r\n EEROM_DATA_SET_FLAG = %d",G_DataSetFlag);
	if(G_DataSetFlag == 1)
	{
    for(i=0;i<69;i++)
	 {
      temp[i] = E2promReadByte(EEROM_DATA_SET_START_ADDR+i);	
    }
		RefreshDataSet(temp);//解析并更新参数设置
  }
	//==================本机IP地址更新=================//
	cIpData = E2promReadByte(EEROM_Local_IP_ADDR);//读取IP高位

		g_cIp[0] = E2promReadByte(EEROM_Local_IP_ADDR);//读取IP
		g_cIp[1] = E2promReadByte(EEROM_Local_IP_ADDR+1);//读取IP
		g_cIp[2] = E2promReadByte(EEROM_Local_IP_ADDR+2);//读取IP
		g_cIp[3] = E2promReadByte(EEROM_Local_IP_ADDR+3);//读取IP
	
	if(cIpData==0xff)
		{
		g_cIp[0] = 192;//读取IP
		g_cIp[1] = 168;//读取IP
		g_cIp[2] = 1;//读取IP
		g_cIp[3] = 5;//读取IP
	}
	//==================网关IP地址更新=================//
	cIpData = E2promReadByte(EEROM_Gateway_ADDR);//读取IP高位
		g_cGateway[0] = E2promReadByte(EEROM_Gateway_ADDR);//读取IP
		g_cGateway[1] = E2promReadByte(EEROM_Gateway_ADDR+1);//读取IP
		g_cGateway[2] = E2promReadByte(EEROM_Gateway_ADDR+2);//读取IP
		g_cGateway[3] = E2promReadByte(EEROM_Gateway_ADDR+3);//读取IP
	if(cIpData==0xff)
		{
		g_cGateway[0] = 192;//读取IP
		g_cGateway[1] = 168;//读取IP
		g_cGateway[2] = 1;//读取IP
		g_cGateway[3] = 1;//读取IP
	}


	g_cPort = E2promReadByte(EEROM_Port_ADDR)<<8|E2promReadByte(EEROM_Port_ADDR+1);//读取端口号
	if(g_cPort==0xffff)
	{
	g_cPort=8001;
	}
	PwmSet(16);//设置PWM输出
	
	unsigned char botelv[6] = {0,0,0,0,0,0};
	for(i=0;i<6;i++)
	botelv[i]=E2promReadByte(EEROM_USART_BPS+i);//读取波特率
	G_BaudRate=(botelv[5]-0x30)+((botelv[4]-0x30)*10)+((botelv[3]-0x30)*100)+((botelv[2]-0x30)*1000)+((botelv[1]-0x30)*10000)+((botelv[0]-0x30)*100000);
	if(botelv[0]==0xff&&botelv[1]==0xff&&botelv[2]==0xff&&botelv[3]==0xff&&botelv[4]==0xff&&botelv[5]==0xff)
	{
	G_BaudRate=9600;
	}
}

/**															   
  ******************************************************************************
  *                            RefreshDataSet()
  *
  * 描述   : 更新设置的屏体、从控区域、模组类别、波特率、IP/MASK/GT/PORT等
  *		   
  * 参数   : p:监控中心发送过来的源数据指针
  *
  *
  * 返回值 : 无
  ******************************************************************************
  */
void RefreshDataSet (uint8_t *p)
{
  int8_t i;
	//LWIP的MAC地址跟这个地址有关，具体见ethernetif.c

	
  Hostdata1=*p++;
  Hostdata2=*p++;
  
	G_ScreenSet.HANG = (p[0]-48)*1000 + (p[1]-48)*100 +(p[2]-48)*10 +(p[3]-48);
	p += 4;  
       
	G_ScreenSet.LIE = (p[0]-48)*1000 + (p[1]-48)*100 +(p[2]-48)*10 +(p[3]-48);
	p += 4;
	G_ScreenSet.FANHUI = G_ScreenSet.LIE >> 3;// 一列点阵字节数
	G_ScreenSet.YANSE = (p[0]-48) + (p[1]-48) + (p[2]-48);
	p += 3;
	
	
	G_BoxSet.HANG = (p[0]-48)*1000 + (p[1]-48)*100 +(p[2]-48)*10 +(p[3]-48);
	p += 4;
	G_BoxSet.HBYTE = G_BoxSet.HANG >> 3;
	G_BoxSet.LIE = (p[0]-48)*1000 + (p[1]-48)*100 +(p[2]-48)*10 +(p[3]-48);
	p += 4;
	
	if(G_ScreenSet.HANG%G_BoxSet.HANG==0)
    SCREEN_HANG_FH=G_ScreenSet.HANG;//.......................................................................//
	else
	{
    SCREEN_HANG_FH=G_ScreenSet.HANG;
    G_ScreenSet.HANG=G_ScreenSet.HANG+G_BoxSet.HANG-G_ScreenSet.HANG%G_BoxSet.HANG;
    SCREEN_HANG=G_ScreenSet.HANG;
	}
	
	G_ScreenSet.HBYTE = G_ScreenSet.HANG >> 3;//一行点阵字节数 
	G_ScreenSet.BYTENUM = G_ScreenSet.HBYTE*G_ScreenSet.LIE;///？？？？？？注意此处！！！！
	
	G_BoxSet.FANHUI = G_BoxSet.LIE >> 3;
	G_BoxSet.YANSE = G_ScreenSet.YANSE;
	G_BoxSet.BYTENUM = G_BoxSet.HBYTE*G_BoxSet.LIE;
	
	G_MoZuType = p[0]-48;  //模组类别
	p += 1;
	
	G_BaudRate = (p[0]-48)*100000 + (p[1]-48)*10000 +(p[2]-48)*1000 +
	             (p[3]-48)*100 + (p[4]-48)*10 + (p[5]-48);//波特率
	p += 6;
	
	for(i=0;i<4;i++)    //IP地址
	{
    IPAddress[i] = (p[0]-48)*100 + (p[1]-48)*10 +(p[2]-48);
		p += 3;
  }
	for(i=0;i<4;i++)    //子网掩码
	{
    NetMask[i] = (p[0]-48)*100 + (p[1]-48)*10 +(p[2]-48);
		p += 3;
  }
	for(i=0;i<4;i++)    //网关
	{
    GetWay[i] = (p[0]-48)*100 + (p[1]-48)*10 +(p[2]-48);
		p += 3;
  }	
	
	LocalPort = (p[0]-48)*10000 + (p[1]-48)*1000 +(p[2]-48)*100 +
	             (p[3]-48)*10 + (p[4]-48);//UDP端口	
	

	G_SlaveNum = (G_ScreenSet.HBYTE/G_BoxSet.HBYTE)*(G_ScreenSet.LIE/G_BoxSet.LIE);
	
//	printf("\r\n 监控中心的主机地址：%c%c",HostAddrH,HostAddrL);
//	printf("\r\n 屏体参数：宽度:%d,高度:%d,颜色:%d",G_ScreenSet.HANG,
//	                         G_ScreenSet.LIE,G_ScreenSet.YANSE);
//	printf("\r\n 从控区域：宽度:%d,高度:%d,颜色:%d",G_BoxSet.HANG,
//	                         G_BoxSet.LIE,G_BoxSet.YANSE);
//	printf("\r\n 模组类别:%d",G_MoZuType);
//	
//	printf("\r\n 波特率:%d",G_BaudRate);
////	
////	printf("\r\n IP:%d.%d.%d.%d",IPAddress[0],IPAddress[1],IPAddress[2],IPAddress[3]);
////	printf("\r\n mask:%d.%d.%d.%d",NetMask[0],NetMask[1],NetMask[2],NetMask[3]);
////	printf("\r\n GT:%d.%d.%d.%d",GetWay[0],GetWay[1],GetWay[2],GetWay[3]);
////	printf("\r\n LocalPort:%d",LocalPort);
}





/**															   
  ******************************************************************************
  *                            StoreDataSet()
  *
  * 描述   : 存储设置的屏体、从控区域、模组类别、波特率、IP/MASK/GT/PORT等
  *		   
  * 参数   : p:监控中心发送过来的源数据指针
  *
  *
  * 返回值 : 无
  ******************************************************************************
  */
void StoreDataSet (uint8_t *p,uint8_t len)
{
  uint8_t i;
	//屏体参数/从控制区域参数/模组/IP参数设置完成标志位
	G_DataSetFlag = 0;//标志位清零-MCU重启后会更新
	E2promWriteByte(EEROM_DATA_SET_FLAG_ADDR,G_DataSetFlag);
	for(i=0;i<69;i++)
	{
		E2promWriteByte(EEROM_DATA_SET_START_ADDR+i,p[i]);//写入原来的参数
	}
	p+=22;//后移28个字节 指向IP地址
	for(i=0;i<6;i++)
		E2promWriteByte(EEROM_USART_BPS+i,p[i]);
	
	p+=6;
	//==========设置IP===========//
	g_cIp[0] = (p[0]-0x30)*100;
	g_cIp[0] += (p[1]-0x30)*10;
	g_cIp[0] += (p[2]-0x30);
	g_cIp[1] = (p[3]-0x30)*100;
	g_cIp[1] += (p[4]-0x30)*10;
	g_cIp[1] += (p[5]-0x30);
	g_cIp[2] = (p[6]-0x30)*100;
	g_cIp[2] += (p[7]-0x30)*10;
	g_cIp[2] += (p[8]-0x30);
	g_cIp[3] = (p[9]-0x30)*100;
	g_cIp[3] += (p[10]-0x30)*10;
	g_cIp[3] += (p[11]-0x30);
	p+=12;//指向子网掩码
	p+=12;//指向网关
	
	//=========设置网关=========//
	g_cGateway[0] = (p[0]-0x30)*100;
	g_cGateway[0] += (p[1]-0x30)*10;
	g_cGateway[0] += (p[2]-0x30);
	g_cGateway[1] = (p[3]-0x30)*100;
	g_cGateway[1] += (p[4]-0x30)*10;
	g_cGateway[1] += (p[5]-0x30);
	g_cGateway[2] = (p[6]-0x30)*100;
	g_cGateway[2] += (p[7]-0x30)*10;
	g_cGateway[2] += (p[8]-0x30);
	g_cGateway[3] = (p[9]-0x30)*100;
	g_cGateway[3] += (p[10]-0x30)*10;
	g_cGateway[3] += (p[11]-0x30);
	p+=12;//指向端口
	
	//=========设置端口===========//
	g_cPort = (p[0]-0x30)*10000;
	g_cPort += (p[1]-0x30)*1000;
	g_cPort += (p[2]-0x30)*100;
	g_cPort += (p[3]-0x30)*10;
	g_cPort += (p[4]-0x30);
	
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
 	G_DataSetFlag = 1;//标志位EEROM中写1-MCU重启后会更新
	E2promWriteByte(EEROM_DATA_SET_FLAG_ADDR,1);
}


void Usart_StoreDataSet(uint8_t *p,uint8_t len,uint8_t type)
{
  uint8_t i,k=0,num=0,m;
	//屏体参数/从控制区域参数/模组/IP参数设置完成标志位
	G_DataSetFlag = 0;//标志位清零-MCU重启后会更新
	E2promWriteByte(EEROM_DATA_SET_FLAG_ADDR,G_DataSetFlag);
  if(type == 0x01)
  {
    for(i=0;i<6-len;i++)
    {
      E2promWriteByte(EEROM_DATA_SET_START_ADDR+22+i,0);
      k++;
    }
    for(i=0;i<len;i++)   //bps
      E2promWriteByte(EEROM_DATA_SET_START_ADDR+22+i+k,p[i]);
    E2promWriteByte(EEROM_USART_BPS_DATA_ADDR,1);
  } 
  else if(type == 0x02)
  {
//    E2promWriteByte(EEROM_USART_IP_ADDR,0);
//    for(i=0;i<3;i++) //ip
//    {
//      E2promWriteByte(EEROM_DATA_SET_START_ADDR+28+i,p[i]);
//        printf("%x ",p[i]);
//    }
//    for(i=4;i<7;i++) //ip
//    {
//      E2promWriteByte(EEROM_DATA_SET_START_ADDR+28+i,p[i]);
//        printf("%x ",p[i]);
//    }
//    
//    for(i=0;i<5;i++) //ip
//    {
//      E2promWriteByte(EEROM_DATA_SET_START_ADDR+64+i,p[i+64]);
//        printf("%x ",p[i]);
//    }
//    E2promWriteByte(EEROM_USART_IP_ADDR,1);
  }
  else if(type == 0x03)
  {
    len =0;
    E2promWriteByte(EEROM_USART_SCREEN_ADDR,0);
//    for(i=2;i<21;i++)
//      E2promWriteByte(EEROM_DATA_SET_START_ADDR+i,0);
    for(m=0;m<2;m++) //主板
    {
      num = p[m]-48;
//      printf("num=%d \n",num);
      for(i=0;i<4-num;i++)
      {
        E2promWriteByte(EEROM_DATA_SET_START_ADDR+2+i+m*4,0);
        k++;
      }
      for(i=0;i<num;i++)  
      {
        E2promWriteByte(EEROM_DATA_SET_START_ADDR+2+m*4+k+i,p[i+4+len]);
//        printf("%x ",p[i+4+len]);
      }
      k=0;
      len+=num;
    }
    
     for(m=0;m<2;m++) //从板
    {
      num = p[m]-48;
//      printf("num=%d \n",num);
      for(i=0;i<4-num;i++)
      {
        E2promWriteByte(EEROM_DATA_SET_START_ADDR+13+i+m*4,0);
        k++;
      }
      for(i=0;i<num;i++)  
      {
        E2promWriteByte(EEROM_DATA_SET_START_ADDR+13+m*4+k+i,p[i+len+4]);
//        printf("%x ",p[i+4+len]);
      }
      k=0;
      len+=num;
    }
    E2promWriteByte(EEROM_USART_SCREEN_ADDR,1);
  }
  
}








