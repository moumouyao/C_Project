#include "mg200.h"

/**
  * @brief  mg200初始化
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
	GPIO_Init(GPIOC, &GPIO_InitStructure);			//PC1普通推挽
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_Init(GPIOC, &GPIO_InitStructure);			//PC0浮空
	GPIO_SetBits(GPIOC, GPIO_Pin_1);
//	GPIO_ResetBits(GPIOC, GPIO_Pin_1);
}	


/**
	* @brief  向指纹模块发送指令
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
  * @brief  抓取指纹图像
  * @param  param-参数 00h, 01h, 02h, 03h, 04h （可在 3~5 次之间， 自定义选择注册指纹的次数）
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
	Delay_ms(1);	//不能删，删了程序就不正常
}
if(flag != 0x62){
	printf("指纹识别错误\r\n");
	result = 0xc3;
}

while(usart6_get_RXflag()){
	printf("111\r\n");
	memset((void *)usart6_RXpacket, 0, 8);
}

/*
返回值 result：
00h: 抓取指纹图像及特征量提取成功
B1h : 指纹过小(small finger)
B2h : 无指纹 (no finger)
B3h : 指纹位置过于靠左，需要将手指靠右边摁按（left finger）
B4h : 指纹位置过于靠右，需要将手指靠左边摁按（right finger）
B5h : 指纹位置过于靠上，需要将手指靠下边摁按（up finger）
B6h : 指纹位置过于靠下，需要将手指靠上边摁按 (down finger）
B7h : 湿手指 （指纹表面出汗或者水渍过多） (wet finger)
B8h : 干燥手指(dry finger)
C0h: 特征提取错误
C2h: 特征提取错误 (图像质量差)
其他: 抓取指纹图像及特征量提取错误
*/
switch(result)
{
case 0x00:printf("抓取指纹图像及特征量提取成功\r\n");break;
case 0xB1:printf("small finger\r\n");break;
case 0xB2:printf("no finger\r\n");break;
case 0xB3:printf("left finger\r\n");break;
case 0xB4:printf("right finger\r\n");break;
case 0xB5:printf("up finger\r\n");break;
case 0xB6:printf("down finger\r\n");break;
case 0xB7:printf("wet finger\r\n");break;
case 0xB8:printf("dry finger\r\n");break;
case 0xC0:printf("特征提取错误\r\n");break;
case 0xC2:printf("特征提取错误 (图像质量差)\r\n");break;
default :printf("抓取指纹图像及特征量提取错误\r\n");break;
}
return result;
}


/**
  * @brief  注册新指纹
  * @param  None
  * @retval 应答参数
  */
uint8_t Enroll(void)
{
	uint8_t result;
	uint16_t wait_time;
//	uint8_t ackparam;
	//抓取指纹的特征 3 次
	printf("第一次录入，请放下手指\r\n");
	send_voi_cmd(put_finger);
	lcd_display_string(30 ,90, (uint8_t*)"                     ",WHITE,WHITE,FONT_SIZE_24,NULL);
	lcd_display_string(30 ,90, (uint8_t*)"第一次录入",BLACK,WHITE,FONT_SIZE_24,NULL);
	while(!GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_0));//等待触摸
	result=CaptureAndExtract(0x00);//第一次抓取指纹的特征-参数 0x00
	while( result!=0x00 )
	{
	while(!GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_0));
	result=CaptureAndExtract(0x00);
	}
	printf("第二次录入，请放下手指\r\n");
	send_voi_cmd(put_finger);
	lcd_display_string(30 ,90, (uint8_t*)"                     ",WHITE,WHITE,FONT_SIZE_24,NULL);
	lcd_display_string(30 ,90, (uint8_t*)"第二次录入",BLACK,WHITE,FONT_SIZE_24,NULL);
	Delay_ms(1000);
	while(!GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_0));//等待触摸
	result=CaptureAndExtract(0x01);//第二次抓取指纹的特征-参数 0x01
	while( result!=0x00 )
	{
	while(!GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_0));
	result=CaptureAndExtract(0x01);
	}
	printf("第三次录入，请放下手指\r\n");
	send_voi_cmd(put_finger);
	lcd_display_string(30 ,90, (uint8_t*)"                     ",WHITE,WHITE,FONT_SIZE_24,NULL);
	lcd_display_string(30 ,90, (uint8_t*)"第三次录入",BLACK,WHITE,FONT_SIZE_24,NULL);
	Delay_ms(1000);
	while(!GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_0));//等待触摸
	result=CaptureAndExtract(0x02);//第三次抓取指纹的特征-参数 0x02
	while( result!=0x00)
	{
	while(!GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_0));
	result=CaptureAndExtract(0x02);
	}
	mg_send_cmd(0x7F,0x00);//注册的 ID 号（ID 范围 1~100，最大用户数为 100； 当该参数为00h 时，模块注册成功的 ID 号是模块内部自动分配的）
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
	case 0x00:printf("注册成功\r\n");
		send_voi_cmd(0x11);
		lcd_display_string(30 ,90, (uint8_t*)"                     ",WHITE,WHITE,FONT_SIZE_24,NULL);
		lcd_display_string(30 ,90, (uint8_t*)"注册成功",BLACK,WHITE,FONT_SIZE_24,NULL);
		break;
	case 0x83:printf("ID 错误(ID < 0 或者 ID > 最大用户数)或者通信错误\r\n");
		send_voi_cmd(0x0f);
		lcd_display_string(30 ,90, (uint8_t*)"                     ",WHITE,WHITE,FONT_SIZE_24,NULL);
		lcd_display_string(30 ,90, (uint8_t*)"通信错误",BLACK,WHITE,FONT_SIZE_24,NULL);
		break;
	case 0x91:printf("注册失败(用户区域已满)\r\n");
		send_voi_cmd(0x0f);
		lcd_display_string(30 ,90, (uint8_t*)"                     ",WHITE,WHITE,FONT_SIZE_24,NULL);
		lcd_display_string(30 ,90, (uint8_t*)"注册失败(用户区域已满)",BLACK,WHITE,FONT_SIZE_24,NULL);
		break;
	case 0x93:printf("已经注册的 ID\r\n");
		send_voi_cmd(0x0f);
		lcd_display_string(30 ,90, (uint8_t*)"                     ",WHITE,WHITE,FONT_SIZE_24,NULL);
		lcd_display_string(30 ,90, (uint8_t*)"已经注册的 ID",BLACK,WHITE,FONT_SIZE_24,NULL);
		break;
	case 0x94:printf("指纹提取次数 < 3\r\n");
	send_voi_cmd(0x0f);
		lcd_display_string(30 ,90, (uint8_t*)"                     ",WHITE,WHITE,FONT_SIZE_24,NULL);
		lcd_display_string(30 ,90, (uint8_t*)"指纹提取次数 < 3",BLACK,WHITE,FONT_SIZE_24,NULL);
		break;
		break;
	}
