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
 * ����������ǹ���Դ���
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
		if(key == 1) { //���²������� ��ȡ�¶�ֵ
			now_temp = SMBus_ReadTemp();
			printf("%0.2f\r\n", now_temp);
			
		}
		if(Led_TaskTime[0] > Led_TaskTime[1]) {
			LED ^= 1;
			Led_TaskTime[0] = 0;
		}
	}
}


