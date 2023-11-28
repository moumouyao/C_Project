/*��ʼ���񴴽�3����������1��ʵ��LED1ÿ500ms��תһ��  ����2��ʵ��LED2ÿ500ms��תһ��  ����3���ж�key1,����ɾ��task1*/
#include "myrtos.h"

/**********************��������******************************/

TaskHandle_t StartTask_Handler;           		 //������
TaskHandle_t Task1_Handler;                    //������
TaskHandle_t Task2_Handler;                    //������
TaskHandle_t Task3_Handler;                    //������
TaskHandle_t Task4_Handler;                    //������
TaskHandle_t Task5_Handler;                    //������
TaskHandle_t Task6_Handler;                    //������
TaskHandle_t Task7_Handler;                    //������
TaskHandle_t Task8_Handler;                    //������
TaskHandle_t Task9_Handler;                    //������
TaskHandle_t Task10_Handler;                    //������
QueueHandle_t BINARY_SEMAPHORE_handle;				//��ֵ�ź������
/************************************************************/

uint8_t key_val;
extern lv_ui_2 test2;
extern uint8_t ap_val[2][32];

/**
  * @brief  FreeRTOS ������ں���
  * @param  None
  * @retval None
  */
    void freertos_demo(void){
			
			BINARY_SEMAPHORE_handle = xSemaphoreCreateBinary();
			if(BINARY_SEMAPHORE_handle == NULL){
				printf("��ֵ�ź�������ʧ��\r\n");
			}
			else{
				printf("��ֵ�ź��������ɹ�\r\n");
			}
			xSemaphoreGive(BINARY_SEMAPHORE_handle);
        //������ʼ����
    xTaskCreate((TaskFunction_t )start_task,            //������
                (const char*    )"start_task",          //��������
                (uint16_t       )START_STK_SIZE,        //�����ջ��С
                (void*          )NULL,                  //���ݸ��������Ĳ���
                (UBaseType_t    )START_TASK_PRIO,       //�������ȼ�
                (TaskHandle_t*  )&StartTask_Handler);   //������              
    vTaskStartScheduler();          //�����������
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
    xTaskCreate((TaskFunction_t )task1,     
                (const char*    )"task1",   
                (uint16_t       )TASK1_STK_SIZE, 
                (void*          )NULL,
                (UBaseType_t    )TASK1_PRIO,
                (TaskHandle_t*  )&Task1_Handler); 
////        
    //����task2����	su_03t����
    xTaskCreate((TaskFunction_t )task2,     
                (const char*    )"task2",   
                (uint16_t       )TASK2_STK_SIZE, 
                (void*          )NULL,
                (UBaseType_t    )TASK2_PRIO,
                (TaskHandle_t*  )&Task2_Handler);
//                                
    //����task3����	�����ϴ�������
    xTaskCreate((TaskFunction_t )task3,     
                (const char*    )"task3",   
                (uint16_t       )TASK3_STK_SIZE, 
                (void*          )NULL,
                (UBaseType_t    )TASK3_PRIO,
                (TaskHandle_t*  )&Task3_Handler);    

    //����task4����	lvgl lcd��ʾ
    xTaskCreate((TaskFunction_t )task4,     
                (const char*    )"task4",   
                (uint16_t       )TASK4_STK_SIZE, 
                (void*          )NULL,
                (UBaseType_t    )TASK4_PRIO,
                (TaskHandle_t*  )&Task4_Handler); 
//							
//����task5����	У׼ʵʱʱ��
		xTaskCreate((TaskFunction_t )task5,     
								(const char*    )"task5",   
								(uint16_t       )TASK5_STK_SIZE, 
								(void*          )NULL,
								(UBaseType_t    )TASK5_PRIO,
								(TaskHandle_t*  )&Task5_Handler);
//								
//����task6����	����������
		xTaskCreate((TaskFunction_t )task6,     
								(const char*    )"task6",   
								(uint16_t       )TASK6_STK_SIZE, 
								(void*          )NULL,
								(UBaseType_t    )TASK6_PRIO,
								(TaskHandle_t*  )&Task6_Handler);

//����task7����	lvgl��������
		xTaskCreate((TaskFunction_t )task7,     
								(const char*    )"task7",   
								(uint16_t       )TASK7_STK_SIZE, 
								(void*          )NULL,
								(UBaseType_t    )TASK7_PRIO,
								(TaskHandle_t*  )&Task7_Handler);
////								
//����task8����	��ȡʵʱ����
		xTaskCreate((TaskFunction_t )task8,     
								(const char*    )"task8",   
								(uint16_t       )TASK8_STK_SIZE, 
								(void*          )NULL,
								(UBaseType_t    )TASK8_PRIO,
								(TaskHandle_t*  )&Task8_Handler);
////								
//����task9����	��������
		xTaskCreate((TaskFunction_t )task9,     
								(const char*    )"task9",   
								(uint16_t       )TASK9_STK_SIZE, 
								(void*          )NULL,
								(UBaseType_t    )TASK9_PRIO,
								(TaskHandle_t*  )&Task9_Handler);
//								
//����task10����
		xTaskCreate((TaskFunction_t )task10,     
								(const char*    )"task10",   
								(uint16_t       )TASK10_STK_SIZE, 
								(void*          )NULL,
								(UBaseType_t    )TASK10_PRIO,
								(TaskHandle_t*  )&Task10_Handler);
								
								vTaskSuspend(Task10_Handler);
										
    vTaskDelete(StartTask_Handler); //ɾ����ʼ����
    taskEXIT_CRITICAL();            //�˳��ٽ���
}

