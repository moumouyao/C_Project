/**
  ******************************************************************************
  * 文件名 : ctrlcenter_app.c
  * 作  者 : 陈光
  * 版  本 ：V1.0
  * 时  间 : 2013年7月30日
  * 公  司 ：汉威光电技术有限公司
  * 摘  要 ：本文件包含了与控制中心处理相关的应用任务
  ******************************************************************************
  */
#include "includes.h"
#include "bsp.h"
#include "usart.h"
#include "user_usart.h"
#include "sw_controlcenter.h"
#include "user_tim3_pwm.h"
#include "sw_global.h"
#include "ctrlcenter_app.h"
#include "memp.h"
#include "ff.h"	
#include "ucos_ii.h"
#include "24cxx.h" 
#include "power.h"
#include "delay.h"
#include "sw_crc16.h"
#include "i2c.h"
#include "iap.h" 
#include "IpWhiteList.h"
#include "dispose.h"
#include "rtc.h"
#include "Code.h"
#include "cJSON.h"
#include "ctrlcenter_app.h"

/**
  ******************************************************************************
  *                          Task_CtrlCenterHandle(void *p_arg)
  *
  * 描述   : 任务2：接收并处理来自控制中心的数据，支持串口和网口两种方式
  *		   
  * 参数   : 无
  *
  *
  * 返回值 : 无
  ******************************************************************************
  */
void Task_CtrlCenterHandle(void *p_arg)
{
	StrFrameHeader *frame_head=NULL;//用于指向来自网络的数据包
	INT8U err;
	(void)p_arg;  // 'p_arg' 并没有用到，防止编译器提示警告
	
	while(1)
	{
		frame_head = OSQPend(CtrlCentRecMsgQeue,0,&err);   	//获取接收控制中心数据的消息指针
		CtrlCenterFrameHandle(frame_head);//处理任务包
	}
}



///*//  ******************************************************************************
//  *                Task_FileStore(StrFrameHeader *frame_head)
//  *
//  * 描述   : BMP/PLAY.LIST文件数据存储函数
//  *		   
//  * 参数   : 存储的数据
//  *
//  *
//  * 返回值 : 无
//  ******************************************************************************
//  */
void Task_FileStore(StrFrameHeader *frame_head)
{
	INT8U err;
	err = FileTypeJudgment(frame_head->pdata+5);   //文件类型判断-0:BMP/1:PLAY.LIST

	switch(err)
	{
		case 1://bmp
			FileStoreHandle(1,frame_head);  //存储BMP文件
      memp_free(MEMP_PBUF_POOL,(void *)frame_head);//释放POOL内存	
			break;
		case 2://play.list
			FileStoreHandle(2,frame_head);//包括数据长度+数据	
			if(G_ItemFlag==1)
			{
				//有任务正在解析当中
				while(Ana_flag)
				{			
					AnaSem_flag=1;           //发送任务信号停止任务解析
					OSTimeDly(50);
				}
				err = OSQPost(PlayListMsgQeue,(void *)frame_head);//发送播放帧表解析消息队列PlayListMsgQeue
				while(err == OS_ERR_Q_FULL)  
				{
					OSQFlush(PlayListMsgQeue);
					err = OSQPost(PlayListMsgQeue,(void *)frame_head);  //发送播放帧表解析消息队列         
				}
			}
			else 
        memp_free(MEMP_PBUF_POOL,(void *)frame_head);//释放POOL内存	               
			break;
		default :;
      memp_free(MEMP_PBUF_POOL,(void *)frame_head);//释放POOL内存	
			break;	
	}
}

