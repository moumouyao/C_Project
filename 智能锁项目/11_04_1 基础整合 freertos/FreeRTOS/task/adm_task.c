#include "myrtos.h"


extern TaskHandle_t StartTask_Handler;           		 //������
extern TaskHandle_t led_Handler;                    //������
extern TaskHandle_t finger_Handler;                    //������
extern TaskHandle_t time_Handler;                    //������
extern TaskHandle_t key_Handler;                    //������
extern TaskHandle_t adm_Handler;                    //������
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
	printf("�������Ա����\r\n");
	uint8_t key_val1 = 0, key_val2 = 0;
	LCD_Clear(WHITE);
	lcd_display_string(36,0,(uint8_t*)"����Ա�����б�",RED,WHITE,FONT_SIZE_24,NULL);
	lcd_display_string(40,16*2,(uint8_t*)"�޸Ĺ���Ա����:�밴1",BLACK,WHITE,FONT_SIZE_16,NULL);
	lcd_display_string(40,16*3,(uint8_t*)"���ÿ�������  :�밴2",BLACK,WHITE,FONT_SIZE_16,NULL);	
	lcd_display_string(40,16*4,(uint8_t*)"�Ǽǿ�Ƭ      :�밴3",BLACK,WHITE,FONT_SIZE_16,NULL);
	lcd_display_string(40,16*5,(uint8_t*)"ɾ��ָ���ſ�  :�밴4",BLACK,WHITE,FONT_SIZE_16,NULL);	
	lcd_display_string(40,16*6,(uint8_t*)"�Ǽ�ָ��      :�밴5",BLACK,WHITE,FONT_SIZE_16,NULL);	
	lcd_display_string(40,16*7,(uint8_t*)"ɾ��ָ��ָ��  :�밴6",BLACK,WHITE,FONT_SIZE_16,NULL);
	lcd_display_string(40,16*8,(uint8_t*)"ɾ������ָ��  :�밴7",BLACK,WHITE,FONT_SIZE_16,NULL);
	lcd_display_string(40,16*9,(uint8_t*)"��������      :�밴8",BLACK,WHITE,FONT_SIZE_16,NULL);
	lcd_display_string(40,16*10,(uint8_t*)"�ָ���������  :�밴9",BLACK,WHITE,FONT_SIZE_16,NULL);		
	lcd_display_string(40,16*12,(uint8_t*)"�˳��밴*  ȷ���밴#",RED,WHITE,FONT_SIZE_16,NULL);
	
	while(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_1));
		key_val1 = BS8116ReadKey();
	label1:
		switch(key_val1){
			case '1': send_voi_cmd(Chang_Adm_Password); lcd_display_string(40,16*2,(uint8_t*)"�޸Ĺ���Ա����:�밴1",BLUE,WHITE,FONT_SIZE_16,NULL);		break;
			case '2': send_voi_cmd(Set_door_password); lcd_display_string(40,16*3,(uint8_t*)"���ÿ�������  :�밴2",BLUE,WHITE,FONT_SIZE_16,NULL);	  break;
			case '3': send_voi_cmd(Registration_card); lcd_display_string(40,16*4,(uint8_t*)"�Ǽǿ�Ƭ      :�밴3",BLUE,WHITE,FONT_SIZE_16,NULL);   break;
			case '4': send_voi_cmd(Delete_specified_card); lcd_display_string(40,16*5,(uint8_t*)"ɾ��ָ���ſ�  :�밴4",BLUE,WHITE,FONT_SIZE_16,NULL);	  break;
			case '5': send_voi_cmd(Register_fingerprint); lcd_display_string(40,16*6,(uint8_t*)"�Ǽ�ָ��      :�밴5",BLUE,WHITE,FONT_SIZE_16,NULL);	  break;
			case '6': send_voi_cmd(Delete_specified_fingerprint); lcd_display_string(40,16*7,(uint8_t*)"ɾ��ָ��ָ��  :�밴6",BLUE,WHITE,FONT_SIZE_16,NULL);   break;
			case '7': send_voi_cmd(Delete_all_fingerprints); lcd_display_string(40,16*8,(uint8_t*)"ɾ������ָ��  :�밴7",BLUE,WHITE,FONT_SIZE_16,NULL);   break;
			case '8': send_voi_cmd(Set_volume); lcd_display_string(40,16*9,(uint8_t*)"��������      :�밴8",BLUE,WHITE,FONT_SIZE_16,NULL);   break;
			case '9': send_voi_cmd(factory_data_reset); lcd_display_string(40,16*10,(uint8_t*)"�ָ���������  :�밴9",BLUE,WHITE,FONT_SIZE_16,NULL);  break;
			case '#': send_voi_cmd(0X20); lcd_display_string(40,16*12,(uint8_t*)"�˳��밴*  ȷ���밴#",BLUE,WHITE,FONT_SIZE_16,NULL);  break;
				
		}
		while(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_1));
		printf("��ǰ��ֵ %c\r\n", key_val1);
		if((key_val2 = BS8116ReadKey()) == '#'){
			printf("����ѡ��\r\n");
			LCD_Clear(WHITE);
			printf("��ǰ��ֵ222 %c\r\n", key_val1);
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
				case '1':  lcd_display_string(40,16*2,(uint8_t*)"�޸Ĺ���Ա����:�밴1",BLACK,WHITE,FONT_SIZE_16,NULL);		break;
				case '2':  lcd_display_string(40,16*3,(uint8_t*)"���ÿ�������  :�밴2",BLACK,WHITE,FONT_SIZE_16,NULL);	  break;
				case '3':  lcd_display_string(40,16*4,(uint8_t*)"�Ǽǿ�Ƭ      :�밴3",BLACK,WHITE,FONT_SIZE_16,NULL);   break;
				case '4':  lcd_display_string(40,16*5,(uint8_t*)"ɾ��ָ���ſ�  :�밴4",BLACK,WHITE,FONT_SIZE_16,NULL);	  break;
				case '5':  lcd_display_string(40,16*6,(uint8_t*)"�Ǽ�ָ��      :�밴5",BLACK,WHITE,FONT_SIZE_16,NULL);	  break;
				case '6':  lcd_display_string(40,16*7,(uint8_t*)"ɾ��ָ��ָ��  :�밴6",BLACK,WHITE,FONT_SIZE_16,NULL);   break;
				case '7':  lcd_display_string(40,16*8,(uint8_t*)"ɾ������ָ��  :�밴7",BLACK,WHITE,FONT_SIZE_16,NULL);   break;
				case '8':  lcd_display_string(40,16*9,(uint8_t*)"��������      :�밴8",BLACK,WHITE,FONT_SIZE_16,NULL);   break;
				case '9':  lcd_display_string(40,16*10,(uint8_t*)"�ָ���������  :�밴9",BLACK,WHITE,FONT_SIZE_16,NULL);  break;
				case '#':  lcd_display_string(40,16*12,(uint8_t*)"�˳��밴*  ȷ���밴#",BLACK,WHITE,FONT_SIZE_16,NULL);  break;
				
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
	printf("��ʼ�޸Ĺ���Ա����\r\n");
	send_voi_cmd(new_password);
	lcd_display_string(0,0,(uint8_t*)"�������µ�����",RED,WHITE,FONT_SIZE_24,NULL);
	new_passwd1:
	lcdabs = lcdabs + 16;
	del_passwd1:
	while(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_1));
		key_val = keyboard_input();
		
		if(key_val == '#'){		//ɾ��
			memset(new_adm_passwd1, 0, 6);
			memset(new_adm_passwd2, 0, 6);
			i = 0;
			return;
		}
	
		if(key_val == '*'){		//ɾ��
			lcdabs = lcdabs - 16;
			lcd_display_string(lcdabs + 16 * 6,90,(uint8_t *)" ",WHITE,WHITE,FONT_SIZE_24,NULL);
			printf("ɾ��lcdabs = %d\r\n", lcdabs);
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
	printf("������1 = %s\r\n", new_adm_passwd1);
	i = 0;
	lcdabs = 10;
	LCD_Clear(WHITE);
	send_voi_cmd(new_password_again);
	lcd_display_string(0,0,(uint8_t*)"���ٴ������µ�����",RED,WHITE,FONT_SIZE_24,NULL);
	new_passwd2:
		lcdabs = lcdabs + 16;
	del_passwd2:
	while(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_1));
		key_val = keyboard_input();
		
		if(key_val == '#'){		//ɾ��
			memset(new_adm_passwd1, 0, 6);
			memset(new_adm_passwd2, 0, 6);
			i = 0;
			return;
		}
		
	
		
		if(key_val == '*'){		//ɾ��
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
	printf("������2 = %s\r\n", new_adm_passwd1);
	
	if(0==strcmp((const char *)new_adm_passwd2,(const char *)new_adm_passwd1))
			{
			
				//����
				printf("����Ա�������óɹ�\r\n");
				send_voi_cmd(0X11);
				LCD_Clear(WHITE);
				lcd_display_string(0 ,100, (uint8_t*)"����Ա�������óɹ�",BLACK,WHITE,FONT_SIZE_24,NULL);
				strcpy((char *)ad_passwd, (char *)new_adm_passwd2);
				Delay_ms(3000);
				i = 0;
				
				//������w25q64
				sFLASH_EraseSector(adm_passwd_addr);
				sFLASH_WriteBuffer(new_adm_passwd2, adm_passwd_addr, 6);
				
			
			//�ϴ�������
				printf("������ %s\r\n", new_adm_passwd2);
				printf("������ %s\r\n", new_adm_passwd1);
			sprintf(data,"{\"method\":\"thing.service.property.set\",\"id\":\"874187529\",\"params\":{\"AdmPwd\":\"%s\"},\"version\":\"1.0.0\"}", new_adm_passwd1);
			
			printf("data���� %s\r\n", data);
			send2server(data);
			}
			else{
				send_voi_cmd(0X10);
				i = 0;
			}
			
	
}


