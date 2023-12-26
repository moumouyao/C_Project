// SPDX-License-Identifier: MIT
// Copyright 2020 NXP

/*
 * custom.h
 *
 *  Created on: July 29, 2020
 *      Author: nxf53801
 */

#ifndef __CUSTOM_H_
#define __CUSTOM_H_

#include "gui_guider.h"
#include "gui_guider.h"
#include <libavutil/log.h>
#include <libavformat/avformat.h>
#include <libavutil/timestamp.h>
#include <libavutil/log.h>
#include <libavutil/timestamp.h>
#include "libavutil/avutil.h"
#include "libavdevice/avdevice.h" //设备头文件
#include <libavformat/avformat.h>//格式头文件（FFMPEG不管是音频还是视频统一用 avformat结构体）
#include <stdio.h>
#include <stdlib.h>
#include "string.h"
#include "libavutil/avutil.h"
#include "libavformat/avformat.h"
#include "libavcodec/packet.h"
#include "libavcodec/avcodec.h"
#include "libavcodec/codec_id.h"
#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
#include <libswscale/swscale.h>
#include <libavutil/imgutils.h>
#include "myargment.h"
#include "time.h"
extern int client_count ;
extern lv_style_t style_screen_name_list_main_child;
extern LV_ATTRIBUTE_LARGE_CONST lv_img_dsc_t my_800x480 ;
extern uint8_t argbdata_buf[800*480*4];
extern uint8_t rgbdata_buf[800*480*3];
extern uint8_t yuv420_buf[800*480*2];
extern lv_ui guider_ui;
extern struct SwsContext * myswsp;
extern AVCodecContext * mydecp;
extern int server_fd;
extern struct client_struct * list_head ;
extern char current_select_strname[32];
extern char old_select_strname[32];
void * pthread_show_time(void * arg);

void custom_init(lv_ui *ui);
char * bydeviceid_get_ckd(uint32_t device_id);
void mylist_add(char * name,uint32_t device_id);
void mylist_remove(uint32_t device_id);
uint32_t byname_get_listid(char *name);
char * bydeviceid_get_name(uint32_t device_id);
uint32_t byname_get_device(char *name);

int Init_server(char * ip,uint16_t port,char * server_ip,uint16_t server_port);
int mysock_read(int sockfd,void * data,int size);
int mysock_write(int sockfd,void * data,int size);
void add_show_list(char * name);
void del_show_list(char * name);
char * bydeviceid_get_database_name(uint32_t device_id);
void rgb_toargb(uint8_t * rgbdata,uint8_t * argbdata,int w,int h);
void show_video_to_img(lv_obj_t * obj, uint8_t * data,size_t size);
AVCodecContext *  open_decoder(void);
struct SwsContext * open_mysws(int oldw,int oldh,int w,int h);



void screen_name_list_item0event_handler(lv_obj_t * obj, lv_event_t event);
#endif /* EVENT_CB_H_ */
