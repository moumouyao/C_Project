#ifndef __MYRTOS_H_
#define __MYRTOS_H_

#include "stm32f10x.h"                  // Device header
#include "FreeRTOS.h"
#include "task.h"
#include "led.h"
#include "key.h"
#include "usart.h"
#include "su_03t.h"
#include "kqm.h"
#include "dht11.h"
#include "ADC.h"
#include "key.h"
#include "RTC.h"
#include "my_lcd.h"
#include "my_spi_flash.h"
#include "esp8266.h"
#include "lvgl.h"
#include "lv_port_disp.h"
#include "lvgl_app.h"
#include "gui_guider.h"
#include "my_spi_flash.h"
#include "semphr.h"
/**********************��������******************************/
/*��ʼ��������*/
#define START_STK_SIZE 		128			  //�����ջ��С	
#define START_TASK_PRIO		1				  //�������ȼ�


/*task1����	led��*/
#define TASK1_STK_SIZE 		128			  //�����ջ��С	
#define TASK1_PRIO		1						  //�������ȼ�


/*task2����	su_o3t*/
#define TASK2_STK_SIZE 		128			  //�����ջ��С	
#define TASK2_PRIO		2						  //�������ȼ�


/*task3����	�����ϴ�������*/
#define TASK3_STK_SIZE 		512			  //�����ջ��С	
#define TASK3_PRIO		3					  //�������ȼ�

/*task4����	lcd��ʾ*/
#define TASK4_STK_SIZE 		1024			  //�����ջ��С	
#define TASK4_PRIO		7						  //�������ȼ�

/*task5����	ʵʱʱ��*/
#define TASK5_STK_SIZE 		256			  //�����ջ��С	
#define TASK5_PRIO		5						  //�������ȼ�

/*task6����	����������*/
#define TASK6_STK_SIZE 		128			  //�����ջ��С	
#define TASK6_PRIO		4						  //�������ȼ�

/*task7����	lvgl����*/
#define TASK7_STK_SIZE 		1024			  //�����ջ��С	
#define TASK7_PRIO		9						  //�������ȼ�

/*task8����	��ȡʵʱ����*/
#define TASK8_STK_SIZE 		512			  //�����ջ��С	
#define TASK8_PRIO		6						  //�������ȼ�

/*task9���� ��������*/
#define TASK9_STK_SIZE 		128			  //�����ջ��С	
#define TASK9_PRIO		8						  //�������ȼ�

/*task10���� ��������*/
#define TASK10_STK_SIZE 		256			  //�����ջ��С	
#define TASK10_PRIO		8						  //�������ȼ�
/************************************************************/

void freertos_demo(void);

void start_task(void *pvParameters);

void task1(void *pvParameters);

void task2(void *pvParameters);

void task3(void *pvParameters);

void task4(void *pvParameters);

void task5(void *pvParameters);

void task6(void *pvParameters);

void task7(void *pvParameters);

void task8(void *pvParameters);

void task9(void *pvParameters);

void task10(void *pvParameters);


#endif
