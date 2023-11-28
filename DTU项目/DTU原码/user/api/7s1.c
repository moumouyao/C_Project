#include "7s1.h"

esp esp8266;	//存放8266接收信息


/**
  * @brief  检测服务器返回值
  * @param  uint16_t time	设置函数运行最大时间
  * @retval 0 连接成功		1 连接失败
  */
uint8_t check_server(char * str, uint16_t time){
	while(time--){
		if(esp8266.RX_flag == 1){
			printf("连接返回值 %s\r\n", esp8266.RX_data);
			esp8266.RX_flag = 0;
			if((esp8266.RX_data[0] == 0x20) && (esp8266.RX_data[1] == 0x02)){
				esp8266.RX_count = 0;
				memset(esp8266.RX_data, 0, sizeof(esp8266.RX_data));
				return 0;
			}
			else if(strstr((char *)esp8266.RX_data, str) != NULL){
				esp8266.RX_count = 0;
				memset(esp8266.RX_data, 0, sizeof(esp8266.RX_data));
				return 0;
			}
		}
		Delay_ms(1);
	}
	return 1;
}


/**
  * @brief  连接服务器函数
  * @param  None
  * @retval None
  */
	void mqtt_connect_server(void){
		
		//配置连接报文
		MQTTPacket_connectData options = MQTTPacket_connectData_initializer;	//通用参数设置默认值
//		options.cleansession = ;
		options.clientID.cstring = my_client_id;
//		options.keepAliveInterval = ;
//		options.MQTTVersion = ;
		options.keepAliveInterval = 60;
		options.cleansession = 1;
		options.password.cstring = my_password;
//		options.struct_id = ;
//		options.struct_version = ;
		options.username.cstring = my_username;
//		options.will = ;
//		options.willFlag = ;
		esp8266.tx_count = 0;
		esp8266.tx_count = MQTTSerialize_connect(esp8266.tx_buf, 1024, &options);
		
		esp8266.RX_count = 0;
		memset(esp8266.RX_data, 0, 2048);
		//发送连接报文
//		printf("报文长度 %d\r\n", esp8266.tx_count);
//		printf("拼接好的报文  %s\r\n", esp8266.tx_buf);
		usaer_sendstr(USART1, (unsigned char *)esp8266.tx_buf, esp8266.tx_count);
//		usaer_sendstr(UART4, (unsigned char *)esp8266.tx_buf	, esp8266.tx_count);
		cat_data_send(SOCKETB_ID, esp8266.tx_buf, esp8266.tx_count);
		
	}
	
	
	
/**
  * @brief  订阅服务器的属性设置
  * @param  None
  * @retval None
  */
void subscription_service(void){
	MQTTString topicFilters[2] = {0};
	int requestedQoSs[2] = {0};
	requestedQoSs[0] = 0;
	topicFilters[0].cstring = property_subscribe_cstring;
	topicFilters[1].cstring = time_subscribe_cstring;
	esp8266.tx_count = MQTTSerialize_subscribe(esp8266.tx_buf, 1024, 0, 0, 2, topicFilters, requestedQoSs);
	memset(esp8266.RX_data, 0, sizeof(esp8266.RX_data));
	
	//发送连接报文
	usaer_sendstr(USART1, (unsigned char *)esp8266.tx_buf, esp8266.tx_count);
	
	usaer_sendstr(UART4, (unsigned char *)esp8266.tx_buf	, esp8266.tx_count);
	//return check_server("?", 1000);
}


/**
  * @brief  配置esp8266
  * @param  char *AT_cmd			AT指令
  * @param	char *AT_retval		输入参数，指令校验 
  * @param	uint16_t AT_time	操作时间
  * @retval 成功 0		失败 1	
  */
