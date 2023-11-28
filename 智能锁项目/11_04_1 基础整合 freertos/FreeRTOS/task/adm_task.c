#include "myrtos.h"


extern TaskHandle_t StartTask_Handler;           		 //任务句柄
extern TaskHandle_t led_Handler;                    //任务句柄
extern TaskHandle_t finger_Handler;                    //任务句柄
extern TaskHandle_t time_Handler;                    //任务句柄
extern TaskHandle_t key_Handler;                    //任务句柄
extern TaskHandle_t adm_Handler;                    //任务句柄
extern	uint8_t card_num;
extern uint8_t card_id[100][5];
uint32_t card_num_addr = 1916928;
uint32_t card_addr_addr = 1925120;
uint32_t door_passwd_addr = 1929216;
uint32_t adm_passwd_addr = 1937408;



void AdmTask(void *pdata)
{

	
	printf("adm task coming!!\r\n");
	while(1)
	{
		adm_task();
		
	
	}
	
}

extern uint8_t door_passwd[];
extern uint8_t ad_passwd[];



void adm_task(void){
	printf("进入管理员任务\r\n");
	uint8_t key_val1 = 0, key_val2 = 0;
	LCD_Clear(WHITE);
	lcd_display_string(36,0,(uint8_t*)"管理员功能列表",RED,WHITE,FONT_SIZE_24,NULL);
	lcd_display_string(40,16*2,(uint8_t*)"修改管理员密码:请按1",BLACK,WHITE,FONT_SIZE_16,NULL);
	lcd_display_string(40,16*3,(uint8_t*)"设置开门密码  :请按2",BLACK,WHITE,FONT_SIZE_16,NULL);	
	lcd_display_string(40,16*4,(uint8_t*)"登记卡片      :请按3",BLACK,WHITE,FONT_SIZE_16,NULL);
	lcd_display_string(40,16*5,(uint8_t*)"删除指定门卡  :请按4",BLACK,WHITE,FONT_SIZE_16,NULL);	
	lcd_display_string(40,16*6,(uint8_t*)"登记指纹      :请按5",BLACK,WHITE,FONT_SIZE_16,NULL);	
	lcd_display_string(40,16*7,(uint8_t*)"删除指定指纹  :请按6",BLACK,WHITE,FONT_SIZE_16,NULL);
	lcd_display_string(40,16*8,(uint8_t*)"删除所有指纹  :请按7",BLACK,WHITE,FONT_SIZE_16,NULL);
	lcd_display_string(40,16*9,(uint8_t*)"设置音量      :请按8",BLACK,WHITE,FONT_SIZE_16,NULL);
	lcd_display_string(40,16*10,(uint8_t*)"恢复出厂设置  :请按9",BLACK,WHITE,FONT_SIZE_16,NULL);		
	lcd_display_string(40,16*12,(uint8_t*)"退出请按*  确认请按#",RED,WHITE,FONT_SIZE_16,NULL);
	
	while(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_1));
		key_val1 = BS8116ReadKey();
	label1:
		switch(key_val1){
			case '1': send_voi_cmd(Chang_Adm_Password); lcd_display_string(40,16*2,(uint8_t*)"修改管理员密码:请按1",BLUE,WHITE,FONT_SIZE_16,NULL);		break;
			case '2': send_voi_cmd(Set_door_password); lcd_display_string(40,16*3,(uint8_t*)"设置开门密码  :请按2",BLUE,WHITE,FONT_SIZE_16,NULL);	  break;
			case '3': send_voi_cmd(Registration_card); lcd_display_string(40,16*4,(uint8_t*)"登记卡片      :请按3",BLUE,WHITE,FONT_SIZE_16,NULL);   break;
			case '4': send_voi_cmd(Delete_specified_card); lcd_display_string(40,16*5,(uint8_t*)"删除指定门卡  :请按4",BLUE,WHITE,FONT_SIZE_16,NULL);	  break;
			case '5': send_voi_cmd(Register_fingerprint); lcd_display_string(40,16*6,(uint8_t*)"登记指纹      :请按5",BLUE,WHITE,FONT_SIZE_16,NULL);	  break;
			case '6': send_voi_cmd(Delete_specified_fingerprint); lcd_display_string(40,16*7,(uint8_t*)"删除指定指纹  :请按6",BLUE,WHITE,FONT_SIZE_16,NULL);   break;
			case '7': send_voi_cmd(Delete_all_fingerprints); lcd_display_string(40,16*8,(uint8_t*)"删除所有指纹  :请按7",BLUE,WHITE,FONT_SIZE_16,NULL);   break;
			case '8': send_voi_cmd(Set_volume); lcd_display_string(40,16*9,(uint8_t*)"设置音量      :请按8",BLUE,WHITE,FONT_SIZE_16,NULL);   break;
			case '9': send_voi_cmd(factory_data_reset); lcd_display_string(40,16*10,(uint8_t*)"恢复出厂设置  :请按9",BLUE,WHITE,FONT_SIZE_16,NULL);  break;
			case '#': send_voi_cmd(0X20); lcd_display_string(40,16*12,(uint8_t*)"退出请按*  确认请按#",BLUE,WHITE,FONT_SIZE_16,NULL);  break;
				
		}
		while(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_1));
		printf("当前键值 %c\r\n", key_val1);
		if((key_val2 = BS8116ReadKey()) == '#'){
			printf("进入选项\r\n");
			LCD_Clear(WHITE);
			printf("当前键值222 %c\r\n", key_val1);
			switch(key_val1){
				case '1':change_adm_passwd();break;
				case '2':change_door_passwd();break;
				case '3':registration_card();break;
				case '4':del_specified_card();break;
				case '5':register_fingerprint();break;
				case '6':del_specified_fingerprint();break;
				case '7':del_all_fingers();break;
				case '8':set_volume();break;
				case '9':factory_reset();
				case '#':
						vTaskResume(finger_Handler);
				    vTaskResume(time_Handler);
				    vTaskResume(key_Handler)  ;
						vTaskSuspend(adm_Handler);
			return;
			}
			
			
		}
		else{
			switch(key_val1){
				case '1':  lcd_display_string(40,16*2,(uint8_t*)"修改管理员密码:请按1",BLACK,WHITE,FONT_SIZE_16,NULL);		break;
				case '2':  lcd_display_string(40,16*3,(uint8_t*)"设置开门密码  :请按2",BLACK,WHITE,FONT_SIZE_16,NULL);	  break;
				case '3':  lcd_display_string(40,16*4,(uint8_t*)"登记卡片      :请按3",BLACK,WHITE,FONT_SIZE_16,NULL);   break;
				case '4':  lcd_display_string(40,16*5,(uint8_t*)"删除指定门卡  :请按4",BLACK,WHITE,FONT_SIZE_16,NULL);	  break;
				case '5':  lcd_display_string(40,16*6,(uint8_t*)"登记指纹      :请按5",BLACK,WHITE,FONT_SIZE_16,NULL);	  break;
				case '6':  lcd_display_string(40,16*7,(uint8_t*)"删除指定指纹  :请按6",BLACK,WHITE,FONT_SIZE_16,NULL);   break;
				case '7':  lcd_display_string(40,16*8,(uint8_t*)"删除所有指纹  :请按7",BLACK,WHITE,FONT_SIZE_16,NULL);   break;
				case '8':  lcd_display_string(40,16*9,(uint8_t*)"设置音量      :请按8",BLACK,WHITE,FONT_SIZE_16,NULL);   break;
				case '9':  lcd_display_string(40,16*10,(uint8_t*)"恢复出厂设置  :请按9",BLACK,WHITE,FONT_SIZE_16,NULL);  break;
				case '#':  lcd_display_string(40,16*12,(uint8_t*)"退出请按*  确认请按#",BLACK,WHITE,FONT_SIZE_16,NULL);  break;
				
		}
			key_val1 = key_val2;
			goto label1;
		}
		
	
}

