#include "esp8266.h"

esp esp8266;	//存放8266接收信息

extern 	uint32_t write_wifi_addr;
extern 	uint32_t write_passwd_addr;
extern 	uint32_t write_addr_addr;
extern 	uint32_t write_len_addr;
extern uint8_t read_len[3];
char send_text[128] = {0};

uint8_t ap_read_val[3][32] = {0};

/**
  * @brief  初始化esp8266
	*						pb6	普通推挽输出
  * @param  None
  * @retval None
  */
void esp8266_init(void){
		usart3_init();
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);
  	GPIO_InitTypeDef GPIO_InitStructure;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  	GPIO_Init(GPIOE, &GPIO_InitStructure);
		GPIO_SetBits(GPIOE, GPIO_Pin_6);
}

/**
  * @brief  串口一转发给串口三
  * @param  None
  * @retval None
  */
void usart12usart3(void){
	if(usart_get_RXflag() == 1){
			if(strstr(usart_RX_str_packet, "AT") != NULL){
				printf("%s", usart_RX_str_packet);
				usart_send_string(USART3, usart_RX_str_packet);
				//usart_send_string(USART1, usart_RX_str_packet);
				memset(usart_RX_str_packet, 0, sizeof(usart_RX_str_packet));
			}
		}
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
	usart_send_string(USART3, AT_cmd);
	
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
	printf("\r\n");
	printf("\r\n阿里云开始连接\r\n");
	soft2esp("AT+RST\r\n","OK",1000);
	Delay_ms(3000);
	if(soft2esp("AT\r\n","OK",1000)==0)
	{
		if(soft2esp("AT+CWMODE=1\r\n","OK",1000)==0)
		{
			memset(ap_read_val[0], 0, 32);
			memset(ap_read_val[1], 0, 32);
			read_len[0] = 0;
			read_len[1] = 0;
			sFLASH_ReadBuffer(&read_len[0], write_len_addr, 1);
			sFLASH_ReadBuffer(&read_len[1], write_len_addr + 1, 1);
			printf("数据长度 %d\r\n%d\r\n", read_len[0], read_len[1]);
			sFLASH_ReadBuffer(ap_read_val[0], write_wifi_addr, read_len[0]);
			sFLASH_ReadBuffer(ap_read_val[1], write_passwd_addr, read_len[1]);
			sprintf(send_text, "AT+CWJAP=\"%s\",\"%s\"\r\n", ap_read_val[0], ap_read_val[1]);
//			if(soft2esp("AT+CWJAP=\"moumouyao\",\"12345678\"\r\n","OK",10000)==0)
			if(soft2esp(send_text,"OK",10000)==0)
			{
				if(soft2esp("AT+CIPMODE=1\r\n","OK",1000)==0)
				{
					if(soft2esp("AT+CIPSTART=\"TCP\",\"iot-060a7g0x.mqtt.iothub.aliyuncs.com\",1883\r\n","OK",5000)==0)
					{
						printf("connect to server successed\r\n");
						if(soft2esp("AT+CIPSEND\r\n",">",1000)==0)
						{
							if(mqtt_connect_server() == 0){
								printf("\r\n连接阿里云成功\r\n");
							}
							else{
								printf("\r\n连接阿里云失败\r\n");
								return 1;
							}
							
							return 0;
						
						}
						else{
							return 1;
						}
					
					}
					else{
						printf("connect to server err\r\n");
						return 1;
					}
				}
				else{
					return 1;
				}
			}
			else
			{
				printf("wifi err\r\n");
				return 1;
			}
		}
		else{
			printf("AT+CWMODE=1 err\r\n");
		return 1;
		}
	}
	else{
		printf("AT err\r\n");
		
		return 1;
	}
}



// Wifi连接函数
uint8_t connect_wifi(void) {

  if(soft2esp("AT\r\n","OK",1000)==0) {

    if(soft2esp("AT+CWMODE=1\r\n","OK",1000)==0) {

      memset(ap_read_val[0], 0, 32);
      memset(ap_read_val[1], 0, 32);
      read_len[0] = 0;
      read_len[1] = 0; 

      sFLASH_ReadBuffer(&read_len[0], write_len_addr, 1);
      sFLASH_ReadBuffer(&read_len[1], write_len_addr + 1, 1);

     printf("数据长度 %d\r\n%d\r\n", read_len[0], read_len[1]);

      sFLASH_ReadBuffer(ap_read_val[0], write_wifi_addr, read_len[0]);
      sFLASH_ReadBuffer(ap_read_val[1], write_passwd_addr, read_len[1]);

      sprintf(send_text, "AT+CWJAP=\"%s\",\"%s\"\r\n", ap_read_val[0], ap_read_val[1]);

      if(soft2esp(send_text,"OK",10000)==0) {
        printf("wifi连接成功\r\n");
        // WIFI连接成功
        return 0;
        
      } else {
        printf("wifi连接失败\r\n");
        // WIFI连接失败
        return 1;
        
      }

    } else {
      return 1;
    }

  } else {
    return 1; 
  }

}



