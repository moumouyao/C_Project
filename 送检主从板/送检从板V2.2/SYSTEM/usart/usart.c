#include "sys.h"
#include "usart.h"	
#include "stm32f4xx_dma.h"
#include "led.h"
#include "sw_config.h"

#include "FreeRTOS.h"
#include "task.h"
#include "string.h"
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
//int fputc(int ch, FILE *f)
//{ 	
//	while((USART1->SR&0X40)==0);//循环发送,直到发送完毕   
//	USART1->DR = (u8) ch;      
//	return ch;
//}

int fputc(int ch, FILE *f)
{ 	
	while((USART6->SR&0X40)==0);//循环发送,直到发送完毕   
	USART6->DR = (u8) ch;      
//	USART_SendData(USART6, (uint8_t)ch);
//	while(USART_GetFlagStatus(USART6, USART_FLAG_TXE) == RESET);
	return ch;
}

#endif
uint16_t USART_RX_STA=0;
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
//初始化IO 串口2
//bound:波特率
void uart2_init(u32 bound){
   //GPIO端口设置
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStruct;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD,ENABLE); //使能GPIOA时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);//使能USART1时钟
 
	//串口1对应引脚复用映射
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource5,GPIO_AF_USART2); //GPIOA9复用为USART1
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource6,GPIO_AF_USART2); //GPIOA10复用为USART1
	
	//USART1端口配置
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6; //GPIOA9与GPIOA10
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//速度50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //上拉
	GPIO_Init(GPIOD,&GPIO_InitStructure); //初始化PA9，PA10
	
	//配置串口2的输入输出控制引脚
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_OUT;//输出
	GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;  //推挽输出
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_NOPULL;  
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_100MHz; //高速GPIO
	GPIO_Init(GPIOC,&GPIO_InitStructure);
	GPIO_ResetBits(GPIOH,GPIO_Pin_3); 

   //USART1 初始化设置
	USART_InitStructure.USART_BaudRate = bound;//波特率设置
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式
	USART_Init(USART2, &USART_InitStructure); //初始化串口1
	

	
	USART_ClearFlag(USART2, USART_FLAG_TC);
	
  /* 使能串口接收中断 */
  USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//接收数据
	USART_ITConfig(USART2, USART_IT_IDLE, ENABLE);//接收完成

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);//设置系统中断优先级分组 4

  /* Enable the USART3 Interrupt */
	NVIC_InitStruct.NVIC_IRQChannel=USART2_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority=1;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority=1;
	NVIC_InitStruct.NVIC_IRQChannelCmd=ENABLE;
	
	NVIC_Init(&NVIC_InitStruct);
	
	USART_Cmd(USART2, ENABLE);  //使能串口1 

  /*控制485芯片进入接收模式*/
  GPIO_ResetBits(GPIOH,GPIO_Pin_3);
	
}
/*
 *
 *初始化串口3
 *
 */
