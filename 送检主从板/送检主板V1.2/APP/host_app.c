/**
  ******************************************************************************
  * �ļ��� : host_app.c
  * ��  �� : �¹�
  * ��  �� ��V1.0
  * ʱ  �� : 2014��7��30��
  * ��  ˾ ��������缼�����޹�˾
  * ժ  Ҫ �����ļ�����������������ӿ������������Ӧ������
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
	f_close(&fdst);/*�ر��ļ� */ 
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
  * ����   : ����6����������֡��������RGB���ݵ��ӻ�
  *		   
  * ����   : ��
  *
  *
  * ����ֵ : ��
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
		frame_head = OSQPend(PlayListMsgQeue,0,&err);  //��ȡ����֡����Ϣָ��

		if(G_DataSetFlag == 1)//��������֮��Ż���н���
		{
       if(KeyFlag>0&&KeyFlag<9 && temporary_flag==0)
			{
				PwmSet(128);
			}
			else 
      {
				for(i=0;(*(frame_head->pdata+i)) !='=';i++);//Ѱ�ҵ�һ��'='		
				
				addr = i;	//��¼��֡��ǰ'='������ƫ����
				frame_num=0;
				for(j=1;(*(frame_head->pdata+i+j)>='0')&&(*(frame_head->pdata+i+j)<='9');j++)
					frame_num = frame_num*10+(*(frame_head->pdata+i+j)-48);
				for(i=0;i<frame_num;i++)
				{

					Ana_flag=1;					//�������ڽ�����־λ
					ClrItemBuffer();//��������				
					IWDG_ReloadCounter();
					temp = ReadOneItem(frame_head->pdata+addr,frame_head->len-addr);//��ÿ֡���ݳ���
					IWDG_ReloadCounter();
					if(temp == 0)
					{
            ;
					}
					Itemlisttab[i*4]=ItemBuffer.itemlist.itemyyy;
					Itemlisttab[i*4+1]=ItemBuffer.itemlist.delay;
					Itemlisttab[i*4+2]=ItemBuffer.itemlist.mod;
					Itemlisttab[i*4+3]=ItemBuffer.itemlist.param;
         
					TranSlaveDataHandle(frame_num,i);//�����ӻ�  һ����һ��
					addr += temp;	
				}
        sprintf((char*)file_name,"0:/ITEMLIST.LST");   // �ļ����洢
        f_mount(0,&fs);
        res = f_open(&fdst,(char*)file_name, FA_CREATE_ALWAYS | FA_WRITE);
        if(res!=FR_OK)
        {
          f_mount(0,NULL);
          break;
        }
        f_write(&fdst,Itemlisttab,i*4,&bw);
        f_close(&fdst);
       
        sprintf((char*)file_name,"0:/ITEM_NUM.LST");   // �ļ����洢
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

        //��Ԥ�ò����б������д�� play.lst��
				if(YuZhiFlag == 1 && temporary_flag == 0)//��Ԥ�ò��ű�������ʱ���ű�
				{
					size = frame_head->len;
					f_mount(0,&fs);
					err = FILE_Write(&size,"play.lst",frame_head->pdata);
					f_mount(0,NULL);
				}

      }
      G_TranFrameCnt = 0;
      G_ItemUpdateFlag = 1;//����֡�������ɱ�־λ
      E2promWriteByte(EEROM_ITEM_UPDATE_FLAG_ADDR,G_ItemUpdateFlag);//д�뵽EEROM��//EEROM�д洢���±�־λ

     }
		else
      ;	
		G_DisplayFlag = 0;    //ֹͣ��ʾ	
		memp_free(MEMP_PBUF_POOL,(void *)frame_head);//�ͷ�POOL�ڴ�	
		IWDG_ReloadCounter();	
	}			
}


/**
  ******************************************************************************
  *                             TranSlaveDataHandle()
  *
  * ����   : ��һ�����ݷְ����͸��ӻ���ÿһ�ӻ�������������һ������
  *		   
  * ����   : num:Ҫ�������͵���֡��
  *          no :���η�������֡��
  *
  *
  * ����ֵ : ��
  ******************************************************************************
  */