extern uint8_t lcdabs;
extern uint8_t row;
void change_adm_passwd(void){
	lcdabs = 0;
	row = 90;
	char data[512] = {0};
	uint8_t del_flag = 0;
	uint8_t new_adm_passwd1[6] = {0}, new_adm_passwd2[6] = {0};
	uint8_t key_val = 0, i = 0;
	printf("开始修改管理员密码\r\n");
	send_voi_cmd(new_password);
	lcd_display_string(0,0,(uint8_t*)"请输入新的密码",RED,WHITE,FONT_SIZE_24,NULL);
	new_passwd1:
	lcdabs = lcdabs + 16;
	del_passwd1:
	while(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_1));
		key_val = keyboard_input();
		
		if(key_val == '#'){		//删除
			memset(new_adm_passwd1, 0, 6);
			memset(new_adm_passwd2, 0, 6);
			i = 0;
			return;
		}
	
		if(key_val == '*'){		//删除
			lcdabs = lcdabs - 16;
			lcd_display_string(lcdabs + 16 * 6,90,(uint8_t *)" ",WHITE,WHITE,FONT_SIZE_24,NULL);
			printf("删除lcdabs = %d\r\n", lcdabs);
			new_adm_passwd1[i] = 0;
			i--;
//			del_flag++;
			Delay_ms(200);
			goto del_passwd1;
		}	
		else{
			
//			del_flag = 0;
			lcd_display_string(lcdabs + 16 * 6,90,(uint8_t *)"*",BLUE,WHITE,FONT_SIZE_24,NULL);
			new_adm_passwd1[i] = key_val;
			i++;
			if((i < 6)){
				
//			printf("lcdabs = %d\r\n", lcdabs);
			
			goto new_passwd1;
			}
			
		}
	printf("新密码1 = %s\r\n", new_adm_passwd1);
	i = 0;
	lcdabs = 10;
	LCD_Clear(WHITE);
	send_voi_cmd(new_password_again);
	lcd_display_string(0,0,(uint8_t*)"请再次输入新的密码",RED,WHITE,FONT_SIZE_24,NULL);
	new_passwd2:
		lcdabs = lcdabs + 16;
	del_passwd2:
	while(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_1));
		key_val = keyboard_input();
		
		if(key_val == '#'){		//删除
			memset(new_adm_passwd1, 0, 6);
			memset(new_adm_passwd2, 0, 6);
			i = 0;
			return;
		}
		
	
		
		if(key_val == '*'){		//删除
			lcd_display_string(lcdabs + 16 * 6,90,(uint8_t *)" ",WHITE,WHITE,FONT_SIZE_24,NULL);
			printf("lcdabs = %d\r\n", lcdabs);
			new_adm_passwd2[i - 1] = 0;
			i--;
			goto new_passwd2;
		}	
		else{
						lcd_display_string(lcdabs + 16 * 6,90,(uint8_t *)"*",BLUE,WHITE,FONT_SIZE_24,NULL);
			new_adm_passwd2[i] = key_val;
			i++;
			if((i < 6)){
				
//			printf("lcdabs = %d\r\n", lcdabs);
			
			goto new_passwd2;
			}
			
		}
	printf("新密码2 = %s\r\n", new_adm_passwd1);
	
	if(0==strcmp((const char *)new_adm_passwd2,(const char *)new_adm_passwd1))
			{
			
				//开锁
				printf("管理员密码设置成功\r\n");
				send_voi_cmd(0X11);
				LCD_Clear(WHITE);
				lcd_display_string(0 ,100, (uint8_t*)"管理员密码设置成功",BLACK,WHITE,FONT_SIZE_24,NULL);
				strcpy((char *)ad_passwd, (char *)new_adm_passwd2);
				Delay_ms(3000);
				i = 0;
				
				//更新至w25q64
				sFLASH_EraseSector(adm_passwd_addr);
				sFLASH_WriteBuffer(new_adm_passwd2, adm_passwd_addr, 6);
				
			
			//上传阿里云
				printf("新密码 %s\r\n", new_adm_passwd2);
				printf("新密码 %s\r\n", new_adm_passwd1);
			sprintf(data,"{\"method\":\"thing.service.property.set\",\"id\":\"874187529\",\"params\":{\"AdmPwd\":\"%s\"},\"version\":\"1.0.0\"}", new_adm_passwd1);
			
			printf("data数据 %s\r\n", data);
			send2server(data);
			}
			else{
				send_voi_cmd(0X10);
				i = 0;
			}
			
	
}


