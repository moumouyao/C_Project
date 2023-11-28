#include "ucos_task.h"

OS_STK START_TASK[START_STACK_LEN];
/**
  * @brief  FreeRTOS 例程入口函数
  * @param  None
  * @retval None
  */
void freertos_demo(void){
	uint8_t ret;
	
		//创建开始任务

	ret = OSTaskCreate(StartTask, NULL, &START_TASK[START_STACK_LEN - 1], START_TASK_PRIO);

	if(ret!=OS_ERR_NONE)
	{
		printf("Task create err!\r\n");
	}
		printf("开始任务调度\r\n");	
	OSStart();//启动UCOS-II操作系统
}
		

	//开始任务
void StartTask(void *pdata){
	pdata=pdata;//防止编译器优化
	printf("进入开始任务\r\n");
	
	OSTaskCreate(LedTask, NULL, &LED_TASK[LED_STACK_LEN - 1], LED_TASK_PRIO);
	OSTaskCreate(LcdTask, NULL, &LCD_TASK[LCD_STACK_LEN - 1], LCD_TASK_PRIO);
	OSTaskCreate(KeyTask, NULL, &KEY_TASK[KEY_STACK_LEN - 1], KEY_TASK_PRIO);
	OSTaskCreate(AdmTask, NULL, &ADM_TASK[ADM_STACK_LEN - 1], ADM_TASK_PRIO);
	OSTaskCreate(FingerTask, NULL, &FINGER_TASK[FINGER_STACK_LEN - 1], FINGER_TASK_PRIO);
	OSTaskSuspend(ADM_TASK_PRIO);
	OSTaskDel(START_TASK_PRIO);
}
