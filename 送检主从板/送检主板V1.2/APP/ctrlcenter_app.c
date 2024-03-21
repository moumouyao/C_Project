/**
  ******************************************************************************
  * �ļ��� : ctrlcenter_app.c
  * ��  �� : �¹�
  * ��  �� ��V1.0
  * ʱ  �� : 2013��7��30��
  * ��  ˾ ��������缼�����޹�˾
  * ժ  Ҫ �����ļ���������������Ĵ�����ص�Ӧ������
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
  * ����   : ����2�����ղ��������Կ������ĵ����ݣ�֧�ִ��ں��������ַ�ʽ
  *		   
  * ����   : ��
  *
  *
  * ����ֵ : ��
  ******************************************************************************
  */
void Task_CtrlCenterHandle(void *p_arg)
{
	StrFrameHeader *frame_head=NULL;//����ָ��������������ݰ�
	INT8U err;
	(void)p_arg;  // 'p_arg' ��û���õ�����ֹ��������ʾ����
	
	while(1)
	{
		frame_head = OSQPend(CtrlCentRecMsgQeue,0,&err);   	//��ȡ���տ����������ݵ���Ϣָ��
		CtrlCenterFrameHandle(frame_head);//���������
	}
}



///*//  ******************************************************************************
//  *                Task_FileStore(StrFrameHeader *frame_head)
//  *
//  * ����   : BMP/PLAY.LIST�ļ����ݴ洢����
//  *		   
//  * ����   : �洢������
//  *
//  *
//  * ����ֵ : ��
//  ******************************************************************************
//  */
void Task_FileStore(StrFrameHeader *frame_head)
{
	INT8U err;
	err = FileTypeJudgment(frame_head->pdata+5);   //�ļ������ж�-0:BMP/1:PLAY.LIST

	switch(err)
	{
		case 1://bmp
			FileStoreHandle(1,frame_head);  //�洢BMP�ļ�
      memp_free(MEMP_PBUF_POOL,(void *)frame_head);//�ͷ�POOL�ڴ�	
			break;
		case 2://play.list
			FileStoreHandle(2,frame_head);//�������ݳ���+����	
			if(G_ItemFlag==1)
			{
				//���������ڽ�������
				while(Ana_flag)
				{			
					AnaSem_flag=1;           //���������ź�ֹͣ�������
					OSTimeDly(50);
				}
				err = OSQPost(PlayListMsgQeue,(void *)frame_head);//���Ͳ���֡�������Ϣ����PlayListMsgQeue
				while(err == OS_ERR_Q_FULL)  
				{
					OSQFlush(PlayListMsgQeue);
					err = OSQPost(PlayListMsgQeue,(void *)frame_head);  //���Ͳ���֡�������Ϣ����         
				}
			}
			else 
        memp_free(MEMP_PBUF_POOL,(void *)frame_head);//�ͷ�POOL�ڴ�	               
			break;
		default :;
      memp_free(MEMP_PBUF_POOL,(void *)frame_head);//�ͷ�POOL�ڴ�	
			break;	
	}
}

