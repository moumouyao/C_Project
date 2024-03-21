/**
  ******************************************************************************
  * 文件名 : host_app.c
  * 作  者 : 陈光
  * 版  本 ：V1.0
  * 时  间 : 2014年7月30日
  * 公  司 ：汉威光电技术有限公司
  * 摘  要 ：本文件包含了主控制器与从控制器处理相关应用任务
  ******************************************************************************
  */
#include "includes.h"
#include "string.h"
#include "stdio.h"
#include "stdlib.h"
#include "usart.h"	
#include "sw_controlcenter.h"
#include "ucos_ii.h"
#include "ff.h"	
#include "sw_global.h"
#include "sw_host.h"
#include "memp.h"
#include "host_app.h"
#include "led.h"
#include  "user_crc.h"
#include "24cxx.h" 
#include "i2c.h"
#include "user_tim3_pwm.h"
#include "delay.h"
BYTE Itemlisttab[800];

uint8_t Slave_Tem = 0;

char FILE_Write(UINT *size,const TCHAR *path,void *data_ptr)
{
	int res;  
	FIL fdst;
	UINT  bw=0; // File R/W count 
	uint8_t err;
	
	OSSemPend(SDSem,0,&err);
	res = f_open(&fdst,path,FA_CREATE_ALWAYS|FA_WRITE);
	if(res!=FR_OK)
	{
////		printf("open %s err %d\r\n",path,res);
		return 0;
	}
	res = f_write(&fdst,data_ptr,*size,&bw);
	f_close(&fdst);/*关闭文件 */ 
	OSSemPost(SDSem);
	if(res!=FR_OK)
	{
////		printf("%s write err %d\r\n",path,res);
		return 0;
	}
	return 1;
}

/**
  ******************************************************************************
  *                             Task_ReadPlayList(void *p_arg)
  *
  * 描述   : 任务6：解析播放帧表，并发送RGB数据到从机
  *		   
  * 参数   : 无
  *
  *
  * 返回值 : 无
  ******************************************************************************
  */
void Task_ReadPlayList(void *p_arg)
{
	int res;  
	FIL fdst;
	FATFS fs;            
	UINT  size=0,bw=0;
  StrFrameHeader *frame_head;
	INT8U err; 
  //INT8U Yiwei_data;//*data_ptr=NULL;
	BYTE file_name[20];
  uint8_t frame_num;//,offset_H,offset_L;
	uint16_t i=0;
	INT16U  addr,j,temp;
	(void)p_arg;

	while(1)
	{
		Ana_flag=0;		
		frame_head = OSQPend(PlayListMsgQeue,0,&err);  //获取播放帧表消息指针

		if(G_DataSetFlag == 1)//参数设置之后才会进行解析
		{
       if(KeyFlag>0&&KeyFlag<9 && temporary_flag==0)
			{
				PwmSet(128);
			}
			else 
      {
				for(i=0;(*(frame_head->pdata+i)) !='=';i++);//寻找第一个'='		
				
				addr = i;	//记录总帧号前'='的数据偏移量
				frame_num=0;
				for(j=1;(*(frame_head->pdata+i+j)>='0')&&(*(frame_head->pdata+i+j)<='9');j++)
					frame_num = frame_num*10+(*(frame_head->pdata+i+j)-48);
				for(i=0;i<frame_num;i++)
				{

					Ana_flag=1;					//数据正在解析标志位
					ClrItemBuffer();//清理数组				
					IWDG_ReloadCounter();
					temp = ReadOneItem(frame_head->pdata+addr,frame_head->len-addr);//求每帧数据长度
					IWDG_ReloadCounter();
					if(temp == 0)
					{
            ;
					}
					Itemlisttab[i*4]=ItemBuffer.itemlist.itemyyy;
					Itemlisttab[i*4+1]=ItemBuffer.itemlist.delay;
					Itemlisttab[i*4+2]=ItemBuffer.itemlist.mod;
					Itemlisttab[i*4+3]=ItemBuffer.itemlist.param;
         
					TranSlaveDataHandle(frame_num,i);//发给从机  一屏发一次
					addr += temp;	
				}
        sprintf((char*)file_name,"0:/ITEMLIST.LST");   // 文件名存储
        f_mount(0,&fs);
        res = f_open(&fdst,(char*)file_name, FA_CREATE_ALWAYS | FA_WRITE);
        if(res!=FR_OK)
        {
          f_mount(0,NULL);
          break;
        }
        f_write(&fdst,Itemlisttab,i*4,&bw);
        f_close(&fdst);
       
        sprintf((char*)file_name,"0:/ITEM_NUM.LST");   // 文件名存储
        f_mount(0,&fs);
        res = f_open(&fdst,(char*)file_name, FA_CREATE_ALWAYS | FA_WRITE);
        if(res!=FR_OK)
        {
          f_mount(0,NULL);
          break;
        }
        f_write(&fdst,&frame_num,1,&bw);
        f_close(&fdst);

        G_UpCpuFlag = 0;

        //将预置播放列表的内容写入 play.lst中
				if(YuZhiFlag == 1 && temporary_flag == 0)//是预置播放表，不是临时播放表
				{
					size = frame_head->len;
					f_mount(0,&fs);
					err = FILE_Write(&size,"play.lst",frame_head->pdata);
					f_mount(0,NULL);
				}

      }
      G_TranFrameCnt = 0;
      G_ItemUpdateFlag = 1;//播放帧表更新完成标志位
      E2promWriteByte(EEROM_ITEM_UPDATE_FLAG_ADDR,G_ItemUpdateFlag);//写入到EEROM中//EEROM中存储更新标志位

     }
		else
      ;	
		G_DisplayFlag = 0;    //停止显示	
		memp_free(MEMP_PBUF_POOL,(void *)frame_head);//释放POOL内存	
		IWDG_ReloadCounter();	
	}			
}