//设置新的开门密码
void change_door_passwd(void){
	row = 90;
	lcdabs = 0;
	uint8_t del_flag = 0;
	char data[128] = {0};
	uint8_t new_door_passwd1[6] = {0}, new_door_passwd2[6] = {0};
	uint8_t key_val = 0, i = 0;
	printf("开始修改开门密码\r\n");
	lcd_display_string(36,0,(uint8_t*)"请输入新的密码",RED,WHITE,FONT_SIZE_24,NULL);
	send_voi_cmd(new_password);
	new_passwd1:
	lcdabs = lcdabs + 16;
	del_passwd1:
	while(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_1));
		key_val = keyboard_input();
		
		if(key_val == '#'){		//删除
			memset(new_door_passwd1, 0, 6);
			memset(new_door_passwd2, 0, 6);
			i = 0;
			return;
		}
	
		if(key_val == '*'){		//删除
			lcdabs = lcdabs - 16;
			lcd_display_string(lcdabs + 16 * 6,90,(uint8_t *)" ",WHITE,WHITE,FONT_SIZE_24,NULL);
			printf("删除lcdabs = %d\r\n", lcdabs);
			new_door_passwd1[i] = 0;
			i--;
//			del_flag++;
			Delay_ms(200);
			goto del_passwd1;
		}	
		else{
			
						lcd_display_string(lcdabs + 16 * 6,90,(uint8_t *)"*",BLUE,WHITE,FONT_SIZE_24,NULL);
			new_door_passwd1[i] = key_val;
			i++;
			if((i < 6)){
				
//			printf("lcdabs = %d\r\n", lcdabs);
			
			goto new_passwd1;
			}
			
		}
	i = 0;
	lcdabs = 10;
	LCD_Clear(WHITE);
	lcd_display_string(36,0,(uint8_t*)"请再次输入新的密码",RED,WHITE,FONT_SIZE_24,NULL);
		send_voi_cmd(new_password_again);
		
	new_passwd2:
		lcdabs = lcdabs + 16;
	del_passwd2:
	while(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_1));
		key_val = keyboard_input();
		
		if(key_val == '#'){		//删除
			memset(new_door_passwd1, 0, 6);
			memset(new_door_passwd2, 0, 6);
			i = 0;
			return;
		}
		
