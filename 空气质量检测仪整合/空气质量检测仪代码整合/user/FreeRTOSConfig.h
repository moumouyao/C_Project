#ifndef FREERTOS_CONFIG_H
#define FREERTOS_CONFIG_H
#include "stm32f10x.h"
/*  ���ǿ�����ӵĺ궨��  */
#define configUSE_TIME_SLICING  1  //ʹ��ʱ��Ƭ����(Ĭ��ʽʹ�ܵ�)
#define configUSE_PORT_OPTIMISED_TASK_SELECTION 1  //Ӳ������ǰ����ָ������ʹ�õģ� MCU û����ЩӲ��ָ��Ļ��˺�Ӧ������Ϊ 0
#define configUSE_TICKLESS_IDLE  0  //�� 1��ʹ�ܵ͹��� tickless ģʽ���� 0������ϵͳ���ģ�tick���ж�һֱ����
#define configUSE_QUEUE_SETS 1  //���ö���
#define configUSE_TASK_NOTIFICATIONS 1  //��������֪ͨ���ܣ�Ĭ�Ͽ���
#define configUSE_MUTEXES  1  //ʹ�û����ź���
#define configUSE_RECURSIVE_MUTEXES 1  //ʹ�õݹ黥���ź���
#define configUSE_COUNTING_SEMAPHORES 1  //Ϊ 1 ʱʹ�ü����ź���
#define configQUEUE_REGISTRY_SIZE 10 //���ÿ���ע����ź�������Ϣ���и���
#define configUSE_APPLICATION_TASK_TAG 0 
#define configSUPPORT_DYNAMIC_ALLOCATION 1  //֧�ֶ�̬�ڴ�����
#define configUSE_MALLOC_FAILED_HOOK 0  //ʹ���ڴ�����ʧ�ܹ��Ӻ���
#define configCHECK_FOR_STACK_OVERFLOW 1// ���� 0 ʱ���ö�ջ�����⹦�ܣ����ʹ�ô˹����û������ṩһ��ջ������Ӻ������ʹ�õĻ���ֵ����Ϊ 1 ���� 2����Ϊ������ջ�����ⷽ��
#define configGENERATE_RUN_TIME_STATS 0 //��������ʱ��ͳ�ƹ���
#define configUSE_STATS_FORMATTING_FUNCTIONS 1
#define configUSE_TIMERS 1  //���������ʱ��
#define configTIMER_TASK_PRIORITY (configMAX_PRIORITIES-1) //�����ʱ�����ȼ�
#define configTIMER_QUEUE_LENGTH 10 //�����ʱ�����г���
#define configTIMER_TASK_STACK_DEPTH (configMINIMAL_STACK_SIZE*2)  //�����ʱ�������ջ��С
//��ѡ��������ѡ��
#define INCLUDE_xTaskGetSchedulerState 1
#define INCLUDE_eTaskGetState 1
#define INCLUDE_xTimerPendFunctionCall 1
//�жϷ����� Ҳ�����޸���ʼ�ļ�
#define vPortSVCHandler SVC_Handler
#define xPortPendSVHandler PendSV_Handler
#define xPortSysTickHandler SysTick_Handler
/*Դ�ļ��Դ��궨�� �����޸�*/
#define configUSE_PREEMPTION  1  //�� 1�� RTOS ʹ����ռʽ���������� 0��RTOS ʹ��Э��ʽ��������ʱ��Ƭ��
#define configUSE_IDLE_HOOK 0  //�� 1��ʹ�ÿ��й��ӣ�Idle Hook �����ڻص����������� 0�����Կ��й���
#define configUSE_TICK_HOOK 1  //�� 1��ʹ��ʱ��Ƭ���ӣ�Tick Hook������ 0������ʱ��Ƭ����
#define configCPU_CLOCK_HZ ( ( unsigned long ) 72000000 )  // д �� ʵ �� ��CPU �ں�ʱ��Ƶ�ʣ�Ҳ���� CPU ָ��ִ��Ƶ��
#define configTICK_RATE_HZ  ( ( TickType_t ) 1000 ) //RTOS ϵ ͳ �����жϵ�Ƶ�ʡ���һ���жϵĴ�����ÿ���ж� RTOS ��������������
#define configMAX_PRIORITIES ( 10 ) //��ʹ�õ�������ȼ�
#define configMINIMAL_STACK_SIZE ( ( unsigned short ) 128 )  //��������ʹ�õĶ�ջ��С
#define configTOTAL_HEAP_SIZE  ( ( size_t ) ( 25 * 1024 ) )  //ϵͳ�����ܵĶѴ�С
#define configMAX_TASK_NAME_LEN  ( 16 )  //���������ַ�������
#define configUSE_TRACE_FACILITY 0  //���ÿ��ӻ����ٵ���
#define configUSE_16_BIT_TICKS 0  //ϵͳ���ļ����������������ͣ� 1 ��ʾΪ16 λ�޷������Σ� 0 ��ʾΪ 32 λ�޷�������
#define configIDLE_SHOULD_YIELD 1  //����������� CPU ʹ��Ȩ������ͬ���ȼ����û�����
/* Co-routine definitions. */
#define configUSE_CO_ROUTINES 0  //����Э�̣�����Э���Ժ��������ļ�croutine.c
#define configMAX_CO_ROUTINE_PRIORITIES ( 2 ) //Э�̵���Ч���ȼ���Ŀ
/* Set the following definitions to 1 to include the API function, or zero
to exclude the API function. */
#define INCLUDE_vTaskPrioritySet  1
#define INCLUDE_uxTaskPriorityGet 1
#define INCLUDE_vTaskDelete 1
#define INCLUDE_vTaskCleanUpResources  1
#define INCLUDE_vTaskSuspend 1
#define INCLUDE_vTaskDelayUntil  1
#define INCLUDE_vTaskDelay  1
/* This is the raw value as per the Cortex-M3 NVIC. Values can be 255
(lowest) to 0 (1?) (highest). */
#define configKERNEL_INTERRUPT_PRIORITY 255
/* !!!! configMAX_SYSCALL_INTERRUPT_PRIORITY must not be set to zero !!!!
See http://www.FreeRTOS.org/RTOS-Cortex-M3-M4.html. */
#define configMAX_SYSCALL_INTERRUPT_PRIORITY 191 /* equivalent to 0xb0, or priority
11. */
/* This is the value being used as per the ST library which permits 16
priority values, 0 to 15. This must correspond to the
configKERNEL_INTERRUPT_PRIORITY setting. Here 15 corresponds to the lowest
NVIC value of 255. */
#define configLIBRARY_KERNEL_INTERRUPT_PRIORITY 15 //�ж�������ȼ�#
#endif
