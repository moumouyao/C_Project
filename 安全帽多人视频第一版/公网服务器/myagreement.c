#include "main.h"


void * pthread_send_boss_msg(void * arg);
void deal_video_data(struct mymessage * msg,int ckd)
{
    int ret = 0;
    int readlen = msg->message_len;
    ret = mysock_read(ckd,rx_buf,readlen);
    if(ret != readlen)
    {
        printf("客户视频数据接受失败！\r\n");
        if(ret == 0)
        {
            //FD_CLR(ckd,&locklist);
            //close(ckd);
            //mylist_remove(ckd);
        }
        return ;
    }
    if(boss_ckd <= 0 )
    {
        printf("老板端不在线！\r\n");
        msg->cmd = XYD_SERVER_REQ_VIDEO_STOP;
        msg->message_len = 0;
        mysock_write(ckd,msg,sizeof(struct mymessage));
        return;

    }
    mysock_write(boss_ckd,msg,sizeof(struct mymessage));
    mysock_write(boss_ckd,rx_buf,readlen);
}
void deal_client_down(struct mymessage * msg,int ckd)
{
    struct mymessage readmsg;
    msg = msg;
    FD_CLR(ckd,&locklist);
    if(MAX == ckd)
    {
        MAX--;
    }
    close(ckd);
    if(boss_ckd > 0)
    {
        readmsg.deviceid = bydeviceid_get_ckd(ckd);
        readmsg.cmd = XYD_CLIENT_DOWN;
        mysock_write(boss_ckd,&readmsg,sizeof(readmsg));
    } 
    mylist_remove(ckd);
}
void deal_boss_online_client(struct mymessage * msg,int ckd)
{
    msg = msg;
    printf("deviceid == %x\r\n",msg->deviceid);
    int flag = verify_bossdeviceid(msg->deviceid);
    if(flag == 0)
    {
        if(boss_ckd > 0)
        {
            printf("已有老板在线！\r\n");
            msg->cmd = XYD_BOSS_BE_OCCUPIED;
            mysock_write(ckd,msg,sizeof(struct mymessage));
            usleep(10*1000);
            close(ckd);
            return ;
        }
        printf("老板端上线！\r\n");
        FD_SET(ckd,&locklist);
        if(ckd > MAX)
        {
            MAX = ckd;
        }
        boss_ckd = ckd;
        pthread_t pd = 0; 
        pthread_create(&pd,NULL,pthread_send_boss_msg,NULL);
    }else
    {   

        msg->cmd = XYD_BOSS_DEIVCE_ERROR;
        mysock_write(ckd,msg,sizeof(struct mymessage));
        usleep(10*1000);
        close(ckd);
        printf("老板端设备ID错误，请联系管理员！\r\n");
        return ;
    }
   
    
}
void deal_boss_offline_client(struct mymessage * msg,int ckd)
{
    msg = msg;
    FD_CLR(ckd,&locklist);
    if(MAX == ckd)
    {
        MAX--;
    }
    close(ckd);
    boss_ckd = 0;
    printf("老板下线!\r\n");
    if(MAX == ckd)
    {
        MAX--;
    }
}


void deal_boss_request_video_data(struct mymessage * msg,int ckd)
{
    int client_ckd = byckd_get_device_id(msg->deviceid);
    mysock_write(client_ckd,msg,sizeof(struct mymessage));
}
void deal_boss_stop_video(struct mymessage * msg,int ckd)
{
    int client_ckd = byckd_get_device_id(msg->deviceid);
    mysock_write(client_ckd,msg,sizeof(struct mymessage));
}
void * pthread_send_boss_msg(void * arg)
{
    struct mymessage smsg ;
    struct client_struct * lastp = list_head; 
    while(lastp!=NULL)
    {
        smsg.cmd = XYD_BOSS_ONLINE_CLIENT_DEVICEID;
        smsg.deviceid = lastp->device_id;
        smsg.message_len = 0;
        mysock_write(boss_ckd, &smsg, sizeof(struct mymessage));
        lastp = lastp->pnext;
        usleep(100000);
    }

}