//		if(key_val == '*'){		//删除
//			lcdabs = lcdabs - 16;
//			lcd_display_string(lcdabs + 16 * 5,90,(uint8_t *)" ",WHITE,WHITE,FONT_SIZE_24,NULL);
//			printf("删除lcdabs = %d\r\n", lcdabs);
//			new_door_passwd1[i] = 0;
//			i--;
//		//			del_flag++;
//			Delay_ms(200);
//			goto del_passwd2;
//		}	
		
		if(key_val == '*'){		//删除
			lcd_display_string(lcdabs + 16 * 6,90,(uint8_t *)" ",WHITE,WHITE,FONT_SIZE_24,NULL);
			printf("lcdabs = %d\r\n", lcdabs);
			new_door_passwd2[i - 1] = 0;
			i--;
			goto new_passwd2;
		}	
		else{
			
						lcd_display_string(lcdabs + 16 * 6,90,(uint8_t *)"*",BLUE,WHITE,FONT_SIZE_24,NULL);
			new_door_passwd2[i] = key_val;
			i++;
			if((i < 6)){
				
//			printf("lcdabs = %d\r\n", lcdabs);
			
			goto new_passwd2;
			}
			
		}
	
	if(0==strcmp((const char *)new_door_passwd2,(const char *)new_door_passwd1))
			{
			
				//开锁
				printf("开门密码设置成功\r\n");
				send_voi_cmd(0X11);
				LCD_Clear(WHITE);
				lcd_display_string(0 ,100, (uint8_t*)"开门密码设置成功",BLACK,WHITE,FONT_SIZE_32,NULL);
				strcpy(door_passwd, new_door_passwd2);
				Delay_ms(3000);
				i = 0;
				//更新至w25q64
				sFLASH_EraseSector(door_passwd_addr);
				sFLASH_WriteBuffer(new_door_passwd2, door_passwd_addr, 6);
				sprintf(data,"{\"method\":\"thing.service.property.set\",\"id\":\"874187529\",\"params\":{\"DoorPwd\":\"%s\"},\"version\":\"1.0.0\"}", new_door_passwd2);
			
			printf("data数据 %s\r\n", data);
			send2server(data);
			}
			else{
				send_voi_cmd(0X10);
				i = 0;
			}
				
				
			
			//跟新w25q64
			
			//上传阿里云
}