///**
//  ******************************************************************************
//  *                             CtrlCenterFrameHandle()
//  *
//  * 描述   : 处理接收到的来自控制中心的数据
//  *		   
//  * 参数   : frame_head 描述接收数据的帧头指针
//  *
//  *
//  * 返回值 : 无
//  ******************************************************************************
//  */
uint8_t read_fime_num = 0;//判断文件是否读取完整
uint16_t read_over_flag = 0;
void CtrlCenterFrameHandle (StrFrameHeader * frame_head)
{
		INT8U err;
		
		static INT8U * pot=NULL;
		static StrFrameHeader *ret;
		INT8U frame_fun;//功能码
		char *id=NULL; //消息编码
		pot = memp_malloc(MEMP_PBUF_POOL);
		if(pot == NULL)
		{
			memp_free(MEMP_PBUF_POOL,(void *)frame_head);//释放POOL内存	
			NVIC_SystemReset();
		}
		ret = (StrFrameHeader *)pot;
		ret->type = frame_head->type;
		ret->len=0;
		ret->pdata = pot + sizeof(StrFrameHeader);
		
		cJSON *json;
		json=cJSON_Parse(((const char *)frame_head->pdata));

		frame_fun=cjson_get_type(json);//根据函数名 判断什么命令


		if(cjson_has_key(json,"id")==1)  //查找是否有id这个键，如果没有则是oxff，不执行命令 如果有则记录并返回
			id=cjson_get_string(json,"id");
		else
			frame_fun=0xff;
		
		
		switch (frame_fun)
		{
			
			case 0x05:   //设置亮度 0x05
				if(LightControlType(json)==0)
					Control_AckOK(ret->pdata,&(ret->len),id);
				else
					Control_AckERR(ret->pdata,&(ret->len),id);
			break;	
					
			case 0x06:   //查询亮度 0x06
					QueryLightset(ret->pdata,&(ret->len),id);
					break;	

			case 0x55://复位  85
					SYS_Reset(ret,id);		
					break;

			case 0x62:   //显示预置播放表   98 设置播放制定播放列表
					PlaySpecifyPlaylist(json);//读取预置表号并保存
					if(UpdateItemToSlaves() == 1) //返回1是失败
						Control_AckERR(ret->pdata,&(ret->len),id);
					else
						Control_AckOK(ret->pdata,&(ret->len),id);
			break;	
			
			case 0xA3:  //A3屏幕开关--开
					Control_Power(json);	
					Control_AckOK(ret->pdata,&(ret->len),id);
			break;
			
			case 0x92:  //设置日期时间  controlDateSet
					HOST_SetTime(json);	
					Control_AckOK(ret->pdata,&(ret->len),id);
			break;
			
			case 0x91:  //查询系统时间  querySystemTime
					HOST_Get_SetTime_Ack(ret->pdata,&(ret->len),id);
//					Control_AckOK(ret->pdata,&(ret->len),id);			
			break;

			case 0x97:  //当前显示画面  queryScreenShot
					InfmBoard_RetDisplayInfm(ret->pdata,&(ret->len),id);	
 			break;
			
			case 0x12:  //故障信息  queryFaultInfo
					GetSlaveErrData();
					InfmBoard_ErrAll(ret->pdata,&(ret->len),id);	
			break;
			
			case 0x10:  //上传文件  uploadDeviceFile
				upload_dev_file(json);
				Control_AckOK(ret->pdata,&(ret->len),id);	
			break;
			
			case 0x19:  //文件下载  downDeviceFile
				do{
					read_over_flag = download_dev_file(json, ret->pdata,&(ret->len),id);
					if(read_over_flag == 512){
						read_fime_num++;
						err = OSQPost(UdpTranMsgQeue,(void *)ret);//循环发送
						err=err;
//						memp_free(MEMP_PBUF_POOL,(void *)pot);
						pot = memp_malloc(MEMP_PBUF_POOL);
						if(pot == NULL)
						{
							memp_free(MEMP_PBUF_POOL,(void *)frame_head);//释放POOL内存	
							NVIC_SystemReset();
						}
						ret = (StrFrameHeader *)pot;
						ret->type = frame_head->type;
						ret->len=0;
						ret->pdata = pot + sizeof(StrFrameHeader);
						
						
					}
					else if(read_over_flag > 0 && read_over_flag < 512){
						err = OSQPost(UdpTranMsgQeue,(void *)ret);//循环发送
						err=err;
////						memp_free(MEMP_PBUF_POOL,(void *)ret);
						memp_free(MEMP_PBUF_POOL,frame_head);//释放memp_buf内存						
//						printf("2.read_over_flag = %d\r\n", read_over_flag);
					
						read_fime_num = 0;
						read_over_flag = 0;
						return;
					}
					else if(read_over_flag == 0){
						memp_free(MEMP_PBUF_POOL,frame_head);//释放memp_buf内存						
						read_fime_num = 0;
						read_over_flag = 0;
//						printf("3.read_over_flag = %d\r\n", read_over_flag);
						return;
					}
						
				}while(read_over_flag == 512);//直到文件内容发送完毕
			
			break;
			
			case 0x11:  //文件删除  deleteDeviceFile
				if(delete_dev_file(json))
					Control_AckERR(ret->pdata,&(ret->len),id);
				else
					Control_AckOK(ret->pdata,&(ret->len),id);
			break;
				
			case 0x13:  //文件列表信息  queryDeviceFileDir
				Dev_File_Dir(json, ret->pdata,&(ret->len),id);
			break;
			
			case 0x14:  //坏点像素检测	queryPixelError
				InfmBoardLed_Err_ACK(ret->pdata,&(ret->len),id);
			break;
			
			default:
				cJSON_Delete(json);
			break;
		}
		
    err = OSQPost(UdpTranMsgQeue,(void *)ret);//发送Udp消息队列
		err=err;
		memp_free(MEMP_PBUF_POOL,frame_head);//释放memp_buf内存	
}

