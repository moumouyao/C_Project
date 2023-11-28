#include "myrtos.h"

/**********************任务配置******************************/

TaskHandle_t StartTask_Handler;           		 //任务句柄
TaskHandle_t led_Handler;                    //任务句柄
TaskHandle_t finger_Handler;                    //任务句柄
TaskHandle_t time_Handler;                    //任务句柄
TaskHandle_t key_Handler;                    //任务句柄
TaskHandle_t adm_Handler;                    //任务句柄
TaskHandle_t rfid_Handler;                    //任务句柄
TaskHandle_t Task7_Handler;                    //任务句柄
TaskHandle_t Task8_Handler;                    //任务句柄
TaskHandle_t Task9_Handler;                    //任务句柄
TaskHandle_t Task10_Handler;                    //任务句柄
//QueueHandle_t BINARY_SEMAPHORE_handle;				//二值信号量句柄
/************************************************************/


/**
  * @brief  FreeRTOS 例程入口函数
  * @param  None
  * @retval None
  */
    void freertos_demo(void){
//			
//			BINARY_SEMAPHORE_handle = xSemaphoreCreateBinary();
//			if(BINARY_SEMAPHORE_handle == NULL){
//				printf("二值信号量创建失败\r\n");
//			}
//			else{
//				printf("二值信号量创建成功\r\n");
//			}
//			xSemaphoreGive(BINARY_SEMAPHORE_handle);
        //创建开始任务
    xTaskCreate((TaskFunction_t )start_task,            //任务函数
                (const char*    )"start_task",          //任务名称
                (uint16_t       )START_STK_SIZE,        //任务堆栈大小
                (void*          )NULL,                  //传递给任务函数的参数
                (UBaseType_t    )START_TASK_PRIO,       //任务优先级
                (TaskHandle_t*  )&StartTask_Handler);   //任务句柄              
    vTaskStartScheduler();          //开启任务调度
}
		
const u8 parket_heart[] = {0xc0,0x00};
		
void led_task(void *pvParameters){
		

	while(1){
//        printf("\r\n任务1正常\r\n");
		taskENTER_CRITICAL();           //进入临界区
    printf("led任务\r\n");
		usart2_send_string(parket_heart, 2);
		taskEXIT_CRITICAL();            //退出临界区
		vTaskDelay(1000 * 30);

    }
}

/**
  * @brief  开始任务任务函数，用于创建其他任务
  * @param  None
  * @retval None
  */
void start_task(void *pvParameters)
{
    taskENTER_CRITICAL();           //进入临界区

    //创建task1任务	led等闪烁
    xTaskCreate((TaskFunction_t )led_task,     
                (const char*    )"led_task",   
                (uint16_t       )LED_STK_SIZE, 
                (void*          )NULL,
                (UBaseType_t    )LED_TASK_PRIO,
                (TaskHandle_t*  )&led_Handler); 

    //创建task2任务	指纹开锁检测
    xTaskCreate((TaskFunction_t )FingerTask,     
                (const char*    )"FingerTask",   
                (uint16_t       )FINGER_STK_SIZE, 
                (void*          )NULL,
                (UBaseType_t    )FINGER_TASK_PRIO,
                (TaskHandle_t*  )&finger_Handler); 
		//创建task3任务	指纹开锁检测
    xTaskCreate((TaskFunction_t )TimeTask,     
                (const char*    )"TimeTask",   
                (uint16_t       )TIME_STK_SIZE, 
                (void*          )NULL,
                (UBaseType_t    )TIME_TASK_PRIO,
                (TaskHandle_t*  )&time_Handler); 
								
		//创建task3任务	按键解锁配置
    xTaskCreate((TaskFunction_t )KeyTask,     
                (const char*    )"KeyTask",   
                (uint16_t       )KEY_STK_SIZE, 
                (void*          )NULL,
                (UBaseType_t    )KEY_TASK_PRIO,
                (TaskHandle_t*  )&key_Handler); 
		//创建task3任务	管理员配置
    xTaskCreate((TaskFunction_t )AdmTask,     
                (const char*    )"AdmTask",   
                (uint16_t       )ADM_STK_SIZE, 
                (void*          )NULL,
                (UBaseType_t    )ADM_TASK_PRIO,
                (TaskHandle_t*  )&adm_Handler);
								
			//创建task3任务	刷卡任务
    xTaskCreate((TaskFunction_t )RfidTask,     
                (const char*    )"RfidTask",   
                (uint16_t       )RFID_STK_SIZE, 
                (void*          )NULL,
                (UBaseType_t    )RFID_TASK_PRIO,
                (TaskHandle_t*  )&rfid_Handler);
								
								
	vTaskSuspend(adm_Handler);
		vTaskDelete(StartTask_Handler); //删除开始任务
    taskEXIT_CRITICAL();            //退出临界区
}




