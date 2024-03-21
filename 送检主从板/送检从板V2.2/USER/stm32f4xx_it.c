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
#include "stdio.h"
#include "malloc.h"

#include "string.h"
#include "FreeRTOS.h"
#include "task.h"
#include "sw_config.h"
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
	printf("\n\rHardFault_Handler---err\r\n");
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
//void SVC_Handler(void)
//{
//}

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


/*
 *
 *����3�жϷ������
 *
 */

//void USART3_IRQHandler(void)               
//{
//	static uint8_t  stage = 0;
//  static uint8_t  j = 0;
//	static uint8_t  startflag = 0;
//	static uint16_t p = 0;
//	static uint16_t num = 0;
//	uint8_t *backupp=NULL;
//	uint16_t rec_data=0;
//	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)
//	{
//		rec_data=USART_ReceiveData(USART3);
//    if(rec_data == (G_IDSlave0|0x100))
//	  {
//			stage = 0;
//			j = 0;
//			if(startflag == 1)          //���ڽ���
//			{
//        startflag = 0;
//			  p = 0;
//      }else{
//          startflag = 1;            //��ʼ�������ݱ�־λ
//				  p += 2;                   //�Լ�2Ϊ����������Ŀ��һ��λ��
//				  USART3_BUF [p++] = G_IDSlave0;         //�洢��ַ
//			}
//		}	  else if(startflag == 1)
//	  {
//			switch(stage)
//			{
//				case 0: //����֡�����
//					USART3_BUF[p++] =(uint8_t)rec_data;
////					*p++ = (uint8_t)rec_data;
//					if(p == 7)  //���յ����ݳ�����һ����
//					{
//						num = (uint16_t )(USART3_BUF[p-2]);
//						if( num > 0)      //������������������
//							stage = 1;
//						else
//							stage = 2;
//					}
//					break;
//				case 1: //��������
//					USART3_BUF [p++] = (uint8_t)rec_data;
//					/* ���ݽ�������ж� */
//					if( p-7 == num) 
//					{
//						stage = 2;					
//					}

//					break;
//				case 2: //����У��
//						USART3_BUF [p++] = (uint8_t)rec_data;
//					if(j++ >= 3)
//					{
//						startflag = 0;
//						* (uint16_t *) (&USART3_BUF[0])=p;
////						USART_ReceiverWakeUpCmd(USART3, ENABLE);       //���뾲Ĭģʽ		
//						backupp=(uint8_t *)mymalloc(SRAMIN,p);
//						memcpy(backupp,USART3_BUF,p);
//						
//						memset(USART3_BUF,0xff,USART3_BUF_SIZE);
//						xQueueSendFromISR(RS485RecMsgQeue,(void *)&backupp,0);

//					}
//					break;
//			  default : break;
//	    }
//	  }
//	}
//	
//	
//	//���ͻ��������ж�
//  if(USART_GetITStatus(USART3, USART_IT_TXE) != RESET)
//  {   
//    /* Write one byte to the transmit data register */
//  	USART_ClearITPendingBit(USART3,USART_IT_TXE);//��������жϱ�־λ
//	  if(RS485TranCnt < RS485TranLength)  //��������
//	  {
//      if(RS485TranCnt == 0)
//				USART_SendData(USART3, (RS485TranDataPot[RS485TranCnt++]|0x100)); //�׸�����Ϊ������ַ����9λ��1
//			else
//			  USART_SendData(USART3, RS485TranDataPot[RS485TranCnt++]);
//	  }
//    if(RS485TranCnt >= RS485TranLength)
//    {
//      /* �رշ��Ϳ��жϣ�������������ж� */
//      USART_ITConfig(USART3, USART_IT_TXE, DISABLE);
// 			USART_ITConfig(USART3, USART_IT_TC, ENABLE);
//    }    
//  }
//	//��������жϣ����һ�ֽڷ���
//  else if(USART_GetITStatus(USART3, USART_IT_TC) != RESET)
//	{
//  	USART_ITConfig(USART3,USART_IT_TC,DISABLE);
//  }

//} 


void USART3_IRQHandler(void)  
{
	static uint8_t  stage = 0;
  static uint8_t  j = 0;
	static uint8_t  startflag = 0;
	static uint16_t p = 0;
	static uint16_t num = 0;
	uint8_t *backupp=NULL;
	uint16_t rec_data=0;
	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)
	{
		rec_data=USART_ReceiveData(USART3);
		if(rec_data == (G_IDSlave0|0x100))
		{
			stage = 0;
			j = 0;
			if(startflag == 1)          //���ڽ���
			{
        startflag = 0;
			  p = 0;
				memset(USART3_BUF,0xff,USART3_BUF_SIZE);
      }
			else{
          startflag = 1;            //��ʼ�������ݱ�־λ
				  p = 2;                   //�Լ�2Ϊ����������Ŀ��һ��λ��
				  USART3_BUF [p++] = G_IDSlave0;         //�洢��ַ
			}
		
			
		}
		 else if(startflag == 1)
		 {
		switch (stage)
		{
				case 0: //����֡�����
					USART3_BUF[p++] =(uint8_t)rec_data;
					if(p == 7)  //���յ����ݳ�����һ����
					{
//						num = (uint16_t )(USART3_BUF[p-2]);
									num = *(uint16_t *)( &USART3_BUF[p-2]);
						if( num > 0)      //������������������
							stage = 1;
						else
							stage = 2;
					}
					break;
				case 1: //��������
					USART3_BUF [p++] = (uint8_t)rec_data;
					/* ���ݽ�������ж� */
					if( p-7 == num) 
					{
						stage = 2;					
					}
					break;
				case 2: //����У��
						USART3_BUF [p++] = (uint8_t)rec_data;
					if(j++ >= 3)
					{
						startflag = 0;
						* (uint16_t *) (&USART3_BUF[0])=p;
						USART_ReceiverWakeUpCmd(USART3, ENABLE);       //���뾲Ĭģʽ		
						backupp=(uint8_t *)mymalloc(SRAMIN,p);
						memcpy(backupp,USART3_BUF,p);
						
						memset(USART3_BUF,0xff,USART3_BUF_SIZE);
						xQueueSendFromISR(RS485RecMsgQeue,(void *)&backupp,0);

					}
					break;
				default:break;
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
				USART_SendData(USART3, (RS485TranDataPot[RS485TranCnt++]|0x100)); //�׸�����Ϊ������ַ����9λ��1
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
		SLAVE485In;   //����RS485����Ϊ����
  }
}
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