// 阿里云连接函数
uint8_t connect_aliyun() {

  if(soft2esp("AT+CIPMODE=1\r\n","OK",1000)==0) {

    if(soft2esp("AT+CIPSTART=\"TCP\",\"iot-060a7g0x.mqtt.iothub.aliyuncs.com\",1883\r\n","OK",5000)==0){

      if(soft2esp("AT+CIPSEND\\r\\n",">",1000)==0) {

        if(mqtt_connect_server() == 0) {
          printf("订阅成功\r\n");
          // 发布MQTT信息 
          // 订阅MQTT主题
          
          return 0;
          
        } else {
					printf("订阅失败\r\n");
          return 1;
        }

      } else {
        return 1;  
      }

    } else {
      return 1;
    }

  } else {
    return 1;
  }

}





/**
  * @brief  esp远程连接服心知天气
  * @param  None
  * @retval 成功 0		失败 1
  */
uint8_t connect_seniverse(void){
	printf("\r\n");
	char send_buf[1024] = "GET https://api.seniverse.com/v3/weather/now.json?key=StvgScKh2NewX_quB&location=zhengzhou&language=en&unit=c\r\n";
	char buf[32] = {0};
	soft2esp("+++","OK",1000);
	printf("AT+CIPCLOSE********\r\n");
	soft2esp("AT+CIPCLOSE\r\n","OK",1000);
	printf("重启esp8266\r\n");
	if(soft2esp("AT+RST\r\n","OK",1000) == 1)
		printf("esp8266重启失败\r\n");
	Delay_ms(3000);
	if(soft2esp("AT\r\n","OK",1000)==0)
	{
		if(soft2esp("AT+CWMODE=1\r\n","OK",1000)==0)
		{
			memset(ap_read_val[0], 0, 32);
			memset(ap_read_val[1], 0, 32);
			read_len[0] = 0;
			read_len[1] = 0;
			sFLASH_ReadBuffer(&read_len[0], write_len_addr, 1);
			sFLASH_ReadBuffer(&read_len[1], write_len_addr + 1, 1);
			printf("数据长度 %d\r\n%d\r\n", read_len[0], read_len[1]);
			sFLASH_ReadBuffer(ap_read_val[0], write_wifi_addr, read_len[0]);
			sFLASH_ReadBuffer(ap_read_val[1], write_passwd_addr, read_len[1]);
			sprintf(send_text, "AT+CWJAP=\"%s\",\"%s\"\r\n", ap_read_val[0], ap_read_val[1]);
//			if(soft2esp("AT+CWJAP=\"moumouyao\",\"12345678\"\r\n","OK",10000)==0)
			if(soft2esp(send_text,"OK",10000)==0)
			{
				if(soft2esp("AT+CIPMODE=1\r\n","OK",1000)==0)
				{
					if(soft2esp("AT+CIPSTART=\"TCP\",\"api.seniverse.com\",80\r\n","OK",5000)==0)
					{
						printf("connect to server successed\r\n");
						//snprintf(buf, sizeof(send_buf), "AT+CIPSEND=%d\r\n", strlen(send_buf));
						if(soft2esp("AT+CIPSEND\r\n",">",1000)==0)
						{
							printf("开始收发数据\r\n");
	
						
							if(soft2esp(send_buf, "temperature", 2000) == 0){
								printf("获取天气成功\r\n");
								
								xinzhi_analisys();
								
							}
							else{
								printf("获取天气失败\r\n");
							}

						}
						else{
							printf("开始收发数据失败\r\n");
							return 1;
						}
					
					}
					else{
						printf("connect to server err\r\n");
						return 1;
					}
				}
				else{
					printf("***********1\r\n");
					return 1;
				}
			}
			else
			{
				printf("wifi err\r\n");
				return 1;
			}
		}
		else{
			printf("***********2\r\n");
		return 1;
		}
	soft2esp("+++","OK",1000);
	printf("AT+CIPCLOSE********\r\n");
	}
	else{
		printf("***********3\r\n");
		return 1;
	}

	
}

