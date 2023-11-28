#include "kqm.h"

uint8_t usart_kqm_RXdata;		//��Ž������ݼĴ�����ֵ
uint8_t usart_kqm_RXflag;		//�������ݱ�־λ
uint8_t usart_kqm_RXpacket[8] = {0};	//hex���ݰ���������
air_data kqm_data = {0};

/**
  * @brief  ����PA3������kqm����ͨ��
  * @param  None
  * @retval None
  */
void kqm_init(void){
	//��ʼ��GPIO��
	//PA3��������
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
  	GPIO_InitTypeDef GPIO_InitStructure;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	  GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	//���ڳ�ʼ��
	//��������ʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
	//��ʼ������
	USART_InitTypeDef USART_InitStruct = {0};
	USART_InitStruct.USART_BaudRate = 9600;	//���ò�����
	USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	//Ӳ��������ʧ��
	USART_InitStruct.USART_Mode = USART_Mode_Rx;	//ѡ�񴮿ڽ���ģʽ
	USART_InitStruct.USART_Parity = USART_Parity_No;	//����ҪУ��
	USART_InitStruct.USART_StopBits = USART_StopBits_1;	//һλֹͣλ
	USART_InitStruct.USART_WordLength = USART_WordLength_8b;	//�ֳ�ѡ��8λ
	USART_Init(USART2, &USART_InitStruct);
	
	//���������ж�
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
	
		//��ʼ��NVIC
		//NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
  	NVIC_InitTypeDef NVIC_InitStruct;
  	NVIC_InitStruct.NVIC_IRQChannel = USART2_IRQn;
  	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
  	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 4;
  	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
  	NVIC_Init(&NVIC_InitStruct);
	
	//ʹ�ܴ���
	USART_Cmd(USART2, ENABLE);
}

/**
  * @brief  ����һ�жϷ�����������һ���ֽڵ����ݣ�������־λ��1
	*       hex���ݰ�		����  8 		��ʼ��־λ 0x5F 
  * @param  None
  * @retval None
  */
void USART2_IRQHandler(void){
	static uint8_t RX_kqm_State = 0;
	static uint8_t kqm_pRXpacket = 0;
	static uint16_t sum = 0X5F;
	//usart_init();
	//usart_send_string("y");
	if(USART_GetITStatus(USART2, USART_IT_RXNE) == SET){
		USART_ClearITPendingBit(USART2, USART_IT_RXNE);
		
		usart_kqm_RXdata = USART_ReceiveData(USART2);
		//�жϽ��յ����ݰ�ͷ
		if(RX_kqm_State == 0){	
			if(usart_kqm_RXdata == 0X5F){	//���յ���ʹhex�ļ�
				usart_kqm_RXpacket[0] = usart_kqm_RXdata;
				RX_kqm_State = 1;	
				kqm_pRXpacket = 1;
				sum = 0X5F;
			}
		}
			else if(RX_kqm_State == 1){	//����hex�ļ�������
				usart_kqm_RXpacket[kqm_pRXpacket] = usart_kqm_RXdata;
				sum = sum + usart_kqm_RXpacket[kqm_pRXpacket];
				kqm_pRXpacket++;
				if(kqm_pRXpacket >= 7){		//�������ݰ�����λ4������
					RX_kqm_State = 2;
				}
			}
			else if(RX_kqm_State == 2){	//�ж�hex���ݰ�������־λ
				if(usart_kqm_RXdata == sum){
					usart_kqm_RXpacket[kqm_pRXpacket] = usart_kqm_RXdata;
					RX_kqm_State = 0;
					usart_kqm_RXflag = 1;
				}
				else{
					RX_kqm_State = 0;
					usart_kqm_RXflag = 0;
					usart_send_array(USART1, usart_kqm_RXpacket, 8);
					usart_kqm_RXpacket[kqm_pRXpacket] = usart_kqm_RXdata;
					usart_send_string(USART1, "$");
				}
			}


	
	}
}

/**
  * @brief  ��ȡusart_kqm_RXflag��ֵ
  * @param  None
  * @retval usart_kqm_RXflag ����2���ձ�־λ 
  */
uint8_t usart_get_kqm__RXflag(void){
	if(usart_kqm_RXflag == 1){
		usart_kqm_RXflag = 0;
		return 1;
	}
	return 0;
}

void air_analysis(void){
	
		if(usart_get_kqm__RXflag() == 1){
			kqm_data.CHO = ((usart_kqm_RXpacket[3] << 8) | usart_kqm_RXpacket[4]) * 0.01;
			kqm_data.CO2 = ((usart_kqm_RXpacket[5] << 8) | usart_kqm_RXpacket[6]);
			kqm_data.VOC = ((usart_kqm_RXpacket[1] << 8) | usart_kqm_RXpacket[2]) * 0.1;
//			printf("VOC = %.2fPPM\t", kqm_data.VOC);
//			printf("CHO = %.2fmg/m3\t", kqm_data.CHO);
//			printf("CO2 = %dPPM\r\n", kqm_data.CO2);
			memset(usart_kqm_RXpacket, 0, sizeof(usart_kqm_RXpacket));
			}
		
}


