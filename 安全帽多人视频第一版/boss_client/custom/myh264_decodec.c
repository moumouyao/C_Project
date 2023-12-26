#include "custom.h"
#include "unistd.h"
uint8_t argbdata_buf[800*480*4]={0};
uint8_t rgbdata_buf[800*480*3]={0};
uint8_t yuv420_buf[800*480*2]={0};
AVCodecContext * mydecp=NULL;
struct SwsContext * myswsp=NULL;


LV_ATTRIBUTE_LARGE_CONST lv_img_dsc_t my_800x480 = {
  .header.always_zero = 0,
  .header.w = 800,
  .header.h = 480,
  .data_size = 384000 * LV_COLOR_SIZE / 8,
  .header.cf = LV_IMG_CF_TRUE_COLOR_ALPHA,
  .data = NULL,
};

AVCodecContext *  open_decoder(void)
{
    AVCodec * codec = avcodec_find_decoder(AV_CODEC_ID_H264);//获取264编码器
    if(codec == NULL)
    {
        printf("h264解码器未找到\r\n");
        exit(0);
    }
    AVCodecContext * codec_context = avcodec_alloc_context3(codec);//开辟上下文指针
    //SPS/PPS信息设置 
    codec_context->profile = FF_PROFILE_H264_MAIN;//压缩等级
    codec_context->level  = 50;//标识level为 5.0 
    codec_context->height = 1080;
    codec_context->width = 1920;
    //GOP相关的
    
    codec_context->gop_size = 250;//组大小 
    codec_context->keyint_min = 30;//最小25帧插入一个 I 帧 
    //B帧 
    codec_context->max_b_frames = 3;//不超过3帧
    codec_context->has_b_frames = 1;
    //参考帧 
    codec_context->refs = 3;
    //设置输出YUV格式 
    codec_context->pix_fmt = AV_PIX_FMT_YUV420P;
    //设置码率
    codec_context->bit_rate = 1920*1080;//600kbps
    //设置帧率  
    codec_context->time_base = (AVRational){1,30};//帧与帧之间的间隔
    codec_context->framerate = (AVRational){30,1};//帧率每秒25帧
    if(codec_context == NULL)
    {
        printf("libx264上下文指针空间开辟失败\r\n");
        exit(0);
    }

    int ret = avcodec_open2(codec_context,codec,NULL);
    if(ret < 0)
    {
        printf("解码器打开失败！\r\n");
        return NULL;
    }    
    return codec_context;
}

struct SwsContext * open_mysws(int oldw,int oldh,int w,int h)
{

    struct SwsContext *img_convert_ctx = sws_getContext(oldw, oldh,
                                                        AV_PIX_FMT_YUV420P,
                                                        w, h,
                                                        AV_PIX_FMT_RGB24,
                                                        SWS_BICUBIC, NULL, NULL, NULL);
    return img_convert_ctx;
}
void rgb_toargb(uint8_t * rgbdata,uint8_t * argbdata,int w,int h)
{
    int i =0,j=0;
    for(i = 0 ; i < h;i++)
    {
        for(j = 0; j<w;j++)
        {
            argbdata[j*4+i*w*4 + 3] = 0xFF;
            argbdata[j*4+i*w*4 + 0] = rgbdata[j*3+i*w*3 + 2];
            argbdata[j*4+i*w*4 + 1] = rgbdata[j*3+i*w*3 + 1];
            argbdata[j*4+i*w*4 + 2] = rgbdata[j*3+i*w*3 + 0];

        }
    }
}