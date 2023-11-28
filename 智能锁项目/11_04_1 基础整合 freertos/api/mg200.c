#include "mg200.h"

/**
  * @brief  mg200��ʼ��
  * @param  None
  * @retval None
  */
void mg200_init(void){
	USART6_init(115200);
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);			//PC1��ͨ����
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_Init(GPIOC, &GPIO_InitStructure);			//PC0����
	GPIO_SetBits(GPIOC, GPIO_Pin_1);
//	GPIO_ResetBits(GPIOC, GPIO_Pin_1);
}	


/**
	* @brief  ��ָ��ģ�鷢��ָ��
  * @param  None
  * @retval None
  */
void mg_send_cmd(uint8_t cmd, uint8_t param){
	uint16_t sum = (SEND_ADDR+RECV_ADDR+cmd+param+RESERVE)&0xFF;
	usart_sendbit(USART6, START_CODE);
	usart_sendbit(USART6, SEND_ADDR);
	usart_sendbit(USART6, RECV_ADDR);
	usart_sendbit(USART6, cmd);
	usart_sendbit(USART6, param);
	usart_sendbit(USART6, RESERVE);
	usart_sendbit(USART6, sum);
	
}

/**
  * @brief  ץȡָ��ͼ��
  * @param  param-���� 00h, 01h, 02h, 03h, 04h ������ 3~5 ��֮�䣬 �Զ���ѡ��ע��ָ�ƵĴ�����
  * @retval None
  */

uint8_t CaptureAndExtract(uint8_t param)
{
uint8_t result = 0xc3, flag = 0;
//uint8_t ackparam;
uint16_t wait_time;
mg_send_cmd(0x51,param);
wait_time = 500;
while(wait_time--){
	if(1 == usart6_get_RXflag()){
		flag = usart6_RXpacket[1];
		result = usart6_RXpacket[4];
			printf("usart6111 = %x\r\n", usart6_RXpacket[0]);
			printf("usart6222 = %x\r\n", usart6_RXpacket[1]);
			printf("usart6333 = %x\r\n", usart6_RXpacket[2]);
			printf("usart6444 = %x\r\n", usart6_RXpacket[3]);
			printf("usart6555 = %x\r\n", usart6_RXpacket[4]);
			printf("usart6666 = %x\r\n", usart6_RXpacket[5]);
			printf("usart7777 = %x\r\n", usart6_RXpacket[6]);
			printf("usart8888 = %x\r\n", usart6_RXpacket[7]);
		memset((void *)usart6_RXpacket, 0, 8);
		break;
	}
	Delay_ms(1);	//����ɾ��ɾ�˳���Ͳ�����
}
if(flag != 0x62){
	printf("ָ��ʶ�����\r\n");
	result = 0xc3;
}

while(usart6_get_RXflag()){
	printf("111\r\n");
	memset((void *)usart6_RXpacket, 0, 8);
}

/*
����ֵ result��
00h: ץȡָ��ͼ����������ȡ�ɹ�
B1h : ָ�ƹ�С(small finger)
B2h : ��ָ�� (no finger)
B3h : ָ��λ�ù��ڿ�����Ҫ����ָ���ұ�������left finger��
B4h : ָ��λ�ù��ڿ��ң���Ҫ����ָ�����������right finger��
B5h : ָ��λ�ù��ڿ��ϣ���Ҫ����ָ���±�������up finger��
B6h : ָ��λ�ù��ڿ��£���Ҫ����ָ���ϱ����� (down finger��
B7h : ʪ��ָ ��ָ�Ʊ����������ˮ�չ��ࣩ (wet finger)
B8h : ������ָ(dry finger)
C0h: ������ȡ����
C2h: ������ȡ���� (ͼ��������)
����: ץȡָ��ͼ����������ȡ����
*/
switch(result)
{
case 0x00:printf("ץȡָ��ͼ����������ȡ�ɹ�\r\n");break;
case 0xB1:printf("small finger\r\n");break;
case 0xB2:printf("no finger\r\n");break;
case 0xB3:printf("left finger\r\n");break;
case 0xB4:printf("right finger\r\n");break;
case 0xB5:printf("up finger\r\n");break;
case 0xB6:printf("down finger\r\n");break;
case 0xB7:printf("wet finger\r\n");break;
case 0xB8:printf("dry finger\r\n");break;
case 0xC0:printf("������ȡ����\r\n");break;
case 0xC2:printf("������ȡ���� (ͼ��������)\r\n");break;
default :printf("ץȡָ��ͼ����������ȡ����\r\n");break;
}
return result;
}


