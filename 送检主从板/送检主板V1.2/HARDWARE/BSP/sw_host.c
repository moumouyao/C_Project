/**
  ******************************************************************************
  * �ļ��� : sw_host.c
  * ��  �� : �¹�
  * ��  �� ��V1.0
  * ʱ  �� : 2014��5��20��
  * ��  ˾ ��������缼�����޹�˾
  * ժ  Ҫ �����ļ���������������LED��ʾ������ģ�顣
  *
  ******************************************************************************
  */



/**
  ******************************************************************************
  *                                  ͷ�ļ�
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
  * ����   : USART1���Ͳ�������
  *		   
  * ����   : INT16U  num    Ҫ���͵����ݳ���
	*          INT8U   *P     Ҫ���͵�����ָ��
  *
  *
  * ����ֵ : ��
  ******************************************************************************
  */


void RS485TranConfig (uint8_t *p,uint16_t num)
{
  RS485TranCnt = 0;                      //�������ݼ���������
  RS485TranLength = num;                 //�������ݳ���
	RS485TranDataPot = p;	                 //��������ָ��
  
//  printf("RS485TranLength=%d",RS485TranLength);
  

}

/**															   
  ******************************************************************************
  *                             SW_HostGPIOConfig()
  *
  * ����   : ��ʼ����������LED��GPIO�������ų�ʼ��
  *		   
  * ����   : ��
  *
  *
  * ����ֵ : ��
  ******************************************************************************
  */

void SW_HostGPIOConfig (void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE , ENABLE);

//	//PG8���������485ģʽ����  
//  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8; //GPIOG8
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//���
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//�ٶ�100MHz
//	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //�������
//	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //����
//	GPIO_Init(GPIOG,&GPIO_InitStructure); //��ʼ��PG8
   
	 
	/* PE2��SlaveRST */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT; 
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOE, &GPIO_InitStructure);  
  
  
	/* PE4��LCP */                                   
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT; 
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init(GPIOE, &GPIO_InitStructure);   
}



