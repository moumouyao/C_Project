#include "LU90614.h"




//�¶ȴ�������ʼ��
void LU90614_init(void){
	Uart2Init();
}

//��ȡ����
char read_body_temperature(void){
	unsigned char wait_time = 0, check = 0, cnt;
	unsigned char read_cmd[6] = {0xFA, 0xC5, 0xBF, 0xFA, 0xCA, 0xC4};	//��ȡ����ָ��
	
	Uart2_SendString(read_cmd, 6);	//��������
	//�ȴ��������ظ�
	while(!UART2_RecvOver){
		Delay_ms(1);
		wait_time++;
		if(wait_time > 100){
			return 1;
		}
	}
	if(UART2_RecvCnt != 9){	//���ݸ�������
		UART2_RecvCnt = 0;
		UART2_RecvTick = 0;
		UART2_RecvOver = 0;
		return -1;
	}
	
	for( cnt = 0; cnt<UART2_RecvCnt-1; cnt++) {
			check += UART2_RecvBuff[cnt];
		}
	if(check != UART2_RecvBuff[UART2_RecvCnt-1]){	//����У�����
		UART2_RecvCnt = 0;
		UART2_RecvTick = 0;
		UART2_RecvOver = 0;
		return -1;
	}
	printf("���� %0.2f\r\n", UART2_RecvBuff[2] + UART2_RecvBuff[3]/100.0);
		UART2_RecvCnt = 0;
		UART2_RecvTick = 0;
		UART2_RecvOver = 0;
	return UART2_RecvBuff[2] + UART2_RecvBuff[3]/100.0;
		
	
}