//w25q64写入卡id
void w25_card_write(char *cardid){
	uint32_t card_addr = card_addr_addr;
	printf("\r\n1111111\r\n");
	sFLASH_ReadBuffer(&card_num, card_num_addr, 1);
	if(card_num == 255){
		card_num = 0;
	}
	printf("card_num = %d\r\n", card_num);
	for(uint8_t i = 0; i < card_num; i++){
		Delay_ms(10);
		printf("%d      ", i);
		sFLASH_ReadBuffer(card_id[i], card_addr, 5);
		Delay_ms(10);
		print_info(card_id[i], 4);
		Delay_ms(10);
		card_addr = card_addr + 5;
	}
	printf("\r\n3333333333\r\n");
	if(cardid != NULL){
		sFLASH_EraseSector(card_num_addr);
		sFLASH_EraseSector(card_addr_addr);
		
		strcpy((char *)card_id[card_num], cardid);
		card_num++;
		card_addr = card_addr_addr;
		for(uint8_t i = 0; i < card_num; i++){
			Delay_ms(10);
			printf("%d      ", i);
			print_info(card_id[i], 4);
			sFLASH_WriteBuffer(card_id[i], card_addr, 5);
			card_addr = card_addr + 5;
		}
		sFLASH_WriteBuffer(&card_num, card_num_addr, 1);
	}
	
	
}




//登记卡片
void registration_card(void){
	char data[128] = {0};
	printf("开始登记卡片\r\n");
	send_voi_cmd(0X13);
	while(Identificate_Card(CardType,CardId));
	printf("card id ");
	print_info(CardId, 4);
	w25_card_write((char *)CardId);
	sprintf(data,"{\"method\":\"thing.service.property.set\",\"id\":\"874187529\",\"params\":{\"NewCard\":\"%d%d%d%d\"},\"version\":\"1.0.0\"}", CardId[0], CardId[1], CardId[2], CardId[3]);
	printf("data数据 %s\r\n", data);
	send2server(data);
	sprintf(data,"{\"method\":\"thing.service.property.set\",\"id\":\"874187529\",\"params\":{\"CardSum\":%d},\"version\":\"1.0.0\"}", card_num);
	printf("data数据 %s\r\n", data);
	send2server(data);
	send_voi_cmd(0x11);
}

