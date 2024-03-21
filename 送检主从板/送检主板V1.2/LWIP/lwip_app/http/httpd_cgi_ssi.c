#include "lwip/debug.h"
#include "httpd.h"
#include "lwip/tcp.h"
#include "fs.h"
#include "lwip_comm.h"
#include "sw_global.h"
#include "tcp_server_demo.h"
#include "os_cpu.h"
#include "delay.h"
#include "i2c.h"
#include "sw_global.h"

#include <string.h>
#include <stdlib.h>

#define NUM_CONFIG_CGI_URIS	(sizeof(ppcURLs) / sizeof(tCGI))
#define NUM_CONFIG_SSI_TAGS	(sizeof(ppcTAGs) / sizeof(char *))
	
extern short Get_Temprate(void);  //声明Get_Temperate()函数
extern void RTC_Get_Time(u8 *hour,u8 *min,u8 *sec,u8 *ampm); //声明RTC_Get_Timer()函数
extern void RTC_Get_Date(u8 *year,u8 *month,u8 *date,u8 *week); //声明RTC_Get_Date()函数

//控制LED的CGI handler
const char* LEDS_CGI_Handler(int iIndex, int iNumParams, char *pcParam[], char *pcValue[]);
const char* BEEP_CGI_Handler(int iIndex,int iNumParams,char *pcParam[],char *pcValue[]);

static const char *ppcTAGs[]=  //SSI的Tag
{
	"p",//端口号
	"w",//网关
	"m",//通讯方式
	"t", //IP地址
	"a", //设备地址
	"b", //屏幕宽度
	"c",//屏幕高度
	"d",//从控宽度
	"e",//从控高度
	"l",//亮度级别
	"h",//最大亮度比例
	"z",//版本
	"r",//颜色r
	"g",//颜色g
	"b",//颜色b
	"\0"
};


static const tCGI ppcURLs[]= //cgi程序
{
	{"/leds.cgi",LEDS_CGI_Handler},
	{"/beep.cgi",BEEP_CGI_Handler},
};


//当web客户端请求浏览器的时候,使用此函数被CGI handler调用
static int FindCGIParameter(const char *pcToFind,char *pcParam[],int iNumParams)
{
	int iLoop;
	for(iLoop = 0;iLoop < iNumParams;iLoop ++ )
	{
		if(strcmp(pcToFind,pcParam[iLoop]) == 0)
		{
			return (iLoop); //返回iLOOP
		}
	}
	return (-1);
}


//=================SSIHandler中需要用到的处理IP的函数==================//
void Port_Handler(char *pcInsert)//端口号
{
	*pcInsert = (g_cPort/1000) +0x30;
	*(pcInsert+1) = (g_cPort%1000/100) +0x30;
	*(pcInsert+2) = (g_cPort%1000%100/10) +0x30;
	*(pcInsert+3) = (g_cPort%1000%100%10) +0x30;
	*(pcInsert+4) = '\0';
}
void Gateway_Handler(char *pcInsert)//网关
{
	unsigned char i,j;
	unsigned char cData;
	unsigned char cDataLast;
	for(i=0,j=0;i<4;i++)//写入IP地址
	{
		cDataLast = 0;
		cData = lwipdev.gateway[i]/100;
		if(cData != 0)
		{
			*(pcInsert+j) = cData+0x30;
			j++;
		}
		cDataLast = cData;
		cData = lwipdev.gateway[i]%100/10;
		if((cData!=0) || (cDataLast!=0))
		{
			*(pcInsert+j) = cData+0x30;
			j++;
		}
		cDataLast = cData;
		cData = lwipdev.gateway[i]%100%10;
		if((cData!=0) || (cDataLast!=0))
		{
			*(pcInsert+j) = cData+0x30;
			j++;
		}
		if(i<3)//最后一位不加点
		{
			*(pcInsert+j) = '.';
			j++;
		}
	}
	*(pcInsert+j) = '\0';
}
void Mode_Handler(char *pcInsert)//通讯方式
{
	*pcInsert = TCPorUDPFlag==1?0x30:0x31;
	*(pcInsert+1) = '\0';
}
void IPaddr_Handler(char *pcInsert)
{
	unsigned char i,j;
	unsigned char cData;
	unsigned char cDataLast;
	for(i=0,j=0;i<4;i++)//写入IP地址
	{
		cDataLast = 0;
		cData = lwipdev.ip[i]/100;
		if(cData != 0)
		{
			*(pcInsert+j) = cData+0x30;
			j++;
		}
		cDataLast = cData;
		cData = lwipdev.ip[i]%100/10;
		if((cData!=0) || (cDataLast!=0))
		{
			*(pcInsert+j) = cData+0x30;
			j++;
		}
		cDataLast = cData;
		cData = lwipdev.ip[i]%100%10;
		if((cData!=0) || (cDataLast!=0))
		{
			*(pcInsert+j) = cData+0x30;
			j++;
		}
		if(i<3)//最后一位不加点
		{
			*(pcInsert+j) = '.';
			j++;
		}
	}
	*(pcInsert+j) = '\0';
}

