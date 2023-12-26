#ifndef _MAIN_H_
#define _MAIN_H_
#include <assert.h>
#include <fcntl.h>
#include <getopt.h>
#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include "sys/types.h"
#include "sys/socket.h"
#include "fcntl.h"
#include "pthread.h"
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "sys/select.h"
#include <mysql/mysql.h>
#include "asm-generic/socket.h"
#define XYD_CLIENT_CONNECT              0x01 //上线验证请求
#define XYD_CLIENT_DOWN                 0x02 //下线通知
#define XYD_SERVER_CONNECT_RES_PASS     0x03 //服务器验证上线通过
#define XYD_SERVER_CONNECT_RES_UNPASS   0x04 //服务器验证上线未通过--设备非法
#define XYD_SERVER_DOWN_RES             0x05 //服务器下线响应
#define XYD_SERVER_REQ_VIDEO_DATA       0x06 //服务器请求摄像头数据
#define XYD_SERVER_REQ_VIDEO_STOP       0x07 //服务器停止请求摄像头数据
#define XYD_SERVER_VIDEO_DATA_RES       0x08 //服务器接收到摄像头数据响应
#define XYD_CLIENT_VIDEO_DATA           0x09 //客户端发送摄像头数据


#define XYD_BOSS_ONLINE_CLIENT_DEVICEID 0xFB //服务器 发给老板端 在线的工人设备ID
#define XYD_BOSS_DEIVCE_ERROR           0xFC //老板客户端设备ID错误
#define XYD_BOSS_BE_OCCUPIED            0xFD //老板客户端被占用(已经上线过了 不允许多个老板客户端在线)
#define XYD_BOSS_ONLINE                 0xFF //老板客户端认证
#define XYD_BOSS_OFFLINE                0xFE //老板客户端离线


struct client_struct{
    int ckd;
    //char name[128];
    uint32_t device_id;
    struct client_struct * pnext;
};
struct mymessage
{
    uint32_t deviceid;
    uint8_t cmd;
    uint32_t message_len;
};
extern int MAX;
extern struct client_struct * list_head;
extern fd_set locklist,list;
extern pthread_t connect_pd;
extern uint8_t tx_buf[1920*1080*2];//发送缓冲区
extern uint8_t rx_buf[1920*1080*2];//接受缓冲区
extern int server_skd;//服务器套接字
extern int client_ckd;//客户端套接字
extern int boss_ckd;
extern int count;//当前连接人数
void * pthread_client_connect(void * arg);
void analyse_msg(struct mymessage * msg,int ckd);
int mysock_write(int sockfd,void * data,int size);
int mysock_read(int sockfd,void * data,int size);
void deal_video_data(struct mymessage * msg,int ckd);
void deal_client_down(struct mymessage * msg,int ckd);
void deal_boss_online_client(struct mymessage * msg,int ckd);
void deal_boss_offline_client(struct mymessage * msg,int ckd);
void deal_boss_stop_video(struct mymessage * msg,int ckd);
void deal_boss_request_video_data(struct mymessage * msg,int ckd);

int verify_deviceid(uint32_t device_id);
int verify_bossdeviceid(uint32_t device_id);

void mylist_add(int ckd,uint32_t device_id);
void mylist_remove(int ckd);
int byckd_get_device_id(uint32_t device_id);
uint32_t bydeviceid_get_ckd(int ckd);

#endif

