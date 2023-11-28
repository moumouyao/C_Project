#include "usart.h"

uint8_t usart_RXdata;		//存放接收数据寄存器的值
uint8_t usart_RXflag;		//接收数据标志位
uint8_t usart_TXpacket[LEN];	//hex数据包发送数组
uint8_t usart_RXpacket[LEN] = {0};	//hex数据包接收数组
char usart_RX_str_packet[STRLEN] = {0};	//文本数据包接收数组

/**
  * @brief  printf输出重定向
  * @param  None
  * @retval None
  */
int fputc(int ch, FILE *f){
	usart_sendbit(USART1, ch);
	return ch;
}

/**
  * @brief  封装sprintf，任何串口都可以用
  * @param  char *format 需要打印的字符串
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
  * @brief  配置串口1 PA9 发送复用推挽    PA10 接收浮空模式  
  * @param  None
  * @retval None
  */
void usart_init(void){
	//初始化GPIO口
	//PA9复用推挽
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
  	GPIO_InitTypeDef GPIO_InitStructure;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  	GPIO_Init(GPIOA, &GPIO_InitStructure);
	//PA10浮空
	  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	  GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	//串口初始化
	//开启串口时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	//初始化串口
	USART_InitTypeDef USART_InitStruct = {0};
	USART_InitStruct.USART_BaudRate = 115200;	//设置波特率
	USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	//硬件流控制失能
	USART_InitStruct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//选择串口发送模式和接收模式
	USART_InitStruct.USART_Parity = USART_Parity_No;	//不需要校验
	USART_InitStruct.USART_StopBits = USART_StopBits_1;	//一位停止位
	USART_InitStruct.USART_WordLength = USART_WordLength_8b;	//字长选择8位
	USART_Init(USART1, &USART_InitStruct);
	
	//开启串口中断
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	
		//初始化NVIC
		//NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
  	NVIC_InitTypeDef NVIC_InitStruct;
  	NVIC_InitStruct.NVIC_IRQChannel = USART1_IRQn;
  	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
  	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 4;
  	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
  	NVIC_Init(&NVIC_InitStruct);
	
	//使能串口
	USART_Cmd(USART1, ENABLE);
}



/**
  * @brief  配置串口3 PB10 发送复用推挽    PB11 接收浮空模式  
	*						用于ESP8266通信
  * @param  None
  * @retval None
  */
void usart3_init(void){
	//初始化GPIO口
	//PB10复用推挽
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
  	GPIO_InitTypeDef GPIO_InitStructure;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  	GPIO_Init(GPIOB, &GPIO_InitStructure);
	//PB11浮空
	  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	  GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	//串口初始化
	//开启串口时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
	//初始化串口
	USART_InitTypeDef USART_InitStruct = {0};
	USART_InitStruct.USART_BaudRate = 115200;	//设置波特率
	USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	//硬件流控制失能
	USART_InitStruct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//选择串口发送模式和接收模式
	USART_InitStruct.USART_Parity = USART_Parity_No;	//不需要校验
	USART_InitStruct.USART_StopBits = USART_StopBits_1;	//一位停止位
	USART_InitStruct.USART_WordLength = USART_WordLength_8b;	//字长选择8位
	USART_Init(USART3, &USART_InitStruct);
	
	//开启串口中断
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);	//接收中断
	USART_ITConfig(USART3, USART_IT_IDLE, ENABLE);	//空闲中断
	
		//初始化NVIC
		//NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
  	NVIC_InitTypeDef NVIC_InitStruct;
  	NVIC_InitStruct.NVIC_IRQChannel = USART3_IRQn;
  	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
  	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 4;
  	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
  	NVIC_Init(&NVIC_InitStruct);
	
	//使能串口
	USART_Cmd(USART3, ENABLE);
}


/**
  * @brief  发送单个字节
* @param  uint8_t data	需要发送的字节
  * @retval None
  */
void usart_sendbit(USART_TypeDef* USARTx, uint8_t data){
	while(USART_GetFlagStatus(USARTx, USART_FLAG_TC) != 1); //等待上一次数据发送完成
	USART_SendData(USARTx, data);	//发送数据
}

/**
	* @brief  发送hex数据包，固定字节长度
	* 					数据包头 0xFF			数据包尾 oxFE
  * @param  None
  * @retval None
  */
void usart_send_hex_packet(USART_TypeDef* USARTx){
	usart_sendbit(USARTx, 0xFF);
	usart_send_array(USARTx, usart_TXpacket, LEN);
	usart_sendbit(USARTx, 0xFE);
}

