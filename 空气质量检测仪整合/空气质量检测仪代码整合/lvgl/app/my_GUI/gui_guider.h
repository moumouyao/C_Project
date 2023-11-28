/*
 * Copyright 2023 NXP
 * SPDX-License-Identifier: MIT
 */

#ifndef GUI_GUIDER_H
#define GUI_GUIDER_H
#ifdef __cplusplus
extern "C" {
#endif

#include "lvgl.h"
#include "guider_fonts.h"

typedef struct
{
	lv_obj_t *screen;
	lv_obj_t *screen_tabview_1;
	lv_obj_t *screen_tabview_1_title1;
	lv_obj_t *screen_img_1;
	lv_obj_t *screen_img_6;
	lv_obj_t *screen_label_5;
	lv_obj_t *screen_label_1;
	lv_obj_t *screen_label_2;
	lv_obj_t *screen_label_3;
	lv_obj_t *screen_label_4;
	lv_obj_t *screen_img_2;
	lv_obj_t *screen_img_3;
	lv_obj_t *screen_img_4;
	lv_obj_t *screen_tabview_1_title2;
	lv_obj_t *screen_img_5;
	lv_obj_t *screen_label_8;
	lv_obj_t *screen_label_9;
	lv_obj_t *screen_label_7;
	lv_obj_t *screen_img_7;
	lv_obj_t *screen_img_8;
	lv_obj_t *screen_img_9;
	lv_obj_t *screen_label_6;
	lv_obj_t *screen_tabview_1_title3;
	lv_obj_t *screen_calendar_1;
}lv_ui;

void setup_ui(lv_ui *ui);
extern lv_ui guider_ui;
void setup_scr_screen(lv_ui *ui);
static lv_calendar_date_t today;
static lv_calendar_date_t highlihted_days[1];
LV_IMG_DECLARE(_2_alpha_50x50);
LV_IMG_DECLARE(_3_alpha_50x50);
LV_IMG_DECLARE(_shuaxin_alpha_25x25);
LV_IMG_DECLARE(_didian_alpha_50x50);
LV_IMG_DECLARE(_6_alpha_50x50);
LV_IMG_DECLARE(_5_alpha_50x50);
LV_IMG_DECLARE(_4_alpha_50x50);
LV_IMG_DECLARE(_1_alpha_50x50);
LV_IMG_DECLARE(_7_alpha_20x20);

LV_IMG_DECLARE(_1x_alpha_50x50);
LV_IMG_DECLARE(_4x_alpha_50x50);
LV_IMG_DECLARE(_5x_alpha_50x50);
LV_IMG_DECLARE(_6x_alpha_50x50);
LV_IMG_DECLARE(_8x_alpha_50x50);
LV_IMG_DECLARE(_9x_alpha_50x50);
LV_IMG_DECLARE(_13x_alpha_50x50);
LV_IMG_DECLARE(_17x_alpha_50x50);
LV_IMG_DECLARE(_29x_alpha_50x50);
LV_IMG_DECLARE(_31x_alpha_50x50);
LV_IMG_DECLARE(_0x_alpha_50x50);



typedef struct
{
	lv_obj_t *screen_2;
	lv_obj_t *screen_2_label_2_1;
	lv_obj_t *screen_2_btn2_1;
	lv_obj_t *screen_2_btn2_1_label;
	lv_obj_t *screen_2_btn_2_2;
	lv_obj_t *screen_2_btn_2_2_label;
	lv_obj_t *screen_2_label_2_2;
	lv_obj_t *screen_2_label_2_3;
	lv_obj_t *screen_3;
//	lv_obj_t *screen_3_animimg_3_1;
	lv_obj_t *screen_3_label_3_1;
	lv_obj_t *screen_3_btn_3_1;
	lv_obj_t *screen_3_img_1;
	lv_obj_t *screen_3_img_2;
	lv_obj_t *screen_3_img_3;
	lv_obj_t *screen_3_img_4;
	lv_obj_t *screen_3_btn_3_1_label;
}lv_ui_2;

void setup_ui_2(lv_ui_2 *ui);
extern lv_ui_2 guider_ui_2;
void setup_scr_screen_2(lv_ui_2 *ui);
void setup_scr_screen_3(lv_ui_2 *ui);

//#include "lvgl/src/lv_widgets/lv_anim_img.h"
LV_IMG_DECLARE(screen_3_animimg_3_1w1)
LV_IMG_DECLARE(screen_3_animimg_3_1w2)
LV_IMG_DECLARE(screen_3_animimg_3_1w3)
LV_IMG_DECLARE(screen_3_animimg_3_1w4)



#ifdef __cplusplus
}
#endif
#endif