/**
  ******************************************************************************
  *                             TranSlaveDataHandle()
  *
  * 描述   : 将一屏数据分包发送给从机，每一从机都发送完整的一屏数据
  *		   
  * 参数   : num:要解析发送的总帧数
  *          no :本次发送数据帧号
  *
  *
  * 返回值 : 无
  ******************************************************************************
  */

void TranSlaveDataHandle (INT8U num,INT8U no)//帧总数，帧号
{
	INT16U test=0;
	INT8U  err,i,last=0;
	INT16U j,temp;
	INT8U  *pot=NULL,*p=NULL,*backpot=NULL;
  
  INT8U k,*retp=NULL;
	INT16U slaves_b;  
  
	p = (INT8U *)&ItemBuffer;
	temp = (sizeof(ItemList)+SCREEN_BYTE_NUM*SCREEN_YANSE)/2048; //一帧文件分几次发送
	if((sizeof(ItemList)+SCREEN_BYTE_NUM*SCREEN_YANSE)%2048 != 0)
	last = 1;
  
	for(i=0;i<temp;i++)
	{
		G_TranFrameCnt++;//发送计数器
		pot = memp_malloc(MEMP_PBUF_POOL);   //申请一块内存	
		test = 0;
		while(pot == NULL)
		{
			test++;
			if(test == 1)
			OSTimeDly(5);
			pot = memp_malloc(MEMP_PBUF_POOL);    //申请一块内存用于数据发送	
		}
		backpot = pot;//备份指针
		pot += 2;     //自留两位存放发送个数
		pot++;        //从机地址
		*pot++ = 0;   //从机二级地址
		*pot++ = 1;   //命令类型
		*(INT16U *)pot = 2048+4;
		pot += 2;
		*pot++ = num; //帧总数
		*pot++ = no;  //帧号
		*pot++ = temp+last;//一帧数据分为几部分
		*pot++ = i;   //第几部分数据
		for(j=0;j<2048;j++)
		{
			*pot++ = *p++;
			if(p == &ItemBuffer.rgb_buf[0][SCREEN_BYTE_NUM])
				p = ItemBuffer.rgb_buf[1];
			else if(p == &ItemBuffer.rgb_buf[1][SCREEN_BYTE_NUM])
				p = ItemBuffer.rgb_buf[2];				
		}
    
    *(INT16U *)backpot = pot-backpot-2;  //数据长度
    OSSemPend(RS485Sem,0,&err);    //获取UART2资源信号量

    for(k=1;k<=SLAVE_NUM;k++)
    {
      *(backpot+2) = k;//从机地址		
      retp = RS485TranSlaves(backpot,&slaves_b);      //从机影噶返回数据
      if(retp != NULL)
        memp_free(MEMP_PBUF_POOL,(void *)retp);//释放POOL内存	
    }
   	OSSemPost(RS485Sem);			     //释放UART2资源信号量
    memp_free(MEMP_PBUF_POOL,(void *)backpot);//释放POOL内存	
	}
	if(last == 1)
	{
		G_TranFrameCnt++;//发送计数器
		pot = memp_malloc(MEMP_PBUF_POOL);     //申请一块内存	
		test = 0;
		while(pot == NULL)
		{
		  test++;
			if(test == 1)
		  OSTimeDly(5);
			pot = memp_malloc(MEMP_PBUF_POOL);     //申请一块内存用于数据发送	
		}
		backpot = pot;                //备份指针
		
		pot += 2;     //自留两位存放发送个数
		*pot++=0;     //从机地址
		*pot++ = 0;   //从机二级地址
		*pot++ = 1;   //命令类型
		*(INT16U *)pot = (sizeof(ItemList)+SCREEN_BYTE_NUM*SCREEN_YANSE)%2048+4;
		pot += 2;
		*pot++ = num; //帧总数
		*pot++ = no;  //帧号
		*pot++ = temp+last;//一帧数据分为几部分
		*pot++ = temp+last-1;    //第几部分数据
		temp = (sizeof(ItemList)+SCREEN_BYTE_NUM*SCREEN_YANSE)%2048;//最后一帧文件大小
    
		for(j=0;j<temp;j++)
		{
			*pot++ = *p++;
			if(p == &ItemBuffer.rgb_buf[0][SCREEN_BYTE_NUM])        
				p = ItemBuffer.rgb_buf[1];
			else if(p == &ItemBuffer.rgb_buf[1][SCREEN_BYTE_NUM])
				p = ItemBuffer.rgb_buf[2];
		}
		*(INT16U *)backpot = pot-backpot-2;
 
    OSSemPend(RS485Sem,0,&err);    //获取UART3资源信号量
    for(k=1;k <= SLAVE_NUM;k++)
    {
      *(backpot+2) = k;               //从机地址		
      retp = RS485TranSlaves(backpot,&slaves_b);      //从机影噶返回数据
      if(retp != NULL)
       		memp_free(MEMP_PBUF_POOL,(void *)retp);//释放POOL内存	

    }
   	OSSemPost(RS485Sem);			     //释放UART2资源信号量
    memp_free(MEMP_PBUF_POOL,(void *)backpot);//释放POOL内存	

  }

}