///**
//  ******************************************************************************
//  *                             CtrlCenterFrameHandle()
//  *
//  * ����   : ������յ������Կ������ĵ�����
//  *		   
//  * ����   : frame_head �����������ݵ�֡ͷָ��
//  *
//  *
//  * ����ֵ : ��
//  ******************************************************************************
//  */
uint8_t read_fime_num = 0;//�ж��ļ��Ƿ��ȡ����
uint16_t read_over_flag = 0;
void CtrlCenterFrameHandle (StrFrameHeader * frame_head)
{
		INT8U err;
		
		static INT8U * pot=NULL;
		static StrFrameHeader *ret;
		INT8U frame_fun;//������
		char *id=NULL; //��Ϣ����
		pot = memp_malloc(MEMP_PBUF_POOL);
		if(pot == NULL)
		{
			memp_free(MEMP_PBUF_POOL,(void *)frame_head);//�ͷ�POOL�ڴ�	
			NVIC_SystemReset();
		}
		ret = (StrFrameHeader *)pot;
		ret->type = frame_head->type;
		ret->len=0;
		ret->pdata = pot + sizeof(StrFrameHeader);
		
		cJSON *json;
		json=cJSON_Parse(((const char *)frame_head->pdata));

		frame_fun=cjson_get_type(json);//���ݺ����� �ж�ʲô����


		if(cjson_has_key(json,"id")==1)  //�����Ƿ���id����������û������oxff����ִ������ ��������¼������
			id=cjson_get_string(json,"id");
		else
			frame_fun=0xff;
		
		
		switch (frame_fun)
		{
			
			case 0x05:   //�������� 0x05
				if(LightControlType(json)==0)
					Control_AckOK(ret->pdata,&(ret->len),id);
				else
					Control_AckERR(ret->pdata,&(ret->len),id);
			break;	
					
			case 0x06:   //��ѯ���� 0x06
					QueryLightset(ret->pdata,&(ret->len),id);
					break;	

			case 0x55://��λ  85
					SYS_Reset(ret,id);		
					break;

			case 0x62:   //��ʾԤ�ò��ű�   98 ���ò����ƶ������б�
					PlaySpecifyPlaylist(json);//��ȡԤ�ñ�Ų�����
					if(UpdateItemToSlaves() == 1) //����1��ʧ��
						Control_AckERR(ret->pdata,&(ret->len),id);
					else
						Control_AckOK(ret->pdata,&(ret->len),id);
			break;	
			
			case 0xA3:  //A3��Ļ����--��
					Control_Power(json);	
					Control_AckOK(ret->pdata,&(ret->len),id);
			break;
			
			case 0x92:  //��������ʱ��  controlDateSet
					HOST_SetTime(json);	
					Control_AckOK(ret->pdata,&(ret->len),id);
			break;
			
			case 0x91:  //��ѯϵͳʱ��  querySystemTime
					HOST_Get_SetTime_Ack(ret->pdata,&(ret->len),id);
//					Control_AckOK(ret->pdata,&(ret->len),id);			
			break;

			case 0x97:  //��ǰ��ʾ����  queryScreenShot
					InfmBoard_RetDisplayInfm(ret->pdata,&(ret->len),id);	
 			break;
			
			case 0x12:  //������Ϣ  queryFaultInfo
					GetSlaveErrData();
					InfmBoard_ErrAll(ret->pdata,&(ret->len),id);	
			break;
			
			case 0x10:  //�ϴ��ļ�  uploadDeviceFile
				upload_dev_file(json);
				Control_AckOK(ret->pdata,&(ret->len),id);	
			break;
			
			case 0x19:  //�ļ�����  downDeviceFile
				do{
					read_over_flag = download_dev_file(json, ret->pdata,&(ret->len),id);
					if(read_over_flag == 512){
						read_fime_num++;
						err = OSQPost(UdpTranMsgQeue,(void *)ret);//ѭ������
						err=err;
//						memp_free(MEMP_PBUF_POOL,(void *)pot);
						pot = memp_malloc(MEMP_PBUF_POOL);
						if(pot == NULL)
						{
							memp_free(MEMP_PBUF_POOL,(void *)frame_head);//�ͷ�POOL�ڴ�	
							NVIC_SystemReset();
						}
						ret = (StrFrameHeader *)pot;
						ret->type = frame_head->type;
						ret->len=0;
						ret->pdata = pot + sizeof(StrFrameHeader);
						
						
					}
					else if(read_over_flag > 0 && read_over_flag < 512){
						err = OSQPost(UdpTranMsgQeue,(void *)ret);//ѭ������
						err=err;
////						memp_free(MEMP_PBUF_POOL,(void *)ret);
						memp_free(MEMP_PBUF_POOL,frame_head);//�ͷ�memp_buf�ڴ�						
//						printf("2.read_over_flag = %d\r\n", read_over_flag);
					
						read_fime_num = 0;
						read_over_flag = 0;
						return;
					}
					else if(read_over_flag == 0){
						memp_free(MEMP_PBUF_POOL,frame_head);//�ͷ�memp_buf�ڴ�						
						read_fime_num = 0;
						read_over_flag = 0;
//						printf("3.read_over_flag = %d\r\n", read_over_flag);
						return;
					}
						
				}while(read_over_flag == 512);//ֱ���ļ����ݷ������
			
			break;
			
			case 0x11:  //�ļ�ɾ��  deleteDeviceFile
				if(delete_dev_file(json))
					Control_AckERR(ret->pdata,&(ret->len),id);
				else
					Control_AckOK(ret->pdata,&(ret->len),id);
			break;
				
			case 0x13:  //�ļ��б���Ϣ  queryDeviceFileDir
				Dev_File_Dir(json, ret->pdata,&(ret->len),id);
			break;
			
			case 0x14:  //�������ؼ��	queryPixelError
				InfmBoardLed_Err_ACK(ret->pdata,&(ret->len),id);
			break;
			
			default:
				cJSON_Delete(json);
			break;
		}
		
    err = OSQPost(UdpTranMsgQeue,(void *)ret);//����Udp��Ϣ����
		err=err;
		memp_free(MEMP_PBUF_POOL,frame_head);//�ͷ�memp_buf�ڴ�	
}

