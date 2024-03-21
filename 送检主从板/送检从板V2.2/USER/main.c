#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "bsp.h"
#include "wdg.h"
#include "sw_led.h"
#include "TIM.h"
#include "sw_rechandle.h"
#include "sw_config.h"
#include "user_crc.h"
#include "malloc.h"
#include "flash.h"

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"//互斥量
#include "stdio.h"


#define LED0_TASK_PRIO 6 //任务优先级
#define LED0_STK_SIZE 50 //任务堆栈大小
TaskHandle_t LED0Task_Handler; //任务句柄
void led0_task(void *p_arg); //任务函数

#define RS485_TASK_PRIO 5 //任务优先级
#define RS485_STK_SIZE 2048 //任务堆栈大小
TaskHandle_t RS485Task_Handler; //任务句柄
void Task_RS485RecHandle(void *pvParameters); //任务函数

#define Task_TASK_PRIO 4 //任务优先级
#define Task_STK_SIZE 400 //任务堆栈大小
TaskHandle_t TaskTask_Handler; //任务句柄
void Task_FrameStore(void *pvParameters); //任务函数

#define Task_Frame_TASK_PRIO 3 //任务优先级
#define Task_Frame_STK_SIZE 400 //任务堆栈大小
TaskHandle_t Task_Frame_Handler; //任务句柄
void Task_FrameDataLoad(void *pvParameters); //任务函数

#define Task_FrameDisplay_TASK_PRIO 2 //任务优先级
#define Task_FrameDisplay_STK_SIZE 400 //任务堆栈大小
TaskHandle_t Task_FrameDisplay_Handler; //任务句柄
void Task_FrameDisplay(void *pvParameters); //任务函数

//485信号量
SemaphoreHandle_t RS485TranSem=NULL;

//485串口中断发送给处理任务的队列
QueueHandle_t RS485RecMsgQeue =NULL;
#define  RS485_QUEUE_LEN    4   /* 队列的长度，最大可包含多少个消息 */
#define  RS485_QUEUE_SIZE   sizeof(uint32_t)   /* 队列中每个消息大小（字节） */

//处理任务发送给存储数据的队列
QueueHandle_t FrameStoreMsgQeue =NULL;
#define  Frame_QUEUE_LEN    4   /* 队列的长度，最大可包含多少个消息 */
#define  Frame_QUEUE_SIZE   sizeof(uint32_t)   /* 队列中每个消息大小（字节） */


//处理任务发送给播放帧数据的队列
QueueHandle_t FrameLoadMbox =NULL;
#define  FrameLoad_QUEUE_LEN    4   /* 队列的长度，最大可包含多少个消息 */
#define  FrameLoad_QUEUE_SIZE   sizeof(uint32_t)   /* 队列中每个消息大小（字节） */


//准备要显示的数据任务发送给播放帧数据加载到屏上的队列
QueueHandle_t FrameDisplayMbox =NULL;
#define  FrameDisplayMbox_QUEUE_LEN    4   /* 队列的长度，最大可包含多少个消息 */
#define  FrameDisplayMbox_QUEUE_SIZE   sizeof(uint32_t)   /* 队列中每个消息大小（字节） */
	

