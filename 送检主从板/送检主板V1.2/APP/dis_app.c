/**
  ******************************************************************************
  * �ļ��� : dis_app.c
  * ��  �� : �¹�
  * ��  �� ��V1.0
  * ʱ  �� : 2014��7��30��
  * ��  ˾ ��������缼�����޹�˾
  * ժ  Ҫ �����ļ������������������ƴӿ�������ʾӦ������
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
uint8_t PlayNum,NOWPlayNum;//��֡��
uint16_t      BrightAD=0;

ItemList PlayListTab[100];           //��ʾ֡�ṹ������


/**
  ******************************************************************************
  *                             Task_DisplayCtrl(void *p_arg)
  *
  * ����   : ����5��������ʾ����
  *		   
  * ����   : ��
  *
  *
  * ����ֵ : ��
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
	if(G_DataSetFlag == 1)//ÿ���ϵ綼�ᷢ������/�ӿ�����/ģ�����������ӻ�
	{
      TranDataSetToSlaves();//���Ͳ������õ��ӻ� ��ɫ ���е���
  }
	if(G_ItemFlag == 1 && G_ItemFlag == 1 && G_DataSetFlag == 1)//�������������� G_ItemFlag ��������������Σ�������д����
	{
	  UpdateItemToSlaves();	//�������ݵ��ӻ�
   }	
  while(1)
	{
again:
		UDPResetFlag=0;
   //�鱨�����������ɡ����������ݣ�����֡��������,��������֡�������
		if((G_DataSetFlag == 1 && G_DisplayFlag == 0 && G_ItemFlag == 1 && 
			  G_ItemUpdateFlag == 1 && G_TranFrameCnt == 0&&G_UpCpuFlag==0)||temporary_flag!=0)//Ԥ��ʱ��SD��ȡ
		{
			sprintf((char*)file_name,"0:/ITEM_NUM.LST");   // �ļ����洢
		
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
			f_close(&fdst);/*�ر��ļ� */
			f_mount(0,NULL);
	    sprintf((char*)file_name,"0:/ITEMLIST.LST");   // �ļ����洢
			if(PlayNum < 100)//������֡����
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
      
        f_close(&fdst);/*�ر��ļ� */
        f_mount(0,NULL);
        OSSemPost(SDSem);
        G_DisplayFlag = 1;
			}	
      G_UpCpuFlag=1;
    }
		else if(G_DataSetFlag == 1 && G_DisplayFlag == 0 && G_ItemFlag == 1 && 
			G_ItemUpdateFlag == 1 && G_TranFrameCnt == 0&&G_UpCpuFlag==1)
		{
		 	if(PlayNum < 100)//������֡����
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
        PlayHandle(&PlayListTab[i],i);//��ʾ�ڼ�֡����
				if(G_DisplayFlag == 0)        //��RGB���ݵ����˳���ʾ
					break;
      }
    }
		else
		{
			OSTimeDly(50); //�ȴ�50ms
		}
  }
}


/**
  ******************************************************************************
  *                             Task_DisplayCtrl(void *p_arg)
  *
  * ����   : ���ƴӻ�������ʾ��������
  *		   
  * ����   : itemp������֡ͷ���ݽṹָ��
  *        : frame_no:��ʾ��֡��
  *
  *
  * ����ֵ : ��
  ******************************************************************************
  */
