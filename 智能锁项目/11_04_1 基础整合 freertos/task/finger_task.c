#include "ucos_task.h"



OS_STK FINGER_TASK[FINGER_STACK_LEN];


void FingerTask(void *pdata)
{
	pdata=pdata;//防止编译器优化
	
	
	
	printf("finger task coming!!\r\n");
	while(1)
	{
		finger_task();
		
		OSTimeDly(50);
	}
	
}

void finger_task(void){
	uint8_t FlagStatus = 0;
	if((FlagStatus = match_fingerprints()) !=0){
		printf("指纹解锁成功\r\n");
		if(FlagStatus == 200)
			return;
		LCD_Clear(WHITE);
		lcd_display_string(36,60,(uint8_t*)"指纹开锁成功",RED,WHITE,FONT_SIZE_32,NULL);
		//电机转动开门
		
		Delay_ms(4000);
		//电机转动关门
		
	}
}



