//�����µĿ�������
void change_door_passwd(void){
	row = 90;
	lcdabs = 0;
	uint8_t del_flag = 0;
	char data[128] = {0};
	uint8_t new_door_passwd1[6] = {0}, new_door_passwd2[6] = {0};
	uint8_t key_val = 0, i = 0;
	printf("��ʼ�޸Ŀ�������\r\n");
	lcd_display_string(36,0,(uint8_t*)"�������µ�����",RED,WHITE,FONT_SIZE_24,NULL);
	send_voi_cmd(new_password);
	new_passwd1:
	lcdabs = lcdabs + 16;
	del_passwd1:
	while(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_1));
		key_val = keyboard_input();
		
		if(key_val == '#'){		//ɾ��
			memset(new_door_passwd1, 0, 6);
			memset(new_door_passwd2, 0, 6);
			i = 0;
			return;
		}
	
		if(key_val == '*'){		//ɾ��
			lcdabs = lcdabs - 16;
			lcd_display_string(lcdabs + 16 * 6,90,(uint8_t *)" ",WHITE,WHITE,FONT_SIZE_24,NULL);
			printf("ɾ��lcdabs = %d\r\n", lcdabs);
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
	lcd_display_string(36,0,(uint8_t*)"���ٴ������µ�����",RED,WHITE,FONT_SIZE_24,NULL);
		send_voi_cmd(new_password_again);
		
	new_passwd2:
		lcdabs = lcdabs + 16;
	del_passwd2:
	while(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_1));
		key_val = keyboard_input();
		
		if(key_val == '#'){		//ɾ��
			memset(new_door_passwd1, 0, 6);
			memset(new_door_passwd2, 0, 6);
			i = 0;
			return;
		}
		
