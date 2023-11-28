#include "my_mqtt.h"

extern esp esp8266;	//���8266������Ϣ

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
	* @brief  ʹ�ô���3������һ���ַ�e
	* @param  char *str ��Ҫ���͵��ַ���
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
  * @brief  ������������ֵ
  * @param  uint16_t time	���ú����������ʱ��
  * @retval 0 ���ӳɹ�		1 ����ʧ��
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
  * @brief  ���ӷ���������
  * @param  None
  * @retval None
  */
	uint8_t mqtt_connect_server(void){
		
		//�������ӱ���
		MQTTPacket_connectData options = MQTTPacket_connectData_initializer;	//ͨ�ò�������Ĭ��ֵ
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
		//�������ӱ���
		usart3_send_string((char *)esp8266.tx_buf, esp8266.tx_count);
		return check_server("success", 1000);
		
	}
	
/**
  * @brief  ����������ͱ���
  * @param  None
  * @retval 0 ���ͳɹ�		1 ����ʧ��
  */
uint8_t mqtt2server(char* cstr, char *payl, char * reply){
	
	MQTTString topicName = MQTTString_initializer;
	topicName.cstring = cstr;
	char* payload = payl;
	esp8266.tx_count = MQTTSerialize_publish(esp8266.tx_buf, 1024, 0, 0, 0, 0, topicName, (unsigned char *)payload, strlen(payload));
	esp8266.RX_count = 0;
	memset(esp8266.RX_data, 0, sizeof(esp8266.RX_data));
	//�������ӱ���
	usart3_send_string((char *)esp8266.tx_buf, esp8266.tx_count);
	printf("����%s", esp8266.tx_buf);
	return check_server(reply, 2000);
}

/**
  * @brief  ���ķ���������������
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
	//�������ӱ���
	usart3_send_string((char *)esp8266.tx_buf, esp8266.tx_count);
	//return check_server("?", 1000);
}
	

/**
  * @brief  �������������÷���
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
		printf("�¶� %s\r\n", ser_val[0]);
		printf("ʪ�� %s\r\n", ser_val[1]);
	}
}
	

/**
  * @brief  ��ȡ������ʱ��
  * @param  None
  * @retval None
  */
uint32_t get_server_time(void){
	char *serverSendTime_addr = NULL;
	uint8_t  serverSendTime[20] = {0};
	printf("\r\nʱ�亯����\r\n");
	if(mqtt2server(time_publish_cstring, time_payload, "serverSendTime") == 0){
		printf("\r\nʱ�����ݷ��ͳɹ�\r\n");
		Delay_ms(2000);
			serverSendTime_addr = strstr((char *)esp8266.RX_data, "serverSendTime");
			strncpy((char *)serverSendTime, serverSendTime_addr + strlen("serverSendTime") + 3, 10);
//		printf("%s\r\n",esp8266.RX_data);
		printf("\r\n*******************************\r\n");	
		printf("ʱ�����%s\r\n", serverSendTime);
		printf("\r\n*******************************\r\n");
			return atoi((char *)serverSendTime);
	
	
								
}
	else{
		printf("ʱ�亯����ʧ��\r\n");
		return 1;
	}
}


/**
  * @brief  ��������ϴ�����
  * @param  None
  * @retval None
  */
void send_data2server(void){
	char send2server_data[1024] = {0};
	char temp1[16];
	float temp2;
	sprintf(temp1, "%d.%d", dht_data.tmp ,dht_data.tmp_flo);
	temp2 = atof(temp1);
//	printf("�¶�1��%f\r\n",temp2);
//	printf("�¶�2%f\r\n", (dht_data.tmp + (dht_data.tmp_flo / 10.0)));
	strcpy(send2server_data, property_payload);
	sprintf(send2server_data, property_payload, temp2, (float)dht_data.hum, kqm_data.CHO, kqm_data.CO2, kqm_data.VOC);
	printf("\r\n�ϴ����ݺ�����\r\n");
	printf("���� %s\r\n", send2server_data);
	if(mqtt2server(property_publish_cstring, send2server_data, "success") == 0){
		printf("\r\n�ϴ����ݳɹ�\r\n");
								
}
	else{
		printf("�ϴ�����ʧ��\r\n");
	}
}



