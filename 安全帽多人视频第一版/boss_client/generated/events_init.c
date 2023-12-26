/*
 * Copyright 2023 NXP
 * SPDX-License-Identifier: MIT
 */

#include "events_init.h"
#include <stdio.h>
#include "lvgl/lvgl.h"
#include "stdio.h"
#include "custom.h"


void events_init(lv_ui *ui)
{
}

void screen_name_list_item0event_handler(lv_obj_t * obj, lv_event_t event)
{
	switch (event)
	{
	case LV_EVENT_CLICKED:
	{
		printf("namelist is press!\r\n");
		strcpy(current_select_strname,lv_list_get_btn_text(obj));
		printf("namelist is press == %s!\r\n",current_select_strname);
		lv_label_set_text(guider_ui.screen_label_1,current_select_strname);
		//del_show_list("赵四");
	}
		break;
	default:
		break;
	}
}

static void screen_start_video_btnevent_handler(lv_obj_t * obj, lv_event_t event)
{
	switch (event)
	{
	case LV_EVENT_CLICKED:
	{
		char * str = lv_label_get_text(guider_ui.screen_start_video_btn_label);
		struct mymessage msg;
		if(strcmp(str,"开启监控")==0)
		{
			lv_label_set_text(guider_ui.screen_start_video_btn_label, "关闭监控");
			strcpy(old_select_strname,current_select_strname);
			msg.cmd = XYD_SERVER_REQ_VIDEO_DATA;//请求摄像头数据
			msg.deviceid = byname_get_device(current_select_strname);
			printf("msg.deviceid == %x\r\n",msg.deviceid);
			msg.message_len = 0;
			if(server_fd>0)
			{
				printf("发送监控指令！\r\n");
				mysock_write(server_fd,&msg,sizeof(msg));
			}
			printf("当前监控人==%s\r\n",current_select_strname);
		}else if(strcmp(str,"关闭监控")==0)
		{
			lv_label_set_text(guider_ui.screen_start_video_btn_label, "开启监控");
			msg.cmd = XYD_SERVER_REQ_VIDEO_STOP;//请求摄像头数据
			msg.deviceid = byname_get_device(current_select_strname);
			msg.message_len = 0;
			if(server_fd>0)
			{
				mysock_write(server_fd,&msg,sizeof(msg));
			}
			
		}
	}
		break;
	default:
		break;
	}
}

static void screen_cut_btnevent_handler(lv_obj_t * obj, lv_event_t event)
{
	switch (event)
	{
	case LV_EVENT_CLICKED:
	{
		struct mymessage msg;
		msg.cmd = XYD_SERVER_REQ_VIDEO_STOP;//请求摄像头数据
		msg.deviceid = byname_get_device(old_select_strname);
		msg.message_len = 0;
		if(server_fd>0)
		{
			mysock_write(server_fd,&msg,sizeof(msg));
		}	
		printf("停止监控此人==%s\r\n",old_select_strname);
		msg.cmd = XYD_SERVER_REQ_VIDEO_DATA;
		msg.deviceid = byname_get_device(current_select_strname);
		if(server_fd>0)
		{
			mysock_write(server_fd,&msg,sizeof(msg));
		}
		printf("开始监控此人==%s\r\n",current_select_strname);
		strcpy(old_select_strname,current_select_strname);
		
	}
		break;
	default:
		break;
	}
}

void events_init_screen(lv_ui *ui)
{
	//lv_obj_set_event_cb(ui->screen_name_list_myitems[3], screen_name_list_item0event_handler);
	//lv_obj_set_event_cb(ui->screen_name_list_myitems[2], screen_name_list_item0event_handler);
	//lv_obj_set_event_cb(ui->screen_name_list_myitems[1], screen_name_list_item0event_handler);
	//lv_obj_set_event_cb(ui->screen_name_list_myitems[0], screen_name_list_item0event_handler);
	//lv_obj_set_event_cb(ui->screen_name_list_item0, screen_name_list_item0event_handler);
	lv_obj_set_event_cb(ui->screen_start_video_btn, screen_start_video_btnevent_handler);
	lv_obj_set_event_cb(ui->screen_cut_btn, screen_cut_btnevent_handler);
}
