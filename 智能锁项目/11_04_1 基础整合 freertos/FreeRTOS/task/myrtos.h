
#ifndef __MYRTOS_H_
#define __MYRTOS_H_

#include "stm32f4xx.h"                  // Device header
#include "main.h"

/**********************任务配置******************************/
/*开始任务配置*/
#define START_STK_SIZE 		128			  //任务堆栈大小	
#define START_TASK_PRIO		1				  //任务优先级

/*led配置*/
#define LED_STK_SIZE 		128			  //任务堆栈大小	
#define LED_TASK_PRIO		17				  //任务优先级

/*指纹解锁配置*/
#define FINGER_STK_SIZE 		128			  //任务堆栈大小	
#define FINGER_TASK_PRIO		4				  //任务优先级

/*时间显示配置*/
#define TIME_STK_SIZE 		256			  //任务堆栈大小	
#define TIME_TASK_PRIO		11				  //任务优先级

/*键盘解锁配置*/
#define KEY_STK_SIZE 		1024			  //任务堆栈大小	
#define KEY_TASK_PRIO		10				  //任务优先级

/*管理员配置*/
#define ADM_STK_SIZE 		2048			  //任务堆栈大小	
#define ADM_TASK_PRIO		16				  //任务优先级

/*刷卡配置*/
#define RFID_STK_SIZE 		256			  //任务堆栈大小	
#define RFID_TASK_PRIO		7				  //任务优先级



void start_task(void *pvParameters);
void FingerTask(void *pdata);
void time_task(void);
void TimeTask(void *pdata);
void finger_task(void);

void KeyTask(void *pdata);
void AdmTask(void *pdata);


void key2passwd(void);
void lcd_show(void);
void adm_task(void);
void change_adm_passwd(void);
void change_door_passwd(void);
void registration_card(void);
void del_specified_card(void);
void register_fingerprint(void);
void del_specified_fingerprint(void);
void del_all_fingers(void);
void set_volume(void);
void factory_reset(void);
void	lcd_task(void);
void RfidTask(void *pdata);
void w25_card_write(char *cardid);


#endif