void SlaveAddr_Handler(char *pcInsert)//从机地址显示
{
	*pcInsert = HostAddrH;
	*(pcInsert+1) = HostAddrL;
	*(pcInsert+2) = '\0';
}

void SumWidth_Handler(char *pcInsert)//屏幕宽度
{
	unsigned char cData;
	unsigned char i=0;
	cData = G_ScreenSet.HANG/100;
	if(cData != 0)
	{
		*(pcInsert+i) = cData + 0x30;
		i++;
	}
	cData = G_ScreenSet.HANG%100/10;
	*(pcInsert+i) = cData + 0x30;
	i++;
	cData = G_ScreenSet.HANG%100%10;
	*(pcInsert+i) = cData + 0x30;
	i++;
	*(pcInsert+i) = '\0';
}
void SumHigh_Handler(char *pcInsert)//屏幕高度
{
	unsigned char cData;
	unsigned char i=0;
	cData = G_ScreenSet.LIE/100;
	if(cData != 0)
	{
		*(pcInsert+i) = cData + 0x30;
		i++;
	}
	cData = G_ScreenSet.LIE%100/10;
	*(pcInsert+i) = cData + 0x30;
	i++;
	cData = G_ScreenSet.LIE%100%10;
	*(pcInsert+i) = cData + 0x30;
	i++;
	*(pcInsert+i) = '\0';
}

void PartWidth_Handler(char *pcInsert)//从控宽度
{
	unsigned char cData;
	unsigned char i=0;
	cData = G_BoxSet.HANG/100;
	if(cData != 0)
	{
		*(pcInsert+i) = cData + 0x30;
		i++;
	}
	cData = G_BoxSet.HANG%100/10;
	*(pcInsert+i) = cData + 0x30;
	i++;
	cData = G_BoxSet.HANG%100%10;
	*(pcInsert+i) = cData + 0x30;
	i++;
	*(pcInsert+i) = '\0';
}
void PartHigh_Handler(char *pcInsert)//从控高度
{
	unsigned char cData;
	unsigned char i=0;
	cData = G_BoxSet.LIE/100;
	if(cData != 0)
	{
		*(pcInsert+i) = cData + 0x30;
		i++;
	}
	cData = G_BoxSet.LIE%100/10;
	*(pcInsert+i) = cData + 0x30;
	i++;
	cData = G_BoxSet.LIE%100%10;
	*(pcInsert+i) = cData + 0x30;
	i++;
	*(pcInsert+i) = '\0';
}
extern uint8_t  bright_level_flag;
void LightLeve_Handler(char *pcInsert)//亮度级别
{
	if(bright_level_flag==0)
	{
	*pcInsert = '3';
	*(pcInsert+1) = '2';
	*(pcInsert+2) = '\0';	
	}else 	if(bright_level_flag==1)
	{
	*pcInsert = '6';
	*(pcInsert+1) = '4';
	*(pcInsert+2) = '\0';	
	}
	else 	if(bright_level_flag==2)
	{
	*pcInsert = '1';
	*(pcInsert+1) = '0';
	*(pcInsert+2) = '2';	
	*(pcInsert+3) = '4';
	*(pcInsert+4) = '\0';
	}

}

void MaxLight_Handler(char *pcInsert)//最大亮度比例
{
	*pcInsert = (max_bright_proportion/10) + 0x30;
	*(pcInsert+1) = (max_bright_proportion%10) + 0x30;
	*(pcInsert+2) = '\0';
}