/**
  * @brief  ע����ָ��
  * @param  None
  * @retval Ӧ�����
  */
uint8_t Enroll(void)
{
	uint8_t result;
	uint16_t wait_time;
//	uint8_t ackparam;
	//ץȡָ�Ƶ����� 3 ��
	printf("��һ��¼�룬�������ָ\r\n");
	send_voi_cmd(put_finger);
	lcd_display_string(30 ,90, (uint8_t*)"                     ",WHITE,WHITE,FONT_SIZE_24,NULL);
	lcd_display_string(30 ,90, (uint8_t*)"��һ��¼��",BLACK,WHITE,FONT_SIZE_24,NULL);
	while(!GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_0));//�ȴ�����
	result=CaptureAndExtract(0x00);//��һ��ץȡָ�Ƶ�����-���� 0x00
	while( result!=0x00 )
	{
	while(!GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_0));
	result=CaptureAndExtract(0x00);
	}
	printf("�ڶ���¼�룬�������ָ\r\n");
	send_voi_cmd(put_finger);
	lcd_display_string(30 ,90, (uint8_t*)"                     ",WHITE,WHITE,FONT_SIZE_24,NULL);
	lcd_display_string(30 ,90, (uint8_t*)"�ڶ���¼��",BLACK,WHITE,FONT_SIZE_24,NULL);
	Delay_ms(1000);
	while(!GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_0));//�ȴ�����
	result=CaptureAndExtract(0x01);//�ڶ���ץȡָ�Ƶ�����-���� 0x01
	while( result!=0x00 )
	{
	while(!GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_0));
	result=CaptureAndExtract(0x01);
	}
	printf("������¼�룬�������ָ\r\n");
	send_voi_cmd(put_finger);
	lcd_display_string(30 ,90, (uint8_t*)"                     ",WHITE,WHITE,FONT_SIZE_24,NULL);
	lcd_display_string(30 ,90, (uint8_t*)"������¼��",BLACK,WHITE,FONT_SIZE_24,NULL);
	Delay_ms(1000);
	while(!GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_0));//�ȴ�����
	result=CaptureAndExtract(0x02);//������ץȡָ�Ƶ�����-���� 0x02
	while( result!=0x00)
	{
	while(!GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_0));
	result=CaptureAndExtract(0x02);
	}
	mg_send_cmd(0x7F,0x00);//ע��� ID �ţ�ID ��Χ 1~100������û���Ϊ 100�� ���ò���Ϊ00h ʱ��ģ��ע��ɹ��� ID ����ģ���ڲ��Զ�����ģ�
	wait_time = 500;
while(wait_time--){
	if(1 == usart6_get_RXflag()){
		result = usart6_RXpacket[4];
		printf("usart6111 = %x\r\n", usart6_RXpacket[0]);
		printf("usart6222 = %x\r\n", usart6_RXpacket[1]);
		printf("usart6333 = %x\r\n", usart6_RXpacket[2]);
		printf("usart6444 = %x\r\n", usart6_RXpacket[3]);
		printf("usart6555 = %x\r\n", usart6_RXpacket[4]);
		printf("usart6666 = %x\r\n", usart6_RXpacket[5]);
		printf("usart7777 = %x\r\n", usart6_RXpacket[6]);
		printf("usart8888 = %x\r\n", usart6_RXpacket[7]);
		break;
	}	
	Delay_ms(1);
}
	switch(result)
	{
	case 0x00:printf("ע��ɹ�\r\n");
		send_voi_cmd(0x11);
		lcd_display_string(30 ,90, (uint8_t*)"                     ",WHITE,WHITE,FONT_SIZE_24,NULL);
		lcd_display_string(30 ,90, (uint8_t*)"ע��ɹ�",BLACK,WHITE,FONT_SIZE_24,NULL);
		break;
	case 0x83:printf("ID ����(ID < 0 ���� ID > ����û���)����ͨ�Ŵ���\r\n");
		send_voi_cmd(0x0f);
		lcd_display_string(30 ,90, (uint8_t*)"                     ",WHITE,WHITE,FONT_SIZE_24,NULL);
		lcd_display_string(30 ,90, (uint8_t*)"ͨ�Ŵ���",BLACK,WHITE,FONT_SIZE_24,NULL);
		break;
	case 0x91:printf("ע��ʧ��(�û���������)\r\n");
		send_voi_cmd(0x0f);
		lcd_display_string(30 ,90, (uint8_t*)"                     ",WHITE,WHITE,FONT_SIZE_24,NULL);
		lcd_display_string(30 ,90, (uint8_t*)"ע��ʧ��(�û���������)",BLACK,WHITE,FONT_SIZE_24,NULL);
		break;
	case 0x93:printf("�Ѿ�ע��� ID\r\n");
		send_voi_cmd(0x0f);
		lcd_display_string(30 ,90, (uint8_t*)"                     ",WHITE,WHITE,FONT_SIZE_24,NULL);
		lcd_display_string(30 ,90, (uint8_t*)"�Ѿ�ע��� ID",BLACK,WHITE,FONT_SIZE_24,NULL);
		break;
	case 0x94:printf("ָ����ȡ���� < 3\r\n");
	send_voi_cmd(0x0f);
		lcd_display_string(30 ,90, (uint8_t*)"                     ",WHITE,WHITE,FONT_SIZE_24,NULL);
		lcd_display_string(30 ,90, (uint8_t*)"ָ����ȡ���� < 3",BLACK,WHITE,FONT_SIZE_24,NULL);
		break;
		break;
	}
