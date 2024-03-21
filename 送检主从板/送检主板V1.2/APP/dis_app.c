/**
  ******************************************************************************
  * 文件名 : dis_app.c
  * 作  者 : 陈光
  * 版  本 ：V1.0
  * 时  间 : 2014年7月30日
  * 公  司 ：汉威光电技术有限公司
  * 摘  要 ：本文件包含了主控制器控制从控制器显示应用任务
  ******************************************************************************
  */
#include "includes.h"
#include "dis_app.h"
#include "usart.h"	
#include "sw_controlcenter.h"
#include "ff.h"
#include "sw_global.h"
#include  "user_crc.h"
#include "memp.h"
#include "delay.h"
#include "host_app.h"
#include "ctrlcenter_app.h"
#include "delay.h"
#include "key.h"
//void Delay(uint32_t nCount)
//{
//  for(; nCount != 0; nCount--);
//}




StrFrameHeader *frame_head;
uint8_t PlayNum,NOWPlayNum;//总帧数
uint16_t      BrightAD=0;

ItemList PlayListTab[100];           //显示帧结构体链表


/**
  ******************************************************************************
  *                             Task_DisplayCtrl(void *p_arg)
  *
  * 描述   : 任务5：控制显示任务
  *		   
  * 参数   : 无
  *
  *
  * 返回值 : 无
  ******************************************************************************
  */

void Task_DisplayCtrl(void *p_arg)
{		
	int res;  
	FIL fdst;
	FATFS fs;
	UINT br=0;// File R/W count 
	BYTE file_name[20];
	
	uint8_t err,i;
	(void)p_arg;
	if(G_DataSetFlag == 1)//每次上电都会发送箱体/从控区域/模组类别参数给从机
	{
      TranDataSetToSlaves();//发送参数设置到从机 颜色 行列点阵
  }
	if(G_ItemFlag == 1 && G_ItemFlag == 1 && G_DataSetFlag == 1)//解析并发送数据 G_ItemFlag 变量被检查了两次，可能是写错了
	{
	  UpdateItemToSlaves();	//更新数据到从机
   }	
  while(1)
	{
again:
		UDPResetFlag=0;
   //情报板参数设置完成、有完整数据，播放帧表更新完毕,解析过的帧发送完成
		if((G_DataSetFlag == 1 && G_DisplayFlag == 0 && G_ItemFlag == 1 && 
			  G_ItemUpdateFlag == 1 && G_TranFrameCnt == 0&&G_UpCpuFlag==0)||temporary_flag!=0)//预置时从SD读取
		{
			sprintf((char*)file_name,"0:/ITEM_NUM.LST");   // 文件名存储
		
			OSSemPend(SDSem,0,&err);
			f_mount(0,&fs);	
			res = f_open(&fdst,(char*)file_name, FA_OPEN_EXISTING | FA_READ);
	if(res!=FR_OK)
	{
			f_mount(0,NULL);
			OSSemPost(SDSem);
			goto again;
	}
			res = f_read(&fdst,&PlayNum, 1, &br);
			f_close(&fdst);/*关闭文件 */
			f_mount(0,NULL);
	    sprintf((char*)file_name,"0:/ITEMLIST.LST");   // 文件名存储
			if(PlayNum < 100)//有完整帧数据
			{
        f_mount(0,&fs);;		
        res = f_open(&fdst,(char*)file_name, FA_OPEN_EXISTING | FA_READ);
        if(res!=FR_OK)
        {
            f_mount(0,NULL);
            OSSemPost(SDSem);
            goto again;
        }					
        res = f_read(&fdst,(u16 *)PlayListTab, PlayNum*4, &br);////////////////
      
        f_close(&fdst);/*关闭文件 */
        f_mount(0,NULL);
        OSSemPost(SDSem);
        G_DisplayFlag = 1;
			}	
      G_UpCpuFlag=1;
    }
		else if(G_DataSetFlag == 1 && G_DisplayFlag == 0 && G_ItemFlag == 1 && 
			G_ItemUpdateFlag == 1 && G_TranFrameCnt == 0&&G_UpCpuFlag==1)
		{
		 	if(PlayNum < 100)//有完整帧数据
			{	
        for(br=0;br<PlayNum;br++)
        {
          PlayListTab[br].itemyyy=Itemlisttab[br*4];
          PlayListTab[br].delay=Itemlisttab[br*4+1];
          PlayListTab[br].mod=Itemlisttab[br*4+2];
          PlayListTab[br].param=Itemlisttab[br*4+3];
        }
				G_DisplayFlag = 1;
			}		
		}
		if(G_DisplayFlag == 1&&PlayNum!=0)
		{
			for(i=0;i<PlayNum;i++)
			{
				NOWPlayNum=i;
        PlayHandle(&PlayListTab[i],i);//显示第几帧数据
				if(G_DisplayFlag == 0)        //新RGB数据到来退出显示
					break;
      }
    }
		else
		{
			OSTimeDly(50); //等待50ms
		}
  }
}