//删除指定卡片
void del_specified_card(void){
	uint32_t card_addr;
	char data[128] = {0};
	printf("开始删除指定卡片");
	send_voi_cmd(0X13);
	while(Identificate_Card(CardType,CardId));
	printf("card num = %d\r\n", card_num);
	for(uint8_t i = 0; i < card_num; i++){
		if(!strcmp((const char *)CardId, (const char *)card_id[i])){
			printf("要删除的卡 %d\r\n", i);
			for(uint8_t j = i; j < card_num; j++){
				strcpy((char *)card_id[j], (char *)card_id[j + 1]);
			}
			card_num--;
		}
	}
	if(!strcmp((const char *)CardId, (const char *)card_id[card_num - 1])){
		card_num--;
	}
		sFLASH_EraseSector(card_num_addr);
		sFLASH_EraseSector(card_addr_addr);
		
	
		card_addr = card_addr_addr;
		for(uint8_t i = 0; i < card_num; i++){
			printf("%d", i);
			print_info(card_id[i], 4);
			sFLASH_WriteBuffer(card_id[i], card_addr, 5);
			card_addr = card_addr + 5;
		}
		sFLASH_WriteBuffer(&card_num, card_num_addr, 1);
		sprintf(data,"{\"method\":\"thing.service.property.set\",\"id\":\"874187529\",\"params\":{\"DelCard\":\"%d%d%d%d\"},\"version\":\"1.0.0\"}", CardId[0], CardId[1], CardId[2], CardId[3]);
		printf("data数据 %s\r\n", data);
		send2server(data);
		sprintf(data,"{\"method\":\"thing.service.property.set\",\"id\":\"874187529\",\"params\":{\"CardSum\":%d},\"version\":\"1.0.0\"}", card_num);
		printf("data数据 %s\r\n", data);
		send2server(data);
		send_voi_cmd(0x11);
}


//登记指纹
void register_fingerprint(void){
	char data[128] = {0};
	LCD_Clear(WHITE);
	lcd_display_string(0 ,0, (uint8_t*)"指纹采集 #退出",BLACK,WHITE,FONT_SIZE_24,NULL);
	if(Enroll() == 0){
//		send_voi_cmd(0X11);
		sprintf(data,"{\"method\":\"thing.service.property.set\",\"id\":\"874187529\",\"params\":{\"FingerprintNum\":%d},\"version\":\"1.0.0\"}", get_finger_num());
			
		printf("data数据 %s\r\n", data);
		send2server(data);
	}
	Delay_ms(3000);
}


//删除指定指纹
void del_specified_fingerprint(void){
	uint8_t fingerprint_num[64] = {0}, fingerprint_id;
	char data[128] = {0};
	sprintf((char *)fingerprint_num, "当前指纹数 %d", get_finger_num());
	lcd_display_string(0 ,0, (uint8_t*)"请放手指 #退出",BLACK,WHITE,FONT_SIZE_24,NULL);
	lcd_display_string(0 ,60, (uint8_t*)fingerprint_num,BLACK,WHITE,FONT_SIZE_24,NULL);
	fig:
	fingerprint_id = match_fingerprints();
	if(fingerprint_id == 0){
		lcd_display_string(0 ,90, (uint8_t*)"                    ",BLACK,WHITE,FONT_SIZE_24,NULL);
		lcd_display_string(0 ,90, (uint8_t*)"指纹匹配失败，请重试",BLACK,WHITE,FONT_SIZE_24,NULL);
		goto fig;
	}
	else if(fingerprint_id == '#'){
		return;
	}
	del_fingerprints(fingerprint_id);
	
	lcd_display_string(0 ,90, (uint8_t*)"                    ",BLACK,WHITE,FONT_SIZE_24,NULL);
	lcd_display_string(0 ,90, (uint8_t*)"删除成功",BLACK,WHITE,FONT_SIZE_24,NULL);
	send_voi_cmd(0x11);
	//上传阿里云
	sprintf(data,"{\"method\":\"thing.service.property.set\",\"id\":\"874187529\",\"params\":{\"FingerprintNum\":%d},\"version\":\"1.0.0\"}", get_finger_num());
			
	printf("data数据 %s\r\n", data);
	send2server(data);
	Delay_ms(3000);
	
}

//删除所有指纹

