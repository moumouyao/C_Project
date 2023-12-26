#include "custom.h"


void add_show_list(char * name)
{
    lv_obj_t *screen_name_list_btn;
    screen_name_list_btn = lv_list_add_btn(guider_ui.screen_name_list, LV_SYMBOL_CALL,name);
	guider_ui.screen_name_list_myitems[client_count] = screen_name_list_btn;
	lv_obj_add_style(screen_name_list_btn, LV_BTN_PART_MAIN, &style_screen_name_list_main_child);
	lv_obj_set_event_cb(guider_ui.screen_name_list_myitems[client_count], screen_name_list_item0event_handler);
}

void del_show_list(char * name)
{
    int i =0;
    lv_obj_t * tmp= NULL;
    char * btnname = NULL;
    for(i=0;i < client_count; i++)
    {
        tmp = lv_list_get_next_btn(guider_ui.screen_name_list,tmp);
        const char * btnname = lv_list_get_btn_text(tmp);

        if(strcmp(name,btnname) == 0)
        {
            uint32_t idindex= lv_list_get_btn_index(guider_ui.screen_name_list,tmp);
            lv_list_remove(guider_ui.screen_name_list,idindex);
            break;
        }

    }

}


void show_video_to_img(lv_obj_t * obj, uint8_t * data,size_t size)
{
   
    int ret = 0;
    AVPacket * packet = (AVPacket *) av_malloc(sizeof(AVPacket));
    av_init_packet(packet);
    //avcodec_receive_frame时作为参数，获取到frame，获取到的frame有些可能是错误的要过滤掉，否则相应帧可能出现绿屏
    AVFrame *pFrame = av_frame_alloc();
    //作为yuv输出的frame承载者，会进行缩放和过滤出错的帧，YUV相应的数据也是从该对象中读取
    AVFrame *pFrameYUV = av_frame_alloc();
 
    //out_buffer中数据用于渲染的,且格式为YUV420P
    uint8_t *out_buffer = (unsigned char *) av_malloc(
            av_image_get_buffer_size(AV_PIX_FMT_RGB24, 800,
                                     480, 1));
 
    av_image_fill_arrays(pFrameYUV->data, pFrameYUV->linesize, out_buffer,
                         AV_PIX_FMT_RGB24 , 800, 480, 1);
    packet->size = size;
    packet->data = data;
    int sendPacket = avcodec_send_packet(mydecp, packet);

    //return 0 on success, otherwise negative error code:
    if (sendPacket != 0) {
        printf("avodec send packet error %d", sendPacket);
        return  ;
    }
    int receiveFrame = avcodec_receive_frame(mydecp, pFrame);
    if (receiveFrame != 0) {
        //如果接收到的fame不等于0，忽略这次receiver否则会出现绿屏帧
        printf("avcodec_receive_frame error %d", receiveFrame);
        return;
    }
    sws_scale(myswsp, (const uint8_t *const *) pFrame->data, pFrame->linesize,
                    0, mydecp->height,
                    pFrameYUV->data, pFrameYUV->linesize);
    int y_size = 800 * 480;
    memcpy(rgbdata_buf,pFrameYUV->data[0],y_size*3);
    rgb_toargb(rgbdata_buf,argbdata_buf,800,480);
    my_800x480.data = argbdata_buf;
    lv_img_set_src(guider_ui.screen_video_img,&my_800x480);

}