return result;
}



/**
  * @brief  ָ�ƱȽ�
  * @param  None
  * @retval Ӧ������
  */
uint8_t match_fingerprints(void){
	uint8_t result;
	uint16_t wait_time;
//	uint8_t ackparam;
//	uint8_t flag = 0;
	//ץȡָ�Ƶ����� 3 ��
	printf("�������ָ\r\n");
	uu:
	while(!GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_0) && GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_1)){
			vTaskDelay(100);
		}
		if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_1) == 0){
			if(BS8116ReadKey() == '#'){
				return 200;
			}
			else 
				goto uu;
		}
	result=CaptureAndExtract(0x00);//��һ��ץȡָ�Ƶ�����-���� 0x00
	while( result!=0x00 )
	{
	while(!GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_0));
	result=CaptureAndExtract(0x00);
	}
	while(usart6_get_RXflag()){
//		printf("111\r\n");
	memset((void *)usart6_RXpacket, 0, 8);
}

memset((void *)usart6_RXpacket, 0, 8);
		mg_send_cmd(0x71,0x00);//	ָ�ƶԱ�
		printf("0x71�ѷ���\r\n");
//Delay_ms(3000);
printf("��ʼ�ж�\r\n");
	result = 1;
wait_time = 500;
	while(wait_time--){
		if(1 == usart6_get_RXflag()){
				result = usart6_RXpacket[5];
//				ackparam = usart6_RXpacket[5];
//				flag = usart6_RXpacket[2];
				printf("usart6111 = %x\r\n", usart6_RXpacket[0]);
				printf("usart6222 = %x\r\n", usart6_RXpacket[1]);
				printf("usart6333 = %x\r\n", usart6_RXpacket[2]);
				printf("usart6444 = %x\r\n", usart6_RXpacket[3]);					
				printf("usart6555 = %x\r\n", usart6_RXpacket[4]);
				printf("usart6666 = %x\r\n", usart6_RXpacket[5]);
				printf("usart7777 = %x\r\n", usart6_RXpacket[6]);
				printf("usart8888 = %x\r\n", usart6_RXpacket[7]);
				printf("�ж����\r\n");
				break;
			}
		Delay_ms(1);
	}
		
		
//		Delay_ms(1);
//}
		
//	}
	
	if(result == 0){
		printf("ָ��ƥ��ʧ��\r\n");
		return result;
	}
	else{
		printf("ָ��ƥ��ɹ�\r\n");
		printf("result = %d\r\n", result);
		return result;
	}
	
}

/**
  * @brief  ɾ��ָ��idָ��
  * @param  uint8_t finger_id	��ɾ��ָ�Ƶ�id
  * @retval �ɹ� 0   ʧ�� 1
  */
