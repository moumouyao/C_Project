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
  * 描述   : 初始化I2C2
  *		   
  * 参数   : 无
  *
  *
  * 返回值 : 无
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
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF; //设置管脚为复用功能开漏输出
  GPIO_Init(GPIOH, &GPIO_InitStructure);          //I2C接口使用的GPIO管脚初始化


  GPIO_PinAFConfig(GPIOH,GPIO_PinSource4,GPIO_AF_I2C2);//GPIOB0复用为定时器3
	GPIO_PinAFConfig(GPIOH,GPIO_PinSource5,GPIO_AF_I2C2);//GPIOB1复用为定时器3
	
  I2C_DeInit(I2C2);	//复位I2C2
  I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;         //设置I2C接口模式 
  I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2; //设置I2C接口的高低电平周期
  I2C_InitStructure.I2C_OwnAddress1 = 0x30;          //设置I2C接口的主机地址
  I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;        //设置是否开启ACK响应          
  I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
  I2C_InitStructure.I2C_ClockSpeed = 200000;        //100K速度
    
  I2C_Cmd(I2C2, ENABLE);                            //使能I2C接口
  I2C_Init(I2C2, &I2C_InitStructure);               //I2C接口初始化
  /*允许1字节1应答模式*/
  I2C_AcknowledgeConfig(I2C2, ENABLE);              //使能I2C接口响应
}

/**
  ******************************************************************************
  *                            I2C_ByteWrite_24C()
  *
  * 描述   : 往24C中addr地址上写一字节数据dat
  *		   
  * 参数   : addr--24C中的某一地址
  *          dat--要写入的数据
  *
  *
  * 返回值 : 无
  * 注意事项：要调用忙判断
  ******************************************************************************
  */

void I2C2_ByteWrite_SHT20(uint8_t addr,uint8_t dat)
{
  /* 起始位 */
  I2C_GenerateSTART(I2C2, ENABLE);
  while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT));  

  /* 发送地址 */
  I2C_SendData(I2C2, addr);
  while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED));

  /* 写一个字节 */
  I2C_SendData(I2C2, dat); 
  while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
	
  /* 停止位 */
  I2C_GenerateSTOP(I2C2, ENABLE);
  
}


/**
  ******************************************************************************
  *                            I2C_ReadS_24C()
  *
  * 描述   : 通过I2C2连续读24C中的数据
  *		   
  * 参数   : addr --要读取24C中的数据的地址；
  *          pBuffer--数据存放的地址
  *          no--要读取数据的个数
  *
  *
  * 返回值 : 无
  ******************************************************************************
  */

void I2C2_Read_SHT20(uint8_t addr ,uint8_t* pBuffer,uint16_t no)
{
  if(no==0)	return;
	
  while(I2C_GetFlagStatus(I2C2, I2C_FLAG_BUSY));
		
  /* 允许1字节1应答模式 */
  I2C_AcknowledgeConfig(I2C2, ENABLE);

  /* 发送起始位 */
  I2C_GenerateSTART(I2C2, ENABLE);
  while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT));/*EV5,主模式*/

  /* 发送器件地址(读)*/
  I2C_Send7bitAddress(I2C2, addr<<1, I2C_Direction_Receiver);
  while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));

  while (no)
  {
    if(no==1)
	{
      I2C_AcknowledgeConfig(I2C2, DISABLE);	//最后一位后要关闭应答的
      I2C_GenerateSTOP(I2C2, ENABLE);			//发送停止位
	}
	while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_RECEIVED)); /* EV7 */
	*pBuffer = I2C_ReceiveData(I2C2);
	pBuffer++;
	/* Decrement the read bytes counter */
	no--;
  }
  /* 再次允许应答模式 */
  I2C_AcknowledgeConfig(I2C2, ENABLE);
}

/**
  ******************************************************************************
  *                            SHT20_Get_Data()
  *
  * 描述   : 通过命令获取温湿度
  *		   
  * 参数   : cmd
  *          		SHT20_CMD_MEASURE_TEMP	获取温度数据
  *          		SHT20_CMD_MEASURE_HUMI	获取湿度数据
  *
  *
  * 返回值 : 温湿度数据
  ******************************************************************************
  */
uint8_t aaa = 0;
uint16_t SHT20_Get_Data(uint8_t cmd){
	uint16_t data;
	uint8_t addr, checksum, buf[2];
	
	addr = SHT20_ADDRESS;
	
	//确保总线不忙
	while(I2C_GetFlagStatus(I2C2, I2C_FLAG_BUSY)){};
	
	/* 发送起始位 */
  I2C_GenerateSTART(I2C2, ENABLE);
  while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT));/*EV5,主模式*/

  /* 发送器件地址(写)*/
  I2C_Send7bitAddress(I2C2, addr<<1, I2C_Direction_Transmitter);
  while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));
	
	/*发送命令*/
	I2C_SendData(I2C2, cmd);
  while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
		
	//等待测量完成
//	delay_ms(70);	
		delay_sys_ms(70);
		
	/* 重启 */
  I2C_GenerateSTART(I2C2, ENABLE);
  while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT));/*EV5,主模式*/
	
	/* 发送器件地址(读)*/
  I2C_Send7bitAddress(I2C2, addr<<1 | 0x01, I2C_Direction_Receiver);
  while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));
	

	
	/*接收数据*/
	I2C_AcknowledgeConfig(I2C2, ENABLE);
	while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_RECEIVED)); /* EV7 */
	buf[0] = I2C_ReceiveData(I2C2);
	
	I2C_AcknowledgeConfig(I2C2, ENABLE);
	while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_RECEIVED)); /* EV7 */
	buf[1] = I2C_ReceiveData(I2C2);
	
	I2C_AcknowledgeConfig(I2C2, ENABLE);
	while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_RECEIVED)); /* EV7 */
	checksum = I2C_ReceiveData(I2C2);
	I2C_AcknowledgeConfig(I2C2, DISABLE);	//最后一位后要关闭应答的
	
  I2C_GenerateSTOP(I2C2, ENABLE);			//发送停止位
	
	data = (buf[0] << 8) + buf[1];
	
	if(CRC_Check(buf, 2, checksum) == 0)//校验通过
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
  * 描述   : 温湿度数据的CRC校验
  *		   
  * 参数   : ptr	要校验的数据
  *          len	数据长度
  *          checksum	预期校验值
  *
  *
  * 返回值 : 校验通过 0			校验失败 1
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

