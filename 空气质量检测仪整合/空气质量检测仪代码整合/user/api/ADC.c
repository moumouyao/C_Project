#include "ADC.h"

//存放中值滤波的采样值
uint16_t light_val[MED_FIL_VAL] = {0};
uint16_t smoke_val[MED_FIL_VAL] = {0};
uint16_t temp_val[MED_FIL_VAL] = {0};
float ADC1_value[3] = {0};
float ADC2V[3] = {0};

uint16_t ADC1_val[30] = {0};	//ADC1扫描模式+DMA转移下的值
/**
  * @brief  配置PA5接收数模转换后的数据
  * @param  None
  * @retval None
  */
void AD_light_init(void){
	//配置ADC1时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
	//设置时钟为6分频 12MHZ
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);
	//配置PA5
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
  	GPIO_InitTypeDef GPIO_InitStructure;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  	GPIO_Init(GPIOA, &GPIO_InitStructure);
		
		ADC_TempSensorVrefintCmd(ENABLE);//传感器这玩意必须打开，否则必定没数据
		
		
	
	//结构体初始化ADC1
	ADC_InitTypeDef ADC_InitStruct;
	ADC_InitStruct.ADC_Mode = ADC_Mode_Independent;		//独立工作模式
	ADC_InitStruct.ADC_DataAlign = ADC_DataAlign_Right;	//数据右对齐
	ADC_InitStruct.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;	//不需要外部中断
	ADC_InitStruct.ADC_ContinuousConvMode = DISABLE;	//关闭连续转换模式
	ADC_InitStruct.ADC_ScanConvMode = DISABLE;	//禁止扫描模式
	ADC_InitStruct.ADC_NbrOfChannel = 1;//采样通道时为1
	ADC_Init(ADC1, &ADC_InitStruct);
	
	//开启ADC电源
	ADC_Cmd(ADC1, ENABLE);
	
	//对ADC进行校准
	ADC_ResetCalibration(ADC1);	//复位校准	1正在重置初始化  0重置初始化完成
	while(ADC_GetResetCalibrationStatus(ADC1));	//等待复位校准置0
	ADC_StartCalibration(ADC1);		//启动校准	1正在校准  0校准完成
	while(ADC_GetCalibrationStatus(ADC1));	//等待校准完成
}

/**
  * @brief  配置PA5接收数模转换后的数据
  * @param  None
  * @retval None
  */
void AD_smoke_init(void){
	//配置ADC2时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC2, ENABLE);
	//设置时钟为6分频 12MHZ
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);
	//配置PC1
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
  	GPIO_InitTypeDef GPIO_InitStructure;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
		
	
	//结构体初始化ADC1
	ADC_InitTypeDef ADC_InitStruct;
	ADC_InitStruct.ADC_Mode = ADC_Mode_Independent;		//独立工作模式
	ADC_InitStruct.ADC_DataAlign = ADC_DataAlign_Right;	//数据右对齐
	ADC_InitStruct.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;	//不需要外部中断
	ADC_InitStruct.ADC_ContinuousConvMode = DISABLE;	//关闭连续转换模式
	ADC_InitStruct.ADC_ScanConvMode = DISABLE;	//禁止扫描模式
	ADC_InitStruct.ADC_NbrOfChannel = 1;//采样通道时为1
	ADC_Init(ADC2, &ADC_InitStruct);
	
	//开启ADC电源
	ADC_Cmd(ADC2, ENABLE);
	
	//对ADC进行校准
	ADC_ResetCalibration(ADC2);	//复位校准	1正在重置初始化  0重置初始化完成
	while(ADC_GetResetCalibrationStatus(ADC2));	//等待复位校准置0
	ADC_StartCalibration(ADC2);		//启动校准	1正在校准  0校准完成
	while(ADC_GetCalibrationStatus(ADC2));	//等待校准完成
}


/**
  * @brief  获取模数转换后的值
  * @param  None
  * @retval 最近一次ADC转换结果
  */
uint16_t ADC1_get_val(uint8_t ADC_Channel){
	//配置ADC1 通道0 序列1 55.5个ADC时钟周期
	ADC_RegularChannelConfig(ADC1, ADC_Channel, 1, ADC_SampleTime_55Cycles5);
	ADC_SoftwareStartConvCmd(ADC1,ENABLE);
	while(ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET);	//返回值为1，转换完成，读取后自动清除
	return ADC_GetConversionValue(ADC1);
}

/**
  * @brief  获取ADC2烟雾传感器模数转换后的值
  * @param  None
  * @retval 最近一次ADC转换结果
  */