void del_all_fingers(void){
	uint8_t fingerprint_num[64] = {0},key_val = 0;
	char data[128] = {0};
	sprintf((char *)fingerprint_num, "当前指纹数 %d", get_finger_num());
	lcd_display_string(0 ,0, (uint8_t*)"请放手指 #退出 *确定",BLACK,WHITE,FONT_SIZE_24,NULL);
	lcd_display_string(0 ,60, (uint8_t*)fingerprint_num,BLACK,WHITE,FONT_SIZE_24,NULL);
	while(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_1));
	key_val = BS8116ReadKey();
	if(key_val == '#'){
		lcd_display_string(0 ,90, (uint8_t*)"                    ",BLACK,WHITE,FONT_SIZE_24,NULL);
		lcd_display_string(0 ,90, (uint8_t*)"操作取消",BLACK,WHITE,FONT_SIZE_24,NULL);
		Delay_ms(3000);
		return;
	}
	else if(key_val == '*'){
		del_all_fingerprints();

		lcd_display_string(0 ,90, (uint8_t*)"                    ",BLACK,WHITE,FONT_SIZE_24,NULL);
		lcd_display_string(0 ,90, (uint8_t*)"删除成功",BLACK,WHITE,FONT_SIZE_24,NULL);
		Delay_ms(3000);
		send_voi_cmd(0x11);
	}
	//上传阿里云
	sprintf(data,"{\"method\":\"thing.service.property.set\",\"id\":\"874187529\",\"params\":{\"FingerprintNum\":%d},\"version\":\"1.0.0\"}", get_finger_num());
			
	printf("data数据 %s\r\n", data);
	send2server(data);
	
}

//设置音量
uint8_t default_vol = 8;
extern uint8_t row;
void set_volume(void){
	uint8_t key_val = 0, x = 0, y = 0;
	char data[128] = {0};
	uint8_t vol_buf[64] = {0}, volflag = 0;
	row = 253;
	LCD_Clear(WHITE);
	lcd_display_string(0 ,0, (uint8_t*)"音量 #退出",BLACK,WHITE,FONT_SIZE_24,NULL);
	lcd_display_string(0 ,30, (uint8_t*)"音量+ 请按5",BLACK,WHITE,FONT_SIZE_24,NULL);
	lcd_display_string(0 ,60, (uint8_t*)"音量- 请按8",BLACK,WHITE,FONT_SIZE_24,NULL);
	sprintf((char *)vol_buf, "当前音量 %d", default_vol);
	lcd_display_string(0 ,90, (uint8_t*)vol_buf,BLACK,WHITE,FONT_SIZE_24,NULL);
	for(int i = 0; i < default_vol; i++){
		LCD_Fill(25 + x * 25,200 - y * 15 - 15, 50 + x * 25, 200, BLACK);
		Delay_ms(100);
		x++;
		y++;
		printf("绘制次数 %d\r\n", i + 1);
	}
	printf("绘制完成\r\n");
	volflag = 3;
	vol_label:
	
	while(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_1));
	key_val = BS8116ReadKey();
	
	if(key_val == '5'){
		if(default_vol == 8){
			default_vol = 8;
			printf("default升为8-------\r\n");
			while(!GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_1));
			goto vol_label;
		}
		
		LCD_Fill(25 + default_vol * 25 - 25,200 - default_vol * 15 - 15, 50 + default_vol * 25 - 25, 200, BLACK);
		default_vol++;
		while(GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_4));
		printf("default_vol = %d\r\n", default_vol);
		sprintf((char *)vol_buf, "当前音量 %d", default_vol);
		lcd_display_string(0 ,90, (uint8_t*)"          ",BLACK,WHITE,FONT_SIZE_24,NULL);
		lcd_display_string(0 ,90, (uint8_t*)vol_buf,BLACK,WHITE,FONT_SIZE_24,NULL);
		send_voi_cmd(0X28 - default_vol);
		while(GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_4));
		send_voi_cmd(0X1d);
		Delay_ms(100);
		while(!GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_1));
		
		goto vol_label;
	}
	if(key_val == '8'){
		if(default_vol == 0){
			default_vol = 0;
			printf("default减为0-------\r\n");
			while(!GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_1));
			goto vol_label;
		}
		
