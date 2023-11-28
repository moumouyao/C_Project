
#ifndef __MYRTOS_H_
#define __MYRTOS_H_

#include "stm32f4xx.h"                  // Device header
#include "main.h"

/**********************��������******************************/
/*��ʼ��������*/
#define START_STK_SIZE 		128			  //�����ջ��С	
#define START_TASK_PRIO		1				  //�������ȼ�

/*led����*/
#define LED_STK_SIZE 		128			  //�����ջ��С	
#define LED_TASK_PRIO		17				  //�������ȼ�

/*ָ�ƽ�������*/
#define FINGER_STK_SIZE 		128			  //�����ջ��С	
#define FINGER_TASK_PRIO		4				  //�������ȼ�

/*ʱ����ʾ����*/
#define TIME_STK_SIZE 		256			  //�����ջ��С	
#define TIME_TASK_PRIO		11				  //�������ȼ�

/*���̽�������*/
#define KEY_STK_SIZE 		1024			  //�����ջ��С	
#define KEY_TASK_PRIO		10				  //�������ȼ�

/*����Ա����*/
#define ADM_STK_SIZE 		2048			  //�����ջ��С	
#define ADM_TASK_PRIO		16				  //�������ȼ�

/*ˢ������*/
#define RFID_STK_SIZE 		256			  //�����ջ��С	
#define RFID_TASK_PRIO		7				  //�������ȼ�



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


