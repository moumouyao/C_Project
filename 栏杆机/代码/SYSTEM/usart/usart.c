#include "sys.h"
#include "usart.h"	
#include "stm32f4xx_dma.h"
#include "sw_conf.h"
#include "memp.h"
#include "sw_global.h"
#include "led.h"
////////////////////////////////////////////////////////////////////////////////// 	 
//如果使用ucos,则包括下面的头文件即可.
#if SYSTEM_SUPPORT_UCOS
#include "includes.h"					//ucos 使用	  
#endif
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK STM32F4探索者开发板
//串口1初始化		   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2014/6/10
//版本：V1.5
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved
//********************************************************************************
//V1.3修改说明 
//支持适应不同频率下的串口波特率设置.
//加入了对printf的支持
//增加了串口接收命令功能.
//修正了printf第一个字符丢失的bug
//V1.4修改说明
//1,修改串口初始化IO的bug
//2,修改了USART_RX_STA,使得串口最大接收字节数为2的14次方
//3,增加了USART_REC_LEN,用于定义串口最大允许接收的字节数(不大于2的14次方)
//4,修改了EN_USART1_RX的使能方式
//V1.5修改说明
//1,增加了对UCOSII的支持
////////////////////////////////////////////////////////////////////////////////// 	  
 

//////////////////////////////////////////////////////////////////
//加入以下代码,支持printf函数,而不需要选择use MicroLIB	  
#if 1
#pragma import(__use_no_semihosting)             
//标准库需要的支持函数                 
struct __FILE 
{ 
	int handle; 
}; 

FILE __stdout;       
//定义_sys_exit()以避免使用半主机模式    
void _sys_exit(int x) 
{ 
	x = x; 
} 
//重定义fputc函数 
int fputc(int ch, FILE *f)
{ 	
	while((USART1->SR&0X40)==0);//循环发送,直到发送完毕   
	USART1->DR = (u8) ch;      
	return ch;
}
#endif
 


//初始化IO 串口1 
//bound:波特率
void uart_init(u32 bound){
   //GPIO端口设置
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE); //使能GPIOA时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);//使能USART1时钟
 
	//串口1对应引脚复用映射
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource9,GPIO_AF_USART1); //GPIOA9复用为USART1
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource10,GPIO_AF_USART1); //GPIOA10复用为USART1
	
	//USART1端口配置
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10; //GPIOA9与GPIOA10
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//速度50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //上拉
	GPIO_Init(GPIOA,&GPIO_InitStructure); //初始化PA9，PA10

   //USART1 初始化设置
	USART_InitStructure.USART_BaudRate = bound;//波特率设置
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式
	USART_Init(USART1, &USART_InitStructure); //初始化串口1
	
	USART_Cmd(USART1, ENABLE);  //使能串口1 
	
	USART_ClearFlag(USART1, USART_FLAG_TC);
}	

void uart_init_2(u32 bound){
   //GPIO端口设置
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD,ENABLE); //使能GPIOA时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);//使能USART1时钟
	//串口2对应引脚复用映射
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource5,GPIO_AF_USART2); //GPIOA3复用为USART2_RX
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource6,GPIO_AF_USART2); //GPIOD5复用为USART2_TX
	
	//USART1端口配置


	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5|GPIO_Pin_6; //GPIOD5
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//速度50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //上拉
	GPIO_Init(GPIOD,&GPIO_InitStructure); //初始化PA9，PA10
   //USART1 初始化设置
	USART_InitStructure.USART_BaudRate = bound;//波特率设置
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式
	USART_Init(USART2, &USART_InitStructure); //初始化串口2
	
	USART_Cmd(USART2, ENABLE);  //使能串口2
	
	USART_ClearFlag(USART2, USART_FLAG_TC);
	
	USART_ITConfig(USART2, USART_IT_IDLE, ENABLE);//开启相关中断

	//Usart1 NVIC 配置
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;//串口1中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =3;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
}

void uart_init_3(u32 bound){
   //GPIO端口设置
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE); //使能GPIOD时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);//使能USART3时钟
 
	//串口1对应引脚复用映射
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource10,GPIO_AF_USART3); //GPIOD8复用为USART3
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource11,GPIO_AF_USART3); //GPIOD9复用为USART3
	
	//USART1端口配置
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11; //GPIOA8与GPIOA9
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//速度50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //上拉
	GPIO_Init(GPIOB,&GPIO_InitStructure); //初始化PA9，PA10

//   //USART1 初始化设置
	USART_InitStructure.USART_BaudRate = bound;//波特率设置
//	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
//	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
//	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
//	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
//	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式
//	USART_Init(USART3, &USART_InitStructure); //初始化串口3
	
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_Mode = USART_Mode_Tx;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;

  /* Configure USART1 */
  USART_Init(USART3, &USART_InitStructure);
//	USART_ITConfig(USART3s, USART_IT_TXE, ENABLE);//使用时打开
  USART_Cmd(USART3, ENABLE);	 
	
	
