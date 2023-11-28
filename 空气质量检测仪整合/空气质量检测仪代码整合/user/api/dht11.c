#include "dht11.h"

DHT dht_data;

/**
  * @brief  ��ʼ��dht11
  * @param  None
  * @retval None
  */
	void dth11_init(void){
			//��ʼ��PG11�˿�Ϊ��©ģʽ
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOG, ENABLE);
    	GPIO_InitTypeDef GPIO_InitStructure;
    	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
    	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
    	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    	GPIO_Init(GPIOG, &GPIO_InitStructure);
		//�ͷ�������     
		GPIO_SetBits(GPIOG,  GPIO_Pin_11);
	}
	
	/**
    * @brief  ��dth11ͨ��
    * @param  None
    * @retval uint8_t
		*		1		�ȴ�dht��Ӧ�͵�ƽʧ��
		*		2		�ȴ�dht��Ӧ�ߵ�ƽʧ��
		*		3		�ȴ�dht����λ�͵�ƽʧ��
		*		4		�ȴ�����λ�ߵ�ƽʧ��
		*		5		����У��ɹ�
		*		6		����У��ʧ��
    */
	uint8_t dht_get_val(void){
		uint8_t time_cnt = 0, i = 0, j = 0;
		uint16_t	dht_sum = 0;
		uint8_t dht_buf[5] = {0};
		//�������Ϳ�ʼ�ź�
		GPIO_ResetBits(GPIOG,  GPIO_Pin_11);
		Delay_ms(20);
		GPIO_SetBits(GPIOG,  GPIO_Pin_11);
		//�ȴ�dht��Ӧ�͵�ƽ
		while(GPIO_ReadInputDataBit(GPIOG, GPIO_Pin_11) == Bit_SET){
			time_cnt++;
			Delay_us(1);
			if(time_cnt > 100){	//
				return 1;	//�ȴ�dht��Ӧ�͵�ƽʧ��
			}
		}
		//�ȴ�dht��Ӧ�ߵ�ƽ
		time_cnt = 0;
		while(GPIO_ReadInputDataBit(GPIOG, GPIO_Pin_11) == RESET){
			time_cnt++;
			Delay_us(1);
			if(time_cnt > 100){
				return 2;	//�ȴ�dht��Ӧ�ߵ�ƽʧ��
			}
		}
		for(i = 0; i < 5; i++){
			for(j = 0; j < 8; j++){
				//���ݽ��� �ȴ�dht����λ�͵�ƽ
				time_cnt = 0;
				while(GPIO_ReadInputDataBit(GPIOG, GPIO_Pin_11) == SET){
					time_cnt++;
					Delay_us(1);
					if(time_cnt > 100){
						return 3;	
					}
				}
				//�ȴ�����λ�ߵ�ƽ
				time_cnt = 0;
				while(GPIO_ReadInputDataBit(GPIOG, GPIO_Pin_11) == RESET){
					time_cnt++;
					Delay_us(1);
					if(time_cnt > 100){
						return 4;	
					}
				}
				//�ж�����λ
				Delay_us(30);
				if(GPIO_ReadInputDataBit(GPIOG, GPIO_Pin_11) == RESET){//���յ�0
					dht_buf[i] &= ~(1 << (7 - j)); 
				}
				else{//���յ�1
					dht_buf[i] |= (1 << (7 - j)); 
				}
		}
			if(i != 4)
				dht_sum = dht_buf[i] + dht_sum;	
	}
		if(dht_sum == dht_buf[4]){
			if((dht_buf[3] | 0x80) == 1){		//1000 0000	����
				dht_data.tmp_flag = 1;
			}
			else{
				dht_data.tmp_flag = 0;
			}
			dht_data.tmp_flo = dht_buf[3] &  0x7f;
			dht_data.hum = dht_buf[0];
			dht_data.tmp = dht_buf[2];
		//	printf("tem = %d.%d\thum = %d\n",dht_data.tmp, dht_data.tmp_flo, dht_data.hum);
		//	printf("flag = %d\n", dht_data.tmp_flag);
			dht_data.hum = dht_buf[0];
			return 5;
			
		}
		else{
			return 6;
		}
	}

	
	
	