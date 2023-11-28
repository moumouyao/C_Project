#include "key.h"

/**
  * @brief  按键初始化
  * @param  None
  * @retval None
  */
void key_init(void){
	  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	
	  GPIO_InitTypeDef GPIO_InitStructure;
		
		//初始化按键1	下拉输入
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
		//初始化按键2	上拉输入
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  	GPIO_Init(GPIOC, &GPIO_InitStructure);

}


/**
  * @brief  按键检测
  * @param  None
  * @retval 按键按下返回对应的键值，没有按键按下返回0
  */
uint8_t key_get_num(void){
	uint8_t keynum = 0;
	//按键1按下
	if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0) == 1){
		Delay_ms(5);	//消抖
		if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0) != 1){
			return 0;
		}
		while(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0) == 1);
		Delay_ms(5);
		keynum = 1;
	}
	//按键2按下
	if(GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_13) == 0){
		Delay_ms(5);	//消抖
		if(GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_13) != 0){
			return 0;
		}
		while(GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_13) == 0);
		Delay_ms(5);
		keynum = 2;
	}
	return keynum;
}

/**
  * @brief  //初始化定时器3，用于计算按下时间
  * @param  None
  * @retval None
  */
uint8_t counter= 0;
void TIM3_Int_Init(u16 arr,u16 psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);  ///使能TIM3时钟
	
  	TIM_TimeBaseInitStructure.TIM_Period = arr; 	//自动重装载值
	TIM_TimeBaseInitStructure.TIM_Prescaler=psc;  //定时器分频
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //向下计数模式
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStructure);//初始化TIM3
	
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE); //允许定时器3更新中断
	TIM_Cmd(TIM3,ENABLE); //使能定时器3
	
	NVIC_InitStructure.NVIC_IRQChannel=TIM3_IRQn; //定时器3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x01; //抢占优先级1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x03; //子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
}

//定时器3中断服务函数
void TIM3_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM3,TIM_IT_Update)==SET) //溢出中断
	{
		if(counter > 10){
			counter = 0;
		}
		counter++;
		
//		printf("counter = %d\r\n", counter);
	}
	TIM_ClearITPendingBit(TIM3,TIM_IT_Update);  //清除中断标志位
}


/**
  * @brief  键盘输入	
  * @param  None
  * @retval None
  */
extern uint8_t lcdabs;
extern uint8_t row;
uint8_t key_flag = 0, temp = 0;

uint8_t keyboard_input(void){
	uint8_t zifu[10][9]={
	{'0',' '},
	{'1','-','_','@','.','/',':','"','~'},
	{'2','a','b','c','A','B','C'},
	{'3','d','e','f','D','E','F'},
	{'4','g','h','i','G','H','I'},
	{'5','j','k','l','J','K','L'},
	{'6','m','n','o','M','N','O'},
	{'7','p','q','r','s','P','Q','R','S'},
	{'8','t','u','v','T','U','V'},
	{'9','w','x','y','z','W','X','Y','Z'},
	};
	uint8_t press_num = 0;
	uint8_t bs_data, Flag;
	
	bs_data = BS8116ReadKey();
	label:
	switch(bs_data){
		case '1':
		{
			press_num = 0;
			counter = 0;
			while(counter < 3){
				lcd_display_string(lcdabs + 16 * 6,row,(unsigned char *)"1",WHITE,RED,FONT_SIZE_24,NULL);
				labe1:
				while(!GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_1));
				send_voi_cmd(dudu);
				while(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_1) && counter <= 3){
//					printf("阻塞1\r\n");
				}
				if(counter > 3)
					break;
				printf("1111\r\n");
				if((Flag = BS8116ReadKey()) == '1'){
					press_num ++;
					counter = 0;
					printf("label2\r\n");
					temp = zifu[1][press_num];
					if(row < 250){
//						lcd_display_string(lcdabs + 16 * 6,row,(unsigned char *)"8",WHITE,WHITE,FONT_SIZE_24,NULL);
						lcd_display_string(lcdabs + 16 * 6,row,&temp,BLUE,RED,FONT_SIZE_24,NULL);
					}
					
					goto labe1;  /* endless loop */
				}
				else{
					printf("1选择的字符 %c\r\n", zifu[1][press_num]);
					bs_data = Flag;
					  goto label;  /* endless loop */
				}
				while(!GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_1)){
					printf("阻塞2\r\n");
				}
			}
			printf("2选择的字符 %c\r\n", zifu[1][press_num]);
			Delay_ms(10);
