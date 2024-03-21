#include "adc3.h"

/**
  ******************************************************************************
  *                            Adc3_Init()
  *
  * ����   : ADC3ͨ����ʼ��
  *		   
  * ����   : ��
  *          
  * ����ֵ : ��
  ******************************************************************************
  */
void Adc3_Init(void){
	GPIO_InitTypeDef  GPIO_InitStructure; 
	ADC_CommonInitTypeDef ADC_CommonInitStruct;
	ADC_InitTypeDef ADC_InitStruct;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC3, ENABLE);
	
	/* PF4 �������� */
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_4;	//ADC��������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN; //��������λģ������
  GPIO_Init(GPIOF, &GPIO_InitStructure);          //ADC�ӿ�ʹ�õ�GPIO�ܽų�ʼ��
	
	ADC_DeInit();//ADC��λ
	
	ADC_CommonInitStruct.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;//����DMAת��
	ADC_CommonInitStruct.ADC_Mode = ADC_Mode_Independent;//����ģʽ��ÿ��ֻת��һ��ͨ��
	ADC_CommonInitStruct.ADC_Prescaler = ADC_Prescaler_Div4;//ʱ�ӷ�Ƶ
	ADC_CommonInitStruct.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;//�������
	ADC_CommonInit(&ADC_CommonInitStruct);
	
	ADC_InitStruct.ADC_ContinuousConvMode = DISABLE;//����ת��ģʽ��ÿ��ֻת��һ��
	ADC_InitStruct.ADC_DataAlign = ADC_DataAlign_Right;//�Ҷ���
//	ADC_InitStruct.ADC_ExternalTrigConv = ;
	ADC_InitStruct.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;//��ʹ���ⲿ����
	ADC_InitStruct.ADC_NbrOfConversion = 1;//ת��ͨ����
	ADC_InitStruct.ADC_Resolution = ADC_Resolution_12b;//�ֱ���Ϊ12λ
	ADC_InitStruct.ADC_ScanConvMode = DISABLE;//����ɨ��ģʽ
	ADC_Init(ADC3, &ADC_InitStruct);
	
	//����ADC3
	ADC_Cmd(ADC3, ENABLE);
//	ADC_StartCalibration

}

/**
  ******************************************************************************
  *                            Get_ADC3_val()
  *
  * ����   : ��ȡADC3ĳ��ͨ���Ĳ���ֵ
  *		   
  * ����   : ch	ͨ����
  *          
  * ����ֵ : ��ͨ���Ĳ���ֵ
  ******************************************************************************
  */
uint16_t Get_ADC3_val(uint8_t ch){
	ADC_RegularChannelConfig(ADC3, ch, 1, ADC_SampleTime_480Cycles);
	ADC_SoftwareStartConv(ADC3);
	while(!ADC_GetFlagStatus(ADC3, ADC_FLAG_EOC));
	return ADC_GetConversionValue(ADC3);
}


/**
  ******************************************************************************
  *                            Get_ADC3_Average_val()
  *
  * ����   : ��ȡADC3ĳ��ͨ����ƽ������ֵ
  *		   
  * ����   : ch	ͨ����
	*					 time	��������
  *          
  * ����ֵ : ��ͨ����ƽ������ֵ
  ******************************************************************************
  */
uint16_t Get_ADC3_Average_val(uint8_t ch, uint8_t time){
	uint32_t temp_val = 0;
	for(uint8_t i = 0; i < time; i++){
		temp_val += Get_ADC3_val(ch);
		delay_ms(5);
	}
	return temp_val / time;
}
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	

