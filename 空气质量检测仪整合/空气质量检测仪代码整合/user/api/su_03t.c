#include "su_03t.h"

uint8_t usart_su_RXdata;		//存放接收数据寄存器的值
uint8_t usart_su_RXflag;		//接收数据标志位
uint8_t usart_su_RXpacket[6] = {0};	//hex数据包接收数组
uint8_t usart_su_TXpacket[14] = {0};	//hex数据包接收数组

/**
  * @brief  配置串口 PC10 发送复用推挽    PC11 接收浮空模式  
  * @param  None
  * @retval None
  */
void su_o3t_init(void){
	//初始化GPIO口
	//PA9复用推挽
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
  	GPIO_InitTypeDef GPIO_InitStructure;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  	GPIO_Init(GPIOC, &GPIO_InitStructure);
	//PA10浮空
	  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	  GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	//串口初始化
	//开启串口时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4, ENABLE);
	//初始化串口
	USART_InitTypeDef USART_InitStruct = {0};
	USART_InitStruct.USART_BaudRate = 115200;	//设置波特率
	USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	//硬件流控制失能
	USART_InitStruct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//选择串口发送模式和接收模式
	USART_InitStruct.USART_Parity = USART_Parity_No;	//不需要校验
	USART_InitStruct.USART_StopBits = USART_StopBits_1;	//一位停止位
	USART_InitStruct.USART_WordLength = USART_WordLength_8b;	//字长选择8位
	USART_Init(UART4, &USART_InitStruct);
	
	//开启串口中断
	USART_ITConfig(UART4, USART_IT_RXNE, ENABLE);
	
		//初始化NVIC
		//NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
  	NVIC_InitTypeDef NVIC_InitStruct;
  	NVIC_InitStruct.NVIC_IRQChannel = UART4_IRQn;
  	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
  	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 4;
  	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
  	NVIC_Init(&NVIC_InitStruct);
	
	//使能串口
	USART_Cmd(UART4, ENABLE);
}

/**
  * @brief  串口一中断服务函数，接收一个字节的数据，并将标志位置1
	*       hex数据包		长度  6 		开始标志位 0xAA 
  * @param  None
  * @retval None
  */
void UART4_IRQHandler(void){
	static uint8_t RX_su_State = 0;
	static uint8_t su_pRXpacket = 0;
	//usart_init();
	if(USART_GetITStatus(UART4, USART_IT_RXNE) == SET){
		USART_ClearITPendingBit(UART4, USART_IT_RXNE);
			//usart_send_string("y");

		usart_su_RXdata = USART_ReceiveData(UART4);
		//判断接收的数据包头
		if(RX_su_State == 0){	
			if(usart_su_RXdata == 0XAA){	//接收到的使hex文件
				usart_su_RXpacket[0] = usart_su_RXdata;
				RX_su_State = 1;	
				su_pRXpacket = 1;
			}
		}
			else if(RX_su_State == 1){	//接收hex文件的内容
				usart_su_RXpacket[su_pRXpacket] = usart_su_RXdata;
				su_pRXpacket++;
				if(su_pRXpacket >= 5){		//接收数据包长度位4的数据
					RX_su_State = 2;
				}
			}
			else if(RX_su_State == 2){	//判断hex数据包结束标志位
				if(usart_su_RXdata == 0XAA){
					usart_send_array(USART1, usart_su_RXpacket,6);
					usart_su_RXpacket[su_pRXpacket] = usart_su_RXdata;
					RX_su_State = 0;
					usart_su_RXflag = 1;
				}
				else{
					RX_su_State = 0;
					usart_su_RXflag = 0;
					usart_send_array(USART1, usart_su_RXpacket,6);
					usart_su_RXpacket[su_pRXpacket] = usart_su_RXdata;
					//usart_send_string("$");
				}
			}
	}
}

/**
  * @brief  获取usart_su_flag的值
  * @param  None
  * @retval usart_su_RXflag 串口2接收标志位 
  */
uint8_t usart_get_su_RXflag(void){
	if(usart_su_RXflag == 1){
		usart_su_RXflag = 0;
		return 1;
	}
	return 0;
}
/**
  * @brief  处理su_03t的数据
  * @param  None
  * @retval None
  */
