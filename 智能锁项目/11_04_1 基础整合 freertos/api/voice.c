#include "voice.h"

/**
  * @brief  ��ʼ������ģ��
  * @param  None
  * @retval None
  */
void voice_init(void){
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_4;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);			//Pc5�������
	
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
//	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
//	GPIO_Init(GPIOC, &GPIO_InitStructure);		//PC4��������
//	GPIO_ResetBits(GPIOC, GPIO_Pin_5);
}

//����1��0
void send_voi_data(uint8_t data){
	if(data > 0){
		GPIO_SetBits(GPIOC, GPIO_Pin_5);
		delay_us(1500);
		GPIO_ResetBits(GPIOC, GPIO_Pin_5);
		delay_us(500);
	}
	else if(data == 0){
		
		
		GPIO_SetBits(GPIOC, GPIO_Pin_5);
		delay_us(500);
		GPIO_ResetBits(GPIOC, GPIO_Pin_5);
		delay_us(1500);
	}
}


/**
	* @brief  ������ģ�鷢��ָ��
  * @param  None
  * @retval None
  */
void send_voi_cmd(uint8_t cmd){
//	GPIO_ResetBits(GPIOC, GPIO_Pin_4);
	Delay_ms(5);
	//����ͬ��ͷ
	GPIO_SetBits(GPIOC, GPIO_Pin_5);
	Delay_ms(8);
	GPIO_ResetBits(GPIOC, GPIO_Pin_5);
	Delay_ms(1);
	for(uint8_t i = 0; i < 8; i++){
//		printf("%d\r\n", cmd & (0x80 >> i));
			send_voi_data(cmd & (0x80 >> i));
	}
	GPIO_ResetBits(GPIOC, GPIO_Pin_5);
	
}


