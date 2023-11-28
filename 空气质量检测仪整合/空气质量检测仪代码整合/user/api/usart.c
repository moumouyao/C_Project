#include "usart.h"

uint8_t usart_RXdata;		//��Ž������ݼĴ�����ֵ
uint8_t usart_RXflag;		//�������ݱ�־λ
uint8_t usart_TXpacket[LEN];	//hex���ݰ���������
uint8_t usart_RXpacket[LEN] = {0};	//hex���ݰ���������
char usart_RX_str_packet[STRLEN] = {0};	//�ı����ݰ���������

/**
  * @brief  printf����ض���
  * @param  None
  * @retval None
  */
int fputc(int ch, FILE *f){
	usart_sendbit(USART1, ch);
	return ch;
}

/**
  * @brief  ��װsprintf���κδ��ڶ�������
  * @param  char *format ��Ҫ��ӡ���ַ���
  * @retval None
  */
void usart_printf(char *format, ...){
	char string[100];
	va_list arg;
	va_start(arg, format);
	vsprintf(string, format, arg);
	va_end(arg);
	usart_send_string(USART1, string);
}

/**
  * @brief  ���ô���1 PA9 ���͸�������    PA10 ���ո���ģʽ  
  * @param  None
  * @retval None
  */
void usart_init(void){
	//��ʼ��GPIO��
	//PA9��������
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
  	GPIO_InitTypeDef GPIO_InitStructure;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  	GPIO_Init(GPIOA, &GPIO_InitStructure);
	//PA10����
	  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	  GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	//���ڳ�ʼ��
	//��������ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	//��ʼ������
	USART_InitTypeDef USART_InitStruct = {0};
	USART_InitStruct.USART_BaudRate = 115200;	//���ò�����
	USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	//Ӳ��������ʧ��
	USART_InitStruct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//ѡ�񴮿ڷ���ģʽ�ͽ���ģʽ
	USART_InitStruct.USART_Parity = USART_Parity_No;	//����ҪУ��
	USART_InitStruct.USART_StopBits = USART_StopBits_1;	//һλֹͣλ
	USART_InitStruct.USART_WordLength = USART_WordLength_8b;	//�ֳ�ѡ��8λ
	USART_Init(USART1, &USART_InitStruct);
	
	//���������ж�
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	
		//��ʼ��NVIC
		//NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
  	NVIC_InitTypeDef NVIC_InitStruct;
  	NVIC_InitStruct.NVIC_IRQChannel = USART1_IRQn;
  	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
  	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 4;
  	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
  	NVIC_Init(&NVIC_InitStruct);
	
	//ʹ�ܴ���
	USART_Cmd(USART1, ENABLE);
}



/**
  * @brief  ���ô���3 PB10 ���͸�������    PB11 ���ո���ģʽ  
	*						����ESP8266ͨ��
  * @param  None
  * @retval None
  */
void usart3_init(void){
	//��ʼ��GPIO��
	//PB10��������
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
  	GPIO_InitTypeDef GPIO_InitStructure;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  	GPIO_Init(GPIOB, &GPIO_InitStructure);
	//PB11����
	  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	  GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	//���ڳ�ʼ��
	//��������ʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
	//��ʼ������
	USART_InitTypeDef USART_InitStruct = {0};
	USART_InitStruct.USART_BaudRate = 115200;	//���ò�����
	USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	//Ӳ��������ʧ��
	USART_InitStruct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//ѡ�񴮿ڷ���ģʽ�ͽ���ģʽ
	USART_InitStruct.USART_Parity = USART_Parity_No;	//����ҪУ��
	USART_InitStruct.USART_StopBits = USART_StopBits_1;	//һλֹͣλ
	USART_InitStruct.USART_WordLength = USART_WordLength_8b;	//�ֳ�ѡ��8λ
	USART_Init(USART3, &USART_InitStruct);
	
	//���������ж�
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);	//�����ж�
	USART_ITConfig(USART3, USART_IT_IDLE, ENABLE);	//�����ж�
	
		//��ʼ��NVIC
		//NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
  	NVIC_InitTypeDef NVIC_InitStruct;
  	NVIC_InitStruct.NVIC_IRQChannel = USART3_IRQn;
  	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
  	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 4;
  	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
  	NVIC_Init(&NVIC_InitStruct);
	
	//ʹ�ܴ���
	USART_Cmd(USART3, ENABLE);
}


/**
  * @brief  ���͵����ֽ�
* @param  uint8_t data	��Ҫ���͵��ֽ�
  * @retval None
  */
void usart_sendbit(USART_TypeDef* USARTx, uint8_t data){
	while(USART_GetFlagStatus(USARTx, USART_FLAG_TC) != 1); //�ȴ���һ�����ݷ������
	USART_SendData(USARTx, data);	//��������
}

/**
	* @brief  ����hex���ݰ����̶��ֽڳ���
	* 					���ݰ�ͷ 0xFF			���ݰ�β oxFE
  * @param  None
  * @retval None
  */
void usart_send_hex_packet(USART_TypeDef* USARTx){
	usart_sendbit(USARTx, 0xFF);
	usart_send_array(USARTx, usart_TXpacket, LEN);
	usart_sendbit(USARTx, 0xFE);
}

/**
	* @brief  ����һ������
	* @param  uint8_t *array ��Ҫ���͵�����
	* @param	uint16_t len	���͵����鳤��
  * @retval None
  */
void usart_send_array(USART_TypeDef* USARTx, uint8_t *array, uint16_t len){
	uint16_t i = 0;
	for(i = 0; i < len; i++){
		usart_sendbit(USARTx, array[i]);
	}
}