//获取从机故障信息
void GetSlaveErrData(void)
{
	INT16U test=0;
	INT8U  err,i;
	INT16U temp;//j,
	INT8U  *pot=NULL,*backpot=NULL;//,*p=NULL
  
  INT8U k,*retp=NULL;
	INT16U slaves_b;  
  
//	p = (INT8U *)&ItemBuffer;

  
//	for(i=0;i<temp;i++)
//	{
//		G_TranFrameCnt++;//发送计数器
		pot = memp_malloc(MEMP_PBUF_POOL);   //申请一块内存	
		test = 0;
		while(pot == NULL)
		{
			test++;
			if(test == 1)
			OSTimeDly(5);
			pot = memp_malloc(MEMP_PBUF_POOL);    //申请一块内存用于数据发送	
		}
		backpot = pot;//备份指针
		pot += 2;     //自留两位存放发送个数
		pot++;        //从机地址
		*pot++ = 0;   //从机二级地址
		*pot++ = 6;   //命令类型
		*(INT16U *)pot = 0;
		pot += 2;
  
    *(INT16U *)backpot = pot-backpot-2;  //数据长度
    OSSemPend(RS485Sem,0,&err);    //获取UART2资源信号量

    for(k=1;k<=SLAVE_NUM;k++)
    {
      *(backpot+2) = k;//从机地址		
      retp = RS485TranSlaves(backpot,&slaves_b);      //从机影噶返回数据
      if(retp != NULL)
        memp_free(MEMP_PBUF_POOL,(void *)retp);//释放POOL内存	
    }
   	OSSemPost(RS485Sem);			     //释放UART2资源信号量
    memp_free(MEMP_PBUF_POOL,(void *)backpot);//释放POOL内存	
//	}


}





