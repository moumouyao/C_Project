#ifndef _APP_H
#define _APP_H

#include "bsp.h"

extern unsigned char g_cTest;//����
extern unsigned char g_cHttpFlag;//TCP�����б�־λ

extern int g_FiCount;//��ȡ����
//������

void Task_LED1(void *p_arg);  
void Task_KEY(void *p_arg); 
extern void TestTxt(void *p_arg);
extern void CorrectFile(char *cmd);
extern unsigned int getSDdata(char *Obuf,char *buf);
extern void init_IP(void);
#endif
