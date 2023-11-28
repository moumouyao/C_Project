#include "usart.h"

uint8_t usart_RXdata;		//��Ž������ݼĴ�����ֵ
uint8_t usart_RXflag;		//�������ݱ�־λ
uint8_t usart6_RXflag;		//�������ݱ�־λ
uint8_t usart_TXpacket[LEN];	//hex���ݰ���������
uint8_t usart_RXpacket[LEN] = {0};	//hex���ݰ���������

uint8_t usart6_TXpacket[USART6_SEND_LEN];	//hex���ݰ���������
uint8_t usart6_RXpacket[USART6_RECV_LEN] = {0};	//hex���ݰ���������
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
  * @brief  ���ô��� PA9 ���͸�������    PA10 ���ո���ģʽ  
  * @param  None
  * @retval None
  */
//void usart_init(void){
//	//��ʼ��GPIO��
//		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
//	//��������ʱ��
//		RCC_APB2PeriphResetCmd(RCC_APB2Periph_USART1, ENABLE);
//		GPIO_PinAFConfig(GPIOA,GPIO_PinSource9,GPIO_AF_USART1); //GPIOA9����ΪUSART1
//		GPIO_PinAFConfig(GPIOA,GPIO_PinSource10,GPIO_AF_USART1); //GPIOA10����ΪUSART1
////	//PA9��������
//  	GPIO_InitTypeDef GPIO_InitStructure;
////  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
////		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
////  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
////		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
////  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
////  	GPIO_Init(GPIOA, &GPIO_InitStructure);
////	//PA10����
////	  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
////		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
////		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
////  	GPIO_InitStructure.GPIO_Pin = GPIO_Speed_100MHz;
////	  GPIO_Init(GPIOA, &GPIO_InitStructure);
//	
//GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10; //GPIOA9 �� GPIOA10
//GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//���ù���
//GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; //�ٶ� 50MHz
//GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //���츴�����
//GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //����
//GPIO_Init(GPIOA,&GPIO_InitStructure); //��ʼ�� PA9��PA10

//		

//	
//	//���ڳ�ʼ��

//	//��ʼ������
//	USART_InitTypeDef USART_InitStruct = {0};
//	USART_InitStruct.USART_BaudRate = 115200;	//���ò�����
//	USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	//Ӳ��������ʧ��
//	USART_InitStruct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//ѡ�񴮿ڷ���ģʽ�ͽ���ģʽ
//	USART_InitStruct.USART_Parity = USART_Parity_No;	//����ҪУ��
//	USART_InitStruct.USART_StopBits = USART_StopBits_1;	//һλֹͣλ
//	USART_InitStruct.USART_WordLength = USART_WordLength_8b;	//�ֳ�ѡ��8λ
//	
//	USART_Init(USART1, &USART_InitStruct);
//		USART_ClearFlag(USART1, USART_FLAG_TC);
//	//���������ж�
//	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
//	
//		//��ʼ��NVIC
//		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
//  	NVIC_InitTypeDef NVIC_InitStruct;
//  	NVIC_InitStruct.NVIC_IRQChannel = USART1_IRQn;
//  	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
//  	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1;
//  	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1;
//  	NVIC_Init(&NVIC_InitStruct);
//	
//	//ʹ�ܴ���
//	USART_Cmd(USART1, ENABLE);
//}

