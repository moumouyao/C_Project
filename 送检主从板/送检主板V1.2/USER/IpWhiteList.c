#include "IpWhiteList.h"
#include "sw_global.h"
#include "sw_crc16.h"
#include "i2c.h"
void ExtractWhiteList(StrFrameHeader *frame_head)//写入白名单
{
	unsigned char i;//白名单的个数
	unsigned char j;//IP地址位 0-11
	unsigned char k;//辅助cData寻址
	unsigned char *cData;//报文数据指针 指向报文所包含的数据（剔除帧头和命令）
	cData = &frame_head->pdata[5];//指向第一个IP地址的第一位
	for(i=0,k=0;i<WhiteListNum;i++)//IP地址数
	{
		for(j=0;j<4;j++)//IP地址位数
		{
			if((cData[i*12] == '1') || (cData[i*12] == '0'))//判断当前指向的IP地址是否是帧尾
			{
				g_cIpWhiteList[i][j] =  (cData[k]-0x30)*100;//字符转数
				k++;
				g_cIpWhiteList[i][j] += (cData[k]-0x30)*10;
				k++;
				g_cIpWhiteList[i][j] += (cData[k]-0x30);
				k++;
			}
			else
			{
				break;//跳出j层
			}
		}
		if(j!=4)//j!=4代表j层是被动跳出的
		{
			break;//跳出i层
		}
	}
	for(i=i;i<WhiteListNum;i++)//清除没有写入的IP地址
	{
		for(j=0;j<4;j++)
		{
			g_cIpWhiteList[i][j] = 0;
		}
	}
	WirteEEROMwhite();//将白名单写入EEROAM
}
void WirteEEROMwhite()
{
	E2promWriteByte(EEROM_White_1,g_cIpWhiteList[0][0]);
	E2promWriteByte(EEROM_White_1+1,g_cIpWhiteList[0][1]);
	E2promWriteByte(EEROM_White_1+2,g_cIpWhiteList[0][2]);
	E2promWriteByte(EEROM_White_1+3,g_cIpWhiteList[0][3]);
	
	E2promWriteByte(EEROM_White_2,g_cIpWhiteList[1][0]);
	E2promWriteByte(EEROM_White_2+1,g_cIpWhiteList[1][1]);
	E2promWriteByte(EEROM_White_2+2,g_cIpWhiteList[1][2]);
	E2promWriteByte(EEROM_White_2+3,g_cIpWhiteList[1][3]);
	
	E2promWriteByte(EEROM_White_3,g_cIpWhiteList[2][0]);
	E2promWriteByte(EEROM_White_3+1,g_cIpWhiteList[2][1]);
	E2promWriteByte(EEROM_White_3+2,g_cIpWhiteList[2][2]);
	E2promWriteByte(EEROM_White_3+3,g_cIpWhiteList[2][3]);
	
	E2promWriteByte(EEROM_White_4,g_cIpWhiteList[3][0]);
	E2promWriteByte(EEROM_White_4+1,g_cIpWhiteList[3][1]);
	E2promWriteByte(EEROM_White_4+2,g_cIpWhiteList[3][2]);
	E2promWriteByte(EEROM_White_4+3,g_cIpWhiteList[3][3]);

	E2promWriteByte(EEROM_White_5,g_cIpWhiteList[4][0]);
	E2promWriteByte(EEROM_White_5+1,g_cIpWhiteList[4][1]);
	E2promWriteByte(EEROM_White_5+2,g_cIpWhiteList[4][2]);
	E2promWriteByte(EEROM_White_5+3,g_cIpWhiteList[4][3]);
}
void WhiteListACK(unsigned char* p,unsigned short*len)//写入成功回复
{
	unsigned char *p1;//指向P,用于计算数据总长度
	unsigned short crc16;
	p1 = p;
	//=====写入报文
	*p++ = 0x02;         //帧头
	*p++ = HostAddrH;    //主控制器地址高位
	*p++ = HostAddrL;    //主控制器地址低位
	*p++ = '0';
	//=======CRC检验
	crc16 = SWCRC16(p1+1,p-p1-1);
	*p++ = (uint8_t)(crc16>>8); //校验高字节在前
	*p++ = (uint8_t)(crc16);    //校验低字节在后
	*p++ = 0x03;              //帧尾
  *len = p-p1;   //数据个数：帧头+地址+数据+crc+帧未
}

void WhiteListACKdata(unsigned char* p,unsigned short*len)//读取IP白名单
{
	unsigned char *p1;//指向P,用于计算数据总长度
	unsigned short crc16;
	unsigned char i,j;
	p1 = p;
	//=====写入报文
	*p++ = 0x02;         //帧头
	*p++ = HostAddrH;    //主控制器地址高位
	*p++ = HostAddrL;    //主控制器地址低位
	//=====写入IP地址
	for(i=0;i<WhiteListNum;i++)
	{
		for(j=0;j<4;j++)
		{
			if(g_cIpWhiteList[i][0] != 0)//该IP地址不是空的
			{
				*p++ = (g_cIpWhiteList[i][j]/100)    + 0x30;
				*p++ = (g_cIpWhiteList[i][j]%100/10) + 0x30;
				*p++ = (g_cIpWhiteList[i][j]%100%10) + 0x30;
			}
			else
			{
				break;//跳出j层
			}
		}
		if(j!=4)
		{
			break;//跳出i层
		}
	}
	//=======CRC检验
	crc16 = SWCRC16(p1+1,p-p1-1);
	*p++ = (uint8_t)(crc16>>8); //校验高字节在前
	*p++ = (uint8_t)(crc16);    //校验低字节在后
	*p++ = 0x03;              //帧尾
  *len = p-p1;   //数据个数：帧头+地址+数据+crc+帧未
}


