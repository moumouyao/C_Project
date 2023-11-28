#include "ADC.h"

//�����ֵ�˲��Ĳ���ֵ
uint16_t light_val[MED_FIL_VAL] = {0};
uint16_t smoke_val[MED_FIL_VAL] = {0};
uint16_t temp_val[MED_FIL_VAL] = {0};
float ADC1_value[3] = {0};
float ADC2V[3] = {0};

uint16_t ADC1_val[30] = {0};	//ADC1ɨ��ģʽ+DMAת���µ�ֵ
/**
  * @brief  ����PA5������ģת���������
  * @param  None
  * @retval None
  */
void AD_light_init(void){
	//����ADC1ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
	//����ʱ��Ϊ6��Ƶ 12MHZ
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);
	//����PA5
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
  	GPIO_InitTypeDef GPIO_InitStructure;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  	GPIO_Init(GPIOA, &GPIO_InitStructure);
		
		ADC_TempSensorVrefintCmd(ENABLE);//���������������򿪣�����ض�û����
		
		
	
	//�ṹ���ʼ��ADC1
	ADC_InitTypeDef ADC_InitStruct;
	ADC_InitStruct.ADC_Mode = ADC_Mode_Independent;		//��������ģʽ
	ADC_InitStruct.ADC_DataAlign = ADC_DataAlign_Right;	//�����Ҷ���
	ADC_InitStruct.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;	//����Ҫ�ⲿ�ж�
	ADC_InitStruct.ADC_ContinuousConvMode = DISABLE;	//�ر�����ת��ģʽ
	ADC_InitStruct.ADC_ScanConvMode = DISABLE;	//��ֹɨ��ģʽ
	ADC_InitStruct.ADC_NbrOfChannel = 1;//����ͨ��ʱΪ1
	ADC_Init(ADC1, &ADC_InitStruct);
	
	//����ADC��Դ
	ADC_Cmd(ADC1, ENABLE);
	
	//��ADC����У׼
	ADC_ResetCalibration(ADC1);	//��λУ׼	1�������ó�ʼ��  0���ó�ʼ�����
	while(ADC_GetResetCalibrationStatus(ADC1));	//�ȴ���λУ׼��0
	ADC_StartCalibration(ADC1);		//����У׼	1����У׼  0У׼���
	while(ADC_GetCalibrationStatus(ADC1));	//�ȴ�У׼���
}

/**
  * @brief  ����PA5������ģת���������
  * @param  None
  * @retval None
  */
void AD_smoke_init(void){
	//����ADC2ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC2, ENABLE);
	//����ʱ��Ϊ6��Ƶ 12MHZ
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);
	//����PC1
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
  	GPIO_InitTypeDef GPIO_InitStructure;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
		
	
	//�ṹ���ʼ��ADC1
	ADC_InitTypeDef ADC_InitStruct;
	ADC_InitStruct.ADC_Mode = ADC_Mode_Independent;		//��������ģʽ
	ADC_InitStruct.ADC_DataAlign = ADC_DataAlign_Right;	//�����Ҷ���
	ADC_InitStruct.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;	//����Ҫ�ⲿ�ж�
	ADC_InitStruct.ADC_ContinuousConvMode = DISABLE;	//�ر�����ת��ģʽ
	ADC_InitStruct.ADC_ScanConvMode = DISABLE;	//��ֹɨ��ģʽ
	ADC_InitStruct.ADC_NbrOfChannel = 1;//����ͨ��ʱΪ1
	ADC_Init(ADC2, &ADC_InitStruct);
	
	//����ADC��Դ
	ADC_Cmd(ADC2, ENABLE);
	
	//��ADC����У׼
	ADC_ResetCalibration(ADC2);	//��λУ׼	1�������ó�ʼ��  0���ó�ʼ�����
	while(ADC_GetResetCalibrationStatus(ADC2));	//�ȴ���λУ׼��0
	ADC_StartCalibration(ADC2);		//����У׼	1����У׼  0У׼���
	while(ADC_GetCalibrationStatus(ADC2));	//�ȴ�У׼���
}


/**
  * @brief  ��ȡģ��ת�����ֵ
  * @param  None
  * @retval ���һ��ADCת�����
  */
