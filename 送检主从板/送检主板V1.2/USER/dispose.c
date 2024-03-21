#include "dispose.h"
#include "stdio.h"
#include "sw_global.h"
#include "ff.h"
#include "rtc.h"
#include "i2c.h"
#include "dis_app.h"

//========================2022-07-29���������ļ������ڴ�������Ĺ���==============================//
/*
���ܣ���ȡ�����е��ļ�����Ϣ

*/
unsigned char *getFileName(unsigned char *cmd)
{
	unsigned char *cData;
	unsigned char cNameLong;//���ֳ���
	unsigned char i;
	for(cNameLong=0;cmd[5+cNameLong]!='+'&& cNameLong<200;cNameLong++);//�����ļ�������
	cData = mymalloc(SRAMEX,cNameLong+1);//�����ڴ�
	for(i=0;i<cNameLong;i++)
	{
		*(cData+i) = cmd[5+i];//��ȡ�ļ���
	}
	*(cData+i) = '\0';//д�������
	if(i==200)//�Ҳ����ļ���
	{
		myfree(SRAMEX,cData);//�ͷ��ļ����ڴ�
	}
	return i==200?NULL:cData;//���i=200�����������󷵻ؿ�ָ��
}
/*
���ܣ���ȡ�����е��ļ�ƫ�Ƶ�ַ
*/
unsigned int getFileAddr(unsigned char *cmd)
{
	unsigned long int iLong=0;
	unsigned int i=0;
	while(1)
	{
		if(cmd[i] == '+')//Ѱ�ұ�ʾ
		{
			i++;//ָ���ַ����
			iLong = cmd[i]<<24 | cmd[i+1]<<16 | cmd[i+2]<<8 | cmd[i+3];
			break;
		}
		else 
		{
			i++;
		}
	}
	return iLong;
}	

