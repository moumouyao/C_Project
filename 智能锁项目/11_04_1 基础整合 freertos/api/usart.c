#include "usart.h"

uint8_t usart_RXdata;		//存放接收数据寄存器的值
uint8_t usart_RXflag;		//接收数据标志位
uint8_t usart6_RXflag;		//接收数据标志位
uint8_t usart_TXpacket[LEN];	//hex数据包发送数组
uint8_t usart_RXpacket[LEN] = {0};	//hex数据包接收数组

uint8_t usart6_TXpacket[USART6_SEND_LEN];	//hex数据包发送数组
uint8_t usart6_RXpacket[USART6_RECV_LEN] = {0};	//hex数据包接收数组
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
  * @brief  配置串口 PA9 发送复用推挽    PA10 接收浮空模式  
  * @param  None
  * @retval None
  */
//void usart_init(void){
//	//初始化GPIO口
//		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
//	//开启串口时钟
//		RCC_APB2PeriphResetCmd(RCC_APB2Periph_USART1, ENABLE);
//		GPIO_PinAFConfig(GPIOA,GPIO_PinSource9,GPIO_AF_USART1); //GPIOA9复用为USART1
//		GPIO_PinAFConfig(GPIOA,GPIO_PinSource10,GPIO_AF_USART1); //GPIOA10复用为USART1
////	//PA9复用推挽
//  	GPIO_InitTypeDef GPIO_InitStructure;
////  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
////		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
////  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
////		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
////  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
////  	GPIO_Init(GPIOA, &GPIO_InitStructure);
////	//PA10浮空
////	  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
////		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
////		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
////  	GPIO_InitStructure.GPIO_Pin = GPIO_Speed_100MHz;
////	  GPIO_Init(GPIOA, &GPIO_InitStructure);
//	
//GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10; //GPIOA9 与 GPIOA10
//GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用功能
//GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; //速度 50MHz
//GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //推挽复用输出
//GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //上拉
//GPIO_Init(GPIOA,&GPIO_InitStructure); //初始化 PA9，PA10

//		

//	
//	//串口初始化

//	//初始化串口
//	USART_InitTypeDef USART_InitStruct = {0};
//	USART_InitStruct.USART_BaudRate = 115200;	//设置波特率
//	USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	//硬件流控制失能
//	USART_InitStruct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//选择串口发送模式和接收模式
//	USART_InitStruct.USART_Parity = USART_Parity_No;	//不需要校验
//	USART_InitStruct.USART_StopBits = USART_StopBits_1;	//一位停止位
//	USART_InitStruct.USART_WordLength = USART_WordLength_8b;	//字长选择8位
//	
//	USART_Init(USART1, &USART_InitStruct);
//		USART_ClearFlag(USART1, USART_FLAG_TC);
//	//开启串口中断
//	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
//	
//		//初始化NVIC
//		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
//  	NVIC_InitTypeDef NVIC_InitStruct;
//  	NVIC_InitStruct.NVIC_IRQChannel = USART1_IRQn;
//  	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
//  	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1;
//  	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1;
//  	NVIC_Init(&NVIC_InitStruct);
//	
//	//使能串口
//	USART_Cmd(USART1, ENABLE);
//}

void USART1_init(u32 bound)
	{
  float USARTDIV;uint16_t DIV_Man;uint16_t DIV_Fra;
	
	//GPIO初始化:
	//PA9---复用成USART1_TX模式
	//PA10--复用成USART1_RX模式
	RCC->AHB1ENR |=0x01<<0;//端口A时钟使能
	
	GPIOA->MODER &=~(0x0F<<18);//清零
	GPIOA->MODER |=(0x0a<<18);//PA9和PA10复用功能
	GPIOA->OTYPER &=~(0x01<<9);//PA9复用功能推挽输出
	GPIOA->OSPEEDR &=~(0x03<<18);//PA9低速2Mhz
	GPIOA->PUPDR &=~(0x0F<<18);//PA9和PA10浮空
	
	GPIOA->AFR[1] &=~(0xFF<<4);
	GPIOA->AFR[1] |=(0x77<<4);//PA9---复用成USART1_TX
														//PA10--复用成USART1_RX
	
	//配置USART1
	RCC->APB2ENR |=0x01<<4;//模块时钟使能--84Mhz
	USART1->CR1=0;//寄存器清零
	USART1->CR2 &=~(0x03<<12);
	/*
	16 倍过采样
	1 起始位， 8 数据位， 1 停止位
	禁止奇偶校验控制
	*/
	
	//波特率设置
	USARTDIV=84000000.0/(16*bound);
	DIV_Man=USARTDIV;
	DIV_Fra=(USARTDIV-DIV_Man)*16+0.5;
	USART1->BRR=DIV_Man<<4  | DIV_Fra;
	
		//USART1->CR1 |=0X01<<4;  //空闲中断使能
	USART1->CR1 |=0X01<<5; //接收缓冲区非空中断使能
	
	//nvic中设置uart1的优先级、使能uart1中断
	NVIC_SetPriority(USART1_IRQn,NVIC_EncodePriority(7-2,3,3));
	NVIC_EnableIRQ(USART1_IRQn);
	
	
	USART1->CR1 |=0x01<<3;//使能发送器
	USART1->CR1 |=0x01<<2;//使能接收器
	USART1->CR1 |=0x01<<13;//使能串口1
}



