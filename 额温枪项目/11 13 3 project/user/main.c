#include "main.h"



char key_val = 0;
char mode_val = 0;

int main(){

	EA = 1; // 开启总中断

	Timer0Init();
	UartInit();
	LU90614_init();	
	P0M0 = 0x00;
	P0M1 = 0x00;
	P1M0 = 0x00;
	P1M1 = 0x00;
	P2M1 &= ~(1<<0),	P2M0 |=  (1<<0);
	P4M1 &= ~(0x1F<<0),	P4M0 |=  (0x1F<<0);
	P0M1 |=  (0xF<<0),	P0M0 &= ~(0xF<<0);
	LCD_Init();
	LCD_ShowString(70, 200, RED, WHITE, "hello wrold");
	LCD_ShowSmg((240-32*5)/2-8, 100, BLUE,WHITE,"12");
	
	
	while(1){
		key_val = Get_KeyValue();
		change_mode();
		Measure_body();
	}
			
		
	
}


//切换模式
void change_mode(void){
	if(key_val == 4){
		mode_val++;
	}
	else{
		return;
	}
	
//	printf("%d\r\n", mode_val);
	switch(mode_val){
		case 1: 
			LCD_ShowString(70, 200, WHITE, WHITE, "               ");
			LCD_ShowString(70, 200, RED, WHITE, "Measure"); 
			break;
		case 2: 
			LCD_ShowString(70, 200, WHITE, WHITE, "               ");
			LCD_ShowString(70, 200, RED, WHITE, "Setup");
			break;
		case 3: 
			LCD_ShowString(70, 200, WHITE, WHITE, "               ");
			LCD_ShowString(70, 200, RED, WHITE, "History");
			break;
		case 4: 
			LCD_ShowString(70, 200, WHITE, WHITE, "               ");
			LCD_ShowString(70, 200, RED, WHITE, "Silent");
			break;
		default : break;
	}
	if(mode_val == 4){
		mode_val = 0;
	}
}


//测量体温
void Measure_body(void){
	char body_val[8] = {0};
	unsigned char i;
	float sum = 0;
	if(key_val == 1){
		for(i = 0; i < 5; i++){
			printf("i = %d\r\n", i);
			sum = read_body_temperature();
			Delay_ms(10);
		}
	}
					
	sum = sum / 5;
	sprintf(body_val, "%.2f", sum);
	LCD_ShowSmg((240-32*5)/2-8, 100, BLUE,WHITE,(unsigned char*)body_val);
	
}
