INT8U * RS485TranSlaves(INT8U *p,INT16U *slaves_b)
{
	uint32_t crc32,rec32;
	uint16_t num,crc32num;
	uint8_t  err,j,k;//,sendslavenum = 0;
	uint8_t  *ackp = NULL;
	uint16_t rec_datalen;   //从机返回数据长度，-不包括数据包头
	
	num = *(INT16U *)p;     //要发送的数据个数
	RS485_TX_EN=1;            //配置RS485为输出并延时            //配置RS485为输出并延时


	*(INT32U *)(p+2+num) = UserHWCRC32(p+2,num);//校验
	if(num > 50)  //发送数据点阵
	{
		for(j=0;j<3;j++)
		{
			RS485TranConfig((p+2),num+4);		     //配置UART2准备发送数据
			USART_ITConfig(USART3, USART_IT_TXE, ENABLE);

			ackp = OSQPend(RS485RecMsgQeue,500,&err);	
			if(ackp != NULL)//有数据返回
			{		
				slave_rst_flag=0;
       
          crc32num = *(INT16U *)ackp;
          crc32 = UserHWCRC32(ackp+2,crc32num-4);//CRC32校验
          rec32 = *(INT32U *)(ackp+2+crc32num-4);
          if(crc32 == rec32 )
          {
            if(*(ackp+2+3) == 0)//成功
            {						
              RS485_TX_EN=1;            //配置RS485为输出并延时
              *slaves_b |= 0x01<<(*(p+2)-1);       //从机应答成功标志位
              rec_datalen = *(uint16_t *)(ackp+2+4);
                  
              if(rec_datalen == 0)
              {
                memp_free(MEMP_PBUF_POOL,ackp);//释放POOL内存	
                return 0;
              }
              else
              {
                return ackp;
              }
            }
            else//失败
            {
              RS485_TX_EN=1;            //配置RS485为输出并延时
              memp_free(MEMP_PBUF_POOL,ackp);//释放POOL内存	

            	Delay(30);
            }
          
          }
          else //校验结果不正确
          {
            RS485_TX_EN=1;            //配置RS485为输出并延时
           memp_free(MEMP_PBUF_POOL,ackp);//释放POOL内存	

            Delay(30);           
          }
       
			}
			else//没有返回数据
			{
				slave_rst_flag++;
				if(slave_rst_flag>1 && slave_rst_flag<4)
				{
					SLAVERSTReset;
					OSTimeDly(1);
					SLAVERSTSet;
        
				}
				else if(slave_rst_flag == 5)
          slave_rst_flag = 5;
				RS485_TX_EN=1;            //配置RS485为输出并延时		
				for(k=0;k<*(p+2);k++)
				{
					LED2 = 1;
					OSTimeDly(300);
					LED2 = 0; 
					OSTimeDly(150);			
				}
			}
		}
	}
	else  //不是数据点阵  发送3次
	{
		for(j=0;j<3;j++)
		{
			RS485TranConfig((p+2),num+4);		     //配置UART2准备发送数据
			USART_ITConfig(USART3, USART_IT_TXE, ENABLE); 
      delay_ms(10);
			while(RS485TranCnt < RS485TranLength){};//等待发送完成//////////////////////////////
//				label:
      delay_ms(100);
			ackp = OSQPend(RS485RecMsgQeue,500,&err);	//主板接收从板的应答
			if(ackp != NULL)//有返回数据
			{
				slave_rst_flag=0;
				Keydebug=0;
				crc32num = *(INT16U *)ackp;
//				printf("crc32num = %d\r\n", crc32num);
				crc32 = UserHWCRC32(ackp+2,crc32num-4);//CRC32校验
				rec32 = *(uint32_t *)(ackp+2+crc32num-4);

//				printf("crc32 = %x,rec32 = %x\r\n", crc32, rec32);
				if(crc32 == rec32)//校验成功
				{
							
					if(*(ackp+2+3) == 0)//成功
					{
//						printf("000000\r\n");
					RS485_TX_EN=1;            //配置RS485为输出并延时
						*slaves_b |= 0x01<<(*(p+2)-1);       //从机应答成功标志位
						rec_datalen = *(uint16_t *)(ackp+2+4);
						
						
						if(rec_datalen == 0)
						{
							memp_free(MEMP_PBUF_POOL,ackp);//释放POOL内存	
							return 0;
						}
						else
            {
              return ackp;
            }	
					}
					else if(*(ackp+2+3) ==6){	//接收到的从板故障信息
//						printf("666666\r\n");
						RS485_TX_EN=1;            //配置RS485为输出并延时
						*slaves_b |= 0x01<<(*(p+2)-1);       //从机应答成功标志位
						err = OSQPost(InfmBoard_ErrQeue,ackp);	//将故障信息发给函数
						return NULL;
					}
					else if(*(ackp+2+3) ==7){	//接收到坏点检测信息
//						printf("77777\r\n");
//						goto label;
						RS485_TX_EN=1;            //配置RS485为输出并延时
						*slaves_b |= 0x01<<(*(p+2)-1);       //从机应答成功标志位
						err = OSQPost(InfmBoardLed_ErrQeue,ackp);	//将坏点信息发给函数
						if(err == OS_ERR_NONE){
//							printf("InfmBoardLed_ErrQeue successful\r\n");
							return NULL;
						}
						else{
//							printf("InfmBoardLed_ErrQeue err\r\n");
						}
					}
					else//失败
					{
//						printf("recv err\r\n");
						RS485_TX_EN=1;            //配置RS485为输出并延时
						memp_free(MEMP_PBUF_POOL,ackp);//释放POOL内存	
						Delay(30);  
					}
				}
				else//校验错误
				{
					RS485_TX_EN=1;            //配置RS485为输出并延时
					memp_free(MEMP_PBUF_POOL,ackp);//释放POOL内存	
				  Delay(30);           
				}
				slave_communication = slave_communication&(~(1<<(*(p+2)-1)));//第n个从机通信成功
			}
			else//无返回数据
			{
//				printf("none data\r\n");
				slave_communication = slave_communication|(1<<(*(p+2)-1));//第n个从机通信失败
				slave_rst_flag++;
        //两次从机未正常响应 就复位从机
        if(slave_rst_flag>=2 && slave_rst_flag<5)
				{
					SLAVERSTReset;
					OSTimeDly(5);
					SLAVERSTSet;
          Delay(30);
				}
				else if(slave_rst_flag>=5)
					slave_rst_flag = 5;
			RS485_TX_EN=1;            //配置RS485为输出并延时			
				for(k=0;k<*(p+2);k++)
				{
					host_Flag++;
					if(host_Flag>=5)
						host_Flag=5;
					LED2 = 1;
					OSTimeDly(300);
					LED2 = 0; 
					OSTimeDly(150);
				}
			}				
		}				
	}
	return 0;	
}


