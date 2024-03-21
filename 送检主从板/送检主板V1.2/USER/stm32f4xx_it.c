/**
  ******************************************************************************
  * @file    Project/STM32F4xx_StdPeriph_Templates/stm32f4xx_it.c 
  * @author  MCD Application Team
  * @version V1.4.0
  * @date    04-August-2014
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2014 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_it.h"
#include "memp.h" 
#include "stdio.h"
#include "sw_global.h"
#include "usart.h"
#include "sw_host.h"
#include "lwip/mem.h"
#include "lwip/memp.h"
#include "malloc.h"
#include "delay.h"
#include "lsens.h"
/** @addtogroup Template_Project
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M4 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
	printf("error\r\n");
	//NVIC_SystemReset();//复位
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
//void PendSV_Handler(void)
//{
//}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
//void SysTick_Handler(void)
//{
// 
//}

/******************************************************************************/
/*                 STM32F4xx Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f4xx.s).                                               */
/******************************************************************************/

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/

/**
  * @}
  */ 
  
  
//从板通信

void USART3_IRQHandler(void)
{
  static uint8_t  stage = 0;
  static uint8_t  j = 0;
  static uint8_t  startflag = 0;
  static uint8_t *p = 0;
  static uint8_t *backupp = 0;
  static uint16_t num = 0, num2 = 0;
  uint16_t  rec_data;
  OS_Q *pq;
  uint8_t err;
#if OS_CRITICAL_METHOD == 3u                     /* Allocate storage for CPU status register           */
  OS_CPU_SR  cpu_sr = 0u;
#endif
  
  err = err;
  //告诉uCOS-II正在进行中断处理，在发送信号量时不进行任务调度  
  OS_ENTER_CRITICAL();     //保存全局中断标志,关总中断/* Tell uC/OS-II that we are starting an ISR*/
  OSIntNesting++;
  OS_EXIT_CRITICAL();	   //恢复全局中断标志
  /* 接收中断处理 */
  if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)
  {
    /* Read one byte from the receive data register */

    USART_ClearFlag(USART3,USART_IT_RXNE);
    USART_ClearITPendingBit(USART3,USART_IT_RXNE);
//    rec_data = USART_ReceiveData(USART3);	//读取接收到的数据
			rec_data = USART3->DR;
//		USART_SendData(USART1, rec_data);	//在串口1打印一下
    if(rec_data == 0x100)//(G_IDHost|0x100))//主机地址 
	  {
      stage = 0;
      j = 0;
      if(startflag == 1)          //正在接收
      {
        startflag = 0;
        memp_free(MEMP_PBUF_POOL,(void *)backupp);
        p = NULL;
        backupp = NULL;
      }
      pq = (OS_Q *)RS485RecMsgQeue->OSEventPtr;
      if(pq->OSQEntries < pq->OSQSize)   //消息队列未满
      {
        p = memp_malloc(MEMP_PBUF_POOL);//申请一块内存pool
        if(p != NULL)                   //内存申请成功
        {
          startflag = 1;            //开始接收数据标志位
          backupp = p;              //备份内存指针
          p += 2;                   //自加2为保存数据数目留一个位置
          *p++ = G_IDHost;          //存储主机地址p[2]
					num2 = 0;
        }
        else//未申请成功
        {
          startflag = 0;
          USART_ReceiverWakeUpCmd(USART3, ENABLE);       //进入静默模式
        }
      }
    }
	  else if(startflag == 1)//正在接收
	  {
			
      switch(stage)
      {
        case 0: //接收帧命令包
          *p++ = (uint8_t)rec_data;
          if(p-backupp == 8)  //接收到数据长度这一数据b . . . . . . p
						
          {
            num = (*(p-2)<<8) + *(p-1);

            if( num > 0)      //如果有数据则接受数据
              stage = 1;
            else
              stage = 2;
          }
         if(p-backupp == 6)
         {

					 resetok[*(backupp+3)]=*p;	//修改应答位为主机发送的功能码
         }
          break;
        case 1: //接收数据
          *p++ = (uint8_t)rec_data;
				num2++;
          /* 数据接收完成判断 */
//					USART1->DR = 'A';
          if( num2 == num){
					  stage = 2;
//						printf("长度 %d\r\n", p-backupp-2 - 1 + 4);
					} 

          break;
        case 2: //接收校验
          *p++ = (uint8_t)rec_data; 
					j++;
//				USART1->DR = 'B';
          if(j == 4)
          {
            startflag = 0;
            DoorStflag=0;
						j = 0;
            
            *(INT16U *)backupp = p - backupp-2;            //存储接收数据个数
//						printf("backupp %d\r\n", *(INT16U *)backupp);
//						printf("num2 = %d\r\n", num2);
						
						
            USART_ReceiverWakeUpCmd(USART3, ENABLE);       //进入静默模式
            err = OSQPost(RS485RecMsgQeue,(void *)backupp);   //发送消息队列
						if(err == OS_ERR_NONE){
//							printf(" 485 successful\r\n");
						}
						else{
							printf("485 err\r\n");
						}
            
          }
          break;
        default : break;
	    }
	  }
  }
  //发送缓冲区空中断
  if(USART_GetITStatus(USART3, USART_IT_TXE) != RESET)
  {   
    /* Write one byte to the transmit data register */
  	USART_ClearITPendingBit(USART3,USART_IT_TXE);//清除发送中断标志位
    if(RS485TranCnt < RS485TranLength)  //发送数据
    {
      if(RS485TranCnt == 0)
        USART_SendData(USART3, (RS485TranDataPot[RS485TranCnt++]|0x100)); //首个数据为从机地址，第9位置1	
      else
        USART_SendData(USART3, RS485TranDataPot[RS485TranCnt++]);

    }
    if(RS485TranCnt >= RS485TranLength)
    {
      /* 关闭发送空中断，开启发送完成中断 */
      USART_ITConfig(USART3, USART_IT_TXE, DISABLE);
      USART_ITConfig(USART3, USART_IT_TC, ENABLE);
    }    
  }
  //发送完成中断，最后一字节发送
  else if(USART_GetITStatus(USART3, USART_IT_TC) != RESET)
  {
  	 USART_ITConfig(USART3,USART_IT_TC,DISABLE);
     RS485_TX_EN =0;                          //配置RS485方向为输入  
  }
 
  OSIntExit();    //在os_core.c文件里定义,如果有更高优先级的任务就绪了,则执行一次任务切换  
}




















