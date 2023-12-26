// SPDX-License-Identifier: MIT
// Copyright 2020 NXP

/**
 * @file custom.c
 *
 */

/*********************
 *      INCLUDES
 *********************/
#include <stdio.h>
#include "lvgl/lvgl.h"
#include "custom.h"
pthread_t time_pd = 0;
/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/

/**********************
 *  STATIC VARIABLES
 **********************/

/**
 * Create a demo application
 */
void exit_clean(void)
{   
    if(server_fd > 0)
    {
        close(server_fd);
    }
    exit(0);
}
void signal_exit_clean(int signum)
{
    if(server_fd > 0)
    {
        close(server_fd);
    }
    exit(0);
}

void custom_init(lv_ui *ui)
{
    signal(2,signal_exit_clean);
    atexit(exit_clean);
    /* Add your codes here */
    //client_count = 4;
    lv_obj_t * ttmmp = lv_list_get_next_btn(guider_ui.screen_name_list,NULL);
    char * btnname = lv_list_get_btn_text(ttmmp);
    strcpy(current_select_strname,btnname);
    mydecp = open_decoder();
    if(mydecp == NULL)
    {
        printf("解码器打开获取失败！\r\n");
    }
    myswsp = open_mysws(1920,1080,800,480);
    lv_list_clean(guider_ui.screen_name_list);
    server_fd = Init_server("0.0.0.0",22222,"121.40.137.232",55555);
    pthread_create(&time_pd,NULL,pthread_show_time,NULL);
}