uint8_t SendCmdAndCheckRet(char *AT_cmd, char *AT_retval, uint16_t AT_time){
	//清零
	esp8266.RX_count = 0;
	esp8266.RX_flag = 0;
	memset(esp8266.RX_data, 0, sizeof(esp8266.RX_data));
	
	//发送
	usart_send_string(UART4, AT_cmd);
	
	//判断接收值
	while(AT_time--){
		if(esp8266.RX_flag == 1){
//			printf("接收到的信息 %s\r\n", esp8266.RX_data);
			esp8266.RX_flag = 0;
			if(strstr((char *)esp8266.RX_data, AT_retval) != NULL){
//				esp8266.RX_flag = 1;
				return 0;	//返回值正确，设置成功
			}
	}
		Delay_ms(1);
		
	}
	return 1;	//返回值不正确，设置失败
}




void Cat_Connect_Net(void){
	uint8_t socket_num = 0;
	//重启关闭穿透模式
	printf("重启设备\r\n");
	if(SendCmdAndCheckRet("AT+Z\r\n","WH-LTE-7S1",6000) != 0){
		printf("重启失败\r\n");
//		return;
	}
	printf("关闭透传模式\r\n");
	if(SendCmdAndCheckRet("+++","a",1000) != 0){
		printf("关闭透传模式失败\r\n");
		return;
	}
	printf("向设备发送一个 a\r\n");
	if(SendCmdAndCheckRet("a","+ok",1000) != 0){
		printf("发送 a 失败\r\n");
		return;
	}
	printf("开启透传模式\r\n");
	if(SendCmdAndCheckRet("AT+WKMOD=NET\r\n","OK",1000) != 0){
		printf("开启透传失败\r\n");
		return;
	}
	
#if  SOCKETA_EN 	//连接心知天气
	printf("使能 SOCKET A\r\n");
	if(SendCmdAndCheckRet("AT+SOCKAEN=ON\r\n","OK",1000) != 0){
		printf("SOCKET A 使能失败\r\n");
		return;
	}
	printf("连接心知天气\r\n");
	sprintf((char *)esp8266.tx_buf,"AT+SOCKA=TCP,%s,%d\r\n",SOCKETA_IP,SOCKETA_PORT);
	SendCmdAndCheckRet((char *)esp8266.tx_buf,"OK",1000);
	socket_num++;
	Delay_ms(100);
#endif

#if  SOCKETB_EN 	//连接阿里云
	printf("使能SOCKET B\r\n");
	memset(esp8266.tx_buf, 0, 1024);
	esp8266.tx_count = 0;
	if(SendCmdAndCheckRet("AT+SOCKBEN=ON\r\n","OK",1000) != 0){
		printf("SOCKET B 使能失败\r\n");
		return;
	}
	printf("连接阿里云\r\n");
	sprintf((char *)esp8266.tx_buf,"AT+SOCKB=TCP,%s,%d\r\n",SOCKETB_IP,SOCKETB_PORT);
	SendCmdAndCheckRet((char *)esp8266.tx_buf,"OK",1000);
	socket_num++;
	Delay_ms(100);
#endif

#if  SOCKETC_EN
	Delay_ms(100);
#endif
	
#if  SOCKETD_EN
	Delay_ms(100);
#endif

	if(socket_num > 1){
		//开启套接字模式
		printf("开启套接字模式\r\n");
		if(SendCmdAndCheckRet("AT+SDPEN=ON\r\n","OK",1000) != 0){
			printf("开启套接字模式失败\r\n");
			return;
		}
	}
	else{
		printf("不用开启套接字模式\r\n");
		if(SendCmdAndCheckRet("AT+SDPEN=OFF\r\n","OK",1000) != 0){
			printf("关闭套接字模式失败\r\n");
			return;
		}
	}
	
	printf("重启设备\r\n");
	if(SendCmdAndCheckRet("AT+S\r\n","WH",6000) != 0){
		printf("重启失败\r\n");
		return;
	}
}

