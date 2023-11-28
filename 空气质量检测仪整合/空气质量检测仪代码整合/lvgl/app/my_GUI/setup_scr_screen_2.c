/*
 * Copyright 2023 NXP
 * SPDX-License-Identifier: MIT
 */

#include "lvgl.h"
#include <stdio.h>
#include "gui_guider.h"
#include "events_init.h"


void setup_scr_screen_2(lv_ui_2 *ui){
	//设置字体
	LV_FONT_DECLARE(CH_font24);

	//Write codes screen_2
	ui->screen_2 = lv_obj_create(NULL, NULL);

	//Write style LV_OBJ_PART_MAIN for screen_2
	static lv_style_t style_screen_2_main;
	lv_style_reset(&style_screen_2_main);

	//Write style state: LV_STATE_DEFAULT for style_screen_2_main
	lv_style_set_bg_color(&style_screen_2_main, LV_STATE_DEFAULT, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_bg_opa(&style_screen_2_main, LV_STATE_DEFAULT, 0);
	lv_obj_add_style(ui->screen_2, LV_OBJ_PART_MAIN, &style_screen_2_main);

	//Write codes screen_2_label_2_1
	ui->screen_2_label_2_1 = lv_label_create(ui->screen_2, NULL);
	lv_label_set_text(ui->screen_2_label_2_1, "是否从新设置网络");
	lv_label_set_long_mode(ui->screen_2_label_2_1, LV_LABEL_LONG_BREAK);
	lv_label_set_align(ui->screen_2_label_2_1, LV_LABEL_ALIGN_CENTER);

	//Write style LV_LABEL_PART_MAIN for screen_2_label_2_1
	static lv_style_t style_screen_2_label_2_1_main;
	lv_style_reset(&style_screen_2_label_2_1_main);

	//Write style state: LV_STATE_DEFAULT for style_screen_2_label_2_1_main
	lv_style_set_radius(&style_screen_2_label_2_1_main, LV_STATE_DEFAULT, 0);
	lv_style_set_bg_color(&style_screen_2_label_2_1_main, LV_STATE_DEFAULT, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_bg_grad_color(&style_screen_2_label_2_1_main, LV_STATE_DEFAULT, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_bg_grad_dir(&style_screen_2_label_2_1_main, LV_STATE_DEFAULT, LV_GRAD_DIR_VER);
	lv_style_set_bg_opa(&style_screen_2_label_2_1_main, LV_STATE_DEFAULT, 0);
	lv_style_set_text_color(&style_screen_2_label_2_1_main, LV_STATE_DEFAULT, lv_color_make(0x00, 0x00, 0x00));
	lv_style_set_text_font(&style_screen_2_label_2_1_main, LV_STATE_DEFAULT, &CH_font24);
	lv_style_set_text_letter_space(&style_screen_2_label_2_1_main, LV_STATE_DEFAULT, 2);
	lv_style_set_pad_left(&style_screen_2_label_2_1_main, LV_STATE_DEFAULT, 0);
	lv_style_set_pad_right(&style_screen_2_label_2_1_main, LV_STATE_DEFAULT, 0);
	lv_style_set_pad_top(&style_screen_2_label_2_1_main, LV_STATE_DEFAULT, 0);
	lv_style_set_pad_bottom(&style_screen_2_label_2_1_main, LV_STATE_DEFAULT, 0);
	lv_obj_add_style(ui->screen_2_label_2_1, LV_LABEL_PART_MAIN, &style_screen_2_label_2_1_main);
	lv_obj_set_pos(ui->screen_2_label_2_1, 57, 90);
	lv_obj_set_size(ui->screen_2_label_2_1, 200, 0);

	//Write codes screen_2_btn2_1
	ui->screen_2_btn2_1 = lv_btn_create(ui->screen_2, NULL);

	//Write style LV_BTN_PART_MAIN for screen_2_btn2_1
	static lv_style_t style_screen_2_btn2_1_main;
	lv_style_reset(&style_screen_2_btn2_1_main);

	//Write style state: LV_STATE_DEFAULT for style_screen_2_btn2_1_main
	lv_style_set_radius(&style_screen_2_btn2_1_main, LV_STATE_DEFAULT, 50);
	lv_style_set_bg_color(&style_screen_2_btn2_1_main, LV_STATE_DEFAULT, lv_color_make(0x5b, 0xd0, 0x25));
	lv_style_set_bg_grad_color(&style_screen_2_btn2_1_main, LV_STATE_DEFAULT, lv_color_make(0x11, 0x45, 0x17));
	lv_style_set_bg_grad_dir(&style_screen_2_btn2_1_main, LV_STATE_DEFAULT, LV_GRAD_DIR_VER);
	lv_style_set_bg_opa(&style_screen_2_btn2_1_main, LV_STATE_DEFAULT, 255);
	lv_style_set_border_color(&style_screen_2_btn2_1_main, LV_STATE_DEFAULT, lv_color_make(0x01, 0xa2, 0xb1));
	lv_style_set_border_width(&style_screen_2_btn2_1_main, LV_STATE_DEFAULT, 2);
	lv_style_set_border_opa(&style_screen_2_btn2_1_main, LV_STATE_DEFAULT, 255);
	lv_style_set_outline_color(&style_screen_2_btn2_1_main, LV_STATE_DEFAULT, lv_color_make(0xd4, 0xd7, 0xd9));
	lv_style_set_outline_opa(&style_screen_2_btn2_1_main, LV_STATE_DEFAULT, 255);
	lv_obj_add_style(ui->screen_2_btn2_1, LV_BTN_PART_MAIN, &style_screen_2_btn2_1_main);
	lv_obj_set_pos(ui->screen_2_btn2_1, 40, 136);
	lv_obj_set_size(ui->screen_2_btn2_1, 100, 50);
	ui->screen_2_btn2_1_label = lv_label_create(ui->screen_2_btn2_1, NULL);
	lv_label_set_text(ui->screen_2_btn2_1_label, "按键1 是 ");
	lv_obj_set_style_local_text_color(ui->screen_2_btn2_1_label, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, lv_color_make(0x00, 0x00, 0x00));
	lv_obj_set_style_local_text_font(ui->screen_2_btn2_1_label, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, &CH_font24);

	//Write codes screen_2_btn_2_2
	ui->screen_2_btn_2_2 = lv_btn_create(ui->screen_2, NULL);

	//Write style LV_BTN_PART_MAIN for screen_2_btn_2_2
	static lv_style_t style_screen_2_btn_2_2_main;
	lv_style_reset(&style_screen_2_btn_2_2_main);

	//Write style state: LV_STATE_DEFAULT for style_screen_2_btn_2_2_main
	lv_style_set_radius(&style_screen_2_btn_2_2_main, LV_STATE_DEFAULT, 50);
	lv_style_set_bg_color(&style_screen_2_btn_2_2_main, LV_STATE_DEFAULT, lv_color_make(0x5b, 0xd0, 0x25));
	lv_style_set_bg_grad_color(&style_screen_2_btn_2_2_main, LV_STATE_DEFAULT, lv_color_make(0x11, 0x45, 0x17));
	lv_style_set_bg_grad_dir(&style_screen_2_btn_2_2_main, LV_STATE_DEFAULT, LV_GRAD_DIR_VER);
	lv_style_set_bg_opa(&style_screen_2_btn_2_2_main, LV_STATE_DEFAULT, 255);
	lv_style_set_border_color(&style_screen_2_btn_2_2_main, LV_STATE_DEFAULT, lv_color_make(0x01, 0xa2, 0xb1));
	lv_style_set_border_width(&style_screen_2_btn_2_2_main, LV_STATE_DEFAULT, 2);
	lv_style_set_border_opa(&style_screen_2_btn_2_2_main, LV_STATE_DEFAULT, 255);
	lv_style_set_outline_color(&style_screen_2_btn_2_2_main, LV_STATE_DEFAULT, lv_color_make(0xd4, 0xd7, 0xd9));
	lv_style_set_outline_opa(&style_screen_2_btn_2_2_main, LV_STATE_DEFAULT, 255);
	lv_obj_add_style(ui->screen_2_btn_2_2, LV_BTN_PART_MAIN, &style_screen_2_btn_2_2_main);
	lv_obj_set_pos(ui->screen_2_btn_2_2, 180, 134);
	lv_obj_set_size(ui->screen_2_btn_2_2, 100, 50);
	ui->screen_2_btn_2_2_label = lv_label_create(ui->screen_2_btn_2_2, NULL);
	lv_label_set_text(ui->screen_2_btn_2_2_label, "按键2 否 ");
	lv_obj_set_style_local_text_color(ui->screen_2_btn_2_2_label, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, lv_color_make(0x00, 0x00, 0x00));
	lv_obj_set_style_local_text_font(ui->screen_2_btn_2_2_label, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, &CH_font24);

	//Write codes screen_2_label_2_2
	ui->screen_2_label_2_2 = lv_label_create(ui->screen_2, NULL);
	lv_label_set_text(ui->screen_2_label_2_2, "当前网络");
	lv_label_set_long_mode(ui->screen_2_label_2_2, LV_LABEL_LONG_BREAK);
	lv_label_set_align(ui->screen_2_label_2_2, LV_LABEL_ALIGN_CENTER);

	//Write style LV_LABEL_PART_MAIN for screen_2_label_2_2
	static lv_style_t style_screen_2_label_2_2_main;
	lv_style_reset(&style_screen_2_label_2_2_main);

	//Write style state: LV_STATE_DEFAULT for style_screen_2_label_2_2_main
	lv_style_set_radius(&style_screen_2_label_2_2_main, LV_STATE_DEFAULT, 0);
	lv_style_set_bg_color(&style_screen_2_label_2_2_main, LV_STATE_DEFAULT, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_bg_grad_color(&style_screen_2_label_2_2_main, LV_STATE_DEFAULT, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_bg_grad_dir(&style_screen_2_label_2_2_main, LV_STATE_DEFAULT, LV_GRAD_DIR_VER);
	lv_style_set_bg_opa(&style_screen_2_label_2_2_main, LV_STATE_DEFAULT, 0);
	lv_style_set_text_color(&style_screen_2_label_2_2_main, LV_STATE_DEFAULT, lv_color_make(0x00, 0x00, 0x00));
	lv_style_set_text_font(&style_screen_2_label_2_2_main, LV_STATE_DEFAULT, &CH_font24);
	lv_style_set_text_letter_space(&style_screen_2_label_2_2_main, LV_STATE_DEFAULT, 2);
	lv_style_set_pad_left(&style_screen_2_label_2_2_main, LV_STATE_DEFAULT, 0);
	lv_style_set_pad_right(&style_screen_2_label_2_2_main, LV_STATE_DEFAULT, 0);
	lv_style_set_pad_top(&style_screen_2_label_2_2_main, LV_STATE_DEFAULT, 0);
	lv_style_set_pad_bottom(&style_screen_2_label_2_2_main, LV_STATE_DEFAULT, 0);
	lv_obj_add_style(ui->screen_2_label_2_2, LV_LABEL_PART_MAIN, &style_screen_2_label_2_2_main);
	lv_obj_set_pos(ui->screen_2_label_2_2, 6, 44);
	lv_obj_set_size(ui->screen_2_label_2_2, 100, 0);

	//Write codes screen_2_label_2_3
	ui->screen_2_label_2_3 = lv_label_create(ui->screen_2, NULL);
	lv_label_set_text(ui->screen_2_label_2_3, "moumouyao");
	lv_label_set_long_mode(ui->screen_2_label_2_3, LV_LABEL_LONG_BREAK);
	lv_label_set_align(ui->screen_2_label_2_3, LV_LABEL_ALIGN_CENTER);

	//Write style LV_LABEL_PART_MAIN for screen_2_label_2_3
	static lv_style_t style_screen_2_label_2_3_main;
	lv_style_reset(&style_screen_2_label_2_3_main);

	//Write style state: LV_STATE_DEFAULT for style_screen_2_label_2_3_main
	lv_style_set_radius(&style_screen_2_label_2_3_main, LV_STATE_DEFAULT, 0);
	lv_style_set_bg_color(&style_screen_2_label_2_3_main, LV_STATE_DEFAULT, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_bg_grad_color(&style_screen_2_label_2_3_main, LV_STATE_DEFAULT, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_bg_grad_dir(&style_screen_2_label_2_3_main, LV_STATE_DEFAULT, LV_GRAD_DIR_VER);
	lv_style_set_bg_opa(&style_screen_2_label_2_3_main, LV_STATE_DEFAULT, 0);
	lv_style_set_text_color(&style_screen_2_label_2_3_main, LV_STATE_DEFAULT, lv_color_make(0x00, 0x00, 0x00));
	lv_style_set_text_font(&style_screen_2_label_2_3_main, LV_STATE_DEFAULT, &CH_font24);
	lv_style_set_text_letter_space(&style_screen_2_label_2_3_main, LV_STATE_DEFAULT, 2);
	lv_style_set_pad_left(&style_screen_2_label_2_3_main, LV_STATE_DEFAULT, 0);
	lv_style_set_pad_right(&style_screen_2_label_2_3_main, LV_STATE_DEFAULT, 0);
	lv_style_set_pad_top(&style_screen_2_label_2_3_main, LV_STATE_DEFAULT, 0);
	lv_style_set_pad_bottom(&style_screen_2_label_2_3_main, LV_STATE_DEFAULT, 0);
	lv_obj_add_style(ui->screen_2_label_2_3, LV_LABEL_PART_MAIN, &style_screen_2_label_2_3_main);
	lv_obj_set_pos(ui->screen_2_label_2_3, 114, 44);
	lv_obj_set_size(ui->screen_2_label_2_3, 200, 0);

	//Init events for screen
	events_init_screen_2(ui);
}