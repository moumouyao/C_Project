#include "myi2c.h"


/**
  * @brief  ����SCLд����
  * @param  None
  * @retval None
  */
	void myi2c_w_scl(uint8_t bitval){
		GPIO_WriteBit(GPIOA, GPIO_Pin_1, (BitAction)bitval);
		Delay_us(10);
	}
	
	/**
  * @brief  ����SDAд����
  * @param  None
  * @retval None
  */
	void myi2c_w_sda(uint8_t bitval){
		GPIO_WriteBit(GPIOA, GPIO_Pin_0, (BitAction)bitval);
		Delay_us(10);
	}
	
	/**
    * @brief  ��ȡSDA����
    * @param  None
    * @retval None
    */
	uint8_t myi2c_r_sda(void){
		return GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0);
	}

/**
  * @brief  ���ģ��I2C��ʼ��
	*		SDA		PA0		�������
	*		SCL		PA1		�������
  * @param  None
  * @retval None
  */
void myi2c_init(void){
	//��ʼ��GPIO��
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
  	GPIO_InitTypeDef GPIO_InitStructure;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	//�ͷ�����
	GPIO_SetBits(GPIOA, GPIO_Pin_0 | GPIO_Pin_1);
}

/**
  * @brief  I2C��ʼ����
  * @param  None
  * @retval None
  */
void i2c_start(void){
	//�����ʼ����
	myi2c_w_sda(1);
	myi2c_w_scl(1);
	
	myi2c_w_sda(0);
	myi2c_w_scl(0);
	
}

/**
  * @brief  I2C��������
  * @param  None
  * @retval None
  */
void i2c_stop(void){
	//�����ʼ����
	myi2c_w_sda(0);
	myi2c_w_scl(1);
	myi2c_w_sda(1);
	
}

/**
* @brief  I2C����һ���ֽ�
  * @param  None
  * @retval None
  */
void myi2c_sendbyte(uint8_t byte){
	for(uint8_t i = 0; i < 8; i++){
		myi2c_w_sda(byte & 0x80 >> i);	//ÿ����һ������ƫ��һ���ֽ�
		myi2c_w_scl(1);
		myi2c_w_scl(0);
	}
}

/**
  * @brief  I2C����һ���ֽ�
  * @param  None
  * @retval None
  */
uint8_t myi2c_recv_byte(void){
	uint8_t byte = 0;
	for(uint8_t i = 0; i < 8; i++){
		myi2c_w_scl(1);
		if(myi2c_r_sda() == 1){byte |= (0x80 >> i);}
			myi2c_w_scl(0);
	}
	return byte;
}

/**
  * @brief  I2C����Ӧ��
  * @param  None
  * @retval None
  */
uint8_t myi2c_recv_ack(void){
	uint8_t ackbit = 0;
	myi2c_w_sda(1);
	myi2c_w_scl(1);
	ackbit = myi2c_r_sda();
	myi2c_w_scl(0);
	return ackbit;
}

/**
* @brief  I2C����Ӧ��
  * @param  None
  * @retval None
  */
void myi2c_send_ack(uint8_t ackbit){
	myi2c_w_sda(ackbit);
	myi2c_w_scl(1);
	myi2c_w_scl(0);
}














