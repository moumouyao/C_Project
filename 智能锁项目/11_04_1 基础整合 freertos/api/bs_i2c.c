#include "bs_i2c.h"



/**
  * @brief  ����SCLд����
  * @param  None
  * @retval None
  */
	void bs_i2c_w_scl(uint8_t bitval){
		GPIO_WriteBit(GPIOB, GPIO_Pin_6, (BitAction)bitval);
		Delay_us(10);
	}
	
	/**
  * @brief  ����SDAд����
  * @param  None
  * @retval None
  */
	void bs_i2c_w_sda(uint8_t bitval){
		GPIO_WriteBit(GPIOB, GPIO_Pin_7, (BitAction)bitval);
		Delay_us(10);
	}
	
	/**
    * @brief  ��ȡSDA����
    * @param  None
    * @retval None
    */
	uint8_t bs_i2c_r_sda(void){
		printf("read_bit %d\r\n", GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_7));
		return GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_7);
	}

	/**
    * @brief  �ж�bs8116busy
    * @param  None
    * @retval None
    */
	uint8_t bs_i2c_busy(void){
		printf("scl = %d\r\n", GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_6));
		return GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_6);
	}
	
	
/**
  * @brief  ���ģ��I2C��ʼ��
	*		SDA		PA0		�������
	*		SCL		PA1		�������
  * @param  None
  * @retval None
  */
void bs_i2c_init(void){
	//��ʼ��GPIO��
	  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
  	GPIO_InitTypeDef GPIO_InitStructure;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
		GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;	//ʱ����
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;	//������
		GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	//�ͷ�����
	GPIO_SetBits(GPIOB, GPIO_Pin_6);
	GPIO_SetBits(GPIOB, GPIO_Pin_7);
}

/**
  * @brief  I2C��ʼ����
  * @param  None
  * @retval None
  */
void bs_i2c_start(void){
	bs_i2c_w_scl(0);
	Delay_us(4);
	//�����ʼ����
	bs_i2c_w_sda(1);
	bs_i2c_w_scl(1);
	
	bs_i2c_w_sda(0);
	bs_i2c_w_scl(0);
	
}

/**
  * @brief  I2C��������
  * @param  None
  * @retval None
  */
void bs_i2c_stop(void){
	//�����ʼ����
	bs_i2c_w_sda(0);
	bs_i2c_w_scl(1);
	bs_i2c_w_sda(1);
	
}

/**
* @brief  I2C����һ���ֽ�
  * @param  None
  * @retval None
  */
void bs_i2c_sendbyte(uint8_t byte){
	for(uint8_t i = 0; i < 8; i++){
	
		bs_i2c_w_scl(1);
		bs_i2c_w_scl(0);
	}
//	Delay_us(10);
}

/**
  * @brief  I2C����һ���ֽ�
  * @param  None
  * @retval None
  */
uint8_t bs_i2c_recv_byte(void){
//	uint8_t byte = 0;
//	for(uint8_t i = 0; i < 8; i++){
//		bs_i2c_w_scl(1);
//		if(bs_i2c_r_sda() == 1){byte |= (0x80 >> i);}
//			bs_i2c_w_scl(0);
//	}
	
	uint8_t i=0;
	uint8_t data=0;
	
	for(i=0;i<8;i++)
	{
		bs_i2c_w_scl(0);//���ӻ�׼�����ݣ�
		bs_i2c_w_sda(1);//��ģʽ-----�������·��ܽŶϣ���������������������
//		Delay_us(6);//��ʱ����ʱ��ӻ�׼�����ݲ��������ȶ����������ϣ�
		bs_i2c_w_scl(1);
		data = (data<<1) | (bs_i2c_r_sda());
		Delay_us(6);//��ʱ (��ʱ��������ȡ����)
		
	}
	
	return data;
}

/**
  * @brief  I2C����Ӧ��
  * @param  None
  * @retval None
  */
uint8_t bs_i2c_recv_ack(void){
	uint8_t ackbit = 0;
	bs_i2c_w_sda(1);
	bs_i2c_w_scl(1);
	ackbit = bs_i2c_r_sda();
//	bs_i2c_w_scl(1);
	return ackbit;
}

/**
* @brief  I2C����Ӧ��
  * @param  None
  * @retval None
  */
void bs_i2c_send_ack(uint8_t ackbit){
	bs_i2c_w_sda(ackbit);
	bs_i2c_w_scl(1);
	bs_i2c_w_scl(0);
}
