#include "main.h"
extern bool quit;
uint8_t tx_buf[1920*1080*2] ={0}; 
int skd = 0;

void exit_clean(void)
{
    quit = true;   
    if(skd > 0)
    {
        close(skd);
    }
    usleep(100000);
    exit(0);
}

void signal_exit_backfuntion(int signum)
{
    signum = signum;
    quit = true;
    if(skd > 0)
    {
        close(skd);
    }
    usleep(100000);
    exit(0);
}

int main(int argc, char *argv[])
{
    argc = argc;
    argv = argv;
    atexit(exit_clean);
    signal(2,signal_exit_backfuntion);
    skd = Init_connect("192.168.31.151",22222,"172.26.9.81",55555);//初始化套接子 连接服务器 
    if(skd <= 0)
    {
        printf("服务器不在线或者网络有问题！\r\n");
        exit(0);
    }
    printf("连接服务器成功！开始正常工作\r\n");
    while(1)
    {
        sleep(1);
    }
    return 0;
}