int main(void)
{	
	BSP_Init();
	taskENTER_CRITICAL(); //进入临界区

	//创建485串口中断发送给处理任务的队列
  RS485RecMsgQeue 	= xQueueCreate(	(UBaseType_t ) RS485_QUEUE_LEN,/* 消息队列的长度 */
																		(UBaseType_t ) RS485_QUEUE_SIZE);/* 消息的大小 */
	//处理任务发送给播放帧数据的队列
	FrameStoreMsgQeue = xQueueCreate(	(UBaseType_t ) Frame_QUEUE_LEN,/* 消息队列的长度 */
																		(UBaseType_t ) Frame_QUEUE_SIZE);/* 消息的大小 */
	//处理任务发送给存储数据的队列
	FrameLoadMbox = xQueueCreate(	(UBaseType_t ) FrameLoad_QUEUE_LEN,/* 消息队列的长度 */
																		(UBaseType_t ) FrameLoad_QUEUE_SIZE);/* 消息的大小 */
	//准备要显示的数据任务发送给播放帧数据加载到屏上的队列	
	FrameDisplayMbox = xQueueCreate(	(UBaseType_t ) FrameDisplayMbox_QUEUE_LEN,/* 消息队列的长度 */
																		(UBaseType_t ) FrameDisplayMbox_QUEUE_SIZE);/* 消息的大小 */

#if Debug_printf
			if(RS485RecMsgQeue==NULL||FrameStoreMsgQeue==NULL||FrameLoadMbox==NULL||FrameDisplayMbox==NULL)
			printf("队列申请失败\n\r");
#endif
//创建 485信号量	
	RS485TranSem=xSemaphoreCreateMutex();//申请485信号量

	xSemaphoreGive(RS485TranSem);//释放485信号量
	
//创建 LED0 任务		
	xTaskCreate((TaskFunction_t )led0_task, 
							(const char* )"led0_task", 
							(uint16_t )LED0_STK_SIZE, 
							(void* )NULL,
							(UBaseType_t )LED0_TASK_PRIO,
							(TaskHandle_t* )&LED0Task_Handler); 


	//创建 485接收任务
	xTaskCreate((TaskFunction_t )Task_RS485RecHandle, 
							(const char* )"RS485_task", 
							(uint16_t )RS485_STK_SIZE, 
							(void* )NULL,
							(UBaseType_t )RS485_TASK_PRIO,
							(TaskHandle_t* )&RS485Task_Handler); 


	//创建 播放帧文件数据存储任务
	xTaskCreate((TaskFunction_t )Task_FrameStore, 
							(const char* )"Task_Frame", 
							(uint16_t )Task_STK_SIZE, 
							(void* )NULL,
							(UBaseType_t )Task_TASK_PRIO,
							(TaskHandle_t* )&TaskTask_Handler); 
							
							
	//创建 准备要显示的数据				
	xTaskCreate((TaskFunction_t )Task_FrameDataLoad, 
							(const char* )"Task_FrameDataLoad", 
							(uint16_t )Task_Frame_STK_SIZE, 
							(void* )NULL,
							(UBaseType_t )Task_Frame_TASK_PRIO,
							(TaskHandle_t* )&Task_Frame_Handler); 				
							
							
	//将数据发送到显示屏上
	xTaskCreate((TaskFunction_t )Task_FrameDisplay, 
							(const char* )"Task_FrameDataLoad", 
							(uint16_t )Task_FrameDisplay_STK_SIZE, 
							(void* )NULL,
							(UBaseType_t )Task_FrameDisplay_TASK_PRIO,
							(TaskHandle_t* )&Task_FrameDisplay_Handler); 						
//							
	taskEXIT_CRITICAL(); //退出临界区
#if Debug_printf
					printf("Free OS\n\r");
#endif
	vTaskStartScheduler();															//开启任务调度 

while(1){
	delay_sys_ms(1000);
	LED0=~LED0;
	printf("hello world\r\n");
	delay_sys_ms(1000);
	LED0=~LED0;
}
	 
}

//LED0 任务函数
void led0_task(void *pvParameters)
{	
	while(1)
	{
		vTaskDelay(1000);
//		printf("hello world\r\n");
		LED0=~LED0;
		IWDG_Feed();
		SlaveAckInit(SlaveAckFaultBuf,6);
	}
} 


//485通讯任务函数
void Task_RS485RecHandle(void *pvParameters)
{
	SlaveAckInit(SlaveAckOKBuf,OK);//初始化应答成功BUF
	uint32_t crc32,re_crc32;
	uint8_t *p=NULL;	
	uint16_t num;
	BaseType_t xReturn = pdTRUE;
	while(1)
	{
		xReturn = xQueueReceive( RS485RecMsgQeue,    /* 消息队列的句柄 */
														 &p,      /* 发送的消息内容 */
														 portMAX_DELAY); /* 等待时间 */
			if(xReturn == pdTRUE)
			{
				num = *(uint16_t *)p;
				crc32 = UserHWCRC32(p+2,num-2-4);//CRC校验-整体数据进行CRC
				re_crc32 = *(uint32_t *)(p+num-4);
				
				if(crc32 == re_crc32)
				{
					RecDataHandle(p);//接收主机数据处理,带前两位总数据长度	
				}else
				{	
					printf("CRCerr\r\n");
					myfree(SRAMIN,(void *)p);							
				}
			}
	}
}
//播放帧文件数据存储
void Task_FrameStore(void *pvParameters)
{	
	BaseType_t xReturn = pdTRUE;
	uint8_t *p;
	
#if Debug_printf
		printf("Task3_FrameStore start up\n\r");
#endif
	
	while(1)
	{
		xReturn = xQueueReceive( FrameStoreMsgQeue,    /* 消息队列的句柄 */
														 &p,      /* 发送的消息内容 */
														 portMAX_DELAY); /* 等待时间 */
			if(xReturn == pdTRUE)
			{
				FrameStoreHandle(p);                   //数据存储处理
				myfree(SRAMIN,(void *)p);		
			}		
	}
} 