/*
/复位单片机
*/
void SYS_Reset(StrFrameHeader *ret,char *id)
{
	uint8_t err;
	
	cJSON* cjson_test=NULL;
	
	cjson_test=cJSON_CreateObject();
	
	cJSON_AddStringToObject(cjson_test,"id",id);//添加字符类型的
	cJSON_AddStringToObject(cjson_test,"status","200");//添加字符类型的
	cJSON_AddStringToObject(cjson_test,"msg"," ");//添加字符类型的
	
	char *cjson_data=cJSON_PrintUnformatted(cjson_test);
	memcpy(ret->pdata,cjson_data,strlen(cjson_data));
	ret->len=strlen(cjson_data);
	cJSON_Delete(cjson_test);

	do
	{ 
		if(ret->type == HEAD_ETH_TYPE)
		{
      err = OSQPost(UdpTranMsgQeue,(void *)ret);//发送Udp消息队列
    }
	}while(err == OS_ERR_Q_FULL);	
	OSTimeDly(200);	
	g_cSysRst=1;
}



uint16_t GetOffSet(StrFrameHeader *frame)
{
	uint8_t i=0;
	uint16_t temp=0;
	StrFrameHeader * backup = frame;
	for(i=0;backup->pdata[i]!='+';i++)
	{
		if(i>20)
			return 0xffff;
	}
	if(backup->pdata[i]=='+')
	{
		temp = ((u32)(*(backup->pdata+i+1)<<24) + (u32)(*(backup->pdata+i+2)<<16) +
			      (u32)(*(backup->pdata+i+3)<< 8) + *(backup->pdata+i+4));
	}
	return temp;
}


void CheckSelfDeal(StrFrameHeader *frame_head)
{
	uint8_t err;
	if((frame_head->pdata[5]-48 == 17) && (frame_head->pdata[6]-48 == 28))//AL全屏点亮
		checkself_flag = 0xff;//0xff时全屏点亮
	else
	{
			checkself_flag = 0xff;
	}
	KeyFlag = frame_head->pdata[7]-48;
	if(KeyFlag == 0)
	{
		UpdateItemToSlaves();                     //更新数据到从机
	}
	else
		err = OSQPost(PlayListMsgQeue,0);
	if(err == OS_ERR_Q_FULL)
	{
		OSQFlush(PlayListMsgQeue);
		err = OSQPost(PlayListMsgQeue,0);
	}
}

/**
  ******************************************************************************
  *                             FileStoreHandle()
  *
  * 描述   : BMP/PLAY.LIST文件存储
  *		   
  * 参数   : stype:文件类型 1:BMP/2:PLAY.LST
  *          frame_head:描述接收数据的帧头指针
  *
  *
  * 返回值 : 无
  ******************************************************************************
  */
