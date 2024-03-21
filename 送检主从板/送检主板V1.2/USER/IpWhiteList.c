#include "IpWhiteList.h"
#include "sw_global.h"
#include "sw_crc16.h"
#include "i2c.h"
void ExtractWhiteList(StrFrameHeader *frame_head)//д�������
{
	unsigned char i;//�������ĸ���
	unsigned char j;//IP��ַλ 0-11
	unsigned char k;//����cDataѰַ
	unsigned char *cData;//��������ָ�� ָ���������������ݣ��޳�֡ͷ�����
	cData = &frame_head->pdata[5];//ָ���һ��IP��ַ�ĵ�һλ
	for(i=0,k=0;i<WhiteListNum;i++)//IP��ַ��
	{
		for(j=0;j<4;j++)//IP��ַλ��
		{
			if((cData[i*12] == '1') || (cData[i*12] == '0'))//�жϵ�ǰָ���IP��ַ�Ƿ���֡β
			{
				g_cIpWhiteList[i][j] =  (cData[k]-0x30)*100;//�ַ�ת��
				k++;
				g_cIpWhiteList[i][j] += (cData[k]-0x30)*10;
				k++;
				g_cIpWhiteList[i][j] += (cData[k]-0x30);
				k++;
			}
			else
			{
				break;//����j��
			}
		}
		if(j!=4)//j!=4����j���Ǳ���������
		{
			break;//����i��
		}
	}
	for(i=i;i<WhiteListNum;i++)//���û��д���IP��ַ
	{
		for(j=0;j<4;j++)
		{
			g_cIpWhiteList[i][j] = 0;
		}
	}
	WirteEEROMwhite();//��������д��EEROAM
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
void WhiteListACK(unsigned char* p,unsigned short*len)//д��ɹ��ظ�
{
	unsigned char *p1;//ָ��P,���ڼ��������ܳ���
	unsigned short crc16;
	p1 = p;
	//=====д�뱨��
	*p++ = 0x02;         //֡ͷ
	*p++ = HostAddrH;    //����������ַ��λ
	*p++ = HostAddrL;    //����������ַ��λ
	*p++ = '0';
	//=======CRC����
	crc16 = SWCRC16(p1+1,p-p1-1);
	*p++ = (uint8_t)(crc16>>8); //У����ֽ���ǰ
	*p++ = (uint8_t)(crc16);    //У����ֽ��ں�
	*p++ = 0x03;              //֡β
  *len = p-p1;   //���ݸ�����֡ͷ+��ַ+����+crc+֡δ
}

void WhiteListACKdata(unsigned char* p,unsigned short*len)//��ȡIP������
{
	unsigned char *p1;//ָ��P,���ڼ��������ܳ���
	unsigned short crc16;
	unsigned char i,j;
	p1 = p;
	//=====д�뱨��
	*p++ = 0x02;         //֡ͷ
	*p++ = HostAddrH;    //����������ַ��λ
	*p++ = HostAddrL;    //����������ַ��λ
	//=====д��IP��ַ
	for(i=0;i<WhiteListNum;i++)
	{
		for(j=0;j<4;j++)
		{
			if(g_cIpWhiteList[i][0] != 0)//��IP��ַ���ǿյ�
			{
				*p++ = (g_cIpWhiteList[i][j]/100)    + 0x30;
				*p++ = (g_cIpWhiteList[i][j]%100/10) + 0x30;
				*p++ = (g_cIpWhiteList[i][j]%100%10) + 0x30;
			}
			else
			{
				break;//����j��
			}
		}
		if(j!=4)
		{
			break;//����i��
		}
	}
	//=======CRC����
	crc16 = SWCRC16(p1+1,p-p1-1);
	*p++ = (uint8_t)(crc16>>8); //У����ֽ���ǰ
	*p++ = (uint8_t)(crc16);    //У����ֽ��ں�
	*p++ = 0x03;              //֡β
  *len = p-p1;   //���ݸ�����֡ͷ+��ַ+����+crc+֡δ
}


