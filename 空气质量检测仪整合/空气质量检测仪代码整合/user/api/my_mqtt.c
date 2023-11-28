#include "my_mqtt.h"

extern esp esp8266;	//存放8266接收信息

//SER_VAL ser_val;
uint8_t ser_val[5][4] = {0};
uint8_t weather_val[4][32] = {0};
uint8_t ap_val[3][32] = {0};

	uint32_t write_wifi_addr = 1770296;
	uint32_t write_passwd_addr = 1770360;
	uint32_t write_addr_addr = 1770424;
	uint32_t write_len_addr = 1770488;
	uint8_t read_len[3] = {0};


/**
	* @brief  使用串口3，发送一个字符e
	* @param  char *str 需要发送的字符串
  * @retval None
  */
void usart3_send_string(char *str, uint16_t count){
	uint8_t i = 0;
	for(i = 0; i < count; i ++){
		usart_sendbit(USART3, str[i]);
		//printf("%c", str[i]);
	}
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
			if((esp8266.RX_data[0] == 0x20) && (esp8266.RX_data[1] == 0x02)){
//				esp8266.RX_count = 0;
//				memset(esp8266.RX_data, 0, sizeof(esp8266.RX_data));
				return 0;
			}
			else if(strstr((char *)esp8266.RX_data, str) != NULL){
//				esp8266.RX_count = 0;
//				memset(esp8266.RX_data, 0, sizeof(esp8266.RX_data));
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
	uint8_t mqtt_connect_server(void){
		
		//配置连接报文
		MQTTPacket_connectData options = MQTTPacket_connectData_initializer;	//通用参数设置默认值
//		options.cleansession = ;
		options.clientID.cstring = my_client_id;
//		options.keepAliveInterval = ;
//		options.MQTTVersion = ;
		options.password.cstring = my_password;
//		options.struct_id = ;
//		options.struct_version = ;
		options.username.cstring = my_username;
//		options.will = ;
//		options.willFlag = ;
		esp8266.tx_count = MQTTSerialize_connect(esp8266.tx_buf, 1024, &options);
		
		esp8266.RX_count = 0;
		memset(esp8266.RX_data, 0, sizeof(esp8266.RX_data));
		//发送连接报文
		usart3_send_string((char *)esp8266.tx_buf, esp8266.tx_count);
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
	memset(esp8266.RX_data, 0, sizeof(esp8266.RX_data));
	//发送连接报文
	usart3_send_string((char *)esp8266.tx_buf, esp8266.tx_count);
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
	usart3_send_string((char *)esp8266.tx_buf, esp8266.tx_count);
	//return check_server("?", 1000);
}
	

/**
  * @brief  服务器属性设置分析
  * @param  None
  * @retval None
  */
void subscription_analisys(void){
	char *property_val[] = {"temperature", "Humidity"};
	char *property_addr[2] = {NULL};
	if(esp8266.RX_flag == 1){
		esp8266.RX_flag = 0;
		for(int i = 0; i < 2; i++){
			property_addr[i] = strstr((char *)esp8266.RX_data, property_val[i]);
			
		}
		for(int i = 0; i < 2; i++){
			strncpy((char *)ser_val[i], property_addr[i] + strlen(property_val[i]) + 2, 4);
		}
		printf("温度 %s\r\n", ser_val[0]);
		printf("湿度 %s\r\n", ser_val[1]);
	}
}
	

/**
  * @brief  获取服务器时间
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
	sprintf(temp1, "%d.%d", dht_data.tmp ,dht_data.tmp_flo);
	temp2 = atof(temp1);
//	printf("温度1：%f\r\n",temp2);
//	printf("温度2%f\r\n", (dht_data.tmp + (dht_data.tmp_flo / 10.0)));
	strcpy(send2server_data, property_payload);
	sprintf(send2server_data, property_payload, temp2, (float)dht_data.hum, kqm_data.CHO, kqm_data.CO2, kqm_data.VOC);
	printf("\r\n上传数据函数内\r\n");
	printf("数据 %s\r\n", send2server_data);
	if(mqtt2server(property_publish_cstring, send2server_data, "success") == 0){
		printf("\r\n上传数据成功\r\n");
								
}
	else{
		printf("上传数据失败\r\n");
	}
}



///**
//  * @brief  Cjsom解析天气信息
//  * @param  None
//  * @retval None
//  */
//void whther_analisys(void){
//	cJSON *root;
//    cJSON *results;
//    cJSON *last_update;
//    cJSON *loc_json, *daily_json;
//    cJSON *forecast_json;
//    char *loc_tmp, *weather_tmp, *update_tmp;
//    int i = 0;

//    root = cJSON_Parse((const char*)esp8266.RX_data);
//    if(root)
//    {
//        printf("JSON格式正确:\n%s\n\n",cJSON_Print(root));    //输出json字符串
//        results = cJSON_GetObjectItem(root, "results");
//        results = cJSON_GetArrayItem(results,0);
//        if(results)
//        {
//            loc_json = cJSON_GetObjectItem(results, "location");   //得到location键对应的值，是一个对象

//            loc_tmp = cJSON_GetObjectItem(loc_json, "id") -> valuestring;
//            printf("\r\n城市ID:%s\n",loc_tmp);
//            loc_tmp = cJSON_GetObjectItem(loc_json, "name") -> valuestring;
//            printf("\r\n城市名称:%s\n",loc_tmp);
//            loc_tmp = cJSON_GetObjectItem(loc_json, "timezone") -> valuestring;
//            printf("\r\n城市时区:%s\n\n",loc_tmp);

//            daily_json = cJSON_GetObjectItem(results, "now");
//            if(daily_json)
//            {
//                
//                    weather_tmp = cJSON_GetObjectItem(daily_json, "text") -> valuestring;
//                    printf("\r\n天气:%s\r\n", weather_tmp);
//                    weather_tmp = cJSON_GetObjectItem(daily_json, "code") -> valuestring;
//                    printf("\r\ncode:%s\r\n", weather_tmp);
//                    weather_tmp = cJSON_GetObjectItem(daily_json, "temperature") -> valuestring;
//                    printf("\r\n温度:%s\r\n", weather_tmp);
////                    weather_tmp = cJSON_GetObjectItem(forecast_json, "high") -> valuestring;
////                    printf("最高温度:%s\r\n", weather_tmp);
////                    weather_tmp = cJSON_GetObjectItem(forecast_json, "low") -> valuestring;
////                    printf("最低温度:%s\r\n", weather_tmp);
////                    weather_tmp = cJSON_GetObjectItem(forecast_json, "wind_direction_degree") -> valuestring;
////                    printf("风向角度:%s\r\n", weather_tmp);
////                    weather_tmp = cJSON_GetObjectItem(forecast_json, "wind_scale") -> valuestring;
////                    printf("风力:%s\r\n\n", weather_tmp);
//                
//            }
//            else
//                printf("daily json格式错误\r\n");
//            last_update = cJSON_GetObjectItem(results, "last_update");
//            update_tmp = last_update->valuestring;
//            if(last_update)
//            {
//                printf("更新时间:%s\r\n", update_tmp);
//            }
//        }
//        else
//        {
//            printf("results格式错误:%s\r\n", cJSON_GetErrorPtr());
//        }
//    }
//    else
//    {
//        printf("JSON格式错误\r\n");
//    }
//    cJSON_Delete(root);
//		printf("\r\n完成*****************\r\n");
//								
//	

//}


/**
  * @brief  服务器属性设置分析
  * @param  None
  * @retval None
  */
void xinzhi_analisys(void){
	char *property_val[] = {"temperature","path", "code", "last_update"};
	char *property_addr[4] = {NULL};
	uint8_t j = 0;
	if(esp8266.RX_flag == 1){
		esp8266.RX_flag = 0;
		for(int i = 0; i < 4; i++){
			property_addr[i] = strstr((char *)esp8266.RX_data, property_val[i]);
			
		}
		for(int i = 0; i < 4; i++){
			j = 0;
			while(1){
				if(*(property_addr[i] + strlen(property_val[i]) + j + 3) != '"'){
				strncpy((char *)(&weather_val[i][j]), property_addr[i] + strlen(property_val[i]) + 3 + j, 1);
				j++;
			}
				else{
					printf("%d\r\n", j);
					
					break;
				}
			}
			
			
		}
	uint8_t m = 0, tmpstr[32] = {0};
	strcpy((char *)tmpstr, (char *)weather_val[1]);
	memset((weather_val[1]), 0, 32);
		while(1){
				if(*(tmpstr + m) != ','){
				strncpy((char *)(&weather_val[1][m]), (char *)tmpstr + m, 1);
				m++;
			}
				else{
					printf("length = %d\r\n", m);
					
					break;
				}
			}
		
		printf("温度 %s\r\n", weather_val[0]);
		printf("地点 %s\r\n", weather_val[1]);
		printf("天气 %s\r\n", weather_val[2]);
		printf("更新时间 %s\r\n", weather_val[3]);
	}
}

/**
  * @brief  wifi属性设置分析
  * @param  None		1770296
  * @retval None
  */
void AP_analisys(void){
	char *property_val[] = {"wifiname","password", "mycity"};
	char *property_addr[3] = {NULL};
	uint32_t clean_addr = 0;

	
	uint8_t j = 0;
	if(esp8266.RX_flag == 1){
		printf("进入分析函数成功\r\n");
		esp8266.RX_flag = 0;
		for(int i = 0; i < 3; i++){
			property_addr[i] = strstr((char *)esp8266.RX_data, property_val[i]);
			
		}
		for(int i = 0; i < 3; i++){
			j = 0;
			while(1){
				//printf("1");
				if(*(property_addr[i] + strlen(property_val[i]) + j + 3) != '"'){
				strncpy((char *)(&ap_val[i][j]), property_addr[i] + strlen(property_val[i]) + 3 + j, 1);
				j++;
			}
				else{
					printf("%d\r\n", j);
					read_len[i] = j;
					break;
				}
			}
			
			
		}
		
		printf("wifi %s\r\n", ap_val[0]);
		printf("passwd %s\r\n", ap_val[1]);
		printf("addr %s\r\n", ap_val[2]);
		printf("%d\r\n", strlen((char *)ap_val[0]));
		printf("%d\r\n", strlen((char *)ap_val[1]));
		printf("%d\r\n", strlen((char *)ap_val[2]));
		
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
		clean_addr = write_addr_addr;
		for(uint8_t i = 0; i < 4; i ++){
			sFLASH_EraseSector(clean_addr);
			clean_addr = clean_addr + 4;
			Delay_ms(100);
		}
		
//		sFLASH_EraseSector(write_passwd_addr);
//		Delay_ms(1000);
//		sFLASH_EraseSector(write_addr_addr);
//		Delay_ms(1000);
		sFLASH_EraseSector(write_len_addr);
		Delay_ms(100);
		
		sFLASH_WriteBuffer(ap_val[0], write_wifi_addr, read_len[0]);
		Delay_ms(100);
		sFLASH_WriteBuffer(ap_val[1], write_passwd_addr, read_len[1]);
		Delay_ms(100);
		sFLASH_WriteBuffer(ap_val[2], write_addr_addr, read_len[2]);
		Delay_ms(100);
		
		sFLASH_WriteBuffer(&read_len[0], write_len_addr, 1);
		Delay_ms(100);
		sFLASH_WriteBuffer(&read_len[1], write_len_addr + 1, 1);
		Delay_ms(100);
		sFLASH_WriteBuffer(&read_len[2], write_len_addr + 2, 1);
		Delay_ms(100);
		
		memset(weather_val[3], 0, 64);
		sFLASH_ReadBuffer(weather_val[3], write_wifi_addr, read_len[0]);
		printf("读出数据 %s\r\n", weather_val[3]);
//		printf("%d\r\n", strlen((char *)ap_val[0]));
		memset(weather_val[3], 0, 64);
		sFLASH_ReadBuffer(weather_val[3], write_passwd_addr, read_len[1]);
		printf("读出数据 %s\r\n", weather_val[3]);
//		printf("%d\r\n", strlen((char *)ap_val[1]));
		memset(weather_val[3], 0, 64);
		sFLASH_ReadBuffer(weather_val[3], write_addr_addr, read_len[2]);
		printf("读出数据 %s\r\n", weather_val[3]);
//		printf("%d\r\n", strlen((char *)ap_val[2]));
		
	
	}
}





