void RecordLog(StrFrameHeader *frame_head,unsigned char cCommand)//��¼��־
{
	unsigned int i;
	FRESULT res;
	FIL fnew;//�ļ�ָ��
	FATFS fs;
	TIME time;
	INT8U err;
	unsigned char *cFileData;//�ļ�����
	unsigned int iFileLong;//�ļ�����
	unsigned int bw;
	unsigned char cTime[] = "2000-00-00 00:00:00:";//ʱ������
	unsigned char cFileName[] = "2000-00.txt";//��־�ļ�������-��
	unsigned char cIP[18]={0};
	char cFileWay[20]={0};
	getTime();//��ȡʱ��
	time = g_tTime;
	int jbk=0;
	BYTE file_name[20];//�ϴ�Ԥ�Ʊ�����
	{//д��ʱ������
		cTime[2] = time.year/10 + 0x30;
		cTime[3] = time.year%10 + 0x30;
		cTime[5] = time.month/10 + 0x30;
		cTime[6] = time.month%10 + 0x30;
		cTime[8] = time.day/10 + 0x30;
		cTime[9] = time.day%10 + 0x30;
		cTime[11] = time.hour/10 + 0x30;
		cTime[12] = time.hour%10 + 0x30;
		cTime[14] = time.min/10 + 0x30;
		cTime[15] = time.min%10 + 0x30;
		cTime[17] = time.sec/10 + 0x30;
		cTime[18] = time.sec%10 + 0x30;
		//д���ļ���
		cFileName[2] = cTime[2];
		cFileName[3] = cTime[3];
		cFileName[5] = cTime[5];
		cFileName[6] = cTime[6];
	}
	if(cCommand==10){
			if((frame_head->pdata[14] >= '0' && frame_head->pdata[14] <= '9'))
			{
				for(i=0;frame_head->pdata[5+i]!='+';i++)
				file_name[i] = frame_head->pdata[5+i];
				cCommand=4;
			}else{
					for(i=0;(i<200)&&(frame_head->pdata[i]!='+');i++);//��λ�ļ���ĩβ
					i = i+1;//֡ͷ����
					i = i+4;//ƫ����4Byte
					iFileLong = frame_head->len - i - 3;//�ļ����� = �ܳ���-֡ͷ����-֡β����-ƫ����
					cFileData = mymalloc(SRAMEX,iFileLong);//�����ڴ�
					for(int j=0;j<iFileLong;j++)
					{
						cFileData[j] = frame_head->pdata[i+j];//�����ļ�����
					}
					}
	}
	sprintf((char*)cFileWay,"0:journal/%s",cFileName);   //�ļ����洢
	OSSemPend(SDSem,0,&err);
	f_mount(0,&fs);//����
	
	while(1)
	{
		res  = f_open(&fnew,cFileWay,FA_WRITE);//ֻд�ķ�ʽ���ļ�
		if(res != FR_OK)
		{
			res  = f_open(&fnew,cFileWay,FA_CREATE_ALWAYS | FA_WRITE);//����һ���ļ�
			if(res == FR_OK)//�����ɹ�
			{
				f_close(&fnew);//�ر��ĵ�
			}
			else//����ʧ��
			{
				break;
			}
		}
		else//�򿪳ɹ�
		{
			res = f_lseek(&fnew, fnew.fsize);//��ָ���ƶ����ĵ�ĩβ
			sprintf((char*)cIP," %d.%d.%d.%d\r\n",g_cRemot_addr[0],g_cRemot_addr[1],g_cRemot_addr[2],g_cRemot_addr[3]);//д��IP��ַ
			for(jbk=0;jbk<20;jbk++)
			{
			if(cIP[jbk]==0x0D&&cIP[++jbk]==0x0A)
			break;
			}
			if(cCommand!=0xff)
			{
			res = f_write(&fnew,"ʱ��:",5,&bw);//��д������
			res = f_write(&fnew,cTime,20,&bw);//��д��ʱ��
			res = f_write(&fnew,"IP:",3,&bw);//��д������
			res = f_write(&fnew,(cIP+1),(jbk-1),&bw);//д��IP
			}
			for(jbk=0;jbk<20;jbk++)
			{
			if(file_name[jbk]==0x0D&&file_name[++jbk]==0x0A)
			break;
			}
			switch(cCommand)
			{
				case 0:
					res = f_write(&fnew,"������һ�κ�����д������\r\n",26,&bw);//��д������
					res = f_write(&fnew,"\r\n",2,&bw);
				break;
				case 1:
					res = f_write(&fnew,"������ȡ��һ�ι�����Ϣ\r\n",24,&bw);//��д������
					res = f_write(&fnew,"\r\n",2,&bw);
				break;
				case 4:
					res = f_write(&fnew,"�����ϴ���һ��Ԥ�Ʊ�:",21,&bw);//��д������
					res = f_write(&fnew,file_name,(jbk-1),&bw);//д��Ԥ�Ʊ����
					res = f_write(&fnew,"\r\n",2,&bw);
				break;
				case 5:
					res = f_write(&fnew,"�����޸���һ��������Ϣ\r\n",24,&bw);//��д������
					res = f_write(&fnew,"\r\n",2,&bw);
				break;
				case 9:
					res = f_write(&fnew,"����������һ���ļ�\r\n",20,&bw);//��д������
					res = f_write(&fnew,"\r\n",2,&bw);
				break;
				case 10:
					res = f_write(&fnew,"�����ϴ���һ�β�����\r\n",22,&bw);//��д������
					res = f_write(&fnew,cFileData,iFileLong,&bw);//д���ļ�
					res = f_write(&fnew,"\r\n",2,&bw);//��д������						
				break;
				case 0xff:				
				break;
				default:
					res = f_write(&fnew,"ִ��������\r\n",12,&bw);//��д������
					res = f_write(&fnew,"\r\n",2,&bw);
				break;
			}
			break;//����
		}
	}
	f_close(&fnew);/*�ر��ļ� */
	f_mount(0,NULL);//ȡ������
	OSSemPost(SDSem);//�ͷ��ź���
}
unsigned char SaveFile(StrFrameHeader *frame_head,unsigned char *cFileName,unsigned int iOffsetAddr)
{
	FRESULT res;
	FIL fnew;//�ļ�ָ��
	FATFS fs;
	unsigned int bw;
	unsigned char *cFileData;//�ļ�����
	unsigned int iFileLong;//�ļ�����
	char cFileWay[25];
	unsigned int i;
	unsigned int j;
	if(cFileName[5]=='S'||cFileName[5]=='Z') //bin/
	sprintf((char*)cFileWay,"0:/font/%s",cFileName+4);   //�ļ����洢
	else
	sprintf((char*)cFileWay,"0:/%s",cFileName);   //�ļ����洢
	
	f_mount(0,&fs);//����
	if(iOffsetAddr == 0)//��һ֡���ݣ�ɾ���������ļ�
	{
		res  = f_open(&fnew,cFileWay,FA_CREATE_ALWAYS | FA_WRITE);//����һ���ļ�
	}
	else
	{
		res  = f_open(&fnew,cFileWay,FA_WRITE);//ֻд�ķ�ʽ���ļ�
	}
	if(res == FR_OK)
	{
		for(i=0;(i<200)&&(frame_head->pdata[i]!='+');i++);//��λ�ļ���ĩβ
		i = i+1;//֡ͷ����
		i = i+4;//ƫ����4Byte
		iFileLong = frame_head->len - i - 3;//�ļ����� = �ܳ���-֡ͷ����-֡β����-ƫ����
		cFileData = mymalloc(SRAMIN,iFileLong);//�����ڴ�
		for(j=0;j<iFileLong;j++)
		{
			cFileData[j] = frame_head->pdata[i+j];//�����ļ�����
		}
		res = f_lseek(&fnew, fnew.fsize);//��ָ���ƶ����ĵ�ĩβ
		res = f_write(&fnew,cFileData,iFileLong,&bw);//д���ļ�
		f_close(&fnew);/*�ر��ļ� */
		f_mount(0,NULL);//ȡ������
		OSSemPost(SDSem);//�ͷ��ź���
		myfree(SRAMIN,cFileData);

		return 0;//д��ɹ�
	}
	else//���ļ�ʧ��
	{
		f_close(&fnew);/*�ر��ļ� */
		f_mount(0,NULL);//ȡ������
		return 1;//��ʧ��
	}
}