void USART6_init(u32 bound)
	{
   //GPIO端口设置
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
//	RXD1_P=USART_RX_BUF;//指针指向串口接收数据的缓冲区	
		
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC,ENABLE); //使能GPIOA时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART6,ENABLE);//使能USART1时钟
 
	//串口1对应引脚复用映射
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource6,GPIO_AF_USART6); //GPIOC6复用为USART6
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource7,GPIO_AF_USART6); //GPIOAC7复用为USART6
	
	//USART1端口配置
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7; //GPIOC6与GPIOC7
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//速度50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //上拉
	GPIO_Init(GPIOC,&GPIO_InitStructure); //初始化PA9，PA10

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
	
#if 1	

	USART_ITConfig(USART6, USART_IT_RXNE, ENABLE);//开启相关中断
	USART_ITConfig(USART6, USART_IT_IDLE, ENABLE);//开启相关中断

	//Usart1 NVIC 配置
  NVIC_InitStructure.NVIC_IRQChannel = USART6_IRQn;//串口1中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =3;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器、

#endif
	
}


void USART2_init(u32 bound)
	{
   //GPIO端口设置
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
//	RXD1_P=USART_RX_BUF;//指针指向串口接收数据的缓冲区	
		
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE); //使能GPIOA时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);//使能USART2时钟
 
	//串口1对应引脚复用映射
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource2,GPIO_AF_USART2); //GPIOC6复用为USART6
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource3,GPIO_AF_USART2); //GPIOAC7复用为USART6
	
	//USART1端口配置
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3; //GPIOC6与GPIOC7
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
  USART_Init(USART2, &USART_InitStructure); //初始化串口1
	
  USART_Cmd(USART2, ENABLE);  //使能串口1 
	
	USART_ClearFlag(USART2, USART_FLAG_TC);
	
#if 1	

	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//开启相关中断
	USART_ITConfig(USART2, USART_IT_IDLE, ENABLE);//开启相关中断

	//Usart1 NVIC 配置
  NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;//串口1中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =3;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器、

#endif
	
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



/**
  * @brief  串口一中断服务函数，接收一个字节的数据，并将标志位置1
	* 			文本数据包	长度 不限		开始标志位 @    结束标志位 * &
	*       hex数据包		长度  8 		开始标志位 0x6C 结束标志位 数据和
  * @param  None
  * @retval None
  */
void USART6_IRQHandler(void){
	static uint8_t pRXpacket = 0;
//	printf("进入串口中断6\r\n");
	
	if(USART_GetITStatus(USART6, USART_IT_RXNE) == SET){
		USART_ClearITPendingBit(USART6, USART_IT_RXNE);
		usart_RXdata = USART_ReceiveData(USART6);

		usart6_RXpacket[pRXpacket++] = USART6->DR;//先读SR，再读DR 清标志;
			
	
	}
	
	if(USART_GetITStatus(USART6, USART_IT_IDLE) == SET) 
	{
		USART_ClearITPendingBit(USART6, USART_IT_IDLE);
		pRXpacket = USART6->SR;//寄存器写法单字符收发
		pRXpacket = USART6->DR;//先读SR，再读DR 清标志
		pRXpacket = 0;
		usart6_RXflag = 1;
	}
}





/**
  * @brief  获取usart_RXflag的值
  * @param  None
  * @retval usart_RXflag 串口1接收标志位 
  */
uint8_t usart_get_RXflag(void){
	if(usart_RXflag == 1){

		return 2;
	}
	return 0;
}

/**
  * @brief  获取usart6_RXflag的值
  * @param  None
  * @retval usart_RXflag 串口1接收标志位 
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
  * @brief  串口1接收到的数据
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
//空闲中断  1帧数据接收完成 会进入
	if(USART_GetITStatus(USART2, USART_IT_IDLE) == SET) 
	{
		//USART_ClearITPendingBit(USART3, USART_IT_IDLE);
		esp8266.RX_flag = 1;
		data = USART2->SR;//寄存器写法单字符收发
		data = USART2->DR;//先读SR，再读DR 清标志
		//printf("中断数值：%d\r\n", esp8266.RX_flag);
		

	}
}


#pragma import(__use_no_semihosting_swi) //取消半主机状态

struct __FILE { int handle; /* Add whatever you need here */ };
FILE __stdout;

//int fputc(int ch, FILE *f) 
//{
//	while((USART1->SR &(0X01<<7))==0);  //等待之前的数据发送完毕
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












