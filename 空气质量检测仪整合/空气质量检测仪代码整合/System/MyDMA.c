#include "MyDMA.h"

uint16_t MyDMA_size = 0;

/**
  * @brief  ��ʼ��DMA1,ת������
  * @param  None
  * @retval None
  */
	void MyDMA_init(uint32_t AddrA, uint32_t AddrB, uint16_t size){
		MyDMA_size = size;
		
		//����DMA1��ʱ��
		RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
		
		DMA_InitTypeDef DMA_InitStruct;
		DMA_InitStruct.DMA_BufferSize =size;	//������� 
		DMA_InitStruct.DMA_DIR = DMA_DIR_PeripheralSRC;	//���ݴ��䷽��
		DMA_InitStruct.DMA_M2M = DMA_M2M_Enable;	//�洢�����洢��	
		DMA_InitStruct.DMA_MemoryBaseAddr = AddrB;	//ת�˵�Ŀ���ַ
		DMA_InitStruct.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;	//���ݳ���
		DMA_InitStruct.DMA_MemoryInc = DMA_MemoryInc_Enable;	//�Ƿ�����
		DMA_InitStruct.DMA_Mode = DMA_Mode_Normal;	//��ѭ��
		DMA_InitStruct.DMA_PeripheralBaseAddr = AddrA;	//��Ҫת�����ݵ���ʼ��ַ
		DMA_InitStruct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;	//һ�δ���8λ
		DMA_InitStruct.DMA_PeripheralInc = DMA_PeripheralInc_Enable;	//����ģʽ
		DMA_InitStruct.DMA_Priority = DMA_Priority_Medium;	//�������ȼ�
		DMA_Init(DMA1_Channel1, &DMA_InitStruct);
		
		//ʹ��DMA
		DMA_Cmd(DMA1_Channel1, ENABLE);
	}
	
	/**
    * @brief  ���¸������������ֵ
    * @param  None
    * @retval None
    */
	void MyDMA_Transfer(void){
		DMA_Cmd(DMA1_Channel1, DISABLE);	//�ر�
		DMA_SetCurrDataCounter(DMA1_Channel1, MyDMA_size);	//�޸Ĵ��������
		DMA_Cmd(DMA1_Channel1, ENABLE);	//����
		
		while(DMA_GetFlagStatus(DMA1_FLAG_TC1) == RESET);//�ȴ��������
		DMA_ClearFlag(DMA1_FLAG_TC1);	//������ɱ�־λ
	}
	