void TranSlaveDataHandle (INT8U num,INT8U no)//֡������֡��
{
	INT16U test=0;
	INT8U  err,i,last=0;
	INT16U j,temp;
	INT8U  *pot=NULL,*p=NULL,*backpot=NULL;
  
  INT8U k,*retp=NULL;
	INT16U slaves_b;  
  
	p = (INT8U *)&ItemBuffer;
	temp = (sizeof(ItemList)+SCREEN_BYTE_NUM*SCREEN_YANSE)/2048; //һ֡�ļ��ּ��η���
	if((sizeof(ItemList)+SCREEN_BYTE_NUM*SCREEN_YANSE)%2048 != 0)
	last = 1;
  
	for(i=0;i<temp;i++)
	{
		G_TranFrameCnt++;//���ͼ�����
		pot = memp_malloc(MEMP_PBUF_POOL);   //����һ���ڴ�	
		test = 0;
		while(pot == NULL)
		{
			test++;
			if(test == 1)
			OSTimeDly(5);
			pot = memp_malloc(MEMP_PBUF_POOL);    //����һ���ڴ��������ݷ���	
		}
		backpot = pot;//����ָ��
		pot += 2;     //������λ��ŷ��͸���
		pot++;        //�ӻ���ַ
		*pot++ = 0;   //�ӻ�������ַ
		*pot++ = 1;   //��������
		*(INT16U *)pot = 2048+4;
		pot += 2;
		*pot++ = num; //֡����
		*pot++ = no;  //֡��
		*pot++ = temp+last;//һ֡���ݷ�Ϊ������
		*pot++ = i;   //�ڼ���������
		for(j=0;j<2048;j++)
		{
			*pot++ = *p++;
			if(p == &ItemBuffer.rgb_buf[0][SCREEN_BYTE_NUM])
				p = ItemBuffer.rgb_buf[1];
			else if(p == &ItemBuffer.rgb_buf[1][SCREEN_BYTE_NUM])
				p = ItemBuffer.rgb_buf[2];				
		}
    
    *(INT16U *)backpot = pot-backpot-2;  //���ݳ���
    OSSemPend(RS485Sem,0,&err);    //��ȡUART2��Դ�ź���

    for(k=1;k<=SLAVE_NUM;k++)
    {
      *(backpot+2) = k;//�ӻ���ַ		
      retp = RS485TranSlaves(backpot,&slaves_b);      //�ӻ�Ӱ����������
      if(retp != NULL)
        memp_free(MEMP_PBUF_POOL,(void *)retp);//�ͷ�POOL�ڴ�	
    }
   	OSSemPost(RS485Sem);			     //�ͷ�UART2��Դ�ź���
    memp_free(MEMP_PBUF_POOL,(void *)backpot);//�ͷ�POOL�ڴ�	
	}
	if(last == 1)
	{
		G_TranFrameCnt++;//���ͼ�����
		pot = memp_malloc(MEMP_PBUF_POOL);     //����һ���ڴ�	
		test = 0;
		while(pot == NULL)
		{
		  test++;
			if(test == 1)
		  OSTimeDly(5);
			pot = memp_malloc(MEMP_PBUF_POOL);     //����һ���ڴ��������ݷ���	
		}
		backpot = pot;                //����ָ��
		
		pot += 2;     //������λ��ŷ��͸���
		*pot++=0;     //�ӻ���ַ
		*pot++ = 0;   //�ӻ�������ַ
		*pot++ = 1;   //��������
		*(INT16U *)pot = (sizeof(ItemList)+SCREEN_BYTE_NUM*SCREEN_YANSE)%2048+4;
		pot += 2;
		*pot++ = num; //֡����
		*pot++ = no;  //֡��
		*pot++ = temp+last;//һ֡���ݷ�Ϊ������
		*pot++ = temp+last-1;    //�ڼ���������
		temp = (sizeof(ItemList)+SCREEN_BYTE_NUM*SCREEN_YANSE)%2048;//���һ֡�ļ���С
    
		for(j=0;j<temp;j++)
		{
			*pot++ = *p++;
			if(p == &ItemBuffer.rgb_buf[0][SCREEN_BYTE_NUM])        
				p = ItemBuffer.rgb_buf[1];
			else if(p == &ItemBuffer.rgb_buf[1][SCREEN_BYTE_NUM])
				p = ItemBuffer.rgb_buf[2];
		}
		*(INT16U *)backpot = pot-backpot-2;
 
    OSSemPend(RS485Sem,0,&err);    //��ȡUART3��Դ�ź���
    for(k=1;k <= SLAVE_NUM;k++)
    {
      *(backpot+2) = k;               //�ӻ���ַ		
      retp = RS485TranSlaves(backpot,&slaves_b);      //�ӻ�Ӱ����������
      if(retp != NULL)
       		memp_free(MEMP_PBUF_POOL,(void *)retp);//�ͷ�POOL�ڴ�	

    }
   	OSSemPost(RS485Sem);			     //�ͷ�UART2��Դ�ź���
    memp_free(MEMP_PBUF_POOL,(void *)backpot);//�ͷ�POOL�ڴ�	

  }

}