uint16_t ADC2_get_val(uint8_t ADC_Channel){
	//配置ADC1 通道0 序列1 55.5个ADC时钟周期
	ADC_RegularChannelConfig(ADC2, ADC_Channel, 1, ADC_SampleTime_55Cycles5);
	ADC_SoftwareStartConvCmd(ADC2,ENABLE);
	while(ADC_GetFlagStatus(ADC2, ADC_FLAG_EOC) == RESET);	//返回值为1，转换完成，读取后自动清除
	return ADC_GetConversionValue(ADC2);
}


/**
  * @brief  对ADC1光照返回值进行中值滤波
  * @param  None
  * @retval 滤波后的参数
  */
uint16_t AD_light_Median_filter(void){
	uint16_t temp = 0;
	//采样
	for(int i = 0; i < MED_FIL_VAL; i++){
		light_val[i] = ADC1_get_val(ADC_Channel_5);
	}
	//冒泡排序取中值
	for(int i = 0; i < MED_FIL_VAL; i++){
		for(int j = 0; j < MED_FIL_VAL - i; j++){
			if(light_val[j] > light_val[j + 1]){
				temp = light_val[j];
				light_val[j] = light_val[j + 1];
				light_val[j + 1] = temp;
			}
		}
	}
	//返回数据中值
	return light_val[(MED_FIL_VAL - 1) / 2];  
}

/**
  * @brief  对ADC2烟雾传感器返回值进行中值滤波
  * @param  None
  * @retval 滤波后的参数
  */
uint16_t AD_smoke_Median_filter(void){
	uint16_t temp = 0;
	//采样
	for(int i = 0; i < MED_FIL_VAL; i++){
		smoke_val[i] = ADC2_get_val(ADC_Channel_11);
	}
	//冒泡排序取中值
	for(int i = 0; i < MED_FIL_VAL; i++){
		for(int j = 0; j < MED_FIL_VAL - i; j++){
			if(smoke_val[j] > smoke_val[j + 1]){
				temp = smoke_val[j];
				smoke_val[j] = smoke_val[j + 1];
				smoke_val[j + 1] = temp;
			}
		}
	}
	//返回数据中值
	return smoke_val[(MED_FIL_VAL - 1) / 2];  
}


/**
  * @brief  对ADC1内部温度返回值进行中值滤波，并计算温度
  * @param  None
  * @retval 滤波后的参数
  */
float AD_temp_Median_filter(void){
	uint16_t temp = 0;
	//采样
	for(int i = 0; i < MED_FIL_VAL; i++){
		temp_val[i] = ADC1_get_val(ADC_Channel_16);
	}
	//冒泡排序取中值
	for(int i = 0; i < MED_FIL_VAL; i++){
		for(int j = 0; j < MED_FIL_VAL - i; j++){
			if(temp_val[j] > temp_val[j + 1]){
				temp = temp_val[j];
				temp_val[j] = temp_val[j + 1];
				temp_val[j + 1] = temp;
			}
		}
	}
	 float VSense = (float)(temp_val[(MED_FIL_VAL - 1) / 2])*(3.3/4096.0);
	//返回数据中值
	return((1.43 - VSense)/0.0043+25.0);  
}

/**
  * @brief  配置ADC1为多通道+DMA转运
  * @param  None
  * @retval None
  */
