#include "lvgl_app.h"

lv_obj_t * scr1;
lv_obj_t * scr2;

lv_obj_t * label0;
lv_obj_t * label1;
lv_obj_t * label2;
lv_obj_t * label3;
lv_obj_t * label4;

lv_obj_t * label_data1;
lv_obj_t * label_data2;
lv_obj_t * label_data3;
lv_obj_t * label_data4;
lv_obj_t * label_time;

lv_obj_t *btn1;
lv_obj_t *btn2;

//数据显示初始化
void lvgl_show_data_init(void){
		LV_FONT_DECLARE(CH_font24);
	/* Create a style for the shadow*/
	static lv_style_t label_shadow_style; //创建一个样式变量
	lv_style_init(&label_shadow_style);//初始化
	lv_style_set_text_opa(&label_shadow_style, LV_STATE_DEFAULT, LV_OPA_50);//设置样式透明度
	lv_style_set_text_color(&label_shadow_style, LV_STATE_DEFAULT, LV_COLOR_RED);//红色
	lv_style_set_text_font(&label_shadow_style, LV_STATE_DEFAULT, &CH_font24);
	
	//创建标签
	scr1 = lv_obj_create(NULL, NULL);//创建第1个屏幕
	scr2 = lv_obj_create(NULL, NULL);//创建第2个屏幕
	
	label0 = lv_label_create(scr1, NULL);
	label1 = lv_label_create(scr1, NULL);
	label2 = lv_label_create(scr1, NULL);
	label3 = lv_label_create(scr1, NULL);
	label4 = lv_label_create(scr1, NULL);
	label_time = lv_label_create(scr2, NULL);
	
	label_data1 = lv_label_create(scr1, NULL);
	label_data2 = lv_label_create(scr1, NULL);
	label_data3 = lv_label_create(scr1, NULL);
	label_data4 = lv_label_create(scr1, NULL);
	
	/* 设置标签位置 */
	lv_obj_align(label0, NULL, LV_ALIGN_CENTER, -72, -4 * LV_VER_RES_MAX / 8 + 5);
	lv_obj_align(label_time, NULL, LV_ALIGN_CENTER, -72, -3 * LV_VER_RES_MAX / 8 + 5);
	lv_obj_align(label1, NULL, LV_ALIGN_CENTER, -LV_HOR_RES_MAX / 3, -3 * LV_VER_RES_MAX / 8 + 5);
	lv_obj_align(label2, NULL, LV_ALIGN_CENTER, -LV_HOR_RES_MAX / 3, -2 * LV_VER_RES_MAX / 8 + 5);
	lv_obj_align(label3, NULL, LV_ALIGN_CENTER, -LV_HOR_RES_MAX / 3, -1 * LV_VER_RES_MAX / 8 + 5);
	lv_obj_align(label4, NULL, LV_ALIGN_CENTER, -LV_HOR_RES_MAX / 3, 0);
	
	lv_obj_align(label_data1, NULL, LV_ALIGN_CENTER, LV_HOR_RES_MAX / 3, -3 * LV_VER_RES_MAX / 8 + 5);
	lv_obj_align(label_data2, NULL, LV_ALIGN_CENTER, LV_HOR_RES_MAX / 3, -2 * LV_VER_RES_MAX / 8 + 5);
	lv_obj_align(label_data3, NULL, LV_ALIGN_CENTER, LV_HOR_RES_MAX / 3, -1 * LV_VER_RES_MAX / 8 + 5);
	lv_obj_align(label_data4, NULL, LV_ALIGN_CENTER, LV_HOR_RES_MAX / 3, 0);

	//设置风格
	lv_obj_add_style(label0, LV_LABEL_PART_MAIN, &label_shadow_style);
	lv_obj_add_style(label_time, LV_LABEL_PART_MAIN, &label_shadow_style);
	lv_obj_add_style(label1, LV_LABEL_PART_MAIN, &label_shadow_style);
	lv_obj_add_style(label2, LV_LABEL_PART_MAIN, &label_shadow_style);
	lv_obj_add_style(label3, LV_LABEL_PART_MAIN, &label_shadow_style);
	lv_obj_add_style(label4, LV_LABEL_PART_MAIN, &label_shadow_style);
	
	lv_obj_add_style(label_data1, LV_LABEL_PART_MAIN, &label_shadow_style);
	lv_obj_add_style(label_data2, LV_LABEL_PART_MAIN, &label_shadow_style);
	lv_obj_add_style(label_data3, LV_LABEL_PART_MAIN, &label_shadow_style);
	lv_obj_add_style(label_data4, LV_LABEL_PART_MAIN, &label_shadow_style);
}

