#ifndef _APP_H
#define _APP_H

#include "bsp.h"

extern unsigned char g_cTest;//测试
extern unsigned char g_cHttpFlag;//TCP发送中标志位

extern int g_FiCount;//读取数量
//任务函数

void Task_LED1(void *p_arg);  
void Task_KEY(void *p_arg); 
extern void TestTxt(void *p_arg);
extern void CorrectFile(char *cmd);
extern unsigned int getSDdata(char *Obuf,char *buf);
extern void init_IP(void);
#endif