/**
	* @brief  发送一个数组
	* @param  uint8_t *array 需要发送的数组
	* @param	uint16_t len	发送的数组长度
  * @retval None
  */
void usart_send_array(USART_TypeDef* USARTx, uint8_t *array, uint16_t len){
	uint16_t i = 0;
	for(i = 0; i < len; i++){
		usart_sendbit(USARTx, array[i]);
	}
}

/**
	* @brief  发送一个字符串
	* @param  char *str 需要发送的字符串
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
  * @brief  获取x的y次方
  * @param  uint32_t x 底数
	* @param  uint32_t y 次方
  * @retval result 运算结果
  */
uint32_t usart_pow(uint32_t x, uint32_t y){
	uint32_t result = 1;
	while(y--){
		result *= x;
	}
	return result;
}

/**
	* @brief  发送一串数字
	* @param  uint32_t num 需要发送的数字
	* @param  uint8_t len 需要发送的数字长度
  * @retval None
  */
void usart_send_num(USART_TypeDef* USARTx, uint32_t num, uint8_t len){
	uint8_t i = 0;
	for(i = 0; i < len; i ++){
		usart_sendbit(USARTx, num / usart_pow(10, len - i - 1) % 10 + '0');
	}
}


/**
* @brief  发送多个字符
  * @param  None
  * @retval None
  */
void usaer_sendstr(USART_TypeDef* USARTx, uint8_t str[], uint8_t len){
	for(uint8_t i = 0; i < len; i++){
		usart_sendbit(USARTx, str[i]);
	}
}
/**
  * @brief  接收单个字节 阻塞接收
  * @param  None
  * @retval None
  */
uint8_t usart_recv(USART_TypeDef* USARTx){
	uint8_t data = 0;
	//判断接收缓冲区标志位
	while(USART_GetFlagStatus(USARTx, USART_FLAG_RXNE) != 1);	//接收到数据
	data = USART_ReceiveData(USARTx);
	return data;
}
/**
  * @brief  串口一中断服务函数，接收一个字节的数据，并将标志位置1
	* 			文本数据包	长度 不限		开始标志位 @    结束标志位 * &
	*       hex数据包		长度  4 		开始标志位 0xFF 结束标志位 0xFE
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
		//判断接收的数据包头
		if(RXState == 0){	
			if(usart_RXdata == 0XFF){	//接收到的使hex文件
				RXState = 1;	
				pRXpacket = 0;
			}
			else if(usart_RXdata == '@'){	//接收到的使文本文件
				RXState = 3;	
				pRXpacket = 0;
			}
		}
			else if(RXState == 1){	//接收hex文件的内容
				usart_RXpacket[pRXpacket] = usart_RXdata;
				pRXpacket++;
				if(pRXpacket >= 3){		//接收数据包长度位4的数据
					RXState = 2;
				}
			}
			else if(RXState == 3){	//接收文本文件的内容
				if(usart_RXdata == '*'){	//第一个结束标志位
					RXState = 4;
				}
				else{
					usart_RX_str_packet[pRXpacket] = usart_RXdata;	//接收文本
					pRXpacket++;
				}
			}	
			else if(RXState == 2){	//判断hex数据包结束标志位
				if(usart_RXdata == 0XFE){
					RXState = 0;
					usart_RXflag = 1;
				}
			}
			else if(RXState == 4){	//判断文本数据包结束标志位
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
//空闲中断  1帧数据接收完成 会进入
	if(USART_GetITStatus(USART3, USART_IT_IDLE) == SET) 
	{
		//USART_ClearITPendingBit(USART3, USART_IT_IDLE);
		esp8266.RX_flag = 1;
		data = USART3->SR;//寄存器写法单字符收发
		data = USART3->DR;//先读SR，再读DR 清标志
		//printf("中断数值：%d\r\n", esp8266.RX_flag);
		

	}
}



/**
  * @brief  获取usart_RXflag的值
  * @param  None
  * @retval usart_RXflag 串口1接收标志位 
  */
uint8_t usart_get_RXflag(void){
	if(usart_RXflag == 1){
		usart_RXflag = 0;
		return 1;
	}
	return 0;
}

/**
  * @brief  串口1接收到的数据
  * @param  None
  * @retval None
  */
uint8_t usart_get_RXdata(void){
	return usart_RXdata;
}