//			break;
			return zifu[1][press_num];
		}

		case '2':
		{
			press_num = 0;
			counter = 0;
			while(counter < 3){
				lcd_display_string(lcdabs + 16 * 6,row,(unsigned char *)"2",WHITE,RED,FONT_SIZE_24,NULL);
				labe2:
				while(!GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_1));
				send_voi_cmd(dudu);
				while(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_1) && counter <= 3){
//					printf("阻塞1\r\n");
				}
				if(counter > 3)
					break;
				printf("1111\r\n");
				if((Flag = BS8116ReadKey()) == '2'){
					press_num ++;
					counter = 0;
					printf("label2\r\n");
					temp = zifu[2][press_num];
					if(row < 250){
//						lcd_display_string(lcdabs + 16 * 6,row,(unsigned char *)"8",WHITE,WHITE,FONT_SIZE_24,NULL);
						lcd_display_string(lcdabs + 16 * 6,row,&temp,BLUE,RED,FONT_SIZE_24,NULL);
					}
					goto labe2;  /* endless loop */
				}
				else{
					printf("1选择的字符 %c\r\n", zifu[2][press_num]);
					bs_data = Flag;
					  goto label;  /* endless loop */
				}
				while(!GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_1)){
					printf("阻塞2\r\n");
				}
			}
			printf("2选择的字符 %c\r\n", zifu[2][press_num]);
			Delay_ms(10);
//			break;
			return zifu[2][press_num];
		}
		case '3':
		{
			press_num = 0;
			counter = 0;
			while(counter < 3){
				lcd_display_string(lcdabs + 16 * 6,row,(unsigned char *)"3",WHITE,RED,FONT_SIZE_24,NULL);
				labe3:
				while(!GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_1));
				send_voi_cmd(dudu);
				while(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_1) && counter <= 3){
//					printf("阻塞1\r\n");
				}
				if(counter > 3)
					break;
				printf("1111\r\n");
				if((Flag = BS8116ReadKey()) == '3'){
					press_num ++;
					counter = 0;
					printf("label2\r\n");
					temp = zifu[3][press_num];
					if(row < 250){
//						lcd_display_string(lcdabs + 16 * 6,row,(unsigned char *)"8",WHITE,WHITE,FONT_SIZE_24,NULL);
						lcd_display_string(lcdabs + 16 * 6,row,&temp,BLUE,RED,FONT_SIZE_24,NULL);
					}
					goto labe3;  /* endless loop */
				}
				else{
					printf("1选择的字符 %c\r\n", zifu[3][press_num]);
					bs_data = Flag;
					  goto label;  /* endless loop */
				}
				while(!GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_1)){
					printf("阻塞2\r\n");
				}
			}
			printf("2选择的字符 %c\r\n", zifu[3][press_num]);
			Delay_ms(10);
//			break;
			return zifu[3][press_num];
		}
		case '4':
		{
			press_num = 0;
			counter = 0;
			while(counter < 3){
				lcd_display_string(lcdabs + 16 * 6,row,(unsigned char *)"4",WHITE,RED,FONT_SIZE_24,NULL);
				labe4:
				while(!GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_1));
				send_voi_cmd(dudu);
				while(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_1) && counter <= 3){
//					printf("阻塞1\r\n");
				}
				if(counter > 3)
					break;
				printf("1111\r\n");
				if((Flag = BS8116ReadKey()) == '4'){
					press_num ++;
					counter = 0;
					printf("label2\r\n");
					temp = zifu[4][press_num];
					if(row < 250){
//						lcd_display_string(lcdabs + 16 * 6,row,(unsigned char *)"8",WHITE,WHITE,FONT_SIZE_24,NULL);
						lcd_display_string(lcdabs + 16 * 6,row,&temp,BLUE,RED,FONT_SIZE_24,NULL);
					}
					goto labe4;  /* endless loop */
				}
				else{
					printf("1选择的字符 %c\r\n", zifu[4][press_num]);
					bs_data = Flag;
					  goto label;  /* endless loop */
				}
				while(!GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_1)){
					printf("阻塞2\r\n");
				}
			}
			printf("2选择的字符 %c\r\n", zifu[4][press_num]);
			Delay_ms(10);
