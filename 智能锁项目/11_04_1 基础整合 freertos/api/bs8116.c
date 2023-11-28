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
  * @brief  ����SCLд����
  * @param  None
  * @retval None
  */
	void i2c_w_scl(uint8_t bitval){
		GPIO_WriteBit(GPIOB, GPIO_Pin_6, (BitAction)bitval);
		
	}
		/**
    * @brief  ��ȡSCL����
    * @param  None
    * @retval None
    */
	uint8_t i2c_r_scl(void)
	{
		return GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_6);
	}
	
	
	/**
  * @brief  ����SDAд����
  * @param  None
  * @retval None
  */
	void i2c_w_sda(uint8_t bitval){
		GPIO_WriteBit(GPIOB, GPIO_Pin_7, (BitAction)bitval);
	
	}
	
	/**
    * @brief  ��ȡSDA����
    * @param  None
    * @retval None
    */
	uint8_t i2c_r_sda(void){
		return GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_7);
	}

/**
  * @brief  ���ģ��I2C��ʼ��
	*		SDA		PB7		�������
	*		SCL		PB6		��©���
  * @param  None
  * @retval None
  */
void i2c_init(void){
	//��ʼ��GPIO��
	  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
  	GPIO_InitTypeDef GPIO_InitStructure;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
		GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;  //��©���
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;	//ʱ����
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  	GPIO_Init(GPIOB, &GPIO_InitStructure);
	  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;   //�������
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
void iic_start(void){
	i2c_w_scl(0);//�����ʼ����
	Delay_us(10);
	i2c_w_sda(1);
	i2c_w_scl(1);
	Delay_us(10);
	i2c_w_sda(0);
	Delay_us(10);
	i2c_w_scl(0);
	
}

/**
  * @brief  I2C��������
  * @param  None
  * @retval None
  */
void iic_stop(void){
	//�����ʼ����
	i2c_w_sda(0);
	i2c_w_scl(1);
	Delay_us(10);
	i2c_w_sda(1);
	Delay_us(10);
	
}

/**
* @brief  I2C����һ���ֽ�
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
  * @brief  I2C����һ���ֽ�
  * @param  None
  * @retval None
  */


uint8_t iic_read_byte(uint8_t ack)/*��ȡ1�ֽ����ݣ�1:ack;0:nack*/
{
		uint8_t i=0;
	uint8_t data=0;
	for(i=0;i<8;i++)
	{
	i2c_w_scl(0);//���ӻ�׼�����ݣ�
	i2c_w_sda(1);//��ģʽ-----�������·��ܽŶϿ���������������������
	 Delay_us(6);//��ʱ����ʱ��ӻ�׼�����ݲ��������ȶ����������ϣ�
	i2c_w_scl(1);
	data<<=1;//�ճ����λ���洢���ζ�ȡ��
	if(i2c_r_sda())//������ȡ SDA ���ϵ�����
	data |=1;
	 Delay_us(6);//��ʱ (��ʱ��������ȡ����)
	}
//	i2c_w_scl(0);//��������Ĳ�������ֹ���������ֹͣ����
    if(ack)iic_nack();
    else iic_ack();
    return data;
}


void iic_ack(void)/*��������Ӧ���źŸ��ӻ�����һ��ʱ������������SDA*/
{
    i2c_w_scl(0);
     Delay_us(6);
    i2c_w_sda(0);/*������Ϊ�͵�ƽ����ʾӦ��*/
     Delay_us(6);
    i2c_w_scl(1);
     Delay_us(6);
}
 
void iic_nack(void)/*�������ͷ�Ӧ���źŸ��ӻ�����һ��ʱ�������ڲ�����SDA*/
{
    i2c_w_scl(0);
     Delay_us(6);
    i2c_w_sda(1);/*������Ϊ�ߵ�ƽ����ʾ��Ӧ��*/
     Delay_us(6);
    i2c_w_scl(1);
     Delay_us(6);
}






//	IIC1_SCL=0;//��������Ĳ�������ֹ���������ֹͣ����
void BS8116_init(void)
{
 
	  i2c_init();
	  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);//ʹ��GPIOAʱ��
	  GPIO_InitTypeDef  GPIO_InitStructure;
	  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;//LED0��LED1��ӦIO��
	  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//��ͨ���ģʽ
	  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
	  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;//����
	  GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIO
	  //GPIO_SetBits(GPIOA,GPIO_Pin_1);//���øߣ�����

}


uint8_t BS8116ReadKey(void) 
{
	u32 data=0;
	iic_start();
	
	if(!I2C_WriteByte(BS8116_ADDR_W))
	{
		printf("����������ַʧ��\r\n");
		 iic_stop();
	} 
	else 
		printf("���ͳɹ�\r\n");
	Delay_us(2);
	
	while (i2c_r_scl()==0);  //�ȴ�SCL�ߵ�ƽ
	
		if(!I2C_WriteByte(0x08))
	{
		printf("����0X08ʧ��\r\n");
		 iic_stop();
	} 
	else 
		printf("����0X08�ɹ�\r\n");
	Delay_us(4);
	while (i2c_r_scl()==0);  //�ȴ�SCL�ߵ�ƽ
   iic_start();
		if(!I2C_WriteByte(BS8116_ADDR_R))
	{
		printf("����BS8116_ADDR_Rʧ��\r\n");
		 iic_stop();
	}
    else 
		printf("����BS8116_ADDR_R�ɹ�\r\n");	
	Delay_us(4);
	while (i2c_r_scl()==0);  //�ȴ�SCL�ߵ�ƽ

	data =iic_read_byte(0);
	printf("data %x",data);
	data |=iic_read_byte(1)<<8;
	
	iic_stop();
    printf("���յ������� %x \r\n",data);
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