//拼接并发送套接字文件
void cat_data_send(uint8_t socket_id, uint8_t *send_buf, uint16_t len){
	uint8_t socket_buf[1024] = {0};
//	printf("套接字拼接函数内\r\n");
//	printf("字符串\r\n");
//	usaer_sendstr(USART1, (unsigned char *)send_buf, len);
//	printf("另一个字符串\r\n");
//	usaer_sendstr(USART1, (unsigned char *)esp8266.tx_buf, esp8266.tx_count);
//	printf("长度 %d\r\n", len);
//	printf("传入参数打印完成\r\n");
	//拼接包头
	socket_buf[0] = 0x55;
	socket_buf[1] = 0xFD;
	socket_buf[2] = 0xAA;
	//数据长度,先高字节再低字节
	socket_buf[3] = (len + 2) & 0xff00;
	socket_buf[4] = (len + 2) & 0xff;
	//参数区
	socket_buf[5] = socket_id;
	socket_buf[6] = 0x00;
	
	for(uint16_t i = 7; i < len + 7; i++){
		socket_buf[i] = send_buf[i - 7];
		
	}
	socket_buf[7 + len] = socket_id + 	socket_buf[6];
	for(uint16_t i = 0; i < len; i++){
		socket_buf[7 + len] += send_buf[i];
	}
//	printf("\r\n发送套接字数据\r\n");
//	for(uint16_t i = 0; i < len + 8; i++){
//		printf("%x\t", socket_buf[i]);
//	}
//	printf("\r\n打印完成\r\n");
	memset(esp8266.RX_data, 0, 2048);
	esp8266.RX_count = 0;
	printf("通过串口四发送发送套接字数据\r\n");
	usaer_sendstr(UART4, (unsigned char *)socket_buf, len + 8);
	printf("发送完成\r\n");
}

//模块恢复数据解析
uint8_t cat_data_analysis(uint8_t socket_id, uint8_t *data){
	uint16_t recv_data_len = 0;
	if(esp8266.RX_flag != 1){
		printf("接收标志位未置1\r\n");
		return 0;
	}
	
	//分析是否为套接字数据
	esp8266.RX_flag = 0;
//	printf("接收到的数据 %s\r\n", esp8266.RX_data);
	if(esp8266.RX_data[0] != 0xAA || esp8266.RX_data[1] != 0xfd || esp8266.RX_data[2] != 0x55){
		printf("非套接字数据\r\n");
		printf("esp8266.RX_data = %s\r\n", esp8266.RX_data);
		memset(esp8266.RX_data, 0, 2048);
		esp8266.RX_count = 0;
		return 0;
	}
	
	//提取数据长度
	recv_data_len = esp8266.RX_data[3];
	recv_data_len = recv_data_len << 8;
	recv_data_len = recv_data_len | esp8266.RX_data[4];
//	printf("esp8266.RX_data[3] = %x\t\nesp8266.RX_data[4] = %x\r\n", esp8266.RX_data[3], esp8266.RX_data[4]);
//	recv_data_len = ((uint16_t)esp8266.RX_data[3] << 8) | esp8266.RX_data[4];
//	printf("接收到的数据总长度为 %d\r\n", recv_data_len);
	recv_data_len = recv_data_len - 2;
	printf("数据区数据长度 %d\r\n", recv_data_len);
	printf("esp8266.RX_data = %s\r\n", esp8266.RX_data);
//	for(uint16_t i = 0; i < recv_data_len + 8; i++){
//		printf("%x\t", esp8266.RX_data[i]);
//	}
	if(recv_data_len > 15000)
		return 0;
	
	//判断参数区
	switch(esp8266.RX_data[5]){
		case 0x61:
			printf("socket A 数据\r\n");
			socket_id = esp8266.RX_data[5];
			printf("socket id = %x\r\n", esp8266.RX_data[5]);
			for(uint16_t i = 0; i < recv_data_len; i++){
				data[i] = esp8266.RX_data[7 + i];
			}
			return 1;
		case 0x62:
			printf("socket B 数据\r\n");
			socket_id = esp8266.RX_data[5];
			printf("socket id = %x\r\n", esp8266.RX_data[5]);
			for(uint16_t i = 0; i < recv_data_len; i++){
				data[i] = esp8266.RX_data[7 + i];
			}
			return 1;
		case 0x63:
			printf("socket C 数据\r\n");
			socket_id = esp8266.RX_data[5];
			for(uint16_t i = 0; i < recv_data_len; i++){
				data[i] = esp8266.RX_data[7 + i];
			}
			return 1;
		case 0x64:
			printf("socket D 数据\r\n");
			socket_id = esp8266.RX_data[5];
			for(uint16_t i = 0; i < recv_data_len; i++){
				data[i] = esp8266.RX_data[7 + i];
			}
			return 1;
		default:
			if(esp8266.RX_data[6] != 0x80){
				printf("网络返回数据错误\r\n");
			}
			switch(esp8266.RX_data[6]){
				case 0x00:printf("包头错误");return 0;
				case 0x01:printf("数据长度错误");return 0;
				case 0x02:printf("参数错误");return 0;
				case 0x03:printf("校验位错误，请检查校验和结果");return 0;
				case 0x10:printf("发送失败（参数正确但是发送失败）");return 0;
				default: printf("error\r\n");
			}
			
			
	}
	memset(esp8266.RX_data, 0, 2048);
	esp8266.RX_count = 0;
}
	