uint16_t ADC1_get_val(uint8_t ADC_Channel){
	//����ADC1 ͨ��0 ����1 55.5��ADCʱ������
	ADC_RegularChannelConfig(ADC1, ADC_Channel, 1, ADC_SampleTime_55Cycles5);
	ADC_SoftwareStartConvCmd(ADC1,ENABLE);
	while(ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET);	//����ֵΪ1��ת����ɣ���ȡ���Զ����
	return ADC_GetConversionValue(ADC1);
}

/**
  * @brief  ��ȡADC2��������ģ��ת�����ֵ
  * @param  None
  * @retval ���һ��ADCת�����
  */
uint16_t ADC2_get_val(uint8_t ADC_Channel){
	//����ADC1 ͨ��0 ����1 55.5��ADCʱ������
	ADC_RegularChannelConfig(ADC2, ADC_Channel, 1, ADC_SampleTime_55Cycles5);
	ADC_SoftwareStartConvCmd(ADC2,ENABLE);
	while(ADC_GetFlagStatus(ADC2, ADC_FLAG_EOC) == RESET);	//����ֵΪ1��ת����ɣ���ȡ���Զ����
	return ADC_GetConversionValue(ADC2);
}


/**
  * @brief  ��ADC1���շ���ֵ������ֵ�˲�
  * @param  None
  * @retval �˲���Ĳ���
  */
uint16_t AD_light_Median_filter(void){
	uint16_t temp = 0;
	//����
	for(int i = 0; i < MED_FIL_VAL; i++){
		light_val[i] = ADC1_get_val(ADC_Channel_5);
	}
	//ð������ȡ��ֵ
	for(int i = 0; i < MED_FIL_VAL; i++){
		for(int j = 0; j < MED_FIL_VAL - i; j++){
			if(light_val[j] > light_val[j + 1]){
				temp = light_val[j];
				light_val[j] = light_val[j + 1];
				light_val[j + 1] = temp;
			}
		}
	}
	//����������ֵ
	return light_val[(MED_FIL_VAL - 1) / 2];  
}

/**
  * @brief  ��ADC2������������ֵ������ֵ�˲�
  * @param  None
  * @retval �˲���Ĳ���
  */
uint16_t AD_smoke_Median_filter(void){
	uint16_t temp = 0;
	//����
	for(int i = 0; i < MED_FIL_VAL; i++){
		smoke_val[i] = ADC2_get_val(ADC_Channel_11);
	}
	//ð������ȡ��ֵ
	for(int i = 0; i < MED_FIL_VAL; i++){
		for(int j = 0; j < MED_FIL_VAL - i; j++){
			if(smoke_val[j] > smoke_val[j + 1]){
				temp = smoke_val[j];
				smoke_val[j] = smoke_val[j + 1];
				smoke_val[j + 1] = temp;
			}
		}
	}
	//����������ֵ
	return smoke_val[(MED_FIL_VAL - 1) / 2];  
}


/**
  * @brief  ��ADC1�ڲ��¶ȷ���ֵ������ֵ�˲����������¶�
  * @param  None
  * @retval �˲���Ĳ���
  */
float AD_temp_Median_filter(void){
	uint16_t temp = 0;
	//����
	for(int i = 0; i < MED_FIL_VAL; i++){
		temp_val[i] = ADC1_get_val(ADC_Channel_16);
	}
	//ð������ȡ��ֵ
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
	//����������ֵ
	return((1.43 - VSense)/0.0043+25.0);  
}

/**
  * @brief  ����ADC1Ϊ��ͨ��+DMAת��
  * @param  None
  * @retval None
  */