//	USART_ClearFlag(USART3, USART_FLAG_TC);
//	
//	USART_ITConfig(USART3, USART_IT_IDLE, ENABLE);//开启相关中断

	//Usart1 NVIC 配置
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;//优先级
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;  //抢占优先级
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
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

void printf_usart1(unsigned char * data,uint16_t len)
{
	unsigned char data1=0;
	for(int i=0;i<len;i++)
	{
	data1=data[i];
	USART_SendData(USART1,data1);//调用这个库函数byte变量写入到TDR
	//需要等待一下，等待TDR数据移到移位寄存器了我们才能进行下一步的输出
	while(USART_GetFlagStatus(USART1,USART_FLAG_TXE)==RESET);
	}
}

void printf_usart2(unsigned char * data,uint16_t len)
{
	unsigned char data1=0;
	for(int i=0;i<len;i++)
	{
	data1=data[i];
	USART_SendData(USART2,data1);//调用这个库函数byte变量写入到TDR
	//需要等待一下，等待TDR数据移到移位寄存器了我们才能进行下一步的输出
	while(USART_GetFlagStatus(USART2,USART_FLAG_TXE)==RESET);
	}
}

void printf_usart3(unsigned char * data,uint16_t len)
{
	unsigned char data1=0;
	for(int i=0;i<len;i++)
	{
	data1=data[i];
	USART_SendData(USART3,data1);//调用这个库函数byte变量写入到TDR
	//需要等待一下，等待TDR数据移到移位寄存器了我们才能进行下一步的输出
	while(USART_GetFlagStatus(USART3,USART_FLAG_TXE)==RESET);
	}
}

extern unsigned char *USART2_BUF;

void USART2_IRQHandler(void)                	//串口1中断服务程序
{
	u8 temp=0;
	StrFrameHeader *frame_head;//用于队列
	INT8U *memp_buf;//报文数据
	unsigned char err=0;
	unsigned int i;
	temp = err;
	err = temp;
	
	OSIntEnter();//告诉系统进入中断
	if(USART_GetFlagStatus(USART2, USART_FLAG_IDLE) != RESET)
	{
  temp = USART2->SR;  
  temp = USART2->DR;//清除IDLE中断
	temp = DMA_GetCurrDataCounter(DMA1_Stream5);//获取剩余的接收数据的量
	if(USART2_BUF[0] == '{')//判断帧头
	{
		memp_buf = memp_malloc(MEMP_PBUF_POOL);//申请一块内存pool
		frame_head = (StrFrameHeader *)memp_buf;
		frame_head->type = HEAD_USART2_TYPE;//代表串口数据
		frame_head->pdata = memp_buf + sizeof(StrFrameHeader);
		memp_buf =frame_head->pdata;         //定位接收数据指针

		//==========计算数据长度====================//
//		unsigned int iLong;//数据长度
//		for(iLong=2048;USART2_BUF[iLong]!='>'||USART2_BUF[iLong]!='}';iLong--);

			//==========写入队列==================//
			for(i=0;i<512;i++)//=======转译报文
			{
				*memp_buf++ = USART2_BUF[i];   
			}
			frame_head->len = memp_buf-frame_head->pdata;//命令长度
			
			err = OSQPost(En_Dis_CodeQeue,(void *)frame_head);//把拷贝的数据发送给加密解密任务
			memset(USART2_BUF, 0x00, USART2_BUF_SIZE);
	}
	MYDMA_Enable(DMA1_Stream5,USART2_BUF_SIZE);     //重新启动一次传输
	}
	OSIntExit();//告诉系统离开中断
}

extern USARTSEND usart3_send;


void USART3_IRQHandler(void)
{
#if OS_CRITICAL_METHOD == 3u                     /* Allocate storage for CPU status register           */
  OS_CPU_SR  cpu_sr = 0u;
#endif
  OS_ENTER_CRITICAL();   //保存全局中断标志,关总中断/* Tell uC/OS-II that we are starting an ISR*/
  OSIntNesting++;
  OS_EXIT_CRITICAL();	   //恢复全局中断标志		
	
	/*串口发送*/
	if(USART_GetITStatus(USART3, USART_IT_TXE) != RESET)
	{
		USART_ClearITPendingBit(USART3,USART_IT_TXE);//清除发送中断标志位

		USART_SendData(USART3, usart3_send.pdata[usart3_send.cnt ++]);

    if(usart3_send.cnt > usart3_send.lenth)
    {
      /* 关闭发送空中断，开启发送完成中断 */
      USART_ITConfig(USART3, USART_IT_TXE, DISABLE);
      USART_ITConfig(USART3, USART_IT_TC, ENABLE);
    }    
  }
  //发送完成中断，最后一字节发送
  else if(USART_GetITStatus(USART3, USART_IT_TC) != RESET)
  {
  	USART_ITConfig(USART3,USART_IT_TC,DISABLE);
  }

	OSIntExit();
}
