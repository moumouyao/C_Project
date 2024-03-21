#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "stmflash.h"
#include "iap.h" 
#include "misc.h"
#include "stm32f4x7_eth.h"
#include "ucos_ii.h"
#include "app_cfg.h"
#include "i2c.h"
#include "sw_global.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32F407������
//IAP ����	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2014/7/21
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 

iapfun jump2app; 
u32 iapbuf[512]; 	//2K�ֽڻ���  
//appxaddr:Ӧ�ó������ʼ��ַ
//appbuf:Ӧ�ó���CODE.
//appsize:Ӧ�ó����С(�ֽ�).
//void iap_write_appbin(u32 appxaddr,u8 *appbuf,u32 appsize)
//{
//	u32 t;
//	u16 i=0;
//	u32 temp;
//	u32 fwaddr=appxaddr;//��ǰд��ĵ�ַ
//	u8 *dfu=appbuf;
//	for(t=0;t<appsize;t+=4)
//	{						   
//		temp=(u32)dfu[3]<<24;   
//		temp|=(u32)dfu[2]<<16;    
//		temp|=(u32)dfu[1]<<8;
//		temp|=(u32)dfu[0];	  
//		dfu+=4;//ƫ��4���ֽ�
//		iapbuf[i++]=temp;	    
//		if(i==512)
//		{
//			i=0; 
//			STMFLASH_Write(fwaddr,iapbuf,512);
//			fwaddr+=2048;//ƫ��2048  512*4=2048
//		}
//	} 
//	if(i)STMFLASH_Write(fwaddr,iapbuf,i);//������һЩ�����ֽ�д��ȥ.  
//}

////��ת��Ӧ�ó����
////appxaddr:�û�������ʼ��ַ.
//void iap_load_app(u32 appxaddr)
//{
//	if(((*(vu32*)appxaddr)&0x2FFE0000)==0x20000000)	//���ջ����ַ�Ƿ�Ϸ�.
//	{ 
//		RCC_DeInit();
//		
//		__disable_irq();			//���ж�
//		
//		TIM_DeInit(TIM9);
//		TIM_Cmd(TIM9,DISABLE); //ʧ�ܶ�ʱ��3
//		USART_DeInit(USART1);
//		USART_Cmd(USART1, DISABLE);  //ʧ�ܴ���1
//		ETH_DeInit();
//		ETH_DMAITConfig(ETH_DMA_IT_NIS|ETH_DMA_IT_R,DISABLE);  	//ʧ����̫�������ж�
//		
//		jump2app=(iapfun)*(vu32*)(appxaddr+4);		//�û��������ڶ�����Ϊ����ʼ��ַ(��λ��ַ)		
//		MSR_MSP(*(vu32*)appxaddr);					//��ʼ��APP��ջָ��(�û��������ĵ�һ�������ڴ��ջ����ַ)
//		jump2app();									//��ת��APP.
//	}
//}	



#define FLASH_IAP_ADDR 0x08000000


//ALIENTEK ̽����STM32F407������ ʵ��11
//���ݴ�������ʵ�� -�⺯���汾 
//����֧�֣�www.openedv.com
//�Ա����̣�http://eboard.taobao.com  
//������������ӿƼ����޹�˾  
//���ߣ�����ԭ�� @ALIENTEK

void JUMPTOIAP(void)
{
//	STMFLASH_Write(FLASH_IAP_APP_FLAG_ADDR,&flag,1);
  
  
	__disable_irq();				//���ж�
	
	RCC_DeInit();								//ʧ������
	
//  OSTaskSuspend(TASK_CCH_PRIO);
//  OSTaskSuspend(TASK_RPL_PRIO);
//  OSTaskSuspend(TASK_DIS_CTRL_PRIO);
//  OSTaskSuspend(TASK_LED1_PRIO);
  
	TIM_DeInit(TIM9);
	TIM_Cmd(TIM9,DISABLE ); 		//ʧ�ܶ�ʱ��2
	
	USART_DeInit(USART1);
	USART_Cmd(USART1,DISABLE);	//ʧ�ܴ���1
	
  USART_DeInit(USART6);
	USART_Cmd(USART6,DISABLE);	//ʧ�ܴ���6
  
  USART_DeInit(USART2);
	USART_Cmd(USART2,DISABLE);	//ʧ�ܴ���4
  
  
 
	__set_CONTROL(0);				//�����û��߳�ģʽ
	
	__set_MSP(*(u32 *)FLASH_IAP_ADDR);
	
	(*((void(*)(void))(*(u32 *)(FLASH_IAP_ADDR+4))))();
}









