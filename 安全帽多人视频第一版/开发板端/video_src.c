#include "main.h"
#include <time.h>
#include <unistd.h>
#include "common/sample_common.h"
#include "rkmedia_api.h"
#include "rkmedia_venc.h"
bool quit = false;
static FILE *g_output_file;
static RK_S32 g_s32FrameCnt = -1;


void myargeement_packing(uint8_t * h264_data,int size)
{

    struct mymessage msg = {0};
    msg.cmd = XYD_CLIENT_VIDEO_DATA;
    msg.deviceid = 0xFFFFFFFF;
    msg.message_len = size;
    memset(tx_buf,0,size+sizeof(struct mymessage) + 10);
    memcpy(tx_buf,&msg,sizeof(struct mymessage));
    memcpy(tx_buf+sizeof(struct mymessage),h264_data,size);
}


void video_packet_cb(MEDIA_BUFFER mb) {
  static RK_S32 packet_cnt = 0;
  if (quit)
    return;
    
  const char *nalu_type = "Jpeg data";
  switch (RK_MPI_MB_GetFlag(mb)) {
  case VENC_NALU_IDRSLICE:
    nalu_type = "IDR Slice";
    break;
  case VENC_NALU_PSLICE:
    nalu_type = "P Slice";
    break;
  default:
    break;
  }
  if(video_data_send_flag == true)
  {
    myargeement_packing(RK_MPI_MB_GetPtr(mb),RK_MPI_MB_GetSize(mb));
    mysock_write(skd,tx_buf,RK_MPI_MB_GetSize(mb)+sizeof(struct mymessage));
  }
  
  if (g_output_file) {
    
    fwrite(RK_MPI_MB_GetPtr(mb), 1, RK_MPI_MB_GetSize(mb), g_output_file);
    printf("#Write packet-%d, %s, size %zu\n", packet_cnt, nalu_type,
           RK_MPI_MB_GetSize(mb));
  } else {
    printf("#Get packet-%d, %s, size %zu\n", packet_cnt, nalu_type,
           RK_MPI_MB_GetSize(mb));
  }
  RK_MPI_MB_TsNodeDump(mb);
  RK_MPI_MB_ReleaseBuffer(mb);

  packet_cnt++;
  if ((g_s32FrameCnt >= 0) && (packet_cnt > g_s32FrameCnt))
    quit = true;
}