void ADC1_DMA_init(void){
	//配置ADC1时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
	//设置时钟为6分频 12MHZ
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);
	//配置PA5 PC1
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOC, ENABLE);
  	GPIO_InitTypeDef GPIO_InitStructure;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  	GPIO_Init(GPIOA, &GPIO_InitStructure);
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
		GPIO_Init(GPIOC, &GPIO_InitStructure);
	
		
	//开启DMA1的时钟
		RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	
		DMA_InitTypeDef DMA_InitStruct;
		DMA_InitStruct.DMA_BufferSize =30;	//传输次数 
		DMA_InitStruct.DMA_DIR = DMA_DIR_PeripheralSRC;	//数据传输方向
		DMA_InitStruct.DMA_M2M = DMA_M2M_Disable;	//使用硬件触发
		DMA_InitStruct.DMA_MemoryBaseAddr = (uint32_t)ADC1_val;	//转运的目标地址
		DMA_InitStruct.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;	//数据长度
		DMA_InitStruct.DMA_MemoryInc = DMA_MemoryInc_Enable;	//是否自增
		DMA_InitStruct.DMA_Mode = DMA_Mode_Circular;	//循环
		DMA_InitStruct.DMA_PeripheralBaseAddr = (uint32_t)&ADC1->DR;	//需要转运数据的起始地址
		DMA_InitStruct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;	//一次传输16位
		DMA_InitStruct.DMA_PeripheralInc = DMA_PeripheralInc_Disable;	//不自增模式
		DMA_InitStruct.DMA_Priority = DMA_Priority_Medium;	//触发优先级
		DMA_Init(DMA1_Channel1, &DMA_InitStruct);
		
		ADC_TempSensorVrefintCmd(ENABLE);//传感器这玩意必须打开，否则必定没数据
		
	//规则组配置
	ADC_RegularChannelConfig(ADC1, ADC_Channel_11, 1, ADC_SampleTime_239Cycles5);	//烟雾传感器
	ADC_RegularChannelConfig(ADC1, ADC_Channel_5, 2, ADC_SampleTime_239Cycles5);	//光敏电阻
	ADC_RegularChannelConfig(ADC1, ADC_Channel_16, 3, ADC_SampleTime_239Cycles5);	//CPU温度	
	
	//结构体初始化ADC1
	ADC_InitTypeDef ADC_InitStruct;
	ADC_InitStruct.ADC_Mode = ADC_Mode_Independent;		//独立工作模式
	ADC_InitStruct.ADC_DataAlign = ADC_DataAlign_Right;	//数据右对齐
	ADC_InitStruct.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;	//不需要外部中断
	ADC_InitStruct.ADC_ContinuousConvMode = ENABLE;	//关闭连续转换模式
	ADC_InitStruct.ADC_ScanConvMode = ENABLE;	//开启扫描模式
	ADC_InitStruct.ADC_NbrOfChannel = 3;//采样通道时为4
	ADC_Init(ADC1, &ADC_InitStruct);
	
	//使能DMA
	DMA_Cmd(DMA1_Channel1, ENABLE);
	
	//开启ADC1到DMA的通道
	ADC_DMACmd(ADC1, ENABLE);
	
	//开启ADC电源
	ADC_Cmd(ADC1, ENABLE);
	
	
	
	//对ADC进行校准
	ADC_ResetCalibration(ADC1);	//复位校准	1正在重置初始化  0重置初始化完成
	while(ADC_GetResetCalibrationStatus(ADC1));	//等待复位校准置0
	ADC_StartCalibration(ADC1);		//启动校准	1正在校准  0校准完成
	while(ADC_GetCalibrationStatus(ADC1));	//等待校准完成
	
	ADC_SoftwareStartConvCmd(ADC1,ENABLE);	//软件触发
}

/**
  * @brief  获取ADC1均值滤波后的值
	*		ADC1_value[0]		烟雾传感器
	*		ADC1_value[1]		光敏电阻
	*		ADC1_value[2]		CPU
  * @param  None
  * @retval None
  */
void ADC1_DMA_get_val(void){
//		DMA_Cmd(DMA1_Channel1, DISABLE);	//关闭
//		DMA_SetCurrDataCounter(DMA1_Channel1, 3);	//修改传输计数器
//		DMA_Cmd(DMA1_Channel1, ENABLE);	//开启
	
//	ADC_SoftwareStartConvCmd(ADC1,ENABLE);
	
//	while(DMA_GetFlagStatus(DMA1_FLAG_TC1) == RESET);//等待传输完成
//	DMA_ClearFlag(DMA1_FLAG_TC1);	//清理完成标志位
	uint8_t num = 1;
	uint16_t temp[3] = {0};
	for(uint8_t i = 0; i < 30; i++){	//各项分别累加
		switch(num){
			case 1:
				num++;
				temp[0] = temp[0] + ADC1_val[i];
				break;
			case 2:
				num++;
				temp[1] = temp[1] + ADC1_val[i];
				//printf("%d\r\n", ADC1_val[i]);
				break;
			case 3:
				num = 1;
				temp[2] = temp[2] + ADC1_val[i];
				break;
		}
		
	}
	
	ADC1_value[0] = (double)temp[0] / 10;
	ADC1_value[1] = (double)temp[1] / 10;
	ADC1_value[2] = (double)temp[2] / 10;
}


/**
  * @brief  ADC1 2 DMA后的数据分析
	*			ADC2V[0]	烟雾传感器电压
	*			ADC2V[1]	光敏电阻电压
	*			ADC2V[2]	CPU温度
  * @param  None
  * @retval None
  */
void ADC2VOL(void){
	ADC1_DMA_get_val();
	ADC2V[0] = ((ADC1_value[0] / 4095) * 3.3);
	ADC2V[1] = ((ADC1_value[1] / 4095) * 3.3);
	ADC2V[2] = ((ADC1_value[2] / 4095) * 3.3);
	ADC2V[2] = ((1.43 - ADC2V[2])/0.0043+25.0);
	if(ADC1_value[0] > 50){
			//buzzer_enable();
			led_enable(LED1);
		}
		else{
			//buzzer_disable();
			led_disable(LED1);
		}
}






