#include "lsens.h"
#include "delay.h"
#include "AD.H"
#include "sw_global.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32F407������
//LSENS(����������)��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2014/5/7
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2014-2024
//All rights reserved										  
//////////////////////////////////////////////////////////////////////////////////
  
//��ʼ������������
void Lsens_Init(void)
{
  GPIO_InitTypeDef  GPIO_InitStructure;
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);//ʹ��GPIOFʱ��
	
  //�ȳ�ʼ��ADC3ͨ��7IO��
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;//PA7 ͨ��7
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;//ģ������
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;//����������
  GPIO_Init(GPIOF, &GPIO_InitStructure);//��ʼ��  
 
	Adc3_Init();//��ʼ��ADC3
}

//��ȡLight Sens��ֵ
//0~100:0,�;100,���� 
u8 Lsens_Get_Val(void)
{
	u32 temp_val=0;
	u8 t;
	for(t=0;t<LSENS_READ_TIMES;t++)
	{
//    printf("11111\n");
		temp_val+=Get_Adc3(ADC_Channel_5);	//��ȡADCֵ,ͨ��5
		delay_ms(5);
	}
//  printf("temp_val=%d\n",temp_val);
	temp_val/=LSENS_READ_TIMES;//�õ�ƽ��ֵ 
  
	if(temp_val>4000)temp_val=4000;
	return (u8)(100-(temp_val/40));
}






//DMAx�ĸ�ͨ������
//����Ĵ�����ʽ�ǹ̶���,���Ҫ���ݲ�ͬ��������޸�
//�Ӵ洢��->����ģʽ/8λ���ݿ��/�洢������ģʽ
//DMA_Streamx:DMA������,DMA1_Stream0~7/DMA2_Stream0~7
//chx:DMAͨ��ѡ��,@ref DMA_channel DMA_Channel_0~DMA_Channel_7
//par:�����ַ
//mar:�洢����ַ
//ndtr:���ݴ�����  
void MYDMA_Config(DMA_Stream_TypeDef *DMA_Streamx,u32 chx,u32 par,u32 mar,u16 ndtr)
{

	DMA_InitTypeDef  DMA_InitStructure;
	/* ʹ��DMA2ʱ�� */
	if((u32)DMA_Streamx>(u32)DMA2)										//�õ���ǰstream������DMA2����DMA1
	{
	  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2,ENABLE);				//DMA2ʱ��ʹ�� 
		
	}else 
	{
	  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1,ENABLE);				//DMA1ʱ��ʹ�� 
	}
	
	/* Reset DMA Stream registers (for debug purpose) */
	DMA_DeInit(DMA_Streamx);
	
	while (DMA_GetCmdStatus(DMA_Streamx) != DISABLE){};	  				//�ȴ�DMA������
	/* Configure DMA Stream */
	/* ���� DMA Stream */
	DMA_InitStructure.DMA_Channel = chx;  										//ͨ��ѡ��
	DMA_InitStructure.DMA_PeripheralBaseAddr = par;								//DMA�����ַ
	DMA_InitStructure.DMA_Memory0BaseAddr = mar;								//DMA �洢��0��ַ
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;						//�洢��������ģʽ
	DMA_InitStructure.DMA_BufferSize = ndtr;									//���ݴ����� 
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;			//���������ģʽ
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;						//�洢������ģʽ
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;		//�������ݳ���:8λ
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;				//�洢�����ݳ���:8λ
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;								//ʹ��ѭ��ģʽ 
	DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;						//�е����ȼ�
	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;         
	DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;
	DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;					//�洢��ͻ�����δ���
	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;			//����ͻ�����δ���
	DMA_Init(DMA_Streamx, &DMA_InitStructure);									//��ʼ��DMA Stream
	
	USART_DMACmd(USART1,USART_DMAReq_Rx,ENABLE); //ʹ�ܴ���1��DMA����
  MYDMA_Enable(DMA_Streamx,SEND_BUF_SIZE);     //��ʼ��1��DMA���䣡
} 

//����һ��DMA����
//DMA_Streamx:DMA������,DMA1_Stream0~7/DMA2_Stream0~7 
//ndtr:���ݴ�����  
void MYDMA_Enable(DMA_Stream_TypeDef *DMA_Streamx,u16 ndtr)
{
	DMA_Cmd(DMA_Streamx, DISABLE);                      //�ر�DMA���� 
	
	while (DMA_GetCmdStatus(DMA_Streamx) != DISABLE){}	//ȷ��DMA���Ա�����  
		
	DMA_SetCurrDataCounter(DMA_Streamx,ndtr);          //���ݴ�����  
 
	DMA_Cmd(DMA_Streamx, ENABLE);                      //����DMA���� 
}
