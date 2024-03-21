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
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK STM32F407开发板
//IAP 代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//创建日期:2014/7/21
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 

iapfun jump2app; 
u32 iapbuf[512]; 	//2K字节缓存  
//appxaddr:应用程序的起始地址
//appbuf:应用程序CODE.
//appsize:应用程序大小(字节).
//void iap_write_appbin(u32 appxaddr,u8 *appbuf,u32 appsize)
//{
//	u32 t;
//	u16 i=0;
//	u32 temp;
//	u32 fwaddr=appxaddr;//当前写入的地址
//	u8 *dfu=appbuf;
//	for(t=0;t<appsize;t+=4)
//	{						   
//		temp=(u32)dfu[3]<<24;   
//		temp|=(u32)dfu[2]<<16;    
//		temp|=(u32)dfu[1]<<8;
//		temp|=(u32)dfu[0];	  
//		dfu+=4;//偏移4个字节
//		iapbuf[i++]=temp;	    
//		if(i==512)
//		{
//			i=0; 
//			STMFLASH_Write(fwaddr,iapbuf,512);
//			fwaddr+=2048;//偏移2048  512*4=2048
//		}
//	} 
//	if(i)STMFLASH_Write(fwaddr,iapbuf,i);//将最后的一些内容字节写进去.  
//}

////跳转到应用程序段
////appxaddr:用户代码起始地址.
//void iap_load_app(u32 appxaddr)
//{
//	if(((*(vu32*)appxaddr)&0x2FFE0000)==0x20000000)	//检查栈顶地址是否合法.
//	{ 
//		RCC_DeInit();
//		
//		__disable_irq();			//关中断
//		
//		TIM_DeInit(TIM9);
//		TIM_Cmd(TIM9,DISABLE); //失能定时器3
//		USART_DeInit(USART1);
//		USART_Cmd(USART1, DISABLE);  //失能串口1
//		ETH_DeInit();
//		ETH_DMAITConfig(ETH_DMA_IT_NIS|ETH_DMA_IT_R,DISABLE);  	//失能以太网接收中断
//		
//		jump2app=(iapfun)*(vu32*)(appxaddr+4);		//用户代码区第二个字为程序开始地址(复位地址)		
//		MSR_MSP(*(vu32*)appxaddr);					//初始化APP堆栈指针(用户代码区的第一个字用于存放栈顶地址)
//		jump2app();									//跳转到APP.
//	}
//}	



#define FLASH_IAP_ADDR 0x08000000


//ALIENTEK 探索者STM32F407开发板 实验11
//电容触摸按键实验 -库函数版本 
//技术支持：www.openedv.com
//淘宝店铺：http://eboard.taobao.com  
//广州市星翼电子科技有限公司  
//作者：正点原子 @ALIENTEK

void JUMPTOIAP(void)
{
//	STMFLASH_Write(FLASH_IAP_APP_FLAG_ADDR,&flag,1);
  
  
	__disable_irq();				//关中断
	
	RCC_DeInit();								//失能外设
	
//  OSTaskSuspend(TASK_CCH_PRIO);
//  OSTaskSuspend(TASK_RPL_PRIO);
//  OSTaskSuspend(TASK_DIS_CTRL_PRIO);
//  OSTaskSuspend(TASK_LED1_PRIO);
  
	TIM_DeInit(TIM9);
	TIM_Cmd(TIM9,DISABLE ); 		//失能定时器2
	
	USART_DeInit(USART1);
	USART_Cmd(USART1,DISABLE);	//失能串口1
	
  USART_DeInit(USART6);
	USART_Cmd(USART6,DISABLE);	//失能串口6
  
  USART_DeInit(USART2);
	USART_Cmd(USART2,DISABLE);	//失能串口4
  
  
 
	__set_CONTROL(0);				//进入用户线程模式
	
	__set_MSP(*(u32 *)FLASH_IAP_ADDR);
	
	(*((void(*)(void))(*(u32 *)(FLASH_IAP_ADDR+4))))();
}