//��ȡ�ӻ�������Ϣ
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
//		G_TranFrameCnt++;//���ͼ�����
		pot = memp_malloc(MEMP_PBUF_POOL);   //����һ���ڴ�	
		test = 0;
		while(pot == NULL)
		{
			test++;
			if(test == 1)
			OSTimeDly(5);
			pot = memp_malloc(MEMP_PBUF_POOL);    //����һ���ڴ��������ݷ���	
		}
		backpot = pot;//����ָ��
		pot += 2;     //������λ��ŷ��͸���
		pot++;        //�ӻ���ַ
		*pot++ = 0;   //�ӻ�������ַ
		*pot++ = 6;   //��������
		*(INT16U *)pot = 0;
		pot += 2;
  
    *(INT16U *)backpot = pot-backpot-2;  //���ݳ���
    OSSemPend(RS485Sem,0,&err);    //��ȡUART2��Դ�ź���

    for(k=1;k<=SLAVE_NUM;k++)
    {
      *(backpot+2) = k;//�ӻ���ַ		
      retp = RS485TranSlaves(backpot,&slaves_b);      //�ӻ�Ӱ����������
      if(retp != NULL)
        memp_free(MEMP_PBUF_POOL,(void *)retp);//�ͷ�POOL�ڴ�	
    }
   	OSSemPost(RS485Sem);			     //�ͷ�UART2��Դ�ź���
    memp_free(MEMP_PBUF_POOL,(void *)backpot);//�ͷ�POOL�ڴ�	
//	}


}





