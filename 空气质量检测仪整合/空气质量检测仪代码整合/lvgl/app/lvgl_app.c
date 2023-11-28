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

//������ʾ��ʼ��
void lvgl_show_data_init(void){
		LV_FONT_DECLARE(CH_font24);
	/* Create a style for the shadow*/
	static lv_style_t label_shadow_style; //����һ����ʽ����
	lv_style_init(&label_shadow_style);//��ʼ��
	lv_style_set_text_opa(&label_shadow_style, LV_STATE_DEFAULT, LV_OPA_50);//������ʽ͸����
	lv_style_set_text_color(&label_shadow_style, LV_STATE_DEFAULT, LV_COLOR_RED);//��ɫ
	lv_style_set_text_font(&label_shadow_style, LV_STATE_DEFAULT, &CH_font24);
	
	//������ǩ
	scr1 = lv_obj_create(NULL, NULL);//������1����Ļ
	scr2 = lv_obj_create(NULL, NULL);//������2����Ļ
	
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
	
	/* ���ñ�ǩλ�� */
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

	//���÷��
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

//��ʾ����
void font_test(void)
{
	char data_str[32] = {0};
	
	//��ʾ�ı�
	lv_label_set_text(label0, "空气质量检测仪");
	lv_label_set_text(label1, "温度");
	lv_label_set_text(label2, "湿度");
	lv_label_set_text(label3, "二氧化碳");
	lv_label_set_text(label4, "甲醛");
	
	lv_label_set_text_fmt(label_data1, "%d.%d", dht_data.tmp, dht_data.tmp_flo);	//�¶�
	lv_label_set_text_fmt(label_data2, "%d", dht_data.hum);	//ʪ��
	sprintf(data_str, "%.2f", (float)kqm_data.CO2);
	lv_label_set_text_fmt(label_data3, "%s", data_str);	//������̼
	sprintf(data_str, "%.2f", (float)kqm_data.CHO);
	lv_label_set_text_fmt(label_data4, "%s", data_str);	//����


//void lvgl_show_data_lcd(void){
//	
//}


}

uint8_t lvgl_flag = 0;

void my_event_cb(lv_obj_t * obj, lv_event_t event)
{
//	unsigned char time_str[64] = {0};
	if(obj == btn1){
		printf("����1 Pressed\r\n");
		if(event == LV_EVENT_PRESSED){

			lvgl_flag = 1;
		}
	}
	else if(obj == btn2){
		printf("����2 Pressed\r\n");
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

  // ����һ����̬���飬��������ͼƬ��ָ��
   static const lv_img_dsc_t *images[4] = {
		&screen_3_animimg_3_1w1,
		&screen_3_animimg_3_1w2,
		&screen_3_animimg_3_1w3,
		&screen_3_animimg_3_1w4
    };


		
		
#define ANIMATION_DELAY 500 // �����ӳ�ʱ�䣨ms��

lv_obj_t *img; // ͼƬ����

void animation_task(lv_task_t *task) {
    static uint8_t frame = 0;

    // ���ݵ�ǰ֡������ͼƬ
    lv_img_set_src(img, images[frame]);

    // ����֡��
    frame = (frame + 1) % 4;
}

void setup_animation() {
    // ����һ��ͼƬ����
    img = lv_img_create(test2.screen_3, NULL);
	lv_obj_set_pos(img, 100, 80);

    // ����ͼƬ��Դ
    lv_img_set_src(img, images[0]);

    // ����һ����ʱ�������ڶ�ʱ���¶���֡
    lv_task_create(animation_task, ANIMATION_DELAY, LV_TASK_PRIO_LOW, NULL);
}




//+IPD,0,85:{"wifiname":"miumiuyao","password":"123456789","mycity":"","alarm_h":"","alarm_m":""}
/**
  * @brief  wifi���ú���
  * @param  None
  * @retval None
  */

void wifi_set(void){
	uint8_t key_val = 0;
	printf("\r\n��ʼ��������\r\n");
//	lv_scr_load(test2.screen_2);
	lv_scr_load(test2.screen_2);//������Ļ
	printf("\r\n��Ļ2���سɹ�\r\n");
	
	lv_obj_set_event_cb(test2.screen_2_btn2_1, my_event_cb);  //���ð�ť�Ļص�����
	lv_obj_set_event_cb(test2.screen_2_btn_2_2, my_event_cb);  //���ð�ť�Ļص�����
	
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
	
	ap_init();	//����APģʽ
	
}










