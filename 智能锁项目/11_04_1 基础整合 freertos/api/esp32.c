#include "esp32.h"


	
 esp esp8266;	//存放8266接收信息


	uint8_t wifi_name[32] = {0};
	uint8_t wifi_passwd[32] = {0};

	uint32_t write_wifi_addr = 1913720;
	uint32_t write_passwd_addr = 1913820;

	uint32_t write_len_addr = 1913920;
	uint8_t read_len[3] = {0};


/**
	* @brief  使用串口3，发送一个字符e
	* @param  char *str 需要发送的字符串
  * @retval None
  */
void usart2_send_string(char *str, uint16_t count){
	uint16_t i = 0;
	printf("进入发送函数\r\n");
	for(i = 0; i < count; i ++){
		usart_sendbit(USART2, str[i]);
//		printf("%c", str[i]);
//		printf("%d\r\n", i);
	}
	printf("发送完成\r\n");
}

/**
  * @brief  检测服务器返回值
  * @param  uint16_t time	设置函数运行最大时间
  * @retval 0 连接成功		1 连接失败
  */
uint8_t check_server(char * str, uint16_t time){
	while(time--){
		if(esp8266.RX_flag == 1){
			esp8266.RX_flag = 0;
//			if((esp8266.RX_data[0] == 0x20) && (esp8266.RX_data[1] == 0x02)){
////				esp8266.RX_count = 0;
////				memset(esp8266.RX_data, 0, sizeof(esp8266.RX_data));
//				return 0;
//			}
//			else 
			if(strstr((char *)esp8266.RX_data, str) != NULL){
//				esp8266.RX_count = 0;
//				memset(esp8266.RX_data, 0, sizeof(esp8266.RX_data));
				return 0;
			}
		}
//		printf("正在检查返回值\r\n");
		Delay_ms(1);
	}
	return 1;
}

/**
  * @brief  连接服务器函数
  * @param  None
  * @retval None
  */
	uint8_t mqtt_connect_server(void){
//		printf("进入发送函数\r\n");
		//配置连接报文
		MQTTPacket_connectData options = MQTTPacket_connectData_initializer;	//通用参数设置默认值
		options.clientID.cstring = my_client_id;

		options.password.cstring = my_password;

		options.username.cstring = my_username;

		esp8266.tx_count = MQTTSerialize_connect(esp8266.tx_buf, 1024, &options);
		
		esp8266.RX_count = 0;
		memset(esp8266.RX_data, 0, sizeof(esp8266.RX_data));
//		printf("发送之前\r\n");
		//发送连接报文
		usart2_send_string((char *)esp8266.tx_buf, esp8266.tx_count);
//		printf("发送之后\r\n");
		return check_server("success", 1000);
		
	}
	
/**
  * @brief  向服务器发送报文
  * @param  None
  * @retval 0 发送成功		1 发送失败
  */
uint8_t mqtt2server(char* cstr, char *payl, char * reply){
	
	MQTTString topicName = MQTTString_initializer;
	topicName.cstring = cstr;
	char* payload = payl;
	esp8266.tx_count = MQTTSerialize_publish(esp8266.tx_buf, 1024, 0, 0, 0, 0, topicName, (unsigned char *)payload, strlen(payload));
	esp8266.RX_count = 0;
//	esp8266.tx_count = 0;
	printf("esp8266.tx_count = %d \r\n", esp8266.tx_count);
	memset(esp8266.RX_data, 0, sizeof(esp8266.RX_data));
	//发送连接报文
	usart2_send_string((char *)esp8266.tx_buf, esp8266.tx_count);
	printf("报文%s", esp8266.tx_buf);
	return check_server(reply, 2000);
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
	usart2_send_string((char *)esp8266.tx_buf, esp8266.tx_count);
	//return check_server("?", 1000);
}
	


	

