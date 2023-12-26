/*
 * Copyright 2023 NXP
 * SPDX-License-Identifier: MIT
 */

#ifndef GUI_GUIDER_H
#define GUI_GUIDER_H
#ifdef __cplusplus
extern "C" {
#endif

#include "lvgl/lvgl.h"
#include "guider_fonts.h"

typedef struct
{
	lv_obj_t *screen;
	lv_obj_t *screen_background_Img;
	lv_obj_t *screen_video_img;
	lv_obj_t *screen_time_label;
	lv_obj_t *screen_date_label;
	lv_obj_t *screen_name_list;
	lv_obj_t *screen_name_list_item0;
	lv_obj_t *screen_name_list_myitems[128];
	lv_obj_t *screen_start_video_btn;
	lv_obj_t *screen_start_video_btn_label;
	lv_obj_t *screen_cut_btn;
	lv_obj_t *screen_cut_btn_label;
	lv_obj_t *screen_label_1;
}lv_ui;

void setup_ui(lv_ui *ui);
extern lv_ui guider_ui;
void setup_scr_screen(lv_ui *ui);
LV_IMG_DECLARE(_back2_alpha_1024x768);
LV_IMG_DECLARE(_bin111_alpha_800x480);
LV_IMG_DECLARE(_bing13_alpha_800x480);
#ifdef __cplusplus
}
#endif
#endif