/**
  ******************************************************************************
  *                             Task_DisplayCtrl(void *p_arg)
  *
  * 描述   : 控制从机进行显示操作任务
  *		   
  * 参数   : itemp：播放帧头数据结构指针
  *        : frame_no:显示的帧号
  *
  *
  * 返回值 : 无
  ******************************************************************************
  */
uint8_t breakflag=0;//将停留时间拆开
void PlayHandle(ItemList *itemp,INT8U frame_no)//显示第几帧数据
{
  uint16_t step=0;
	uint16_t step1=0;
	uint8_t time=0,shanshuo=0;
	uint16_t i,j;
	uint8_t ack=0;
  switch(itemp->mod/10)  //移入
	{
    case 1://立即显示
			step = 0;
			break;
		case 2://左移
		  step = SCREEN_HANG-1;
			break;
		case 3://右移
			step = SCREEN_HANG-1;
		  time = 1;//右进时增加延时，因为最后一个从机加载数据快
			break;
		case 4://上移
			step = SCREEN_LIE-1;
			break;
		case 5://下移
			step = SCREEN_LIE-1;
			break;
		case 6://横百叶窗 
			step = 7;
			break;
		case 7://竖百叶窗
			step = 7;
			break;
		case 8://闪烁
			step = 0;shanshuo=1;    //TIM_Cmd(TIM3,ENABLE);
			break;
		default:
			step = 0;
			break;
  }
	for(i=0;i<=step;i++)
	{
		TranSlavesDis(itemp->itemyyy,i);//发送从机加载数据
		delay_ms(15+itemp->param*20);		// 移动速度
		OSTimeDly(5);
		WaitSlaveLoadOK(SLAVE_NUM,&ack);			//可以增加最后一个从机是否加载数据完成
			if(i < SCREEN_HANG-BOX_HANG)		//右移未进入最后一个从控区域时需延时
		  delay_ms(time);
		if(G_DisplayFlag == 0)
			return;
		LCPSet;
		delay_us(10);
		LCPReset;
		delay_us(10);
  }
	time = 0;
	host_Flag=0;
	breakflag=0;
	if(KeyFlag>0&&itemp->delay==0)
	OSTimeDly(100);//停留时间
	else if(shanshuo==1) ////////////////////////shanshuo
	for(j=0;j<((itemp->delay)*100)/((itemp->param+1)*40)+2;j++)
	{
	  if(G_DisplayFlag == 0)break;
	 shanshuo=0;
	}	
	else if(breakflag==0)//将停留时间拆开，若有新数据到来时可第一时间处理
	{
		breakflag=1;
		for(j=0;j<(itemp->delay*10);j++)
		{
			OSTimeDly(100);//停留时间
			if(temporary_flag!=0)
			{
				if(itemp->mod/10==1)
				{
					temporary_flag++;
					if(temporary_flag<5)
						break;
				}
			}
			if(G_DisplayFlag==0)
				break;
		}
	}
	breakflag=0;
	if((frame_no <  PlayNum-1) && 
		(PlayListTab[frame_no].mod == PlayListTab[frame_no+1].mod))
	{
	  return;
	}
	if(G_DisplayFlag == 0)
	{
		return;
	}
  switch(itemp->mod%10)  //移出
	{
    case 1://立即显示
			step1 = 1;
			break;
		case 2://左移
		  step1 = SCREEN_HANG;
		  time = 3;//左出时增加延时，因为最后一个从机加载数据快///////////////////////////////////////////
			break;
		case 3://右移
			step1 = SCREEN_HANG;
			break;
		case 4://上移
			step1 = SCREEN_LIE;
			break;
		case 5://下移
			step1 = SCREEN_LIE;
			break;
		case 6://横百叶窗 
			step1 = 8;
			break;
		case 7://竖百叶窗
			step1 = 8;
			break;
		case 8://闪烁
      step1 = 1;
		  break;
		default:
			step1 = 1;
			break;
  }
	if(step1!=1)
	for(i=1;i<=step1;i++)
	{		
    TranSlavesDis(itemp->itemyyy,i+step);//发送从机加载数据
    delay_ms(15+itemp->param*20);
		delay_ms(2);
		WaitSlaveLoadOK(SLAVE_NUM,&ack);  //等待最后一个有效(判断有效？)从机是否加载数据完成	
		if(i >= BOX_HANG)//最后一个从控区域左移完全移出后需延时
		  delay_ms(time);
		if(G_DisplayFlag == 0)
			return;
		LCPSet;
		delay_us(20);
		LCPReset;
		delay_us(20);
  }
}

