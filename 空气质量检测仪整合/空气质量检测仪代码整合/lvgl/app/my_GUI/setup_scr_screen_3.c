/*
 * Copyright 2023 NXP
 * SPDX-License-Identifier: MIT
 */

#include "lvgl.h"
#include <stdio.h>
#include "gui_guider.h"
#include "events_init.h"


//static const lv_img_dsc_t* screen_3_animimg_3_1_imgs[4] = {
//	&screen_3_animimg_3_1w1,
//	&screen_3_animimg_3_1w2,
//	&screen_3_animimg_3_1w3,
//	&screen_3_animimg_3_1w4
//};

void setup_scr_screen_3(lv_ui_2 *ui){
	//设置字体
	LV_FONT_DECLARE(CH_font24);

	//Write codes screen_3
	ui->screen_3 = lv_obj_create(NULL, NULL);

	//Write style LV_OBJ_PART_MAIN for screen_3
	static lv_style_t style_screen_3_main;
	lv_style_reset(&style_screen_3_main);

	//Write style state: LV_STATE_DEFAULT for style_screen_3_main
	lv_style_set_bg_color(&style_screen_3_main, LV_STATE_DEFAULT, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_bg_opa(&style_screen_3_main, LV_STATE_DEFAULT, 0);
	lv_obj_add_style(ui->screen_3, LV_OBJ_PART_MAIN, &style_screen_3_main);

	//Write codes screen_3_animimg_3_1
//	ui->screen_3_animimg_3_1 = lv_animimg_create(ui->screen_3, NULL);
//	lv_obj_set_pos(ui->screen_3_animimg_3_1, 106, 77);
//	lv_obj_set_size(ui->screen_3_animimg_3_1, 100, 100);
//	lv_set_anim_img_sources(ui->screen_3_animimg_3_1, (lv_img_dsc_t**) screen_3_animimg_3_1_imgs);
//	lv_set_anim_img_duration(ui->screen_3_animimg_3_1, 2000);
//	lv_set_anim_img_numbers(ui->screen_3_animimg_3_1, 4);
//	lv_set_anim_img_playback_time(ui->screen_3_animimg_3_1, 2000);
//	lv_set_anim_img_startup_repeat_count(ui->screen_3_animimg_3_1, 3000);
//	lv_anim_img_startup(ui->screen_3_animimg_3_1);

////Write codes screen_img_1
//	ui->screen_3_img_1 = lv_img_create(ui->screen_3, NULL);

//	//Write style LV_IMG_PART_MAIN for screen_img_1
//	static lv_style_t style_screen_img_main;
//	lv_style_reset(&style_screen_img_main);

//	//Write style state: LV_STATE_DEFAULT for style_screen_img_1_main
//	lv_style_set_image_recolor(&style_screen_img_main, LV_STATE_DEFAULT, lv_color_make(0xff, 0xff, 0xff));
//	lv_style_set_image_recolor_opa(&style_screen_img_main, LV_STATE_DEFAULT, 0);
//	lv_style_set_image_opa(&style_screen_img_main, LV_STATE_DEFAULT, 255);
//	lv_obj_add_style(ui->screen_3_img_1, LV_IMG_PART_MAIN, &style_screen_img_main);
//	lv_obj_set_pos(ui->screen_3_img_1, 20, 30);
//	lv_obj_set_size(ui->screen_3_img_1, 50, 50);
//	lv_obj_set_click(ui->screen_3_img_1, true);
//	lv_img_set_src(ui->screen_3_img_1,&screen_3_animimg_3_1w1);
//	lv_img_set_pivot(ui->screen_3_img_1, 0,0);
//	lv_img_set_angle(ui->screen_3_img_1, 0);



	//Write codes screen_3_label_3_1
	ui->screen_3_label_3_1 = lv_label_create(ui->screen_3, NULL);
	lv_label_set_text(ui->screen_3_label_3_1, "请通过手机设置网络");
	lv_label_set_long_mode(ui->screen_3_label_3_1, LV_LABEL_LONG_BREAK);
	lv_label_set_align(ui->screen_3_label_3_1, LV_LABEL_ALIGN_CENTER);

	//Write style LV_LABEL_PART_MAIN for screen_3_label_3_1
	static lv_style_t style_screen_3_label_3_1_main;
	lv_style_reset(&style_screen_3_label_3_1_main);

	//Write style state: LV_STATE_DEFAULT for style_screen_3_label_3_1_main
	lv_style_set_radius(&style_screen_3_label_3_1_main, LV_STATE_DEFAULT, 0);
	lv_style_set_bg_color(&style_screen_3_label_3_1_main, LV_STATE_DEFAULT, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_bg_grad_color(&style_screen_3_label_3_1_main, LV_STATE_DEFAULT, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_bg_grad_dir(&style_screen_3_label_3_1_main, LV_STATE_DEFAULT, LV_GRAD_DIR_VER);
	lv_style_set_bg_opa(&style_screen_3_label_3_1_main, LV_STATE_DEFAULT, 0);
	lv_style_set_text_color(&style_screen_3_label_3_1_main, LV_STATE_DEFAULT, lv_color_make(0x00, 0x00, 0x00));
	lv_style_set_text_font(&style_screen_3_label_3_1_main, LV_STATE_DEFAULT, &CH_font24);
	lv_style_set_text_letter_space(&style_screen_3_label_3_1_main, LV_STATE_DEFAULT, 2);
	lv_style_set_pad_left(&style_screen_3_label_3_1_main, LV_STATE_DEFAULT, 0);
	lv_style_set_pad_right(&style_screen_3_label_3_1_main, LV_STATE_DEFAULT, 0);
	lv_style_set_pad_top(&style_screen_3_label_3_1_main, LV_STATE_DEFAULT, 0);
	lv_style_set_pad_bottom(&style_screen_3_label_3_1_main, LV_STATE_DEFAULT, 0);
	lv_obj_add_style(ui->screen_3_label_3_1, LV_LABEL_PART_MAIN, &style_screen_3_label_3_1_main);
	lv_obj_set_pos(ui->screen_3_label_3_1, 59, 33);
	lv_obj_set_size(ui->screen_3_label_3_1, 200, 0);

	//Write codes screen_3_btn_3_1
	ui->screen_3_btn_3_1 = lv_btn_create(ui->screen_3, NULL);

	//Write style LV_BTN_PART_MAIN for screen_3_btn_3_1
	static lv_style_t style_screen_3_btn_3_1_main;
	lv_style_reset(&style_screen_3_btn_3_1_main);

	//Write style state: LV_STATE_DEFAULT for style_screen_3_btn_3_1_main
	lv_style_set_radius(&style_screen_3_btn_3_1_main, LV_STATE_DEFAULT, 50);
	lv_style_set_bg_color(&style_screen_3_btn_3_1_main, LV_STATE_DEFAULT, lv_color_make(0x32, 0x87, 0xe2));
	lv_style_set_bg_grad_color(&style_screen_3_btn_3_1_main, LV_STATE_DEFAULT, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_bg_grad_dir(&style_screen_3_btn_3_1_main, LV_STATE_DEFAULT, LV_GRAD_DIR_VER);
	lv_style_set_bg_opa(&style_screen_3_btn_3_1_main, LV_STATE_DEFAULT, 255);
	lv_style_set_border_color(&style_screen_3_btn_3_1_main, LV_STATE_DEFAULT, lv_color_make(0x01, 0xa2, 0xb1));
	lv_style_set_border_width(&style_screen_3_btn_3_1_main, LV_STATE_DEFAULT, 2);
	lv_style_set_border_opa(&style_screen_3_btn_3_1_main, LV_STATE_DEFAULT, 255);
	lv_style_set_outline_color(&style_screen_3_btn_3_1_main, LV_STATE_DEFAULT, lv_color_make(0xd4, 0xd7, 0xd9));
	lv_style_set_outline_opa(&style_screen_3_btn_3_1_main, LV_STATE_DEFAULT, 255);
	lv_obj_add_style(ui->screen_3_btn_3_1, LV_BTN_PART_MAIN, &style_screen_3_btn_3_1_main);
	lv_obj_set_pos(ui->screen_3_btn_3_1, 203, 160);
	lv_obj_set_size(ui->screen_3_btn_3_1, 100, 50);
	ui->screen_3_btn_3_1_label = lv_label_create(ui->screen_3_btn_3_1, NULL);
	lv_label_set_text(ui->screen_3_btn_3_1_label, "按键2 取消");
	lv_obj_set_style_local_text_color(ui->screen_3_btn_3_1_label, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, lv_color_make(0x00, 0x00, 0x00));
	lv_obj_set_style_local_text_font(ui->screen_3_btn_3_1_label, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, &CH_font24);
}