//准备要显示的数据
void Task_FrameDataLoad(void *pvParameters)
{
	uint8_t *p=NULL;
	LoadDataNode pnode;    //当前要显示数据加载结点
	static LoadDataNode last_node;

	BaseType_t xReturn = pdTRUE;
#if Debug_printf
	printf("\n\r Task5_FrameDataPre start up");
#endif
	//等待主机发送箱体屏体参数--现在不需要
	GetSetEEPROM();
#if Debug_printf
	if(G_FrameFlag != 1)//flash中存有完整数据标志位
		printf("\n\r 未有完整数据");
	else 
		printf("\n\r 有完整数据");
#endif
	if(G_FrameFlag == 1 && ScreenBoxSetFlag == 1)//有完整数据并且屏体箱体模组参数已经设置
	{		
		SPI_FlashRead(&G_FrameNum,FLASH_FRAME_TAB_ADDR,1);//读取总的帧数
		GetLoadNodeTab(LoadNodeTab,G_FrameNum);           //所有帧加载头表
	}
#if Debug_printf
	printf("\n\r 总的帧数:%d",G_FrameNum);
#endif
  //判断是否存储有完整播放帧数据
  //如果有数据则取校验结果	
	//建立一个播放帧链表：帧号+FLASH地址+步数
	while(1)
	{
		xReturn = xQueueReceive( FrameLoadMbox,    /* 消息队列的句柄 */
														 &p,      /* 发送的消息内容 */
														 portMAX_DELAY); /* 等待时间 */

		if(xReturn == pdTRUE)
			{		
				if(G_NewFramesFlag == 1)//更新帧标志位
				{
					last_node.frame_no = 0xff;  //初始化记录上次加载的帧号和步数
					last_node.step = 0xffff;
					G_NewFramesFlag = 0;  //清零
				}
				pnode.frame_no = p[7];
				pnode.step = *((uint16_t *)(p+8));
				if(pnode.frame_no == last_node.frame_no && pnode.step == last_node.step)
				{
					G_LoadDataFlag = 0;   //装载数据完成标志位清零
					xReturn=xQueueSend(FrameDisplayMbox,(void *)&DisplayBuf,0);//发送到队列
				}
				else
				{
					if(G_FrameFlag == 1 && ScreenBoxSetFlag ==1 && pnode.frame_no < G_FrameNum )
					{			     
						G_LoadDataFlag = 0; //装载数据完成标志位清零
						LoadDataHandle(DisplayBuf,&pnode);
						xReturn=xQueueSend(FrameDisplayMbox,(void *)&DisplayBuf,0);//发送到队列			 
						last_node.frame_no = pnode.frame_no;
						last_node.step = pnode.step;
					}
				}
						myfree(SRAMIN,(void *)p);	
			}
	}		
}


//播放帧数据加载到屏上
void Task_FrameDisplay(void *p_arg)
{
#if Debug_printf
	printf("\n\r Task4_FrameDisplay start up");
	printf("\n\r *******Slave%d start up*******",G_IDSlave0);
#endif
	BaseType_t xReturn = pdTRUE;	
	uint8_t p=NULL;
	while(1)
	{
		xReturn = xQueueReceive( FrameDisplayMbox,    /* 消息队列的句柄 */
														 &p,      /* 发送的消息内容 */
														 portMAX_DELAY); /* 等待时间 */
			if(xReturn == pdTRUE)
			{
				if(G_FrameFlag == 1 && ScreenBoxSetFlag ==1)
				{
					LedDisplay();
#if Debug_printf
		printf("\n\rLedDisplay ok!!!\n\r");
#endif
					G_LoadDataFlag = 1; //装载数据完成标志位置1		
				}
			}
	}		
}

void vApplicationStackOverflowHook(TaskHandle_t xTask, char* pcTaskName){
	while(1){
	
	}
}