void TranSlavesDis(uint8_t frame_no,uint16_t step)
{
	uint8_t i,*recp=NULL,err;
	uint8_t buf[20];
	uint16_t slaves_b;
	
	*(uint16_t *)(buf) = 8;//自留两位存放发送数据个数
	buf[3] = 0;//从机二级地址
	buf[4] = 2;//帧命令-播放帧
	*(uint16_t *)(buf+5) = 3;//数据长度
  buf[7] = frame_no;
  *(uint16_t *)(buf+8) = step;	
	OSSemPend(RS485Sem,0,&err);    //获取UART2资源信号量
  for(i=1;i<=SLAVE_NUM;i++)
	{
    buf[2] = i;//从机地址
		recp = RS485TranSlaves(buf,&slaves_b);
		Delay(400);  //100US-RS485方向改变后稳定下////////////////////////////////////////////////////////
		if(recp != NULL)
    memp_free(MEMP_PBUF_POOL,(void *)recp);//释放POOL内存	

  }
	OSSemPost(RS485Sem); //释放UART2资源信号量
}


/*************************************************/
//等待从机加载好数据
void WaitSlaveLoadOK (uint8_t slave_no,uint8_t *ack)
{
	uint8_t *ackp=NULL;
	uint8_t err;
	uint32_t crc32,rec32;
	uint8_t buf[20];
	uint16_t crc32num,i;
	*ack=0;
	buf[0] = slave_no;//最后一个从机地址
	buf[1] = 0;       //从机二级地址
	buf[2] = 5;       //帧命令-从机加载数据是否完成
	*(uint16_t *)(buf+3) = 0;      //数据长度	
	OSSemPend(RS485Sem,0,&err);    //获取UART2资源信号量
	RS485_TX_EN=1;                   //配置RS485为输出并延时
	*(INT32U *)(buf+5) = UserHWCRC32(buf,5);
	 i=800;                  //这个值的大小应该随着从空区域变换而变换///////////

	while(--i != 0)
	{
    
		if(G_DisplayFlag == 0)//如果有新数据则退出循环
			break;
		RS485TranConfig(buf,9);		     //配置UART2准备发送数据
		USART_ITConfig(USART3, USART_IT_TXE, ENABLE);
    delay_ms(2);
		while(RS485TranCnt < RS485TranLength);//等待发送完成
    
		delay_ms(1);//时间足够从机返回数据///////////////////////////
		ackp = OSQAccept(RS485RecMsgQeue,&err);
		if(ackp != NULL)//有返回数据
		{
			crc32num = *(INT16U *)ackp;
			crc32 = UserHWCRC32(ackp+2,crc32num-4);//CRC32校验
			rec32 = *(INT32U *)(ackp+2+crc32num-4);
			if(crc32 == rec32)//校验成功
			{
				if(*(ackp+2+3) == 0)//成功
				{
					RS485_TX_EN=1;
          memp_free(MEMP_PBUF_POOL,(void *)ackp);//释放POOL内存
          *ack=0;
          OSSemPost(RS485Sem); //释放UART2资源信号量  
					return ;
				}
				else//失败
				{
					RS485_TX_EN=1;;                         //配置RS485为输出并延时
          memp_free(MEMP_PBUF_POOL,(void *)ackp);//释放POOL内存	          
					Delay(3000);  
				}
			}
			else//校验错误
			{
				RS485_TX_EN=1;;                           //配置RS485为输出并延时

        memp_free(MEMP_PBUF_POOL,(void *)ackp);//释放POOL内存	        
				Delay(3000);           
			}
			slave_communication = slave_communication&(~(1<<(slave_no-1)));//第n个从机通信成功
		}
		else//无返回数据
		{
			slave_communication = slave_communication|(1<<(slave_no-1));//第n个从机通信失败
			RS485_TX_EN=1;;  //配置RS485为输出并延时
			Delay(3000);  
		}
	}
	OSSemPost(RS485Sem); //释放UART2资源信号量  
}