/**
  * @brief  设置无线网
  * @param  None
  * @retval 成功 0		失败 1
  */
extern lv_ui_2 test2;
uint8_t ap_init(void){
	printf("\r\n");
	printf("\r\n开始设置AP模式\r\n");
	//设置字体
//	LV_FONT_DECLARE(CH_font24);
//	lv_obj_t *label_err = lv_label_create(test2.screen_3, NULL);
//	
//	//Write style LV_LABEL_PART_MAIN for screen_3_label_3_1
//	static lv_style_t style_screen_3_label_3_1_main;
//	lv_style_reset(&style_screen_3_label_3_1_main);

	
//	//Write style state: LV_STATE_DEFAULT for style_screen_3_label_3_1_main
//	lv_style_set_radius(&style_screen_3_label_3_1_main, LV_STATE_DEFAULT, 0);
//	lv_style_set_bg_color(&style_screen_3_label_3_1_main, LV_STATE_DEFAULT, lv_color_make(0xff, 0xff, 0xff));
//	lv_style_set_bg_grad_color(&style_screen_3_label_3_1_main, LV_STATE_DEFAULT, lv_color_make(0xff, 0xff, 0xff));
//	lv_style_set_bg_grad_dir(&style_screen_3_label_3_1_main, LV_STATE_DEFAULT, LV_GRAD_DIR_VER);
//	lv_style_set_bg_opa(&style_screen_3_label_3_1_main, LV_STATE_DEFAULT, 0);
//	lv_style_set_text_color(&style_screen_3_label_3_1_main, LV_STATE_DEFAULT, lv_color_make(0x00, 0x00, 0x00));
//	lv_style_set_text_font(&style_screen_3_label_3_1_main, LV_STATE_DEFAULT, &CH_font24);
//	lv_style_set_text_letter_space(&style_screen_3_label_3_1_main, LV_STATE_DEFAULT, 2);
//	lv_style_set_pad_left(&style_screen_3_label_3_1_main, LV_STATE_DEFAULT, 0);
//	lv_style_set_pad_right(&style_screen_3_label_3_1_main, LV_STATE_DEFAULT, 0);
//	lv_style_set_pad_top(&style_screen_3_label_3_1_main, LV_STATE_DEFAULT, 0);
//	lv_style_set_pad_bottom(&style_screen_3_label_3_1_main, LV_STATE_DEFAULT, 0);
//	lv_obj_add_style(test2.screen_3_label_3_1, LV_LABEL_PART_MAIN, &style_screen_3_label_3_1_main);
//	lv_obj_set_pos(test2.screen_3_label_3_1, 59, 33);
//	lv_obj_set_size(test2.screen_3_label_3_1, 200, 0);
	soft2esp("+++","OK",1000);
	printf("AT+CIPCLOSE********\r\n");
	soft2esp("AT+CIPCLOSE\r\n","OK",1000);
	printf("重启esp8266\r\n");
	if(soft2esp("AT+RST\r\n","OK",1000) == 1)
		printf("esp8266 重启失败\r\n");
	Delay_ms(3000);
	if(soft2esp("AT\r\n","OK",4000)==0)
	{
		if(soft2esp("AT+CWMODE=2\r\n","OK",1000)==0)
		{
			printf("\r\n配置AP模式成功\r\n");
			if(soft2esp("AT+RST\r\n","OK",4000)==0)
			{
				printf("\r\n重启成功\r\n");
				Delay_ms(3000);
				if(soft2esp("AT+CWSAP=\"esp8266yyy\",\"123456789\",1,4\r\n","OK",1000)==0)
				{
					printf("\r\n参数配置成功\r\n");
					if(soft2esp("AT+CIPMUX=1\r\n","OK",2000)==0)
					{
						printf("\r\n开启多链接成功\r\n");
						if(soft2esp("AT+CIPSERVER=1,8086\r\n","OK",1000)==0)
						{
							printf("wifi设置成功\r\n");
							return 0;
								
							}
							else{
								printf("\r\nwifi设置失败\r\n");
								return 1;
							}
							
							
						
						}
					
					else{
						printf("\r\n开启多链接失败\r\n");
						return 1;
					}
				}
				else{
					printf("\r\n设置模块的AP参数失败\r\n");
					return 1;
				}
			}
			else
			{
				printf("\r\n重启失败\r\n");
				return 1;
			}
		
		}
		else{
			printf("\r\n配置ESP8266模块为AP模式失败\r\n");
		return 1;
		}
	
	}
	else{
		printf("\r\n测试指令失败\r\n");
		
		return 1;
	}
}

