void ADC1_DMA_init(void){
	//����ADC1ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
	//����ʱ��Ϊ6��Ƶ 12MHZ
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);
	//����PA5 PC1
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOC, ENABLE);
  	GPIO_InitTypeDef GPIO_InitStructure;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  	GPIO_Init(GPIOA, &GPIO_InitStructure);
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
		GPIO_Init(GPIOC, &GPIO_InitStructure);
	
		
	//����DMA1��ʱ��
		RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	
		DMA_InitTypeDef DMA_InitStruct;
		DMA_InitStruct.DMA_BufferSize =30;	//������� 
		DMA_InitStruct.DMA_DIR = DMA_DIR_PeripheralSRC;	//���ݴ��䷽��
		DMA_InitStruct.DMA_M2M = DMA_M2M_Disable;	//ʹ��Ӳ������
		DMA_InitStruct.DMA_MemoryBaseAddr = (uint32_t)ADC1_val;	//ת�˵�Ŀ���ַ
		DMA_InitStruct.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;	//���ݳ���
		DMA_InitStruct.DMA_MemoryInc = DMA_MemoryInc_Enable;	//�Ƿ�����
		DMA_InitStruct.DMA_Mode = DMA_Mode_Circular;	//ѭ��
		DMA_InitStruct.DMA_PeripheralBaseAddr = (uint32_t)&ADC1->DR;	//��Ҫת�����ݵ���ʼ��ַ
		DMA_InitStruct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;	//һ�δ���16λ
		DMA_InitStruct.DMA_PeripheralInc = DMA_PeripheralInc_Disable;	//������ģʽ
		DMA_InitStruct.DMA_Priority = DMA_Priority_Medium;	//�������ȼ�
		DMA_Init(DMA1_Channel1, &DMA_InitStruct);
		
		ADC_TempSensorVrefintCmd(ENABLE);//���������������򿪣�����ض�û����
		
	//����������
	ADC_RegularChannelConfig(ADC1, ADC_Channel_11, 1, ADC_SampleTime_239Cycles5);	//��������
	ADC_RegularChannelConfig(ADC1, ADC_Channel_5, 2, ADC_SampleTime_239Cycles5);	//��������
	ADC_RegularChannelConfig(ADC1, ADC_Channel_16, 3, ADC_SampleTime_239Cycles5);	//CPU�¶�	
	
	//�ṹ���ʼ��ADC1
	ADC_InitTypeDef ADC_InitStruct;
	ADC_InitStruct.ADC_Mode = ADC_Mode_Independent;		//��������ģʽ
	ADC_InitStruct.ADC_DataAlign = ADC_DataAlign_Right;	//�����Ҷ���
	ADC_InitStruct.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;	//����Ҫ�ⲿ�ж�
	ADC_InitStruct.ADC_ContinuousConvMode = ENABLE;	//�ر�����ת��ģʽ
	ADC_InitStruct.ADC_ScanConvMode = ENABLE;	//����ɨ��ģʽ
	ADC_InitStruct.ADC_NbrOfChannel = 3;//����ͨ��ʱΪ4
	ADC_Init(ADC1, &ADC_InitStruct);
	
	//ʹ��DMA
	DMA_Cmd(DMA1_Channel1, ENABLE);
	
	//����ADC1��DMA��ͨ��
	ADC_DMACmd(ADC1, ENABLE);
	
	//����ADC��Դ
	ADC_Cmd(ADC1, ENABLE);
	
	
	
	//��ADC����У׼
	ADC_ResetCalibration(ADC1);	//��λУ׼	1�������ó�ʼ��  0���ó�ʼ�����
	while(ADC_GetResetCalibrationStatus(ADC1));	//�ȴ���λУ׼��0
	ADC_StartCalibration(ADC1);		//����У׼	1����У׼  0У׼���
	while(ADC_GetCalibrationStatus(ADC1));	//�ȴ�У׼���
	
	ADC_SoftwareStartConvCmd(ADC1,ENABLE);	//�������
}

/**
  * @brief  ��ȡADC1��ֵ�˲����ֵ
	*		ADC1_value[0]		��������
	*		ADC1_value[1]		��������
	*		ADC1_value[2]		CPU
  * @param  None
  * @retval None
  */
void ADC1_DMA_get_val(void){
//		DMA_Cmd(DMA1_Channel1, DISABLE);	//�ر�
//		DMA_SetCurrDataCounter(DMA1_Channel1, 3);	//�޸Ĵ��������
//		DMA_Cmd(DMA1_Channel1, ENABLE);	//����
	
//	ADC_SoftwareStartConvCmd(ADC1,ENABLE);
	
//	while(DMA_GetFlagStatus(DMA1_FLAG_TC1) == RESET);//�ȴ��������
//	DMA_ClearFlag(DMA1_FLAG_TC1);	//������ɱ�־λ
	uint8_t num = 1;
	uint16_t temp[3] = {0};
	for(uint8_t i = 0; i < 30; i++){	//����ֱ��ۼ�
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
  * @brief  ADC1 2 DMA������ݷ���
	*			ADC2V[0]	����������ѹ
	*			ADC2V[1]	���������ѹ
	*			ADC2V[2]	CPU�¶�
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






