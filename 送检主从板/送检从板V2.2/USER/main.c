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
#include "semphr.h"//������
#include "stdio.h"


#define LED0_TASK_PRIO 6 //�������ȼ�
#define LED0_STK_SIZE 50 //�����ջ��С
TaskHandle_t LED0Task_Handler; //������
void led0_task(void *p_arg); //������

#define RS485_TASK_PRIO 5 //�������ȼ�
#define RS485_STK_SIZE 2048 //�����ջ��С
TaskHandle_t RS485Task_Handler; //������
void Task_RS485RecHandle(void *pvParameters); //������

#define Task_TASK_PRIO 4 //�������ȼ�
#define Task_STK_SIZE 400 //�����ջ��С
TaskHandle_t TaskTask_Handler; //������
void Task_FrameStore(void *pvParameters); //������

#define Task_Frame_TASK_PRIO 3 //�������ȼ�
#define Task_Frame_STK_SIZE 400 //�����ջ��С
TaskHandle_t Task_Frame_Handler; //������
void Task_FrameDataLoad(void *pvParameters); //������

#define Task_FrameDisplay_TASK_PRIO 2 //�������ȼ�
#define Task_FrameDisplay_STK_SIZE 400 //�����ջ��С
TaskHandle_t Task_FrameDisplay_Handler; //������
void Task_FrameDisplay(void *pvParameters); //������

//485�ź���
SemaphoreHandle_t RS485TranSem=NULL;

//485�����жϷ��͸���������Ķ���
QueueHandle_t RS485RecMsgQeue =NULL;
#define  RS485_QUEUE_LEN    4   /* ���еĳ��ȣ����ɰ������ٸ���Ϣ */
#define  RS485_QUEUE_SIZE   sizeof(uint32_t)   /* ������ÿ����Ϣ��С���ֽڣ� */

//���������͸��洢���ݵĶ���
QueueHandle_t FrameStoreMsgQeue =NULL;
#define  Frame_QUEUE_LEN    4   /* ���еĳ��ȣ����ɰ������ٸ���Ϣ */
#define  Frame_QUEUE_SIZE   sizeof(uint32_t)   /* ������ÿ����Ϣ��С���ֽڣ� */


//���������͸�����֡���ݵĶ���
QueueHandle_t FrameLoadMbox =NULL;
#define  FrameLoad_QUEUE_LEN    4   /* ���еĳ��ȣ����ɰ������ٸ���Ϣ */
#define  FrameLoad_QUEUE_SIZE   sizeof(uint32_t)   /* ������ÿ����Ϣ��С���ֽڣ� */


//׼��Ҫ��ʾ�����������͸�����֡���ݼ��ص����ϵĶ���
QueueHandle_t FrameDisplayMbox =NULL;
#define  FrameDisplayMbox_QUEUE_LEN    4   /* ���еĳ��ȣ����ɰ������ٸ���Ϣ */
#define  FrameDisplayMbox_QUEUE_SIZE   sizeof(uint32_t)   /* ������ÿ����Ϣ��С���ֽڣ� */
	

