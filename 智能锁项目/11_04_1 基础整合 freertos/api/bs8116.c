#include "stm32f4xx.h"                  // Device header
#include "usart.h"
#include "delay.h"
#include "bs8116.h"
#define BS8116_ADDR_W   0xA0
#define BS8116_ADDR_R    0xA1
#define I2C_SCL_PIN      GPIO_Pin_6
#define I2C_SDA_PIN      GPIO_Pin_7
#define I2C_GPIO_PORT    GPIOB
/**
  * @brief  定义SCL写函数
  * @param  None
  * @retval None
  */
	void i2c_w_scl(uint8_t bitval){
		GPIO_WriteBit(GPIOB, GPIO_Pin_6, (BitAction)bitval);
		
	}
		/**
    * @brief  读取SCL数据
    * @param  None
    * @retval None
    */
	uint8_t i2c_r_scl(void)
	{
		return GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_6);
	}
	
	
	/**
  * @brief  定义SDA写函数
  * @param  None
  * @retval None
  */
	void i2c_w_sda(uint8_t bitval){
		GPIO_WriteBit(GPIOB, GPIO_Pin_7, (BitAction)bitval);
	
	}
	
	/**
    * @brief  读取SDA数据
    * @param  None
    * @retval None
    */
	uint8_t i2c_r_sda(void){
		return GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_7);
	}

/**
  * @brief  软件模拟I2C初始化
	*		SDA		PB7		推挽输出
	*		SCL		PB6		开漏输出
  * @param  None
  * @retval None
  */
void i2c_init(void){
	//初始化GPIO口
	  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
  	GPIO_InitTypeDef GPIO_InitStructure;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
		GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;  //开漏输出
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;	//时钟线
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  	GPIO_Init(GPIOB, &GPIO_InitStructure);
	  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;   //推挽输出
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
void iic_start(void){
	i2c_w_scl(0);//输出起始条件
	Delay_us(10);
	i2c_w_sda(1);
	i2c_w_scl(1);
	Delay_us(10);
	i2c_w_sda(0);
	Delay_us(10);
	i2c_w_scl(0);
	
}

/**
  * @brief  I2C结束条件
  * @param  None
  * @retval None
  */
void iic_stop(void){
	//输出起始条件
	i2c_w_sda(0);
	i2c_w_scl(1);
	Delay_us(10);
	i2c_w_sda(1);
	Delay_us(10);
	
}

/**
* @brief  I2C发送一个字节
  * @param  None
  * @retval None
  */
uint8_t I2C_WriteByte(uint8_t byte) {

    uint8_t i, ack;
	i2c_w_scl(0);
    for(i = 0; i < 8; i++) {
        if(byte & 0x80) {
				i2c_w_sda(1);
        } else {
            	i2c_w_sda(0);
        }
        byte <<= 1;
        Delay_us(10);
        i2c_w_scl(1);
        Delay_us(10);
        i2c_w_scl(0);
        Delay_us(10);
    }
    i2c_w_sda(1);
    Delay_us(10);
    i2c_w_scl(1);
    Delay_us(10);

    if(GPIO_ReadInputDataBit(I2C_GPIO_PORT, I2C_SDA_PIN)) {
        ack = 0;
    } else {
        ack = 1;
    }

    
    Delay_ms(10);

    return ack;
}


/**
  * @brief  I2C接收一个字节
  * @param  None
  * @retval None
  */


uint8_t iic_read_byte(uint8_t ack)/*读取1字节数据：1:ack;0:nack*/
{
		uint8_t i=0;
	uint8_t data=0;
	for(i=0;i<8;i++)
	{
	i2c_w_scl(0);//（从机准备数据）
	i2c_w_sda(1);//读模式-----让输出电路与管脚断开！！！！！！！！！！
	 Delay_us(6);//延时（给时间从机准备数据并且数据稳定在数据线上）
	i2c_w_scl(1);
	data<<=1;//空出最低位来存储本次读取的
	if(i2c_r_sda())//主机读取 SDA 线上的数据
	data |=1;
	 Delay_us(6);//延时 (给时间主机读取数据)
	}
//	i2c_w_scl(0);//方便后续的操作；防止意外产生了停止条件
    if(ack)iic_nack();
    else iic_ack();
    return data;
}


void iic_ack(void)/*主机发送应答信号给从机，在一个时钟周期内拉低SDA*/
{
    i2c_w_scl(0);
     Delay_us(6);
    i2c_w_sda(0);/*数据线为低电平，表示应答*/
     Delay_us(6);
    i2c_w_scl(1);
     Delay_us(6);
}
 
void iic_nack(void)/*主机发送非应答信号给从机，在一个时钟周期内不操作SDA*/
{
    i2c_w_scl(0);
     Delay_us(6);
    i2c_w_sda(1);/*数据线为高电平，表示非应答*/
     Delay_us(6);
    i2c_w_scl(1);
     Delay_us(6);
}






//	IIC1_SCL=0;//方便后续的操作；防止意外产生了停止条件
void BS8116_init(void)
{
 
	  i2c_init();
	  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);//使能GPIOA时钟
	  GPIO_InitTypeDef  GPIO_InitStructure;
	  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;//LED0和LED1对应IO口
	  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//普通输出模式
	  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
	  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;//上拉
	  GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIO
	  //GPIO_SetBits(GPIOA,GPIO_Pin_1);//设置高，灯灭

}


uint8_t BS8116ReadKey(void) 
{
	u32 data=0;
	iic_start();
	
	if(!I2C_WriteByte(BS8116_ADDR_W))
	{
		printf("发送器件地址失败\r\n");
		 iic_stop();
	} 
	else 
		printf("发送成功\r\n");
	Delay_us(2);
	
	while (i2c_r_scl()==0);  //等待SCL高电平
	
		if(!I2C_WriteByte(0x08))
	{
		printf("发送0X08失败\r\n");
		 iic_stop();
	} 
	else 
		printf("发送0X08成功\r\n");
	Delay_us(4);
	while (i2c_r_scl()==0);  //等待SCL高电平
   iic_start();
		if(!I2C_WriteByte(BS8116_ADDR_R))
	{
		printf("发送BS8116_ADDR_R失败\r\n");
		 iic_stop();
	}
    else 
		printf("发送BS8116_ADDR_R成功\r\n");	
	Delay_us(4);
	while (i2c_r_scl()==0);  //等待SCL高电平

	data =iic_read_byte(0);
	printf("data %x",data);
	data |=iic_read_byte(1)<<8;
	
	iic_stop();
    printf("接收到的数据 %x \r\n",data);
	switch(data)
	{
		
	case 0X8081:return '1';
	case 0X8480:return '2';
	case 0X8080:return '3';
	case 0X8082:return '4';
	case 0X8880:return '5';
	case 0X80C0:return '6';
	case 0X8088:return '7';
	case 0X8180:return '8';
	case 0X80A0:return '9';
	case 0X8084:return '*';
	case 0X8280:return '0';
	case 0X8090:return '#';
	default : break;
	}
	return 0;

		
}
