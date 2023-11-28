#ifndef __MYRTOS_H_
#define __MYRTOS_H_

#include "stm32f4xx.h"
#include "delay.h"
#include "key.h"
#include "usart.h"
#include <stdio.h>
#include "AT24.h"
#include "bs8116.h"
#include "my_spi_flash.h"
#include "my_lcd.h"
#include "mg200.h"
#include "voice.h"
#include "my_rtc.h"
#include "esp32.h"


/**********************��������******************************/
/*��ʼ��������*/
#define START_STK_SIZE 		512			  //�����ջ��С	
#define START_TASK_PRIO		15				  //�������ȼ�


/*task1����	led��*/
#define LED_STACK_LEN 		512			  //�����ջ��С	
#define LED_TASK_PRIO		12						  //�������ȼ�


///*task2����	su_o3t*/
//#define TASK2_STK_SIZE 		128			  //�����ջ��С	
//#define TASK2_PRIO		2						  //�������ȼ�


///*task3����	�����ϴ�������*/
//#define TASK3_STK_SIZE 		512			  //�����ջ��С	
//#define TASK3_PRIO		3					  //�������ȼ�

///*task4����	lcd��ʾ*/
//#define TASK4_STK_SIZE 		1024			  //�����ջ��С	
//#define TASK4_PRIO		7						  //�������ȼ�

///*task5����	ʵʱʱ��*/
//#define TASK5_STK_SIZE 		256			  //�����ջ��С	
//#define TASK5_PRIO		5						  //�������ȼ�

///*task6����	����������*/
//#define TASK6_STK_SIZE 		128			  //�����ջ��С	
//#define TASK6_PRIO		4						  //�������ȼ�

///*task7����	lvgl����*/
//#define TASK7_STK_SIZE 		1024			  //�����ջ��С	
//#define TASK7_PRIO		9						  //�������ȼ�

///*task8����	��ȡʵʱ����*/
//#define TASK8_STK_SIZE 		512			  //�����ջ��С	
//#define TASK8_PRIO		6						  //�������ȼ�

///*task9���� ��������*/
//#define TASK9_STK_SIZE 		128			  //�����ջ��С	
//#define TASK9_PRIO		8						  //�������ȼ�

///*task10���� ��������*/
//#define TASK10_STK_SIZE 		256			  //�����ջ��С	
//#define TASK10_PRIO		8						  //�������ȼ�
/************************************************************/

void freertos_demo(void);

void start_task(void *pdata);

void led_task(void *pdata);

//void task2(void *pvParameters);

//void task3(void *pvParameters);

//void task4(void *pvParameters);

//void task5(void *pvParameters);

//void task6(void *pvParameters);

//void task7(void *pvParameters);

//void task8(void *pvParameters);

//void task9(void *pvParameters);

//void task10(void *pvParameters);


#endif

