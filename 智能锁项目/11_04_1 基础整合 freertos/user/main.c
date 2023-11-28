
#include "main.h"



RCC_ClocksTypeDef RCC_Clocks;


  uint8_t door_passwd[7] = "888888";
	uint8_t ad_passwd[7] = "666666\0";
	uint8_t card_num = 0;
	uint8_t card_id[100][5] = {0};
	
extern uint32_t door_passwd_addr;
extern uint32_t adm_passwd_addr;
extern	uint8_t wifi_name[32] ;
extern	uint8_t wifi_passwd[32];
extern	uint32_t write_wifi_addr;
extern	uint32_t write_passwd_addr;
extern uint8_t read_len[3];
	
	
	void lcd_show(void);
	
/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{

	
	//初始化系统定时器间隔为1ms

//	Systick_Interrupt(1000/200);//5ms
//	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

	
	
	key_init();
	AT24_init();
  USART1_init(115200);
	BS8116_init();
	sFLASH_Init();
	mg200_init();
	Lcd_Init();			//初始化OLED  
	LCD_Clear(WHITE);
	BACK_COLOR=WHITE;
	voice_init();
	
	motor_init();
	esp32_init();
	Delay_ms(7000);
	TIM3_Int_Init(5000-1,8400-1);
	send_voi_cmd(0X20);

//	
	//变量
	RTC_TimeTypeDef RTC_TimeStruct;
	RTC_DateTypeDef RTC_DateStruct;
	u8 tbuf[40];
	
	printf("开始读取密码\r\n");
	sFLASH_ReadBuffer(door_passwd, door_passwd_addr, 6);
	printf("开门密码 %s\r\n", door_passwd);
	sFLASH_ReadBuffer(ad_passwd, adm_passwd_addr, 6);
	printf("管理员密码 %s\r\n", ad_passwd);
	sFLASH_ReadBuffer(&read_len[0], write_len_addr, 1);
	sFLASH_ReadBuffer(&read_len[1], write_len_addr + 1, 1);
	sFLASH_ReadBuffer(wifi_name, write_wifi_addr, read_len[0]);
	sFLASH_ReadBuffer(wifi_passwd, write_passwd_addr, read_len[1]);
	printf("WiFi name %s\r\n", wifi_name);
	printf("WIFI passwd %s\r\n", wifi_passwd);
	w25_card_write(NULL);

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
//	wifi_write("moumouyao", "12345678");
//	wifi_write("XYD", "12345678");
	lcd_show();
	LCD_Clear(WHITE);
wifi_init();
my_rtc_init();
Delay_ms(2000);
LCD_Clear(WHITE);	
//	
////	subscription_service();
//	get_server_time();

//	send_data2server();
  /* Infinite loop */

	
	
	printf("开始rfid初始化\r\n");
	RFID_Init(); 
//	Reset_RC522();
//	PcdReset();       //功    能：复位RC522
//	ClearBitMask();
	Delay_ms(3000);
	
	printf("开始执行任务\r\n");
	freertos_demo();
	

//	uint8_t data[128];
//	sFLASH_ReadBuffer(data, 1770296, 64);
//	printf("data = %s\r\n", data);
  while (1)
  {
//		printf("循环");
		adm_task();
		
//		 while(PcdRequest(0x52,(uint8_t *)0x0400)!=MI_OK);
//  
//	   printf("寻卡成功");

//		
		Delay_ms(1000);

//	adm_task();
			
			
		if(key_get_num() == 2){
			while(1){
				
				RTC_GetTime(RTC_Format_BIN,&RTC_TimeStruct);
			 sprintf((char*)tbuf,"Time:%02d:%02d:%02d",RTC_TimeStruct.RTC_Hours,RTC_TimeStruct.RTC_Minutes,RTC_TimeStruct.RTC_Seconds); 
			printf("时间 %s\r\n",tbuf);
			RTC_GetDate(RTC_Format_BIN, &RTC_DateStruct);
			 sprintf((char*)tbuf,"Date:20%02d-%02d-%02d",RTC_DateStruct.RTC_Year,RTC_DateStruct.RTC_Month,RTC_DateStruct.RTC_Date); 
			printf("日期 %s\r\n",tbuf);
			sprintf((char*)tbuf,"Week:%d",RTC_DateStruct.RTC_WeekDay); 
			printf("周 %s\r\n",tbuf);

				

				GPIO_ResetBits(GPIOB,GPIO_Pin_8);
				Delay_ms(1000);
				GPIO_SetBits(GPIOB,GPIO_Pin_8);
				Delay_ms(1000);
//				usart_printf("hello");
		printf("hello world\r\n");
//			usart_send_string("hello world\r\n");
			}
			
		}
		
		
  }
}








