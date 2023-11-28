#include "myrtos.h"






void FingerTask(void *pdata)
{
	printf("finger task coming!!\r\n");
	while(1){
//        printf("\r\n任务1正常\r\n");
    finger_task();
		vTaskDelay(10);

    }
	
}

void finger_task(void){
	uint8_t FlagStatus = 0;
	if((FlagStatus = match_fingerprints()) !=0){
		printf("指纹解锁成功\r\n");
		send_voi_cmd(Welcome_home);
		if(FlagStatus == 200)
			return;
		LCD_Clear(WHITE);
		lcd_display_string(36,60,(uint8_t*)"指纹开锁成功",RED,WHITE,FONT_SIZE_32,NULL);
		//电机转动开门
		send_voi_cmd(Welcome_home);
		open_door(3);
		//电机转动关门
		lcd_display_string(36,60,(uint8_t*)"             ",RED,WHITE,FONT_SIZE_32,NULL);
		
	}
	else{
		send_voi_cmd(opening_failure);
	}
}



