///**
//  * @brief  Cjsom����������Ϣ
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
//        printf("JSON��ʽ��ȷ:\n%s\n\n",cJSON_Print(root));    //���json�ַ���
//        results = cJSON_GetObjectItem(root, "results");
//        results = cJSON_GetArrayItem(results,0);
//        if(results)
//        {
//            loc_json = cJSON_GetObjectItem(results, "location");   //�õ�location����Ӧ��ֵ����һ������

//            loc_tmp = cJSON_GetObjectItem(loc_json, "id") -> valuestring;
//            printf("\r\n����ID:%s\n",loc_tmp);
//            loc_tmp = cJSON_GetObjectItem(loc_json, "name") -> valuestring;
//            printf("\r\n��������:%s\n",loc_tmp);
//            loc_tmp = cJSON_GetObjectItem(loc_json, "timezone") -> valuestring;
//            printf("\r\n����ʱ��:%s\n\n",loc_tmp);

//            daily_json = cJSON_GetObjectItem(results, "now");
//            if(daily_json)
//            {
//                
//                    weather_tmp = cJSON_GetObjectItem(daily_json, "text") -> valuestring;
//                    printf("\r\n����:%s\r\n", weather_tmp);
//                    weather_tmp = cJSON_GetObjectItem(daily_json, "code") -> valuestring;
//                    printf("\r\ncode:%s\r\n", weather_tmp);
//                    weather_tmp = cJSON_GetObjectItem(daily_json, "temperature") -> valuestring;
//                    printf("\r\n�¶�:%s\r\n", weather_tmp);
////                    weather_tmp = cJSON_GetObjectItem(forecast_json, "high") -> valuestring;
////                    printf("����¶�:%s\r\n", weather_tmp);
////                    weather_tmp = cJSON_GetObjectItem(forecast_json, "low") -> valuestring;
////                    printf("����¶�:%s\r\n", weather_tmp);
////                    weather_tmp = cJSON_GetObjectItem(forecast_json, "wind_direction_degree") -> valuestring;
////                    printf("����Ƕ�:%s\r\n", weather_tmp);
////                    weather_tmp = cJSON_GetObjectItem(forecast_json, "wind_scale") -> valuestring;
////                    printf("����:%s\r\n\n", weather_tmp);
//                
//            }
//            else
//                printf("daily json��ʽ����\r\n");
//            last_update = cJSON_GetObjectItem(results, "last_update");
//            update_tmp = last_update->valuestring;
//            if(last_update)
//            {
//                printf("����ʱ��:%s\r\n", update_tmp);
//            }
//        }
//        else
//        {
//            printf("results��ʽ����:%s\r\n", cJSON_GetErrorPtr());
//        }
//    }
//    else
//    {
//        printf("JSON��ʽ����\r\n");
//    }
//    cJSON_Delete(root);
//		printf("\r\n���*****************\r\n");
//								
//	

//}


/**
  * @brief  �������������÷���
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
		
		printf("�¶� %s\r\n", weather_val[0]);
		printf("�ص� %s\r\n", weather_val[1]);
		printf("���� %s\r\n", weather_val[2]);
		printf("����ʱ�� %s\r\n", weather_val[3]);
	}
}

/**
  * @brief  wifi�������÷���
  * @param  None		1770296
  * @retval None
  */
void AP_analisys(void){
	char *property_val[] = {"wifiname","password", "mycity"};
	char *property_addr[3] = {NULL};
	uint32_t clean_addr = 0;

	
	uint8_t j = 0;
	if(esp8266.RX_flag == 1){
		printf("������������ɹ�\r\n");
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
		printf("�������� %s\r\n", weather_val[3]);
//		printf("%d\r\n", strlen((char *)ap_val[0]));
		memset(weather_val[3], 0, 64);
		sFLASH_ReadBuffer(weather_val[3], write_passwd_addr, read_len[1]);
		printf("�������� %s\r\n", weather_val[3]);
//		printf("%d\r\n", strlen((char *)ap_val[1]));
		memset(weather_val[3], 0, 64);
		sFLASH_ReadBuffer(weather_val[3], write_addr_addr, read_len[2]);
		printf("�������� %s\r\n", weather_val[3]);
//		printf("%d\r\n", strlen((char *)ap_val[2]));
		
	
	}
}





















