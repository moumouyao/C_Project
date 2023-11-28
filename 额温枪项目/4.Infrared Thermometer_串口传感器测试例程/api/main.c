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
 * 版本号：V0.0.2
 * 日期：2021年12月11日
 * 描述：额温枪实训版本测试代码
 * 备注：STC8G2K32S2、系统时钟24MHz、复位引脚可用、串口1波特率9600
 * 历史记录：
 * 
 **********************************************************************************
 */ 

#include "main.h"
#include "tim.h"
#include "uart1.h"
#include "stdio.h"

float now_temp = 0; //温度和温度阈值





void main()
{
	AUXR &= ~(1<<1);//使用内扩RAM
	Tim0_Config();
	UartInit();
	Uart2Init();
	EA = 1;
	Uart_SendString("hello\r\n", 7);

	while(1)
	{
		printf("hello\r\n");
		Lu90614_ReadData(&now_temp);
		printf("now_temp:%0.2f", now_temp);
		Delay_ms(500);
	}
}










