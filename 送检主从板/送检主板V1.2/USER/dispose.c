#include "dispose.h"
#include "stdio.h"
#include "sw_global.h"
#include "ff.h"
#include "rtc.h"
#include "i2c.h"
#include "dis_app.h"

//========================2022-07-29新增代码文件，用于处理琐碎的功能==============================//
/*
功能：获取报文中的文件名信息

*/
unsigned char *getFileName(unsigned char *cmd)
{
	unsigned char *cData;
	unsigned char cNameLong;//名字长度
	unsigned char i;
	for(cNameLong=0;cmd[5+cNameLong]!='+'&& cNameLong<200;cNameLong++);//计算文件名长度
	cData = mymalloc(SRAMEX,cNameLong+1);//申请内存
	for(i=0;i<cNameLong;i++)
	{
		*(cData+i) = cmd[5+i];//获取文件名
	}
	*(cData+i) = '\0';//写入结束符
	if(i==200)//找不到文件名
	{
		myfree(SRAMEX,cData);//释放文件名内存
	}
	return i==200?NULL:cData;//如果i=200代表名字有误返回空指针
}
/*
功能：获取报文中的文件偏移地址
*/
unsigned int getFileAddr(unsigned char *cmd)
{
	unsigned long int iLong=0;
	unsigned int i=0;
	while(1)
	{
		if(cmd[i] == '+')//寻找表示
		{
			i++;//指向地址数据
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

void RecordLog(StrFrameHeader *frame_head,unsigned char cCommand)//记录日志
{
	unsigned int i;
	FRESULT res;
	FIL fnew;//文件指针
	FATFS fs;
	TIME time;
	INT8U err;
	unsigned char *cFileData;//文件数据
	unsigned int iFileLong;//文件长度
	unsigned int bw;
	unsigned char cTime[] = "2000-00-00 00:00:00:";//时间数组
	unsigned char cFileName[] = "2000-00.txt";//日志文件名，年-月
	unsigned char cIP[18]={0};
	char cFileWay[20]={0};
	getTime();//获取时间
	time = g_tTime;
	int jbk=0;
	BYTE file_name[20];//上次预制表名字
	{//写入时间数组
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
		//写入文件名
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
					for(i=0;(i<200)&&(frame_head->pdata[i]!='+');i++);//定位文件名末尾
					i = i+1;//帧头长度
					i = i+4;//偏移量4Byte
					iFileLong = frame_head->len - i - 3;//文件长度 = 总长度-帧头长度-帧尾长度-偏移量
					cFileData = mymalloc(SRAMEX,iFileLong);//申请内存
					for(int j=0;j<iFileLong;j++)
					{
						cFileData[j] = frame_head->pdata[i+j];//复制文件内容
					}
					}
	}
	sprintf((char*)cFileWay,"0:journal/%s",cFileName);   //文件名存储
	OSSemPend(SDSem,0,&err);
	f_mount(0,&fs);//挂载
	
	while(1)
	{
		res  = f_open(&fnew,cFileWay,FA_WRITE);//只写的方式打开文件
		if(res != FR_OK)
		{
			res  = f_open(&fnew,cFileWay,FA_CREATE_ALWAYS | FA_WRITE);//创建一个文件
			if(res == FR_OK)//创建成功
			{
				f_close(&fnew);//关闭文档
			}
			else//创建失败
			{
				break;
			}
		}
		else//打开成功
		{
			res = f_lseek(&fnew, fnew.fsize);//将指针移动到文档末尾
			sprintf((char*)cIP," %d.%d.%d.%d\r\n",g_cRemot_addr[0],g_cRemot_addr[1],g_cRemot_addr[2],g_cRemot_addr[3]);//写入IP地址
			for(jbk=0;jbk<20;jbk++)
			{
			if(cIP[jbk]==0x0D&&cIP[++jbk]==0x0A)
			break;
			}
			if(cCommand!=0xff)
			{
			res = f_write(&fnew,"时间:",5,&bw);//先写入内容
			res = f_write(&fnew,cTime,20,&bw);//先写入时间
			res = f_write(&fnew,"IP:",3,&bw);//先写入内容
			res = f_write(&fnew,(cIP+1),(jbk-1),&bw);//写入IP
			}
			for(jbk=0;jbk<20;jbk++)
			{
			if(file_name[jbk]==0x0D&&file_name[++jbk]==0x0A)
			break;
			}
			switch(cCommand)
			{
				case 0:
					res = f_write(&fnew,"拦截了一次黑名单写入数据\r\n",26,&bw);//先写入内容
					res = f_write(&fnew,"\r\n",2,&bw);
				break;
				case 1:
					res = f_write(&fnew,"主机获取了一次故障信息\r\n",24,&bw);//先写入内容
					res = f_write(&fnew,"\r\n",2,&bw);
				break;
				case 4:
					res = f_write(&fnew,"主机上传了一次预制表:",21,&bw);//先写入内容
					res = f_write(&fnew,file_name,(jbk-1),&bw);//写入预制表代号
					res = f_write(&fnew,"\r\n",2,&bw);
				break;
				case 5:
					res = f_write(&fnew,"主机修改了一次亮度信息\r\n",24,&bw);//先写入内容
					res = f_write(&fnew,"\r\n",2,&bw);
				break;
				case 9:
					res = f_write(&fnew,"主机下载了一次文件\r\n",20,&bw);//先写入内容
					res = f_write(&fnew,"\r\n",2,&bw);
				break;
				case 10:
					res = f_write(&fnew,"主机上传了一次播报表\r\n",22,&bw);//先写入内容
					res = f_write(&fnew,cFileData,iFileLong,&bw);//写入文件
					res = f_write(&fnew,"\r\n",2,&bw);//先写入内容						
				break;
				case 0xff:				
				break;
				default:
					res = f_write(&fnew,"执行了命令\r\n",12,&bw);//先写入内容
					res = f_write(&fnew,"\r\n",2,&bw);
				break;
			}
			break;//跳出
		}
	}
	f_close(&fnew);/*关闭文件 */
	f_mount(0,NULL);//取消挂载
	OSSemPost(SDSem);//释放信号量
}
unsigned char SaveFile(StrFrameHeader *frame_head,unsigned char *cFileName,unsigned int iOffsetAddr)
{
	FRESULT res;
	FIL fnew;//文件指针
	FATFS fs;
	unsigned int bw;
	unsigned char *cFileData;//文件数据
	unsigned int iFileLong;//文件长度
	char cFileWay[25];
	unsigned int i;
	unsigned int j;
	if(cFileName[5]=='S'||cFileName[5]=='Z') //bin/
	sprintf((char*)cFileWay,"0:/font/%s",cFileName+4);   //文件名存储
	else
	sprintf((char*)cFileWay,"0:/%s",cFileName);   //文件名存储
	
	f_mount(0,&fs);//挂载
	if(iOffsetAddr == 0)//第一帧数据，删除并创建文件
	{
		res  = f_open(&fnew,cFileWay,FA_CREATE_ALWAYS | FA_WRITE);//创建一个文件
	}
	else
	{
		res  = f_open(&fnew,cFileWay,FA_WRITE);//只写的方式打开文件
	}
	if(res == FR_OK)
	{
		for(i=0;(i<200)&&(frame_head->pdata[i]!='+');i++);//定位文件名末尾
		i = i+1;//帧头长度
		i = i+4;//偏移量4Byte
		iFileLong = frame_head->len - i - 3;//文件长度 = 总长度-帧头长度-帧尾长度-偏移量
		cFileData = mymalloc(SRAMIN,iFileLong);//申请内存
		for(j=0;j<iFileLong;j++)
		{
			cFileData[j] = frame_head->pdata[i+j];//复制文件内容
		}
		res = f_lseek(&fnew, fnew.fsize);//将指针移动到文档末尾
		res = f_write(&fnew,cFileData,iFileLong,&bw);//写入文件
		f_close(&fnew);/*关闭文件 */
		f_mount(0,NULL);//取消挂载
		OSSemPost(SDSem);//释放信号量
		myfree(SRAMIN,cFileData);

		return 0;//写入成功
	}
	else//打开文件失败
	{
		f_close(&fnew);/*关闭文件 */
		f_mount(0,NULL);//取消挂载
		return 1;//打开失败
	}
}


unsigned char up_play(StrFrameHeader *frame_head)
{
	FRESULT res;
	FIL fnew;//文件指针
	FATFS fs;
	unsigned int bw;
	unsigned char *cFileData;//文件数据
	unsigned int iFileLong;//文件长度
	unsigned int i;
	unsigned int j;
	unsigned char zh; //字体大小
	int zb_x=0; //字体起始坐标
	int zb_y=0; //字体起始坐标
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
	f_mount(0,&fs);//挂载

	res  = f_open(&fnew,"0:/PLAY.LST",FA_CREATE_ALWAYS | FA_WRITE);//创建一个文件

	if(res == FR_OK)
	{
		for(i=0;(i<200)&&(frame_head->pdata[i]!=']');i++);//定位文件名末尾
		i++;
		
		if(frame_head->pdata[frame_head->len-1]=='>')
		{
		iFileLong = frame_head->len - i - 1 - 3;//文件长度 = 总长度-帧头长度-帧尾长度
		yanse=frame_head->pdata[frame_head->len-2];
		}else
		{
		iFileLong = frame_head->len - i - 1;//文件长度 = 总长度-帧头长度-帧尾长度
		yanse='1';
		}
//		iFileLong = frame_head->len - i - 1 - 3;//文件长度 = 总长度-帧头长度-帧尾长度
		
		cFileData = mymalloc(SRAMIN,iFileLong);//申请内存
		
		if(iFileLong<=10)
			zh=16;
		else
			zh=12;
	//计算居中坐标
		zb_y=(G_ScreenSet.LIE-zh*2)/2;		
		zb_x=(G_ScreenSet.HANG-(iFileLong*zh))/2;
		
		if(iFileLong>12) //超长靠左显示，多余不显示
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
//		res = f_write(&fnew,cData,(sizeof(cData)-1),&bw);//写入文件
		res = f_write(&fnew,cData,51,&bw);//写入文件	
//		res = f_lseek(&fnew, fnew.fsize);//将指针移动到文档末尾		

		if(yanse=='1')
		res = f_write(&fnew,R,12,&bw);//写入文件			
		else if(yanse=='2')
		res = f_write(&fnew,G,12,&bw);//写入文件	
		else if(yanse=='3')
		res = f_write(&fnew,Y,12,&bw);//写入文件	
		
		for(j=0;j<iFileLong;j++)
		{
			cFileData[j] = frame_head->pdata[i+j];//复制文件内容
		}
		res = f_write(&fnew,cFileData,iFileLong,&bw);//写入文件
		
		f_close(&fnew);/*关闭文件 */
		f_mount(0,NULL);//取消挂载
		OSSemPost(SDSem);//释放信号量
		myfree(SRAMIN,cFileData);

		return 0;//写入成功
	}
	else//打开文件失败
	{
		f_close(&fnew);/*关闭文件 */
		f_mount(0,NULL);//取消挂载
		return 1;//打开失败
	}
}