unsigned char up_play(StrFrameHeader *frame_head)
{
	FRESULT res;
	FIL fnew;//�ļ�ָ��
	FATFS fs;
	unsigned int bw;
	unsigned char *cFileData;//�ļ�����
	unsigned int iFileLong;//�ļ�����
	unsigned int i;
	unsigned int j;
	unsigned char zh; //�����С
	int zb_x=0; //������ʼ����
	int zb_y=0; //������ʼ����
	char yanse=0;
	char cData[60];
	char R[]="255000000000";
	char G[]="000255000000";
	char Y[]="255255000000";
//unsigned char cData[] = "[list]\r\nitem_no=1\r\nitem0=500,11,0,\\C000000\\fs2424\\c255255000000\\";
	INT8U err;

	do
		{
			OSSemPend(SDSem,0,&err);
		}
	while(err != OS_ERR_NONE);
	f_mount(0,&fs);//����

	res  = f_open(&fnew,"0:/PLAY.LST",FA_CREATE_ALWAYS | FA_WRITE);//����һ���ļ�

	if(res == FR_OK)
	{
		for(i=0;(i<200)&&(frame_head->pdata[i]!=']');i++);//��λ�ļ���ĩβ
		i++;
		
		if(frame_head->pdata[frame_head->len-1]=='>')
		{
		iFileLong = frame_head->len - i - 1 - 3;//�ļ����� = �ܳ���-֡ͷ����-֡β����
		yanse=frame_head->pdata[frame_head->len-2];
		}else
		{
		iFileLong = frame_head->len - i - 1;//�ļ����� = �ܳ���-֡ͷ����-֡β����
		yanse='1';
		}
//		iFileLong = frame_head->len - i - 1 - 3;//�ļ����� = �ܳ���-֡ͷ����-֡β����
		
		cFileData = mymalloc(SRAMIN,iFileLong);//�����ڴ�
		
		if(iFileLong<=10)
			zh=16;
		else
			zh=12;
	//�����������
		zb_y=(G_ScreenSet.LIE-zh*2)/2;		
		zb_x=(G_ScreenSet.HANG-(iFileLong*zh))/2;
		
		if(iFileLong>12) //����������ʾ�����಻��ʾ
		zb_x=0;
		
		if(zb_x<10&&zb_y>=10)
		sprintf(cData,"[list]\r\nitem_no=1\r\nitem0=500,11,0,\\C00%1d0%2d\\fs%2d%2d\\c",zb_x,zb_y,zh*2,zh*2);
		else if(zb_x>10&&zb_x<100&&zb_y>=10)
		sprintf(cData,"[list]\r\nitem_no=1\r\nitem0=500,11,0,\\C0%2d0%2d\\fs%2d%2d\\c",zb_x,zb_y,zh*2,zh*2);
		else if(zb_x>100&&zb_y>=10)
		sprintf(cData,"[list]\r\nitem_no=1\r\nitem0=500,11,0,\\C%3d0%2d\\fs%2d%2d\\c",zb_x,zb_y,zh*2,zh*2);
		
		if(zb_x<10&&zb_y<10)
		sprintf(cData,"[list]\r\nitem_no=1\r\nitem0=500,11,0,\\C00%1d00%1d\\fs%2d%2d\\c",zb_x,zb_y,zh*2,zh*2);
		else if(zb_x>10&&zb_x<100&&zb_y<10)
		sprintf(cData,"[list]\r\nitem_no=1\r\nitem0=500,11,0,\\C0%2d00%1d\\fs%2d%2d\\c",zb_x,zb_y,zh*2,zh*2);
		else if(zb_x>100&&zb_y<10)
		sprintf(cData,"[list]\r\nitem_no=1\r\nitem0=500,11,0,\\C%3d00%1d\\fs%2d%2d\\c",zb_x,zb_y,zh*2,zh*2);
//		res = f_write(&fnew,cData,(sizeof(cData)-1),&bw);//д���ļ�
		res = f_write(&fnew,cData,51,&bw);//д���ļ�	
//		res = f_lseek(&fnew, fnew.fsize);//��ָ���ƶ����ĵ�ĩβ		

		if(yanse=='1')
		res = f_write(&fnew,R,12,&bw);//д���ļ�			
		else if(yanse=='2')
		res = f_write(&fnew,G,12,&bw);//д���ļ�	
		else if(yanse=='3')
		res = f_write(&fnew,Y,12,&bw);//д���ļ�	
		
		for(j=0;j<iFileLong;j++)
		{
			cFileData[j] = frame_head->pdata[i+j];//�����ļ�����
		}
		res = f_write(&fnew,cFileData,iFileLong,&bw);//д���ļ�
		
		f_close(&fnew);/*�ر��ļ� */
		f_mount(0,NULL);//ȡ������
		OSSemPost(SDSem);//�ͷ��ź���
		myfree(SRAMIN,cFileData);

		return 0;//д��ɹ�
	}
	else//���ļ�ʧ��
	{
		f_close(&fnew);/*�ر��ļ� */
		f_mount(0,NULL);//ȡ������
		return 1;//��ʧ��
	}
}