void FileStoreHandle (INT8U stype,StrFrameHeader *frame_head)
{
	UINT num;
	UINT file_no;
	UCHAR file_head; 
	uint8_t err,file_part;

	int res;  
	FIL fdst;
	FATFS fs;
	UINT br=0, bw=0,bt=0,W_Time;//,size;            // File R/W count                      
	uint16_t bftype,bmtype,i;//,j,temp,addr;
	BYTE file_name[24];
  uint8_t da[2] ={0x0d,0x0a};//,frame_num;
	num = frame_head->len;//如果num为奇数呢？？？
  OSSemPend(SDSem,0,&err);
	switch(stype)
	{
		case 1 : //bmp
			bftype=num-24; 
			file_head=frame_head->pdata[9];
			file_no = ((frame_head->pdata[10])-48)*10 + ((frame_head->pdata[11])-48); //解析文件名
      sprintf((char*)file_name,"0:/bmp/%c%c%c.bmp",file_head,frame_head->pdata[10],frame_head->pdata[11]);   // 文件名存储
			f_mount(0,&fs);
			for(i=0;*(frame_head->pdata+i)!='+';i++);
			file_part = ((u32)(*(frame_head->pdata+i+1)<<24) + (u32)(*(frame_head->pdata+i+2)<<16) +
			             (u32)(*(frame_head->pdata+i+3)<<8) + *(frame_head->pdata+i+4))>>11;
			if(file_part == 0) 						
				res = f_open(&fdst,(char*)file_name, FA_CREATE_ALWAYS | FA_WRITE);	
			else                        // 文件第几部分			
				res = f_open(&fdst,(char*)file_name, FA_OPEN_ALWAYS | FA_WRITE);
			
			if(res != FR_OK)
			{
				f_mount(0,NULL);
				OSSemPost(SDSem);
				return;
			}					
			for(W_Time=0;bt==0;W_Time++)
			{  
				if(bftype>510)//bmp数据存储分批存入SD卡，每批510B
					{bmtype=510;bftype=bftype-510;}
				else 	
					{bmtype=bftype;bt=1;}
				res = f_lseek(&fdst,fdst.fsize);
				if(W_Time==0)							
					res = f_write(&fdst,(u16 *)(frame_head->pdata+21), bmtype, &bw);	
				else							
					res = f_write(&fdst,(u16 *)(frame_head->pdata+21+W_Time*510), bmtype,&bw);							
				if(res != FR_OK)
				{
					f_close(&fdst);
					f_mount(0,NULL);
					OSSemPost(SDSem);
					return;
				}
			}		  	
			f_close(&fdst);/*关闭文件 */  
			f_mount(0,NULL);
			for(br=0;br<24;br++)
				file_name[br]=0x00;  //	清文件名缓存	 
			break;
      
      
		case 2 : //LIST
		G_ItemFlag = 0;       //完整play.lst文件标志位
			E2promWriteByte(EEROM_ITEM_FLAG_ADDR,G_ItemFlag);
			bftype=num+sizeof(StrFrameHeader); 
			if((frame_head->pdata[5]=='P'||frame_head->pdata[5]=='p')&&  
			   (frame_head->pdata[6]=='L'||frame_head->pdata[6]=='l')&&
			   (frame_head->pdata[7]=='A'||frame_head->pdata[7]=='a')&&
			   (frame_head->pdata[8]=='Y'||frame_head->pdata[8]=='y')&&
			   (frame_head->pdata[9]=='L'||frame_head->pdata[9]=='l')&&
			   (frame_head->pdata[10]=='I'||frame_head->pdata[10]=='i')&&
			   (frame_head->pdata[11]=='S'||frame_head->pdata[11]=='s')&&
			   (frame_head->pdata[12]=='T'||frame_head->pdata[12]=='t')&&
			   (frame_head->pdata[15]=='+'||frame_head->pdata[16]=='+'||frame_head->pdata[17]=='+'))// 预置表判断
			 {
					if(frame_head->pdata[17]=='+')
						file_no = ((frame_head->pdata[14])-48)*100 +((frame_head->pdata[15])-48)*10+ ((frame_head->pdata[16])-48); //解析文件名
					else if(frame_head->pdata[16]=='+')
						file_no = ((frame_head->pdata[14])-48)*10 +((frame_head->pdata[15])-48);
					else
						file_no = (frame_head->pdata[14])-48;
					sprintf((char*)file_name,"0:/playlist/%d",file_no);   // 文件名存储 存成预置播放列表							 
			 }
        else		
        {
            sprintf((char*)file_name,"0:/PLAY.LST");   // 文件名存储	
        }
			f_mount(0,&fs);
			if(offset==0)//没有文件名 重新创建文件
			{
				i=0;
				res = f_open(&fdst,(char*)file_name,FA_CREATE_ALWAYS | FA_WRITE);
			}
			else
			{
				for(i=0;frame_head->pdata[i]!='+';i++);
				i+=5;//定位到数据
				res = f_open(&fdst,(char*)file_name,FA_OPEN_EXISTING | FA_WRITE);
				res = f_lseek(&fdst,fdst.fsize);
			}
			if(res != FR_OK)
			{
				f_mount(0,NULL);
				OSSemPost(SDSem);
				return;
			}

			res = f_write(&fdst,(u16 *)(frame_head->pdata+18),frame_head->len-18-3,&bw);
			if(res != FR_OK)
			{
				f_close(&fdst);
				f_mount(0,NULL);
				OSSemPost(SDSem);
				return;
			}					
			f_close(&fdst);/*关闭文件 */ 
			f_mount(0,NULL);   			
      if(frame_head->len  <2048)		
			{	
         //添加换行符
        sprintf((char*)file_name,"0:/PLAY.LST");   // 文件名存储
        f_mount(0,&fs);
        res = f_open(&fdst,(char*)file_name, FA_OPEN_EXISTING | FA_READ | FA_WRITE);
        if(res!=FR_OK)
        {
          f_mount(0,NULL);
           OSSemPost(SDSem);
          break;
        }
        f_lseek(&fdst, fdst.fsize);
        f_write(&fdst,da,2,&bw);
        f_close(&fdst);
        
				G_ItemFlag = 1;       //完整play.lst文件标志位
				E2promWriteByte(EEROM_ITEM_FLAG_ADDR,G_ItemFlag);
				G_ItemUpdateFlag = 0; //帧表更新标志位
				E2promWriteByte(EEROM_ITEM_UPDATE_FLAG_ADDR,G_ItemUpdateFlag);
			}
#if EnableTest
			g_cTxtUp = 1;//更新了一次文件数据		
#else				
			G_ItemFlag = 1;       //完整play.lst文件标志位
			E2promWriteByte(EEROM_ITEM_FLAG_ADDR,G_ItemFlag);
			UpdateItemToSlaves();	//更新数据到从机
			G_ItemUpdateFlag = 0; //帧表更新标志位
			E2promWriteByte(EEROM_ITEM_UPDATE_FLAG_ADDR,G_ItemUpdateFlag);								
#endif						
			break;
		default:break;
	}
	OSSemPost(SDSem);
}