///**
//  * @brief  获取服务器时间
//  * @param  None
//  * @retval None
//  */
//uint32_t get_server_time(void){
//	char *serverSendTime_addr = NULL;
//	uint8_t  serverSendTime[20] = {0};
//	printf("\r\n时间函数内\r\n");
//	if(mqtt2server(time_publish_cstring, time_payload, "serverSendTime") == 0){
//		printf("\r\n时间数据发送成功\r\n");
//		Delay_ms(2000);
//			serverSendTime_addr = strstr((char *)esp8266.RX_data, "serverSendTime");
//			strncpy((char *)serverSendTime, serverSendTime_addr + strlen("serverSendTime") + 3, 10);
////		printf("%s\r\n",esp8266.RX_data);
//		printf("\r\n*******************************\r\n");	
//		printf("时间戳：%s\r\n", serverSendTime);
//		printf("\r\n*******************************\r\n");
//			return atoi((char *)serverSendTime);
//	
//	
//								
//}
//	else{
//		printf("时间函数内失败\r\n");
//		return 1;
//	}
//}


/**
  * @brief  获取服务器时间	cjson
  * @param  None
  * @retval None
  */
uint32_t get_server_time(void){
	char *serverSendTime_addr = NULL;
	uint8_t  serverSendTime[20] = {0};
	printf("\r\n时间函数内\r\n");
	if(mqtt2server(time_publish_cstring, time_payload, "serverSendTime") == 0){
		printf("\r\n时间数据发送成功\r\n");
		Delay_ms(2000);
			serverSendTime_addr = strstr((char *)esp8266.RX_data, "serverSendTime");
			strncpy((char *)serverSendTime, serverSendTime_addr + strlen("serverSendTime") + 3, 10);
//		printf("%s\r\n",esp8266.RX_data);
		printf("\r\n*******************************\r\n");	
		printf("时间戳：%s\r\n", serverSendTime);
		printf("\r\n*******************************\r\n");
			return atoi((char *)serverSendTime);
	
	
								
}
	else{
		printf("时间函数内失败\r\n");
		return 1;
	}
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
	
	strcpy(send2server_data, property_payload);
//	sprintf(send2server_data, property_payload, temp2, (float)dht_data.hum, kqm_data.CHO, kqm_data.CO2, kqm_data.VOC);
	printf("\r\n上传数据函数内\r\n");
	printf("数据 %s\r\n", send2server_data);
	if(mqtt2server(property_publish_cstring, send2server_data, "success") == 0){
		printf("\r\n上传数据成功\r\n");
								
}
	else{
		printf("上传数据失败\r\n");
	}
}



/**
  * @brief  向服务器上传数据
  * @param  None
  * @retval None
  */
void send2server(char * data){
//	char send2server_data[1024] = {0};

//	strcpy(send2server_data, data);
////	sprintf(send2server_data, property_payload, temp2, (float)dht_data.hum, kqm_data.CHO, kqm_data.CO2, kqm_data.VOC);
//	printf("\r\n上传数据函数内\r\n");
//	printf("数据 %s\r\n", send2server_data);
	if(mqtt2server(property_publish_cstring, data, "success") == 0){
		printf("\r\n上传数据成功\r\n");
								
}
	else{
		printf("上传数据失败\r\n");
	}
}




/**
  * @brief  初始化esp8266
	*						pb6	普通推挽输出
  * @param  None
  * @retval None
  */
void esp32_init(void){
		USART2_init(115200);
}


/**
  * @brief  配置esp8266
  * @param  char *AT_cmd			AT指令
  * @param	char *AT_retval		输入参数，指令校验 
  * @param	uint16_t AT_time	操作时间
  * @retval 成功 0		失败 1	
  */
uint8_t soft2esp(char *AT_cmd, char *AT_retval, uint16_t AT_time){
	//清零
	esp8266.RX_count = 0;
	esp8266.RX_flag = 0;
	memset(esp8266.RX_data, 0, sizeof(esp8266.RX_data));
	
	//发送
	usart_send_string(USART2, AT_cmd);
	
	//判断接收值
	while(AT_time--){
		if(esp8266.RX_flag == 1){
			esp8266.RX_flag = 0;
			if(strstr((char *)esp8266.RX_data, AT_retval) != NULL){
				esp8266.RX_flag = 1;
				return 0;	//返回值正确，设置成功
			}
	}
		Delay_ms(1);
		
	}
	return 1;	//返回值不正确，设置失败
}