/**
  * @brief  ����1��ʵ��LED1ÿ500ms��תһ��
  * @param  None
  * @retval None
  */
void task1(void *pvParameters){
	
    while(1){
//        printf("\r\n����1����\r\n");
        led_turn(LED1_PORT, LED1_PIN);
			
        vTaskDelay(500);
    }
}


/**
  * @brief  ����2��su_03t����
  * @param  None
  * @retval None
  */
void task2(void *pvParameters){
   
	while(1){
//		printf("\r\n����2����\r\n");
        vioce_analysis();
		
        vTaskDelay(50);
    }
}

/**
  * @brief  �����ϴ��Ʒ�����
  * @param  None
  * @retval None
  */
void task3(void *pvParameters){
	
    while(1){
			printf("\r\n����3����\r\n");
			xSemaphoreTake(BINARY_SEMAPHORE_handle, 1000);
//			wifi_init();
//			taskENTER_CRITICAL();           //�����ٽ���//
			printf("�����ϴ���ʼִ��\r\n");
			send_data2server();
			xSemaphoreGive(BINARY_SEMAPHORE_handle);
//			taskEXIT_CRITICAL();            //�˳��ٽ��� 
				vTaskDelay(1000 * 10);
        }
       
    
}

/**
  * @brief  ����4��lcd��ʾ
  * @param  None
  * @retval None
  */
