#include "sys.h"
#include "usart.h"	
#include "stm32f4xx_dma.h"
#include "sw_conf.h"
#include "memp.h"
#include "sw_global.h"
#include "led.h"
////////////////////////////////////////////////////////////////////////////////// 	 
//���ʹ��ucos,����������ͷ�ļ�����.
#if SYSTEM_SUPPORT_UCOS
#include "includes.h"					//ucos ʹ��	  
#endif
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32F4̽���߿�����
//����1��ʼ��		   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2014/6/10
//�汾��V1.5
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved
//********************************************************************************
//V1.3�޸�˵�� 
//֧����Ӧ��ͬƵ���µĴ��ڲ���������.
//�����˶�printf��֧��
//�����˴��ڽ��������.
//������printf��һ���ַ���ʧ��bug
//V1.4�޸�˵��
//1,�޸Ĵ��ڳ�ʼ��IO��bug
//2,�޸���USART_RX_STA,ʹ�ô����������ֽ���Ϊ2��14�η�
//3,������USART_REC_LEN,���ڶ��崮�����������յ��ֽ���(������2��14�η�)
//4,�޸���EN_USART1_RX��ʹ�ܷ�ʽ
//V1.5�޸�˵��
//1,�����˶�UCOSII��֧��
////////////////////////////////////////////////////////////////////////////////// 	  
 

//////////////////////////////////////////////////////////////////
//�������´���,֧��printf����,������Ҫѡ��use MicroLIB	  
#if 1
#pragma import(__use_no_semihosting)             
//��׼����Ҫ��֧�ֺ���                 
struct __FILE 
{ 
	int handle; 
}; 

FILE __stdout;       
//����_sys_exit()�Ա���ʹ�ð�����ģʽ    
void _sys_exit(int x) 
{ 
	x = x; 
} 
//�ض���fputc���� 
int fputc(int ch, FILE *f)
{ 	
	while((USART1->SR&0X40)==0);//ѭ������,ֱ���������   
	USART1->DR = (u8) ch;      
	return ch;
}
#endif
 


//��ʼ��IO ����1 
//bound:������
void uart_init(u32 bound){
   //GPIO�˿�����
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE); //ʹ��GPIOAʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);//ʹ��USART1ʱ��
 
	//����1��Ӧ���Ÿ���ӳ��
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource9,GPIO_AF_USART1); //GPIOA9����ΪUSART1
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource10,GPIO_AF_USART1); //GPIOA10����ΪUSART1
	
	//USART1�˿�����
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10; //GPIOA9��GPIOA10
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//�ٶ�50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //����
	GPIO_Init(GPIOA,&GPIO_InitStructure); //��ʼ��PA9��PA10

   //USART1 ��ʼ������
	USART_InitStructure.USART_BaudRate = bound;//����������
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ
	USART_Init(USART1, &USART_InitStructure); //��ʼ������1
	
	USART_Cmd(USART1, ENABLE);  //ʹ�ܴ���1 
	
	USART_ClearFlag(USART1, USART_FLAG_TC);
}	

void uart_init_2(u32 bound){
   //GPIO�˿�����
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD,ENABLE); //ʹ��GPIOAʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);//ʹ��USART1ʱ��
	//����2��Ӧ���Ÿ���ӳ��
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource5,GPIO_AF_USART2); //GPIOA3����ΪUSART2_RX
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource6,GPIO_AF_USART2); //GPIOD5����ΪUSART2_TX
	
	//USART1�˿�����


	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5|GPIO_Pin_6; //GPIOD5
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//�ٶ�50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //����
	GPIO_Init(GPIOD,&GPIO_InitStructure); //��ʼ��PA9��PA10
   //USART1 ��ʼ������
	USART_InitStructure.USART_BaudRate = bound;//����������
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ
	USART_Init(USART2, &USART_InitStructure); //��ʼ������2
	
	USART_Cmd(USART2, ENABLE);  //ʹ�ܴ���2
	
	USART_ClearFlag(USART2, USART_FLAG_TC);
	
	USART_ITConfig(USART2, USART_IT_IDLE, ENABLE);//��������ж�

	//Usart1 NVIC ����
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;//����1�ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =3;		//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���
}

void uart_init_3(u32 bound){
   //GPIO�˿�����
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE); //ʹ��GPIODʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);//ʹ��USART3ʱ��
 
	//����1��Ӧ���Ÿ���ӳ��
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource10,GPIO_AF_USART3); //GPIOD8����ΪUSART3
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource11,GPIO_AF_USART3); //GPIOD9����ΪUSART3
	
	//USART1�˿�����
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11; //GPIOA8��GPIOA9
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//�ٶ�50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //����
	GPIO_Init(GPIOB,&GPIO_InitStructure); //��ʼ��PA9��PA10

//   //USART1 ��ʼ������
	USART_InitStructure.USART_BaudRate = bound;//����������
//	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
//	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
//	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
//	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
//	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ
//	USART_Init(USART3, &USART_InitStructure); //��ʼ������3
	
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_Mode = USART_Mode_Tx;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;

  /* Configure USART1 */
  USART_Init(USART3, &USART_InitStructure);
//	USART_ITConfig(USART3s, USART_IT_TXE, ENABLE);//ʹ��ʱ��
  USART_Cmd(USART3, ENABLE);	 
	
	
