#include "main.h"


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
            //printf("客户离线!\r\n");
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
