#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "led.h"
#include "buzzer.h"
#include "key.h"
#include "interrupt.h"
#include "timer.h"
#include "usart.h"
#include <string.h>
#include "kqm.h"
#include "su_03t.h"
#include "dht11.h"
#include "ADC.h"
//#include "pwm.h"
#include "MyDMA.h"
#include "RTC.h"
#include "bsp_lcd.h"
//#include "my_lcd.h"
#include "myrtos.h"
#include "my_spi_flash.h"
#include "lvgl.h"
#include "lv_port_disp.h"
#include "lvgl_app.h"

	lv_ui test;
	lv_ui_2 test2;

extern lv_obj_t * scr1;
void api_init(void);
void cartion(void);
void setup_animation();
int main(){
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);

	api_init();
	
	freertos_demo();
	while(1){
			printf("\r\nFreeRTOS已经崩溃\r\n");
	}
} 


//任务栈溢出狗子函数
void vApplicationStackOverflowHook(TaskHandle_t xTask, signed char *pcTaskName )
{
	while(1)
	{
		printf("Stack overflow in task ---%s\r\n", pcTaskName);
	}
}

//时间片狗子函数
void vApplicationTickHook(void)
{
	
		lv_tick_inc(1);	//告知lvgl 1ms过去了
	
}


void api_init(void){
	usart_init();			//串口一 串口通信初始化
	led_init(LED1);		//led1初始化
	led_init(LED2);		//led1初始化
	key_init();				//按键初始化
	buzzer_init();		//蜂鸣器初始化
	
	dth11_init();			//dth11初始化
//	LCD_Init();				//LCD屏初始化
	kqm_init();				//kqm初始化
	su_o3t_init();		//su_o3t初始化
	dth11_init();			//dth11初始化
	ADC1_DMA_init();	//ADC1多通道+DMA转运初始化
	sFLASH_Init();
	esp8266_init();
	Delay_ms(3000);
	wifi_init();
	subscription_service();
	rtc_init();
//	ap_init();
	lv_init();
	lv_port_disp_init();
	lvgl_show_data_init();
//	btn_event_test();
usart_send_string(USART1, "姚乐毅\r\n");
	setup_scr_screen(&test);
	setup_scr_screen_3(&test2);
	setup_scr_screen_2(&test2);
	
	
//	wifi_set();
	
//	cartion();
//	setup_scr_screen(&test);
	
//	font_test();

	
	
		//测试代码--显示一个label
//	lv_obj_t * label1 = lv_label_create(lv_scr_act(), NULL);
//	lv_label_set_long_mode(label1, LV_LABEL_LONG_BREAK);     
//	lv_label_set_recolor(label1, true);                      
//	lv_label_set_align(label1, LV_LABEL_ALIGN_CENTER);
//	lv_label_set_text(label1, "#0000ff Re-color# #ff00ff words# #ff0000 of a# label "
//																										"and  wrap long text automatically.");
//	lv_obj_set_width(label1, 150);
//	lv_obj_align(label1, NULL, LV_ALIGN_CENTER, 0, -30);

}

//void cartion(void){
//static const lv_img_dsc_t* screen_3_animimg_3_1_imgs[4] = {
//	&screen_3_animimg_3_1w1,
//	&screen_3_animimg_3_1w2,
//	&screen_3_animimg_3_1w3,
//	&screen_3_animimg_3_1w4
//};
//	lv_obj_t * img = lv_img_create(test2.screen_3, NULL);
//	// 设置image对象的起始图片为frames数组的第一张
//	lv_img_set_src(img, screen_3_animimg_3_1_imgs[0]);
//	// 创建一个动画
//	static lv_anim_t a;  

//	// 初始化动画参数  
//	lv_anim_init(&a);

//	// 设置动画长度为1000毫秒
//	lv_anim_set_time(&a, 10000);

//	// 设置动画起止值,为frames数组首尾地址
//	lv_anim_set_values(&a, screen_3_animimg_3_1_imgs[0], screen_3_animimg_3_1_imgs[3]);  

//	// 设置动画函数为lv_img_set_src
//	lv_anim_set_exec_cb(&a, (lv_anim_exec_xcb_t) lv_img_set_src);

//	// 将image对象作为动画函数的参数
//	lv_anim_set_var(&a, img);   

//	// 启动动画
//	lv_anim_start(&a);
//}








