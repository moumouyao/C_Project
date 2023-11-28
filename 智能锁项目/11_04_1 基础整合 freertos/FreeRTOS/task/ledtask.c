#include "ucos_task.h"

OS_STK LED_TASK[LED_STACK_LEN];


void LedTask(void *pdata)
{
	pdata=pdata;//防止编译器优化
	
	
	
	printf("led task coming!!\r\n");
	while(1)
	{
//		printf("led任务\r\n");
		GPIO_SetBits(GPIOB, GPIO_Pin_8);
		OSTimeDly(50);

		GPIO_ResetBits(GPIOB, GPIO_Pin_8);
		OSTimeDly(50);
		
	}
	
}