void USART1_IRQHandler(void)                	//串口1中断服务程序
{
	u8 temp=0;
	StrFrameHeader *frame_head;//用于队列
	INT8U *memp_buf;//报文数据
	unsigned char err=0;
	unsigned short i;
	unsigned short iLong;//数据长度
	unsigned char escap_flag;//转义标志位
	temp = err;
	err = temp;
	
	OSIntEnter();//告诉系统进入中断
	if(USART_GetFlagStatus(USART1, USART_FLAG_IDLE) != RESET)
	{
  temp = USART1->SR;  
  temp = USART1->DR;//清除IDLE中断
	temp = DMA_GetCurrDataCounter(DMA2_Stream2);//获取剩余的接收数据的量
	if(g_cSEND_BUF_SIZE[0] == 0x02)//判断帧头
	{
		memp_buf = memp_malloc(MEMP_PBUF_POOL);//申请一块内存pool
		frame_head = (StrFrameHeader *)memp_buf;
		frame_head->type = HEAD_USART1_TYPE;//代表串口数据
		frame_head->pdata = memp_buf + sizeof(StrFrameHeader);
		memp_buf =frame_head->pdata;         //定位接收数据指针
		
		//==========计算数据长度====================//
		for(iLong=0;g_cSEND_BUF_SIZE[iLong]!=0x03 && iLong<SEND_BUF_SIZE;iLong++);
		iLong++;//矫正长度
		
		if(iLong < SEND_BUF_SIZE)//如果越界代表数据错误
		{
			//==========转义并写入队列==================//
			for(i=0;i<iLong;i++)//=======转译报文
			{
				if(g_cSEND_BUF_SIZE[i] != 0x1B)     //非转义
				{
					if(escap_flag == 1)    //上次是否为转义
					{
						*memp_buf++ = g_cSEND_BUF_SIZE[i] + 0x1B;
						escap_flag = 0;
					}
					else
						*memp_buf++ = g_cSEND_BUF_SIZE[i];   
				}
				else
					escap_flag = 1;        //转义标志位置1
			}
			frame_head->len = memp_buf-frame_head->pdata;//命令长度

#if EnableCode
			err = OSQPost(En_Dis_CodeQeue,(void *)frame_head);//把拷贝的数据发送给加密解密任务
#else
			err = OSQPost(CtrlCentRecMsgQeue,(void *)frame_head);//把拷贝的数据发送给控制处理任务
#endif
		}
	}
		if(err==1)
			{
				err=err;
			}
	MYDMA_Enable(DMA2_Stream2,SEND_BUF_SIZE);     //重新启动一次传输
	}
	
	//  //发送数据
//   if(USART_GetITStatus(USART1, USART_IT_TXE) != RESET)
//  {   
//    /* Write one byte to the transmit data register */
//  	USART_ClearITPendingBit(USART1,USART_IT_TXE);//清除发送中断标志位
//		temp = USART1->SR; //先读SR,再读DR才能完成idle中断的清零，否则一直进入中断。
//		temp = USART1->DR;
//		printf("123\r\n");     
//  }
	OSIntExit();//告诉系统离开中断
} 



