#include "key.h"
#include "sw_global.h"
#include "delay.h"
#include "dis_app.h"
#include "memp.h"
#include "delay.h"
#include "host_app.h"
//情报板 PC4-1 PC5-2  按键输入
void SPARE_Gpio(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOC | RCC_AHB1Periph_GPIOE, ENABLE);

  GPIO_InitStructure.GPIO_Pin= GPIO_Pin_4|GPIO_Pin_5;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOC,&GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin= GPIO_Pin_13;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOE,&GPIO_InitStructure);

}


void Key_UpdateItemToSlaves(uint8_t keyvalue)
{
   YuZhiXuHao=0;
   YuZhiFlag=0;
	switch(keyvalue)
   {
			case 1:
			YuZhiXuHao=83;
			YuZhiFlag=1; 
			Updatekey();
         break;
      case 2:
			YuZhiXuHao=82;
			YuZhiFlag=1;
			Updatekey();
         break;
			case 3:
			YuZhiXuHao=81;
			YuZhiFlag=1;
			Updatekey();
         break;
			case 4:
			YuZhiXuHao=86;
			YuZhiFlag=1;
			Updatekey();
         break;
			case 5:
			YuZhiXuHao=85;
			YuZhiFlag=1;
			Updatekey();
         break;
			case 6:
			YuZhiXuHao=84;
			YuZhiFlag=1;
			Updatekey();
         break;
			case 7:
			YuZhiXuHao=87;
			YuZhiFlag=1;
			Updatekey();
         break;
      default:
      break;
   }

}

uint8_t Updatekey (void)
{
  int res;  
	FATFS fs;
	UINT br=0;
	BYTE file_name[30];
	INT8U err = 0;
	FIL fdst;
	INT8U *memp_buf;//报文数据
	StrFrameHeader *frame_head = NULL;
  memp_buf = memp_malloc(MEMP_PBUF_POOL);//申请一块内存pool	
	frame_head=(StrFrameHeader *)memp_buf;
  if(frame_head==NULL)
  {
     memp_free(MEMP_PBUF_POOL,(void *)frame_head);//释放POOL内存
     return 1;
  }
		OSSemPend(SDSem,0,&err);
	if(YuZhiFlag == 1)
	{
		if(YuZhiXuHao<10)
			sprintf((char*)file_name,"0:/playlist/00%d",YuZhiXuHao);   // 文件名存储  
		else if(YuZhiXuHao>10 && YuZhiXuHao<100)
			sprintf((char*)file_name,"0:/playlist/0%d",YuZhiXuHao);   // 文件名存储
		else if(YuZhiXuHao>100)
			sprintf((char*)file_name,"0:/playlist/%d",YuZhiXuHao);   // 文件名存储		
		f_mount(0,&fs);
		res = f_open(&fdst,(char*)file_name, FA_READ);
		if(res!=FR_OK)
		{
			f_mount(0,NULL);
       OSSemPost(SDSem);
			return 1;
		}

		frame_head->pdata = memp_buf + sizeof(StrFrameHeader);
		memp_buf =frame_head->pdata;         //定位接收数据指针
		
		res = f_read(&fdst,memp_buf, fdst.fsize, &br);//感觉不是很对，memp_buf后续没用到
		frame_head->len = fdst.fsize;
		f_close(&fdst);/*关闭文件 */
		f_mount(0,NULL);		
	}
	OSSemPost(SDSem);	
	
	err = OSQPost(PlayListMsgQeue,(void *)frame_head);//发送播放帧表解析消息队列	
	while(err == OS_ERR_Q_FULL)  
	{
		OSTimeDly(1);
//		OSQFlush(PlayListMsgQeue);
		err = OSQPost(PlayListMsgQeue,(void *)frame_head);  //发送播放帧表解析消息队列         
	}

//	 memp_free(MEMP_PBUF_POOL,(void *)frame_head);//释放POOL内存
	return 0;
}

//uint8_t Key_Value(void)
//{
////ANJIAN1  PA0
////ANJIAN2  PC3
////ANJIAN3  PC2
//	//	static uint8_t key_flag1=0;
////	static uint8_t key_flag2=0;
//	if((GPIOA->IDR&GPIO_Pin_0)==0)//按下
//	{
//		OSTimeDly(5);
//			return 1;
//  	}
//	//if( GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_4) == 0)
//	else if((GPIOC->IDR&GPIO_Pin_3) ==0)
//	{
//		OSTimeDly(5);
//			return 2;
//	}
//	else if((GPIOC->IDR&GPIO_Pin_2) ==0)
//	{
//		OSTimeDly(5);
//			return 3;
//	}
//	return 0;	
//}

uint8_t Key_Value(void)
{
  if((GPIOA->IDR&GPIO_Pin_0)==0 && (GPIOC->IDR&GPIO_Pin_3) ==0&&(GPIOC->IDR&GPIO_Pin_2) ==0)
  {
    delay_ms(5);
			return 7;
  }
  else
  {
    if( (GPIOA->IDR&GPIO_Pin_0) == 0 )//按下
    {
      delay_ms(3);
      if((GPIOC->IDR&GPIO_Pin_3) ==0)
      {
        delay_ms(3);
        return 4;
      }
      else if((GPIOC->IDR&GPIO_Pin_2) ==0)
      {
        delay_ms(3);
        return 5;
      }
      else
      {
        delay_ms(5);
        return 1;
      }  
    }
    else if((GPIOC->IDR&GPIO_Pin_3) ==0)
    {
      delay_ms(2);
      if((GPIOC->IDR&GPIO_Pin_2) ==0)
      {
        delay_ms(3);
        return 6;
      }
      else
      {
        delay_ms(3);
        return 2;
      }
    }
    else if((GPIOC->IDR&GPIO_Pin_2) ==0)
    {
      delay_ms(5);
      return 3;
    }
  }
	return 0;	
}

void EXTI_UserKey_Config(void)
{
  GPIO_InitTypeDef GPIO_InitStructure; 
	EXTI_InitTypeDef EXTI_InitStructure;
//  NVIC_InitTypeDef NVIC_InitStructure;
	/* config the extiline(PA0) clock and AFIO clock */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC|RCC_AHB1Periph_GPIOE|RCC_AHB1Periph_GPIOA,ENABLE);
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
	
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;  
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
//ANJIAN1  PA0  ANJIAN2  PC3   ANJIAN3  PC2
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;  
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3|GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;  
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;  
	GPIO_Init(GPIOE, &GPIO_InitStructure);

	
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE, EXTI_PinSource6);    
	EXTI_InitStructure.EXTI_Line = EXTI_Line6;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);
}


