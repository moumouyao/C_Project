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
	
extern short Get_Temprate(void);  //����Get_Temperate()����
extern void RTC_Get_Time(u8 *hour,u8 *min,u8 *sec,u8 *ampm); //����RTC_Get_Timer()����
extern void RTC_Get_Date(u8 *year,u8 *month,u8 *date,u8 *week); //����RTC_Get_Date()����

//����LED��CGI handler
const char* LEDS_CGI_Handler(int iIndex, int iNumParams, char *pcParam[], char *pcValue[]);
const char* BEEP_CGI_Handler(int iIndex,int iNumParams,char *pcParam[],char *pcValue[]);

static const char *ppcTAGs[]=  //SSI��Tag
{
	"p",//�˿ں�
	"w",//����
	"m",//ͨѶ��ʽ
	"t", //IP��ַ
	"a", //�豸��ַ
	"b", //��Ļ���
	"c",//��Ļ�߶�
	"d",//�ӿؿ��
	"e",//�ӿظ߶�
	"l",//���ȼ���
	"h",//������ȱ���
	"z",//�汾
	"r",//��ɫr
	"g",//��ɫg
	"b",//��ɫb
	"\0"
};


static const tCGI ppcURLs[]= //cgi����
{
	{"/leds.cgi",LEDS_CGI_Handler},
	{"/beep.cgi",BEEP_CGI_Handler},
};


//��web�ͻ��������������ʱ��,ʹ�ô˺�����CGI handler����
static int FindCGIParameter(const char *pcToFind,char *pcParam[],int iNumParams)
{
	int iLoop;
	for(iLoop = 0;iLoop < iNumParams;iLoop ++ )
	{
		if(strcmp(pcToFind,pcParam[iLoop]) == 0)
		{
			return (iLoop); //����iLOOP
		}
	}
	return (-1);
}


//=================SSIHandler����Ҫ�õ��Ĵ���IP�ĺ���==================//
void Port_Handler(char *pcInsert)//�˿ں�
{
	*pcInsert = (g_cPort/1000) +0x30;
	*(pcInsert+1) = (g_cPort%1000/100) +0x30;
	*(pcInsert+2) = (g_cPort%1000%100/10) +0x30;
	*(pcInsert+3) = (g_cPort%1000%100%10) +0x30;
	*(pcInsert+4) = '\0';
}
void Gateway_Handler(char *pcInsert)//����
{
	unsigned char i,j;
	unsigned char cData;
	unsigned char cDataLast;
	for(i=0,j=0;i<4;i++)//д��IP��ַ
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
		if(i<3)//���һλ���ӵ�
		{
			*(pcInsert+j) = '.';
			j++;
		}
	}
	*(pcInsert+j) = '\0';
}
void Mode_Handler(char *pcInsert)//ͨѶ��ʽ
{
	*pcInsert = TCPorUDPFlag==1?0x30:0x31;
	*(pcInsert+1) = '\0';
}
void IPaddr_Handler(char *pcInsert)
{
	unsigned char i,j;
	unsigned char cData;
	unsigned char cDataLast;
	for(i=0,j=0;i<4;i++)//д��IP��ַ
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
		if(i<3)//���һλ���ӵ�
		{
			*(pcInsert+j) = '.';
			j++;
		}
	}
	*(pcInsert+j) = '\0';
}

void SlaveAddr_Handler(char *pcInsert)//�ӻ���ַ��ʾ
{
	*pcInsert = HostAddrH;
	*(pcInsert+1) = HostAddrL;
	*(pcInsert+2) = '\0';
}

void SumWidth_Handler(char *pcInsert)//��Ļ���
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
void SumHigh_Handler(char *pcInsert)//��Ļ�߶�
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

void PartWidth_Handler(char *pcInsert)//�ӿؿ��
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
void PartHigh_Handler(char *pcInsert)//�ӿظ߶�
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
void LightLeve_Handler(char *pcInsert)//���ȼ���
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

void MaxLight_Handler(char *pcInsert)//������ȱ���
{
	*pcInsert = (max_bright_proportion/10) + 0x30;
	*(pcInsert+1) = (max_bright_proportion%10) + 0x30;
	*(pcInsert+2) = '\0';
}

void Edition_Handler(char *pcInsert)//�汾
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

void R_Handler(char *pcInsert)//��ɫr
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

void G_Handler(char *pcInsert)//��ɫG
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

void B_Handler(char *pcInsert)//��ɫB
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

//SSI��Handler���
static u16_t SSIHandler(int iIndex,char *pcInsert,int iInsertLen)
{
	switch(iIndex)
	{
		case 0: //�˿�
			Port_Handler(pcInsert);
		break;
		case 1://����
			Gateway_Handler(pcInsert);
		break;
		case 2://ͨѶ��ʽ
			Mode_Handler(pcInsert);
		break;
		case 3://IP��ַ
			IPaddr_Handler(pcInsert);
		break;
		case 4://�豸��ַ
			SlaveAddr_Handler(pcInsert);
		break;
		case 5://��Ļ���
			SumWidth_Handler(pcInsert);
		break;
		case 6://��Ļ�߶�
			SumHigh_Handler(pcInsert);
		break;
		case 7://�ӿؿ��
			PartWidth_Handler(pcInsert);
		break;
		case 8://�ӿظ߶�
			PartHigh_Handler(pcInsert);
		break;
		case 9://���ȼ���
			LightLeve_Handler(pcInsert);
		break;
		case 10://������ȱ���
			MaxLight_Handler(pcInsert);
		break;
		case 11://�汾
			Edition_Handler(pcInsert);
		break;
		case 12://��ɫR
			R_Handler(pcInsert);
		break;
		case 13://��ɫG
			G_Handler(pcInsert);
		break;
		case 14://��ɫB
			B_Handler(pcInsert);
		break;
	}
	return strlen(pcInsert);
}
//================================================CGI����====================================================//
//CGI LED���ƾ��
const char* LEDS_CGI_Handler(int iIndex, int iNumParams, char *pcParam[], char *pcValue[])
{

	return "/index.shtml";			
}