//		if(key_val == '*'){		//ɾ��
//			lcdabs = lcdabs - 16;
//			lcd_display_string(lcdabs + 16 * 5,90,(uint8_t *)" ",WHITE,WHITE,FONT_SIZE_24,NULL);
//			printf("ɾ��lcdabs = %d\r\n", lcdabs);
//			new_door_passwd1[i] = 0;
//			i--;
//		//			del_flag++;
//			Delay_ms(200);
//			goto del_passwd2;
//		}	
		
		if(key_val == '*'){		//ɾ��
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
			
				//����
				printf("�����������óɹ�\r\n");
				send_voi_cmd(0X11);
				LCD_Clear(WHITE);
				lcd_display_string(0 ,100, (uint8_t*)"�����������óɹ�",BLACK,WHITE,FONT_SIZE_32,NULL);
				strcpy(door_passwd, new_door_passwd2);
				Delay_ms(3000);
				i = 0;
				//������w25q64
				sFLASH_EraseSector(door_passwd_addr);
				sFLASH_WriteBuffer(new_door_passwd2, door_passwd_addr, 6);
				sprintf(data,"{\"method\":\"thing.service.property.set\",\"id\":\"874187529\",\"params\":{\"DoorPwd\":\"%s\"},\"version\":\"1.0.0\"}", new_door_passwd2);
			
			printf("data���� %s\r\n", data);
			send2server(data);
			}
			else{
				send_voi_cmd(0X10);
				i = 0;
			}
				
				
			
			//����w25q64
			
			//�ϴ�������
}


