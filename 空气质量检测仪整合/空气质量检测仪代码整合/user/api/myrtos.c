/*开始任务创建3个任务，任务1，实现LED1每500ms翻转一次  任务2，实现LED2每500ms翻转一次  任务3，判断key1,按下删除task1*/
#include "myrtos.h"

/**********************任务配置******************************/

TaskHandle_t StartTask_Handler;           		 //任务句柄
TaskHandle_t Task1_Handler;                    //任务句柄
TaskHandle_t Task2_Handler;                    //任务句柄
TaskHandle_t Task3_Handler;                    //任务句柄
TaskHandle_t Task4_Handler;                    //任务句柄
TaskHandle_t Task5_Handler;                    //任务句柄
TaskHandle_t Task6_Handler;                    //任务句柄
TaskHandle_t Task7_Handler;                    //任务句柄
TaskHandle_t Task8_Handler;                    //任务句柄
TaskHandle_t Task9_Handler;                    //任务句柄
TaskHandle_t Task10_Handler;                    //任务句柄
QueueHandle_t BINARY_SEMAPHORE_handle;				//二值信号量句柄
/************************************************************/

uint8_t key_val;
extern lv_ui_2 test2;
extern uint8_t ap_val[2][32];

/**
  * @brief  FreeRTOS 例程入口函数
  * @param  None
  * @retval None
  */
    void freertos_demo(void){
			
			BINARY_SEMAPHORE_handle = xSemaphoreCreateBinary();
			if(BINARY_SEMAPHORE_handle == NULL){
				printf("二值信号量创建失败\r\n");
			}
			else{
				printf("二值信号量创建成功\r\n");
			}
			xSemaphoreGive(BINARY_SEMAPHORE_handle);
        //创建开始任务
    xTaskCreate((TaskFunction_t )start_task,            //任务函数
                (const char*    )"start_task",          //任务名称
                (uint16_t       )START_STK_SIZE,        //任务堆栈大小
                (void*          )NULL,                  //传递给任务函数的参数
                (UBaseType_t    )START_TASK_PRIO,       //任务优先级
                (TaskHandle_t*  )&StartTask_Handler);   //任务句柄              
    vTaskStartScheduler();          //开启任务调度
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
    xTaskCreate((TaskFunction_t )task1,     
                (const char*    )"task1",   
                (uint16_t       )TASK1_STK_SIZE, 
                (void*          )NULL,
                (UBaseType_t    )TASK1_PRIO,
                (TaskHandle_t*  )&Task1_Handler); 
////        
    //创建task2任务	su_03t控制
    xTaskCreate((TaskFunction_t )task2,     
                (const char*    )"task2",   
                (uint16_t       )TASK2_STK_SIZE, 
                (void*          )NULL,
                (UBaseType_t    )TASK2_PRIO,
                (TaskHandle_t*  )&Task2_Handler);
//                                
    //创建task3任务	数据上传阿里云
    xTaskCreate((TaskFunction_t )task3,     
                (const char*    )"task3",   
                (uint16_t       )TASK3_STK_SIZE, 
                (void*          )NULL,
                (UBaseType_t    )TASK3_PRIO,
                (TaskHandle_t*  )&Task3_Handler);    

    //创建task4任务	lvgl lcd显示
    xTaskCreate((TaskFunction_t )task4,     
                (const char*    )"task4",   
                (uint16_t       )TASK4_STK_SIZE, 
                (void*          )NULL,
                (UBaseType_t    )TASK4_PRIO,
                (TaskHandle_t*  )&Task4_Handler); 
//							
//创建task5任务	校准实时时钟
		xTaskCreate((TaskFunction_t )task5,     
								(const char*    )"task5",   
								(uint16_t       )TASK5_STK_SIZE, 
								(void*          )NULL,
								(UBaseType_t    )TASK5_PRIO,
								(TaskHandle_t*  )&Task5_Handler);
//								
//创建task6任务	传感器采样
		xTaskCreate((TaskFunction_t )task6,     
								(const char*    )"task6",   
								(uint16_t       )TASK6_STK_SIZE, 
								(void*          )NULL,
								(UBaseType_t    )TASK6_PRIO,
								(TaskHandle_t*  )&Task6_Handler);

//创建task7任务	lvgl任务处理函数
		xTaskCreate((TaskFunction_t )task7,     
								(const char*    )"task7",   
								(uint16_t       )TASK7_STK_SIZE, 
								(void*          )NULL,
								(UBaseType_t    )TASK7_PRIO,
								(TaskHandle_t*  )&Task7_Handler);
////								
//创建task8任务	获取实时天气
		xTaskCreate((TaskFunction_t )task8,     
								(const char*    )"task8",   
								(uint16_t       )TASK8_STK_SIZE, 
								(void*          )NULL,
								(UBaseType_t    )TASK8_PRIO,
								(TaskHandle_t*  )&Task8_Handler);
////								
//创建task9任务	按键切屏
		xTaskCreate((TaskFunction_t )task9,     
								(const char*    )"task9",   
								(uint16_t       )TASK9_STK_SIZE, 
								(void*          )NULL,
								(UBaseType_t    )TASK9_PRIO,
								(TaskHandle_t*  )&Task9_Handler);
//								
//创建task10任务
		xTaskCreate((TaskFunction_t )task10,     
								(const char*    )"task10",   
								(uint16_t       )TASK10_STK_SIZE, 
								(void*          )NULL,
								(UBaseType_t    )TASK10_PRIO,
								(TaskHandle_t*  )&Task10_Handler);
								
								vTaskSuspend(Task10_Handler);
										
    vTaskDelete(StartTask_Handler); //删除开始任务
    taskEXIT_CRITICAL();            //退出临界区
}