//BEEP��CGI���ƾ��
const char *BEEP_CGI_Handler(int iIndex,int iNumParams,char *pcParam[],char *pcValue[])
{
	u8 i=0;
	unsigned char j,k;
	//=========================�˿ں�����====================================//
	iIndex = FindCGIParameter("port",pcParam,iNumParams);  //�ҵ�������
	if(iIndex != -1) //�ҵ�������
	{
		for(i = 0;i < iNumParams;i++)
		{
			if(strcmp(pcParam[i],"port") == 0)  //����CGI����
			{
				g_cPort = 0;//������
				for(k=0;(pcValue[i][k]!='.')&&(pcValue[i][k]!='\0');k++)
				{
					g_cPort *= 10;//����10��
					g_cPort += (pcValue[i][k]-0x30);
				}
			}
		}
		//======д��EEPROM
		E2promWriteByte(EEROM_Port_ADDR,g_cPort>>8);//�����λ
		E2promWriteByte(EEROM_Port_ADDR+1,g_cPort);//�����λ
//		printf("д��Ķ˿ں�Ϊ%d\r\n",g_cPort);
		g_cSysRst = 1;//��λ
	}
	
	//==========================��������=====================================//
	iIndex = FindCGIParameter("gateway",pcParam,iNumParams);  //�ҵ�������
	if(iIndex != -1) //�ҵ�������
	{
		for(i = 0;i < iNumParams;i++)
		{
			if(strcmp(pcParam[i],"gateway") == 0)  //����CGI����
			{
				for(j=0,k=0;j<4;j++)//д��Ҫ�޸ĵ�IP��ַ
				{
					g_cGateway[j] = 0;//������
					for(k=k;(pcValue[i][k]!='.')&&(pcValue[i][k]!='\0');k++)
					{
						g_cGateway[j] *= 10;//����10��
						g_cGateway[j] += (pcValue[i][k]-0x30);
					}
					k++;//����'.'
				}
			}
		}
		//==========д��EEPROM
		for(j=0;j<4;j++)
		{
			E2promWriteByte(EEROM_Gateway_ADDR+j,g_cGateway[j]);
		}
		g_cSysRst = 1;//��λ
	}
	
	//==========================ģʽ����====================================//
	iIndex = FindCGIParameter("mode",pcParam,iNumParams);  //�ҵ�������
	if(iIndex != -1) //�ҵ�������
	{
		for(i = 0;i < iNumParams;i++)
		{
			if(strcmp(pcParam[i],"mode") == 0)  //����CGI����
			{
				TCPorUDPFlag = 0;//������
				for(k=0;(pcValue[i][k]!='.')&&(pcValue[i][k]!='\0');k++)
				{
					TCPorUDPFlag *= 10;//����10��
					TCPorUDPFlag += (pcValue[i][k]-0x30);
				}
			}
		}
		TCPorUDPFlag = TCPorUDPFlag==1?0:1;//������
		//======д��EEPROM
//		printf("д���ģʽΪ%d\r\n",TCPorUDPFlag);
		E2promWriteByte(EEROM_TCP_OR_UDP_ADDR,TCPorUDPFlag);
		g_cSysRst = 1;//��λ
	}
	
	//==========================IP����=======================================//
	iIndex = FindCGIParameter("ip",pcParam,iNumParams);  //�ҵ�ip��������
	if(iIndex != -1) //�ҵ�������
	{
		for(i = 0;i < iNumParams;i++)
		{
			if(strcmp(pcParam[i],"ip") == 0)  //����CGI����
			{
				for(j=0,k=0;j<4;j++)//д��Ҫ�޸ĵ�IP��ַ
				{
					g_cIp[j] = 0;//������
					for(k=k;(pcValue[i][k]!='.')&&(pcValue[i][k]!='\0');k++)
					{
						g_cIp[j] *= 10;//����10��
						g_cIp[j] += (pcValue[i][k]-0x30);
					}
					k++;//����'.'
				}
//				printf("�޸ĺ��IP��ַΪ%d.%d.%d.%d\r\n",g_cIp[0],g_cIp[1],g_cIp[2],g_cIp[3]);//����޸ĺ��IP��ַ
			}
		}
		//=======д��EEPROM
		for(j=0;j<4;j++)
		{
			E2promWriteByte(EEROM_Local_IP_ADDR+j,g_cIp[j]);
		}
		g_cSysRst = 1;//��λ
	}
	
	//=========================������ť============================//
	iIndex = FindCGIParameter("power",pcParam,iNumParams);  //�ҵ�ip��������
	if(iIndex != -1) //�ҵ�������
	{
		for(i = 0;i < iNumParams;i++)
		{
			if(strcmp(pcParam[i],"power") == 1)  //����CGI����
			{
//				printf("�����ɹ�\r\n");//����޸ĺ��IP��ַ
			}
		}
		g_cSysRst = 1;//��λ
	}
	return "/index.shtml";
}

//SSI�����ʼ��
void httpd_ssi_init(void)
{  
	//�����ڲ��¶ȴ�������SSI���
	http_set_ssi_handler(SSIHandler,ppcTAGs,NUM_CONFIG_SSI_TAGS);
}

//CGI�����ʼ��
void httpd_cgi_init(void)
{ 
  //����CGI���LEDs control CGI) */
  http_set_cgi_handlers(ppcURLs, NUM_CONFIG_CGI_URIS);
}


