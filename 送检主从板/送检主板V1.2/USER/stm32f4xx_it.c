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
	//NVIC_SystemReset();//��λ
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
  
  
//�Ӱ�ͨ��

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
  //����uCOS-II���ڽ����жϴ����ڷ����ź���ʱ�������������  
  OS_ENTER_CRITICAL();     //����ȫ���жϱ�־,�����ж�/* Tell uC/OS-II that we are starting an ISR*/
  OSIntNesting++;
  OS_EXIT_CRITICAL();	   //�ָ�ȫ���жϱ�־
  /* �����жϴ��� */
  if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)
  {
    /* Read one byte from the receive data register */

    USART_ClearFlag(USART3,USART_IT_RXNE);
    USART_ClearITPendingBit(USART3,USART_IT_RXNE);
//    rec_data = USART_ReceiveData(USART3);	//��ȡ���յ�������
			rec_data = USART3->DR;
//		USART_SendData(USART1, rec_data);	//�ڴ���1��ӡһ��
    if(rec_data == 0x100)//(G_IDHost|0x100))//������ַ 
	  {
      stage = 0;
      j = 0;
      if(startflag == 1)          //���ڽ���
      {
        startflag = 0;
        memp_free(MEMP_PBUF_POOL,(void *)backupp);
        p = NULL;
        backupp = NULL;
      }
      pq = (OS_Q *)RS485RecMsgQeue->OSEventPtr;
      if(pq->OSQEntries < pq->OSQSize)   //��Ϣ����δ��
      {
        p = memp_malloc(MEMP_PBUF_POOL);//����һ���ڴ�pool
        if(p != NULL)                   //�ڴ�����ɹ�
        {
          startflag = 1;            //��ʼ�������ݱ�־λ
          backupp = p;              //�����ڴ�ָ��
          p += 2;                   //�Լ�2Ϊ����������Ŀ��һ��λ��
          *p++ = G_IDHost;          //�洢������ַp[2]
					num2 = 0;
        }
        else//δ����ɹ�
        {
          startflag = 0;
          USART_ReceiverWakeUpCmd(USART3, ENABLE);       //���뾲Ĭģʽ
        }
      }
    }
	  else if(startflag == 1)//���ڽ���
	  {
			
      switch(stage)
      {
        case 0: //����֡�����
          *p++ = (uint8_t)rec_data;
          if(p-backupp == 8)  //���յ����ݳ�����һ����b . . . . . . p
						
          {
            num = (*(p-2)<<8) + *(p-1);

            if( num > 0)      //������������������
              stage = 1;
            else
              stage = 2;
          }
         if(p-backupp == 6)
         {

					 resetok[*(backupp+3)]=*p;	//�޸�Ӧ��λΪ�������͵Ĺ�����
         }
          break;
        case 1: //��������
          *p++ = (uint8_t)rec_data;
				num2++;
          /* ���ݽ�������ж� */
//					USART1->DR = 'A';
          if( num2 == num){
					  stage = 2;
//						printf("���� %d\r\n", p-backupp-2 - 1 + 4);
					} 

          break;
        case 2: //����У��
          *p++ = (uint8_t)rec_data; 
					j++;
//				USART1->DR = 'B';
          if(j == 4)
          {
            startflag = 0;
            DoorStflag=0;
						j = 0;
            
            *(INT16U *)backupp = p - backupp-2;            //�洢�������ݸ���
//						printf("backupp %d\r\n", *(INT16U *)backupp);
//						printf("num2 = %d\r\n", num2);
						
						
            USART_ReceiverWakeUpCmd(USART3, ENABLE);       //���뾲Ĭģʽ
            err = OSQPost(RS485RecMsgQeue,(void *)backupp);   //������Ϣ����
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
  //���ͻ��������ж�
  if(USART_GetITStatus(USART3, USART_IT_TXE) != RESET)
  {   
    /* Write one byte to the transmit data register */
  	USART_ClearITPendingBit(USART3,USART_IT_TXE);//��������жϱ�־λ
    if(RS485TranCnt < RS485TranLength)  //��������
    {
      if(RS485TranCnt == 0)
        USART_SendData(USART3, (RS485TranDataPot[RS485TranCnt++]|0x100)); //�׸�����Ϊ�ӻ���ַ����9λ��1	
      else
        USART_SendData(USART3, RS485TranDataPot[RS485TranCnt++]);

    }
    if(RS485TranCnt >= RS485TranLength)
    {
      /* �رշ��Ϳ��жϣ�������������ж� */
      USART_ITConfig(USART3, USART_IT_TXE, DISABLE);
      USART_ITConfig(USART3, USART_IT_TC, ENABLE);
    }    
  }
  //��������жϣ����һ�ֽڷ���
  else if(USART_GetITStatus(USART3, USART_IT_TC) != RESET)
  {
  	 USART_ITConfig(USART3,USART_IT_TC,DISABLE);
     RS485_TX_EN =0;                          //����RS485����Ϊ����  
  }
 
  OSIntExit();    //��os_core.c�ļ��ﶨ��,����и������ȼ������������,��ִ��һ�������л�  
}




















void USART1_IRQHandler(void)                	//����1�жϷ������
{
	u8 temp=0;
	StrFrameHeader *frame_head;//���ڶ���
	INT8U *memp_buf;//��������
	unsigned char err=0;
	unsigned short i;
	unsigned short iLong;//���ݳ���
	unsigned char escap_flag;//ת���־λ
	temp = err;
	err = temp;
	
	OSIntEnter();//����ϵͳ�����ж�
	if(USART_GetFlagStatus(USART1, USART_FLAG_IDLE) != RESET)
	{
  temp = USART1->SR;  
  temp = USART1->DR;//���IDLE�ж�
	temp = DMA_GetCurrDataCounter(DMA2_Stream2);//��ȡʣ��Ľ������ݵ���
	if(g_cSEND_BUF_SIZE[0] == 0x02)//�ж�֡ͷ
	{
		memp_buf = memp_malloc(MEMP_PBUF_POOL);//����һ���ڴ�pool
		frame_head = (StrFrameHeader *)memp_buf;
		frame_head->type = HEAD_USART1_TYPE;//����������
		frame_head->pdata = memp_buf + sizeof(StrFrameHeader);
		memp_buf =frame_head->pdata;         //��λ��������ָ��
		
		//==========�������ݳ���====================//
		for(iLong=0;g_cSEND_BUF_SIZE[iLong]!=0x03 && iLong<SEND_BUF_SIZE;iLong++);
		iLong++;//��������
		
		if(iLong < SEND_BUF_SIZE)//���Խ��������ݴ���
		{
			//==========ת�岢д�����==================//
			for(i=0;i<iLong;i++)//=======ת�뱨��
			{
				if(g_cSEND_BUF_SIZE[i] != 0x1B)     //��ת��
				{
					if(escap_flag == 1)    //�ϴ��Ƿ�Ϊת��
					{
						*memp_buf++ = g_cSEND_BUF_SIZE[i] + 0x1B;
						escap_flag = 0;
					}
					else
						*memp_buf++ = g_cSEND_BUF_SIZE[i];   
				}
				else
					escap_flag = 1;        //ת���־λ��1
			}
			frame_head->len = memp_buf-frame_head->pdata;//�����

#if EnableCode
			err = OSQPost(En_Dis_CodeQeue,(void *)frame_head);//�ѿ��������ݷ��͸����ܽ�������
#else
			err = OSQPost(CtrlCentRecMsgQeue,(void *)frame_head);//�ѿ��������ݷ��͸����ƴ�������
#endif
		}
	}
		if(err==1)
			{
				err=err;
			}
	MYDMA_Enable(DMA2_Stream2,SEND_BUF_SIZE);     //��������һ�δ���
	}
	
	//  //��������
//   if(USART_GetITStatus(USART1, USART_IT_TXE) != RESET)
//  {   
//    /* Write one byte to the transmit data register */
//  	USART_ClearITPendingBit(USART1,USART_IT_TXE);//��������жϱ�־λ
//		temp = USART1->SR; //�ȶ�SR,�ٶ�DR�������idle�жϵ����㣬����һֱ�����жϡ�
//		temp = USART1->DR;
//		printf("123\r\n");     
//  }
	OSIntExit();//����ϵͳ�뿪�ж�
} 



////������ͨ��
//void USART6_IRQHandler(void)                	//����6�жϷ������
//{
//	#if OS_CRITICAL_METHOD == 3u                     /* Allocate storage for CPU status register           */
//  OS_CPU_SR  cpu_sr = 0u;
//#endif
//  INT8U  err;
//  static INT8U tran_flag = 0;     //����-ת���־
//  OS_Q *pq;
//  uint8_t rec_data;               //����-����UART1���ݱ���
//  static INT8U *recpot = 0;       //����-����UART1���ݴ洢ָ��
////	static INT8U *backuppot = 0;  	//����-recpot�ĳ�ʼֵ����
//  static INT8U switch_flag = 0;   //����-ת���־	
//  static INT8U recstartflag = 0;  //����-���տ�ʼ��־λ
//  static StrFrameHeader  *frame_head;
//	
//  err = err;//��ֹ����
//  //����uCOS-II���ڽ����жϴ����ڷ����ź���ʱ�������������  
//  OS_ENTER_CRITICAL(); //����ȫ���жϱ�־,�����ж�/* Tell uC/OS-II that we are starting an ISR*/
//  OSIntNesting++;
//  OS_EXIT_CRITICAL();  //�ָ�ȫ���жϱ�־
//	
//  //�����ж�
//  if(USART_GetITStatus(USART6, USART_IT_RXNE) != RESET)
//  {
//		//IWDG_ReloadCounter();
//    /* Read one byte from the receive data register */
//	  USART_ClearFlag(USART6,USART_FLAG_RXNE);
//    USART_ClearITPendingBit(USART6,USART_IT_RXNE);//��������жϱ�־λ
//    rec_data = USART_ReceiveData(USART6);
//    if(rec_data == 0x06)	                      //�ж��Ƿ���֡ͷ
//    {	  
//      usartflag =1;//��������
////			FHDYflag=1;		
//      if(recstartflag == 1)                       //���ڽ���������һ֡�����ݣ������ͷ�δ�������
//      {
//        recstartflag = 0;
//        memp_free(MEMP_PBUF_POOL,(void *)frame_head);//�ͷ�POOL�ڴ�	
////				OSMemPut(MemPoint,frame_head);  //�ͷ��ڴ�
//        recpot = NULL;
//        frame_head = NULL;
//      }
//      pq = (OS_Q *)CtrlCentRecMsgQeue->OSEventPtr;
//      if(pq->OSQEntries < pq->OSQSize)       //��Ϣ����δ��
//      {
//         
//        recpot = memp_malloc(MEMP_PBUF_POOL);//����һ���ڴ�pool
////         recpot = OSMemGet(MemPoint,&err);//����һ���ڴ�
//        if(recpot != NULL)                   //�ڴ�����ɹ�
//        {
//          recstartflag = 1;                  //��ʼ���մ洢���ݱ�־λ��1
//          frame_head = (StrFrameHeader *)recpot;    //��λ��ͷ
//          frame_head->type = HEAD_USART6_TYPE;
//          frame_head->len = 0; 
//          frame_head->pdata = recpot + sizeof(StrFrameHeader);
//          recpot =frame_head->pdata;         //��λ��������ָ��
//        }
//        //���򷵻�һ֡������Ϣ
//      }
//	    //���Լ��϶�ʱ����һ��ʱ���������������ͷ��ڴ�
//	  }
//	  if(recstartflag == 1)					 //��ʼ��������
//	  {      	 
//	    if(rec_data != 0x1B)				     //��ת��
//	    {
//        if(switch_flag == 1)				 //�ϴ��Ƿ�Ϊת��
//		    {
//		      *recpot++ = rec_data+0x1B;
//		      switch_flag = 0;
//		    }
//		    else
//		      *recpot++ = rec_data;
//	    }						   
//	    else
//	      switch_flag = 1;
//	    if(rec_data == 0x03)					  //֡δ
//	    {
//				recstartflag = 0;         
//		    frame_head->len = recpot - frame_head->pdata;//�洢�������ݸ���
//		    err = OSQPost(CtrlCentRecMsgQeue,(void *)frame_head);//������Ϣ����
//// 		  if(err != OS_ERR_NONE )                      //���󡪡����ͻ�������
////        ���������ķ��ش�����Ϣ
//	    }
//    }
//	
//  } 
//  //��������
//   if(USART_GetITStatus(USART6, USART_IT_TXE) != RESET)
//  {   
//    /* Write one byte to the transmit data register */
//  	USART_ClearITPendingBit(USART6,USART_IT_TXE);//��������жϱ�־λ
//	if(UsartTranCnt ==0 || UsartTranCnt == UsartTranLength-1)  //��֡ͷ��֡β���д���
//	{
//      USART_SendData(USART6, UsartTranDataPot[UsartTranCnt++]);
//			tran_flag = 0;	
//	}
//	else
//	{
//	  if(tran_flag == 1)
//	  {
//	    USART_SendData(USART6,UsartTranDataPot[UsartTranCnt++]-0x1B);
//	    tran_flag = 0;			   //ת���־
//	  }
//	  //ת�崦��
//	  else
//	  {
//	    if(UsartTranDataPot[UsartTranCnt] == 0x02 || UsartTranDataPot[UsartTranCnt] == 0x03 ||
//           UsartTranDataPot[UsartTranCnt] == 0x1B)
//        {
//	      USART_SendData(USART6, 0X1B);
//	      tran_flag = 1;			 //ת���־
//	    }
//	    else
//	      USART_SendData(USART6, UsartTranDataPot[UsartTranCnt++]);
//	  }
//	}
//    if(UsartTranCnt >= UsartTranLength)
//    {
//      /* �رշ��Ϳ��жϣ�������������ж� */
//      USART_ITConfig(USART6, USART_IT_TXE, DISABLE);
//      USART_ITConfig(USART6, USART_IT_TC, ENABLE);
//    }    
//  }
//  //��������жϣ����һ�ֽڷ���
//  else if(USART_GetITStatus(USART6, USART_IT_TC) != RESET)
//  {
//		USART_ITConfig(USART6,USART_IT_TC,DISABLE);
//   
////    memp_free(MEMP_PBUF_POOL,(void *)(Uart1TranDataPot-2));	 //�ͷ�POOL�ڴ�	
////		OSMemPut(MemPoint,(Uart1TranDataPot-sizeof(StrFrameHeader)));  //�ͷ��ڴ�
//		OSSemPost(UsartTranSem); 
//  }
//  OSIntExit();    //��os_core.c�ļ��ﶨ��,����и������ȼ������������,��ִ��һ�������л�  
//} 
 

//PE6�ж�
void EXTI9_5_IRQHandler (void)
{
//	INT8U err;	
	OS_CPU_SR  cpu_sr = 0u;
  OS_ENTER_CRITICAL(); //����ȫ���жϱ�־,�����ж�/* Tell uC/OS-II that we are starting an ISR*/
  OSIntNesting++;
  OS_EXIT_CRITICAL();  //�ָ�ȫ���жϱ�־
	delay_ms(200);//����
	
	if(EXTI_GetITStatus(EXTI_Line6) != RESET)//�����ж�
	{
		KeyFlag++;
		if(KeyFlag>8)
			KeyFlag=1;
		checkself_flag = 0xff;//0xffȫ����ʾ����Ԫģ�������δ����
		OSQPost(PlayListMsgQeue,0);
	}
	EXTI_ClearITPendingBit(EXTI_Line6);
//	if(G_UpCpuFlag==0)
//		KeyFlag=0;
	 OSIntExit();    //��os_core.c�ļ��ﶨ��,����и������ȼ������������,��ִ��һ�������л�  
}
 