/**
  * @brief  向服务器发送报文
  * @param  None
  * @retval 0 发送成功		1 发送失败
  */
void mqtt2server(char* cstr, char *payl, char * reply){
	
	MQTTString topicName = MQTTString_initializer;
	topicName.cstring = cstr;
	char* payload = payl;
	esp8266.tx_count = MQTTSerialize_publish(esp8266.tx_buf, 1024, 0, 0, 0, 0, topicName, (unsigned char *)payload, strlen(payload));
	esp8266.RX_count = 0;
//	memset(esp8266.RX_data, 0, 2048);
	printf("mqtt拼接函数内\r\n");
	usaer_sendstr(USART1, (unsigned char *)esp8266.tx_buf, esp8266.tx_count);
	printf("数据长度%d\r\n", esp8266.tx_count);
	//发送连接报文
	cat_data_send(SOCKETB_ID, esp8266.tx_buf, esp8266.tx_count);

}




/**
  * @brief  向服务器上传数据
  * @param  None
  * @retval None
  */
void send_data2server(void){
	char send2server_data[1024] = {0};
	char temp1[16];
	float temp2;

	sprintf(send2server_data, property_payload, modbus_data[0], modbus_data[1], modbus_data[2], modbus_data[3]);
	printf("\r\n上传数据函数内\r\n");
	printf("数据 %s\r\n", property_payload);
	mqtt2server(property_publish_cstring, send2server_data, "success"); 
								

}




//获取天气
void get_weather(void){
	uint8_t send_buf[] = "GET https://api.seniverse.com/v3/weather/now.json?key=StvgScKh2NewX_quB&location=zhengzhou&language=en&unit=c\r\n";
	uint16_t len = sizeof(send_buf)/sizeof(send_buf[0]);
	printf("天气数据长度 %d\r\n", len);
	cat_data_send(SOCKETA_ID, send_buf, len);
}

uint8_t aliyun_flag = 0;
//数据上传阿里云
void updata_aliyun(void){
	if(aliyun_flag == 0){
		printf("开始连接阿里云\r\n");
		mqtt_connect_server();
//		Delay_ms(5000);
		aliyun_flag++;
	}
	else{
		send_data2server();
	}
}


	

