#include "ucos_task.h"

OS_STK START_TASK[START_STACK_LEN];
/**
  * @brief  FreeRTOS ������ں���
  * @param  None
  * @retval None
  */
void freertos_demo(void){
	uint8_t ret;
	
		//������ʼ����

	ret = OSTaskCreate(StartTask, NULL, &START_TASK[START_STACK_LEN - 1], START_TASK_PRIO);

	if(ret!=OS_ERR_NONE)
	{
		printf("Task create err!\r\n");
	}
		printf("��ʼ�������\r\n");	
	OSStart();//����UCOS-II����ϵͳ
}
		

	//��ʼ����
void StartTask(void *pdata){
	pdata=pdata;//��ֹ�������Ż�
	printf("���뿪ʼ����\r\n");
	
	OSTaskCreate(LedTask, NULL, &LED_TASK[LED_STACK_LEN - 1], LED_TASK_PRIO);
	OSTaskCreate(LcdTask, NULL, &LCD_TASK[LCD_STACK_LEN - 1], LCD_TASK_PRIO);
	OSTaskCreate(KeyTask, NULL, &KEY_TASK[KEY_STACK_LEN - 1], KEY_TASK_PRIO);
	OSTaskCreate(AdmTask, NULL, &ADM_TASK[ADM_STACK_LEN - 1], ADM_TASK_PRIO);
	OSTaskCreate(FingerTask, NULL, &FINGER_TASK[FINGER_STACK_LEN - 1], FINGER_TASK_PRIO);
	OSTaskSuspend(ADM_TASK_PRIO);
	OSTaskDel(START_TASK_PRIO);
}
