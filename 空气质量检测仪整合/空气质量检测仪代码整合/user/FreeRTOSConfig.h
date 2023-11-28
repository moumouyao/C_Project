#ifndef FREERTOS_CONFIG_H
#define FREERTOS_CONFIG_H
#include "stm32f10x.h"
/*  我们可以添加的宏定义  */
#define configUSE_TIME_SLICING  1  //使能时间片调度(默认式使能的)
#define configUSE_PORT_OPTIMISED_TASK_SELECTION 1  //硬件计算前导零指令，如果所使用的， MCU 没有这些硬件指令的话此宏应该设置为 0
#define configUSE_TICKLESS_IDLE  0  //置 1：使能低功耗 tickless 模式；置 0：保持系统节拍（tick）中断一直运行
#define configUSE_QUEUE_SETS 1  //启用队列
#define configUSE_TASK_NOTIFICATIONS 1  //开启任务通知功能，默认开启
#define configUSE_MUTEXES  1  //使用互斥信号量
#define configUSE_RECURSIVE_MUTEXES 1  //使用递归互斥信号量
#define configUSE_COUNTING_SEMAPHORES 1  //为 1 时使用计数信号量
#define configQUEUE_REGISTRY_SIZE 10 //设置可以注册的信号量和消息队列个数
#define configUSE_APPLICATION_TASK_TAG 0 
#define configSUPPORT_DYNAMIC_ALLOCATION 1  //支持动态内存申请
#define configUSE_MALLOC_FAILED_HOOK 0  //使用内存申请失败钩子函数
#define configCHECK_FOR_STACK_OVERFLOW 1// 大于 0 时启用堆栈溢出检测功能，如果使用此功能用户必须提供一个栈溢出钩子函数如果使用的话此值可以为 1 或者 2，因为有两种栈溢出检测方法
#define configGENERATE_RUN_TIME_STATS 0 //启用运行时间统计功能
#define configUSE_STATS_FORMATTING_FUNCTIONS 1
#define configUSE_TIMERS 1  //启用软件定时器
#define configTIMER_TASK_PRIORITY (configMAX_PRIORITIES-1) //软件定时器优先级
#define configTIMER_QUEUE_LENGTH 10 //软件定时器队列长度
#define configTIMER_TASK_STACK_DEPTH (configMINIMAL_STACK_SIZE*2)  //软件定时器任务堆栈大小
//可选函数配置选项
#define INCLUDE_xTaskGetSchedulerState 1
#define INCLUDE_eTaskGetState 1
#define INCLUDE_xTimerPendFunctionCall 1
//中断服务函数 也可以修改起始文件
#define vPortSVCHandler SVC_Handler
#define xPortPendSVHandler PendSV_Handler
#define xPortSysTickHandler SysTick_Handler
/*源文件自带宏定义 可以修改*/
#define configUSE_PREEMPTION  1  //置 1： RTOS 使用抢占式调度器；置 0：RTOS 使用协作式调度器（时间片）
#define configUSE_IDLE_HOOK 0  //置 1：使用空闲钩子（Idle Hook 类似于回调函数）；置 0：忽略空闲钩子
#define configUSE_TICK_HOOK 1  //置 1：使用时间片钩子（Tick Hook）；置 0：忽略时间片钩子
#define configCPU_CLOCK_HZ ( ( unsigned long ) 72000000 )  // 写 入 实 际 的CPU 内核时钟频率，也就是 CPU 指令执行频率
#define configTICK_RATE_HZ  ( ( TickType_t ) 1000 ) //RTOS 系 统 节拍中断的频率。即一秒中断的次数，每次中断 RTOS 都会进行任务调度
#define configMAX_PRIORITIES ( 10 ) //可使用的最大优先级
#define configMINIMAL_STACK_SIZE ( ( unsigned short ) 128 )  //空闲任务使用的堆栈大小
#define configTOTAL_HEAP_SIZE  ( ( size_t ) ( 25 * 1024 ) )  //系统所有总的堆大小
#define configMAX_TASK_NAME_LEN  ( 16 )  //任务名字字符串长度
#define configUSE_TRACE_FACILITY 0  //启用可视化跟踪调试
#define configUSE_16_BIT_TICKS 0  //系统节拍计数器变量数据类型， 1 表示为16 位无符号整形， 0 表示为 32 位无符号整形
#define configIDLE_SHOULD_YIELD 1  //空闲任务放弃 CPU 使用权给其他同优先级的用户任务
/* Co-routine definitions. */
#define configUSE_CO_ROUTINES 0  //启用协程，启用协程以后必须添加文件croutine.c
#define configMAX_CO_ROUTINE_PRIORITIES ( 2 ) //协程的有效优先级数目
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
#define configLIBRARY_KERNEL_INTERRUPT_PRIORITY 15 //中断最低优先级#
#endif