//显示数据
void font_test(void)
{
	char data_str[32] = {0};
	
	//显示文本
	lv_label_set_text(label0, "绌烘皵璐ㄩ噺妫�娴嬩华");
	lv_label_set_text(label1, "娓╁害");
	lv_label_set_text(label2, "婀垮害");
	lv_label_set_text(label3, "浜屾哀鍖栫⒊");
	lv_label_set_text(label4, "鐢查啗");
	
	lv_label_set_text_fmt(label_data1, "%d.%d", dht_data.tmp, dht_data.tmp_flo);	//温度
	lv_label_set_text_fmt(label_data2, "%d", dht_data.hum);	//湿度
	sprintf(data_str, "%.2f", (float)kqm_data.CO2);
	lv_label_set_text_fmt(label_data3, "%s", data_str);	//二氧化碳
	sprintf(data_str, "%.2f", (float)kqm_data.CHO);
	lv_label_set_text_fmt(label_data4, "%s", data_str);	//甲烷


//void lvgl_show_data_lcd(void){
//	
//}


}

uint8_t lvgl_flag = 0;

void my_event_cb(lv_obj_t * obj, lv_event_t event)
{
//	unsigned char time_str[64] = {0};
	if(obj == btn1){
		printf("按键1 Pressed\r\n");
		if(event == LV_EVENT_PRESSED){

			lvgl_flag = 1;
		}
	}
	else if(obj == btn2){
		printf("按键2 Pressed\r\n");
		if(event == LV_EVENT_PRESSED){

			lvgl_flag = 2;
		}
	}
  
}

void btn_event_test(void)
{
	btn1 = lv_btn_create(scr1, NULL);
	btn2 = lv_btn_create(scr1, NULL);
  
}


extern lv_ui_2 test2;

  // 创建一个静态数组，包含四张图片的指针
   static const lv_img_dsc_t *images[4] = {
		&screen_3_animimg_3_1w1,
		&screen_3_animimg_3_1w2,
		&screen_3_animimg_3_1w3,
		&screen_3_animimg_3_1w4
    };


		
		
#define ANIMATION_DELAY 500 // 动画延迟时间（ms）

lv_obj_t *img; // 图片对象

void animation_task(lv_task_t *task) {
    static uint8_t frame = 0;

    // 根据当前帧数更新图片
    lv_img_set_src(img, images[frame]);

    // 更新帧数
    frame = (frame + 1) % 4;
}

void setup_animation() {
    // 创建一个图片对象
    img = lv_img_create(test2.screen_3, NULL);
	lv_obj_set_pos(img, 100, 80);

    // 设置图片的源
    lv_img_set_src(img, images[0]);

    // 创建一个定时器，用于定时更新动画帧
    lv_task_create(animation_task, ANIMATION_DELAY, LV_TASK_PRIO_LOW, NULL);
}




//+IPD,0,85:{"wifiname":"miumiuyao","password":"123456789","mycity":"","alarm_h":"","alarm_m":""}
/**
  * @brief  wifi配置函数
  * @param  None
  * @retval None
  */

void wifi_set(void){
	uint8_t key_val = 0;
	printf("\r\n开始设置网络\r\n");
//	lv_scr_load(test2.screen_2);
	lv_scr_load(test2.screen_2);//加载屏幕
	printf("\r\n屏幕2加载成功\r\n");
	
	lv_obj_set_event_cb(test2.screen_2_btn2_1, my_event_cb);  //设置按钮的回调函数
	lv_obj_set_event_cb(test2.screen_2_btn_2_2, my_event_cb);  //设置按钮的回调函数
	
	while(1){
		key_val = key_get_num();
		if(key_val == 1){
//			lv_event_send(test2.screen_2_btn2_1, LV_EVENT_PRESSED, NULL);
				lv_obj_set_style_local_bg_color(test2.screen_2_btn2_1, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, lv_color_make(0x28, 0xa6, 0xaf));

			break;
		}
		else if(key_val == 2){
//			lv_event_send(test2.screen_2_btn_2_2, LV_EVENT_PRESSED, NULL);
				lv_obj_set_style_local_bg_color(test2.screen_2_btn_2_2, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, lv_color_make(0x28, 0xa6, 0xaf));

			break;
		}
	}
	
	lv_scr_load(test2.screen_3);
	setup_animation();
	
	ap_init();	//开启AP模式
	
}