////串口屏通信
//void USART6_IRQHandler(void)                	//串口6中断服务程序
//{
//	#if OS_CRITICAL_METHOD == 3u                     /* Allocate storage for CPU status register           */
//  OS_CPU_SR  cpu_sr = 0u;
//#endif
//  INT8U  err;
//  static INT8U tran_flag = 0;     //发送-转义标志
//  OS_Q *pq;
//  uint8_t rec_data;               //接收-接收UART1数据变量
//  static INT8U *recpot = 0;       //接收-接收UART1数据存储指针
////	static INT8U *backuppot = 0;  	//接收-recpot的初始值备份
//  static INT8U switch_flag = 0;   //接收-转义标志	
//  static INT8U recstartflag = 0;  //接收-接收开始标志位
//  static StrFrameHeader  *frame_head;
//	
//  err = err;//防止警告
//  //告诉uCOS-II正在进行中断处理，在发送信号量时不进行任务调度  
//  OS_ENTER_CRITICAL(); //保存全局中断标志,关总中断/* Tell uC/OS-II that we are starting an ISR*/
//  OSIntNesting++;
//  OS_EXIT_CRITICAL();  //恢复全局中断标志
//	
//  //接收中断
//  if(USART_GetITStatus(USART6, USART_IT_RXNE) != RESET)
//  {
//		//IWDG_ReloadCounter();
//    /* Read one byte from the receive data register */
//	  USART_ClearFlag(USART6,USART_FLAG_RXNE);
//    USART_ClearITPendingBit(USART6,USART_IT_RXNE);//清除接收中断标志位
//    rec_data = USART_ReceiveData(USART6);
//    if(rec_data == 0x06)	                      //判断是否是帧头
//    {	  
//      usartflag =1;//串口数据
////			FHDYflag=1;		
//      if(recstartflag == 1)                       //正在接收中又来一帧新数据，则先释放未完成数据
//      {
//        recstartflag = 0;
//        memp_free(MEMP_PBUF_POOL,(void *)frame_head);//释放POOL内存	
////				OSMemPut(MemPoint,frame_head);  //释放内存
//        recpot = NULL;
//        frame_head = NULL;
//      }
//      pq = (OS_Q *)CtrlCentRecMsgQeue->OSEventPtr;
//      if(pq->OSQEntries < pq->OSQSize)       //消息队列未满
//      {
//         
//        recpot = memp_malloc(MEMP_PBUF_POOL);//申请一块内存pool
////         recpot = OSMemGet(MemPoint,&err);//申请一块内存
//        if(recpot != NULL)                   //内存申请成功
//        {
//          recstartflag = 1;                  //开始接收存储数据标志位置1
//          frame_head = (StrFrameHeader *)recpot;    //定位包头
//          frame_head->type = HEAD_USART6_TYPE;
//          frame_head->len = 0; 
//          frame_head->pdata = recpot + sizeof(StrFrameHeader);
//          recpot =frame_head->pdata;         //定位接收数据指针
//        }
//        //否则返回一帧错误信息
//      }
//	    //可以加上定时器，一段时间内若无数据则释放内存
//	  }
//	  if(recstartflag == 1)					 //开始接收数据
//	  {      	 
//	    if(rec_data != 0x1B)				     //非转义
//	    {
//        if(switch_flag == 1)				 //上次是否为转义
//		    {
//		      *recpot++ = rec_data+0x1B;
//		      switch_flag = 0;
//		    }
//		    else
//		      *recpot++ = rec_data;
//	    }						   
//	    else
//	      switch_flag = 1;
//	    if(rec_data == 0x03)					  //帧未
//	    {
//				recstartflag = 0;         
//		    frame_head->len = recpot - frame_head->pdata;//存储接收数据个数
//		    err = OSQPost(CtrlCentRecMsgQeue,(void *)frame_head);//发送消息队列
//// 		  if(err != OS_ERR_NONE )                      //错误——发送缓冲区满
////        给控制中心返回错误信息
//	    }
//    }
//	
//  } 
//  //发送数据
//   if(USART_GetITStatus(USART6, USART_IT_TXE) != RESET)
//  {   
//    /* Write one byte to the transmit data register */
//  	USART_ClearITPendingBit(USART6,USART_IT_TXE);//清除发送中断标志位
//	if(UsartTranCnt ==0 || UsartTranCnt == UsartTranLength-1)  //对帧头和帧尾进行处理
//	{
//      USART_SendData(USART6, UsartTranDataPot[UsartTranCnt++]);
//			tran_flag = 0;	
//	}
//	else
//	{
//	  if(tran_flag == 1)
//	  {
//	    USART_SendData(USART6,UsartTranDataPot[UsartTranCnt++]-0x1B);
//	    tran_flag = 0;			   //转义标志
//	  }
//	  //转义处理
//	  else
//	  {
//	    if(UsartTranDataPot[UsartTranCnt] == 0x02 || UsartTranDataPot[UsartTranCnt] == 0x03 ||
//           UsartTranDataPot[UsartTranCnt] == 0x1B)
//        {
//	      USART_SendData(USART6, 0X1B);
//	      tran_flag = 1;			 //转义标志
//	    }
//	    else
//	      USART_SendData(USART6, UsartTranDataPot[UsartTranCnt++]);
//	  }
//	}
//    if(UsartTranCnt >= UsartTranLength)
//    {
//      /* 关闭发送空中断，开启发送完成中断 */
//      USART_ITConfig(USART6, USART_IT_TXE, DISABLE);
//      USART_ITConfig(USART6, USART_IT_TC, ENABLE);
//    }    
//  }
//  //发送完成中断，最后一字节发送
//  else if(USART_GetITStatus(USART6, USART_IT_TC) != RESET)
//  {
//		USART_ITConfig(USART6,USART_IT_TC,DISABLE);
//   
////    memp_free(MEMP_PBUF_POOL,(void *)(Uart1TranDataPot-2));	 //释放POOL内存	
////		OSMemPut(MemPoint,(Uart1TranDataPot-sizeof(StrFrameHeader)));  //释放内存
//		OSSemPost(UsartTranSem); 
//  }
//  OSIntExit();    //在os_core.c文件里定义,如果有更高优先级的任务就绪了,则执行一次任务切换  
//} 
 

//PE6中断
void EXTI9_5_IRQHandler (void)
{
//	INT8U err;	
	OS_CPU_SR  cpu_sr = 0u;
  OS_ENTER_CRITICAL(); //保存全局中断标志,关总中断/* Tell uC/OS-II that we are starting an ISR*/
  OSIntNesting++;
  OS_EXIT_CRITICAL();  //恢复全局中断标志
	delay_ms(200);//消抖
	
	if(EXTI_GetITStatus(EXTI_Line6) != RESET)//进入中断
	{
		KeyFlag++;
		if(KeyFlag>8)
			KeyFlag=1;
		checkself_flag = 0xff;//0xff全屏显示（单元模块点亮，未做）
		OSQPost(PlayListMsgQeue,0);
	}
	EXTI_ClearITPendingBit(EXTI_Line6);
//	if(G_UpCpuFlag==0)
//		KeyFlag=0;
	 OSIntExit();    //在os_core.c文件里定义,如果有更高优先级的任务就绪了,则执行一次任务切换  
}
 