/**
  * @brief  esp远程连接服务器
  * @param  None
  * @retval 成功 0		失败 1
  */
uint8_t wifi_init(void){
	uint8_t send_text[128] = {0};
	printf("\r\n");
	printf("\r\n阿里云开始连接\r\n");
	printf("开始初始化\r\n");
	soft2esp("AT+RST\r\n","OK",1000);
	Delay_ms(3000);
	printf("初始化完成\r\n");
//	if(soft2esp("AT\r\n","OK",1000)==0)
	soft2esp("AT\r\n","OK",1000);
	{
		if(soft2esp("AT+CWMODE=1\r\n","OK",1000)==0)
		{
			memset(wifi_name, 0, 32);
			memset(wifi_passwd, 0, 32);
			read_len[0] = 0;
			read_len[1] = 0;
			sFLASH_ReadBuffer(&read_len[0], write_len_addr, 1);
			sFLASH_ReadBuffer(&read_len[1], write_len_addr + 1, 1);
			printf("数据长度 %d\r\n%d\r\n", read_len[0], read_len[1]);
			sFLASH_ReadBuffer(wifi_name, write_wifi_addr, read_len[0]);
			sFLASH_ReadBuffer(wifi_passwd, write_passwd_addr, read_len[1]);
			sprintf((char *)send_text, "AT+CWJAP=\"%s\",\"%s\"\r\n", wifi_name, wifi_passwd);
//			if(soft2esp("AT+CWJAP=\"moumouyao\",\"12345678\"\r\n","OK",10000)==0)
			if(soft2esp((char *)send_text,"OK",10000)==0)
			{
				LCD_Clear(WHITE);
				lcd_display_string(0,90,(uint8_t*)"正在连接服务器",BLUE,WHITE,FONT_SIZE_32,NULL);
				if(soft2esp("AT+CIPMODE=1\r\n","OK",3000)==0)
				{
					if(soft2esp("AT+CIPSTART=\"TCP\",\"iot-06z00ak627vyl6x.mqtt.iothub.aliyuncs.com\",1883\r\n","OK",10000)==0)
//					if(soft2esp("AT+CIPSTART=\"TCP\",\"14.215.177.38\",80\r\n","OK",10000)==0)
					{
						printf("connect to server successed\r\n");
						if(soft2esp("AT+CIPSEND\r\n",">",1000)==0)
						{
							if(mqtt_connect_server() == 0){
								LCD_Clear(WHITE);	
								lcd_display_string(0,90,(uint8_t*)"服务器连接成功",BLUE,WHITE,FONT_SIZE_32,NULL);
								Delay_ms(3000);
								printf("\r\n连接阿里云成功\r\n");
							}
							else{
								printf("\r\n连接阿里云失败\r\n");
								LCD_Clear(WHITE);	
								lcd_display_string(0,90,(uint8_t*)"服务器连接成功",BLUE,WHITE,FONT_SIZE_32,NULL);
								return 1;
							}
							
							return 0;
						
						}
						else{
							LCD_Clear(WHITE);	
							lcd_display_string(0,90,(uint8_t*)"服务器连接失败",BLUE,WHITE,FONT_SIZE_32,NULL);
							return 1;
						}
					
					}
					else{
						printf("connect to server err\r\n");
						LCD_Clear(WHITE);	
						lcd_display_string(0,90,(uint8_t*)"服务器连接失败",BLUE,WHITE,FONT_SIZE_32,NULL);
						return 1;
					}
				}
				else{
					LCD_Clear(WHITE);	
					lcd_display_string(0,90,(uint8_t*)"服务器连接失败",BLUE,WHITE,FONT_SIZE_32,NULL);
					return 1;
				}
			}
			else
			{
				LCD_Clear(WHITE);
				lcd_display_string(0,90,(uint8_t*)"网络连接失败，请检查网络设置",BLUE,WHITE,FONT_SIZE_32,NULL);
				Delay_ms(3000);
				printf("wifi err\r\n");
				return 1;
			}
		}
		else{
			printf("AT+CWMODE=1 err\r\n");
		return 1;
		}
	}
//	else{
//		printf("AT err\r\n");
//		
//		return 1;
//	}
}



