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
			printf("\r\nFreeRTOS�Ѿ�����\r\n");
	}
} 


//����ջ������Ӻ���
void vApplicationStackOverflowHook(TaskHandle_t xTask, signed char *pcTaskName )
{
	while(1)
	{
		printf("Stack overflow in task ---%s\r\n", pcTaskName);
	}
}

//ʱ��Ƭ���Ӻ���
void vApplicationTickHook(void)
{
	
		lv_tick_inc(1);	//��֪lvgl 1ms��ȥ��
	
}


void api_init(void){
	usart_init();			//����һ ����ͨ�ų�ʼ��
	led_init(LED1);		//led1��ʼ��
	led_init(LED2);		//led1��ʼ��
	key_init();				//������ʼ��
	buzzer_init();		//��������ʼ��
	
	dth11_init();			//dth11��ʼ��
//	LCD_Init();				//LCD����ʼ��
	kqm_init();				//kqm��ʼ��
	su_o3t_init();		//su_o3t��ʼ��
	dth11_init();			//dth11��ʼ��
	ADC1_DMA_init();	//ADC1��ͨ��+DMAת�˳�ʼ��
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
usart_send_string(USART1, "Ҧ����\r\n");
	setup_scr_screen(&test);
	setup_scr_screen_3(&test2);
	setup_scr_screen_2(&test2);
	
	
//	wifi_set();
	
//	cartion();
//	setup_scr_screen(&test);
	
//	font_test();

	
	
		//���Դ���--��ʾһ��label
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
//	// ����image�������ʼͼƬΪframes����ĵ�һ��
//	lv_img_set_src(img, screen_3_animimg_3_1_imgs[0]);
//	// ����һ������
//	static lv_anim_t a;  

//	// ��ʼ����������  
//	lv_anim_init(&a);

//	// ���ö�������Ϊ1000����
//	lv_anim_set_time(&a, 10000);

//	// ���ö�����ֵֹ,Ϊframes������β��ַ
//	lv_anim_set_values(&a, screen_3_animimg_3_1_imgs[0], screen_3_animimg_3_1_imgs[3]);  

//	// ���ö�������Ϊlv_img_set_src
//	lv_anim_set_exec_cb(&a, (lv_anim_exec_xcb_t) lv_img_set_src);

//	// ��image������Ϊ���������Ĳ���
//	lv_anim_set_var(&a, img);   

//	// ��������
//	lv_anim_start(&a);
//}