void Edition_Handler(char *pcInsert)//版本
{
	#if EnableCode
	*(pcInsert) = 'V';
	*(pcInsert+1) = '1';
	*(pcInsert+2) = '.';
	*(pcInsert+3) = '5';
	*(pcInsert+4) = '4';
	*(pcInsert+5) = 'S';
	*(pcInsert+6) = '\0';
#else
	*(pcInsert) = 'V';
	*(pcInsert+1) = '1';
	*(pcInsert+2) = '.';
	*(pcInsert+3) = '5';
	*(pcInsert+4) = '4';
	*(pcInsert+5) = '\0';
#endif
}

void R_Handler(char *pcInsert)//颜色r
{
	unsigned char cData;
	cData = G_ScreenSet.YANSE;
	if(cData==1 || cData==2 || cData==3)//R
	{
		*pcInsert = '1';
		*(pcInsert+1) = '\0';
	}
	else
	{
		*pcInsert = '0';
		*(pcInsert+1) = '\0';
	}
}

void G_Handler(char *pcInsert)//颜色G
{
	unsigned char cData;
	cData = G_ScreenSet.YANSE;
	if(cData==2 || cData==3)//G
	{
		*pcInsert = '1';
		*(pcInsert+1) = '\0';
	}
	else
	{
		*pcInsert = '0';
		*(pcInsert+1) = '\0';
	}
}

void B_Handler(char *pcInsert)//颜色B
{
	unsigned char cData;
	cData = G_ScreenSet.YANSE;
	if(cData==3)//B
	{
		*pcInsert = '1';
		*(pcInsert+1) = '\0';
	}
	else
	{
		*pcInsert = '0';
		*(pcInsert+1) = '\0';
	}
}

//SSI的Handler句柄
static u16_t SSIHandler(int iIndex,char *pcInsert,int iInsertLen)
{
	switch(iIndex)
	{
		case 0: //端口
			Port_Handler(pcInsert);
		break;
		case 1://网关
			Gateway_Handler(pcInsert);
		break;
		case 2://通讯方式
			Mode_Handler(pcInsert);
		break;
		case 3://IP地址
			IPaddr_Handler(pcInsert);
		break;
		case 4://设备地址
			SlaveAddr_Handler(pcInsert);
		break;
		case 5://屏幕宽度
			SumWidth_Handler(pcInsert);
		break;
		case 6://屏幕高度
			SumHigh_Handler(pcInsert);
		break;
		case 7://从控宽度
			PartWidth_Handler(pcInsert);
		break;
		case 8://从控高度
			PartHigh_Handler(pcInsert);
		break;
		case 9://亮度级别
			LightLeve_Handler(pcInsert);
		break;
		case 10://最大亮度比例
			MaxLight_Handler(pcInsert);
		break;
		case 11://版本
			Edition_Handler(pcInsert);
		break;
		case 12://颜色R
			R_Handler(pcInsert);
		break;
		case 13://颜色G
			G_Handler(pcInsert);
		break;
		case 14://颜色B
			B_Handler(pcInsert);
		break;
	}
	return strlen(pcInsert);
}
//================================================CGI控制====================================================//
//CGI LED控制句柄
const char* LEDS_CGI_Handler(int iIndex, int iNumParams, char *pcParam[], char *pcValue[])
{

	return "/index.shtml";			
}