extern uint8_t lvgl_flag;
extern lv_obj_t *scr1, *scr2;
extern lv_obj_t *label_time;
extern lv_ui test;
extern uint8_t weather_val[4][32];
void task4(void *pvParameters){
	
	unsigned char time_str[64] = {0};
	char data_str[32] = {0};
		lv_scr_load(test.screen);
    while(1){
//			printf("\r\n����4����\r\n");
      
        if(key_val == 1){
//					printf("����1���£���ʾ��������\r\n");
					lv_label_set_text_fmt(test.screen_label_1, "%d.%d", dht_data.tmp, dht_data.tmp_flo);	//�¶�
          lv_label_set_text_fmt(test.screen_label_2, "%d%", dht_data.hum);	//ʪ��
					sprintf(data_str, "%.2f", (float)kqm_data.CHO);
					lv_label_set_text_fmt(test.screen_label_3, "%s", data_str);	//����
//					printf(data_str, "%.2f", (float)kqm_data.CO2);
//					sprintf(data_str, "%.2f", (float)kqm_data.CHO);
					lv_label_set_text_fmt(test.screen_label_4, "%d", kqm_data.CO2);	//������̼
					get_time_str(time_str);
					lv_label_set_text_fmt(test.screen_label_5, "%s", time_str);	//������̼
					lv_tabview_set_tab_act(test.screen_tabview_1,0, false);
            }
				if(key_val == 2){
//					printf("����2���£���ʾ����\r\n");
					switch(atoi((char *)(weather_val[2]))){
						case 0:lv_img_set_src(test.screen_img_7,&_0x_alpha_50x50);	break;
						case 1:lv_img_set_src(test.screen_img_7,&_1x_alpha_50x50);	break;
						case 4:lv_img_set_src(test.screen_img_7,&_4x_alpha_50x50);	break;
						case 5:lv_img_set_src(test.screen_img_7,&_5x_alpha_50x50);	break;
						case 6:
						case 7:
						case 8:
						case 9:lv_img_set_src(test.screen_img_7,&_9x_alpha_50x50);	break;
						case 10:
						case 11:
						case 12:
						case 13:lv_img_set_src(test.screen_img_7,&_13x_alpha_50x50);	break;
						case 14:
						case 15:
						case 16:
						case 17:
						case 18:
						case 19:lv_img_set_src(test.screen_img_7,&_17x_alpha_50x50);	break;
						default:break;
					}
//					lv_label_set_text(test.screen_label_6, "����Ԥ��");
					lv_label_set_text_fmt(test.screen_label_8, "%s", weather_val[0]);	//�����¶�
					lv_label_set_long_mode(test.screen_label_9, LV_LABEL_LONG_SROLL_CIRC);
					lv_label_set_long_mode(test.screen_label_7, LV_LABEL_LONG_SROLL_CIRC);
					lv_label_set_text_fmt(test.screen_label_9, "%s", weather_val[1]);	//�ص�
					lv_label_set_text_fmt(test.screen_label_7, "%s", weather_val[3]);	//�ص�
					lv_tabview_set_tab_act(test.screen_tabview_1,1, false);
				}
				else if(key_val == 3){
//					printf("����3���£���ʾ����\r\n");
					lv_tabview_set_tab_act(test.screen_tabview_1,2, false);
				}
			
			
        vTaskDelay(400);
    }
}

/**
  * @brief  ����5��У׼ʵʱʱ�ӣ���Լÿһ��СʱУ׼һ��
  * @param  None
  * @retval None
  */
void task5(void *pvParameters){
	
    while(1){
			xSemaphoreTake(BINARY_SEMAPHORE_handle, 1000);
			printf("\r\n����5����\r\n");
//			taskENTER_CRITICAL();           //�����ٽ���
			printf("\r\nУ׼ʵʱʱ��\r\n");
//			wifi_init();
			Time_Adjust();
//			taskEXIT_CRITICAL();            //�˳��ٽ���
			xSemaphoreGive(BINARY_SEMAPHORE_handle);
		vTaskDelay(1000 * 60);
		
    }
}


/**
  * @brief  ����6�����ִ���������
  * @param  None
  * @retval None
  */
void task6(void *pvParameters){
	
    while(1){
//			printf("\r\n����6����\r\n");
			//taskENTER_CRITICAL();           //�����ٽ���
//			printf("��������6\r\n");
			air_analysis();
			
			dht_get_val();
			
			ADC2VOL();
		//	taskEXIT_CRITICAL();            //�˳��ٽ���
		vTaskDelay(5000);
		
    }
}

/**
  * @brief  ����7��lvgl
  * @param  None
  * @retval None
  */
void task7(void *pvParameters){
	printf("\r\n����7����\r\n");
    
			TickType_t xLastWakeTime;
	const TickType_t xPeriod = pdMS_TO_TICKS( 5 );
	xLastWakeTime = xTaskGetTickCount();  
while(1){
		
//		printf("\r\n2\r\n");
		lv_task_handler();
		vTaskDelayUntil( &xLastWakeTime, xPeriod );//������ʱ
	}

		
    
}


/**
  * @brief  ����8����ȡʵʱ����
  * @param  None
  * @retval None
  */
