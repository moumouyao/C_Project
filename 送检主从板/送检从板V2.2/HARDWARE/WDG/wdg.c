#include "wdg.h"
#include "stm32f4xx.h"
//���Ź���������
//��Ƶ��:0~7��ֻ�е���λ��Ч��
//��Ƶ����=4*2^prer���������ֻ��Ϊ256
//rlr��װ�ؼĴ���ֵ.����12λ��Ч��
//ʱ����㣺��׼ȷʱ����17~47Khz֮�䰴32Khz��
//Timeout=((4*2^prer)*rlr)/32 (ms).

//��ʼ���������Ź�
void IWDG_Init(void)
{
//1.�����������Ź��ļĴ���
	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);
	//2.����32��Ƶ���������Ź���Ӳ��ʱ��Ƶ��=32KHz/32=1000Hz
	IWDG_SetPrescaler(IWDG_Prescaler_32);
	//3.���ö������Ź��ļ���ֵ,1.5���ӵ�ʱ�䵽��
	IWDG_SetReload(2000-1);
	//4.���������Ź�����д����
	IWDG_ReloadCounter();
        //5.ʹ�ܶ������Ź�
	IWDG_Enable();
}
	
	
	
//ι������
void IWDG_Feed(void)
{
 IWDG_ReloadCounter();
}

