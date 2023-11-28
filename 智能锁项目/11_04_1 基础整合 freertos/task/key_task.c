#include "ucos_task.h"

OS_STK KEY_TASK[KEY_STACK_LEN];


void KeyTask(void *pdata)
{
	pdata=pdata;//防止编译器优化
	
	
	
	printf("key task coming!!\r\n");
	while(1)
	{
		key2passwd();
		OSTimeDly(20);
	}
	
}

extern uint8_t door_passwd[];
extern uint8_t ad_passwd[];
extern uint8_t row;
extern uint8_t lcdabs;

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
		lcd_display_string(keylen * 32,100,(uint8_t *)"*",BLUE,WHITE,FONT_SIZE_24,NULL);
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
			
				//开锁
				printf("开锁成功\r\n");
				
				lcd_display_string(0 ,100, (uint8_t*)"           ",WHITE,WHITE,FONT_SIZE_32,NULL);
				lcd_display_string(0 ,100, (uint8_t*)"开锁成功",BLACK,WHITE,FONT_SIZE_32,NULL);
				memset(keybuf, 0, 6);
				i = 0;
			}
		
			else if(0==strcmp((const char *)keybuf,(const char *)ad_passwd))//输入管理员模式密码成功
			{
				printf("%s\r\n",keybuf);
				printf("进入管理员模式\r\n");
				OSTaskSuspend(LED_TASK_PRIO);
				OSTaskSuspend(KEY_TASK_PRIO);
				OSTaskSuspend(LCD_TASK_PRIO);
				OSTaskSuspend(ADM_TASK_PRIO);
				memset(keybuf, 0, 6);
				i = 0;
			}
			else//密码错误
			{
				printf("密码错误\r\n");
				printf("输入的密码 %s", keybuf);
				lcd_display_string(0 ,100, (uint8_t*)"           ",RED,WHITE,FONT_SIZE_32,NULL);
				memset(keybuf, 0, 6);
				i = 0;
				
				
			}
			}

		}
		
}