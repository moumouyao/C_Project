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

//LEDRGB����
//�������ȼ�
#define Write_RGB_TASK_PRIO		7
//�����ջ��С
#define Write_RGB_STK_SIZE		1500
//�����ջ
OS_STK	Write_RGB_TASK_STK[Write_RGB_STK_SIZE];
//������ 
void Write_RGB(void *pdata);

//LED����
//�������ȼ�
#define LED_TASK_PRIO		15
//�����ջ��С
#define LED_STK_SIZE		128
//�����ջ
OS_STK	LED_TASK_STK[LED_STK_SIZE];
//������
void led_task(void *pdata);

//START����
//�������ȼ�
#define START_TASK_PRIO		11
//�����ջ��С
#define START_STK_SIZE		128
//�����ջ
OS_STK START_TASK_STK[START_STK_SIZE];
//������
void start_task(void *pdata);

//TestCode����
//�������ȼ�
#define TestCode_PRIO		9
//�����ջ��С
#define TestCode_STK_SIZE		2500
//�����ջ
OS_STK TestCode_TASK_STK[TestCode_STK_SIZE];
//������
void TestCode(void *pdata);

//Testkey����
//�������ȼ�
#define TestCode_KEY		8
//�����ջ��С
#define TestCode_KEY_SIZE		512
//�����ջ
OS_STK TestCode_KEY_STK[TestCode_KEY_SIZE];
//������
void key_task(void *pdata);

//TASK_OverDataHandle����
//�������ȼ�
#define TASK_OVER_DATA_PRIO		14
//�����ջ��С
#define TASK_OverDataHandle_SIZE		1500
//�����ջ
OS_STK TASK_OverDataHandle_STK[TASK_OverDataHandle_SIZE];
//������
void TASK_OverDataHandle(void *pdata);

//���Ľ���
void     * En_Dis_CodeQeueTab[En_Dis_Code_QEUE_LENGTH];
OS_EVENT * En_Dis_CodeQeue;//��λ������������������Ϣ����

void     * En_Udp_CodeQeueTab[En_Udp_Code_QEUE_LENGTH];
OS_EVENT * En_Udp_CodeQeue;//�������������͵�UDP��Ϣ����

OS_EVENT * ZMSem;               //��ģ������Դ�ź���

int main(void)
{	
	BSP_Init();
	OSInit(); 					//UCOS��ʼ��	
	En_Dis_CodeQeue = OSQCreate(&En_Dis_CodeQeueTab[0],En_Dis_Code_QEUE_LENGTH);//������λ��������������������
	En_Udp_CodeQeue = OSQCreate(&En_Udp_CodeQeueTab[0],En_Udp_Code_QEUE_LENGTH);//���ڽ������������͵�UDP����
	
  ZMSem = OSSemCreate(1); 
	
	while(lwip_comm_init()); 	//lwip��ʼ��
	while(udp_demo_init()); 										//��ʼ��udp_demo(����udp_demo�߳�)
	OSTaskCreate(start_task,(void*)0,(OS_STK*)&START_TASK_STK[START_STK_SIZE-1],START_TASK_PRIO);
	OSStart(); //����UCOS
}

//start����
void start_task(void *pdata)
{
	OS_CPU_SR cpu_sr;
	pdata = pdata ;
	OSStatInit();  			//��ʼ��ͳ������
	OS_ENTER_CRITICAL();  	//���ж�

	OSTaskCreate(led_task,(void*)0,(OS_STK*)&LED_TASK_STK[LED_STK_SIZE-1],LED_TASK_PRIO); 	//����LED����
	OSTaskCreate(TestCode,(void*)0,(OS_STK*)&TestCode_TASK_STK[TestCode_STK_SIZE-1],TestCode_PRIO); 	//�����������ű�����
	OSTaskCreate(Write_RGB,(void*)0,(OS_STK*)&Write_RGB_TASK_STK[Write_RGB_STK_SIZE-1],Write_RGB_TASK_PRIO); 	//������ʾ����
	OSTaskCreate(key_task,(void*)0,(OS_STK*)&TestCode_KEY_STK[TestCode_KEY_SIZE-1],TestCode_KEY); 	//����key�������
#if shihangzuoyi
//	OSTaskCreate(TASK_OverDataHandle,(void*)0,(OS_STK*)&TASK_OverDataHandle_STK[TASK_OverDataHandle_SIZE-1],TASK_OVER_DATA_PRIO); 	//������������
#endif

	OSTaskSuspend(OS_PRIO_SELF); //����start_task����
	OS_EXIT_CRITICAL();  //���ж�
}
