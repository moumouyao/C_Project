/*
 * Copyright 2023 NXP
 * SPDX-License-Identifier: MIT
 */

#include "lvgl.h"
#include <stdio.h>
#include "gui_guider.h"
#include "events_init.h"
#include "kqm.h"
#include "kqm.h"
#include "dht11.h"
#include "ADC.h"
//#include "custom.h"


void setup_scr_screen(lv_ui *ui){
	//设置字体
	LV_FONT_DECLARE(CH_font24);
	//Write codes screen
	ui->screen = lv_obj_create(NULL, NULL);

	//Write style LV_OBJ_PART_MAIN for screen
	static lv_style_t style_screen_main;
	lv_style_reset(&style_screen_main);

	//Write style state: LV_STATE_DEFAULT for style_screen_main
	lv_style_set_bg_color(&style_screen_main, LV_STATE_DEFAULT, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_bg_opa(&style_screen_main, LV_STATE_DEFAULT, 0);
	//lv_style_set_text_font(&style_screen_main, LV_STATE_DEFAULT, &CH_font24);	//设置字库
	
	lv_obj_add_style(ui->screen, LV_OBJ_PART_MAIN, &style_screen_main);
	lv_anim_path_t lv_anim_path_screen_tabview_1 = {.cb = lv_anim_path_ease_in};

	//Write codes screen_tabview_1
	ui->screen_tabview_1 = lv_tabview_create(ui->screen, NULL);

//	static lv_style_t label_title_style; //创建一个样式变量
//	lv_style_init(&label_title_style);//初始化初始化
//	lv_style_set_text_font(&label_title_style, LV_STATE_DEFAULT, &CH_font24);
//	lv_style_set_text_color(&label_title_style, LV_STATE_DEFAULT, LV_COLOR_RED);//红色
//	//title1
//	lv_obj_add_style(ui->screen_tabview_1, LV_OBJ_PART_MAIN, &label_title_style);
	ui->screen_tabview_1_title1 = lv_tabview_add_tab(ui->screen_tabview_1,"室内空气");
	
	
	//title2
	ui->screen_tabview_1_title2 = lv_tabview_add_tab(ui->screen_tabview_1,"天气");

	//title3
	ui->screen_tabview_1_title3 = lv_tabview_add_tab(ui->screen_tabview_1,"日期");

	lv_tabview_set_anim_time(ui->screen_tabview_1, 1000);

	//Write style LV_TABVIEW_PART_BG for screen_tabview_1
	static lv_style_t style_screen_tabview_1_bg;
	lv_style_reset(&style_screen_tabview_1_bg);

	//Write style state: LV_STATE_DEFAULT for style_screen_tabview_1_bg
	lv_style_set_bg_color(&style_screen_tabview_1_bg, LV_STATE_DEFAULT, lv_color_make(0x1e, 0x86, 0xdc));
	lv_style_set_bg_grad_color(&style_screen_tabview_1_bg, LV_STATE_DEFAULT, lv_color_make(0xea, 0xef, 0xf3));
	lv_style_set_bg_grad_dir(&style_screen_tabview_1_bg, LV_STATE_DEFAULT, LV_GRAD_DIR_VER);
	lv_style_set_bg_opa(&style_screen_tabview_1_bg, LV_STATE_DEFAULT, 255);
	lv_style_set_border_color(&style_screen_tabview_1_bg, LV_STATE_DEFAULT, lv_color_make(0xc0, 0xc0, 0xc0));
	lv_style_set_border_width(&style_screen_tabview_1_bg, LV_STATE_DEFAULT, 0);
	lv_style_set_text_color(&style_screen_tabview_1_bg, LV_STATE_DEFAULT, lv_color_make(0x4d, 0x4d, 0x4d));
	lv_style_set_text_font(&style_screen_tabview_1_bg, LV_STATE_DEFAULT, &CH_font24);
	lv_style_set_text_letter_space(&style_screen_tabview_1_bg, LV_STATE_DEFAULT, 2);
	lv_style_set_text_line_space(&style_screen_tabview_1_bg, LV_STATE_DEFAULT, 16);
	lv_obj_add_style(ui->screen_tabview_1, LV_TABVIEW_PART_BG, &style_screen_tabview_1_bg);

	//Write style LV_TABVIEW_PART_INDIC for screen_tabview_1
	static lv_style_t style_screen_tabview_1_indic;
	lv_style_reset(&style_screen_tabview_1_indic);

	//Write style state: LV_STATE_DEFAULT for style_screen_tabview_1_indic
	lv_style_set_bg_color(&style_screen_tabview_1_indic, LV_STATE_DEFAULT, lv_color_make(0x01, 0xa2, 0xb1));
	lv_style_set_bg_grad_color(&style_screen_tabview_1_indic, LV_STATE_DEFAULT, lv_color_make(0x01, 0xa2, 0xb1));
	lv_style_set_bg_grad_dir(&style_screen_tabview_1_indic, LV_STATE_DEFAULT, LV_GRAD_DIR_VER);
	lv_style_set_bg_opa(&style_screen_tabview_1_indic, LV_STATE_DEFAULT, 255);
	lv_obj_add_style(ui->screen_tabview_1, LV_TABVIEW_PART_INDIC, &style_screen_tabview_1_indic);

	//Write style LV_TABVIEW_PART_TAB_BG for screen_tabview_1
	static lv_style_t style_screen_tabview_1_tab_bg;
	lv_style_reset(&style_screen_tabview_1_tab_bg);

	//Write style state: LV_STATE_DEFAULT for style_screen_tabview_1_tab_bg
	lv_style_set_bg_color(&style_screen_tabview_1_tab_bg, LV_STATE_DEFAULT, lv_color_make(0x18, 0x9a, 0x75));
	lv_style_set_bg_grad_color(&style_screen_tabview_1_tab_bg, LV_STATE_DEFAULT, lv_color_make(0x6a, 0xf7, 0x36));
	lv_style_set_bg_grad_dir(&style_screen_tabview_1_tab_bg, LV_STATE_DEFAULT, LV_GRAD_DIR_VER);
	lv_style_set_bg_opa(&style_screen_tabview_1_tab_bg, LV_STATE_DEFAULT, 150);
	lv_style_set_border_color(&style_screen_tabview_1_tab_bg, LV_STATE_DEFAULT, lv_color_make(0xc0, 0xc0, 0xc0));
	lv_style_set_border_width(&style_screen_tabview_1_tab_bg, LV_STATE_DEFAULT, 0);
	lv_style_set_pad_left(&style_screen_tabview_1_tab_bg, LV_STATE_DEFAULT, 0);
	lv_style_set_pad_right(&style_screen_tabview_1_tab_bg, LV_STATE_DEFAULT, 0);
	lv_style_set_pad_top(&style_screen_tabview_1_tab_bg, LV_STATE_DEFAULT, 0);
	lv_style_set_pad_bottom(&style_screen_tabview_1_tab_bg, LV_STATE_DEFAULT, 0);
	lv_obj_add_style(ui->screen_tabview_1, LV_TABVIEW_PART_TAB_BG, &style_screen_tabview_1_tab_bg);

	//Write style LV_TABVIEW_PART_TAB_BTN for screen_tabview_1
	static lv_style_t style_screen_tabview_1_tab_btn;
	lv_style_reset(&style_screen_tabview_1_tab_btn);

	//Write style state: LV_STATE_DEFAULT for style_screen_tabview_1_tab_btn
	lv_style_set_text_color(&style_screen_tabview_1_tab_btn, LV_STATE_DEFAULT, lv_color_make(0x4d, 0x4d, 0x4d));
	lv_style_set_text_letter_space(&style_screen_tabview_1_tab_btn, LV_STATE_DEFAULT, 0);
	lv_obj_add_style(ui->screen_tabview_1, LV_TABVIEW_PART_TAB_BTN, &style_screen_tabview_1_tab_btn);
	lv_obj_set_pos(ui->screen_tabview_1, 0, 0);
	lv_obj_set_size(ui->screen_tabview_1, 320, 240);

	//Write codes screen_tabview_1_title1

	//Write codes screen_img_1
	ui->screen_img_1 = lv_img_create(ui->screen_tabview_1_title1, NULL);

	//Write style LV_IMG_PART_MAIN for screen_img_1
	static lv_style_t style_screen_img_1_main;
	lv_style_reset(&style_screen_img_1_main);

	//Write style state: LV_STATE_DEFAULT for style_screen_img_1_main
	lv_style_set_image_recolor(&style_screen_img_1_main, LV_STATE_DEFAULT, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_image_recolor_opa(&style_screen_img_1_main, LV_STATE_DEFAULT, 0);
	lv_style_set_image_opa(&style_screen_img_1_main, LV_STATE_DEFAULT, 255);
	lv_obj_add_style(ui->screen_img_1, LV_IMG_PART_MAIN, &style_screen_img_1_main);
	lv_obj_set_pos(ui->screen_img_1, 20, 30);
	lv_obj_set_size(ui->screen_img_1, 50, 50);
	lv_obj_set_click(ui->screen_img_1, true);
	lv_img_set_src(ui->screen_img_1,&_5_alpha_50x50);
	lv_img_set_pivot(ui->screen_img_1, 0,0);
	lv_img_set_angle(ui->screen_img_1, 0);

	//Write codes screen_img_6
	ui->screen_img_6 = lv_img_create(ui->screen_tabview_1_title1, NULL);

	//Write style LV_IMG_PART_MAIN for screen_img_6
	static lv_style_t style_screen_img_6_main;
	lv_style_reset(&style_screen_img_6_main);

	//Write style state: LV_STATE_DEFAULT for style_screen_img_6_main
	lv_style_set_image_recolor(&style_screen_img_6_main, LV_STATE_DEFAULT, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_image_recolor_opa(&style_screen_img_6_main, LV_STATE_DEFAULT, 0);
	lv_style_set_image_opa(&style_screen_img_6_main, LV_STATE_DEFAULT, 255);
	lv_obj_add_style(ui->screen_img_6, LV_IMG_PART_MAIN, &style_screen_img_6_main);
	lv_obj_set_pos(ui->screen_img_6, 11, 7.5);
	lv_obj_set_size(ui->screen_img_6, 20, 20);
	lv_obj_set_click(ui->screen_img_6, true);
	lv_img_set_src(ui->screen_img_6,&_7_alpha_20x20);
	lv_img_set_pivot(ui->screen_img_6, 0,0);
	lv_img_set_angle(ui->screen_img_6, 0);

	//Write codes screen_label_5
	ui->screen_label_5 = lv_label_create(ui->screen_tabview_1_title1, NULL);
	lv_label_set_text(ui->screen_label_5, "2023-10-19 19：54：26");
	lv_label_set_long_mode(ui->screen_label_5, LV_LABEL_LONG_BREAK);
	lv_label_set_align(ui->screen_label_5, LV_LABEL_ALIGN_CENTER);

	//Write style LV_LABEL_PART_MAIN for screen_label_5
	static lv_style_t style_screen_label_5_main;
	lv_style_reset(&style_screen_label_5_main);

	//Write style state: LV_STATE_DEFAULT for style_screen_label_5_main
	lv_style_set_radius(&style_screen_label_5_main, LV_STATE_DEFAULT, 0);
	lv_style_set_bg_color(&style_screen_label_5_main, LV_STATE_DEFAULT, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_bg_grad_color(&style_screen_label_5_main, LV_STATE_DEFAULT, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_bg_grad_dir(&style_screen_label_5_main, LV_STATE_DEFAULT, LV_GRAD_DIR_VER);
	lv_style_set_bg_opa(&style_screen_label_5_main, LV_STATE_DEFAULT, 0);
	lv_style_set_text_color(&style_screen_label_5_main, LV_STATE_DEFAULT, lv_color_make(0x00, 0x00, 0x00));
	lv_style_set_text_font(&style_screen_label_5_main, LV_STATE_DEFAULT, &lv_font_simsun_12);
	lv_style_set_text_letter_space(&style_screen_label_5_main, LV_STATE_DEFAULT, 2);
	lv_style_set_pad_left(&style_screen_label_5_main, LV_STATE_DEFAULT, 0);
	lv_style_set_pad_right(&style_screen_label_5_main, LV_STATE_DEFAULT, 0);
	lv_style_set_pad_top(&style_screen_label_5_main, LV_STATE_DEFAULT, 0);
	lv_style_set_pad_bottom(&style_screen_label_5_main, LV_STATE_DEFAULT, 0);
	lv_obj_add_style(ui->screen_label_5, LV_LABEL_PART_MAIN, &style_screen_label_5_main);
	lv_obj_set_pos(ui->screen_label_5, 65, 10);
	lv_obj_set_size(ui->screen_label_5, 200, 0);

	//Write codes screen_label_1
	ui->screen_label_1 = lv_label_create(ui->screen_tabview_1_title1, NULL);
	//lv_label_set_text(ui->screen_label_1, "33");
	lv_label_set_text_fmt(ui->screen_label_1, "%d.%d", dht_data.tmp, dht_data.tmp_flo);	//温度
	lv_label_set_long_mode(ui->screen_label_1, LV_LABEL_LONG_BREAK);
	lv_label_set_align(ui->screen_label_1, LV_LABEL_ALIGN_CENTER);

	//Write style LV_LABEL_PART_MAIN for screen_label_1
	static lv_style_t style_screen_label_1_main;
	lv_style_reset(&style_screen_label_1_main);

	//Write style state: LV_STATE_DEFAULT for style_screen_label_1_main
	lv_style_set_radius(&style_screen_label_1_main, LV_STATE_DEFAULT, 0);
	lv_style_set_bg_color(&style_screen_label_1_main, LV_STATE_DEFAULT, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_bg_grad_color(&style_screen_label_1_main, LV_STATE_DEFAULT, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_bg_grad_dir(&style_screen_label_1_main, LV_STATE_DEFAULT, LV_GRAD_DIR_VER);
	lv_style_set_bg_opa(&style_screen_label_1_main, LV_STATE_DEFAULT, 0);
	lv_style_set_text_color(&style_screen_label_1_main, LV_STATE_DEFAULT, lv_color_make(0x00, 0x00, 0x00));
	lv_style_set_text_font(&style_screen_label_1_main, LV_STATE_DEFAULT, &lv_font_simsun_20);
	lv_style_set_text_letter_space(&style_screen_label_1_main, LV_STATE_DEFAULT, 2);
	lv_style_set_pad_left(&style_screen_label_1_main, LV_STATE_DEFAULT, 0);
	lv_style_set_pad_right(&style_screen_label_1_main, LV_STATE_DEFAULT, 0);
	lv_style_set_pad_top(&style_screen_label_1_main, LV_STATE_DEFAULT, 0);
	lv_style_set_pad_bottom(&style_screen_label_1_main, LV_STATE_DEFAULT, 0);
	lv_obj_add_style(ui->screen_label_1, LV_LABEL_PART_MAIN, &style_screen_label_1_main);
	lv_obj_set_pos(ui->screen_label_1, 80, 50);
	lv_obj_set_size(ui->screen_label_1, 80, 0);

	//Write codes screen_label_2
	ui->screen_label_2 = lv_label_create(ui->screen_tabview_1_title1, NULL);
//	lv_label_set_text(ui->screen_label_2, "33");
	lv_label_set_text_fmt(ui->screen_label_2, "%d", dht_data.hum);	//湿度
	lv_label_set_long_mode(ui->screen_label_2, LV_LABEL_LONG_BREAK);
	lv_label_set_align(ui->screen_label_2, LV_LABEL_ALIGN_CENTER);

	//Write style LV_LABEL_PART_MAIN for screen_label_2
	static lv_style_t style_screen_label_2_main;
	lv_style_reset(&style_screen_label_2_main);

	//Write style state: LV_STATE_DEFAULT for style_screen_label_2_main
	lv_style_set_radius(&style_screen_label_2_main, LV_STATE_DEFAULT, 0);
	lv_style_set_bg_color(&style_screen_label_2_main, LV_STATE_DEFAULT, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_bg_grad_color(&style_screen_label_2_main, LV_STATE_DEFAULT, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_bg_grad_dir(&style_screen_label_2_main, LV_STATE_DEFAULT, LV_GRAD_DIR_VER);
	lv_style_set_bg_opa(&style_screen_label_2_main, LV_STATE_DEFAULT, 0);
	lv_style_set_text_color(&style_screen_label_2_main, LV_STATE_DEFAULT, lv_color_make(0x00, 0x00, 0x00));
	lv_style_set_text_font(&style_screen_label_2_main, LV_STATE_DEFAULT, &lv_font_simsun_20);
	lv_style_set_text_letter_space(&style_screen_label_2_main, LV_STATE_DEFAULT, 2);
	lv_style_set_pad_left(&style_screen_label_2_main, LV_STATE_DEFAULT, 0);
	lv_style_set_pad_right(&style_screen_label_2_main, LV_STATE_DEFAULT, 0);
	lv_style_set_pad_top(&style_screen_label_2_main, LV_STATE_DEFAULT, 0);
	lv_style_set_pad_bottom(&style_screen_label_2_main, LV_STATE_DEFAULT, 0);
	lv_obj_add_style(ui->screen_label_2, LV_LABEL_PART_MAIN, &style_screen_label_2_main);
	lv_obj_set_pos(ui->screen_label_2, 220, 50);
	lv_obj_set_size(ui->screen_label_2, 80, 0);

	char data_str[32] = {0};
	//Write codes screen_label_3
	ui->screen_label_3 = lv_label_create(ui->screen_tabview_1_title1, NULL);
//	lv_label_set_text(ui->screen_label_3, "33");
	sprintf(data_str, "%.2f", (float)kqm_data.CHO);
	lv_label_set_text_fmt(ui->screen_label_3, "%s", data_str);	//甲烷
	lv_label_set_long_mode(ui->screen_label_3, LV_LABEL_LONG_BREAK);
	lv_label_set_align(ui->screen_label_3, LV_LABEL_ALIGN_CENTER);

	//Write style LV_LABEL_PART_MAIN for screen_label_3
	static lv_style_t style_screen_label_3_main;
	lv_style_reset(&style_screen_label_3_main);

	//Write style state: LV_STATE_DEFAULT for style_screen_label_3_main
	lv_style_set_radius(&style_screen_label_3_main, LV_STATE_DEFAULT, 0);
	lv_style_set_bg_color(&style_screen_label_3_main, LV_STATE_DEFAULT, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_bg_grad_color(&style_screen_label_3_main, LV_STATE_DEFAULT, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_bg_grad_dir(&style_screen_label_3_main, LV_STATE_DEFAULT, LV_GRAD_DIR_VER);
	lv_style_set_bg_opa(&style_screen_label_3_main, LV_STATE_DEFAULT, 0);
	lv_style_set_text_color(&style_screen_label_3_main, LV_STATE_DEFAULT, lv_color_make(0x00, 0x00, 0x00));
	lv_style_set_text_font(&style_screen_label_3_main, LV_STATE_DEFAULT, &lv_font_simsun_20);
	lv_style_set_text_letter_space(&style_screen_label_3_main, LV_STATE_DEFAULT, 2);
	lv_style_set_pad_left(&style_screen_label_3_main, LV_STATE_DEFAULT, 0);
	lv_style_set_pad_right(&style_screen_label_3_main, LV_STATE_DEFAULT, 0);
	lv_style_set_pad_top(&style_screen_label_3_main, LV_STATE_DEFAULT, 0);
	lv_style_set_pad_bottom(&style_screen_label_3_main, LV_STATE_DEFAULT, 0);
	lv_obj_add_style(ui->screen_label_3, LV_LABEL_PART_MAIN, &style_screen_label_3_main);
	lv_obj_set_pos(ui->screen_label_3, 80, 120);
	lv_obj_set_size(ui->screen_label_3, 80, 0);

	//Write codes screen_label_4
	ui->screen_label_4 = lv_label_create(ui->screen_tabview_1_title1, NULL);
//	lv_label_set_text(ui->screen_label_4, "33");
	printf(data_str, "%.2f", (float)kqm_data.CO2);
	lv_label_set_text_fmt(ui->screen_label_4, "%s", data_str);	//二氧化碳
	lv_label_set_long_mode(ui->screen_label_4, LV_LABEL_LONG_BREAK);
	lv_label_set_align(ui->screen_label_4, LV_LABEL_ALIGN_CENTER);

	//Write style LV_LABEL_PART_MAIN for screen_label_4
	static lv_style_t style_screen_label_4_main;
	lv_style_reset(&style_screen_label_4_main);

	//Write style state: LV_STATE_DEFAULT for style_screen_label_4_main
	lv_style_set_radius(&style_screen_label_4_main, LV_STATE_DEFAULT, 0);
	lv_style_set_bg_color(&style_screen_label_4_main, LV_STATE_DEFAULT, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_bg_grad_color(&style_screen_label_4_main, LV_STATE_DEFAULT, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_bg_grad_dir(&style_screen_label_4_main, LV_STATE_DEFAULT, LV_GRAD_DIR_VER);
	lv_style_set_bg_opa(&style_screen_label_4_main, LV_STATE_DEFAULT, 0);
	lv_style_set_text_color(&style_screen_label_4_main, LV_STATE_DEFAULT, lv_color_make(0x00, 0x00, 0x00));
	lv_style_set_text_font(&style_screen_label_4_main, LV_STATE_DEFAULT, &lv_font_simsun_20);
	lv_style_set_text_letter_space(&style_screen_label_4_main, LV_STATE_DEFAULT, 2);
	lv_style_set_pad_left(&style_screen_label_4_main, LV_STATE_DEFAULT, 0);
	lv_style_set_pad_right(&style_screen_label_4_main, LV_STATE_DEFAULT, 0);
	lv_style_set_pad_top(&style_screen_label_4_main, LV_STATE_DEFAULT, 0);
	lv_style_set_pad_bottom(&style_screen_label_4_main, LV_STATE_DEFAULT, 0);
	lv_obj_add_style(ui->screen_label_4, LV_LABEL_PART_MAIN, &style_screen_label_4_main);
	lv_obj_set_pos(ui->screen_label_4, 220, 120);
	lv_obj_set_size(ui->screen_label_4, 80, 0);

	//Write codes screen_img_2
	ui->screen_img_2 = lv_img_create(ui->screen_tabview_1_title1, NULL);

	//Write style LV_IMG_PART_MAIN for screen_img_2
	static lv_style_t style_screen_img_2_main;
	lv_style_reset(&style_screen_img_2_main);

	//Write style state: LV_STATE_DEFAULT for style_screen_img_2_main
	lv_style_set_image_recolor(&style_screen_img_2_main, LV_STATE_DEFAULT, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_image_recolor_opa(&style_screen_img_2_main, LV_STATE_DEFAULT, 0);
	lv_style_set_image_opa(&style_screen_img_2_main, LV_STATE_DEFAULT, 255);
	lv_obj_add_style(ui->screen_img_2, LV_IMG_PART_MAIN, &style_screen_img_2_main);
	lv_obj_set_pos(ui->screen_img_2, 160, 30);
	lv_obj_set_size(ui->screen_img_2, 50, 50);
	lv_obj_set_click(ui->screen_img_2, true);
	lv_img_set_src(ui->screen_img_2,&_4_alpha_50x50);
	lv_img_set_pivot(ui->screen_img_2, 0,0);
	lv_img_set_angle(ui->screen_img_2, 0);

	//Write codes screen_img_3
	ui->screen_img_3 = lv_img_create(ui->screen_tabview_1_title1, NULL);

	//Write style LV_IMG_PART_MAIN for screen_img_3
	static lv_style_t style_screen_img_3_main;
	lv_style_reset(&style_screen_img_3_main);

	//Write style state: LV_STATE_DEFAULT for style_screen_img_3_main
	lv_style_set_image_recolor(&style_screen_img_3_main, LV_STATE_DEFAULT, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_image_recolor_opa(&style_screen_img_3_main, LV_STATE_DEFAULT, 0);
	lv_style_set_image_opa(&style_screen_img_3_main, LV_STATE_DEFAULT, 255);
	lv_obj_add_style(ui->screen_img_3, LV_IMG_PART_MAIN, &style_screen_img_3_main);
	lv_obj_set_pos(ui->screen_img_3, 20, 110);
	lv_obj_set_size(ui->screen_img_3, 50, 50);
	lv_obj_set_click(ui->screen_img_3, true);
	lv_img_set_src(ui->screen_img_3,&_1_alpha_50x50);
	lv_img_set_pivot(ui->screen_img_3, 0,0);
	lv_img_set_angle(ui->screen_img_3, 0);

	//Write codes screen_img_4
	ui->screen_img_4 = lv_img_create(ui->screen_tabview_1_title1, NULL);

	//Write style LV_IMG_PART_MAIN for screen_img_4
	static lv_style_t style_screen_img_4_main;
	lv_style_reset(&style_screen_img_4_main);

	//Write style state: LV_STATE_DEFAULT for style_screen_img_4_main
	lv_style_set_image_recolor(&style_screen_img_4_main, LV_STATE_DEFAULT, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_image_recolor_opa(&style_screen_img_4_main, LV_STATE_DEFAULT, 0);
	lv_style_set_image_opa(&style_screen_img_4_main, LV_STATE_DEFAULT, 255);
	lv_obj_add_style(ui->screen_img_4, LV_IMG_PART_MAIN, &style_screen_img_4_main);
	lv_obj_set_pos(ui->screen_img_4, 160, 110);
	lv_obj_set_size(ui->screen_img_4, 50, 50);
	lv_obj_set_click(ui->screen_img_4, true);
	lv_img_set_src(ui->screen_img_4,&_2_alpha_50x50);
	lv_img_set_pivot(ui->screen_img_4, 0,0);
	lv_img_set_angle(ui->screen_img_4, 0);

	//Write codes screen_tabview_1_title2

	//Write codes screen_img_5
	ui->screen_img_5 = lv_img_create(ui->screen_tabview_1_title2, NULL);

	//Write style LV_IMG_PART_MAIN for screen_img_5
	static lv_style_t style_screen_img_5_main;
	lv_style_reset(&style_screen_img_5_main);

	//Write style state: LV_STATE_DEFAULT for style_screen_img_5_main
	lv_style_set_image_recolor(&style_screen_img_5_main, LV_STATE_DEFAULT, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_image_recolor_opa(&style_screen_img_5_main, LV_STATE_DEFAULT, 0);
	lv_style_set_image_opa(&style_screen_img_5_main, LV_STATE_DEFAULT, 255);
	lv_obj_add_style(ui->screen_img_5, LV_IMG_PART_MAIN, &style_screen_img_5_main);
	lv_obj_set_pos(ui->screen_img_5, 0, 50);
	lv_obj_set_size(ui->screen_img_5, 50, 50);
	lv_obj_set_click(ui->screen_img_5, true);
	lv_img_set_src(ui->screen_img_5,&_6_alpha_50x50);
	lv_img_set_pivot(ui->screen_img_5, 0,0);
	lv_img_set_angle(ui->screen_img_5, 0);

	//Write codes screen_label_8
	ui->screen_label_8 = lv_label_create(ui->screen_tabview_1_title2, NULL);
	lv_label_set_text(ui->screen_label_8, "33");
	lv_label_set_long_mode(ui->screen_label_8, LV_LABEL_LONG_BREAK);
	lv_label_set_align(ui->screen_label_8, LV_LABEL_ALIGN_CENTER);

	//Write style LV_LABEL_PART_MAIN for screen_label_8
	static lv_style_t style_screen_label_8_main;
	lv_style_reset(&style_screen_label_8_main);

	//Write style state: LV_STATE_DEFAULT for style_screen_label_8_main
	lv_style_set_radius(&style_screen_label_8_main, LV_STATE_DEFAULT, 0);
	lv_style_set_bg_color(&style_screen_label_8_main, LV_STATE_DEFAULT, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_bg_grad_color(&style_screen_label_8_main, LV_STATE_DEFAULT, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_bg_grad_dir(&style_screen_label_8_main, LV_STATE_DEFAULT, LV_GRAD_DIR_VER);
	lv_style_set_bg_opa(&style_screen_label_8_main, LV_STATE_DEFAULT, 0);
	lv_style_set_text_color(&style_screen_label_8_main, LV_STATE_DEFAULT, lv_color_make(0x00, 0x00, 0x00));
	lv_style_set_text_font(&style_screen_label_8_main, LV_STATE_DEFAULT, &lv_font_simsun_18);
	lv_style_set_text_letter_space(&style_screen_label_8_main, LV_STATE_DEFAULT, 2);
	lv_style_set_pad_left(&style_screen_label_8_main, LV_STATE_DEFAULT, 0);
	lv_style_set_pad_right(&style_screen_label_8_main, LV_STATE_DEFAULT, 0);
	lv_style_set_pad_top(&style_screen_label_8_main, LV_STATE_DEFAULT, 0);
	lv_style_set_pad_bottom(&style_screen_label_8_main, LV_STATE_DEFAULT, 0);
	lv_obj_add_style(ui->screen_label_8, LV_LABEL_PART_MAIN, &style_screen_label_8_main);
	lv_obj_set_pos(ui->screen_label_8, 38, 63);
	lv_obj_set_size(ui->screen_label_8, 50, 0);

	//Write codes screen_label_9
	ui->screen_label_9 = lv_label_create(ui->screen_tabview_1_title2, NULL);
//	lv_label_set_text(ui->screen_label_9, "郑州市");
	lv_label_set_long_mode(ui->screen_label_9, LV_LABEL_LONG_BREAK);
	lv_label_set_align(ui->screen_label_9, LV_LABEL_ALIGN_CENTER);

	//Write style LV_LABEL_PART_MAIN for screen_label_9
	static lv_style_t style_screen_label_9_main;
	lv_style_reset(&style_screen_label_9_main);

	//Write style state: LV_STATE_DEFAULT for style_screen_label_9_main
	lv_style_set_radius(&style_screen_label_9_main, LV_STATE_DEFAULT, 0);
	lv_style_set_bg_color(&style_screen_label_9_main, LV_STATE_DEFAULT, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_bg_grad_color(&style_screen_label_9_main, LV_STATE_DEFAULT, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_bg_grad_dir(&style_screen_label_9_main, LV_STATE_DEFAULT, LV_GRAD_DIR_VER);
	lv_style_set_bg_opa(&style_screen_label_9_main, LV_STATE_DEFAULT, 0);
	lv_style_set_text_color(&style_screen_label_9_main, LV_STATE_DEFAULT, lv_color_make(0x00, 0x00, 0x00));
	lv_style_set_text_font(&style_screen_label_9_main, LV_STATE_DEFAULT, &lv_font_simsun_18);
	lv_style_set_text_letter_space(&style_screen_label_9_main, LV_STATE_DEFAULT, 2);
	lv_style_set_pad_left(&style_screen_label_9_main, LV_STATE_DEFAULT, 0);
	lv_style_set_pad_right(&style_screen_label_9_main, LV_STATE_DEFAULT, 0);
	lv_style_set_pad_top(&style_screen_label_9_main, LV_STATE_DEFAULT, 0);
	lv_style_set_pad_bottom(&style_screen_label_9_main, LV_STATE_DEFAULT, 0);
	lv_obj_add_style(ui->screen_label_9, LV_LABEL_PART_MAIN, &style_screen_label_9_main);
	lv_obj_set_pos(ui->screen_label_9, 220, 64);
	lv_obj_set_size(ui->screen_label_9, 100, 0);

	//Write codes screen_label_7
	ui->screen_label_7 = lv_label_create(ui->screen_tabview_1_title2, NULL);
	lv_label_set_text(ui->screen_label_7, "2023-10-19 19：54：26");
	lv_label_set_long_mode(ui->screen_label_7, LV_LABEL_LONG_BREAK);
	lv_label_set_align(ui->screen_label_7, LV_LABEL_ALIGN_CENTER);

	//Write style LV_LABEL_PART_MAIN for screen_label_7
	static lv_style_t style_screen_label_7_main;
	lv_style_reset(&style_screen_label_7_main);

	//Write style state: LV_STATE_DEFAULT for style_screen_label_7_main
	lv_style_set_radius(&style_screen_label_7_main, LV_STATE_DEFAULT, 0);
	lv_style_set_bg_color(&style_screen_label_7_main, LV_STATE_DEFAULT, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_bg_grad_color(&style_screen_label_7_main, LV_STATE_DEFAULT, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_bg_grad_dir(&style_screen_label_7_main, LV_STATE_DEFAULT, LV_GRAD_DIR_VER);
	lv_style_set_bg_opa(&style_screen_label_7_main, LV_STATE_DEFAULT, 0);
	lv_style_set_text_color(&style_screen_label_7_main, LV_STATE_DEFAULT, lv_color_make(0x00, 0x00, 0x00));
	lv_style_set_text_font(&style_screen_label_7_main, LV_STATE_DEFAULT, &lv_font_simsun_12);
	lv_style_set_text_letter_space(&style_screen_label_7_main, LV_STATE_DEFAULT, 2);
	lv_style_set_pad_left(&style_screen_label_7_main, LV_STATE_DEFAULT, 0);
	lv_style_set_pad_right(&style_screen_label_7_main, LV_STATE_DEFAULT, 0);
	lv_style_set_pad_top(&style_screen_label_7_main, LV_STATE_DEFAULT, 0);
	lv_style_set_pad_bottom(&style_screen_label_7_main, LV_STATE_DEFAULT, 0);
	lv_obj_add_style(ui->screen_label_7, LV_LABEL_PART_MAIN, &style_screen_label_7_main);
	lv_obj_set_pos(ui->screen_label_7, 80, 138);
	lv_obj_set_size(ui->screen_label_7, 200, 0);

	//Write codes screen_img_7
	ui->screen_img_7 = lv_img_create(ui->screen_tabview_1_title2, NULL);

	//Write style LV_IMG_PART_MAIN for screen_img_7
	static lv_style_t style_screen_img_7_main;
	lv_style_reset(&style_screen_img_7_main);

	//Write style state: LV_STATE_DEFAULT for style_screen_img_7_main
	lv_style_set_image_recolor(&style_screen_img_7_main, LV_STATE_DEFAULT, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_image_recolor_opa(&style_screen_img_7_main, LV_STATE_DEFAULT, 0);
	lv_style_set_image_opa(&style_screen_img_7_main, LV_STATE_DEFAULT, 255);
	lv_obj_add_style(ui->screen_img_7, LV_IMG_PART_MAIN, &style_screen_img_7_main);
	lv_obj_set_pos(ui->screen_img_7, 110, 50);
	lv_obj_set_size(ui->screen_img_7, 50, 50);
	lv_obj_set_click(ui->screen_img_7, true);
	lv_img_set_src(ui->screen_img_7,&_3_alpha_50x50);
	lv_img_set_pivot(ui->screen_img_7, 0,0);
	lv_img_set_angle(ui->screen_img_7, 0);

	//Write codes screen_img_8
	ui->screen_img_8 = lv_img_create(ui->screen_tabview_1_title2, NULL);

	//Write style LV_IMG_PART_MAIN for screen_img_8
	static lv_style_t style_screen_img_8_main;
	lv_style_reset(&style_screen_img_8_main);

	//Write style state: LV_STATE_DEFAULT for style_screen_img_8_main
	lv_style_set_image_recolor(&style_screen_img_8_main, LV_STATE_DEFAULT, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_image_recolor_opa(&style_screen_img_8_main, LV_STATE_DEFAULT, 0);
	lv_style_set_image_opa(&style_screen_img_8_main, LV_STATE_DEFAULT, 255);
	lv_obj_add_style(ui->screen_img_8, LV_IMG_PART_MAIN, &style_screen_img_8_main);
	lv_obj_set_pos(ui->screen_img_8, 180, 50);
	lv_obj_set_size(ui->screen_img_8, 50, 50);
	lv_obj_set_click(ui->screen_img_8, true);
	lv_img_set_src(ui->screen_img_8,&_didian_alpha_50x50);
	lv_img_set_pivot(ui->screen_img_8, 0,0);
	lv_img_set_angle(ui->screen_img_8, 0);

	//Write codes screen_img_9
	ui->screen_img_9 = lv_img_create(ui->screen_tabview_1_title2, NULL);

	//Write style LV_IMG_PART_MAIN for screen_img_9
	static lv_style_t style_screen_img_9_main;
	lv_style_reset(&style_screen_img_9_main);

	//Write style state: LV_STATE_DEFAULT for style_screen_img_9_main
	lv_style_set_image_recolor(&style_screen_img_9_main, LV_STATE_DEFAULT, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_image_recolor_opa(&style_screen_img_9_main, LV_STATE_DEFAULT, 0);
	lv_style_set_image_opa(&style_screen_img_9_main, LV_STATE_DEFAULT, 255);
	lv_obj_add_style(ui->screen_img_9, LV_IMG_PART_MAIN, &style_screen_img_9_main);
	lv_obj_set_pos(ui->screen_img_9, 53, 133);
	lv_obj_set_size(ui->screen_img_9, 25, 25);
	lv_obj_set_click(ui->screen_img_9, true);
	lv_img_set_src(ui->screen_img_9,&_shuaxin_alpha_25x25);
	lv_img_set_pivot(ui->screen_img_9, 0,0);
	lv_img_set_angle(ui->screen_img_9, 0);

	//Write codes screen_label_6
	ui->screen_label_6 = lv_label_create(ui->screen_tabview_1_title2, NULL);
	lv_label_set_text(ui->screen_label_6, "天气预报");
	lv_label_set_long_mode(ui->screen_label_6, LV_LABEL_LONG_BREAK);
	lv_label_set_align(ui->screen_label_6, LV_LABEL_ALIGN_CENTER);

	//Write style LV_LABEL_PART_MAIN for screen_label_6
	static lv_style_t style_screen_label_6_main;
	lv_style_reset(&style_screen_label_6_main);

	//Write style state: LV_STATE_DEFAULT for style_screen_label_6_main
	lv_style_set_radius(&style_screen_label_6_main, LV_STATE_DEFAULT, 0);
	lv_style_set_bg_color(&style_screen_label_6_main, LV_STATE_DEFAULT, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_bg_grad_color(&style_screen_label_6_main, LV_STATE_DEFAULT, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_bg_grad_dir(&style_screen_label_6_main, LV_STATE_DEFAULT, LV_GRAD_DIR_VER);
	lv_style_set_bg_opa(&style_screen_label_6_main, LV_STATE_DEFAULT, 0);
	lv_style_set_text_color(&style_screen_label_6_main, LV_STATE_DEFAULT, lv_color_make(0x00, 0x00, 0x00));
	lv_style_set_text_font(&style_screen_label_6_main, LV_STATE_DEFAULT, &lv_font_simsun_20);
	lv_style_set_text_letter_space(&style_screen_label_6_main, LV_STATE_DEFAULT, 2);
	lv_style_set_pad_left(&style_screen_label_6_main, LV_STATE_DEFAULT, 0);
	lv_style_set_pad_right(&style_screen_label_6_main, LV_STATE_DEFAULT, 0);
	lv_style_set_pad_top(&style_screen_label_6_main, LV_STATE_DEFAULT, 0);
	lv_style_set_pad_bottom(&style_screen_label_6_main, LV_STATE_DEFAULT, 0);
	lv_obj_add_style(ui->screen_label_6, LV_LABEL_PART_MAIN, &style_screen_label_6_main);
	lv_obj_set_pos(ui->screen_label_6, 106, 11);
	lv_obj_set_size(ui->screen_label_6, 100, 0);

	//Write codes screen_tabview_1_title3

	//Write codes screen_calendar_1
	ui->screen_calendar_1 = lv_calendar_create(ui->screen_tabview_1_title3, NULL);

	//Write style LV_CALENDAR_PART_BG for screen_calendar_1
	static lv_style_t style_screen_calendar_1_bg;
	lv_style_reset(&style_screen_calendar_1_bg);

	//Write style state: LV_STATE_DEFAULT for style_screen_calendar_1_bg
	lv_style_set_bg_color(&style_screen_calendar_1_bg, LV_STATE_DEFAULT, lv_color_make(0x13, 0xc9, 0x8c));
	lv_style_set_bg_grad_color(&style_screen_calendar_1_bg, LV_STATE_DEFAULT, lv_color_make(0x12, 0x3b, 0x7d));
	lv_style_set_bg_grad_dir(&style_screen_calendar_1_bg, LV_STATE_DEFAULT, LV_GRAD_DIR_VER);
	lv_style_set_bg_opa(&style_screen_calendar_1_bg, LV_STATE_DEFAULT, 255);
	lv_style_set_border_color(&style_screen_calendar_1_bg, LV_STATE_DEFAULT, lv_color_make(0xc0, 0xc0, 0xc0));
	lv_style_set_border_width(&style_screen_calendar_1_bg, LV_STATE_DEFAULT, 1);
	lv_obj_add_style(ui->screen_calendar_1, LV_CALENDAR_PART_BG, &style_screen_calendar_1_bg);

	//Write style LV_CALENDAR_PART_HEADER for screen_calendar_1
	static lv_style_t style_screen_calendar_1_header;
	lv_style_reset(&style_screen_calendar_1_header);

	//Write style state: LV_STATE_DEFAULT for style_screen_calendar_1_header
	lv_style_set_text_color(&style_screen_calendar_1_header, LV_STATE_DEFAULT, lv_color_make(0x0D, 0x30, 0x55));
	lv_style_set_text_font(&style_screen_calendar_1_header, LV_STATE_DEFAULT, &lv_font_simsun_12);
	lv_style_set_text_letter_space(&style_screen_calendar_1_header, LV_STATE_DEFAULT, 0);
	lv_obj_add_style(ui->screen_calendar_1, LV_CALENDAR_PART_HEADER, &style_screen_calendar_1_header);

	//Write style LV_CALENDAR_PART_DAY_NAMES for screen_calendar_1
	static lv_style_t style_screen_calendar_1_day_names;
	lv_style_reset(&style_screen_calendar_1_day_names);

	//Write style state: LV_STATE_DEFAULT for style_screen_calendar_1_day_names
	lv_style_set_text_color(&style_screen_calendar_1_day_names, LV_STATE_DEFAULT, lv_color_make(0x0D, 0x30, 0x55));
	lv_style_set_text_font(&style_screen_calendar_1_day_names, LV_STATE_DEFAULT, &lv_font_simsun_12);
	lv_obj_add_style(ui->screen_calendar_1, LV_CALENDAR_PART_DAY_NAMES, &style_screen_calendar_1_day_names);

	//Write style LV_CALENDAR_PART_DATE for screen_calendar_1
	static lv_style_t style_screen_calendar_1_date;
	lv_style_reset(&style_screen_calendar_1_date);

	//Write style state: LV_STATE_DEFAULT for style_screen_calendar_1_date
	time_t current_sec = 0;
	current_sec = RTC_GetCounter();
	struct tm *time_info = NULL;
	time_info = localtime(&current_sec);
	
	lv_style_set_text_color(&style_screen_calendar_1_date, LV_STATE_DEFAULT, lv_color_make(0x0D, 0x30, 0x55));
	lv_style_set_text_font(&style_screen_calendar_1_date, LV_STATE_DEFAULT, &lv_font_simsun_12);
	lv_obj_add_style(ui->screen_calendar_1, LV_CALENDAR_PART_DATE, &style_screen_calendar_1_date);
	lv_obj_set_pos(ui->screen_calendar_1, 0, 0);
	lv_obj_set_size(ui->screen_calendar_1, 320, 190);
	today.year = time_info->tm_year + 1900;
	today.month = time_info->tm_mon + 1;
	today.day = time_info->tm_mday;
	lv_calendar_set_today_date(ui->screen_calendar_1, &today);
	lv_calendar_set_showed_date(ui->screen_calendar_1, &today);
	highlihted_days[0].year = time_info->tm_year + 1900;
	highlihted_days[0].month = time_info->tm_mon + 1;
	highlihted_days[0].day = time_info->tm_mday;
	lv_calendar_set_highlighted_dates(ui->screen_calendar_1, highlihted_days, 1);
}