//	USART_ClearFlag(USART3, USART_FLAG_TC);
//	
//	USART_ITConfig(USART3, USART_IT_IDLE, ENABLE);//��������ж�

	//Usart1 NVIC ����
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;//���ȼ�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;  //��ռ���ȼ�
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
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

void printf_usart1(unsigned char * data,uint16_t len)
{
	unsigned char data1=0;
	for(int i=0;i<len;i++)
	{
	data1=data[i];
	USART_SendData(USART1,data1);//��������⺯��byte����д�뵽TDR
	//��Ҫ�ȴ�һ�£��ȴ�TDR�����Ƶ���λ�Ĵ��������ǲ��ܽ�����һ�������
	while(USART_GetFlagStatus(USART1,USART_FLAG_TXE)==RESET);
	}
}

void printf_usart2(unsigned char * data,uint16_t len)
{
	unsigned char data1=0;
	for(int i=0;i<len;i++)
	{
	data1=data[i];
	USART_SendData(USART2,data1);//��������⺯��byte����д�뵽TDR
	//��Ҫ�ȴ�һ�£��ȴ�TDR�����Ƶ���λ�Ĵ��������ǲ��ܽ�����һ�������
	while(USART_GetFlagStatus(USART2,USART_FLAG_TXE)==RESET);
	}
}

void printf_usart3(unsigned char * data,uint16_t len)
{
	unsigned char data1=0;
	for(int i=0;i<len;i++)
	{
	data1=data[i];
	USART_SendData(USART3,data1);//��������⺯��byte����д�뵽TDR
	//��Ҫ�ȴ�һ�£��ȴ�TDR�����Ƶ���λ�Ĵ��������ǲ��ܽ�����һ�������
	while(USART_GetFlagStatus(USART3,USART_FLAG_TXE)==RESET);
	}
}

extern unsigned char *USART2_BUF;

void USART2_IRQHandler(void)                	//����1�жϷ������
{
	u8 temp=0;
	StrFrameHeader *frame_head;//���ڶ���
	INT8U *memp_buf;//��������
	unsigned char err=0;
	unsigned int i;
	temp = err;
	err = temp;
	
	OSIntEnter();//����ϵͳ�����ж�
	if(USART_GetFlagStatus(USART2, USART_FLAG_IDLE) != RESET)
	{
  temp = USART2->SR;  
  temp = USART2->DR;//���IDLE�ж�
	temp = DMA_GetCurrDataCounter(DMA1_Stream5);//��ȡʣ��Ľ������ݵ���
	if(USART2_BUF[0] == '{')//�ж�֡ͷ
	{
		memp_buf = memp_malloc(MEMP_PBUF_POOL);//����һ���ڴ�pool
		frame_head = (StrFrameHeader *)memp_buf;
		frame_head->type = HEAD_USART2_TYPE;//����������
		frame_head->pdata = memp_buf + sizeof(StrFrameHeader);
		memp_buf =frame_head->pdata;         //��λ��������ָ��

		//==========�������ݳ���====================//
//		unsigned int iLong;//���ݳ���
//		for(iLong=2048;USART2_BUF[iLong]!='>'||USART2_BUF[iLong]!='}';iLong--);

			//==========д�����==================//
			for(i=0;i<512;i++)//=======ת�뱨��
			{
				*memp_buf++ = USART2_BUF[i];   
			}
			frame_head->len = memp_buf-frame_head->pdata;//�����
			
			err = OSQPost(En_Dis_CodeQeue,(void *)frame_head);//�ѿ��������ݷ��͸����ܽ�������
			memset(USART2_BUF, 0x00, USART2_BUF_SIZE);
	}
	MYDMA_Enable(DMA1_Stream5,USART2_BUF_SIZE);     //��������һ�δ���
	}
	OSIntExit();//����ϵͳ�뿪�ж�
}

extern USARTSEND usart3_send;


void USART3_IRQHandler(void)
{
#if OS_CRITICAL_METHOD == 3u                     /* Allocate storage for CPU status register           */
  OS_CPU_SR  cpu_sr = 0u;
#endif
  OS_ENTER_CRITICAL();   //����ȫ���жϱ�־,�����ж�/* Tell uC/OS-II that we are starting an ISR*/
  OSIntNesting++;
  OS_EXIT_CRITICAL();	   //�ָ�ȫ���жϱ�־		
	
	/*���ڷ���*/
	if(USART_GetITStatus(USART3, USART_IT_TXE) != RESET)
	{
		USART_ClearITPendingBit(USART3,USART_IT_TXE);//��������жϱ�־λ

		USART_SendData(USART3, usart3_send.pdata[usart3_send.cnt ++]);

    if(usart3_send.cnt > usart3_send.lenth)
    {
      /* �رշ��Ϳ��жϣ�������������ж� */
      USART_ITConfig(USART3, USART_IT_TXE, DISABLE);
      USART_ITConfig(USART3, USART_IT_TC, ENABLE);
    }    
  }
  //��������жϣ����һ�ֽڷ���
  else if(USART_GetITStatus(USART3, USART_IT_TC) != RESET)
  {
  	USART_ITConfig(USART3,USART_IT_TC,DISABLE);
  }

	OSIntExit();
}
