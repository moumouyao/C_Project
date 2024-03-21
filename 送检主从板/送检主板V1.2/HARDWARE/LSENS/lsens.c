#include "lsens.h"
#include "delay.h"
#include "AD.H"
#include "sw_global.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK STM32F407开发板
//LSENS(光敏传感器)驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//创建日期:2014/5/7
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2014-2024
//All rights reserved										  
//////////////////////////////////////////////////////////////////////////////////
  
//初始化光敏传感器
void Lsens_Init(void)
{
  GPIO_InitTypeDef  GPIO_InitStructure;
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);//使能GPIOF时钟
	
  //先初始化ADC3通道7IO口
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;//PA7 通道7
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;//模拟输入
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;//不带上下拉
  GPIO_Init(GPIOF, &GPIO_InitStructure);//初始化  
 
	Adc3_Init();//初始化ADC3
}

//读取Light Sens的值
//0~100:0,最暗;100,最亮 
u8 Lsens_Get_Val(void)
{
	u32 temp_val=0;
	u8 t;
	for(t=0;t<LSENS_READ_TIMES;t++)
	{
//    printf("11111\n");
		temp_val+=Get_Adc3(ADC_Channel_5);	//读取ADC值,通道5
		delay_ms(5);
	}
//  printf("temp_val=%d\n",temp_val);
	temp_val/=LSENS_READ_TIMES;//得到平均值 
  
	if(temp_val>4000)temp_val=4000;
	return (u8)(100-(temp_val/40));
}






//DMAx的各通道配置
//这里的传输形式是固定的,这点要根据不同的情况来修改
//从存储器->外设模式/8位数据宽度/存储器增量模式
//DMA_Streamx:DMA数据流,DMA1_Stream0~7/DMA2_Stream0~7
//chx:DMA通道选择,@ref DMA_channel DMA_Channel_0~DMA_Channel_7
//par:外设地址
//mar:存储器地址
//ndtr:数据传输量  
void MYDMA_Config(DMA_Stream_TypeDef *DMA_Streamx,u32 chx,u32 par,u32 mar,u16 ndtr)
{

	DMA_InitTypeDef  DMA_InitStructure;
	/* 使能DMA2时钟 */
	if((u32)DMA_Streamx>(u32)DMA2)										//得到当前stream是属于DMA2还是DMA1
	{
	  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2,ENABLE);				//DMA2时钟使能 
		
	}else 
	{
	  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1,ENABLE);				//DMA1时钟使能 
	}
	
	/* Reset DMA Stream registers (for debug purpose) */
	DMA_DeInit(DMA_Streamx);
	
	while (DMA_GetCmdStatus(DMA_Streamx) != DISABLE){};	  				//等待DMA可配置
	/* Configure DMA Stream */
	/* 配置 DMA Stream */
	DMA_InitStructure.DMA_Channel = chx;  										//通道选择
	DMA_InitStructure.DMA_PeripheralBaseAddr = par;								//DMA外设地址
	DMA_InitStructure.DMA_Memory0BaseAddr = mar;								//DMA 存储器0地址
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;						//存储器到外设模式
	DMA_InitStructure.DMA_BufferSize = ndtr;									//数据传输量 
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;			//外设非增量模式
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;						//存储器增量模式
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;		//外设数据长度:8位
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;				//存储器数据长度:8位
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;								//使用循环模式 
	DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;						//中等优先级
	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;         
	DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;
	DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;					//存储器突发单次传输
	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;			//外设突发单次传输
	DMA_Init(DMA_Streamx, &DMA_InitStructure);									//初始化DMA Stream
	
	USART_DMACmd(USART1,USART_DMAReq_Rx,ENABLE); //使能串口1的DMA接收
  MYDMA_Enable(DMA_Streamx,SEND_BUF_SIZE);     //开始第1次DMA传输！
} 

//开启一次DMA传输
//DMA_Streamx:DMA数据流,DMA1_Stream0~7/DMA2_Stream0~7 
//ndtr:数据传输量  
void MYDMA_Enable(DMA_Stream_TypeDef *DMA_Streamx,u16 ndtr)
{
	DMA_Cmd(DMA_Streamx, DISABLE);                      //关闭DMA传输 
	
	while (DMA_GetCmdStatus(DMA_Streamx) != DISABLE){}	//确保DMA可以被设置  
		
	DMA_SetCurrDataCounter(DMA_Streamx,ndtr);          //数据传输量  
 
	DMA_Cmd(DMA_Streamx, ENABLE);                      //开启DMA传输 
}