//任务栈溢出狗子函数
void vApplicationStackOverflowHook()
{
	while(1)
	{
//		printf("Stack overflow in task ---%s\r\n", pcTaskName);
		printf("Stack overflow in task \r\n");
	}
}

//时间片狗子函数
void vApplicationTickHook(void)
{
	
		
	
}

int lcdabs;
uint8_t row;
uint8_t key_wifiname[32] = {0};
uint8_t key_wifipasswd[32] = {0};

void lcd_show(void){
	uint8_t key_val = 0;
	uint8_t  i = 0;
	memset(wifi_name, 0, 32);
	read_len[0] = 0;
	sFLASH_ReadBuffer(&read_len[0], write_len_addr, 1);

	sFLASH_ReadBuffer(wifi_name, write_wifi_addr, read_len[0]);

	lcd_display_string(0,0,(uint8_t*)"当前网络",BLUE,WHITE,FONT_SIZE_24,NULL);
	
	lcd_display_string(0,30,(uint8_t*)wifi_name,BLUE,RED,FONT_SIZE_24,NULL);
	lcd_display_string(0,60,(uint8_t*)"# 退出",BLUE,WHITE,FONT_SIZE_24,NULL);
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
	lcdabs = 0;
	wifi_name:
	lcdabs = lcdabs + 16;
	wifi_name_del:
	while(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_1));
		key_val = keyboard_input();
		
		
		if(key_val == '*'){		//删除
			lcd_display_string(lcdabs + 16 * 5,90,&key_val,WHITE,WHITE,FONT_SIZE_24,NULL);
			printf("lcdabs = %d\r\n", lcdabs);
			if(i == 0)
				goto wifi_passwd_del;
			key_wifiname[i - 1] = 0;
			i--;
			lcdabs = lcdabs - 16;
			printf("WiFi name %s\r\n", key_wifiname);
			goto wifi_name_del;
		}	
		if(key_val != '#'){
			lcd_display_string(lcdabs + 16 * 6,90,&key_val,BLUE,WHITE,FONT_SIZE_24,NULL);
			printf("lcdabs = %d\r\n", lcdabs);
			key_wifiname[i++] = key_val;
			goto wifi_name;
		}
	lcd_display_string(0,0,(uint8_t*)"请输入WIFI密码",BLUE,WHITE,FONT_SIZE_24,NULL);	
	lcdabs = 0;
	i = 0;
	row = 120;
	wifi_passwd:
		lcdabs = lcdabs + 16;
		wifi_passwd_del:
	while(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_1));
		key_val = keyboard_input();
		
		
		if(key_val == '*'){		//删除
			lcd_display_string(lcdabs + 16 * 5,120,&key_val,WHITE,WHITE,FONT_SIZE_24,NULL);
			printf("lcdabs = %d\r\n", lcdabs);
			if(i == 0)
				goto wifi_passwd_del;
			key_wifipasswd[i - 1] = 0;
			i--;
			lcdabs = lcdabs - 16;
			printf("wifi passwd %s\r\n", key_wifipasswd);
			goto wifi_passwd_del;
		}	
		if(key_val != '#'){
			lcd_display_string(lcdabs + 16 * 6,120,&key_val,BLUE,WHITE,FONT_SIZE_24,NULL);
			printf("lcdabs = %d\r\n", lcdabs);
			key_wifipasswd[i++] = key_val;
			goto wifi_passwd;
		}
		
	printf("输出完成\r\n");
	printf("wifiname %s\r\n",key_wifiname);
	printf("wifipasswd %s\r\n", key_wifipasswd);
	
	LCD_Clear(WHITE);
		
	lcd_display_string(0,90,(uint8_t*)"正在连接WiFi",BLUE,WHITE,FONT_SIZE_32,NULL);
		wifi_write((char *)key_wifiname, (char *)key_wifipasswd);		

}





















