/**
  ******************************************************************************
  * �ļ��� : AD.h
  * ��  �� : �¹�
  * ��  �� ��V1.0
  * ʱ  �� : 2014��8��12��
  * ��  ˾ ��������缼�����޹�˾
  * ժ  Ҫ �����ļ�������ADģ��ĺ���ԭ������
  ******************************************************************************
  */

#ifndef __AD_H__
#define __AD_H__

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "includes.h"




extern  void User_ADConfig (void);
	 
extern  __IO uint16_t ADC1ConvertedValue;
extern void ADC3_CH5_DMA_Config(void);



#ifdef __cplusplus
}
#endif


#endif