//获取从机坏点检测信息
void GetSlaveLedErrData(uint8_t num)
{
	INT16U test=0;
	INT8U  err,i;
	INT16U temp;//j,
	INT8U  *pot=NULL,*backpot=NULL;//,*p=NULL
  
  INT8U k,*retp=NULL;
	INT16U slaves_b;  
  
		pot = memp_malloc(MEMP_PBUF_POOL);   //申请一块内存	
		test = 0;
		while(pot == NULL)
		{
			test++;
			if(test == 1)
			OSTimeDly(5);
			pot = memp_malloc(MEMP_PBUF_POOL);    //申请一块内存用于数据发送	
		}
		backpot = pot;//备份指针
		pot += 2;     //自留两位存放发送个数
		pot++;        //从机地址
		*pot++ = 0;   //从机二级地址
		*pot++ = 7;   //命令类型，坏点检测
		
		*(INT16U *)pot = 1;
		pot += 2;
		*pot++ = num;
    *(INT16U *)backpot = pot-backpot-2;  //数据长度
    OSSemPend(RS485Sem,0,&err);    //获取UART2资源信号量

    for(k=1;k<=SLAVE_NUM;k++)
    {
      *(backpot+2) = k;//从机地址		
      retp = RS485TranSlaves(backpot,&slaves_b);      //从机影噶返回数据
      if(retp != NULL)
        memp_free(MEMP_PBUF_POOL,(void *)retp);//释放POOL内存	
			printf("从机地址 %d\r\n", k);
    }
   	OSSemPost(RS485Sem);			     //释放UART2资源信号量
    memp_free(MEMP_PBUF_POOL,(void *)backpot);//释放POOL内存	
//	}


}








