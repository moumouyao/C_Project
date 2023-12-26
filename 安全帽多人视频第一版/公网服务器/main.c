#include "main.h"
pthread_t connect_pd = 0;
struct client_struct * list_head = NULL; 
int boss_ckd = 0;//老板套接子
int server_skd = 0;//服务器套接字
int client_ckd=0;//客户端套接字
int count = 0;//当前连接人数
uint8_t tx_buf[1920*1080*2]={0};//发送缓冲区
uint8_t rx_buf[1920*1080*2]={0};//接受缓冲区
fd_set locklist,list;
int MAX;
int main(int argc,char * argv[])
{
    int val = 1;//开启
    int ret = 0;
    struct sockaddr_in addr;

    server_skd = socket(AF_INET,SOCK_STREAM,0);
    
    setsockopt(server_skd,SOL_SOCKET,SO_REUSEADDR,&val,sizeof(val));
    setsockopt(server_skd,SOL_SOCKET,SO_REUSEPORT,&val,sizeof(val));
    addr.sin_addr.s_addr = inet_addr("172.26.9.81");
    addr.sin_port = htons(55555);
    addr.sin_family = AF_INET;
    ret = bind(server_skd,(struct sockaddr *)&addr,sizeof(addr));
    if(ret < 0)
    {
        perror("bind");
        printf("绑定IP地址失败！\r\n");
        close(server_skd);
        return ret;
    }
    ret = listen(server_skd,64);
    if(ret < 0)
    {
        perror("listen");
        printf("监听失败！\r\n");
        close(server_skd);
        return ret;
    }

    printf("服务器创建完成 开始监听！！\r\n");
    struct sockaddr_in clientaddr;
    socklen_t client_len = sizeof(clientaddr);
    struct timeval selecttimeout;
    selecttimeout.tv_sec = 0;
    selecttimeout.tv_usec = 100000;
    
    MAX = server_skd;
    FD_ZERO(&locklist);
    FD_SET(server_skd,&locklist);
    while(1)
    {
        list = locklist;
        ret = select(MAX+1,&list,NULL,NULL,&selecttimeout);
        if(ret == 0)//超时
        {
            list = locklist;
        }else if(ret > 0)
        {
            printf("监测到客户！\r\n");
            for(int i=3;i<=MAX;i++)
            {
                if(FD_ISSET(i,&list))
                {
                    if(i == server_skd)//有人连接
                    {
                        client_ckd = accept(server_skd,(struct sockaddr *)&clientaddr,&client_len);
                        pthread_create(&connect_pd,NULL,pthread_client_connect,(void *)client_ckd);
                    }else
                    {
                        struct mymessage readmsg;
                        ret = mysock_read(i,&readmsg,sizeof(readmsg));
                        if(ret != sizeof(readmsg))
                        {
                            if(ret == 0)
                            {
                                if(i == boss_ckd)
                                {
                                    boss_ckd = 0;
                                    printf("老板下线!\r\n");
                                    
                                }else
                                {
                                   if(boss_ckd > 0)
                                   {
                                        readmsg.deviceid = bydeviceid_get_ckd(i);
                                        readmsg.cmd = XYD_CLIENT_DOWN;
                                        mysock_write(boss_ckd,&readmsg,sizeof(readmsg));
                                   } 
                                   
                                   mylist_remove(i);     
                                }
                                FD_CLR(i,&locklist);
                                close(i);
                                if(MAX == i)
                                {
                                    MAX--;
                                }
                                continue;
                            }
                            printf("读取错误! 或者离线\r\n");
                            continue;
                        }
                        analyse_msg(&readmsg,i);


                    }
                }
            }
        }
    }
    return 0;
}


void * pthread_client_connect(void * arg)
{
    int ret = 0;
    int ckd = (int)arg;
    struct mymessage readmsg;
    ret = mysock_read(ckd,&readmsg,sizeof(readmsg));
    if(ret != sizeof(readmsg))
    {
        printf("读取错误!\r\n");
        close(ckd);
        return NULL;
    }
    if(readmsg.cmd == XYD_CLIENT_CONNECT)
    {
        int flag = verify_deviceid(readmsg.deviceid);
        if(flag == 0)
        {
            printf("客户%d上线！\r\n",ckd);
            FD_SET(ckd,&locklist);
            if(ckd > MAX)
            {
                MAX = ckd;
            }
            readmsg.cmd = XYD_SERVER_CONNECT_RES_PASS;
            mysock_write(ckd,&readmsg,sizeof(readmsg));
            mylist_add(ckd,readmsg.deviceid);
            if(boss_ckd > 0)
            {
                readmsg.cmd = XYD_BOSS_ONLINE_CLIENT_DEVICEID;
                mysock_write(boss_ckd,&readmsg,sizeof(readmsg));
            }
        }else  
        {
            printf("此设备ID非法,请联系管理员！\r\n");
            readmsg.cmd = XYD_SERVER_CONNECT_RES_UNPASS;
            mysock_write(ckd,&readmsg,sizeof(readmsg));
            sleep(1);
            close(ckd);
            return NULL;
        }
    }else if(readmsg.cmd == XYD_BOSS_ONLINE)
	{
		deal_boss_online_client(&readmsg,ckd);
	}

}

//分析数据做出相应动作
void analyse_msg(struct mymessage * msg,int ckd)
{
    switch (msg->cmd)
    {
    case XYD_CLIENT_VIDEO_DATA:
        deal_video_data(msg,ckd);
        break;
    //case XYD_CLIENT_CONNECT:
    //    deal_client_connect();
    //    break;
    case XYD_CLIENT_DOWN:
        deal_client_down(msg,ckd);
        break;
    // case XYD_BOSS_ONLINE:
    //    deal_boss_online_client(msg,ckd);
    //    break;
    case XYD_BOSS_OFFLINE:
        deal_boss_offline_client(msg,ckd);
        break;
    case XYD_SERVER_REQ_VIDEO_DATA:
        deal_boss_request_video_data(msg,ckd);
        break;
    case XYD_SERVER_REQ_VIDEO_STOP:
        deal_boss_stop_video(msg,ckd);
        break;
    default:
        printf("指令错误！\r\n");
        break;
    }
}



