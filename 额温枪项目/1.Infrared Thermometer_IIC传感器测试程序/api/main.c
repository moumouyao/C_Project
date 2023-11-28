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
 * 描述：额温枪测试代码
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
#include "power.h"
#include "stdio.h"
#include "string.h"

float now_temp = 0.0;

void main()
{
	unsigned char key = 0;
	IO_Config();
	Tim0_Config();
	UartInit();
	EA = 1;
	SMBus_Init();
	while(1)
	{
		key = Get_KeyValue();
		if(key == 1) { //按下测量按键 获取温度值
			now_temp = SMBus_ReadTemp();
			printf("%0.2f\r\n", now_temp);
			
		}
		if(Led_TaskTime[0] > Led_TaskTime[1]) {
			LED ^= 1;
			Led_TaskTime[0] = 0;
		}
	}
}


