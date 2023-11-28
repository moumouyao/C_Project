#include "ucos_task.h"

uint8_t key_wifiname[32] = {0};
uint8_t key_wifipasswd[32] = {0};
OS_STK LCD_TASK[LCD_STACK_LEN];


void LcdTask(void *pdata)
{
	pdata=pdata;//防止编译器优化
	
	
	
	printf("key task coming!!\r\n");
	while(1)
	{
//		printf("lcd_time任务\r\n");
		lcd_task();
		OSTimeDly(25);
	}
	
}
uint8_t lcdabs = 0, row;
void lcd_show(void){
	uint8_t key_val = 0;
	uint8_t  i = 0;
	memset(wifi_name, 0, 32);
	read_len[0] = 0;
	sFLASH_ReadBuffer(&read_len[0], write_len_addr, 1);

	sFLASH_ReadBuffer(wifi_name, write_wifi_addr, read_len[0]);

	lcd_display_string(0,0,(uint8_t*)"当前网络",BLUE,WHITE,FONT_SIZE_24,NULL);
	
	lcd_display_string(0,30,(uint8_t*)wifi_name,BLUE,RED,FONT_SIZE_24,NULL);
	lcd_display_string(0,60,(uint8_t*)"# 修改",BLUE,WHITE,FONT_SIZE_24,NULL);
	while(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_1));
		key_val = keyboard_input();
	if(key_val == '#'){
		return;
	}

	LCD_Clear(WHITE);
	lcd_display_string(0,0,(uint8_t*)"请输入WIFI名称",BLUE,WHITE,FONT_SIZE_24,NULL);
	lcd_display_string(0,30,(uint8_t*)"按 # 结束",BLUE,WHITE,FONT_SIZE_24,NULL);
	lcd_display_string(0,90,(uint8_t*)"WIFI名称:",BLUE,WHITE,FONT_SIZE_24,NULL);
	lcd_display_string(0,120,(uint8_t*)"WIFI密码:",BLUE,WHITE,FONT_SIZE_24,NULL);
	lcdabs = 10;
	//输入WiFi名称
	i = 0;
	row = 90;
	wifi_name:
	while(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_1));
		key_val = keyboard_input();
		
		lcdabs = lcdabs + 16;
		if(key_val == '*'){		//删除
			lcd_display_string(lcdabs + 16 * 4,90,&key_val,WHITE,WHITE,FONT_SIZE_24,NULL);
			printf("lcdabs = %d\r\n", lcdabs);
			key_wifiname[i - 1] = 0;
			i--;
			goto wifi_name;
		}	
		if(key_val != '#'){
			lcd_display_string(lcdabs + 16 * 5,90,&key_val,BLUE,WHITE,FONT_SIZE_24,NULL);
			printf("lcdabs = %d\r\n", lcdabs);
			key_wifiname[i++] = key_val;
			goto wifi_name;
		}
	lcd_display_string(0,0,(uint8_t*)"请输入WIFI密码",BLUE,WHITE,FONT_SIZE_24,NULL);	
	lcdabs = 10;
	i = 0;
	row = 120;
	wifi_passwd:
	while(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_1));
		key_val = keyboard_input();
		
		lcdabs = lcdabs + 16;
		if(key_val == '*'){		//删除
			lcd_display_string(lcdabs + 16 * 4,90,&key_val,WHITE,WHITE,FONT_SIZE_24,NULL);
			printf("lcdabs = %d\r\n", lcdabs);
			key_wifiname[i - 1] = 0;
			i--;
			goto wifi_name;
		}	
		if(key_val != '#'){
			lcd_display_string(lcdabs + 16 * 5,120,&key_val,BLUE,WHITE,FONT_SIZE_24,NULL);
			printf("lcdabs = %d\r\n", lcdabs);
			key_wifipasswd[i++] = key_val;
			goto wifi_passwd;
		}
		
	printf("输出完成\r\n");
	printf("wifiname %s\r\n",key_wifiname);
	printf("wifipasswd %s\r\n", key_wifipasswd);
	
	LCD_Clear(WHITE);
		
	lcd_display_string(0,90,(uint8_t*)"正在连接WiFi",BLUE,WHITE,FONT_SIZE_32,NULL);
//		wifi_write(key_wifiname, key_wifipasswd);		

}

	uint8_t time_data[128];
	RTC_TimeTypeDef RTC_TimeStruct;
void	lcd_task(void){

	RTC_GetTime(RTC_Format_BIN,&RTC_TimeStruct);
	sprintf((char*)time_data,"Time:%02d:%02d:%02d",RTC_TimeStruct.RTC_Hours,RTC_TimeStruct.RTC_Minutes,RTC_TimeStruct.RTC_Seconds);
	
	lcd_display_string(0,0,(uint8_t*)time_data,BLUE,WHITE,FONT_SIZE_32,NULL);
	lcd_display_string(0,120,(uint8_t *)"_ _ _ _ _ _",BLUE,WHITE,FONT_SIZE_24,NULL);
//	printf("time %s\r\n", time_data);
	memset(time_data, 0, 128);
}



