void Cat_Connect_Modify(void){
	uint8_t socket_num = 0;
	uint8_t sock_en[32] = {0}, sock_id[32] = {0};
//	char temp[128] = {0};
	//重启关闭穿透模式
	printf("重启设备\r\n");
	if(SendCmdAndCheckRet("AT+Z\r\n","WH-LTE-7S1",6000) != 0){
		printf("重启失败\r\n");
//		return;
	}
	printf("关闭透传模式\r\n");
	if(SendCmdAndCheckRet("+++","a",1000) != 0){
		printf("关闭透传模式失败\r\n");
		return;
	}
	printf("向设备发送一个 a\r\n");
	if(SendCmdAndCheckRet("a","+ok",1000) != 0){
		printf("发送 a 失败\r\n");
		return;
	}
	printf("开启透传模式\r\n");
	if(SendCmdAndCheckRet("AT+WKMOD=NET\r\n","OK",1000) != 0){
		printf("开启透传失败\r\n");
		return;
	}
	
	//mqtt
	switch(js_re_data.mqtt_data.socket){
	
		case 1:strcpy((char *)sock_en, "AT+SOCKAEN=ON\r\n");strcpy((char *)sock_id, "AT+SOCKA=TCP");break;
		case 2:strcpy((char *)sock_en, "AT+SOCKBEN=ON\r\n");strcpy((char *)sock_id, "AT+SOCKB=TCP");break;
		case 3:strcpy((char *)sock_en, "AT+SOCKCEN=ON\r\n");strcpy((char *)sock_id, "AT+SOCKC=TCP");break;
		case 4:strcpy((char *)sock_en, "AT+SOCKDEN=ON\r\n");strcpy((char *)sock_id, "AT+SOCKD=TCP");break;
		
	}
	

	printf("使能 SOCKET \r\n");
	if(SendCmdAndCheckRet((char *)sock_en,"OK",1000) != 0){
		printf("%s 使能失败\r\n", sock_en);
		return;
	}
	
	printf("mqtt port %d\r\n", js_re_data.mqtt_data.port);
	sprintf((char *)esp8266.tx_buf,"%s,%s,%d\r\n",sock_id,js_re_data.mqtt_data.IP,js_re_data.mqtt_data.port);
	SendCmdAndCheckRet((char *)esp8266.tx_buf,"OK",1000);
	socket_num++;
	Delay_ms(100);


	//mqtt
	switch(js_re_data.http_data.socket){
	
		case 1:strcpy((char *)sock_en, "AT+SOCKAEN=ON\r\n");strcpy((char *)sock_id, "AT+SOCKA=TCP");break;
		case 2:strcpy((char *)sock_en, "AT+SOCKBEN=ON\r\n");strcpy((char *)sock_id, "AT+SOCKB=TCP");break;
		case 3:strcpy((char *)sock_en, "AT+SOCKCEN=ON\r\n");strcpy((char *)sock_id, "AT+SOCKC=TCP");break;
		case 4:strcpy((char *)sock_en, "AT+SOCKDEN=ON\r\n");strcpy((char *)sock_id, "AT+SOCKD=TCP");break;
		
	}
	

	printf("使能 SOCKET \r\n");
	if(SendCmdAndCheckRet((char *)sock_en,"OK",1000) != 0){
		printf("%s 使能失败\r\n", sock_en);
		return;
	}
	
	sprintf((char *)esp8266.tx_buf,"%s,%s,%d\r\n",sock_id,js_re_data.http_data.IP,js_re_data.http_data.port);
	SendCmdAndCheckRet((char *)esp8266.tx_buf,"OK",1000);
	socket_num++;
	Delay_ms(100);
	
	


#if  SOCKETC_EN
	Delay_ms(100);
#endif
	
#if  SOCKETD_EN
	Delay_ms(100);
#endif

	if(socket_num > 1){
		//开启套接字模式
		printf("开启套接字模式\r\n");
		if(SendCmdAndCheckRet("AT+SDPEN=ON\r\n","OK",1000) != 0){
			printf("开启套接字模式失败\r\n");
			return;
		}
	}
	else{
		printf("不用开启套接字模式\r\n");
		if(SendCmdAndCheckRet("AT+SDPEN=OFF\r\n","OK",1000) != 0){
			printf("关闭套接字模式失败\r\n");
			return;
		}
	}
	
	printf("重启设备\r\n");
	if(SendCmdAndCheckRet("AT+S\r\n","WH",6000) != 0){
		printf("重启失败\r\n");
		return;
	}
}















	
	
	
	
	
	
	
	