#include "MyDMA.h"

uint16_t MyDMA_size = 0;

/**
  * @brief  初始化DMA1,转运数组
  * @param  None
  * @retval None
  */
	void MyDMA_init(uint32_t AddrA, uint32_t AddrB, uint16_t size){
		MyDMA_size = size;
		
		//开启DMA1的时钟
		RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
		
		DMA_InitTypeDef DMA_InitStruct;
		DMA_InitStruct.DMA_BufferSize =size;	//传输次数 
		DMA_InitStruct.DMA_DIR = DMA_DIR_PeripheralSRC;	//数据传输方向
		DMA_InitStruct.DMA_M2M = DMA_M2M_Enable;	//存储器到存储器	
		DMA_InitStruct.DMA_MemoryBaseAddr = AddrB;	//转运的目标地址
		DMA_InitStruct.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;	//数据长度
		DMA_InitStruct.DMA_MemoryInc = DMA_MemoryInc_Enable;	//是否自增
		DMA_InitStruct.DMA_Mode = DMA_Mode_Normal;	//不循环
		DMA_InitStruct.DMA_PeripheralBaseAddr = AddrA;	//需要转运数据的起始地址
		DMA_InitStruct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;	//一次传输8位
		DMA_InitStruct.DMA_PeripheralInc = DMA_PeripheralInc_Enable;	//自增模式
		DMA_InitStruct.DMA_Priority = DMA_Priority_Medium;	//触发优先级
		DMA_Init(DMA1_Channel1, &DMA_InitStruct);
		
		//使能DMA
		DMA_Cmd(DMA1_Channel1, ENABLE);
	}
	
	/**
    * @brief  重新给传输计数器赋值
    * @param  None
    * @retval None
    */
	void MyDMA_Transfer(void){
		DMA_Cmd(DMA1_Channel1, DISABLE);	//关闭
		DMA_SetCurrDataCounter(DMA1_Channel1, MyDMA_size);	//修改传输计数器
		DMA_Cmd(DMA1_Channel1, ENABLE);	//开启
		
		while(DMA_GetFlagStatus(DMA1_FLAG_TC1) == RESET);//等待传输完成
		DMA_ClearFlag(DMA1_FLAG_TC1);	//清理完成标志位
	}
	
