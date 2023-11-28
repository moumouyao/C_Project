#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "led.h"
#include "interrupt.h"
#include "timer.h"
#include "usart.h"
#include <string.h>
#include "RS485.h"
#include "key.h"
#include "7s1.h"
#include "myjson.h"


int main(){
	sys_tick_init();
	usart_init();
	uart4_init();
	RGB_init();
	led_init(LED1);
	timer2_init();
	RS485_init();
//	Modbus_Init();
	key_init();				//按键初始化
//	Cat_Connect_Net();
//	Delay_ms(30000);
	
	
	printf("你好，世界\r\n");


	while(1){
	led_time_scale(2000);
	dataanalysis_time_scale(1000);
	getdata_time_scale(5000);
	modbus_time_scale(3000);
//		if(json_data.RX_flag == 1){
//			json_data.RX_count = 0;
//			json_data.RX_flag = 0;
//			
//			json_analysis();
//			Cat_Connect_Modify();
//			memset(json_data.RX_data, 0, 2048);
//		}
			
//		Delay_ms(5000);

	
}

}











