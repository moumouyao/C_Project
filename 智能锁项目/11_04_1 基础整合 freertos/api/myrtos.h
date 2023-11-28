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


/**********************任务配置******************************/
/*开始任务配置*/
#define START_STK_SIZE 		512			  //任务堆栈大小	
#define START_TASK_PRIO		15				  //任务优先级


/*task1配置	led灯*/
#define LED_STACK_LEN 		512			  //任务堆栈大小	
#define LED_TASK_PRIO		12						  //任务优先级


///*task2配置	su_o3t*/
//#define TASK2_STK_SIZE 		128			  //任务堆栈大小	
//#define TASK2_PRIO		2						  //任务优先级


///*task3配置	数据上传阿里云*/
//#define TASK3_STK_SIZE 		512			  //任务堆栈大小	
//#define TASK3_PRIO		3					  //任务优先级

///*task4配置	lcd显示*/
//#define TASK4_STK_SIZE 		1024			  //任务堆栈大小	
//#define TASK4_PRIO		7						  //任务优先级

///*task5配置	实时时钟*/
//#define TASK5_STK_SIZE 		256			  //任务堆栈大小	
//#define TASK5_PRIO		5						  //任务优先级

///*task6配置	传感器采样*/
//#define TASK6_STK_SIZE 		128			  //任务堆栈大小	
//#define TASK6_PRIO		4						  //任务优先级

///*task7配置	lvgl配置*/
//#define TASK7_STK_SIZE 		1024			  //任务堆栈大小	
//#define TASK7_PRIO		9						  //任务优先级

///*task8配置	获取实时天气*/
//#define TASK8_STK_SIZE 		512			  //任务堆栈大小	
//#define TASK8_PRIO		6						  //任务优先级

///*task9配置 按键控制*/
//#define TASK9_STK_SIZE 		128			  //任务堆栈大小	
//#define TASK9_PRIO		8						  //任务优先级

///*task10配置 按键控制*/
//#define TASK10_STK_SIZE 		256			  //任务堆栈大小	
//#define TASK10_PRIO		8						  //任务优先级
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

