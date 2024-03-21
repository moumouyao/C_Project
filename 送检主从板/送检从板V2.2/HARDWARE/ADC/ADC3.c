#include "adc3.h"

/**
  ******************************************************************************
  *                            Adc3_Init()
  *
  * 描述   : ADC3通道初始化
  *		   
  * 参数   : 无
  *          
  * 返回值 : 无
  ******************************************************************************
  */
void Adc3_Init(void){
	GPIO_InitTypeDef  GPIO_InitStructure; 
	ADC_CommonInitTypeDef ADC_CommonInitStruct;
	ADC_InitTypeDef ADC_InitStruct;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC3, ENABLE);
	
	/* PF4 引脚配置 */
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_4;	//ADC采样引脚
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN; //设置引脚位模拟输入
  GPIO_Init(GPIOF, &GPIO_InitStructure);          //ADC接口使用的GPIO管脚初始化
	
	ADC_DeInit();//ADC复位
	
	ADC_CommonInitStruct.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;//禁用DMA转运
	ADC_CommonInitStruct.ADC_Mode = ADC_Mode_Independent;//独立模式，每次只转换一个通道
	ADC_CommonInitStruct.ADC_Prescaler = ADC_Prescaler_Div4;//时钟分频
	ADC_CommonInitStruct.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;//采样间隔
	ADC_CommonInit(&ADC_CommonInitStruct);
	
	ADC_InitStruct.ADC_ContinuousConvMode = DISABLE;//禁用转换模式，每次只转换一次
	ADC_InitStruct.ADC_DataAlign = ADC_DataAlign_Right;//右对齐
//	ADC_InitStruct.ADC_ExternalTrigConv = ;
	ADC_InitStruct.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;//不使用外部触发
	ADC_InitStruct.ADC_NbrOfConversion = 1;//转换通道数
	ADC_InitStruct.ADC_Resolution = ADC_Resolution_12b;//分辨率为12位
	ADC_InitStruct.ADC_ScanConvMode = DISABLE;//禁用扫描模式
	ADC_Init(ADC3, &ADC_InitStruct);
	
	//启动ADC3
	ADC_Cmd(ADC3, ENABLE);
//	ADC_StartCalibration

}

/**
  ******************************************************************************
  *                            Get_ADC3_val()
  *
  * 描述   : 获取ADC3某个通道的采样值
  *		   
  * 参数   : ch	通道号
  *          
  * 返回值 : 该通道的采样值
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
  * 描述   : 获取ADC3某个通道的平均采样值
  *		   
  * 参数   : ch	通道号
	*					 time	采样次数
  *          
  * 返回值 : 该通道的平均采样值
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
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	

