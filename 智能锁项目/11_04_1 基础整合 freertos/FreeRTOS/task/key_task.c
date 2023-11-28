#include "myrtos.h"


extern uint8_t door_passwd[];
extern uint8_t ad_passwd[];
extern uint8_t row;
extern uint8_t lcdabs;
extern uint8_t counter;


extern TaskHandle_t StartTask_Handler;           		 //������
extern TaskHandle_t led_Handler;                    //������
extern TaskHandle_t finger_Handler;                    //������
extern TaskHandle_t time_Handler;                    //������
extern TaskHandle_t key_Handler;                    //������
extern TaskHandle_t adm_Handler;                    //������




void KeyTask(void *pdata)
{
	pdata=pdata;//��ֹ�������Ż�
	
	
	
	printf("key task coming!!\r\n");
	 uint8_t keybuf[6]={0};
	 uint8_t keylen=0;
	uint8_t wait_time=0;
	uint8_t i=0;
	uint8_t key_val=0;


	printf("Ckey task coming!!\r\n");
	
	
	while(1)
	{
		row = 100;
		lcdabs = keylen * 32 - 16 * 6;
		while(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_1) && wait_time <= 7){
			
			vTaskDelay(100);
		}
		if(wait_time > 7){
//			lcd_display_string(0 ,100, (uint8																										)"           ",WHITE,WHITE,FONT_SIZE_32,NULL);
			memset(keybuf, 0, 6);
			i = 0;
			wait_time = 0;
		}
		wait_time = 0;
		key_val = keyboard_input();	
		lcd_display_string(keylen * 32,100,(uint8_t *)"*",BLUE,WHITE,FONT_SIZE_32,NULL);
			keybuf[i++] = key_val;
			keylen ++;
		printf("key = %s\r\n",keybuf);
		if(keylen==6)
		{
			keylen=0;
			printf("key = %s\r\n",keybuf);
			printf("door_posswd = %s", door_passwd);
			printf("adm_posswd = %s", door_passwd);
			if(0==strcmp((const char *)door_passwd,(const char *)keybuf))
			{
			
				//����
				printf("�����ɹ�\r\n");
				send_voi_cmd(Welcome_home);
				lcd_display_string(0 ,100, (uint8_t*)"           ",WHITE,WHITE,FONT_SIZE_32,NULL);
				lcd_display_string(0 ,100, (uint8_t*)"�����ɹ�",BLACK,WHITE,FONT_SIZE_32,NULL);
				open_door(3);
				lcd_display_string(0 ,100, (uint8_t*)"           ",WHITE,WHITE,FONT_SIZE_32,NULL);
				memset(keybuf, 0, 6);
				i = 0;
			}
			else if(0==strcmp((const char *)keybuf,(const char *)ad_passwd))//�������Աģʽ����ɹ�
			{
				printf("%s\r\n",keybuf);
				
				printf("�������Աģʽ\r\n");
				memset(keybuf, 0, 6);
				i = 0;
					taskENTER_CRITICAL();           //�����ٽ���
					vTaskSuspend(finger_Handler);
					vTaskSuspend(time_Handler);
					vTaskSuspend(key_Handler)  ;
					vTaskResume(adm_Handler)  ;
					taskEXIT_CRITICAL();            //�˳��ٽ���

				
//					vTaskSuspend(Task8_Handler);
			}
			else//�������
			{
				printf("�������\r\n");
				send_voi_cmd(opening_failure);
				printf("��������� %s", keybuf);
				lcd_display_string(0 ,100, (uint8_t*)"           ",RED,WHITE,FONT_SIZE_32,NULL);
				lcd_display_string(0 ,100, (uint8_t*)"�������",RED,WHITE,FONT_SIZE_32,NULL);
				Delay_ms(3000);
				lcd_display_string(0 ,100, (uint8_t*)"           ",RED,WHITE,FONT_SIZE_32,NULL);
				memset(keybuf, 0, 6);
				i = 0;
				
				
			}
			}
		

		}
	
}



void key2passwd(void)
{
	 uint8_t keybuf[6]={0};
	 uint8_t keylen=0;
	uint8_t wait_time=0;
	uint8_t i=0;
	uint8_t key_val=0;


	printf("Ckey task coming!!\r\n");
	
	
	while(1)
	{
		row = 100;
		lcdabs = keylen * 32;
		while(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_1) && wait_time <= 7);
		if(wait_time > 7){
			lcd_display_string(0 ,100, (uint8_t*)"           ",WHITE,WHITE,FONT_SIZE_32,NULL);
			memset(keybuf, 0, 6);
			i = 0;
			wait_time = 0;
		}
		wait_time = 0;
		key_val = keyboard_input();	
		lcd_display_string(keylen * 32,100,(uint8_t *)"*",BLUE,WHITE,FONT_SIZE_32,NULL);
			keybuf[i++] = key_val;
			keylen ++;
		printf("key = %s\r\n",keybuf);
		if(keylen==6)
		{
			keylen=0;
			printf("key = %s\r\n",keybuf);
			printf("door_posswd = %s", door_passwd);
			if(0==strcmp((const char *)door_passwd,(const char *)keybuf))
			{
			
				//����
				printf("�����ɹ�\r\n");
				
				lcd_display_string(0 ,100, (uint8_t*)"           ",WHITE,WHITE,FONT_SIZE_32,NULL);
				lcd_display_string(0 ,100, (uint8_t*)"�����ɹ�",BLACK,WHITE,FONT_SIZE_32,NULL);
				memset(keybuf, 0, 6);
				i = 0;
			}
		
			else if(0==strcmp((const char *)keybuf,(const char *)ad_passwd))//�������Աģʽ����ɹ�
			{
				printf("%s\r\n",keybuf);
				printf("�������Աģʽ\r\n");
				memset(keybuf, 0, 6);
				i = 0;
			}
			else//�������
			{
				printf("�������\r\n");
				printf("��������� %s", keybuf);
				lcd_display_string(0 ,100, (uint8_t*)"           ",RED,WHITE,FONT_SIZE_32,NULL);
				lcd_display_string(0 ,100, (uint8_t*)"�������",RED,WHITE,FONT_SIZE_32,NULL);
				lcd_display_string(0 ,100, (uint8_t*)"           ",RED,WHITE,FONT_SIZE_32,NULL);
				memset(keybuf, 0, 6);
				i = 0;
				
				
			}
			}
//		vTaskDelay(500);
		}
		
}