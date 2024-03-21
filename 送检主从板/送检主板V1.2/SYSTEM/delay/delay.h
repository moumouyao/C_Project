//#ifndef __DELAY_H
//#define __DELAY_H 			   
//#include <sys.h>	  
////////////////////////////////////////////////////////////////////////////////////  
////������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
////ALIENTEK STM32F407������
////ʹ��SysTick����ͨ����ģʽ���ӳٽ��й���(֧��ucosii)
////����delay_us,delay_ms
////����ԭ��@ALIENTEK
////������̳:www.openedv.com
////�޸�����:2014/5/2
////�汾��V1.0
////��Ȩ���У�����ؾ���
////Copyright(C) ������������ӿƼ����޹�˾ 2014-2024
////All rights reserved
////********************************************************************************
////�޸�˵��
////��
//////////////////////////////////////////////////////////////////////////////////// 	 
//void delay_init(u8 SYSCLK);
//void delay_ms(u16 nms);
//void delay_us(u32 nus);
////void delay_ms (uint16_t time);
//#endif


/**
  ******************************************************************************
  * �ļ��� : user_delay.h
  * ��  �� : �¹�
  * ��  �� ��V1.0
  * ʱ  �� : 2014��7��31��
  * ��  ˾ ��������缼�����޹�˾
  * ժ  Ҫ �����ļ������������ʱ��������
  ******************************************************************************
  */

#ifndef __USER_DELAY_H__
#define __USER_DELAY_H__

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"
#include <sys.h>	
	 
void delay_init(u8 SYSCLK);
extern void Delay(uint32_t nCount);
extern void delay_us (uint32_t time);
extern void delay_ms (uint16_t time);






#ifdef __cplusplus
}
#endif


#endif
	



























