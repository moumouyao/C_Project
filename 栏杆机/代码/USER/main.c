#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "lwip_comm.h"
#include "LAN8720.h"
#include "malloc.h"
#include "lwip_comm.h"
#include "includes.h"
#include "lwipopts.h"
#include "udp_demo.h"
#include "i2c.h"
#include <sw_conf.h>
#include "sw_global.h"
#include "bsp.h"
#include "sw_app.h"

#include <stdio.h>

//LEDRGB任务
//任务优先级
#define Write_RGB_TASK_PRIO		7
//任务堆栈大小
#define Write_RGB_STK_SIZE		1500
//任务堆栈
OS_STK	Write_RGB_TASK_STK[Write_RGB_STK_SIZE];
//任务函数 
void Write_RGB(void *pdata);

//LED任务
//任务优先级
#define LED_TASK_PRIO		15
//任务堆栈大小
#define LED_STK_SIZE		128
//任务堆栈
OS_STK	LED_TASK_STK[LED_STK_SIZE];
//任务函数
void led_task(void *pdata);

//START任务
//任务优先级
#define START_TASK_PRIO		11
//任务堆栈大小
#define START_STK_SIZE		128
//任务堆栈
OS_STK START_TASK_STK[START_STK_SIZE];
//任务函数
void start_task(void *pdata);

//TestCode任务
//任务优先级
#define TestCode_PRIO		9
//任务堆栈大小
#define TestCode_STK_SIZE		2500
//任务堆栈
OS_STK TestCode_TASK_STK[TestCode_STK_SIZE];
//任务函数
void TestCode(void *pdata);

//Testkey任务
//任务优先级
#define TestCode_KEY		8
//任务堆栈大小
#define TestCode_KEY_SIZE		512
//任务堆栈
OS_STK TestCode_KEY_STK[TestCode_KEY_SIZE];
//任务函数
void key_task(void *pdata);

//TASK_OverDataHandle任务
//任务优先级
#define TASK_OVER_DATA_PRIO		14
//任务堆栈大小
#define TASK_OverDataHandle_SIZE		1500
//任务堆栈
OS_STK TASK_OverDataHandle_STK[TASK_OverDataHandle_SIZE];
//任务函数
void TASK_OverDataHandle(void *pdata);

//报文解析
void     * En_Dis_CodeQeueTab[En_Dis_Code_QEUE_LENGTH];
OS_EVENT * En_Dis_CodeQeue;//上位机发送至解析报文消息队列

void     * En_Udp_CodeQeueTab[En_Udp_Code_QEUE_LENGTH];
OS_EVENT * En_Udp_CodeQeue;//解析报文任务发送到UDP消息队列

OS_EVENT * ZMSem;               //字模数组资源信号量

int main(void)
{	
	BSP_Init();
	OSInit(); 					//UCOS初始化	
	En_Dis_CodeQeue = OSQCreate(&En_Dis_CodeQeueTab[0],En_Dis_Code_QEUE_LENGTH);//用于上位机发送至解析报文任务
	En_Udp_CodeQeue = OSQCreate(&En_Udp_CodeQeueTab[0],En_Udp_Code_QEUE_LENGTH);//用于解析报文任务发送到UDP任务
	
  ZMSem = OSSemCreate(1); 
	
	while(lwip_comm_init()); 	//lwip初始化
	while(udp_demo_init()); 										//初始化udp_demo(创建udp_demo线程)
	OSTaskCreate(start_task,(void*)0,(OS_STK*)&START_TASK_STK[START_STK_SIZE-1],START_TASK_PRIO);
	OSStart(); //开启UCOS
}

//start任务
void start_task(void *pdata)
{
	OS_CPU_SR cpu_sr;
	pdata = pdata ;
	OSStatInit();  			//初始化统计任务
	OS_ENTER_CRITICAL();  	//关中断

	OSTaskCreate(led_task,(void*)0,(OS_STK*)&LED_TASK_STK[LED_STK_SIZE-1],LED_TASK_PRIO); 	//创建LED任务
	OSTaskCreate(TestCode,(void*)0,(OS_STK*)&TestCode_TASK_STK[TestCode_STK_SIZE-1],TestCode_PRIO); 	//创建解析播放表任务
	OSTaskCreate(Write_RGB,(void*)0,(OS_STK*)&Write_RGB_TASK_STK[Write_RGB_STK_SIZE-1],Write_RGB_TASK_PRIO); 	//创建显示任务
	OSTaskCreate(key_task,(void*)0,(OS_STK*)&TestCode_KEY_STK[TestCode_KEY_SIZE-1],TestCode_KEY); 	//创建key检测任务
#if shihangzuoyi
//	OSTaskCreate(TASK_OverDataHandle,(void*)0,(OS_STK*)&TASK_OverDataHandle_STK[TASK_OverDataHandle_SIZE-1],TASK_OVER_DATA_PRIO); 	//创建左移任务
#endif

	OSTaskSuspend(OS_PRIO_SELF); //挂起start_task任务
	OS_EXIT_CRITICAL();  //开中断
}
