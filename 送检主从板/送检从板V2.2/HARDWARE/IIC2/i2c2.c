#include "i2c2.h"
#include <stdio.h>
#include "stm32f4xx.h" 

#include "Delay.h"

#include "FreeRTOS.h"
#include "task.h"

#define SHT20_ADDRESS		0x40
#define CRC_MODEL  0x131



/**
  ******************************************************************************
  *                            User_I2C2Config()
  *
  * ����   : ��ʼ��I2C2
  *		   
  * ����   : ��
  *
  *
  * ����ֵ : ��
  ******************************************************************************
  */

void User_I2C2Config (void)
{
  I2C_InitTypeDef  I2C_InitStructure;
  GPIO_InitTypeDef  GPIO_InitStructure; 

  RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C2,ENABLE);
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOH,ENABLE);
    

  /* PB6,7 SCL and SDA */
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_4 | GPIO_Pin_5;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType=GPIO_OType_OD;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF; //���ùܽ�Ϊ���ù��ܿ�©���
  GPIO_Init(GPIOH, &GPIO_InitStructure);          //I2C�ӿ�ʹ�õ�GPIO�ܽų�ʼ��


  GPIO_PinAFConfig(GPIOH,GPIO_PinSource4,GPIO_AF_I2C2);//GPIOB0����Ϊ��ʱ��3
	GPIO_PinAFConfig(GPIOH,GPIO_PinSource5,GPIO_AF_I2C2);//GPIOB1����Ϊ��ʱ��3
	
  I2C_DeInit(I2C2);	//��λI2C2
  I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;         //����I2C�ӿ�ģʽ 
  I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2; //����I2C�ӿڵĸߵ͵�ƽ����
  I2C_InitStructure.I2C_OwnAddress1 = 0x30;          //����I2C�ӿڵ�������ַ
  I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;        //�����Ƿ���ACK��Ӧ          
  I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
  I2C_InitStructure.I2C_ClockSpeed = 200000;        //100K�ٶ�
    
  I2C_Cmd(I2C2, ENABLE);                            //ʹ��I2C�ӿ�
  I2C_Init(I2C2, &I2C_InitStructure);               //I2C�ӿڳ�ʼ��
  /*����1�ֽ�1Ӧ��ģʽ*/
  I2C_AcknowledgeConfig(I2C2, ENABLE);              //ʹ��I2C�ӿ���Ӧ
}

/**
  ******************************************************************************
  *                            I2C_ByteWrite_24C()
  *
  * ����   : ��24C��addr��ַ��дһ�ֽ�����dat
  *		   
  * ����   : addr--24C�е�ĳһ��ַ
  *          dat--Ҫд�������
  *
  *
  * ����ֵ : ��
  * ע�����Ҫ����æ�ж�
  ******************************************************************************
  */

void I2C2_ByteWrite_SHT20(uint8_t addr,uint8_t dat)
{
  /* ��ʼλ */
  I2C_GenerateSTART(I2C2, ENABLE);
  while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT));  

  /* ���͵�ַ */
  I2C_SendData(I2C2, addr);
  while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED));

  /* дһ���ֽ� */
  I2C_SendData(I2C2, dat); 
  while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
	
  /* ֹͣλ */
  I2C_GenerateSTOP(I2C2, ENABLE);
  
}


/**
  ******************************************************************************
  *                            I2C_ReadS_24C()
  *
  * ����   : ͨ��I2C2������24C�е�����
  *		   
  * ����   : addr --Ҫ��ȡ24C�е����ݵĵ�ַ��
  *          pBuffer--���ݴ�ŵĵ�ַ
  *          no--Ҫ��ȡ���ݵĸ���
  *
  *
  * ����ֵ : ��
  ******************************************************************************
  */

