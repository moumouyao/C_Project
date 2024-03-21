#include "AD.h"
#include "delay.h"
__IO uint16_t ADC1ConvertedValue = 0;


void ADC3_CH5_DMA_Config(void)
{
  ADC_InitTypeDef       ADC_InitStructure;
  ADC_CommonInitTypeDef ADC_CommonInitStructure;
  DMA_InitTypeDef       DMA_InitStructure;
  GPIO_InitTypeDef      GPIO_InitStructure;

  /* Enable ADC3, DMA2 and PF7 clocks ****************************************/
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2 | RCC_AHB1Periph_GPIOF, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC3, ENABLE);

  /* DMA2 Stream0 channel2 configuration **************************************/
  DMA_InitStructure.DMA_Channel = DMA_Channel_2;  //通道选择
  DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&(ADC3->DR);   //DMA外设地址
  DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)&ADC1ConvertedValue;  //DMA 存储器0地址
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;  //外设到存储器模式
  DMA_InitStructure.DMA_BufferSize = 1;//数据传输量 
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;//外设非增量模式
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;//存储器增量模式
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;//外设数据长度:16位
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;//存储器数据长度:16位
  DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;            //工作在循环缓存模式 
  DMA_InitStructure.DMA_Priority = DMA_Priority_High;        //中等优先级
  DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;     /*禁止FIFO*/    
  DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;/*FIFO的值*/
  DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;//存储器突发单次传输
  DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;//外设突发单次传输
  DMA_Init(DMA2_Stream0, &DMA_InitStructure);
  DMA_Cmd(DMA2_Stream0, ENABLE);//开启DMA传输


  /* Configure PB.00 (ADC Channel13) as analog input -------------------------*/
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
  GPIO_Init(GPIOF, &GPIO_InitStructure);

  /* ADC Common Init **********************************************************/
  ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;//独立模式
  ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;//两个采样阶段之间的延迟5个时钟
  ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_1; //DMA失能
  ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div4;//预分频4分频。ADCCLK=PCLK2/4=84/4=21Mhz,ADC时钟最好不要超过36Mhz 
  ADC_CommonInit(&ADC_CommonInitStructure);//初始化

  /* ADC3 Init ****************************************************************/
  ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;//12位模式
  ADC_InitStructure.ADC_ScanConvMode = DISABLE;         //扫描模式	
  ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;    //连续转换
  ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;//禁止触发检测，使用软件触发
  ADC_InitStructure.ADC_ExternalTrigConv = 0;
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;//右对齐	
  ADC_InitStructure.ADC_NbrOfConversion = 1;//1个转换在规则序列中 也就是只转换规则序列1 
  ADC_Init(ADC3, &ADC_InitStructure);//ADC初始化

  /* ADC1 regular channel13 configuration *************************************/
  ADC_RegularChannelConfig(ADC3, ADC_Channel_5, 1, ADC_SampleTime_480Cycles);

 /* Enable DMA request after last transfer (Single-ADC mode) */
  ADC_DMARequestAfterLastTransferCmd(ADC3, ENABLE); //源数据变化时开启DMA传输

  /* Enable ADC1 DMA */
  ADC_DMACmd(ADC3, ENABLE);

  /* Enable ADC1 */
  ADC_Cmd(ADC3, ENABLE);
}


void User_ADConfig (void)
{
	ADC3_CH5_DMA_Config(); 
	/* Start ADC3 Software Conversion */  
	ADC_SoftwareStartConv(ADC3);		//使能指定的ADC3的软件转换启动功能	  
	
	//OSTimeDly(2);
	delay_ms(1000);
	if((ADC1ConvertedValue>3950 && ADC1ConvertedValue<4050))
	{
//		printf("未检测到光敏\r\n");
	}
		
//		SYS_LOG("未检测到光敏",12);
	else if(ADC1ConvertedValue < 20)
	{
//		printf("光敏短路\r\n");
	}
		
//		SYS_LOG("光敏短路",8);
}


		 