uint8_t del_fingerprints(uint8_t finger_id){
	uint16_t wait_time;
	uint8_t result = 0xc3, flag = 0;
	mg_send_cmd(0x73, finger_id);
	
	wait_time = 500;
	while(wait_time--){
		if(1 == usart6_get_RXflag()){
			flag = usart6_RXpacket[1];
			result = usart6_RXpacket[4];
				printf("usart6111 = %x\r\n", usart6_RXpacket[0]);
				printf("usart6222 = %x\r\n", usart6_RXpacket[1]);
				printf("usart6333 = %x\r\n", usart6_RXpacket[2]);
				printf("usart6444 = %x\r\n", usart6_RXpacket[3]);
				printf("usart6555 = %x\r\n", usart6_RXpacket[4]);
				printf("usart6666 = %x\r\n", usart6_RXpacket[5]);
				printf("usart7777 = %x\r\n", usart6_RXpacket[6]);
				printf("usart8888 = %x\r\n", usart6_RXpacket[7]);
			memset((void *)usart6_RXpacket, 0, 8);
			break;
		}
		Delay_ms(1);
	}
	
	if(flag != 0x62){
	printf("ָ��idʶ�����\r\n");
	result = 0xc3;
	}

	while(usart6_get_RXflag()){
		memset((void *)usart6_RXpacket, 0, 8);
	}
	
	switch(result)
	{
	case 0x00:printf("ָ��ɾ���ɹ�\r\n");break;
	case 0x83:printf("��������\r\n");break;
	case 0x90:printf("δע���û�\r\n");break;
	case 0xFF:printf("д��ROM����\r\n");break;
	default :printf("ɾ��ָ�Ʒ��ش�����\r\n");break;
	}
	return result;
}


/**
  * @brief  ɾ��ָ��idָ��
  * @param  uint8_t finger_id	��ɾ��ָ�Ƶ�id
  * @retval �ɹ� 0   ʧ�� 1
  */
uint8_t del_all_fingerprints(void){
	uint16_t wait_time;
	uint8_t result = 0xc3, flag = 0;
	mg_send_cmd(0x54, 0x00);
	
	wait_time = 500;
	while(wait_time--){
		if(1 == usart6_get_RXflag()){
			flag = usart6_RXpacket[1];
			result = usart6_RXpacket[4];
				printf("usart6111 = %x\r\n", usart6_RXpacket[0]);
				printf("usart6222 = %x\r\n", usart6_RXpacket[1]);
				printf("usart6333 = %x\r\n", usart6_RXpacket[2]);
				printf("usart6444 = %x\r\n", usart6_RXpacket[3]);
				printf("usart6555 = %x\r\n", usart6_RXpacket[4]);
				printf("usart6666 = %x\r\n", usart6_RXpacket[5]);
				printf("usart7777 = %x\r\n", usart6_RXpacket[6]);
				printf("usart8888 = %x\r\n", usart6_RXpacket[7]);
			memset((void *)usart6_RXpacket, 0, 8);
			break;
		}
		Delay_ms(1);
	}
	
	if(flag != 0x62){
	printf("ɾ������ָ�ƴ���\r\n");
	result = 0xc3;
	}

	while(usart6_get_RXflag()){
		memset((void *)usart6_RXpacket, 0, 8);
	}
	
	switch(result)
	{
	case 0x00:printf("ɾ������ָ�Ƴɹ�\r\n");break;
	case 0x90:printf("ɾ��ʧ�ܣ�ע���û�Ϊ0\r\n");break;
	default :printf("ɾ��ָ�Ʒ��ش�����\r\n");break;
	}
	return result;
}


/**
  * @brief  ɾ��ָ��idָ��
  * @param  uint8_t finger_id	��ɾ��ָ�Ƶ�id
  * @retval �ɹ� 0   ʧ�� 1
  */
uint8_t get_finger_num(void){
	uint16_t wait_time;
	uint8_t result = 0xc3, flag = 0;
	mg_send_cmd(0x55, 0x00);
	
	wait_time = 500;
	while(wait_time--){
		if(1 == usart6_get_RXflag()){
			flag = usart6_RXpacket[1];
			result = usart6_RXpacket[5];
				printf("usart6111 = %x\r\n", usart6_RXpacket[0]);
				printf("usart6222 = %x\r\n", usart6_RXpacket[1]);
				printf("usart6333 = %x\r\n", usart6_RXpacket[2]);
				printf("usart6444 = %x\r\n", usart6_RXpacket[3]);
				printf("usart6555 = %x\r\n", usart6_RXpacket[4]);
				printf("usart6666 = %x\r\n", usart6_RXpacket[5]);
				printf("usart7777 = %x\r\n", usart6_RXpacket[6]);
				printf("usart8888 = %x\r\n", usart6_RXpacket[7]);
			memset((void *)usart6_RXpacket, 0, 8);
			break;
		}
		Delay_ms(1);
	}
	
	if(flag != 0x62){
	printf("��ȡ����ָ�ƴ���\r\n");
	result = 0xc3;
	}

	while(usart6_get_RXflag()){
		memset((void *)usart6_RXpacket, 0, 8);
	}
	if(result == 0x00){
		printf("��ȡ�û�ָ������ʧ��\r\n");
	}
	else{
		printf("�û�ָ������ %d\r\n", result);
	}
	return result;
}



