void User_USART3Config (uint8_t slave_addr)
{
	
	
   //GPIO端口设置
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
  NVIC_InitTypeDef NVIC_InitStructure;
	
	USART_DeInit(USART3);
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC,ENABLE); //使能GPIOB时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);//使能USART3时钟
 
	//串口3对应引脚复用映射
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource10,GPIO_AF_USART3); //GPIOB10复用为USART3
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource11,GPIO_AF_USART3); //GPIOB11复用为USART3
	
	//USART3端口配置
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF;//复用功能
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//速度50MHz
	GPIO_Init(GPIOC,&GPIO_InitStructure); //初始化PA9，PA10	

	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF;//复用功能
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_Init(GPIOC,&GPIO_InitStructure); //初始化PA9，PA10	
	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_OUT;//输出
	GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;  //推挽输出
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_NOPULL;  
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_100MHz; //高速GPIO
	GPIO_Init(GPIOC,&GPIO_InitStructure);
	GPIO_ResetBits(GPIOC,GPIO_Pin_12); 
	
  USART_InitStructure.USART_BaudRate = 460800;//波特率9600
  USART_InitStructure.USART_WordLength = USART_WordLength_9b;//9位数据位
  USART_InitStructure.USART_StopBits = USART_StopBits_1;//1位停止位
  USART_InitStructure.USART_Parity = USART_Parity_No;//无校验位
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件流控
  USART_InitStructure.USART_Mode = USART_Mode_Tx|USART_Mode_Rx;//发送和接收模式
  USART_Init(USART3,&USART_InitStructure);
  
  
	/*使能USART3接收中断*/
  USART_ITConfig(USART3,USART_IT_RXNE,ENABLE);
  
  /*使能USART3*/
  USART_Cmd(USART3,ENABLE); 
	/* CPU的小缺陷：串口配置好，如果直接Send，则第1个字节发送不出去
		如下语句解决第1个字节无法正确发送出去的问题 
		如果采用中断中发送，则下面这个语句不需要执行 */
 	//USART_ClearFlag(USART3, USART_FLAG_TC);     /* 清发送外城标志，Transmission Complete flag */	
  /* Set the USART2 Address */
  USART_SetAddress(USART3, slave_addr);
  /* Select the USART2 WakeUp Method */
  USART_WakeUpConfig(USART3, USART_WakeUp_AddressMark);	   //开启多处理器通讯
  /* Enable the USART2 mute mode*/
  USART_ReceiverWakeUpCmd(USART3, ENABLE);    //进入静默模式
	
  /* Configure the NVIC Preemption Priority Bits */  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);//设置系统中断优先级分组 4

  /* Enable the USART3 Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel=USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=1;
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	
	NVIC_Init(&NVIC_InitStructure);


}

//初始化IO 串口6
//bound:波特率
//PG9 RX 		PG14 TX
void uart6_init(u32 bound){
   //GPIO端口设置
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG,ENABLE); //使能GPIOA时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART6,ENABLE);//使能USART1时钟
 
	//串口1对应引脚复用映射
	GPIO_PinAFConfig(GPIOG,GPIO_PinSource9,GPIO_AF_USART6); //GPIOA9复用为USART1
	GPIO_PinAFConfig(GPIOG,GPIO_PinSource14,GPIO_AF_USART6); //GPIOA10复用为USART1
	
	//USART1端口配置
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_14; //GPIOA9与GPIOA10
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//速度50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //上拉
	GPIO_Init(GPIOG,&GPIO_InitStructure); //初始化PA9，PA10

   //USART1 初始化设置
	USART_InitStructure.USART_BaudRate = bound;//波特率设置
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式
	USART_Init(USART6, &USART_InitStructure); //初始化串口1
	
	USART_Cmd(USART6, ENABLE);  //使能串口1
	
	
	USART_ClearFlag(USART6, USART_FLAG_TC);
}	



void RS485TranConfig (uint8_t *p,uint16_t num)
{
  RS485TranCnt = 0;                      //发送数据计数器清零
  RS485TranLength = num;                 //发送数据长度清零
	RS485TranDataPot = p;	                 //发送数据指针
}


//串口2中断函数
uint8_t temp;

struct RS485RECV usart2_buf = {0};

void USART2_IRQHandler(void)
{
    if (usart2_buf.len<USART2_RECV_BUF_SIZE) {
        if (USART_GetITStatus(USART2, USART_IT_RXNE) != RESET) {
					usart2_buf.data[usart2_buf.len++] = USART_ReceiveData(USART2);
					
          USART_ClearITPendingBit(USART2, USART_IT_RXNE);
        }
				if (USART_GetITStatus(USART2, USART_IT_IDLE) != RESET){
//					temp = USART2->SR;
//					temp = USART2->DR;
					USART_ClearITPendingBit(USART2, USART_IT_IDLE);
					
					usart2_buf.recv_flag = 1;
				}
    } else {
        USART_ClearITPendingBit(USART2, USART_IT_RXNE);
				usart2_buf.recv_flag = 1;
    }
}


//清空缓冲区
void clean_usart2_rebuff(void)
{
    usart2_buf.recv_flag = 0;
    usart2_buf.len = 0;
    memset(usart2_buf.data, 0, USART2_RECV_BUF_SIZE);
}

//使用485协议发送一串数据
void RS485_SendByte(USART_TypeDef* USARTx, uint8_t* data, uint16_t size)
{
		U2_SLAVE485Out;
		for(uint16_t i = 0; i < size; i++){
			USART_SendData(USARTx,(*data)++);
			while (USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET);
		}
    U2_SLAVE485In;
}