/**
  * @brief  任务1，实现LED1每500ms翻转一次
  * @param  None
  * @retval None
  */
void task1(void *pvParameters){
	
    while(1){
//        printf("\r\n任务1正常\r\n");
        led_turn(LED1_PORT, LED1_PIN);
			
        vTaskDelay(500);
    }
}


/**
  * @brief  任务2，su_03t控制
  * @param  None
  * @retval None
  */
void task2(void *pvParameters){
   
	while(1){
//		printf("\r\n任务2正常\r\n");
        vioce_analysis();
		
        vTaskDelay(50);
    }
}

/**
  * @brief  数据上传云服务器
  * @param  None
  * @retval None
  */
void task3(void *pvParameters){
	
    while(1){
			printf("\r\n任务3正常\r\n");
			xSemaphoreTake(BINARY_SEMAPHORE_handle, 1000);
//			wifi_init();
//			taskENTER_CRITICAL();           //进入临界区//
			printf("数据上传开始执行\r\n");
			send_data2server();
			xSemaphoreGive(BINARY_SEMAPHORE_handle);
//			taskEXIT_CRITICAL();            //退出临界区 
				vTaskDelay(1000 * 10);
        }
       
    
}

/**
  * @brief  任务4，lcd显示
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
//			printf("\r\n任务4正常\r\n");
      
        if(key_val == 1){
//					printf("按键1按下，显示空气数据\r\n");
					lv_label_set_text_fmt(test.screen_label_1, "%d.%d", dht_data.tmp, dht_data.tmp_flo);	//温度
          lv_label_set_text_fmt(test.screen_label_2, "%d%", dht_data.hum);	//湿度
					sprintf(data_str, "%.2f", (float)kqm_data.CHO);
					lv_label_set_text_fmt(test.screen_label_3, "%s", data_str);	//甲烷
//					printf(data_str, "%.2f", (float)kqm_data.CO2);
//					sprintf(data_str, "%.2f", (float)kqm_data.CHO);
					lv_label_set_text_fmt(test.screen_label_4, "%d", kqm_data.CO2);	//二氧化碳
					get_time_str(time_str);
					lv_label_set_text_fmt(test.screen_label_5, "%s", time_str);	//二氧化碳
					lv_tabview_set_tab_act(test.screen_tabview_1,0, false);
            }
				if(key_val == 2){
//					printf("按键2按下，显示天气\r\n");
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
//					lv_label_set_text(test.screen_label_6, "天气预报");
					lv_label_set_text_fmt(test.screen_label_8, "%s", weather_val[0]);	//室外温度
					lv_label_set_long_mode(test.screen_label_9, LV_LABEL_LONG_SROLL_CIRC);
					lv_label_set_long_mode(test.screen_label_7, LV_LABEL_LONG_SROLL_CIRC);
					lv_label_set_text_fmt(test.screen_label_9, "%s", weather_val[1]);	//地点
					lv_label_set_text_fmt(test.screen_label_7, "%s", weather_val[3]);	//地点
					lv_tabview_set_tab_act(test.screen_tabview_1,1, false);
				}
				else if(key_val == 3){
//					printf("按键3按下，显示日历\r\n");
					lv_tabview_set_tab_act(test.screen_tabview_1,2, false);
				}
			
			
        vTaskDelay(400);
    }
}

/**
  * @brief  任务5，校准实时时钟，大约每一个小时校准一次
  * @param  None
  * @retval None
  */
void task5(void *pvParameters){
	
    while(1){
			xSemaphoreTake(BINARY_SEMAPHORE_handle, 1000);
			printf("\r\n任务5正常\r\n");
//			taskENTER_CRITICAL();           //进入临界区
			printf("\r\n校准实时时钟\r\n");
//			wifi_init();
			Time_Adjust();
//			taskEXIT_CRITICAL();            //退出临界区
			xSemaphoreGive(BINARY_SEMAPHORE_handle);
		vTaskDelay(1000 * 60);
		
    }
}


