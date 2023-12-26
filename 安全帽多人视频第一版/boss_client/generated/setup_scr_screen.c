/*
 * Copyright 2023 NXP
 * SPDX-License-Identifier: MIT
 */

#include "lvgl/lvgl.h"
#include <stdio.h>
#include "gui_guider.h"
#include "events_init.h"
#include "custom.h"
lv_style_t style_screen_name_list_main_child;

void setup_scr_screen(lv_ui *ui){

	//Write codes screen
	ui->screen = lv_obj_create(NULL, NULL);

	//Write codes screen_background_Img
	ui->screen_background_Img = lv_img_create(ui->screen, NULL);

	//Write style LV_IMG_PART_MAIN for screen_background_Img
	static lv_style_t style_screen_background_Img_main;
	lv_style_reset(&style_screen_background_Img_main);

	//Write style state: LV_STATE_DEFAULT for style_screen_background_Img_main
	lv_style_set_image_recolor(&style_screen_background_Img_main, LV_STATE_DEFAULT, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_image_recolor_opa(&style_screen_background_Img_main, LV_STATE_DEFAULT, 0);
	lv_style_set_image_opa(&style_screen_background_Img_main, LV_STATE_DEFAULT, 255);
	lv_obj_add_style(ui->screen_background_Img, LV_IMG_PART_MAIN, &style_screen_background_Img_main);
	lv_obj_set_pos(ui->screen_background_Img, 0, 0);
	lv_obj_set_size(ui->screen_background_Img, 1024, 768);
	lv_obj_set_click(ui->screen_background_Img, true);
	lv_img_set_src(ui->screen_background_Img,&_back2_alpha_1024x768);
	lv_img_set_pivot(ui->screen_background_Img, 0,0);
	lv_img_set_angle(ui->screen_background_Img, 0);

	//Write codes screen_video_img
	ui->screen_video_img = lv_img_create(ui->screen, NULL);

	//Write style LV_IMG_PART_MAIN for screen_video_img
	static lv_style_t style_screen_video_img_main;
	lv_style_reset(&style_screen_video_img_main);

	//Write style state: LV_STATE_DEFAULT for style_screen_video_img_main
	lv_style_set_image_recolor(&style_screen_video_img_main, LV_STATE_DEFAULT, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_image_recolor_opa(&style_screen_video_img_main, LV_STATE_DEFAULT, 0);
	lv_style_set_image_opa(&style_screen_video_img_main, LV_STATE_DEFAULT, 255);
	lv_obj_add_style(ui->screen_video_img, LV_IMG_PART_MAIN, &style_screen_video_img_main);
	lv_obj_set_pos(ui->screen_video_img, 0, 0);
	lv_obj_set_size(ui->screen_video_img, 800, 480);
	lv_obj_set_click(ui->screen_video_img, true);
	lv_img_set_src(ui->screen_video_img,&_bing13_alpha_800x480);
	lv_img_set_pivot(ui->screen_video_img, 0,0);
	lv_img_set_angle(ui->screen_video_img, 0);

	//Write codes screen_time_label
	ui->screen_time_label = lv_label_create(ui->screen, NULL);
	lv_label_set_text(ui->screen_time_label, "14:40:32");
	lv_label_set_long_mode(ui->screen_time_label, LV_LABEL_LONG_BREAK);
	lv_label_set_align(ui->screen_time_label, LV_LABEL_ALIGN_CENTER);

	//Write style LV_LABEL_PART_MAIN for screen_time_label
	static lv_style_t style_screen_time_label_main;
	lv_style_reset(&style_screen_time_label_main);

	//Write style state: LV_STATE_DEFAULT for style_screen_time_label_main
	lv_style_set_radius(&style_screen_time_label_main, LV_STATE_DEFAULT, 0);
	lv_style_set_bg_color(&style_screen_time_label_main, LV_STATE_DEFAULT, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_bg_grad_color(&style_screen_time_label_main, LV_STATE_DEFAULT, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_bg_grad_dir(&style_screen_time_label_main, LV_STATE_DEFAULT, LV_GRAD_DIR_VER);
	lv_style_set_bg_opa(&style_screen_time_label_main, LV_STATE_DEFAULT, 0);
	lv_style_set_text_color(&style_screen_time_label_main, LV_STATE_DEFAULT, lv_color_make(0x00, 0x00, 0x00));
	lv_style_set_text_font(&style_screen_time_label_main, LV_STATE_DEFAULT, &lv_font_simsun_100);
	lv_style_set_text_letter_space(&style_screen_time_label_main, LV_STATE_DEFAULT, 2);
	lv_style_set_pad_left(&style_screen_time_label_main, LV_STATE_DEFAULT, 0);
	lv_style_set_pad_right(&style_screen_time_label_main, LV_STATE_DEFAULT, 0);
	lv_style_set_pad_top(&style_screen_time_label_main, LV_STATE_DEFAULT, 0);
	lv_style_set_pad_bottom(&style_screen_time_label_main, LV_STATE_DEFAULT, 0);
	lv_obj_add_style(ui->screen_time_label, LV_LABEL_PART_MAIN, &style_screen_time_label_main);
	lv_obj_set_pos(ui->screen_time_label, 20, 506);
	lv_obj_set_size(ui->screen_time_label, 766, 0);

	//Write codes screen_date_label
	ui->screen_date_label = lv_label_create(ui->screen, NULL);
	lv_label_set_text(ui->screen_date_label, "2023/07/04");
	lv_label_set_long_mode(ui->screen_date_label, LV_LABEL_LONG_BREAK);
	lv_label_set_align(ui->screen_date_label, LV_LABEL_ALIGN_CENTER);

	//Write style LV_LABEL_PART_MAIN for screen_date_label
	static lv_style_t style_screen_date_label_main;
	lv_style_reset(&style_screen_date_label_main);

	//Write style state: LV_STATE_DEFAULT for style_screen_date_label_main
	lv_style_set_radius(&style_screen_date_label_main, LV_STATE_DEFAULT, 0);
	lv_style_set_bg_color(&style_screen_date_label_main, LV_STATE_DEFAULT, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_bg_grad_color(&style_screen_date_label_main, LV_STATE_DEFAULT, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_bg_grad_dir(&style_screen_date_label_main, LV_STATE_DEFAULT, LV_GRAD_DIR_VER);
	lv_style_set_bg_opa(&style_screen_date_label_main, LV_STATE_DEFAULT, 0);
	lv_style_set_text_color(&style_screen_date_label_main, LV_STATE_DEFAULT, lv_color_make(0x00, 0x00, 0x00));
	lv_style_set_text_font(&style_screen_date_label_main, LV_STATE_DEFAULT, &lv_font_simsun_100);
	lv_style_set_text_letter_space(&style_screen_date_label_main, LV_STATE_DEFAULT, 2);
	lv_style_set_pad_left(&style_screen_date_label_main, LV_STATE_DEFAULT, 0);
	lv_style_set_pad_right(&style_screen_date_label_main, LV_STATE_DEFAULT, 0);
	lv_style_set_pad_top(&style_screen_date_label_main, LV_STATE_DEFAULT, 0);
	lv_style_set_pad_bottom(&style_screen_date_label_main, LV_STATE_DEFAULT, 0);
	lv_obj_add_style(ui->screen_date_label, LV_LABEL_PART_MAIN, &style_screen_date_label_main);
	lv_obj_set_pos(ui->screen_date_label, 20, 632);
	lv_obj_set_size(ui->screen_date_label, 766, 0);

	//Write codes screen_name_list
	ui->screen_name_list = lv_list_create(ui->screen, NULL);
	lv_list_set_edge_flash(ui->screen_name_list, true);

	//Write style LV_LIST_PART_BG for screen_name_list
	static lv_style_t style_screen_name_list_bg;
	lv_style_reset(&style_screen_name_list_bg);

	//Write style state: LV_STATE_DEFAULT for style_screen_name_list_bg
	lv_style_set_radius(&style_screen_name_list_bg, LV_STATE_DEFAULT, 3);
	lv_style_set_border_color(&style_screen_name_list_bg, LV_STATE_DEFAULT, lv_color_make(0xe1, 0xe6, 0xee));
	lv_style_set_border_width(&style_screen_name_list_bg, LV_STATE_DEFAULT, 1);
	lv_style_set_pad_left(&style_screen_name_list_bg, LV_STATE_DEFAULT, 5);
	lv_style_set_pad_right(&style_screen_name_list_bg, LV_STATE_DEFAULT, 5);
	lv_style_set_pad_top(&style_screen_name_list_bg, LV_STATE_DEFAULT, 5);
	lv_obj_add_style(ui->screen_name_list, LV_LIST_PART_BG, &style_screen_name_list_bg);

	//Write style LV_LIST_PART_SCROLLABLE for screen_name_list
	static lv_style_t style_screen_name_list_scrollable;
	lv_style_reset(&style_screen_name_list_scrollable);

	//Write style state: LV_STATE_DEFAULT for style_screen_name_list_scrollable
	lv_style_set_radius(&style_screen_name_list_scrollable, LV_STATE_DEFAULT, 3);
	lv_style_set_bg_color(&style_screen_name_list_scrollable, LV_STATE_DEFAULT, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_bg_grad_color(&style_screen_name_list_scrollable, LV_STATE_DEFAULT, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_bg_grad_dir(&style_screen_name_list_scrollable, LV_STATE_DEFAULT, LV_GRAD_DIR_VER);
	lv_style_set_bg_opa(&style_screen_name_list_scrollable, LV_STATE_DEFAULT, 255);
	lv_obj_add_style(ui->screen_name_list, LV_LIST_PART_SCROLLABLE, &style_screen_name_list_scrollable);

	//Write style LV_BTN_PART_MAIN for screen_name_list
	
	lv_style_reset(&style_screen_name_list_main_child);

	//Write style state: LV_STATE_DEFAULT for style_screen_name_list_main_child
	lv_style_set_radius(&style_screen_name_list_main_child, LV_STATE_DEFAULT, 3);
	lv_style_set_bg_color(&style_screen_name_list_main_child, LV_STATE_DEFAULT, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_bg_grad_color(&style_screen_name_list_main_child, LV_STATE_DEFAULT, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_bg_grad_dir(&style_screen_name_list_main_child, LV_STATE_DEFAULT, LV_GRAD_DIR_VER);
	lv_style_set_bg_opa(&style_screen_name_list_main_child, LV_STATE_DEFAULT, 255);
	lv_style_set_text_color(&style_screen_name_list_main_child, LV_STATE_DEFAULT, lv_color_make(0x0D, 0x30, 0x55));
	lv_style_set_text_font(&style_screen_name_list_main_child, LV_STATE_DEFAULT, &myFont20);
	lv_obj_set_pos(ui->screen_name_list, 806, 5);
	lv_obj_set_size(ui->screen_name_list, 208, 356);
	lv_obj_t *screen_name_list_btn;

	screen_name_list_btn = lv_list_add_btn(ui->screen_name_list, LV_SYMBOL_CALL, "张三");
	ui->screen_name_list_item0 = screen_name_list_btn;
	lv_obj_add_style(screen_name_list_btn, LV_BTN_PART_MAIN, &style_screen_name_list_main_child);
/*
	screen_name_list_btn = lv_list_add_btn(ui->screen_name_list, LV_SYMBOL_CALL, "赵四");
	ui->screen_name_list_myitems[0] = screen_name_list_btn;
	lv_obj_add_style(screen_name_list_btn, LV_BTN_PART_MAIN, &style_screen_name_list_main_child);
	screen_name_list_btn = lv_list_add_btn(ui->screen_name_list, LV_SYMBOL_CALL, "王二");
	ui->screen_name_list_myitems[1] = screen_name_list_btn;
	lv_obj_add_style(screen_name_list_btn, LV_BTN_PART_MAIN, &style_screen_name_list_main_child);
	screen_name_list_btn = lv_list_add_btn(ui->screen_name_list, LV_SYMBOL_CALL, "麻子");
	ui->screen_name_list_myitems[2] = screen_name_list_btn;
	lv_obj_add_style(screen_name_list_btn, LV_BTN_PART_MAIN, &style_screen_name_list_main_child);
*/	


	//Write codes screen_start_video_btn
	ui->screen_start_video_btn = lv_btn_create(ui->screen, NULL);

	//Write style LV_BTN_PART_MAIN for screen_start_video_btn
	static lv_style_t style_screen_start_video_btn_main;
	lv_style_reset(&style_screen_start_video_btn_main);

	//Write style state: LV_STATE_DEFAULT for style_screen_start_video_btn_main
	lv_style_set_radius(&style_screen_start_video_btn_main, LV_STATE_DEFAULT, 50);
	lv_style_set_bg_color(&style_screen_start_video_btn_main, LV_STATE_DEFAULT, lv_color_make(0x00, 0xee, 0xff));
	lv_style_set_bg_grad_color(&style_screen_start_video_btn_main, LV_STATE_DEFAULT, lv_color_make(0x10, 0xb2, 0x9f));
	lv_style_set_bg_grad_dir(&style_screen_start_video_btn_main, LV_STATE_DEFAULT, LV_GRAD_DIR_VER);
	lv_style_set_bg_opa(&style_screen_start_video_btn_main, LV_STATE_DEFAULT, 100);
	lv_style_set_border_color(&style_screen_start_video_btn_main, LV_STATE_DEFAULT, lv_color_make(0x01, 0xa2, 0xb1));
	lv_style_set_border_width(&style_screen_start_video_btn_main, LV_STATE_DEFAULT, 0);
	lv_style_set_border_opa(&style_screen_start_video_btn_main, LV_STATE_DEFAULT, 0);
	lv_style_set_outline_color(&style_screen_start_video_btn_main, LV_STATE_DEFAULT, lv_color_make(0xd4, 0xd7, 0xd9));
	lv_style_set_outline_opa(&style_screen_start_video_btn_main, LV_STATE_DEFAULT, 255);
	lv_obj_add_style(ui->screen_start_video_btn, LV_BTN_PART_MAIN, &style_screen_start_video_btn_main);
	lv_obj_set_pos(ui->screen_start_video_btn, 850, 571);
	lv_obj_set_size(ui->screen_start_video_btn, 163, 86);
	ui->screen_start_video_btn_label = lv_label_create(ui->screen_start_video_btn, NULL);
	lv_label_set_text(ui->screen_start_video_btn_label, "开启监控");
	lv_obj_set_style_local_text_color(ui->screen_start_video_btn_label, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, lv_color_make(0x00, 0x00, 0x00));
	lv_obj_set_style_local_text_font(ui->screen_start_video_btn_label, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, &myFont20);

	//Write codes screen_cut_btn
	ui->screen_cut_btn = lv_btn_create(ui->screen, NULL);

	//Write style LV_BTN_PART_MAIN for screen_cut_btn
	static lv_style_t style_screen_cut_btn_main;
	lv_style_reset(&style_screen_cut_btn_main);

	//Write style state: LV_STATE_DEFAULT for style_screen_cut_btn_main
	lv_style_set_radius(&style_screen_cut_btn_main, LV_STATE_DEFAULT, 50);
	lv_style_set_bg_color(&style_screen_cut_btn_main, LV_STATE_DEFAULT, lv_color_make(0x00, 0xee, 0xff));
	lv_style_set_bg_grad_color(&style_screen_cut_btn_main, LV_STATE_DEFAULT, lv_color_make(0x10, 0xb2, 0x9f));
	lv_style_set_bg_grad_dir(&style_screen_cut_btn_main, LV_STATE_DEFAULT, LV_GRAD_DIR_VER);
	lv_style_set_bg_opa(&style_screen_cut_btn_main, LV_STATE_DEFAULT, 100);
	lv_style_set_border_color(&style_screen_cut_btn_main, LV_STATE_DEFAULT, lv_color_make(0x01, 0xa2, 0xb1));
	lv_style_set_border_width(&style_screen_cut_btn_main, LV_STATE_DEFAULT, 0);
	lv_style_set_border_opa(&style_screen_cut_btn_main, LV_STATE_DEFAULT, 0);
	lv_style_set_outline_color(&style_screen_cut_btn_main, LV_STATE_DEFAULT, lv_color_make(0xd4, 0xd7, 0xd9));
	lv_style_set_outline_opa(&style_screen_cut_btn_main, LV_STATE_DEFAULT, 255);
	lv_obj_add_style(ui->screen_cut_btn, LV_BTN_PART_MAIN, &style_screen_cut_btn_main);
	lv_obj_set_pos(ui->screen_cut_btn, 850, 677);
	lv_obj_set_size(ui->screen_cut_btn, 163, 86);
	ui->screen_cut_btn_label = lv_label_create(ui->screen_cut_btn, NULL);
	lv_label_set_text(ui->screen_cut_btn_label, "确认切换");
	lv_obj_set_style_local_text_color(ui->screen_cut_btn_label, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, lv_color_make(0x00, 0x00, 0x00));
	lv_obj_set_style_local_text_font(ui->screen_cut_btn_label, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, &lv_font_simsun_20);

	//Write codes screen_label_1
	ui->screen_label_1 = lv_label_create(ui->screen, NULL);
	lv_label_set_text(ui->screen_label_1, "张三");
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
	lv_style_set_bg_opa(&style_screen_label_1_main, LV_STATE_DEFAULT, 255);
	lv_style_set_text_color(&style_screen_label_1_main, LV_STATE_DEFAULT, lv_color_make(0x00, 0x00, 0x00));
	lv_style_set_text_font(&style_screen_label_1_main, LV_STATE_DEFAULT, &myFont20);
	lv_style_set_text_letter_space(&style_screen_label_1_main, LV_STATE_DEFAULT, 2);
	lv_style_set_pad_left(&style_screen_label_1_main, LV_STATE_DEFAULT, 0);
	lv_style_set_pad_right(&style_screen_label_1_main, LV_STATE_DEFAULT, 0);
	lv_style_set_pad_top(&style_screen_label_1_main, LV_STATE_DEFAULT, 0);
	lv_style_set_pad_bottom(&style_screen_label_1_main, LV_STATE_DEFAULT, 0);
	lv_obj_add_style(ui->screen_label_1, LV_LABEL_PART_MAIN, &style_screen_label_1_main);
	lv_obj_set_pos(ui->screen_label_1, 810, 381);
	lv_obj_set_size(ui->screen_label_1, 198, 0);

	//Init events for screen
	events_init_screen(ui);
}