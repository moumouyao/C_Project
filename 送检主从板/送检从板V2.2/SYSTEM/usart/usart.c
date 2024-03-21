#include "sys.h"
#include "usart.h"	
#include "stm32f4xx_dma.h"
#include "led.h"
#include "sw_config.h"

#include "FreeRTOS.h"
#include "task.h"
#include "string.h"
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
//int fputc(int ch, FILE *f)
//{ 	
//	while((USART1->SR&0X40)==0);//ѭ������,ֱ���������   
//	USART1->DR = (u8) ch;      
//	return ch;
//}

int fputc(int ch, FILE *f)
{ 	
	while((USART6->SR&0X40)==0);//ѭ������,ֱ���������   
	USART6->DR = (u8) ch;      
//	USART_SendData(USART6, (uint8_t)ch);
//	while(USART_GetFlagStatus(USART6, USART_FLAG_TXE) == RESET);
	return ch;
}

#endif
uint16_t USART_RX_STA=0;
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
//��ʼ��IO ����2
//bound:������
void uart2_init(u32 bound){
   //GPIO�˿�����
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStruct;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD,ENABLE); //ʹ��GPIOAʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);//ʹ��USART1ʱ��
 
	//����1��Ӧ���Ÿ���ӳ��
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource5,GPIO_AF_USART2); //GPIOA9����ΪUSART1
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource6,GPIO_AF_USART2); //GPIOA10����ΪUSART1
	
	//USART1�˿�����
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6; //GPIOA9��GPIOA10
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//�ٶ�50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //����
	GPIO_Init(GPIOD,&GPIO_InitStructure); //��ʼ��PA9��PA10
	
	//���ô���2�����������������
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_OUT;//���
	GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;  //�������
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_NOPULL;  
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_100MHz; //����GPIO
	GPIO_Init(GPIOC,&GPIO_InitStructure);
	GPIO_ResetBits(GPIOH,GPIO_Pin_3); 

   //USART1 ��ʼ������
	USART_InitStructure.USART_BaudRate = bound;//����������
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ
	USART_Init(USART2, &USART_InitStructure); //��ʼ������1
	

	
	USART_ClearFlag(USART2, USART_FLAG_TC);
	
  /* ʹ�ܴ��ڽ����ж� */
  USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//��������
	USART_ITConfig(USART2, USART_IT_IDLE, ENABLE);//�������

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);//����ϵͳ�ж����ȼ����� 4

  /* Enable the USART3 Interrupt */
	NVIC_InitStruct.NVIC_IRQChannel=USART2_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority=1;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority=1;
	NVIC_InitStruct.NVIC_IRQChannelCmd=ENABLE;
	
	NVIC_Init(&NVIC_InitStruct);
	
	USART_Cmd(USART2, ENABLE);  //ʹ�ܴ���1 

  /*����485оƬ�������ģʽ*/
  GPIO_ResetBits(GPIOH,GPIO_Pin_3);
	
}
/*
 *
 *��ʼ������3
 *
 */