void USART1_init(u32 bound)
	{
  float USARTDIV;uint16_t DIV_Man;uint16_t DIV_Fra;
	
	//GPIO��ʼ��:
	//PA9---���ó�USART1_TXģʽ
	//PA10--���ó�USART1_RXģʽ
	RCC->AHB1ENR |=0x01<<0;//�˿�Aʱ��ʹ��
	
	GPIOA->MODER &=~(0x0F<<18);//����
	GPIOA->MODER |=(0x0a<<18);//PA9��PA10���ù���
	GPIOA->OTYPER &=~(0x01<<9);//PA9���ù����������
	GPIOA->OSPEEDR &=~(0x03<<18);//PA9����2Mhz
	GPIOA->PUPDR &=~(0x0F<<18);//PA9��PA10����
	
	GPIOA->AFR[1] &=~(0xFF<<4);
	GPIOA->AFR[1] |=(0x77<<4);//PA9---���ó�USART1_TX
														//PA10--���ó�USART1_RX
	
	//����USART1
	RCC->APB2ENR |=0x01<<4;//ģ��ʱ��ʹ��--84Mhz
	USART1->CR1=0;//�Ĵ�������
	USART1->CR2 &=~(0x03<<12);
	/*
	16 ��������
	1 ��ʼλ�� 8 ����λ�� 1 ֹͣλ
	��ֹ��żУ�����
	*/
	
	//����������
	USARTDIV=84000000.0/(16*bound);
	DIV_Man=USARTDIV;
	DIV_Fra=(USARTDIV-DIV_Man)*16+0.5;
	USART1->BRR=DIV_Man<<4  | DIV_Fra;
	
		//USART1->CR1 |=0X01<<4;  //�����ж�ʹ��
	USART1->CR1 |=0X01<<5; //���ջ������ǿ��ж�ʹ��
	
	//nvic������uart1�����ȼ���ʹ��uart1�ж�
	NVIC_SetPriority(USART1_IRQn,NVIC_EncodePriority(7-2,3,3));
	NVIC_EnableIRQ(USART1_IRQn);
	
	
	USART1->CR1 |=0x01<<3;//ʹ�ܷ�����
	USART1->CR1 |=0x01<<2;//ʹ�ܽ�����
	USART1->CR1 |=0x01<<13;//ʹ�ܴ���1
}



void USART6_init(u32 bound)
	{
   //GPIO�˿�����
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
//	RXD1_P=USART_RX_BUF;//ָ��ָ�򴮿ڽ������ݵĻ�����	
		
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC,ENABLE); //ʹ��GPIOAʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART6,ENABLE);//ʹ��USART1ʱ��
 
	//����1��Ӧ���Ÿ���ӳ��
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource6,GPIO_AF_USART6); //GPIOC6����ΪUSART6
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource7,GPIO_AF_USART6); //GPIOAC7����ΪUSART6
	
	//USART1�˿�����
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7; //GPIOC6��GPIOC7
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//�ٶ�50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //����
	GPIO_Init(GPIOC,&GPIO_InitStructure); //��ʼ��PA9��PA10

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
	
#if 1	

	USART_ITConfig(USART6, USART_IT_RXNE, ENABLE);//��������ж�
	USART_ITConfig(USART6, USART_IT_IDLE, ENABLE);//��������ж�

	//Usart1 NVIC ����
  NVIC_InitStructure.NVIC_IRQChannel = USART6_IRQn;//����1�ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3;//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =3;		//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ�����

#endif
	
}


void USART2_init(u32 bound)
	{
   //GPIO�˿�����
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
//	RXD1_P=USART_RX_BUF;//ָ��ָ�򴮿ڽ������ݵĻ�����	
		
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE); //ʹ��GPIOAʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);//ʹ��USART2ʱ��
 
	//����1��Ӧ���Ÿ���ӳ��
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource2,GPIO_AF_USART2); //GPIOC6����ΪUSART6
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource3,GPIO_AF_USART2); //GPIOAC7����ΪUSART6
	
	//USART1�˿�����
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3; //GPIOC6��GPIOC7
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
  USART_Init(USART2, &USART_InitStructure); //��ʼ������1
	
  USART_Cmd(USART2, ENABLE);  //ʹ�ܴ���1 
	
	USART_ClearFlag(USART2, USART_FLAG_TC);
	
#if 1	

	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//��������ж�
	USART_ITConfig(USART2, USART_IT_IDLE, ENABLE);//��������ж�

	//Usart1 NVIC ����
  NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;//����1�ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3;//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =3;		//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ�����

#endif
	
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



/**
  * @brief  ����һ�жϷ�����������һ���ֽڵ����ݣ�������־λ��1
	* 			�ı����ݰ�	���� ����		��ʼ��־λ @    ������־λ * &
	*       hex���ݰ�		����  8 		��ʼ��־λ 0x6C ������־λ ���ݺ�
  * @param  None
  * @retval None
  */
void USART6_IRQHandler(void){
	static uint8_t pRXpacket = 0;
//	printf("���봮���ж�6\r\n");
	
	if(USART_GetITStatus(USART6, USART_IT_RXNE) == SET){
		USART_ClearITPendingBit(USART6, USART_IT_RXNE);
		usart_RXdata = USART_ReceiveData(USART6);

		usart6_RXpacket[pRXpacket++] = USART6->DR;//�ȶ�SR���ٶ�DR ���־;
			
	
	}
	
	if(USART_GetITStatus(USART6, USART_IT_IDLE) == SET) 
	{
		USART_ClearITPendingBit(USART6, USART_IT_IDLE);
		pRXpacket = USART6->SR;//�Ĵ���д�����ַ��շ�
		pRXpacket = USART6->DR;//�ȶ�SR���ٶ�DR ���־
		pRXpacket = 0;
		usart6_RXflag = 1;
	}
}





/**
  * @brief  ��ȡusart_RXflag��ֵ
  * @param  None
  * @retval usart_RXflag ����1���ձ�־λ 
  */
uint8_t usart_get_RXflag(void){
	if(usart_RXflag == 1){

		return 2;
	}
	return 0;
}

/**
  * @brief  ��ȡusart6_RXflag��ֵ
  * @param  None
  * @retval usart_RXflag ����1���ձ�־λ 
  */
uint8_t usart6_get_RXflag(void){
	if(usart6_RXflag == 1){
		usart6_RXflag = 0;
		if(usart6_RXpacket[7] == ((usart6_RXpacket[1] + usart6_RXpacket[2] + usart6_RXpacket[3] + usart6_RXpacket[4] + usart6_RXpacket[5] + usart6_RXpacket[6]) & 0xff)){
			usart_RXflag = 0;
			return 1;
		}
		return 0;
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


void USART2_IRQHandler(void){
	uint8_t data;
	if(USART_GetITStatus(USART2, USART_IT_RXNE) == SET){
		USART_ClearITPendingBit(USART2, USART_IT_RXNE);
//		usart_RXdata = USART_ReceiveData(USART3);
		data = USART2->DR;
		USART1->DR = data;
		if(data == '\0'){
			data = '?';
		}
		esp8266.RX_data[esp8266.RX_count++] = data;
		
		//printf("%d", esp8266.RX_count);
	}
//�����ж�  1֡���ݽ������ �����
	if(USART_GetITStatus(USART2, USART_IT_IDLE) == SET) 
	{
		//USART_ClearITPendingBit(USART3, USART_IT_IDLE);
		esp8266.RX_flag = 1;
		data = USART2->SR;//�Ĵ���д�����ַ��շ�
		data = USART2->DR;//�ȶ�SR���ٶ�DR ���־
		//printf("�ж���ֵ��%d\r\n", esp8266.RX_flag);
		

	}
}


#pragma import(__use_no_semihosting_swi) //ȡ��������״̬

struct __FILE { int handle; /* Add whatever you need here */ };
FILE __stdout;

//int fputc(int ch, FILE *f) 
//{
//	while((USART1->SR &(0X01<<7))==0);  //�ȴ�֮ǰ�����ݷ������
//		USART1->DR=ch;
//  return (ch);
//}

int ferror(FILE *f) {
  /* Your implementation of ferror */
  return EOF;
}


void _ttywrch(int ch) {
  while((USART1->SR &(0X01<<7))==0);
		USART1->DR=ch;
}


void _sys_exit(int return_code) {
label:  goto label;  /* endless loop */
}












