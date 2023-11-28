#include "myi2c.h"


/**
  * @brief  定义SCL写函数
  * @param  None
  * @retval None
  */
	void myi2c_w_scl(uint8_t bitval){
		GPIO_WriteBit(GPIOA, GPIO_Pin_1, (BitAction)bitval);
		Delay_us(10);
	}
	
	/**
  * @brief  定义SDA写函数
  * @param  None
  * @retval None
  */
	void myi2c_w_sda(uint8_t bitval){
		GPIO_WriteBit(GPIOA, GPIO_Pin_0, (BitAction)bitval);
		Delay_us(10);
	}
	
	/**
    * @brief  读取SDA数据
    * @param  None
    * @retval None
    */
	uint8_t myi2c_r_sda(void){
		return GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0);
	}

/**
  * @brief  软件模拟I2C初始化
	*		SDA		PA0		推挽输出
	*		SCL		PA1		推挽输出
  * @param  None
  * @retval None
  */
void myi2c_init(void){
	//初始化GPIO口
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
  	GPIO_InitTypeDef GPIO_InitStructure;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	//释放总线
	GPIO_SetBits(GPIOA, GPIO_Pin_0 | GPIO_Pin_1);
}

/**
  * @brief  I2C起始条件
  * @param  None
  * @retval None
  */
void i2c_start(void){
	//输出起始条件
	myi2c_w_sda(1);
	myi2c_w_scl(1);
	
	myi2c_w_sda(0);
	myi2c_w_scl(0);
	
}

/**
  * @brief  I2C结束条件
  * @param  None
  * @retval None
  */
void i2c_stop(void){
	//输出起始条件
	myi2c_w_sda(0);
	myi2c_w_scl(1);
	myi2c_w_sda(1);
	
}

/**
* @brief  I2C发送一个字节
  * @param  None
  * @retval None
  */
void myi2c_sendbyte(uint8_t byte){
	for(uint8_t i = 0; i < 8; i++){
		myi2c_w_sda(byte & 0x80 >> i);	//每发送一次向右偏移一个字节
		myi2c_w_scl(1);
		myi2c_w_scl(0);
	}
}

/**
  * @brief  I2C接收一个字节
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
  * @brief  I2C接收应答
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
* @brief  I2C发送应答
  * @param  None
  * @retval None
  */
void myi2c_send_ack(uint8_t ackbit){
	myi2c_w_sda(ackbit);
	myi2c_w_scl(1);
	myi2c_w_scl(0);
}














