#include "myrtos.h"


void TimeTask(void *pdata)
{
	printf("finger task coming!!\r\n");
	while(1){
//        printf("\r\n时间显示任务正在运行\r\n");
    
		vTaskDelay(600);

    }
	
}