void GetE2PROMSet(void )//G_ItemFlag,G_ItemUpdateFlag,,G_FrameCRC32
{
	u8 temp[80],i;
	unsigned char cIpData;
	G_ItemFlag = E2promReadByte(EEROM_ITEM_FLAG_ADDR);  //flash�д�������play.lst���ݱ�־λ
	
  TCPorUDPFlag = E2promReadByte(EEROM_TCP_OR_UDP_ADDR); //TCP��UDPͨѶ��־λ
//  printf(" TCPorUDPFlag=%d\n",TCPorUDPFlag);
  
	bright_level_flag = E2promReadByte(EEROM_BRIGHT_LEVEL_FLAG_ADDR);//���ȼ���־λ	
//  printf("bright_level_flag1=%d\r\n",bright_level_flag);
	if(bright_level_flag!=0 && bright_level_flag!=1 && bright_level_flag!=2)
		bright_level_flag = 0;
    BRIGHT_Level_Set(0);
	
	max_bright_proportion = E2promReadByte(EEROM_MAX_BRIGHT_PROPORTION_ADDR);//������Ȱٷֱ�
//   printf("max_bright_proportion= %d\r\n",max_bright_proportion);
  if(max_bright_proportion>99 || max_bright_proportion<=0)
	{
		max_bright_proportion = 60;
		E2promWriteByte(EEROM_MAX_BRIGHT_PROPORTION_ADDR,max_bright_proportion);
	}
	YuZhiFlag = E2promReadByte(EEROM_YUZHIFLAG_ADDR);//Ԥ�Ʋ��ű�־λ
	YuZhiXuHao = E2promReadByte(EEROM_YUZHIXUHAO_ADDR); //Ԥ���ļ����
////	printf("\r\n ��������G_ItemFlag=%d",G_ItemFlag);
	G_ItemUpdateFlag = E2promReadByte(EEROM_ITEM_UPDATE_FLAG_ADDR); //֡���ݸ��µ��ӻ���ϱ�־λ
// 	I2C_ReadS_24C(EEROM_ITEM_UPDATE_FLAG_ADDR,&G_ItemUpdateFlag,1);
	;//printf("\r\n �������ݸ��±�־λG_ItemUpdateFlag=%d",G_ItemUpdateFlag);
	
	//�������/�ӿ����������/ģ��/IP����������ɱ�־λ
	G_DataSetFlag = E2promReadByte(EEROM_DATA_SET_FLAG_ADDR);
	;//printf("\r\n EEROM_DATA_SET_FLAG = %d",G_DataSetFlag);
	if(G_DataSetFlag == 1)
	{
    for(i=0;i<69;i++)
	 {
      temp[i] = E2promReadByte(EEROM_DATA_SET_START_ADDR+i);	
    }
		RefreshDataSet(temp);//���������²�������
  }
	//==================����IP��ַ����=================//
	cIpData = E2promReadByte(EEROM_Local_IP_ADDR);//��ȡIP��λ

		g_cIp[0] = E2promReadByte(EEROM_Local_IP_ADDR);//��ȡIP
		g_cIp[1] = E2promReadByte(EEROM_Local_IP_ADDR+1);//��ȡIP
		g_cIp[2] = E2promReadByte(EEROM_Local_IP_ADDR+2);//��ȡIP
		g_cIp[3] = E2promReadByte(EEROM_Local_IP_ADDR+3);//��ȡIP
	
	if(cIpData==0xff)
		{
		g_cIp[0] = 192;//��ȡIP
		g_cIp[1] = 168;//��ȡIP
		g_cIp[2] = 1;//��ȡIP
		g_cIp[3] = 5;//��ȡIP
	}
	//==================����IP��ַ����=================//
	cIpData = E2promReadByte(EEROM_Gateway_ADDR);//��ȡIP��λ
		g_cGateway[0] = E2promReadByte(EEROM_Gateway_ADDR);//��ȡIP
		g_cGateway[1] = E2promReadByte(EEROM_Gateway_ADDR+1);//��ȡIP
		g_cGateway[2] = E2promReadByte(EEROM_Gateway_ADDR+2);//��ȡIP
		g_cGateway[3] = E2promReadByte(EEROM_Gateway_ADDR+3);//��ȡIP
	if(cIpData==0xff)
		{
		g_cGateway[0] = 192;//��ȡIP
		g_cGateway[1] = 168;//��ȡIP
		g_cGateway[2] = 1;//��ȡIP
		g_cGateway[3] = 1;//��ȡIP
	}


	g_cPort = E2promReadByte(EEROM_Port_ADDR)<<8|E2promReadByte(EEROM_Port_ADDR+1);//��ȡ�˿ں�
	if(g_cPort==0xffff)
	{
	g_cPort=8001;
	}
	PwmSet(16);//����PWM���
	
	unsigned char botelv[6] = {0,0,0,0,0,0};
	for(i=0;i<6;i++)
	botelv[i]=E2promReadByte(EEROM_USART_BPS+i);//��ȡ������
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
  * ����   : �������õ����塢�ӿ�����ģ����𡢲����ʡ�IP/MASK/GT/PORT��
  *		   
  * ����   : p:������ķ��͹�����Դ����ָ��
  *
  *
  * ����ֵ : ��
  ******************************************************************************
  */
void RefreshDataSet (uint8_t *p)
{
  int8_t i;
	//LWIP��MAC��ַ�������ַ�йأ������ethernetif.c

	
  Hostdata1=*p++;
  Hostdata2=*p++;
  
	G_ScreenSet.HANG = (p[0]-48)*1000 + (p[1]-48)*100 +(p[2]-48)*10 +(p[3]-48);
	p += 4;  
       
	G_ScreenSet.LIE = (p[0]-48)*1000 + (p[1]-48)*100 +(p[2]-48)*10 +(p[3]-48);
	p += 4;
	G_ScreenSet.FANHUI = G_ScreenSet.LIE >> 3;// һ�е����ֽ���
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
	
	G_ScreenSet.HBYTE = G_ScreenSet.HANG >> 3;//һ�е����ֽ��� 
	G_ScreenSet.BYTENUM = G_ScreenSet.HBYTE*G_ScreenSet.LIE;///������������ע��˴���������
	
	G_BoxSet.FANHUI = G_BoxSet.LIE >> 3;
	G_BoxSet.YANSE = G_ScreenSet.YANSE;
	G_BoxSet.BYTENUM = G_BoxSet.HBYTE*G_BoxSet.LIE;
	
	G_MoZuType = p[0]-48;  //ģ�����
	p += 1;
	
	G_BaudRate = (p[0]-48)*100000 + (p[1]-48)*10000 +(p[2]-48)*1000 +
	             (p[3]-48)*100 + (p[4]-48)*10 + (p[5]-48);//������
	p += 6;
	
	for(i=0;i<4;i++)    //IP��ַ
	{
    IPAddress[i] = (p[0]-48)*100 + (p[1]-48)*10 +(p[2]-48);
		p += 3;
  }
	for(i=0;i<4;i++)    //��������
	{
    NetMask[i] = (p[0]-48)*100 + (p[1]-48)*10 +(p[2]-48);
		p += 3;
  }
	for(i=0;i<4;i++)    //����
	{
    GetWay[i] = (p[0]-48)*100 + (p[1]-48)*10 +(p[2]-48);
		p += 3;
  }	
	
	LocalPort = (p[0]-48)*10000 + (p[1]-48)*1000 +(p[2]-48)*100 +
	             (p[3]-48)*10 + (p[4]-48);//UDP�˿�	
	

	G_SlaveNum = (G_ScreenSet.HBYTE/G_BoxSet.HBYTE)*(G_ScreenSet.LIE/G_BoxSet.LIE);
	
//	printf("\r\n ������ĵ�������ַ��%c%c",HostAddrH,HostAddrL);
//	printf("\r\n ������������:%d,�߶�:%d,��ɫ:%d",G_ScreenSet.HANG,
//	                         G_ScreenSet.LIE,G_ScreenSet.YANSE);
//	printf("\r\n �ӿ����򣺿��:%d,�߶�:%d,��ɫ:%d",G_BoxSet.HANG,
//	                         G_BoxSet.LIE,G_BoxSet.YANSE);
//	printf("\r\n ģ�����:%d",G_MoZuType);
//	
//	printf("\r\n ������:%d",G_BaudRate);
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
  * ����   : �洢���õ����塢�ӿ�����ģ����𡢲����ʡ�IP/MASK/GT/PORT��
  *		   
  * ����   : p:������ķ��͹�����Դ����ָ��
  *
  *
  * ����ֵ : ��
  ******************************************************************************
  */
void StoreDataSet (uint8_t *p,uint8_t len)
{
  uint8_t i;
	//�������/�ӿ����������/ģ��/IP����������ɱ�־λ
	G_DataSetFlag = 0;//��־λ����-MCU����������
	E2promWriteByte(EEROM_DATA_SET_FLAG_ADDR,G_DataSetFlag);
	for(i=0;i<69;i++)
	{
		E2promWriteByte(EEROM_DATA_SET_START_ADDR+i,p[i]);//д��ԭ���Ĳ���
	}
	p+=22;//����28���ֽ� ָ��IP��ַ
	for(i=0;i<6;i++)
		E2promWriteByte(EEROM_USART_BPS+i,p[i]);
	
	p+=6;
	//==========����IP===========//
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
	p+=12;//ָ����������
	p+=12;//ָ������
	
	//=========��������=========//
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
	p+=12;//ָ��˿�
	
	//=========���ö˿�===========//
	g_cPort = (p[0]-0x30)*10000;
	g_cPort += (p[1]-0x30)*1000;
	g_cPort += (p[2]-0x30)*100;
	g_cPort += (p[3]-0x30)*10;
	g_cPort += (p[4]-0x30);
	
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
 	G_DataSetFlag = 1;//��־λEEROM��д1-MCU����������
	E2promWriteByte(EEROM_DATA_SET_FLAG_ADDR,1);
}


void Usart_StoreDataSet(uint8_t *p,uint8_t len,uint8_t type)
{
  uint8_t i,k=0,num=0,m;
	//�������/�ӿ����������/ģ��/IP����������ɱ�־λ
	G_DataSetFlag = 0;//��־λ����-MCU����������
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
    for(m=0;m<2;m++) //����
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
    
     for(m=0;m<2;m++) //�Ӱ�
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