/*
 *模块名：获取cjson整数型数字
 */
int cjson_get_number(cJSON *cjson,char *key)
{
	if(strlen(key)>0x00)
	{
		return cJSON_GetObjectItem(cjson,key)->valueint;
	}
	else
	{
	return 0x00;
	}
}
/*
 *模块名：比较json字符串
 */
uint8_t cjson_item_srtcmp(cJSON *cjson,char *key,char *value)
{
	char *cjson_value=cjson_get_string(cjson,key);
	if(cjson_value==NULL)
	{
		return 0x00;
	}
	if(strlen(value))
	{
		return 0x00;
	}
	if(strncmp(cjson_value,value,strlen(value))==0x00)
	{
		return 0x01;
	}
return 0x00;
}
/*
 *模块名：获取json字符串
 */
char *cjson_get_string(cJSON *cjson,char *key)
{
	if(strlen(key)>0x00)
	{
		return cJSON_GetObjectItem(cjson,key)->valuestring;
	}
	else
		return NULL;
}
/*
 *检测json数据中是否是key
 *0：未找到 1找到
 */
uint8_t cjson_has_key(cJSON *cjson,char *key)
{
	if(key!=NULL)
	{
		cJSON *json_temp=cjson->child;
			while(json_temp!=NULL)
			{
#if cJSON_Dbug
			printf("%s\r\n",json_temp->string);
#endif	
				
			if(strcmp(json_temp->string,key)==0x00) //字符串比较是否相等
				{
					return 0x01;
				}
				json_temp=json_temp->next;
			}
		cJSON_Delete(json_temp);//没有分配内存，不需要手动释放，可能会导致内存错误
	}
	return 0x00;
}
/*
 *检测json数据中是否有命令号
 *0xff：未找到 其他：对应命令
 */