int main(void)
{	
	BSP_Init();
	taskENTER_CRITICAL(); //�����ٽ���

	//����485�����жϷ��͸���������Ķ���
  RS485RecMsgQeue 	= xQueueCreate(	(UBaseType_t ) RS485_QUEUE_LEN,/* ��Ϣ���еĳ��� */
																		(UBaseType_t ) RS485_QUEUE_SIZE);/* ��Ϣ�Ĵ�С */
	//���������͸�����֡���ݵĶ���
	FrameStoreMsgQeue = xQueueCreate(	(UBaseType_t ) Frame_QUEUE_LEN,/* ��Ϣ���еĳ��� */
																		(UBaseType_t ) Frame_QUEUE_SIZE);/* ��Ϣ�Ĵ�С */
	//���������͸��洢���ݵĶ���
	FrameLoadMbox = xQueueCreate(	(UBaseType_t ) FrameLoad_QUEUE_LEN,/* ��Ϣ���еĳ��� */
																		(UBaseType_t ) FrameLoad_QUEUE_SIZE);/* ��Ϣ�Ĵ�С */
	//׼��Ҫ��ʾ�����������͸�����֡���ݼ��ص����ϵĶ���	
	FrameDisplayMbox = xQueueCreate(	(UBaseType_t ) FrameDisplayMbox_QUEUE_LEN,/* ��Ϣ���еĳ��� */
																		(UBaseType_t ) FrameDisplayMbox_QUEUE_SIZE);/* ��Ϣ�Ĵ�С */

#if Debug_printf
			if(RS485RecMsgQeue==NULL||FrameStoreMsgQeue==NULL||FrameLoadMbox==NULL||FrameDisplayMbox==NULL)
			printf("��������ʧ��\n\r");
#endif
//���� 485�ź���	
	RS485TranSem=xSemaphoreCreateMutex();//����485�ź���

	xSemaphoreGive(RS485TranSem);//�ͷ�485�ź���
	
//���� LED0 ����		
	xTaskCreate((TaskFunction_t )led0_task, 
							(const char* )"led0_task", 
							(uint16_t )LED0_STK_SIZE, 
							(void* )NULL,
							(UBaseType_t )LED0_TASK_PRIO,
							(TaskHandle_t* )&LED0Task_Handler); 


	//���� 485��������
	xTaskCreate((TaskFunction_t )Task_RS485RecHandle, 
							(const char* )"RS485_task", 
							(uint16_t )RS485_STK_SIZE, 
							(void* )NULL,
							(UBaseType_t )RS485_TASK_PRIO,
							(TaskHandle_t* )&RS485Task_Handler); 


	//���� ����֡�ļ����ݴ洢����
	xTaskCreate((TaskFunction_t )Task_FrameStore, 
							(const char* )"Task_Frame", 
							(uint16_t )Task_STK_SIZE, 
							(void* )NULL,
							(UBaseType_t )Task_TASK_PRIO,
							(TaskHandle_t* )&TaskTask_Handler); 
							
							
	//���� ׼��Ҫ��ʾ������				
	xTaskCreate((TaskFunction_t )Task_FrameDataLoad, 
							(const char* )"Task_FrameDataLoad", 
							(uint16_t )Task_Frame_STK_SIZE, 
							(void* )NULL,
							(UBaseType_t )Task_Frame_TASK_PRIO,
							(TaskHandle_t* )&Task_Frame_Handler); 				
							
							
	//�����ݷ��͵���ʾ����
	xTaskCreate((TaskFunction_t )Task_FrameDisplay, 
							(const char* )"Task_FrameDataLoad", 
							(uint16_t )Task_FrameDisplay_STK_SIZE, 
							(void* )NULL,
							(UBaseType_t )Task_FrameDisplay_TASK_PRIO,
							(TaskHandle_t* )&Task_FrameDisplay_Handler); 						
//							
	taskEXIT_CRITICAL(); //�˳��ٽ���
#if Debug_printf
					printf("Free OS\n\r");
#endif
	vTaskStartScheduler();															//����������� 

while(1){
	delay_sys_ms(1000);
	LED0=~LED0;
	printf("hello world\r\n");
	delay_sys_ms(1000);
	LED0=~LED0;
}
	 
}

//LED0 ������
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


//485ͨѶ������
void Task_RS485RecHandle(void *pvParameters)
{
	SlaveAckInit(SlaveAckOKBuf,OK);//��ʼ��Ӧ��ɹ�BUF
	uint32_t crc32,re_crc32;
	uint8_t *p=NULL;	
	uint16_t num;
	BaseType_t xReturn = pdTRUE;
	while(1)
	{
		xReturn = xQueueReceive( RS485RecMsgQeue,    /* ��Ϣ���еľ�� */
														 &p,      /* ���͵���Ϣ���� */
														 portMAX_DELAY); /* �ȴ�ʱ�� */
			if(xReturn == pdTRUE)
			{
				num = *(uint16_t *)p;
				crc32 = UserHWCRC32(p+2,num-2-4);//CRCУ��-�������ݽ���CRC
				re_crc32 = *(uint32_t *)(p+num-4);
				
				if(crc32 == re_crc32)
				{
					RecDataHandle(p);//�����������ݴ���,��ǰ��λ�����ݳ���	
				}else
				{	
					printf("CRCerr\r\n");
					myfree(SRAMIN,(void *)p);							
				}
			}
	}
}
//����֡�ļ����ݴ洢
void Task_FrameStore(void *pvParameters)
{	
	BaseType_t xReturn = pdTRUE;
	uint8_t *p;
	
#if Debug_printf
		printf("Task3_FrameStore start up\n\r");
#endif
	
	while(1)
	{
		xReturn = xQueueReceive( FrameStoreMsgQeue,    /* ��Ϣ���еľ�� */
														 &p,      /* ���͵���Ϣ���� */
														 portMAX_DELAY); /* �ȴ�ʱ�� */
			if(xReturn == pdTRUE)
			{
				FrameStoreHandle(p);                   //���ݴ洢����
				myfree(SRAMIN,(void *)p);		
			}		
	}
} 