void I2C2_Read_SHT20(uint8_t addr ,uint8_t* pBuffer,uint16_t no)
{
  if(no==0)	return;
	
  while(I2C_GetFlagStatus(I2C2, I2C_FLAG_BUSY));
		
  /* ����1�ֽ�1Ӧ��ģʽ */
  I2C_AcknowledgeConfig(I2C2, ENABLE);

  /* ������ʼλ */
  I2C_GenerateSTART(I2C2, ENABLE);
  while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT));/*EV5,��ģʽ*/

  /* ����������ַ(��)*/
  I2C_Send7bitAddress(I2C2, addr<<1, I2C_Direction_Receiver);
  while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));

  while (no)
  {
    if(no==1)
	{
      I2C_AcknowledgeConfig(I2C2, DISABLE);	//���һλ��Ҫ�ر�Ӧ���
      I2C_GenerateSTOP(I2C2, ENABLE);			//����ֹͣλ
	}
	while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_RECEIVED)); /* EV7 */
	*pBuffer = I2C_ReceiveData(I2C2);
	pBuffer++;
	/* Decrement the read bytes counter */
	no--;
  }
  /* �ٴ�����Ӧ��ģʽ */
  I2C_AcknowledgeConfig(I2C2, ENABLE);
}

/**
  ******************************************************************************
  *                            SHT20_Get_Data()
  *
  * ����   : ͨ�������ȡ��ʪ��
  *		   
  * ����   : cmd
  *          		SHT20_CMD_MEASURE_TEMP	��ȡ�¶�����
  *          		SHT20_CMD_MEASURE_HUMI	��ȡʪ������
  *
  *
  * ����ֵ : ��ʪ������
  ******************************************************************************
  */
uint8_t aaa = 0;
uint16_t SHT20_Get_Data(uint8_t cmd){
	uint16_t data;
	uint8_t addr, checksum, buf[2];
	
	addr = SHT20_ADDRESS;
	
	//ȷ�����߲�æ
	while(I2C_GetFlagStatus(I2C2, I2C_FLAG_BUSY)){};
	
	/* ������ʼλ */
  I2C_GenerateSTART(I2C2, ENABLE);
  while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT));/*EV5,��ģʽ*/

  /* ����������ַ(д)*/
  I2C_Send7bitAddress(I2C2, addr<<1, I2C_Direction_Transmitter);
  while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));
	
	/*��������*/
	I2C_SendData(I2C2, cmd);
  while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
		
	//�ȴ��������
//	delay_ms(70);	
		delay_sys_ms(70);
		
	/* ���� */
  I2C_GenerateSTART(I2C2, ENABLE);
  while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT));/*EV5,��ģʽ*/
	
	/* ����������ַ(��)*/
  I2C_Send7bitAddress(I2C2, addr<<1 | 0x01, I2C_Direction_Receiver);
  while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));
	

	
	/*��������*/
	I2C_AcknowledgeConfig(I2C2, ENABLE);
	while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_RECEIVED)); /* EV7 */
	buf[0] = I2C_ReceiveData(I2C2);
	
	I2C_AcknowledgeConfig(I2C2, ENABLE);
	while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_RECEIVED)); /* EV7 */
	buf[1] = I2C_ReceiveData(I2C2);
	
	I2C_AcknowledgeConfig(I2C2, ENABLE);
	while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_RECEIVED)); /* EV7 */
	checksum = I2C_ReceiveData(I2C2);
	I2C_AcknowledgeConfig(I2C2, DISABLE);	//���һλ��Ҫ�ر�Ӧ���
	
  I2C_GenerateSTOP(I2C2, ENABLE);			//����ֹͣλ
	
	data = (buf[0] << 8) + buf[1];
	
	if(CRC_Check(buf, 2, checksum) == 0)//У��ͨ��
	{
		aaa = 1;
		data = data & 0xFFFC;
		if(cmd == SHT20_CMD_MEASURE_TEMP)
			data = (175.72 * data / 65536 - 46.85);
		else
			data = (125.0 * data / 65536 - 6.0);
		return data;
	}
	else
		aaa = 0;
	return 0xffff;
}

/**
  ******************************************************************************
  *                            CRC_Check()
  *
  * ����   : ��ʪ�����ݵ�CRCУ��
  *		   
  * ����   : ptr	ҪУ�������
  *          len	���ݳ���
  *          checksum	Ԥ��У��ֵ
  *
  *
  * ����ֵ : У��ͨ�� 0			У��ʧ�� 1
  ******************************************************************************
  */
uint8_t CRC_Check(uint8_t *ptr, uint8_t len, uint8_t checksum){
	uint8_t i;
	uint8_t crc = 0x00;
	while(len--){
		crc ^= *ptr++;
		for(i = 8; i > 0; i--){
			if(crc & 0x80){
				crc = (crc << 1) ^ CRC_MODEL;
			}
			else
				crc = (crc << 1);
		}
	}
	if(checksum == crc)
		return 0;
	else
		return 1;
		
}