void User_USART3Config (uint8_t slave_addr)
{
	
	
   //GPIO�˿�����
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
  NVIC_InitTypeDef NVIC_InitStructure;
	
	USART_DeInit(USART3);
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC,ENABLE); //ʹ��GPIOBʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);//ʹ��USART3ʱ��
 
	//����3��Ӧ���Ÿ���ӳ��
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource10,GPIO_AF_USART3); //GPIOB10����ΪUSART3
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource11,GPIO_AF_USART3); //GPIOB11����ΪUSART3
	
	//USART3�˿�����
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF;//���ù���
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//�ٶ�50MHz
	GPIO_Init(GPIOC,&GPIO_InitStructure); //��ʼ��PA9��PA10	

	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF;//���ù���
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_Init(GPIOC,&GPIO_InitStructure); //��ʼ��PA9��PA10	
	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_OUT;//���
	GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;  //�������
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_NOPULL;  
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_100MHz; //����GPIO
	GPIO_Init(GPIOC,&GPIO_InitStructure);
	GPIO_ResetBits(GPIOC,GPIO_Pin_12); 
	
  USART_InitStructure.USART_BaudRate = 460800;//������9600
  USART_InitStructure.USART_WordLength = USART_WordLength_9b;//9λ����λ
  USART_InitStructure.USART_StopBits = USART_StopBits_1;//1λֹͣλ
  USART_InitStructure.USART_Parity = USART_Parity_No;//��У��λ
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������
  USART_InitStructure.USART_Mode = USART_Mode_Tx|USART_Mode_Rx;//���ͺͽ���ģʽ
  USART_Init(USART3,&USART_InitStructure);
  
  
	/*ʹ��USART3�����ж�*/
  USART_ITConfig(USART3,USART_IT_RXNE,ENABLE);
  
  /*ʹ��USART3*/
  USART_Cmd(USART3,ENABLE); 
	/* CPU��Сȱ�ݣ��������úã����ֱ��Send�����1���ֽڷ��Ͳ���ȥ
		�����������1���ֽ��޷���ȷ���ͳ�ȥ������ 
		��������ж��з��ͣ������������䲻��Ҫִ�� */
 	//USART_ClearFlag(USART3, USART_FLAG_TC);     /* �巢����Ǳ�־��Transmission Complete flag */	
  /* Set the USART2 Address */
  USART_SetAddress(USART3, slave_addr);
  /* Select the USART2 WakeUp Method */
  USART_WakeUpConfig(USART3, USART_WakeUp_AddressMark);	   //�����ദ����ͨѶ
  /* Enable the USART2 mute mode*/
  USART_ReceiverWakeUpCmd(USART3, ENABLE);    //���뾲Ĭģʽ
	
  /* Configure the NVIC Preemption Priority Bits */  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);//����ϵͳ�ж����ȼ����� 4

  /* Enable the USART3 Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel=USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=1;
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	
	NVIC_Init(&NVIC_InitStructure);


}

//��ʼ��IO ����6
//bound:������
//PG9 RX 		PG14 TX
void uart6_init(u32 bound){
   //GPIO�˿�����
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG,ENABLE); //ʹ��GPIOAʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART6,ENABLE);//ʹ��USART1ʱ��
 
	//����1��Ӧ���Ÿ���ӳ��
	GPIO_PinAFConfig(GPIOG,GPIO_PinSource9,GPIO_AF_USART6); //GPIOA9����ΪUSART1
	GPIO_PinAFConfig(GPIOG,GPIO_PinSource14,GPIO_AF_USART6); //GPIOA10����ΪUSART1
	
	//USART1�˿�����
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_14; //GPIOA9��GPIOA10
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//�ٶ�50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //����
	GPIO_Init(GPIOG,&GPIO_InitStructure); //��ʼ��PA9��PA10

   //USART1 ��ʼ������
	USART_InitStructure.USART_BaudRate = bound;//����������
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ
	USART_Init(USART6, &USART_InitStructure); //��ʼ������1
	
	USART_Cmd(USART6, ENABLE);  //ʹ�ܴ���1
	
	
	USART_ClearFlag(USART6, USART_FLAG_TC);
}	



void RS485TranConfig (uint8_t *p,uint16_t num)
{
  RS485TranCnt = 0;                      //�������ݼ���������
  RS485TranLength = num;                 //�������ݳ�������
	RS485TranDataPot = p;	                 //��������ָ��
}


//����2�жϺ���
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


//��ջ�����
void clean_usart2_rebuff(void)
{
    usart2_buf.recv_flag = 0;
    usart2_buf.len = 0;
    memset(usart2_buf.data, 0, USART2_RECV_BUF_SIZE);
}

//ʹ��485Э�鷢��һ������
void RS485_SendByte(USART_TypeDef* USARTx, uint8_t* data, uint16_t size)
{
		U2_SLAVE485Out;
		for(uint16_t i = 0; i < size; i++){
			USART_SendData(USARTx,(*data)++);
			while (USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET);
		}
    U2_SLAVE485In;
}