//			break;
			return zifu[4][press_num];
		}
		case '5':
		{
			press_num = 0;
			counter = 0;
			while(counter < 3){
				lcd_display_string(lcdabs + 16 * 6,row,(unsigned char *)"5",WHITE,RED,FONT_SIZE_24,NULL);
				labe5:
				while(!GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_1));
				send_voi_cmd(dudu);
				while(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_1) && counter <= 3){
//					printf("阻塞1\r\n");
				}
				if(counter > 3)
					break;
				printf("1111\r\n");
				if((Flag = BS8116ReadKey()) == '5'){
					press_num ++;
					counter = 0;
					printf("label222222\r\n");
					temp = zifu[5][press_num];
					if(row < 250){
//						lcd_display_string(lcdabs + 16 * 6,row,(unsigned char *)"8",WHITE,WHITE,FONT_SIZE_24,NULL);
						lcd_display_string(lcdabs + 16 * 6,row,&temp,BLUE,RED,FONT_SIZE_24,NULL);
					}
					goto labe5;  /* endless loop */
				}
				else{
					printf("1选择的字符 %c\r\n", zifu[5][press_num]);
					bs_data = Flag;
					  goto label;  /* endless loop */
				}
				while(!GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_1)){
					printf("阻塞2\r\n");
				}
			}
			printf("2选择的字符 %c\r\n", zifu[5][press_num]);
			Delay_ms(10);
//			break;
			return zifu[5][press_num];
		}
		case '6':
		{
			press_num = 0;
			counter = 0;
			while(counter < 3){
				lcd_display_string(lcdabs + 16 * 6,row,(unsigned char *)"6",WHITE,RED,FONT_SIZE_24,NULL);
				labe6:
				while(!GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_1));
				send_voi_cmd(dudu);
				while(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_1) && counter <= 3){
//					printf("阻塞1\r\n");
				}
				if(counter > 3)
					break;
				printf("1111\r\n");
				if((Flag = BS8116ReadKey()) == '6'){
					press_num ++;
					counter = 0;
					printf("label2\r\n");
					temp = zifu[6][press_num];
					if(row < 250){
//						lcd_display_string(lcdabs + 16 * 6,row,(unsigned char *)"8",WHITE,WHITE,FONT_SIZE_24,NULL);
						lcd_display_string(lcdabs + 16 * 6,row,&temp,BLUE,RED,FONT_SIZE_24,NULL);
					}
					goto labe6;  /* endless loop */
				}
				else{
					printf("1选择的字符 %c\r\n", zifu[6][press_num]);
					bs_data = Flag;
					  goto label;  /* endless loop */
				}
				while(!GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_1)){
					printf("阻塞2\r\n");
				}
			}
			printf("2选择的字符 %c\r\n", zifu[6][press_num]);
			Delay_ms(10);
//			break;
			return zifu[6][press_num];
		}
		case '7':
		{
			press_num = 0;
			counter = 0;
			while(counter < 3){
				lcd_display_string(lcdabs + 16 * 6,row,(unsigned char *)"7",WHITE,RED,FONT_SIZE_24,NULL);
				labe7:
				while(!GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_1));
				send_voi_cmd(dudu);
				while(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_1) && counter <= 3){
//					printf("阻塞1\r\n");
				}
				if(counter > 3)
					break;
				printf("1111\r\n");
				if((Flag = BS8116ReadKey()) == '1'){
					press_num ++;
					counter = 0;
					printf("label2\r\n");
					temp = zifu[7][press_num];
					if(row < 250){
//						lcd_display_string(lcdabs + 16 * 6,row,(unsigned char *)"8",WHITE,WHITE,FONT_SIZE_24,NULL);
						lcd_display_string(lcdabs + 16 * 6,row,&temp,BLUE,RED,FONT_SIZE_24,NULL);
					}
					goto labe7;  /* endless loop */
				}
				else{
					printf("1选择的字符 %c\r\n", zifu[7][press_num]);
					bs_data = Flag;
					  goto label;  /* endless loop */
				}
				while(!GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_1)){
					printf("阻塞2\r\n");
				}
			}
			printf("2选择的字符 %c\r\n", zifu[7][press_num]);
			Delay_ms(10);
