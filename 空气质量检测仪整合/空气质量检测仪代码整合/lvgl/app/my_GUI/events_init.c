/*
 * Copyright 2023 NXP
 * SPDX-License-Identifier: MIT
 */

#include "events_init.h"
#include <stdio.h>
#include "lvgl.h"

void events_init(lv_ui *ui)
{
}

static void screen_2_btn2_1event_handler(lv_obj_t * obj, lv_event_t event)
{
	switch (event)
	{
	case LV_EVENT_CLICKED:
	{
		if (!lv_debug_check_obj_valid(guider_ui_2.screen_3))
			setup_scr_screen_3(&guider_ui_2);
		lv_disp_t * d = lv_obj_get_disp(lv_scr_act());
		if (d->prev_scr == NULL && d->scr_to_load == NULL)
			lv_scr_load_anim(guider_ui_2.screen_3, LV_SCR_LOAD_ANIM_FADE_ON, 1000, 1000, true);
		lv_obj_set_style_local_bg_color(guider_ui_2.screen_2_btn2_1, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, lv_color_make(0x2c, 0xe8, 0xe4));
	}
		break;
	default:
		break;
	}
}

static void screen_2_btn_2_2event_handler(lv_obj_t * obj, lv_event_t event)
{
	switch (event)
	{
	case LV_EVENT_CLICKED:
	{
		lv_obj_set_style_local_bg_color(guider_ui_2.screen_2_btn_2_2, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, lv_color_make(0x28, 0xa6, 0xaf));
	}
		break;
	default:
		break;
	}
}

void events_init_screen_2(lv_ui_2 *ui)
{
	lv_obj_set_event_cb(ui->screen_2_btn2_1, screen_2_btn2_1event_handler);
	lv_obj_set_event_cb(ui->screen_2_btn_2_2, screen_2_btn_2_2event_handler);
}