//BEEP的CGI控制句柄
const char *BEEP_CGI_Handler(int iIndex,int iNumParams,char *pcParam[],char *pcValue[])
{
	u8 i=0;
	unsigned char j,k;
	//=========================端口号设置====================================//
	iIndex = FindCGIParameter("port",pcParam,iNumParams);  //找到索引号
	if(iIndex != -1) //找到索引号
	{
		for(i = 0;i < iNumParams;i++)
		{
			if(strcmp(pcParam[i],"port") == 0)  //查找CGI参数
			{
				g_cPort = 0;//先清零
				for(k=0;(pcValue[i][k]!='.')&&(pcValue[i][k]!='\0');k++)
				{
					g_cPort *= 10;//扩大10倍
					g_cPort += (pcValue[i][k]-0x30);
				}
			}
		}
		//======写入EEPROM
		E2promWriteByte(EEROM_Port_ADDR,g_cPort>>8);//保存高位
		E2promWriteByte(EEROM_Port_ADDR+1,g_cPort);//保存地位
//		printf("写入的端口号为%d\r\n",g_cPort);
		g_cSysRst = 1;//复位
	}
	
	//==========================网关设置=====================================//
	iIndex = FindCGIParameter("gateway",pcParam,iNumParams);  //找到索引号
	if(iIndex != -1) //找到索引号
	{
		for(i = 0;i < iNumParams;i++)
		{
			if(strcmp(pcParam[i],"gateway") == 0)  //查找CGI参数
			{
				for(j=0,k=0;j<4;j++)//写入要修改的IP地址
				{
					g_cGateway[j] = 0;//先清零
					for(k=k;(pcValue[i][k]!='.')&&(pcValue[i][k]!='\0');k++)
					{
						g_cGateway[j] *= 10;//扩大10倍
						g_cGateway[j] += (pcValue[i][k]-0x30);
					}
					k++;//跳过'.'
				}
			}
		}
		//==========写入EEPROM
		for(j=0;j<4;j++)
		{
			E2promWriteByte(EEROM_Gateway_ADDR+j,g_cGateway[j]);
		}
		g_cSysRst = 1;//复位
	}
	
	//==========================模式设置====================================//
	iIndex = FindCGIParameter("mode",pcParam,iNumParams);  //找到索引号
	if(iIndex != -1) //找到索引号
	{
		for(i = 0;i < iNumParams;i++)
		{
			if(strcmp(pcParam[i],"mode") == 0)  //查找CGI参数
			{
				TCPorUDPFlag = 0;//先清零
				for(k=0;(pcValue[i][k]!='.')&&(pcValue[i][k]!='\0');k++)
				{
					TCPorUDPFlag *= 10;//扩大10倍
					TCPorUDPFlag += (pcValue[i][k]-0x30);
				}
			}
		}
		TCPorUDPFlag = TCPorUDPFlag==1?0:1;//反相器
		//======写入EEPROM
//		printf("写入的模式为%d\r\n",TCPorUDPFlag);
		E2promWriteByte(EEROM_TCP_OR_UDP_ADDR,TCPorUDPFlag);
		g_cSysRst = 1;//复位
	}
	
	//==========================IP设置=======================================//
	iIndex = FindCGIParameter("ip",pcParam,iNumParams);  //找到ip的索引号
	if(iIndex != -1) //找到索引号
	{
		for(i = 0;i < iNumParams;i++)
		{
			if(strcmp(pcParam[i],"ip") == 0)  //查找CGI参数
			{
				for(j=0,k=0;j<4;j++)//写入要修改的IP地址
				{
					g_cIp[j] = 0;//先清零
					for(k=k;(pcValue[i][k]!='.')&&(pcValue[i][k]!='\0');k++)
					{
						g_cIp[j] *= 10;//扩大10倍
						g_cIp[j] += (pcValue[i][k]-0x30);
					}
					k++;//跳过'.'
				}
//				printf("修改后的IP地址为%d.%d.%d.%d\r\n",g_cIp[0],g_cIp[1],g_cIp[2],g_cIp[3]);//输出修改后的IP地址
			}
		}
		//=======写入EEPROM
		for(j=0;j<4;j++)
		{
			E2promWriteByte(EEROM_Local_IP_ADDR+j,g_cIp[j]);
		}
		g_cSysRst = 1;//复位
	}
	
	//=========================重启按钮============================//
	iIndex = FindCGIParameter("power",pcParam,iNumParams);  //找到ip的索引号
	if(iIndex != -1) //找到索引号
	{
		for(i = 0;i < iNumParams;i++)
		{
			if(strcmp(pcParam[i],"power") == 1)  //查找CGI参数
			{
//				printf("重启成功\r\n");//输出修改后的IP地址
			}
		}
		g_cSysRst = 1;//复位
	}
	return "/index.shtml";
}

//SSI句柄初始化
void httpd_ssi_init(void)
{  
	//配置内部温度传感器的SSI句柄
	http_set_ssi_handler(SSIHandler,ppcTAGs,NUM_CONFIG_SSI_TAGS);
}

//CGI句柄初始化
void httpd_cgi_init(void)
{ 
  //配置CGI句柄LEDs control CGI) */
  http_set_cgi_handlers(ppcURLs, NUM_CONFIG_CGI_URIS);
}


