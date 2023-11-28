#include "bs_i2c.h"



/**
  * @brief  定义SCL写函数
  * @param  None
  * @retval None
  */
	void bs_i2c_w_scl(uint8_t bitval){
		GPIO_WriteBit(GPIOB, GPIO_Pin_6, (BitAction)bitval);
		Delay_us(10);
	}
	
	/**
  * @brief  定义SDA写函数
  * @param  None
  * @retval None
  */
	void bs_i2c_w_sda(uint8_t bitval){
		GPIO_WriteBit(GPIOB, GPIO_Pin_7, (BitAction)bitval);
		Delay_us(10);
	}
	
	/**
    * @brief  读取SDA数据
    * @param  None
    * @retval None
    */
	uint8_t bs_i2c_r_sda(void){
		printf("read_bit %d\r\n", GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_7));
		return GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_7);
	}

	/**
    * @brief  判断bs8116busy
    * @param  None
    * @retval None
    */
	uint8_t bs_i2c_busy(void){
		printf("scl = %d\r\n", GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_6));
		return GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_6);
	}
	
	
/**
  * @brief  软件模拟I2C初始化
	*		SDA		PA0		推挽输出
	*		SCL		PA1		推挽输出
  * @param  None
  * @retval None
  */
void bs_i2c_init(void){
	//初始化GPIO口
	  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
  	GPIO_InitTypeDef GPIO_InitStructure;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
		GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;	//时钟线
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;	//数据线
		GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	//释放总线
	GPIO_SetBits(GPIOB, GPIO_Pin_6);
	GPIO_SetBits(GPIOB, GPIO_Pin_7);
}

/**
  * @brief  I2C起始条件
  * @param  None
  * @retval None
  */
void bs_i2c_start(void){
	bs_i2c_w_scl(0);
	Delay_us(4);
	//输出起始条件
	bs_i2c_w_sda(1);
	bs_i2c_w_scl(1);
	
	bs_i2c_w_sda(0);
	bs_i2c_w_scl(0);
	
}

/**
  * @brief  I2C结束条件
  * @param  None
  * @retval None
  */
void bs_i2c_stop(void){
	//输出起始条件
	bs_i2c_w_sda(0);
	bs_i2c_w_scl(1);
	bs_i2c_w_sda(1);
	
}

/**
* @brief  I2C发送一个字节
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
  * @brief  I2C接收一个字节
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
		bs_i2c_w_scl(0);//（从机准备数据）
		bs_i2c_w_sda(1);//读模式-----让输出电路与管脚断！！开！！！！！！！！
//		Delay_us(6);//延时（给时间从机准备数据并且数据稳定在数据线上）
		bs_i2c_w_scl(1);
		data = (data<<1) | (bs_i2c_r_sda());
		Delay_us(6);//延时 (给时间主机读取数据)
		
	}
	
	return data;
}

/**
  * @brief  I2C接收应答
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
* @brief  I2C发送应答
  * @param  None
  * @retval None
  */
void bs_i2c_send_ack(uint8_t ackbit){
	bs_i2c_w_sda(ackbit);
	bs_i2c_w_scl(1);
	bs_i2c_w_scl(0);
}
