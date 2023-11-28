#include "adc3.h"

void	ADC_config(void)
{
	ADC_InitTypeDef		ADC_InitStructure;		//�ṹ����
	ADC_InitStructure.ADC_SMPduty   = 31;		//ADC ģ���źŲ���ʱ�����, 0~31��ע�⣺ SMPDUTY һ����������С�� 10��
	ADC_InitStructure.ADC_CsSetup   = 0;		//ADC ͨ��ѡ��ʱ����� 0(Ĭ��),1
	ADC_InitStructure.ADC_CsHold    = 1;		//ADC ͨ��ѡ�񱣳�ʱ����� 0,1(Ĭ��),2,3
	ADC_InitStructure.ADC_Speed     = ADC_SPEED_2X1T;		//���� ADC ����ʱ��Ƶ��	ADC_SPEED_2X1T~ADC_SPEED_2X16T
	ADC_InitStructure.ADC_Power     = ENABLE;				//ADC��������/�ر�	ENABLE,DISABLE
	ADC_InitStructure.ADC_AdjResult = ADC_RIGHT_JUSTIFIED;	//ADC�������,	ADC_LEFT_JUSTIFIED,ADC_RIGHT_JUSTIFIED
	ADC_InitStructure.ADC_Priority    = Priority_0;			//ָ���ж����ȼ�(�͵���) Priority_0,Priority_1,Priority_2,Priority_3
	ADC_InitStructure.ADC_Interrupt = DISABLE;			//�ж�����	ENABLE,DISABLE
	ADC_Inilize(&ADC_InitStructure);					//��ʼ��
	ADC_PowerControl(ENABLE);							//������ADC��Դ��������, ENABLE��DISABLE
}



