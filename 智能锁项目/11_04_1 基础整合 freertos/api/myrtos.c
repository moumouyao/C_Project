#include "myrtos.h"

/**********************任务配置******************************/
OS_STK START_TASK[START_STK_SIZE];
OS_STK LED_TASK[LED_STACK_LEN];

/************************************************************/

/**
  * @brief  FreeRTOS 例程入口函数
  * @param  None
  * @retval None
  */
    void freertos_demo(void){
			uint8_t ret;
			
        //创建开始任务
		
    ret = OSTaskCreate(start_task, NULL, &START_TASK[START_STK_SIZE - 1], START_TASK_PRIO);
		
if(ret!=OS_ERR_NONE)
	{
		printf("Task create err!\r\n");
	}
	printf("开始任务调度\r\n");	
	OSStart();//启动UCOS-II操作系统
}
	//开始任务
void start_task(void *pdata){
	pdata=pdata;//防止编译器优化
	printf("进入开始任务\r\n");
	 OSTaskCreate(led_task, NULL, &LED_TASK[LED_STACK_LEN - 1], LED_TASK_PRIO);
	OSTaskDel(START_TASK_PRIO);
}


	//开始任务
void led_task(void *pdata){
	while(1){
		printf("进入led任务\r\n");
		OSTimeDly(200);
	}
	
}

//OS_STK LED_TASK[LED_STACK_LEN];


//void LedTask(void *pdata)
//{
//	pdata=pdata;//防止编译器优化
//	
//	
//	
//	printf("led task coming!!\r\n");
//	while(1)
//	{
//		printf("led 1111111111\r\n");
//		OSTimeDly(1000);
//		printf("led 2222222222\r\n");
//		OSTimeDly(1000);
//	}
//	
//}

//OS_STK START_TASK[START_STK_SIZE];


//void StartTask(void *pdata)
//{
//	INT8U ret;
//	
//	pdata=pdata;//防止编译器优化
//	
//	//成功创建任务后会立即发生一次系统调度
//	ret= OSTaskCreate(LedTask,NULL,&LED_TASK[LED_STACK_LEN-1],LED_TASK_PRIO);
//	if(ret!=OS_ERR_NONE)
//	{
//		printf("Task create err!\r\n");
//	}
//	
//	printf("start task coming!!\r\n");
//	
//	
//}



//    void freertos_demo(void){
//			uint8_t ret;
//			
//        //创建开始任务
//		
//    ret = OSTaskCreate(StartTask, NULL, &START_TASK[START_STK_SIZE - 1], START_TASK_PRIO);
//		
//if(ret!=OS_ERR_NONE)
//	{
//		printf("Task create err!\r\n");
//	}
//	printf("开始任务调度\r\n");	
//	OSStart();//启动UCOS-II操作系统
//}
//	