void task8(void *pvParameters){
	
    while(1){
			printf("\r\n����8����\r\n");
			xSemaphoreTake(BINARY_SEMAPHORE_handle, 1000);
//			taskENTER_CRITICAL();           //�����ٽ���//
//			printf("����\r\n");
			connect_seniverse();
//			printf("\r\n��ӡ����\r\n");
			//whther_analisys();
			wifi_init();
//			taskEXIT_CRITICAL();            //�˳��ٽ��� 
			
			xSemaphoreGive(BINARY_SEMAPHORE_handle);
		
			vTaskDelay(1000 * 60 * 20);
		
    }
}


/**
  * @brief  ��������
  * @param  None
  * @retval None
  */
//extern lv_obj_t *btn1;
//extern lv_obj_t *btn2;
extern 	uint32_t write_wifi_addr;
extern 	uint32_t write_len_addr;
void task9(void *pvParameters){
//	printf("9\r\n");
	uint8_t key;
	uint8_t flag = 0, len_wifi = 0;
	unsigned char wifi_name[32] = {0};
    while(1){
//			printf("\r\n����9����\r\n");
			key = key_get_num();
			if(flag == 0){
				key =4;
			}
        if(key == 1){
					printf("����1���£���ʾ��������\r\n");
          key_val = 1;
            }
				if(key == 2){
					printf("����2���£���ʾʵʱʱ��\r\n");
					key_val = 2;
				}
				else if(key == 3){
					printf("����3����\r\n");
					key_val = 3;
				}
				else if(key == 4){
					flag = 2;
					taskENTER_CRITICAL();           //�����ٽ���//
					printf("����4������\r\n");
					vTaskSuspend(Task3_Handler);
					vTaskSuspend(Task4_Handler);
					vTaskSuspend(Task5_Handler);
					vTaskSuspend(Task8_Handler);
					memset(wifi_name, 0, 32);
					len_wifi = 0;
					sFLASH_ReadBuffer(&len_wifi, write_len_addr, 1);
					sFLASH_ReadBuffer(wifi_name, write_wifi_addr, len_wifi);
					lv_label_set_text(test2.screen_2_label_2_3, (const char *)wifi_name);
					lv_scr_load(test2.screen_2);//������Ļ
					vTaskResume(Task10_Handler);
					printf("����������\r\n");
					taskEXIT_CRITICAL();            //�˳��ٽ��� 
					vTaskSuspend(Task9_Handler);
					
				}
           
				vTaskDelay(50);
        }
       
    
}


/**
  * @brief  ����10������wifi����
  * @param  None
  * @retval None
  */
void task10(void *pvParameters){
	uint8_t key_val = 0;
	uint8_t flag = 0;
	printf("����10��������\r\n");
    while(1){
//        printf("\r\n����10����\r\n");
      key_val = key_get_num();  
			if(key_val == 1){
				printf("����1������\r\n");
				flag = 2;
//				lv_obj_set_style_local_bg_color(test2.screen_2_btn2_1, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, lv_color_make(0x28, 0xa6, 0xaf));
				lv_scr_load(test2.screen_3);
				
				setup_animation();
				
				ap_init();
				
				
	
			}
		else if(key_val == 2){
			taskENTER_CRITICAL();           //�����ٽ���//
			printf("����2������\r\n");
			flag =0;
				lv_obj_set_style_local_bg_color(test2.screen_2_btn_2_2, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, lv_color_make(0x28, 0xa6, 0xaf));
				vTaskResume(Task3_Handler);
			  vTaskResume(Task4_Handler);
			  vTaskResume(Task5_Handler);
			  vTaskResume(Task8_Handler);
				vTaskResume(Task9_Handler);
				lv_scr_load(test.screen);
//				wifi_init();
				taskEXIT_CRITICAL();            //�˳��ٽ��� 
				vTaskSuspend(Task10_Handler);
			
				
    }
		
		if(flag == 2){
//			printf("2");
			//printf("�������� %s", esp8266.RX_data);
			if(strstr((char *)esp8266.RX_data, "wifiname") != NULL)
				AP_analisys();
				}
		
		vTaskDelay(10);
	}
}