//w25q64д�뿨id
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




//�Ǽǿ�Ƭ
void registration_card(void){
	char data[128] = {0};
	printf("��ʼ�Ǽǿ�Ƭ\r\n");
	send_voi_cmd(0X13);
	while(Identificate_Card(CardType,CardId));
	printf("card id ");
	print_info(CardId, 4);
	w25_card_write((char *)CardId);
	sprintf(data,"{\"method\":\"thing.service.property.set\",\"id\":\"874187529\",\"params\":{\"NewCard\":\"%d%d%d%d\"},\"version\":\"1.0.0\"}", CardId[0], CardId[1], CardId[2], CardId[3]);
	printf("data���� %s\r\n", data);
	send2server(data);
	sprintf(data,"{\"method\":\"thing.service.property.set\",\"id\":\"874187529\",\"params\":{\"CardSum\":%d},\"version\":\"1.0.0\"}", card_num);
	printf("data���� %s\r\n", data);
	send2server(data);
	send_voi_cmd(0x11);
}

//ɾ��ָ����Ƭ
void del_specified_card(void){
	uint32_t card_addr;
	char data[128] = {0};
	printf("��ʼɾ��ָ����Ƭ");
	send_voi_cmd(0X13);
	while(Identificate_Card(CardType,CardId));
	printf("card num = %d\r\n", card_num);
	for(uint8_t i = 0; i < card_num; i++){
		if(!strcmp((const char *)CardId, (const char *)card_id[i])){
			printf("Ҫɾ���Ŀ� %d\r\n", i);
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
		printf("data���� %s\r\n", data);
		send2server(data);
		sprintf(data,"{\"method\":\"thing.service.property.set\",\"id\":\"874187529\",\"params\":{\"CardSum\":%d},\"version\":\"1.0.0\"}", card_num);
		printf("data���� %s\r\n", data);
		send2server(data);
		send_voi_cmd(0x11);
}


//�Ǽ�ָ��
void register_fingerprint(void){
	char data[128] = {0};
	LCD_Clear(WHITE);
	lcd_display_string(0 ,0, (uint8_t*)"ָ�Ʋɼ� #�˳�",BLACK,WHITE,FONT_SIZE_24,NULL);
	if(Enroll() == 0){
//		send_voi_cmd(0X11);
		sprintf(data,"{\"method\":\"thing.service.property.set\",\"id\":\"874187529\",\"params\":{\"FingerprintNum\":%d},\"version\":\"1.0.0\"}", get_finger_num());
			
		printf("data���� %s\r\n", data);
		send2server(data);
	}
	Delay_ms(3000);
}


//ɾ��ָ��ָ��
void del_specified_fingerprint(void){
	uint8_t fingerprint_num[64] = {0}, fingerprint_id;
	char data[128] = {0};
	sprintf((char *)fingerprint_num, "��ǰָ���� %d", get_finger_num());
	lcd_display_string(0 ,0, (uint8_t*)"�����ָ #�˳�",BLACK,WHITE,FONT_SIZE_24,NULL);
	lcd_display_string(0 ,60, (uint8_t*)fingerprint_num,BLACK,WHITE,FONT_SIZE_24,NULL);
	fig:
	fingerprint_id = match_fingerprints();
	if(fingerprint_id == 0){
		lcd_display_string(0 ,90, (uint8_t*)"                    ",BLACK,WHITE,FONT_SIZE_24,NULL);
		lcd_display_string(0 ,90, (uint8_t*)"ָ��ƥ��ʧ�ܣ�������",BLACK,WHITE,FONT_SIZE_24,NULL);
		goto fig;
	}
	else if(fingerprint_id == '#'){
		return;
	}
	del_fingerprints(fingerprint_id);
	
	lcd_display_string(0 ,90, (uint8_t*)"                    ",BLACK,WHITE,FONT_SIZE_24,NULL);
	lcd_display_string(0 ,90, (uint8_t*)"ɾ���ɹ�",BLACK,WHITE,FONT_SIZE_24,NULL);
	send_voi_cmd(0x11);
	//�ϴ�������
	sprintf(data,"{\"method\":\"thing.service.property.set\",\"id\":\"874187529\",\"params\":{\"FingerprintNum\":%d},\"version\":\"1.0.0\"}", get_finger_num());
			
	printf("data���� %s\r\n", data);
	send2server(data);
	Delay_ms(3000);
	
}

//ɾ������ָ��

void del_all_fingers(void){
	uint8_t fingerprint_num[64] = {0},key_val = 0;
	char data[128] = {0};
	sprintf((char *)fingerprint_num, "��ǰָ���� %d", get_finger_num());
	lcd_display_string(0 ,0, (uint8_t*)"�����ָ #�˳� *ȷ��",BLACK,WHITE,FONT_SIZE_24,NULL);
	lcd_display_string(0 ,60, (uint8_t*)fingerprint_num,BLACK,WHITE,FONT_SIZE_24,NULL);
	while(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_1));
	key_val = BS8116ReadKey();
	if(key_val == '#'){
		lcd_display_string(0 ,90, (uint8_t*)"                    ",BLACK,WHITE,FONT_SIZE_24,NULL);
		lcd_display_string(0 ,90, (uint8_t*)"����ȡ��",BLACK,WHITE,FONT_SIZE_24,NULL);
		Delay_ms(3000);
		return;
	}
	else if(key_val == '*'){
		del_all_fingerprints();

		lcd_display_string(0 ,90, (uint8_t*)"                    ",BLACK,WHITE,FONT_SIZE_24,NULL);
		lcd_display_string(0 ,90, (uint8_t*)"ɾ���ɹ�",BLACK,WHITE,FONT_SIZE_24,NULL);
		Delay_ms(3000);
		send_voi_cmd(0x11);
	}
	//�ϴ�������
	sprintf(data,"{\"method\":\"thing.service.property.set\",\"id\":\"874187529\",\"params\":{\"FingerprintNum\":%d},\"version\":\"1.0.0\"}", get_finger_num());
			
	printf("data���� %s\r\n", data);
	send2server(data);
	
}

//��������
uint8_t default_vol = 8;
extern uint8_t row;
void set_volume(void){
	uint8_t key_val = 0, x = 0, y = 0;
	char data[128] = {0};
	uint8_t vol_buf[64] = {0}, volflag = 0;
	row = 253;
	LCD_Clear(WHITE);
	lcd_display_string(0 ,0, (uint8_t*)"���� #�˳�",BLACK,WHITE,FONT_SIZE_24,NULL);
	lcd_display_string(0 ,30, (uint8_t*)"����+ �밴5",BLACK,WHITE,FONT_SIZE_24,NULL);
	lcd_display_string(0 ,60, (uint8_t*)"����- �밴8",BLACK,WHITE,FONT_SIZE_24,NULL);
	sprintf((char *)vol_buf, "��ǰ���� %d", default_vol);
	lcd_display_string(0 ,90, (uint8_t*)vol_buf,BLACK,WHITE,FONT_SIZE_24,NULL);
	for(int i = 0; i < default_vol; i++){
		LCD_Fill(25 + x * 25,200 - y * 15 - 15, 50 + x * 25, 200, BLACK);
		Delay_ms(100);
		x++;
		y++;
		printf("���ƴ��� %d\r\n", i + 1);
	}
	printf("�������\r\n");
	volflag = 3;
	vol_label:
	
	while(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_1));
	key_val = BS8116ReadKey();
	
	if(key_val == '5'){
		if(default_vol == 8){
			default_vol = 8;
			printf("default��Ϊ8-------\r\n");
			while(!GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_1));
			goto vol_label;
		}
		
		LCD_Fill(25 + default_vol * 25 - 25,200 - default_vol * 15 - 15, 50 + default_vol * 25 - 25, 200, BLACK);
		default_vol++;
		while(GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_4));
		printf("default_vol = %d\r\n", default_vol);
		sprintf((char *)vol_buf, "��ǰ���� %d", default_vol);
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
			printf("default��Ϊ0-------\r\n");
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
		sprintf((char *)vol_buf, "��ǰ���� %d", default_vol);
		lcd_display_string(0 ,90, (uint8_t*)"          ",BLACK,WHITE,FONT_SIZE_24,NULL);
		lcd_display_string(0 ,90, (uint8_t*)vol_buf,BLACK,WHITE,FONT_SIZE_24,NULL);
		send_voi_cmd(0X1d);
		Delay_ms(100);
		
		goto vol_label;
	}
	if(key_val == '#'){
		send_voi_cmd(0x11);
		sprintf(data,"{\"method\":\"thing.service.property.set\",\"id\":\"874187529\",\"params\":{\"VoicePwr\":%d},\"version\":\"1.0.0\"}", default_vol - 1);
			
		printf("data���� %s\r\n", data);
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


//�ָ���������
void factory_reset(void){
	uint8_t key_val = 0, data[512] = {0};
	LCD_Clear(WHITE);
	lcd_display_string(0 ,0, (uint8_t*)"�ָ���������",BLACK,WHITE,FONT_SIZE_24,NULL);
	lcd_display_string(0 ,30, (uint8_t*)"* ȷ��  #�˳�",BLACK,WHITE,FONT_SIZE_24,NULL);
	while(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_1));
	key_val = BS8116ReadKey();
	
	if(key_val == '*'){
		LCD_Clear(WHITE);
		lcd_display_string(0 ,30, (uint8_t*)"                ",WHITE,WHITE,FONT_SIZE_24,NULL);
		lcd_display_string(0 ,30, (uint8_t*)"���ڻָ���������",BLACK,WHITE,FONT_SIZE_24,NULL);
		memset(door_passwd, 0, 6);
		memset(ad_passwd, 0, 6);
		sprintf((char *)door_passwd, "%s", "666666");
		sprintf((char *)ad_passwd, "%s", "888888");
		//д��w25q64
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
		lcd_display_string(0 ,30, (uint8_t*)"�ָ��������óɹ�",BLACK,WHITE,FONT_SIZE_24,NULL);
		send_voi_cmd(0x11);
		Delay_ms(3000);
		LCD_Clear(WHITE);
		//�ϴ�������
			
		printf("data���� %s\r\n", property_payload);
		send2server(property_payload);
		
		
	}
	if(key_val == '#'){
		LCD_Clear(WHITE);
		return;
	}
	
}