/**
  * @brief  任务6，各种传感器采样
  * @param  None
  * @retval None
  */
void task6(void *pvParameters){
	
    while(1){
//			printf("\r\n任务6正常\r\n");
			//taskENTER_CRITICAL();           //进入临界区
//			printf("进入任务6\r\n");
			air_analysis();
			
			dht_get_val();
			
			ADC2VOL();
		//	taskEXIT_CRITICAL();            //退出临界区
		vTaskDelay(5000);
		
    }
}

/**
  * @brief  任务7，lvgl
  * @param  None
  * @retval None
  */
void task7(void *pvParameters){
	printf("\r\n任务7正常\r\n");
    
			TickType_t xLastWakeTime;
	const TickType_t xPeriod = pdMS_TO_TICKS( 5 );
	xLastWakeTime = xTaskGetTickCount();  
while(1){
		
//		printf("\r\n2\r\n");
		lv_task_handler();
		vTaskDelayUntil( &xLastWakeTime, xPeriod );//绝对延时
	}

		
    
}


/**
  * @brief  任务8，获取实时天气
  * @param  None
  * @retval None
  */
void task8(void *pvParameters){
	
    while(1){
			printf("\r\n任务8正常\r\n");
			xSemaphoreTake(BINARY_SEMAPHORE_handle, 1000);
//			taskENTER_CRITICAL();           //进入临界区//
//			printf("天气\r\n");
			connect_seniverse();
//			printf("\r\n打印数据\r\n");
			//whther_analisys();
			wifi_init();
//			taskEXIT_CRITICAL();            //退出临界区 
			
			xSemaphoreGive(BINARY_SEMAPHORE_handle);
		
			vTaskDelay(1000 * 60 * 20);
		
    }
}


/**
  * @brief  按键任务
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
//			printf("\r\n任务9正常\r\n");
			key = key_get_num();
			if(flag == 0){
				key =4;
			}
        if(key == 1){
					printf("按键1按下，显示空气数据\r\n");
          key_val = 1;
            }
				if(key == 2){
					printf("按键2按下，显示实时时钟\r\n");
					key_val = 2;
				}
				else if(key == 3){
					printf("按键3按下\r\n");
					key_val = 3;
				}
				else if(key == 4){
					flag = 2;
					taskENTER_CRITICAL();           //进入临界区//
					printf("按键4，俺下\r\n");
					vTaskSuspend(Task3_Handler);
					vTaskSuspend(Task4_Handler);
					vTaskSuspend(Task5_Handler);
					vTaskSuspend(Task8_Handler);
					memset(wifi_name, 0, 32);
					len_wifi = 0;
					sFLASH_ReadBuffer(&len_wifi, write_len_addr, 1);
					sFLASH_ReadBuffer(wifi_name, write_wifi_addr, len_wifi);
					lv_label_set_text(test2.screen_2_label_2_3, (const char *)wifi_name);
					lv_scr_load(test2.screen_2);//加载屏幕
					vTaskResume(Task10_Handler);
					printf("任务挂起完成\r\n");
					taskEXIT_CRITICAL();            //退出临界区 
					vTaskSuspend(Task9_Handler);
					
				}
           
				vTaskDelay(50);
        }
       
    
}


/**
  * @brief  任务10，设置wifi网络
  * @param  None
  * @retval None
  */
void task10(void *pvParameters){
	uint8_t key_val = 0;
	uint8_t flag = 0;
	printf("任务10正常运行\r\n");
    while(1){
//        printf("\r\n任务10正常\r\n");
      key_val = key_get_num();  
			if(key_val == 1){
				printf("按键1，按下\r\n");
				flag = 2;
//				lv_obj_set_style_local_bg_color(test2.screen_2_btn2_1, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, lv_color_make(0x28, 0xa6, 0xaf));
				lv_scr_load(test2.screen_3);
				
				setup_animation();
				
				ap_init();
				
				
	
			}
		else if(key_val == 2){
			taskENTER_CRITICAL();           //进入临界区//
			printf("按键2，按下\r\n");
			flag =0;
				lv_obj_set_style_local_bg_color(test2.screen_2_btn_2_2, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, lv_color_make(0x28, 0xa6, 0xaf));
				vTaskResume(Task3_Handler);
			  vTaskResume(Task4_Handler);
			  vTaskResume(Task5_Handler);
			  vTaskResume(Task8_Handler);
				vTaskResume(Task9_Handler);
				lv_scr_load(test.screen);
//				wifi_init();
				taskEXIT_CRITICAL();            //退出临界区 
				vTaskSuspend(Task10_Handler);
			
				
    }
		
		if(flag == 2){
//			printf("2");
			//printf("分析数据 %s", esp8266.RX_data);
			if(strstr((char *)esp8266.RX_data, "wifiname") != NULL)
				AP_analisys();
				}
		
		vTaskDelay(10);
	}
}


