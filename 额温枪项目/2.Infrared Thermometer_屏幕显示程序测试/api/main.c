/***********************************************************************************
 * __  ____    __ ______
 * \ \/ /\ \  / /|  __  \	官方网站： www.edu118.com
 *  \  /  \_\/_/ | |  \ |					
 *  /  \   |  |  | |__/ |					
 * /_/\_\  |__|  |______/					
 *
 * 版权◎深圳信盈达科技有限公司（郑州分部）保留所有权
 * 文件名：main.c
 * 作者：XYD_HWG
 * 版本号：V0.0.4
 * 日期：2021年12月11日
 * 描述：额温枪实训版本测试代码
 * 备注：STC8G2K32S2、系统时钟24MHz、复位引脚可用、串口1波特率9600
 * 历史记录：
 * 
 **********************************************************************************
 */ 

#include "main.h"
#include "ioConfig.h"
#include "tim.h"
#include "uart1.h"
#include "key.h"
#include "mlx90614.h"
#include "lcd_driver.h"
#include "power.h"
#include "stdio.h"
#include "string.h"


extern code unsigned char gImage_logo[6264];
extern code unsigned char gImage_beep_off_icon[2592];
extern code unsigned char gImage_beep_on_icon[2592];
extern code unsigned char gImage_history[2592];
extern code unsigned char gImage_alarm[2592];

typedef enum {
	Measure = 0,
	Setup = 1,
	History = 2,
	Silent = 3,
}__MODE_TYPE;


code char ModeString[4][16] = {"Measure ", "Setup  ", "History ", "Silent "};
unsigned char Mode = Measure;	//模式
float now_temp = 0.0, Threshold = 37.0; //温度和温度阈值
unsigned char keyValue = 0;
char Mantissa[4] = {0};	//温度整数显示缓存区
char Fraction[4] = {0};	//温度小数显示缓存区
char showBuff[20] = {0};	//显示缓存区
volatile unsigned char beepFlag = 1;


void Mode_Change(unsigned char key);
void View_Show(unsigned char mode);
void Mode_Measure(unsigned char key);
void main()
{

	IO_Config();
	Tim0_Config();
	UartInit();
	EA = 1;
	SMBus_Init();
	LCD_Init();
	Uart_SendString("hello world\r\n", strlen("hello world\r\n"));
	LCD_ShowPicture(10, 10, 87, 36, gImage_logo);
	sprintf(showBuff, "Mode:%s", ModeString[Mode]);
	LCD_ShowString(70, 200, RED, WHITE, (unsigned char*)showBuff);
	while(1)
	{
		keyValue = Get_KeyValue();
		Mode_Change(keyValue);
		Mode_Measure(keyValue);
		if(Led_TaskTime[0] > Led_TaskTime[1]) {
			LED ^= 1;
			Led_TaskTime[0] = 0;
		}
		if(View_TaskTime[0] > View_TaskTime[1]) {
			View_Show(Mode);
			View_TaskTime[0] = 0;
		}
		
	}
}

//模式切换
void Mode_Change(unsigned char key)
{
	if(key != 4)	return;
	Mode++; 
	Mode %= 4;
	sprintf(showBuff, "Mode:%s", ModeString[Mode]);
	LCD_ShowString(70, 200, RED, WHITE, (unsigned char*)showBuff);
}

//测量模式
void Mode_Measure(unsigned char key)
{
	unsigned char i=0;
	if(key != 1)	return;
	Mode = Measure; //测量按键，强制切换模式到测量模式
	now_temp = 0;
	OLED_BLK_Set();
	sprintf(showBuff, "Mode:%s", ModeString[Mode]);
	LCD_ShowString(70, 200, RED, WHITE, (unsigned char*)showBuff);
	if(beepFlag == 1) BEEP = 0;
	for(i=0; i<5; i++) {	//连续采样5次
		now_temp += SMBus_ReadTemp();
		Delay_ms(5);
	}
	now_temp = now_temp/5;//求取平均值
	BEEP = 1;
	now_temp = now_temp+3.5;//修正温度值
	
	if(now_temp > 99)	now_temp = 99;
	if(now_temp < 0) now_temp = 0;
}


//数据显示
void View_Show(unsigned char mode)
{
	sprintf(Mantissa, "%02d", (int)now_temp);
	sprintf(Fraction, "%02d", (int)((now_temp-(int)now_temp)*100));
	sprintf(showBuff, "%s.%s ", Mantissa, Fraction);
	if(now_temp > Threshold) {//温度过高
		if(beepFlag == 1) BEEP = 0;
		LCD_ShowSmg((240-32*5)/2-8, 100, RED, WHITE,(unsigned char*)showBuff);
		LCD_ShowPicture(160, 10, 36, 36, gImage_alarm);//报警图片
	}
	else {//温度正常
		BEEP = 1;
		LCD_ShowSmg((240-32*5)/2-8, 100, BLUE,WHITE,(unsigned char*)showBuff);
		LCD_ClearRect(160, 10, 36, 36, WHITE);
	}
}





