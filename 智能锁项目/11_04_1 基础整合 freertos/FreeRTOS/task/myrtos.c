#include "myrtos.h"

/**********************��������******************************/

TaskHandle_t StartTask_Handler;           		 //������
TaskHandle_t led_Handler;                    //������
TaskHandle_t finger_Handler;                    //������
TaskHandle_t time_Handler;                    //������
TaskHandle_t key_Handler;                    //������
TaskHandle_t adm_Handler;                    //������
TaskHandle_t rfid_Handler;                    //������
TaskHandle_t Task7_Handler;                    //������
TaskHandle_t Task8_Handler;                    //������
TaskHandle_t Task9_Handler;                    //������
TaskHandle_t Task10_Handler;                    //������
//QueueHandle_t BINARY_SEMAPHORE_handle;				//��ֵ�ź������
/************************************************************/


/**
  * @brief  FreeRTOS ������ں���
  * @param  None
  * @retval None
  */
    void freertos_demo(void){
//			
//			BINARY_SEMAPHORE_handle = xSemaphoreCreateBinary();
//			if(BINARY_SEMAPHORE_handle == NULL){
//				printf("��ֵ�ź�������ʧ��\r\n");
//			}
//			else{
//				printf("��ֵ�ź��������ɹ�\r\n");
//			}
//			xSemaphoreGive(BINARY_SEMAPHORE_handle);
        //������ʼ����
    xTaskCreate((TaskFunction_t )start_task,            //������
                (const char*    )"start_task",          //��������
                (uint16_t       )START_STK_SIZE,        //�����ջ��С
                (void*          )NULL,                  //���ݸ��������Ĳ���
                (UBaseType_t    )START_TASK_PRIO,       //�������ȼ�
                (TaskHandle_t*  )&StartTask_Handler);   //������              
    vTaskStartScheduler();          //�����������
}
		
const u8 parket_heart[] = {0xc0,0x00};
		
void led_task(void *pvParameters){
		

	while(1){
//        printf("\r\n����1����\r\n");
		taskENTER_CRITICAL();           //�����ٽ���
    printf("led����\r\n");
		usart2_send_string(parket_heart, 2);
		taskEXIT_CRITICAL();            //�˳��ٽ���
		vTaskDelay(1000 * 30);

    }
}

/**
  * @brief  ��ʼ���������������ڴ�����������
  * @param  None
  * @retval None
  */
void start_task(void *pvParameters)
{
    taskENTER_CRITICAL();           //�����ٽ���

    //����task1����	led����˸
    xTaskCreate((TaskFunction_t )led_task,     
                (const char*    )"led_task",   
                (uint16_t       )LED_STK_SIZE, 
                (void*          )NULL,
                (UBaseType_t    )LED_TASK_PRIO,
                (TaskHandle_t*  )&led_Handler); 

    //����task2����	ָ�ƿ������
    xTaskCreate((TaskFunction_t )FingerTask,     
                (const char*    )"FingerTask",   
                (uint16_t       )FINGER_STK_SIZE, 
                (void*          )NULL,
                (UBaseType_t    )FINGER_TASK_PRIO,
                (TaskHandle_t*  )&finger_Handler); 
		//����task3����	ָ�ƿ������
    xTaskCreate((TaskFunction_t )TimeTask,     
                (const char*    )"TimeTask",   
                (uint16_t       )TIME_STK_SIZE, 
                (void*          )NULL,
                (UBaseType_t    )TIME_TASK_PRIO,
                (TaskHandle_t*  )&time_Handler); 
								
		//����task3����	������������
    xTaskCreate((TaskFunction_t )KeyTask,     
                (const char*    )"KeyTask",   
                (uint16_t       )KEY_STK_SIZE, 
                (void*          )NULL,
                (UBaseType_t    )KEY_TASK_PRIO,
                (TaskHandle_t*  )&key_Handler); 
		//����task3����	����Ա����
    xTaskCreate((TaskFunction_t )AdmTask,     
                (const char*    )"AdmTask",   
                (uint16_t       )ADM_STK_SIZE, 
                (void*          )NULL,
                (UBaseType_t    )ADM_TASK_PRIO,
                (TaskHandle_t*  )&adm_Handler);
								
			//����task3����	ˢ������
    xTaskCreate((TaskFunction_t )RfidTask,     
                (const char*    )"RfidTask",   
                (uint16_t       )RFID_STK_SIZE, 
                (void*          )NULL,
                (UBaseType_t    )RFID_TASK_PRIO,
                (TaskHandle_t*  )&rfid_Handler);
								
								
	vTaskSuspend(adm_Handler);
		vTaskDelete(StartTask_Handler); //ɾ����ʼ����
    taskEXIT_CRITICAL();            //�˳��ٽ���
}