//		LCD_Fill(25 + default_vol * 25 - 25,200 - default_vol * 15 - 15, 50 + default_vol * 25 - 25, 200, WHITE);
		LCD_Fill(25 + default_vol * 25 - 25,200 - default_vol * 15 - 15, 50 + default_vol * 25 - 25, 200, WHITE);
		default_vol--;
		while(GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_4));
		send_voi_cmd(0X28 - default_vol);
		while(GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_4));
		printf("default_vol = %d\r\n", default_vol);
		sprintf((char *)vol_buf, "当前音量 %d", default_vol);
		lcd_display_string(0 ,90, (uint8_t*)"          ",BLACK,WHITE,FONT_SIZE_24,NULL);
		lcd_display_string(0 ,90, (uint8_t*)vol_buf,BLACK,WHITE,FONT_SIZE_24,NULL);
		send_voi_cmd(0X1d);
		Delay_ms(100);
		
		goto vol_label;
	}
	if(key_val == '#'){
		send_voi_cmd(0x11);
		sprintf(data,"{\"method\":\"thing.service.property.set\",\"id\":\"874187529\",\"params\":{\"VoicePwr\":%d},\"version\":\"1.0.0\"}", default_vol - 1);
			
		printf("data数据 %s\r\n", data);
		send2server(data);
		return;
	}
//	
//	
//	
//	Delay_ms(3000);
//	LCD_Fill(0, 100, 25, 200, WHITE);
	while(1);
	
}


//恢复出场设置
void factory_reset(void){
	uint8_t key_val = 0, data[512] = {0};
	LCD_Clear(WHITE);
	lcd_display_string(0 ,0, (uint8_t*)"恢复出场设置",BLACK,WHITE,FONT_SIZE_24,NULL);
	lcd_display_string(0 ,30, (uint8_t*)"* 确认  #退出",BLACK,WHITE,FONT_SIZE_24,NULL);
	while(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_1));
	key_val = BS8116ReadKey();
	
	if(key_val == '*'){
		LCD_Clear(WHITE);
		lcd_display_string(0 ,30, (uint8_t*)"                ",WHITE,WHITE,FONT_SIZE_24,NULL);
		lcd_display_string(0 ,30, (uint8_t*)"正在恢复出场设置",BLACK,WHITE,FONT_SIZE_24,NULL);
		memset(door_passwd, 0, 6);
		memset(ad_passwd, 0, 6);
		sprintf((char *)door_passwd, "%s", "666666");
		sprintf((char *)ad_passwd, "%s", "888888");
		//写入w25q64
		sFLASH_EraseSector(door_passwd_addr);
		sFLASH_WriteBuffer(door_passwd, door_passwd_addr, 6);
		sFLASH_EraseSector(adm_passwd_addr);
		sFLASH_WriteBuffer(ad_passwd, adm_passwd_addr, 6);
		del_all_fingerprints();
		sFLASH_EraseSector(card_num_addr);
		sFLASH_EraseSector(card_addr_addr);
		card_num = 0;
		sFLASH_WriteBuffer(&card_num, card_num_addr, 1);
		wifi_write("moumouyao", "12345678");
		default_vol = 8;
		send_voi_cmd(0X20);
		
		lcd_display_string(0 ,30, (uint8_t*)"                ",WHITE,WHITE,FONT_SIZE_24,NULL);
		lcd_display_string(0 ,30, (uint8_t*)"恢复出场设置成功",BLACK,WHITE,FONT_SIZE_24,NULL);
		send_voi_cmd(0x11);
		Delay_ms(3000);
		LCD_Clear(WHITE);
		//上床阿里云
			
		printf("data数据 %s\r\n", property_payload);
		send2server(property_payload);
		
		
	}
	if(key_val == '#'){
		LCD_Clear(WHITE);
		return;
	}
	
}























