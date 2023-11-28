/*
 * Copyright 2023 NXP
 * SPDX-License-Identifier: MIT
 */

#include "lvgl.h"
#include <stdio.h>
#include "gui_guider.h"

lv_ui_2 guider_ui_2;
void setup_ui(lv_ui *ui){
	LV_THEME_DEFAULT_INIT(LV_THEME_DEFAULT_COLOR_PRIMARY, LV_THEME_DEFAULT_COLOR_SECONDARY, 
		LV_THEME_MATERIAL_FLAG_LIGHT, LV_THEME_DEFAULT_FONT_SMALL,  LV_THEME_DEFAULT_FONT_NORMAL, 
		LV_THEME_DEFAULT_FONT_SUBTITLE, LV_THEME_DEFAULT_FONT_TITLE);
	setup_scr_screen(ui);
	lv_scr_load(ui->screen);
}
