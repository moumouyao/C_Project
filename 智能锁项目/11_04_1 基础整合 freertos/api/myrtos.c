#include "myrtos.h"

/**********************��������******************************/
OS_STK START_TASK[START_STK_SIZE];
OS_STK LED_TASK[LED_STACK_LEN];

/************************************************************/

/**
  * @brief  FreeRTOS ������ں���
  * @param  None
  * @retval None
  */
    void freertos_demo(void){
			uint8_t ret;
			
        //������ʼ����
		
    ret = OSTaskCreate(start_task, NULL, &START_TASK[START_STK_SIZE - 1], START_TASK_PRIO);
		
if(ret!=OS_ERR_NONE)
	{
		printf("Task create err!\r\n");
	}
	printf("��ʼ�������\r\n");	
	OSStart();//����UCOS-II����ϵͳ
}
	//��ʼ����
void start_task(void *pdata){
	pdata=pdata;//��ֹ�������Ż�
	printf("���뿪ʼ����\r\n");
	 OSTaskCreate(led_task, NULL, &LED_TASK[LED_STACK_LEN - 1], LED_TASK_PRIO);
	OSTaskDel(START_TASK_PRIO);
}


	//��ʼ����
void led_task(void *pdata){
	while(1){
		printf("����led����\r\n");
		OSTimeDly(200);
	}
	
}

//OS_STK LED_TASK[LED_STACK_LEN];


//void LedTask(void *pdata)
//{
//	pdata=pdata;//��ֹ�������Ż�
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
//	pdata=pdata;//��ֹ�������Ż�
//	
//	//�ɹ�������������������һ��ϵͳ����
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
//        //������ʼ����
//		
//    ret = OSTaskCreate(StartTask, NULL, &START_TASK[START_STK_SIZE - 1], START_TASK_PRIO);
//		
//if(ret!=OS_ERR_NONE)
//	{
//		printf("Task create err!\r\n");
//	}
//	printf("��ʼ�������\r\n");	
//	OSStart();//����UCOS-II����ϵͳ
//}
//	