INT8U * RS485TranSlaves(INT8U *p,INT16U *slaves_b)
{
	uint32_t crc32,rec32;
	uint16_t num,crc32num;
	uint8_t  err,j,k;//,sendslavenum = 0;
	uint8_t  *ackp = NULL;
	uint16_t rec_datalen;   //�ӻ��������ݳ��ȣ�-���������ݰ�ͷ
	
	num = *(INT16U *)p;     //Ҫ���͵����ݸ���
	RS485_TX_EN=1;            //����RS485Ϊ�������ʱ            //����RS485Ϊ�������ʱ


	*(INT32U *)(p+2+num) = UserHWCRC32(p+2,num);//У��
	if(num > 50)  //�������ݵ���
	{
		for(j=0;j<3;j++)
		{
			RS485TranConfig((p+2),num+4);		     //����UART2׼����������
			USART_ITConfig(USART3, USART_IT_TXE, ENABLE);

			ackp = OSQPend(RS485RecMsgQeue,500,&err);	
			if(ackp != NULL)//�����ݷ���
			{		
				slave_rst_flag=0;
       
          crc32num = *(INT16U *)ackp;
          crc32 = UserHWCRC32(ackp+2,crc32num-4);//CRC32У��
          rec32 = *(INT32U *)(ackp+2+crc32num-4);
          if(crc32 == rec32 )
          {
            if(*(ackp+2+3) == 0)//�ɹ�
            {						
              RS485_TX_EN=1;            //����RS485Ϊ�������ʱ
              *slaves_b |= 0x01<<(*(p+2)-1);       //�ӻ�Ӧ��ɹ���־λ
              rec_datalen = *(uint16_t *)(ackp+2+4);
                  
              if(rec_datalen == 0)
              {
                memp_free(MEMP_PBUF_POOL,ackp);//�ͷ�POOL�ڴ�	
                return 0;
              }
              else
              {
                return ackp;
              }
            }
            else//ʧ��
            {
              RS485_TX_EN=1;            //����RS485Ϊ�������ʱ
              memp_free(MEMP_PBUF_POOL,ackp);//�ͷ�POOL�ڴ�	

            	Delay(30);
            }
          
          }
          else //У��������ȷ
          {
            RS485_TX_EN=1;            //����RS485Ϊ�������ʱ
           memp_free(MEMP_PBUF_POOL,ackp);//�ͷ�POOL�ڴ�	

            Delay(30);           
          }
       
			}
			else//û�з�������
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
				RS485_TX_EN=1;            //����RS485Ϊ�������ʱ		
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
	else  //�������ݵ���  ����3��
	{
		for(j=0;j<3;j++)
		{
			RS485TranConfig((p+2),num+4);		     //����UART2׼����������
			USART_ITConfig(USART3, USART_IT_TXE, ENABLE); 
      delay_ms(10);
			while(RS485TranCnt < RS485TranLength){};//�ȴ��������//////////////////////////////
//				label:
      delay_ms(100);
			ackp = OSQPend(RS485RecMsgQeue,500,&err);	//������մӰ��Ӧ��
			if(ackp != NULL)//�з�������
			{
				slave_rst_flag=0;
				Keydebug=0;
				crc32num = *(INT16U *)ackp;
//				printf("crc32num = %d\r\n", crc32num);
				crc32 = UserHWCRC32(ackp+2,crc32num-4);//CRC32У��
				rec32 = *(uint32_t *)(ackp+2+crc32num-4);

//				printf("crc32 = %x,rec32 = %x\r\n", crc32, rec32);
				if(crc32 == rec32)//У��ɹ�
				{
							
					if(*(ackp+2+3) == 0)//�ɹ�
					{
//						printf("000000\r\n");
					RS485_TX_EN=1;            //����RS485Ϊ�������ʱ
						*slaves_b |= 0x01<<(*(p+2)-1);       //�ӻ�Ӧ��ɹ���־λ
						rec_datalen = *(uint16_t *)(ackp+2+4);
						
						
						if(rec_datalen == 0)
						{
							memp_free(MEMP_PBUF_POOL,ackp);//�ͷ�POOL�ڴ�	
							return 0;
						}
						else
            {
              return ackp;
            }	
					}
					else if(*(ackp+2+3) ==6){	//���յ��ĴӰ������Ϣ
//						printf("666666\r\n");
						RS485_TX_EN=1;            //����RS485Ϊ�������ʱ
						*slaves_b |= 0x01<<(*(p+2)-1);       //�ӻ�Ӧ��ɹ���־λ
						err = OSQPost(InfmBoard_ErrQeue,ackp);	//��������Ϣ��������
						return NULL;
					}
					else if(*(ackp+2+3) ==7){	//���յ���������Ϣ
//						printf("77777\r\n");
//						goto label;
						RS485_TX_EN=1;            //����RS485Ϊ�������ʱ
						*slaves_b |= 0x01<<(*(p+2)-1);       //�ӻ�Ӧ��ɹ���־λ
						err = OSQPost(InfmBoardLed_ErrQeue,ackp);	//��������Ϣ��������
						if(err == OS_ERR_NONE){
//							printf("InfmBoardLed_ErrQeue successful\r\n");
							return NULL;
						}
						else{
//							printf("InfmBoardLed_ErrQeue err\r\n");
						}
					}
					else//ʧ��
					{
//						printf("recv err\r\n");
						RS485_TX_EN=1;            //����RS485Ϊ�������ʱ
						memp_free(MEMP_PBUF_POOL,ackp);//�ͷ�POOL�ڴ�	
						Delay(30);  
					}
				}
				else//У�����
				{
					RS485_TX_EN=1;            //����RS485Ϊ�������ʱ
					memp_free(MEMP_PBUF_POOL,ackp);//�ͷ�POOL�ڴ�	
				  Delay(30);           
				}
				slave_communication = slave_communication&(~(1<<(*(p+2)-1)));//��n���ӻ�ͨ�ųɹ�
			}
			else//�޷�������
			{
//				printf("none data\r\n");
				slave_communication = slave_communication|(1<<(*(p+2)-1));//��n���ӻ�ͨ��ʧ��
				slave_rst_flag++;
        //���δӻ�δ������Ӧ �͸�λ�ӻ�
        if(slave_rst_flag>=2 && slave_rst_flag<5)
				{
					SLAVERSTReset;
					OSTimeDly(5);
					SLAVERSTSet;
          Delay(30);
				}
				else if(slave_rst_flag>=5)
					slave_rst_flag = 5;
			RS485_TX_EN=1;            //����RS485Ϊ�������ʱ			
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


//��ȡ�ӻ���������Ϣ
void GetSlaveLedErrData(uint8_t num)
{
	INT16U test=0;
	INT8U  err,i;
	INT16U temp;//j,
	INT8U  *pot=NULL,*backpot=NULL;//,*p=NULL
  
  INT8U k,*retp=NULL;
	INT16U slaves_b;  
  
		pot = memp_malloc(MEMP_PBUF_POOL);   //����һ���ڴ�	
		test = 0;
		while(pot == NULL)
		{
			test++;
			if(test == 1)
			OSTimeDly(5);
			pot = memp_malloc(MEMP_PBUF_POOL);    //����һ���ڴ��������ݷ���	
		}
		backpot = pot;//����ָ��
		pot += 2;     //������λ��ŷ��͸���
		pot++;        //�ӻ���ַ
		*pot++ = 0;   //�ӻ�������ַ
		*pot++ = 7;   //�������ͣ�������
		
		*(INT16U *)pot = 1;
		pot += 2;
		*pot++ = num;
    *(INT16U *)backpot = pot-backpot-2;  //���ݳ���
    OSSemPend(RS485Sem,0,&err);    //��ȡUART2��Դ�ź���

    for(k=1;k<=SLAVE_NUM;k++)
    {
      *(backpot+2) = k;//�ӻ���ַ		
      retp = RS485TranSlaves(backpot,&slaves_b);      //�ӻ�Ӱ����������
      if(retp != NULL)
        memp_free(MEMP_PBUF_POOL,(void *)retp);//�ͷ�POOL�ڴ�	
			printf("�ӻ���ַ %d\r\n", k);
    }
   	OSSemPost(RS485Sem);			     //�ͷ�UART2��Դ�ź���
    memp_free(MEMP_PBUF_POOL,(void *)backpot);//�ͷ�POOL�ڴ�	
//	}


}