//w25q64写入wifi
void wifi_write(char *wifiname, char *wifipasswd){
	uint32_t clean_addr = 0;
	uint8_t read_len[2] = {0};
	
	read_len[0] = strlen((char *)wifiname);
	read_len[1] = strlen((char *)wifipasswd);
	
	clean_addr = write_wifi_addr;
	for(uint8_t i = 0; i < 4; i ++){
		sFLASH_EraseSector(clean_addr);
		clean_addr = clean_addr + 4;
		Delay_ms(100);
	}
	clean_addr = write_passwd_addr;
	for(uint8_t i = 0; i < 4; i ++){
		sFLASH_EraseSector(clean_addr);
		clean_addr = clean_addr + 4;
		Delay_ms(100);
	}
	sFLASH_EraseSector(write_len_addr);
	Delay_ms(100);
	
	sFLASH_WriteBuffer(wifiname, write_wifi_addr, read_len[0]);
	Delay_ms(100);
	sFLASH_WriteBuffer(wifipasswd, write_passwd_addr, read_len[1]);
	Delay_ms(100);
	
	sFLASH_WriteBuffer(&read_len[0], write_len_addr, 1);
	Delay_ms(100);
	sFLASH_WriteBuffer(&read_len[1], write_len_addr + 1, 1);
	Delay_ms(100);

	
}



/**
  * @brief  Cjsom解析天气信息
  * @param  None
  * @retval None
  */
void aliyun_analisys(void){
	cJSON *root;
    cJSON *params;
    cJSON *last_update;
    cJSON *loc_json, *daily_json;
    cJSON *forecast_json;
    char *DeviceSn, *DelCard, *NewCard, *DoorPwd, *AdmPwd;
		char *json_addr = NULL;
		int CardSum, FingerprintNum, VoicePwr;
    int i = 0;
		json_addr = strstr((char *)esp8266.RX_data, "/set");
	printf("接收到的字符串 %s\r\n", esp8266.RX_data);
		printf("整合后的字符串%s\r\n", json_addr + 4);
		esp8266.RX_flag = 0;
    root = cJSON_Parse(json_addr + 4);
    if(root)
    {
        printf("JSON格式正确:\n%s\n\n",cJSON_Print(root));    //输出json字符串
        params = cJSON_GetObjectItem(root, "params");
			
        if(params)
        {
            DeviceSn = cJSON_GetObjectItem(params, "DeviceSn")->valuestring;   //得到location键对应的值，是一个对象
						printf("\r\n设备id %s\r\n", DeviceSn);
            DelCard = cJSON_GetObjectItem(params, "DelCard") -> valuestring;
            printf("\r\n删除门卡 %s\n",DelCard);
            DoorPwd = cJSON_GetObjectItem(params, "DoorPwd") -> valuestring;
            printf("\r\n开门密码 %s\n",DoorPwd);
            AdmPwd = cJSON_GetObjectItem(params, "AdmPwd") -> valuestring;
            printf("\r\n管理员密码 %s\n\n",AdmPwd);
						
						CardSum = cJSON_GetObjectItem(params, "CardSum") ->valueint;
            printf("\r\n注册的卡片数量 %d\n\n",CardSum);
						FingerprintNum = cJSON_GetObjectItem(params, "FingerprintNum") ->valueint;
            printf("\r\n注册的指纹数量 %d\n\n",FingerprintNum);
						VoicePwr = cJSON_GetObjectItem(params, "VoicePwr") ->valueint;
            printf("\r\n音量 %d\n\n",VoicePwr);
					
					

            
        }
        else
        {
            printf("results格式错误:%s\r\n", cJSON_GetErrorPtr());
        }
    }
    else
    {
        printf("JSON格式错误\r\n");
    }
    cJSON_Delete(root);
		printf("\r\n完成*****************\r\n");
								
	

}