//			break;
			return zifu[7][press_num];
		}
		case '8':
		{
			press_num = 0;
			counter = 0;
			while(counter < 3){
				lcd_display_string(lcdabs + 16 * 6,row,(unsigned char *)"8",WHITE,RED,FONT_SIZE_24,NULL);
				labe8:
				while(!GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_1));
				send_voi_cmd(dudu);
				while(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_1) && counter <= 3){
//					printf("阻塞1\r\n");
				}
				if(counter > 3)
					break;
				printf("1111\r\n");
				if((Flag = BS8116ReadKey()) == '8'){
					press_num ++;
					counter = 0;
					printf("label2\r\n");
					temp = zifu[8][press_num];
					if(row < 250){
//					lcd_display_string(lcdabs + 16 * 6,row,(unsigned char *)"8",WHITE,WHITE,FONT_SIZE_24,NULL);
						lcd_display_string(lcdabs + 16 * 6,row,&temp,BLUE,RED,FONT_SIZE_24,NULL);
					}
					goto labe8;  /* endless loop */
				}
				else{
					printf("1选择的字符 %c\r\n", zifu[8][press_num]);
					bs_data = Flag;
					  goto label;  /* endless loop */
				}
				while(!GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_1)){
					printf("阻塞2\r\n");
				}
			}
			printf("2选择的字符 %c\r\n", zifu[8][press_num]);
			Delay_ms(10);
//			break;
			return zifu[8][press_num];
		}
		case '9':
		{
			press_num = 0;
			counter = 0;
			while(counter < 3){
				lcd_display_string(lcdabs + 16 * 6,row,(unsigned char *)"9",WHITE,RED,FONT_SIZE_24,NULL);
				labe9:
				while(!GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_1));
				send_voi_cmd(dudu);
				while(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_1) && counter <= 3){
//					printf("阻塞1\r\n");
				}
				if(counter > 3)
					break;
				printf("1111\r\n");
				if((Flag = BS8116ReadKey()) == '9'){
					press_num ++;
					counter = 0;
					printf("label2\r\n");
//					if(!key_flag){
						temp = zifu[9][press_num];
						if(row < 250){
//						lcd_display_string(lcdabs + 16 * 6,row,(unsigned char *)"8",WHITE,WHITE,FONT_SIZE_24,NULL);
						lcd_display_string(lcdabs + 16 * 6,row,&temp,BLUE,RED,FONT_SIZE_24,NULL);
					}
//						printf("labasd %d\r\n", lcdabs);
//					}
						
					goto labe9;  /* endless loop */
				}
				else{
					printf("1选择的字符 %c\r\n", zifu[9][press_num]);
					bs_data = Flag;
					  goto labe9;  /* endless loop */
				}
				while(!GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_1)){
					printf("阻塞2\r\n");
				}
			}
			printf("2选择的字符 %c\r\n", zifu[9][press_num]);
			Delay_ms(10);
//			break;
			return zifu[9][press_num];
		}
		case '*':{
			{

				while(!GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_1));
				send_voi_cmd(dudu);
				while(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_1)){
				}

			
			Delay_ms(10);

			return '*';
		}
		}
//		case '0'
		case '#':
		{
			{

				while(!GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_1));
				send_voi_cmd(dudu);
				while(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_1)){
				}

			Delay_ms(10);

			return '#';
		}
			
		}
			
	}
	
	
}






















