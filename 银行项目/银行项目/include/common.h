//所有.c要用到的头文件
#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <glob.h>
//传递信息要用到的结构类型
#define C2SKEY 0X12345//客户端向服务器发送
#define S2CKEY 0X54321//服务器向客户端发送
	//开户 存款 取款 查询 转账 销户 退出
enum opt {OPEN = 1, DEP, DISDEP, CHECK, TRANS, DELAC, QUIT};

struct DATA{    
    int id;//账号
    char name[20];//姓名
    char passwd[20];//密码
    float money;//余额
    int o_id;//收款方账号（只有转账用）
    pid_t pid;//客户端的pid号
};

struct C2SMSG{
    long type;//客户端向服务器发送消息的类型
    struct DATA data;//消息内容
};

struct S2CMSG{
    long type;//服务器回复客户端的消息类型
    int id;//账号（开户使用）
    float money;//余额（和钱有关的操作都要使用）
    char info[32];//提示信息
};

