#include "sys/types.h"
#include "sys/socket.h"
#include "fcntl.h"
#include "pthread.h"
#include <netdb.h>
#include <netinet/in.h>
#include "stdio.h"
#include "string.h"
#include "stdint.h"
#include "stdlib.h"
#include "custom.h"

int client_count = 0;
int server_fd = 0;
uint8_t h264_databuf[1920*1080*3]={0};
pthread_t pthread_sockpd = 0;
char current_select_strname[32] ={0};
char old_select_strname[32] ={0};
void * pthread_sock_main(void * arg);

int Init_server(char * ip,uint16_t port,char * server_ip,uint16_t server_port)
{
    char passwd[32]={"Ww8718001"};
    int val = 1;//开启
    int ret = 0;
    struct sockaddr_in addr;
    struct sockaddr_in server_addr;
    int sock = socket(AF_INET,SOCK_STREAM,0);
    
    setsockopt(sock,SOL_SOCKET,SO_REUSEADDR,&val,sizeof(val));
    setsockopt(sock,SOL_SOCKET,SO_REUSEPORT,&val,sizeof(val));
    addr.sin_addr.s_addr = inet_addr(ip);
    addr.sin_port = htons(port);
    addr.sin_family = AF_INET;
    ret = bind(sock,(struct sockaddr *)&addr,sizeof(addr));
    if(ret < 0)
    {
        perror("bind");
        printf("绑定IP地址失败！\r\n");
        close(sock);
        return ret;
    }
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(server_ip);
    server_addr.sin_port = htons(server_port);
    ret = connect(sock,(struct sockaddr *)&server_addr,sizeof(server_addr));
    
    if(ret < 0)
    {
        perror("connect");
        printf("服务器不在线！\r\n");
        close(sock);
        exit(0);
    }
    printf("服务器连接成功!\r\n");
    struct mymessage msg;
    msg.cmd = XYD_BOSS_ONLINE;
    msg.deviceid = 0xFFFFFFFF;
    //msg.message_len = 32;
    msg.message_len = 0;
    write(sock,&msg,sizeof(msg));
    //write(sock,passwd,32);
    pthread_create(&pthread_sockpd,NULL,pthread_sock_main,(void * )sock);
    return sock;

}


void * pthread_sock_main(void * arg)
{
    int retlen  = 0;
    int skd = (int) arg;
    struct mymessage msg;
    while(1)
    {
       read(skd,&msg,sizeof(msg));  
       if(msg.cmd == XYD_BOSS_DEIVCE_ERROR)
       {
            printf("账号密码错误！\r\n");
            close(skd);
            exit(0);
       }else if(msg.cmd == XYD_BOSS_BE_OCCUPIED)
       {
            printf("账户已有人登陆！\r\n");
            close(skd);
            exit(0);
       }else if(msg.cmd == XYD_BOSS_ONLINE_CLIENT_DEVICEID)
       {
            
            char * name = bydeviceid_get_database_name(msg.deviceid);
            printf("name==%s\r\n",name);
            mylist_add(name,msg.deviceid);
            //显示到界面
            add_show_list(name);
            client_count++;
            
       }else if(msg.cmd == XYD_CLIENT_DOWN)
       {
            char * name = bydeviceid_get_database_name(msg.deviceid);
            printf("name==%s下线\r\n",name);
            del_show_list(name);
            mylist_remove(msg.deviceid);
            client_count--;
       }else if(msg.cmd == XYD_CLIENT_VIDEO_DATA)
       {
            retlen = mysock_read(skd,h264_databuf,msg.message_len);
            if(retlen !=msg.message_len)
            {
                printf("视频数据读取失败！\r\n");
            }
            show_video_to_img(guider_ui.screen_video_img,h264_databuf,retlen);
       }
    }

}


int mysock_read(int sockfd,void * data,int size)
{
    int len = size;
    int ret = 0;
    uint8_t * p = data;
    while(1)
    {
        ret = read(sockfd,p,len);
        if(ret < 0)
        {
            perror("read");
            printf("接受数据失败！\r\n");
            return ret;
        }else if(ret == 0)
        {
            printf("客户离线!\r\n");
            close(sockfd);
            return 0;
        }else 
        {

            len = len - ret;
            p+=ret;
            if(len == 0)
            {
                break;
            }
        }

    }
    return size;
}

int mysock_write(int sockfd,void * data,int size)
{
    int len = size;
    int ret = 0;
    uint8_t * p = data;
    while(1)
    {
        ret = write(sockfd,p,len);
        if(ret < 0)
        {
            perror("write");
            printf("发送数据失败！\r\n");
            return ret;
        }else if(ret == 0)
        {
            printf("客户离线!\r\n");
            close(sockfd);
            return 0;
        }else 
        {

            len = len - ret;
            p+=ret;
            if(len == 0)
            {
                //printf("数据发送完毕！\r\n");
                break;
            }
        }

    }
    return size;
}