uint8_t breakflag=0;//��ͣ��ʱ���
void PlayHandle(ItemList *itemp,INT8U frame_no)//��ʾ�ڼ�֡����
{
  uint16_t step=0;
	uint16_t step1=0;
	uint8_t time=0,shanshuo=0;
	uint16_t i,j;
	uint8_t ack=0;
  switch(itemp->mod/10)  //����
	{
    case 1://������ʾ
			step = 0;
			break;
		case 2://����
		  step = SCREEN_HANG-1;
			break;
		case 3://����
			step = SCREEN_HANG-1;
		  time = 1;//�ҽ�ʱ������ʱ����Ϊ���һ���ӻ��������ݿ�
			break;
		case 4://����
			step = SCREEN_LIE-1;
			break;
		case 5://����
			step = SCREEN_LIE-1;
			break;
		case 6://���Ҷ�� 
			step = 7;
			break;
		case 7://����Ҷ��
			step = 7;
			break;
		case 8://��˸
			step = 0;shanshuo=1;    //TIM_Cmd(TIM3,ENABLE);
			break;
		default:
			step = 0;
			break;
  }
	for(i=0;i<=step;i++)
	{
		TranSlavesDis(itemp->itemyyy,i);//���ʹӻ���������
		delay_ms(15+itemp->param*20);		// �ƶ��ٶ�
		OSTimeDly(5);
		WaitSlaveLoadOK(SLAVE_NUM,&ack);			//�����������һ���ӻ��Ƿ�����������
			if(i < SCREEN_HANG-BOX_HANG)		//����δ�������һ���ӿ�����ʱ����ʱ
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
	OSTimeDly(100);//ͣ��ʱ��
	else if(shanshuo==1) ////////////////////////shanshuo
	for(j=0;j<((itemp->delay)*100)/((itemp->param+1)*40)+2;j++)
	{
	  if(G_DisplayFlag == 0)break;
	 shanshuo=0;
	}	
	else if(breakflag==0)//��ͣ��ʱ��𿪣����������ݵ���ʱ�ɵ�һʱ�䴦��
	{
		breakflag=1;
		for(j=0;j<(itemp->delay*10);j++)
		{
			OSTimeDly(100);//ͣ��ʱ��
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
  switch(itemp->mod%10)  //�Ƴ�
	{
    case 1://������ʾ
			step1 = 1;
			break;
		case 2://����
		  step1 = SCREEN_HANG;
		  time = 3;//���ʱ������ʱ����Ϊ���һ���ӻ��������ݿ�///////////////////////////////////////////
			break;
		case 3://����
			step1 = SCREEN_HANG;
			break;
		case 4://����
			step1 = SCREEN_LIE;
			break;
		case 5://����
			step1 = SCREEN_LIE;
			break;
		case 6://���Ҷ�� 
			step1 = 8;
			break;
		case 7://����Ҷ��
			step1 = 8;
			break;
		case 8://��˸
      step1 = 1;
		  break;
		default:
			step1 = 1;
			break;
  }
	if(step1!=1)
	for(i=1;i<=step1;i++)
	{		
    TranSlavesDis(itemp->itemyyy,i+step);//���ʹӻ���������
    delay_ms(15+itemp->param*20);
		delay_ms(2);
		WaitSlaveLoadOK(SLAVE_NUM,&ack);  //�ȴ����һ����Ч(�ж���Ч��)�ӻ��Ƿ�����������	
		if(i >= BOX_HANG)//���һ���ӿ�����������ȫ�Ƴ�������ʱ
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
	
	*(uint16_t *)(buf) = 8;//������λ��ŷ������ݸ���
	buf[3] = 0;//�ӻ�������ַ
	buf[4] = 2;//֡����-����֡
	*(uint16_t *)(buf+5) = 3;//���ݳ���
  buf[7] = frame_no;
  *(uint16_t *)(buf+8) = step;	
	OSSemPend(RS485Sem,0,&err);    //��ȡUART2��Դ�ź���
  for(i=1;i<=SLAVE_NUM;i++)
	{
    buf[2] = i;//�ӻ���ַ
		recp = RS485TranSlaves(buf,&slaves_b);
		Delay(400);  //100US-RS485����ı���ȶ���////////////////////////////////////////////////////////
		if(recp != NULL)
    memp_free(MEMP_PBUF_POOL,(void *)recp);//�ͷ�POOL�ڴ�	

  }
	OSSemPost(RS485Sem); //�ͷ�UART2��Դ�ź���
}


/*************************************************/
//�ȴ��ӻ����غ�����
void WaitSlaveLoadOK (uint8_t slave_no,uint8_t *ack)
{
	uint8_t *ackp=NULL;
	uint8_t err;
	uint32_t crc32,rec32;
	uint8_t buf[20];
	uint16_t crc32num,i;
	*ack=0;
	buf[0] = slave_no;//���һ���ӻ���ַ
	buf[1] = 0;       //�ӻ�������ַ
	buf[2] = 5;       //֡����-�ӻ����������Ƿ����
	*(uint16_t *)(buf+3) = 0;      //���ݳ���	
	OSSemPend(RS485Sem,0,&err);    //��ȡUART2��Դ�ź���
	RS485_TX_EN=1;                   //����RS485Ϊ�������ʱ
	*(INT32U *)(buf+5) = UserHWCRC32(buf,5);
	 i=800;                  //���ֵ�Ĵ�СӦ�����Ŵӿ�����任���任///////////

	while(--i != 0)
	{
    
		if(G_DisplayFlag == 0)//��������������˳�ѭ��
			break;
		RS485TranConfig(buf,9);		     //����UART2׼����������
		USART_ITConfig(USART3, USART_IT_TXE, ENABLE);
    delay_ms(2);
		while(RS485TranCnt < RS485TranLength);//�ȴ��������
    
		delay_ms(1);//ʱ���㹻�ӻ���������///////////////////////////
		ackp = OSQAccept(RS485RecMsgQeue,&err);
		if(ackp != NULL)//�з�������
		{
			crc32num = *(INT16U *)ackp;
			crc32 = UserHWCRC32(ackp+2,crc32num-4);//CRC32У��
			rec32 = *(INT32U *)(ackp+2+crc32num-4);
			if(crc32 == rec32)//У��ɹ�
			{
				if(*(ackp+2+3) == 0)//�ɹ�
				{
					RS485_TX_EN=1;
          memp_free(MEMP_PBUF_POOL,(void *)ackp);//�ͷ�POOL�ڴ�
          *ack=0;
          OSSemPost(RS485Sem); //�ͷ�UART2��Դ�ź���  
					return ;
				}
				else//ʧ��
				{
					RS485_TX_EN=1;;                         //����RS485Ϊ�������ʱ
          memp_free(MEMP_PBUF_POOL,(void *)ackp);//�ͷ�POOL�ڴ�	          
					Delay(3000);  
				}
			}
			else//У�����
			{
				RS485_TX_EN=1;;                           //����RS485Ϊ�������ʱ

        memp_free(MEMP_PBUF_POOL,(void *)ackp);//�ͷ�POOL�ڴ�	        
				Delay(3000);           
			}
			slave_communication = slave_communication&(~(1<<(slave_no-1)));//��n���ӻ�ͨ�ųɹ�
		}
		else//�޷�������
		{
			slave_communication = slave_communication|(1<<(slave_no-1));//��n���ӻ�ͨ��ʧ��
			RS485_TX_EN=1;;  //����RS485Ϊ�������ʱ
			Delay(3000);  
		}
	}
	OSSemPost(RS485Sem); //�ͷ�UART2��Դ�ź���  
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
  frame_head = memp_malloc(MEMP_PBUF_POOL);//����һ���ڴ�pool	

  if(frame_head==NULL)
  {
     memp_free(MEMP_PBUF_POOL,(void *)frame_head);//�ͷ�POOL�ڴ�
     return 1;
  }
	OSSemPend(SDSem,0,&err);

	if(YuZhiFlag==1)
	{
		if(YuZhiXuHao<10)
		sprintf((char*)file_name,"0:/playlist/00%d",YuZhiXuHao);   // �ļ����洢
		else
		sprintf((char*)file_name,"0:/playlist/0%d",YuZhiXuHao);   // �ļ����洢
	} 
	else
	{
	
	if(temporary_flag == 0)//��ʱ���ű��־λ
		sprintf((char*)file_name,"0:/PLAY.lst");   // �ļ����洢
	if(temporary_flag!=0)
		sprintf((char*)file_name,"0:/PLAYL.LST");   // �ļ����洢
	}

		f_mount(0,&fs);
		res = f_open(&fdst,(char*)file_name, FA_OPEN_EXISTING | FA_READ);
		if(res!=FR_OK)
		{
			f_mount(0,NULL);
       OSSemPost(SDSem);
			return 1;
		}
		frame_head->pdata = ((INT8U *)frame_head + sizeof(StrFrameHeader));//��λpdataָ��	
		res = f_read(&fdst,(INT16U *)frame_head->pdata, fdst.fsize, &br);//////////////////////////////////////
		frame_head->len = fdst.fsize;
		f_close(&fdst);/*�ر��ļ� */
		f_mount(0,NULL);		
	
	err = OSQPost(PlayListMsgQeue,(void *)frame_head);//���Ͳ���֡�������Ϣ����	
	while(err == OS_ERR_Q_FULL)  
	{
		OSTimeDly(1);
		OSQFlush(PlayListMsgQeue);
		err = OSQPost(PlayListMsgQeue,(void *)frame_head);  //���Ͳ���֡�������Ϣ����         
	}
	OSSemPost(SDSem);
	return 0;
}




void TranDataSetToSlaves (void)//���Ͳ������õ��ӻ� ��ɫ ���е���
{
	uint8_t i,*recp=NULL,err;
	uint8_t buf[100] = {0};
	uint16_t slaves_b;
	ScreenBoxDataStr datastr;
	
	datastr.Yanse = G_ScreenSet.YANSE;//������ɫ����
	datastr.SHang = G_ScreenSet.HANG;//һ�е�����
	datastr.SLie = G_ScreenSet.LIE;//һ�е�����
	
	datastr.BHang = G_BoxSet.HANG;
	datastr.BLie = G_BoxSet.LIE;
	datastr.MozuType = G_MoZuType;
	
	//������λ��ŷ������ݸ���
	*(uint16_t *)(buf) = sizeof(ScreenBoxDataStr)+5;
	buf[3] = 0;//�ӻ�������ַ
	buf[4] = 3;//֡����-����/�ӿ�����/ģ�����
	*(uint16_t *)(buf+5) = sizeof(ScreenBoxDataStr);//���ݳ���
	recp = (uint8_t *)&datastr;
	for(i=0;i<sizeof(ScreenBoxDataStr);i++)
	{
    buf[7+i] = recp[i];
  }	
	OSSemPend(RS485Sem,0,&err);    //��ȡUART2��Դ�ź��� 
  for(i=1;i<=G_SlaveNum;i++)
	{
    buf[2] = i;//�ӻ���ַ
		recp = RS485TranSlaves (buf,&slaves_b);
		Delay(200);  //100US-RS485����ı���ȶ���//////////////////////////////////////
		if(recp != NULL)
      memp_free(MEMP_PBUF_POOL,(void *)recp);//�ͷ�POOL�ڴ�	
  }
	OSSemPost(RS485Sem); //�ͷ�UART2��Դ�ź���  
}