//׼��Ҫ��ʾ������
void Task_FrameDataLoad(void *pvParameters)
{
	uint8_t *p=NULL;
	LoadDataNode pnode;    //��ǰҪ��ʾ���ݼ��ؽ��
	static LoadDataNode last_node;

	BaseType_t xReturn = pdTRUE;
#if Debug_printf
	printf("\n\r Task5_FrameDataPre start up");
#endif
	//�ȴ��������������������--���ڲ���Ҫ
	GetSetEEPROM();
#if Debug_printf
	if(G_FrameFlag != 1)//flash�д����������ݱ�־λ
		printf("\n\r δ����������");
	else 
		printf("\n\r ����������");
#endif
	if(G_FrameFlag == 1 && ScreenBoxSetFlag == 1)//���������ݲ�����������ģ������Ѿ�����
	{		
		SPI_FlashRead(&G_FrameNum,FLASH_FRAME_TAB_ADDR,1);//��ȡ�ܵ�֡��
		GetLoadNodeTab(LoadNodeTab,G_FrameNum);           //����֡����ͷ��
	}
#if Debug_printf
	printf("\n\r �ܵ�֡��:%d",G_FrameNum);
#endif
  //�ж��Ƿ�洢����������֡����
  //�����������ȡУ����	
	//����һ������֡����֡��+FLASH��ַ+����
	while(1)
	{
		xReturn = xQueueReceive( FrameLoadMbox,    /* ��Ϣ���еľ�� */
														 &p,      /* ���͵���Ϣ���� */
														 portMAX_DELAY); /* �ȴ�ʱ�� */

		if(xReturn == pdTRUE)
			{		
				if(G_NewFramesFlag == 1)//����֡��־λ
				{
					last_node.frame_no = 0xff;  //��ʼ����¼�ϴμ��ص�֡�źͲ���
					last_node.step = 0xffff;
					G_NewFramesFlag = 0;  //����
				}
				pnode.frame_no = p[7];
				pnode.step = *((uint16_t *)(p+8));
				if(pnode.frame_no == last_node.frame_no && pnode.step == last_node.step)
				{
					G_LoadDataFlag = 0;   //װ��������ɱ�־λ����
					xReturn=xQueueSend(FrameDisplayMbox,(void *)&DisplayBuf,0);//���͵�����
				}
				else
				{
					if(G_FrameFlag == 1 && ScreenBoxSetFlag ==1 && pnode.frame_no < G_FrameNum )
					{			     
						G_LoadDataFlag = 0; //װ��������ɱ�־λ����
						LoadDataHandle(DisplayBuf,&pnode);
						xReturn=xQueueSend(FrameDisplayMbox,(void *)&DisplayBuf,0);//���͵�����			 
						last_node.frame_no = pnode.frame_no;
						last_node.step = pnode.step;
					}
				}
						myfree(SRAMIN,(void *)p);	
			}
	}		
}


//����֡���ݼ��ص�����
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
		xReturn = xQueueReceive( FrameDisplayMbox,    /* ��Ϣ���еľ�� */
														 &p,      /* ���͵���Ϣ���� */
														 portMAX_DELAY); /* �ȴ�ʱ�� */
			if(xReturn == pdTRUE)
			{
				if(G_FrameFlag == 1 && ScreenBoxSetFlag ==1)
				{
					LedDisplay();
#if Debug_printf
		printf("\n\rLedDisplay ok!!!\n\r");
#endif
					G_LoadDataFlag = 1; //װ��������ɱ�־λ��1		
				}
			}
	}		
}

void vApplicationStackOverflowHook(TaskHandle_t xTask, char* pcTaskName){
	while(1){
	
	}
}
