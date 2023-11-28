/***********************************************************************************
 * __  ____    __ ______
 * \ \/ /\ \  / /|  __  \	�ٷ���վ�� www.edu118.com
 *  \  /  \_\/_/ | |  \ |					
 *  /  \   |  |  | |__/ |					
 * /_/\_\  |__|  |______/					
 *
 * ��Ȩ��������ӯ��Ƽ����޹�˾��֣�ݷֲ�����������Ȩ
 * �ļ�����main.c
 * ���ߣ�XYD_HWG
 * �汾�ţ�V0.0.4
 * ���ڣ�2021��12��11��
 * ����������ǹʵѵ�汾���Դ���
 * ��ע��STC8G2K32S2��ϵͳʱ��24MHz����λ���ſ��á�����1������9600
 * ��ʷ��¼��
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
unsigned char Mode = Measure;	//ģʽ
float now_temp = 0.0, Threshold = 37.0; //�¶Ⱥ��¶���ֵ
unsigned char keyValue = 0;
char Mantissa[4] = {0};	//�¶�������ʾ������
char Fraction[4] = {0};	//�¶�С����ʾ������
char showBuff[20] = {0};	//��ʾ������
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

//ģʽ�л�
void Mode_Change(unsigned char key)
{
	if(key != 4)	return;
	Mode++; 
	Mode %= 4;
	sprintf(showBuff, "Mode:%s", ModeString[Mode]);
	LCD_ShowString(70, 200, RED, WHITE, (unsigned char*)showBuff);
}

//����ģʽ
void Mode_Measure(unsigned char key)
{
	unsigned char i=0;
	if(key != 1)	return;
	Mode = Measure; //����������ǿ���л�ģʽ������ģʽ
	now_temp = 0;
	OLED_BLK_Set();
	sprintf(showBuff, "Mode:%s", ModeString[Mode]);
	LCD_ShowString(70, 200, RED, WHITE, (unsigned char*)showBuff);
	if(beepFlag == 1) BEEP = 0;
	for(i=0; i<5; i++) {	//��������5��
		now_temp += SMBus_ReadTemp();
		Delay_ms(5);
	}
	now_temp = now_temp/5;//��ȡƽ��ֵ
	BEEP = 1;
	now_temp = now_temp+3.5;//�����¶�ֵ
	
	if(now_temp > 99)	now_temp = 99;
	if(now_temp < 0) now_temp = 0;
}


//������ʾ
void View_Show(unsigned char mode)
{
	sprintf(Mantissa, "%02d", (int)now_temp);
	sprintf(Fraction, "%02d", (int)((now_temp-(int)now_temp)*100));
	sprintf(showBuff, "%s.%s ", Mantissa, Fraction);
	if(now_temp > Threshold) {//�¶ȹ���
		if(beepFlag == 1) BEEP = 0;
		LCD_ShowSmg((240-32*5)/2-8, 100, RED, WHITE,(unsigned char*)showBuff);
		LCD_ShowPicture(160, 10, 36, 36, gImage_alarm);//����ͼƬ
	}
	else {//�¶�����
		BEEP = 1;
		LCD_ShowSmg((240-32*5)/2-8, 100, BLUE,WHITE,(unsigned char*)showBuff);
		LCD_ClearRect(160, 10, 36, 36, WHITE);
	}
}