uint8_t UpdateItemToSlaves (void)
{
  int res;  
	FIL fdst;
	FATFS fs;
	UINT br=0;//, bw=0;            // File R/W count 
	BYTE file_name[30];
	INT8U err = 0;

	StrFrameHeader *frame_head = NULL;
  frame_head = memp_malloc(MEMP_PBUF_POOL);//申请一块内存pool	

  if(frame_head==NULL)
  {
     memp_free(MEMP_PBUF_POOL,(void *)frame_head);//释放POOL内存
     return 1;
  }
	OSSemPend(SDSem,0,&err);

	if(YuZhiFlag==1)
	{
		if(YuZhiXuHao<10)
		sprintf((char*)file_name,"0:/playlist/00%d",YuZhiXuHao);   // 文件名存储
		else
		sprintf((char*)file_name,"0:/playlist/0%d",YuZhiXuHao);   // 文件名存储
	} 
	else
	{
	
	if(temporary_flag == 0)//临时播放表标志位
		sprintf((char*)file_name,"0:/PLAY.lst");   // 文件名存储
	if(temporary_flag!=0)
		sprintf((char*)file_name,"0:/PLAYL.LST");   // 文件名存储
	}

		f_mount(0,&fs);
		res = f_open(&fdst,(char*)file_name, FA_OPEN_EXISTING | FA_READ);
		if(res!=FR_OK)
		{
			f_mount(0,NULL);
       OSSemPost(SDSem);
			return 1;
		}
		frame_head->pdata = ((INT8U *)frame_head + sizeof(StrFrameHeader));//定位pdata指针	
		res = f_read(&fdst,(INT16U *)frame_head->pdata, fdst.fsize, &br);//////////////////////////////////////
		frame_head->len = fdst.fsize;
		f_close(&fdst);/*关闭文件 */
		f_mount(0,NULL);		
	
	err = OSQPost(PlayListMsgQeue,(void *)frame_head);//发送播放帧表解析消息队列	
	while(err == OS_ERR_Q_FULL)  
	{
		OSTimeDly(1);
		OSQFlush(PlayListMsgQeue);
		err = OSQPost(PlayListMsgQeue,(void *)frame_head);  //发送播放帧表解析消息队列         
	}
	OSSemPost(SDSem);
	return 0;
}




void TranDataSetToSlaves (void)//发送参数设置到从机 颜色 行列点阵
{
	uint8_t i,*recp=NULL,err;
	uint8_t buf[100] = {0};
	uint16_t slaves_b;
	ScreenBoxDataStr datastr;
	
	datastr.Yanse = G_ScreenSet.YANSE;//屏体颜色种类
	datastr.SHang = G_ScreenSet.HANG;//一行点阵数
	datastr.SLie = G_ScreenSet.LIE;//一列点阵数
	
	datastr.BHang = G_BoxSet.HANG;
	datastr.BLie = G_BoxSet.LIE;
	datastr.MozuType = G_MoZuType;
	
	//自留两位存放发送数据个数
	*(uint16_t *)(buf) = sizeof(ScreenBoxDataStr)+5;
	buf[3] = 0;//从机二级地址
	buf[4] = 3;//帧命令-屏体/从控区域/模组参数
	*(uint16_t *)(buf+5) = sizeof(ScreenBoxDataStr);//数据长度
	recp = (uint8_t *)&datastr;
	for(i=0;i<sizeof(ScreenBoxDataStr);i++)
	{
    buf[7+i] = recp[i];
  }	
	OSSemPend(RS485Sem,0,&err);    //获取UART2资源信号量 
  for(i=1;i<=G_SlaveNum;i++)
	{
    buf[2] = i;//从机地址
		recp = RS485TranSlaves (buf,&slaves_b);
		Delay(200);  //100US-RS485方向改变后稳定下//////////////////////////////////////
		if(recp != NULL)
      memp_free(MEMP_PBUF_POOL,(void *)recp);//释放POOL内存	
  }
	OSSemPost(RS485Sem); //释放UART2资源信号量  
}

