#include "ucos_task.h"



OS_STK FINGER_TASK[FINGER_STACK_LEN];


void FingerTask(void *pdata)
{
	pdata=pdata;//��ֹ�������Ż�
	
	
	
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
		printf("ָ�ƽ����ɹ�\r\n");
		if(FlagStatus == 200)
			return;
		LCD_Clear(WHITE);
		lcd_display_string(36,60,(uint8_t*)"ָ�ƿ����ɹ�",RED,WHITE,FONT_SIZE_32,NULL);
		//���ת������
		
		Delay_ms(4000);
		//���ת������
		
	}
}



















