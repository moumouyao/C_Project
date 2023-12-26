#include "main.h"
pthread_t clieat_pthread_pd = 0;
pthread_t initvoide_pd = 0;
bool video_data_send_flag = false;


void * pthread_read_main(void * arg);
int Init_connect(char * host, uint16_t port, char * server_host,uint16_t server_port)
{
    int ret =  0;
    int val = 1;
    struct sockaddr_in addr;
    struct sockaddr_in server_addr;
	//创建套接字
    int sock_fd  = socket(AF_INET,SOCK_STREAM,0);  
	//允许在套接字关闭后立即重新使用相同的地址和端口
    setsockopt(sock_fd,SOL_SOCKET,SO_REUSEADDR,&val,sizeof(val));
	//允许多个套接字绑定到相同的端口
    setsockopt(sock_fd,SOL_SOCKET,SO_REUSEPORT,&val,sizeof(val));
    addr.sin_addr.s_addr = inet_addr(host);
    addr.sin_port = htons(port);
    addr.sin_family = AF_INET;
	//绑定本机的IP和端口号
    ret = bind(sock_fd,(struct sockaddr *)&addr,sizeof(addr));
    if(ret < 0)
    {
        perror("bind");
        printf("绑定IP地址失败！\r\n");
        close(sock_fd);
        return ret;
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(server_host);
    server_addr.sin_port = htons(server_port);
	//连接服务器
    ret = connect(sock_fd,(struct sockaddr *)&server_addr,sizeof(server_addr));
     
    if(ret < 0)
    {
        perror("connect");
        printf("服务器不在线！\r\n");
        close(sock_fd);
        exit(0);
    }
    printf("服务器连接成功!\r\n");
    struct mymessage msg;
    msg.cmd = XYD_CLIENT_CONNECT;
    msg.deviceid = 0xFFFFFFFF;
    msg.message_len = 0;
    mysock_write(sock_fd,&msg,sizeof(msg));//上线验证请求
    usleep(1000);
    mysock_read(sock_fd,&msg,sizeof(msg));
    if(msg.cmd == XYD_SERVER_CONNECT_RES_PASS)//验证服务器返回数据
    {
        printf("连接服务器验证通过  可以正常工作！\r\n");
    }else if(msg.cmd == XYD_SERVER_CONNECT_RES_UNPASS)//非法设备
    {
        printf("此设备ID非法,请联系管理员！\r\n");
        close(sock_fd);
        sock_fd = 0;
        exit(0);
    }
	//处理从服务器介绍的指令
    pthread_create(&clieat_pthread_pd,NULL,pthread_read_main,NULL);
	//视频的采集、编码、推流、保存
    pthread_create(&initvoide_pd,NULL,Init_video_rkmeida,NULL);
    return sock_fd;
}


void * pthread_read_main(void * arg)
{   
    arg = arg;
    struct mymessage msg;
    int ret = 0;
    while(1)
    {
        ret = mysock_read(skd,&msg,sizeof(msg));  
        if(ret != sizeof(msg))
        {
            printf("数据接受长度异常!\r\n");
            if(ret == 0)
            {
                printf("服务器离线!或服务器主动断开!!\r\n");
                exit(0);
            }
        }
        if(msg.cmd == XYD_SERVER_REQ_VIDEO_DATA)//上传视频
        {
            video_data_send_flag = true;
        }else if(msg.cmd == XYD_SERVER_REQ_VIDEO_STOP)//停止视频上传
        {
           video_data_send_flag = false;
        }
    }
    return 0;
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