return result;
}



/**
  * @brief  指纹比较
  * @param  None
  * @retval 应带参数
  */
uint8_t match_fingerprints(void){
	uint8_t result;
	uint16_t wait_time;
//	uint8_t ackparam;
//	uint8_t flag = 0;
	//抓取指纹的特征 3 次
	printf("请放下手指\r\n");
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
	result=CaptureAndExtract(0x00);//第一次抓取指纹的特征-参数 0x00
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
		mg_send_cmd(0x71,0x00);//	指纹对比
		printf("0x71已发送\r\n");
//Delay_ms(3000);
printf("开始判断\r\n");
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
				printf("判断完成\r\n");
				break;
			}
		Delay_ms(1);
	}
		
		
//		Delay_ms(1);
//}
		
//	}
	
	if(result == 0){
		printf("指纹匹配失败\r\n");
		return result;
	}
	else{
		printf("指纹匹配成功\r\n");
		printf("result = %d\r\n", result);
		return result;
	}
	
}

/**
  * @brief  删除指定id指纹
  * @param  uint8_t finger_id	待删除指纹的id
  * @retval 成功 0   失败 1
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
	printf("指纹id识别错误\r\n");
	result = 0xc3;
	}

	while(usart6_get_RXflag()){
		memset((void *)usart6_RXpacket, 0, 8);
	}
	
	switch(result)
	{
	case 0x00:printf("指纹删除成功\r\n");break;
	case 0x83:printf("参数错误\r\n");break;
	case 0x90:printf("未注册用户\r\n");break;
	case 0xFF:printf("写入ROM错误\r\n");break;
	default :printf("删除指纹返回错误码\r\n");break;
	}
	return result;
}


/**
  * @brief  删除指定id指纹
  * @param  uint8_t finger_id	待删除指纹的id
  * @retval 成功 0   失败 1
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
	printf("删除所有指纹错误\r\n");
	result = 0xc3;
	}

	while(usart6_get_RXflag()){
		memset((void *)usart6_RXpacket, 0, 8);
	}
	
	switch(result)
	{
	case 0x00:printf("删除所有指纹成功\r\n");break;
	case 0x90:printf("删除失败，注册用户为0\r\n");break;
	default :printf("删除指纹返回错误码\r\n");break;
	}
	return result;
}


/**
  * @brief  删除指定id指纹
  * @param  uint8_t finger_id	待删除指纹的id
  * @retval 成功 0   失败 1
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
	printf("获取所有指纹错误\r\n");
	result = 0xc3;
	}

	while(usart6_get_RXflag()){
		memset((void *)usart6_RXpacket, 0, 8);
	}
	if(result == 0x00){
		printf("获取用户指纹数量失败\r\n");
	}
	else{
		printf("用户指纹数量 %d\r\n", result);
	}
	return result;
}



