void vioce_analysis(void){
	//usart_send_array(usart_su_RXpacket, 5);

	time_t rawtime;
	struct tm *info = NULL;
	//info = localtime(&rawtime);
	if(usart_get_su_RXflag() == 1){
		//char str[5] = {0};
		switch(usart_su_RXpacket[3]){
			case 0x01: //温度
				usart_su_TXpacket[0] = 0XAA;
				usart_su_TXpacket[1] = 0X55;
				usart_su_TXpacket[3] = dht_data.tmp;
				usart_su_TXpacket[4] = dht_data.tmp_flo;
				usart_su_TXpacket[5] = 0X55;
				usart_su_TXpacket[6] = 0XAA;
				if(dht_data.tmp_flag == 0){	//温度为正
					usart_su_TXpacket[2] = 0x02;
				}
				else{
					usart_su_TXpacket[2] = 0x09;
				}
				usart_send2su_array(usart_su_TXpacket, 6); 
				break;
			case 0x02://湿度
				usart_su_TXpacket[0] = 0XAA;
				usart_su_TXpacket[1] = 0X55;
				usart_su_TXpacket[2] = 0X03;
				usart_su_TXpacket[3] = dht_data.hum;
				usart_su_TXpacket[4] = 0X55;
				usart_su_TXpacket[5] = 0XAA;
				usart_send2su_array(usart_su_TXpacket, 6);
				break;
			case 0x03://空气成分
				
				usart_su_TXpacket[0] = 0XAA;
				usart_su_TXpacket[1] = 0X55;
				usart_su_TXpacket[2] = 0X01;
				usart_su_TXpacket[6] = 0X55;
				usart_su_TXpacket[7] = 0XAA;
				//sprintf(str,"0x%x",(int)(kqm_data.VOC * 100));
				//usart_su_TXpacket[4] = atoi(str);
				usart_su_TXpacket[3] = (uint8_t)(kqm_data.VOC * 100);
			//	printf("%d", usart_su_TXpacket[3]);
				usart_su_TXpacket[4] = (uint8_t)(kqm_data.CHO * 100);
				//printf("%d", usart_su_TXpacket[4]);
				usart_su_TXpacket[5] = kqm_data.CO2;
			//	printf("%d", usart_su_TXpacket[5]);
				usart_send2su_array(usart_su_TXpacket, 8);
			//	usart_send_array(usart_su_TXpacket,8);
				break;
			case 0x04://开灯
				led_enable(LED1);
				break;
			case 0x05://关灯
				led_disable(LED1);
				break;
			case 0x06://甲烷
				usart_su_TXpacket[0] = 0XAA;
				usart_su_TXpacket[1] = 0X55;
				usart_su_TXpacket[3] = (uint8_t)(kqm_data.CHO * 100);
				usart_su_TXpacket[4] = 0X55;
				usart_su_TXpacket[5] = 0XAA;
				if(kqm_data.CHO <0.03){
					usart_su_TXpacket[2] = 0x04;
				}
				else if(kqm_data.CHO >=0.03 && kqm_data.CHO <0.1){
					usart_su_TXpacket[2] = 0x05;
				}
				else{
					usart_su_TXpacket[2] = 0x06;
				}
				usart_send2su_array(usart_su_TXpacket, 6);
				break;
			case 0x07://开启蜂鸣器
				buzzer_enable();
				break;
			case 0x08://关闭蜂鸣器
				buzzer_disable();
				break;
			case 0x9://二氧化碳
				usart_su_TXpacket[0] = 0XAA;
				usart_su_TXpacket[1] = 0X55;
				usart_su_TXpacket[2] = 0x07;
				usart_su_TXpacket[3] = kqm_data.CO2;
				usart_su_TXpacket[4] = 0X55;
				usart_su_TXpacket[5] = 0XAA;
				usart_send2su_array(usart_su_TXpacket, 6);
				break;
			case 0x10://时间
				rawtime = RTC_GetCounter();
				info = localtime(&rawtime);
				
				//info=gmtime(&rawtime);
				usart_su_TXpacket[0] = 0XAA;
				usart_su_TXpacket[1] = 0X55;
				usart_su_TXpacket[2] = 0x08;
				usart_su_TXpacket[3] = (info->tm_year + 1900) / 1000;
				usart_su_TXpacket[4] = ((info->tm_year + 1900) / 100) % 10;
				usart_su_TXpacket[5] = ((info->tm_year + 1900) / 10) % 100;
				usart_su_TXpacket[6] = (info->tm_year + 1900) % 10;
				usart_su_TXpacket[7] = info->tm_mon + 1;
				usart_su_TXpacket[8] = info->tm_mday;
				usart_su_TXpacket[9] = info->tm_hour;
				usart_su_TXpacket[10] = info->tm_min;
				usart_su_TXpacket[11] = info->tm_sec;
				usart_su_TXpacket[12] = 0X55;
				usart_su_TXpacket[13] = 0XAA;
				usart_send_array(USART1, usart_su_TXpacket, 14);
				printf("%d\t%d\t%d\t%d\t", info->tm_year, info->tm_mon, info->tm_mday, info->tm_hour);
				usart_send2su_array(usart_su_TXpacket, 14);
				break;
		}
	}
}

/**
	* @brief  发送一个数组
	* @param  uint8_t *array 需要发送的数组
	* @param	uint16_t len	发送的数组长度
  * @retval None
  */
void usart_send2su_array(uint8_t *array, uint16_t len){
	uint16_t i = 0;
	for(i = 0; i < len; i++){
		usart_send2su_bit(array[i]);
	}
}

/**
  * @brief  发送单个字节
* @param  uint8_t data	需要发送的字节
  * @retval None
  */
void usart_send2su_bit(uint8_t data){
	while(USART_GetFlagStatus(UART4, USART_FLAG_TC) != 1); //等待上一次数据发送完成
	USART_SendData(UART4, data);	//发送数据
}