uint8_t cjson_get_type(cJSON *cjson)
{
	char * value;
	value=cjson_get_string(cjson,"msgType");
	if(value!=NULL)
	{
		
#if cJSON_Dbug
					printf("msgTyep=%s\r\n",value);
#endif

			if(strncmp("controlSwitch",value,strlen(value))==0x00)//开关屏
			{
				return 0xA3;
			}
			if(strncmp("controlDeviceReboot",value,strlen(value))==0x00)//单片机重启
			{
				return 0x55;
			}
			if(strncmp("controlPlaySpecifyPlaylist",value,strlen(value))==0x00)//播放预制表
			{
				return 0x62;
			}
			if(strncmp("controlLightControlType",value,strlen(value))==0x00)//设置亮度
			{
				return 0x05;
			}
			if(strncmp("queryLightSet",value,strlen(value))==0x00)//查询亮度
			{
				return 0x06;
			}
			if(strncmp("controlDateSet",value,strlen(value))==0x00)//设置日期时间
			{
				return 0x92;
			}
			if(strncmp("querySystemTime",value,strlen(value))==0x00)//查询系统时间
			{
				return 0x91;
			}
			if(strncmp("queryScreenShot",value,strlen(value))==0x00)//当前显示画面
			{
				return 0x97;
			}
			if(strncmp("queryFaultInfo",value,strlen(value))==0x00)//故障信息
			{
				return 0x12;
			}
			if(strncmp("uploadDeviceFile",value,strlen(value))==0x00)//故障信息
			{
				return 0x10;
			}
			if(strncmp("downDeviceFile",value,strlen(value))==0x00)//文件下载
			{
				return 0x19;
			}
			if(strncmp("deleteDeviceFile",value,strlen(value))==0x00)//文件删除
			{
				return 0x11;
			}
			if(strncmp("queryDeviceFileDir",value,strlen(value))==0x00)//文件列表获取
			{
				return 0x13;
			}
			if(strncmp("queryPixelError",value,strlen(value))==0x00)//坏点像素检测
			{
				return 0x14;
			}
	}
	
	return 0xff;
}

/*
 *读取预制表号
 *无
 */
void PlaySpecifyPlaylist (cJSON *json)
{
	char *filename;
	filename=cjson_get_string(json,"fileName");
	YuZhiXuHao=(filename[0]-0x30)*100+(filename[1]-0x30)*10+(filename[2]-0x30);
	cJSON_Delete(json);	

#if cJSON_Dbug
					printf("%d\r\n",YuZhiXuHao);
#endif
	
	YuZhiFlag=1;
	KeyFlag = 0;
	E2promWriteByte(EEROM_YUZHIXUHAO_ADDR,YuZhiXuHao);
	E2promWriteByte(EEROM_YUZHIFLAG_ADDR,YuZhiFlag);
}

/*
 *调整亮度控制方式和亮度
 *无
 */
uint8_t LightControlType (cJSON *json)
{

	int type=cjson_get_number(json,"type");
	BrightValue=cjson_get_number(json,"value");
	if(BrightValue>100||BrightValue<0)
	{
		return 0xff;	
	}
	else
	{
	cJSON_Delete(json);	

	BrightMod = type; //亮度调节方式 0-自动/1-手动
	if(BrightMod)
	BrightPwm=BrightValue*128/100;
	
	PwmSet(BrightPwm);
#if cJSON_Dbug
					printf("BrightMod=%d  value=%d  BrightPwm=%d\r\n",BrightMod,BrightValue,BrightPwm);
#endif	
	return 0;
	}
}
/*
 *查询亮度
 *无
 */
void QueryLightset (uint8_t * p,uint16_t *len,char *id)
{
	cJSON* cjson_test=NULL;
	cjson_test=cJSON_CreateObject();
	
	cJSON_AddStringToObject(cjson_test,"id",id);//添加字符类型的
	cJSON_AddNumberToObject(cjson_test,"type",BrightMod);
	cJSON_AddNumberToObject(cjson_test,"value",BrightValue);
	
	char *cjson_data=cJSON_PrintUnformatted(cjson_test);
	memcpy(p,cjson_data,strlen(cjson_data));
	*len=strlen(cjson_data);
	cJSON_Delete(cjson_test);
#if cJSON_Dbug
					printf("BrightMod=%d  value=%d  BrightPwm=%d\r\n",BrightMod,BrightValue,BrightPwm);
#endif
}

