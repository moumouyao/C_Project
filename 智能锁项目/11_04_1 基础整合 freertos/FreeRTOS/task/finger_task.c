#include "myrtos.h"






void FingerTask(void *pdata)
{
	printf("finger task coming!!\r\n");
	while(1){
//        printf("\r\n����1����\r\n");
    finger_task();
		vTaskDelay(10);

    }
	
}

void finger_task(void){
	uint8_t FlagStatus = 0;
	if((FlagStatus = match_fingerprints()) !=0){
		printf("ָ�ƽ����ɹ�\r\n");
		send_voi_cmd(Welcome_home);
		if(FlagStatus == 200)
			return;
		LCD_Clear(WHITE);
		lcd_display_string(36,60,(uint8_t*)"ָ�ƿ����ɹ�",RED,WHITE,FONT_SIZE_32,NULL);
		//���ת������
		send_voi_cmd(Welcome_home);
		open_door(3);
		//���ת������
		lcd_display_string(36,60,(uint8_t*)"             ",RED,WHITE,FONT_SIZE_32,NULL);
		
	}
	else{
		send_voi_cmd(opening_failure);
	}
}



