/*
/��λ��Ƭ��
*/
void SYS_Reset(StrFrameHeader *ret,char *id)
{
	uint8_t err;
	
	cJSON* cjson_test=NULL;
	
	cjson_test=cJSON_CreateObject();
	
	cJSON_AddStringToObject(cjson_test,"id",id);//����ַ����͵�
	cJSON_AddStringToObject(cjson_test,"status","200");//����ַ����͵�
	cJSON_AddStringToObject(cjson_test,"msg"," ");//����ַ����͵�
	
	char *cjson_data=cJSON_PrintUnformatted(cjson_test);
	memcpy(ret->pdata,cjson_data,strlen(cjson_data));
	ret->len=strlen(cjson_data);
	cJSON_Delete(cjson_test);

	do
	{ 
		if(ret->type == HEAD_ETH_TYPE)
		{
      err = OSQPost(UdpTranMsgQeue,(void *)ret);//����Udp��Ϣ����
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
	if((frame_head->pdata[5]-48 == 17) && (frame_head->pdata[6]-48 == 28))//ALȫ������
		checkself_flag = 0xff;//0xffʱȫ������
	else
	{
			checkself_flag = 0xff;
	}
	KeyFlag = frame_head->pdata[7]-48;
	if(KeyFlag == 0)
	{
		UpdateItemToSlaves();                     //�������ݵ��ӻ�
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
  * ����   : BMP/PLAY.LIST�ļ��洢
  *		   
  * ����   : stype:�ļ����� 1:BMP/2:PLAY.LST
  *          frame_head:�����������ݵ�֡ͷָ��
  *
  *
  * ����ֵ : ��
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
	num = frame_head->len;//���numΪ�����أ�����
  OSSemPend(SDSem,0,&err);
	switch(stype)
	{
		case 1 : //bmp
			bftype=num-24; 
			file_head=frame_head->pdata[9];
			file_no = ((frame_head->pdata[10])-48)*10 + ((frame_head->pdata[11])-48); //�����ļ���
      sprintf((char*)file_name,"0:/bmp/%c%c%c.bmp",file_head,frame_head->pdata[10],frame_head->pdata[11]);   // �ļ����洢
			f_mount(0,&fs);
			for(i=0;*(frame_head->pdata+i)!='+';i++);
			file_part = ((u32)(*(frame_head->pdata+i+1)<<24) + (u32)(*(frame_head->pdata+i+2)<<16) +
			             (u32)(*(frame_head->pdata+i+3)<<8) + *(frame_head->pdata+i+4))>>11;
			if(file_part == 0) 						
				res = f_open(&fdst,(char*)file_name, FA_CREATE_ALWAYS | FA_WRITE);	
			else                        // �ļ��ڼ�����			
				res = f_open(&fdst,(char*)file_name, FA_OPEN_ALWAYS | FA_WRITE);
			
			if(res != FR_OK)
			{
				f_mount(0,NULL);
				OSSemPost(SDSem);
				return;
			}					
			for(W_Time=0;bt==0;W_Time++)
			{  
				if(bftype>510)//bmp���ݴ洢��������SD����ÿ��510B
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
			f_close(&fdst);/*�ر��ļ� */  
			f_mount(0,NULL);
			for(br=0;br<24;br++)
				file_name[br]=0x00;  //	���ļ�������	 
			break;
      
      
		case 2 : //LIST
		G_ItemFlag = 0;       //����play.lst�ļ���־λ
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
			   (frame_head->pdata[15]=='+'||frame_head->pdata[16]=='+'||frame_head->pdata[17]=='+'))// Ԥ�ñ��ж�
			 {
					if(frame_head->pdata[17]=='+')
						file_no = ((frame_head->pdata[14])-48)*100 +((frame_head->pdata[15])-48)*10+ ((frame_head->pdata[16])-48); //�����ļ���
					else if(frame_head->pdata[16]=='+')
						file_no = ((frame_head->pdata[14])-48)*10 +((frame_head->pdata[15])-48);
					else
						file_no = (frame_head->pdata[14])-48;
					sprintf((char*)file_name,"0:/playlist/%d",file_no);   // �ļ����洢 ���Ԥ�ò����б�							 
			 }
        else		
        {
            sprintf((char*)file_name,"0:/PLAY.LST");   // �ļ����洢	
        }
			f_mount(0,&fs);
			if(offset==0)//û���ļ��� ���´����ļ�
			{
				i=0;
				res = f_open(&fdst,(char*)file_name,FA_CREATE_ALWAYS | FA_WRITE);
			}
			else
			{
				for(i=0;frame_head->pdata[i]!='+';i++);
				i+=5;//��λ������
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
			f_close(&fdst);/*�ر��ļ� */ 
			f_mount(0,NULL);   			
      if(frame_head->len  <2048)		
			{	
         //��ӻ��з�
        sprintf((char*)file_name,"0:/PLAY.LST");   // �ļ����洢
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
        
				G_ItemFlag = 1;       //����play.lst�ļ���־λ
				E2promWriteByte(EEROM_ITEM_FLAG_ADDR,G_ItemFlag);
				G_ItemUpdateFlag = 0; //֡����±�־λ
				E2promWriteByte(EEROM_ITEM_UPDATE_FLAG_ADDR,G_ItemUpdateFlag);
			}
#if EnableTest
			g_cTxtUp = 1;//������һ���ļ�����		
#else				
			G_ItemFlag = 1;       //����play.lst�ļ���־λ
			E2promWriteByte(EEROM_ITEM_FLAG_ADDR,G_ItemFlag);
			UpdateItemToSlaves();	//�������ݵ��ӻ�
			G_ItemUpdateFlag = 0; //֡����±�־λ
			E2promWriteByte(EEROM_ITEM_UPDATE_FLAG_ADDR,G_ItemUpdateFlag);								
#endif						
			break;
		default:break;
	}
	OSSemPost(SDSem);
}

/*
 *ģ��������ȡcjson����������
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
 *ģ�������Ƚ�json�ַ���
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
 *ģ��������ȡjson�ַ���
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
 *���json�������Ƿ���key
 *0��δ�ҵ� 1�ҵ�
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
				
			if(strcmp(json_temp->string,key)==0x00) //�ַ����Ƚ��Ƿ����
				{
					return 0x01;
				}
				json_temp=json_temp->next;
			}
		cJSON_Delete(json_temp);//û�з����ڴ棬����Ҫ�ֶ��ͷţ����ܻᵼ���ڴ����
	}
	return 0x00;
}
/*
 *���json�������Ƿ��������
 *0xff��δ�ҵ� ��������Ӧ����
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

			if(strncmp("controlSwitch",value,strlen(value))==0x00)//������
			{
				return 0xA3;
			}
			if(strncmp("controlDeviceReboot",value,strlen(value))==0x00)//��Ƭ������
			{
				return 0x55;
			}
			if(strncmp("controlPlaySpecifyPlaylist",value,strlen(value))==0x00)//����Ԥ�Ʊ�
			{
				return 0x62;
			}
			if(strncmp("controlLightControlType",value,strlen(value))==0x00)//��������
			{
				return 0x05;
			}
			if(strncmp("queryLightSet",value,strlen(value))==0x00)//��ѯ����
			{
				return 0x06;
			}
			if(strncmp("controlDateSet",value,strlen(value))==0x00)//��������ʱ��
			{
				return 0x92;
			}
			if(strncmp("querySystemTime",value,strlen(value))==0x00)//��ѯϵͳʱ��
			{
				return 0x91;
			}
			if(strncmp("queryScreenShot",value,strlen(value))==0x00)//��ǰ��ʾ����
			{
				return 0x97;
			}
			if(strncmp("queryFaultInfo",value,strlen(value))==0x00)//������Ϣ
			{
				return 0x12;
			}
			if(strncmp("uploadDeviceFile",value,strlen(value))==0x00)//������Ϣ
			{
				return 0x10;
			}
			if(strncmp("downDeviceFile",value,strlen(value))==0x00)//�ļ�����
			{
				return 0x19;
			}
			if(strncmp("deleteDeviceFile",value,strlen(value))==0x00)//�ļ�ɾ��
			{
				return 0x11;
			}
			if(strncmp("queryDeviceFileDir",value,strlen(value))==0x00)//�ļ��б��ȡ
			{
				return 0x13;
			}
			if(strncmp("queryPixelError",value,strlen(value))==0x00)//�������ؼ��
			{
				return 0x14;
			}
	}
	
	return 0xff;
}

/*
 *��ȡԤ�Ʊ��
 *��
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
 *�������ȿ��Ʒ�ʽ������
 *��
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

	BrightMod = type; //���ȵ��ڷ�ʽ 0-�Զ�/1-�ֶ�
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
 *��ѯ����
 *��
 */
void QueryLightset (uint8_t * p,uint16_t *len,char *id)
{
	cJSON* cjson_test=NULL;
	cjson_test=cJSON_CreateObject();
	
	cJSON_AddStringToObject(cjson_test,"id",id);//����ַ����͵�
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