/**
	* @brief  ����һ���ַ���
	* @param  char *str ��Ҫ���͵��ַ���
  * @retval None
  */
void usart_send_string(USART_TypeDef* USARTx, char *str){
	uint8_t i = 0;
	for(i = 0; str[i] != '\0'; i ++){
		usart_sendbit(USARTx, str[i]);
		//printf("%c", str[i]);
	}
}

/**
  * @brief  ��ȡx��y�η�
  * @param  uint32_t x ����
	* @param  uint32_t y �η�
  * @retval result ������
  */
uint32_t usart_pow(uint32_t x, uint32_t y){
	uint32_t result = 1;
	while(y--){
		result *= x;
	}
	return result;
}

/**
	* @brief  ����һ������
	* @param  uint32_t num ��Ҫ���͵�����
	* @param  uint8_t len ��Ҫ���͵����ֳ���
  * @retval None
  */
void usart_send_num(USART_TypeDef* USARTx, uint32_t num, uint8_t len){
	uint8_t i = 0;
	for(i = 0; i < len; i ++){
		usart_sendbit(USARTx, num / usart_pow(10, len - i - 1) % 10 + '0');
	}
}


/**
* @brief  ���Ͷ���ַ�
  * @param  None
  * @retval None
  */
void usaer_sendstr(USART_TypeDef* USARTx, uint8_t str[], uint8_t len){
	for(uint8_t i = 0; i < len; i++){
		usart_sendbit(USARTx, str[i]);
	}
}
/**
  * @brief  ���յ����ֽ� ��������
  * @param  None
  * @retval None
  */
uint8_t usart_recv(USART_TypeDef* USARTx){
	uint8_t data = 0;
	//�жϽ��ջ�������־λ
	while(USART_GetFlagStatus(USARTx, USART_FLAG_RXNE) != 1);	//���յ�����
	data = USART_ReceiveData(USARTx);
	return data;
}
/**
  * @brief  ����һ�жϷ�����������һ���ֽڵ����ݣ�������־λ��1
	* 			�ı����ݰ�	���� ����		��ʼ��־λ @    ������־λ * &
	*       hex���ݰ�		����  4 		��ʼ��־λ 0xFF ������־λ 0xFE
  * @param  None
  * @retval None
  */
void USART1_IRQHandler(void){
	static uint8_t RXState = 0;
	static uint8_t pRXpacket = 0;
	
	if(USART_GetITStatus(USART1, USART_IT_RXNE) == SET){
		USART_ClearITPendingBit(USART1, USART_IT_RXNE);
		usart_RXdata = USART_ReceiveData(USART1);
//		  USART3->DR =  usart_RXdata;
//		//printf("%s", USART3->DR);
//		return;
	//	usart_send_string(USART1, "y");
		//�жϽ��յ����ݰ�ͷ
		if(RXState == 0){	
			if(usart_RXdata == 0XFF){	//���յ���ʹhex�ļ�
				RXState = 1;	
				pRXpacket = 0;
			}
			else if(usart_RXdata == '@'){	//���յ���ʹ�ı��ļ�
				RXState = 3;	
				pRXpacket = 0;
			}
		}
			else if(RXState == 1){	//����hex�ļ�������
				usart_RXpacket[pRXpacket] = usart_RXdata;
				pRXpacket++;
				if(pRXpacket >= 3){		//�������ݰ�����λ4������
					RXState = 2;
				}
			}
			else if(RXState == 3){	//�����ı��ļ�������
				if(usart_RXdata == '*'){	//��һ��������־λ
					RXState = 4;
				}
				else{
					usart_RX_str_packet[pRXpacket] = usart_RXdata;	//�����ı�
					pRXpacket++;
				}
			}	
			else if(RXState == 2){	//�ж�hex���ݰ�������־λ
				if(usart_RXdata == 0XFE){
					RXState = 0;
					usart_RXflag = 1;
				}
			}
			else if(RXState == 4){	//�ж��ı����ݰ�������־λ
				if(usart_RXdata == '&'){
					usart_RXpacket[pRXpacket] = '\0';
					RXState = 0;
					usart_RXflag = 1;
				//	printf("%s", usart_RXpacket);
				}
			}

	
	}
}





void USART3_IRQHandler(void){
	uint8_t data;
	if(USART_GetITStatus(USART3, USART_IT_RXNE) == SET){
		USART_ClearITPendingBit(USART3, USART_IT_RXNE);
//		usart_RXdata = USART_ReceiveData(USART3);
		data = USART3->DR;
		USART1->DR = data;
		if(data == '\0'){
			data = '?';
		}
		esp8266.RX_data[esp8266.RX_count++] = data;
		
		//printf("%d", esp8266.RX_count);
	}
//�����ж�  1֡���ݽ������ �����
	if(USART_GetITStatus(USART3, USART_IT_IDLE) == SET) 
	{
		//USART_ClearITPendingBit(USART3, USART_IT_IDLE);
		esp8266.RX_flag = 1;
		data = USART3->SR;//�Ĵ���д�����ַ��շ�
		data = USART3->DR;//�ȶ�SR���ٶ�DR ���־
		//printf("�ж���ֵ��%d\r\n", esp8266.RX_flag);
		

	}
}



/**
  * @brief  ��ȡusart_RXflag��ֵ
  * @param  None
  * @retval usart_RXflag ����1���ձ�־λ 
  */
uint8_t usart_get_RXflag(void){
	if(usart_RXflag == 1){
		usart_RXflag = 0;
		return 1;
	}
	return 0;
}

/**
  * @brief  ����1���յ�������
  * @param  None
  * @retval None
  */
uint8_t usart_get_RXdata(void){
	return usart_RXdata;
}