void *  Init_video_rkmeida(void * arg)
{
    arg = arg;
    int ret;
    RK_U32 u32Width = 1920;
    RK_U32 u32Height = 1080;
    RK_CHAR *pDeviceName = "rkispp_scale0";
    RK_CHAR *pOutPath = "./out.h264";
    RK_CHAR *pIqfilesPath = "/oem/etc/iqfiles";
    CODEC_TYPE_E enCodecType = RK_CODEC_TYPE_H264;
    RK_CHAR *pCodecName = "H264";
    RK_S32 s32CamId = 0;
    RK_U32 u32BufCnt = 3;
    
    printf("#Device: %s\n", pDeviceName);
    printf("#CodecName:%s\n", pCodecName);
    printf("#Resolution: %dx%d\n", u32Width, u32Height);
    printf("#Frame Count to save: %d\n", g_s32FrameCnt);
    printf("#Output Path: %s\n", pOutPath);
    printf("#CameraIdx: %d\n\n", s32CamId);
    RK_BOOL bMultictx = RK_FALSE;

    
    RK_U32 u32Fps = 30;
#ifdef RKAIQ

    rk_aiq_working_mode_t hdr_mode = RK_AIQ_WORKING_MODE_NORMAL;
    SAMPLE_COMM_ISP_Init(s32CamId, hdr_mode, bMultictx, pIqfilesPath);
    SAMPLE_COMM_ISP_Run(s32CamId);
    SAMPLE_COMM_ISP_SetFrameRate(s32CamId, u32Fps);
#endif
    if (pOutPath) {
        g_output_file = fopen(pOutPath, "w");
        if (!g_output_file) {
            printf("ERROR: open file: %s fail, exit\n", pOutPath);
            pthread_exit(NULL);
        }
    }
	//初始化rkmedia
    RK_MPI_SYS_Init();
    VI_CHN_ATTR_S vi_chn_attr;
    vi_chn_attr.pcVideoNode = pDeviceName;
    vi_chn_attr.u32BufCnt = u32BufCnt;
    vi_chn_attr.u32Width = u32Width;
    vi_chn_attr.u32Height = u32Height;
    vi_chn_attr.enPixFmt = IMAGE_TYPE_NV12;
    vi_chn_attr.enBufType = VI_CHN_BUF_TYPE_MMAP;
    vi_chn_attr.enWorkMode = VI_WORK_MODE_NORMAL;
    ret = RK_MPI_VI_SetChnAttr(s32CamId, 0, &vi_chn_attr);//初始化vi通道
    ret |= RK_MPI_VI_EnableChn(s32CamId, 0);//使能vi通道
    if (ret) {
        printf("ERROR: create VI[0] error! ret=%d\n", ret);
        pthread_exit(NULL);
    }

    VENC_CHN_ATTR_S venc_chn_attr;
    memset(&venc_chn_attr, 0, sizeof(venc_chn_attr));
    switch (enCodecType) {
    case RK_CODEC_TYPE_H264:
    default:
        printf("111111111111111111111111111111111111\r\n");
        printf("u32Width=+%d **%d \r\n",u32Width,u32Height);
        venc_chn_attr.stVencAttr.enType = RK_CODEC_TYPE_H264;
        venc_chn_attr.stRcAttr.enRcMode = VENC_RC_MODE_H264CBR;
        venc_chn_attr.stRcAttr.stH264Cbr.u32Gop = 30;
        venc_chn_attr.stRcAttr.stH264Cbr.u32BitRate = u32Width * u32Height;
        // frame rate: in 30/1, out 30/1.
        venc_chn_attr.stRcAttr.stH264Cbr.fr32DstFrameRateDen = 1;
        venc_chn_attr.stRcAttr.stH264Cbr.fr32DstFrameRateNum = 30;
        venc_chn_attr.stRcAttr.stH264Cbr.u32SrcFrameRateDen = 1;
        venc_chn_attr.stRcAttr.stH264Cbr.u32SrcFrameRateNum = 30;
        break;
    }
    venc_chn_attr.stVencAttr.imageType = IMAGE_TYPE_NV12;
    venc_chn_attr.stVencAttr.u32PicWidth = u32Width;
    venc_chn_attr.stVencAttr.u32PicHeight = u32Height;
    venc_chn_attr.stVencAttr.u32VirWidth = u32Width;
    venc_chn_attr.stVencAttr.u32VirHeight = u32Height;
    venc_chn_attr.stVencAttr.u32Profile = 77;
    ret = RK_MPI_VENC_CreateChn(0, &venc_chn_attr);//创建编码通道
    if (ret) {
        printf("ERROR: create VENC[0] error! ret=%d\n", ret);
        pthread_exit(NULL);
    }

    MPP_CHN_S stEncChn;
    stEncChn.enModId = RK_ID_VENC;
    stEncChn.s32DevId = 0;
    stEncChn.s32ChnId = 0;
    ret = RK_MPI_SYS_RegisterOutCb(&stEncChn, video_packet_cb);//注册编码通道的回调函数
    if (ret) {
        printf("ERROR: register output callback for VENC[0] error! ret=%d\n", ret);
        pthread_exit(NULL);
    }

    MPP_CHN_S stSrcChn;
    stSrcChn.enModId = RK_ID_VI;
    stSrcChn.s32DevId = 0;
    stSrcChn.s32ChnId = 0;
    MPP_CHN_S stDestChn;
    stDestChn.enModId = RK_ID_VENC;
    stDestChn.s32DevId = 0;
    stDestChn.s32ChnId = 0;
    ret = RK_MPI_SYS_Bind(&stSrcChn, &stDestChn);//绑定
    if (ret) {
        printf("ERROR: Bind VI[0] and VENC[0] error! ret=%d\n", ret);
        pthread_exit(NULL);
    }

    printf("%s initial finish\n", __func__);
    //signal(SIGINT, sigterm_handler);

    while (!quit) {
        usleep(10000);
    }

    if (g_output_file)
        fclose(g_output_file);

    printf("%s exit!\n", __func__);
    // unbind first
    ret = RK_MPI_SYS_UnBind(&stSrcChn, &stDestChn);//解绑
    if (ret) {
        printf("ERROR: UnBind VI[0] and VENC[0] error! ret=%d\n", ret);
        pthread_exit(NULL);
    }
    // destroy venc before vi
    ret = RK_MPI_VENC_DestroyChn(0);//销毁编码通道
    if (ret) {
        printf("ERROR: Destroy VENC[0] error! ret=%d\n", ret);
        pthread_exit(NULL);
    }
    // destroy vi
    ret = RK_MPI_VI_DisableChn(s32CamId, 0);//失能vi通道
    if (ret) {
        printf("ERROR: Destroy VI[0] error! ret=%d\n", ret);
        pthread_exit(NULL);
    }

    if (pIqfilesPath) {
    #ifdef RKAIQ
        SAMPLE_COMM_ISP_Stop(s32CamId);
    #endif
    }
    pthread_exit(NULL);
}
