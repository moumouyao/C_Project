#include "i2c.h"
#include <stdio.h>
#include "stm32f4xx.h" 

#include "Delay.h"

#include "FreeRTOS.h"
#include "task.h"

/**
  ******************************************************************************
  *                                   ȫ����
  ******************************************************************************
  */

#define AT24C01A		            //24C01A,I2Cʱ��������24C02һ��
#define EEPROM_ADDRESS		0xA0
#define I2C_PAGESIZE	    8		  //24C02ҳ������8��

/**
  ******************************************************************************
  *                            User_I2C1Config()
  *
  * ����   : ��ʼ��I2C1
  *		   
  * ����   : ��
  *
  *
  * ����ֵ : ��
  ******************************************************************************
  */

void User_I2CConfig (void)
{
  I2C_InitTypeDef  I2C_InitStructure;
  GPIO_InitTypeDef  GPIO_InitStructure; 

  RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1,ENABLE);
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);
    

  /* PB6,7 SCL and SDA */
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_8 | GPIO_Pin_9;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType=GPIO_OType_OD;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF; //���ùܽ�Ϊ���ù��ܿ�©���
  GPIO_Init(GPIOB, &GPIO_InitStructure);          //I2C�ӿ�ʹ�õ�GPIO�ܽų�ʼ��


  GPIO_PinAFConfig(GPIOB,GPIO_PinSource8,GPIO_AF_I2C1);//GPIOB0����Ϊ��ʱ��3
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource9,GPIO_AF_I2C1);//GPIOB1����Ϊ��ʱ��3
	
  I2C_DeInit(I2C1);	//��λI2C1
  I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;         //����I2C�ӿ�ģʽ 
  I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2; //����I2C�ӿڵĸߵ͵�ƽ����
  I2C_InitStructure.I2C_OwnAddress1 = 0x30;          //����I2C�ӿڵ�������ַ
  I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;        //�����Ƿ���ACK��Ӧ          
  I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
  I2C_InitStructure.I2C_ClockSpeed = 200000;        //100K�ٶ�
    
  I2C_Cmd(I2C1, ENABLE);                            //ʹ��I2C�ӿ�
  I2C_Init(I2C1, &I2C_InitStructure);               //I2C�ӿڳ�ʼ��
  /*����1�ֽ�1Ӧ��ģʽ*/
  I2C_AcknowledgeConfig(I2C1, ENABLE);              //ʹ��I2C�ӿ���Ӧ
}


/**
  ******************************************************************************
  *                            I2C_ReadS_24C()
  *
  * ����   : ͨ��I2C1������24C�е�����
  *		   
  * ����   : addr --Ҫ��ȡ24C�е����ݵĵ�ַ��
  *          pBuffer--���ݴ�ŵĵ�ַ
  *          no--Ҫ��ȡ���ݵĸ���
  *
  *
  * ����ֵ : ��
  ******************************************************************************
  */

void I2C_ReadS_24C(u8 addr ,u8* pBuffer,u16 no)
{
  if(no==0)	return;
	
  while(I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY));
		
  /* ����1�ֽ�1Ӧ��ģʽ */
  I2C_AcknowledgeConfig(I2C1, ENABLE);

  /* ������ʼλ */
  I2C_GenerateSTART(I2C1, ENABLE);
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));/*EV5,��ģʽ*/

#ifdef AT24C01A	
  /* ����������ַ(д) */
  I2C_Send7bitAddress(I2C1,  EEPROM_ADDRESS, I2C_Direction_Transmitter);
  while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));

  /* ���͵�ַ */
  I2C_SendData(I2C1, addr);
  while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));/*�����ѷ���*/
		
  /* ��ʼλ */
  I2C_GenerateSTART(I2C1, ENABLE);
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));
	
  /* ������ */
  I2C_Send7bitAddress(I2C1, EEPROM_ADDRESS, I2C_Direction_Receiver);
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));
	
	
#else	
  /* ����������ַ(��)24C02 */
  I2C_Send7bitAddress(I2C1, addr<<1, I2C_Direction_Receiver);
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));
#endif	
  while (no)
  {
    if(no==1)
	{
      I2C_AcknowledgeConfig(I2C1, DISABLE);	//���һλ��Ҫ�ر�Ӧ���
      I2C_GenerateSTOP(I2C1, ENABLE);			//����ֹͣλ
	}
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_RECEIVED)); /* EV7 */
	*pBuffer = I2C_ReceiveData(I2C1);
	pBuffer++;
	/* Decrement the read bytes counter */
	no--;
  }
  /* �ٴ�����Ӧ��ģʽ */
  I2C_AcknowledgeConfig(I2C1, ENABLE);
}

/**
  ******************************************************************************
  *                            I2C_Standby_24C()
  *
  * ����   : �ж�24C�Ƿ�׼������д��
  *		   
  * ����   : ��
  *
  *
  * ����ֵ : ��
  ******************************************************************************
  */

void I2C_Standby_24C(void)      
{
  vu16 SR1_Tmp;
  do
  {
    /* ��ʼλ */
    I2C_GenerateSTART(I2C1, ENABLE);
    /* ��SR1 */
    SR1_Tmp = I2C_ReadRegister(I2C1, I2C_Register_SR1);
    /* ������ַ(д) */
    #ifdef AT24C01A
	I2C_Send7bitAddress(I2C1, EEPROM_ADDRESS, I2C_Direction_Transmitter);
	#else
	I2C_Send7bitAddress(I2C1, 0, I2C_Direction_Transmitter);
	#endif
  }while(!(I2C_ReadRegister(I2C1, I2C_Register_SR1) & 0x0002));
  I2C_ClearFlag(I2C1, I2C_FLAG_AF);
  /* ֹͣλ */    
  I2C_GenerateSTOP(I2C1, ENABLE);
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

void I2C_ByteWrite_24C(u8 addr,u8 dat)
{
  /* ��ʼλ */
  I2C_GenerateSTART(I2C1, ENABLE);
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));  

#ifdef AT24C01A
  /* ����������ַ(д) */
  I2C_Send7bitAddress(I2C1, EEPROM_ADDRESS, I2C_Direction_Transmitter);
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
  
  /* ���͵�ַ */
  I2C_SendData(I2C1, addr);
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));

#else	
  I2C_Send7bitAddress(I2C1, addr<<1, I2C_Direction_Transmitter);
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
#endif

  /* дһ���ֽ� */
  I2C_SendData(I2C1, dat); 
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
	
  /* ֹͣλ */
  I2C_GenerateSTOP(I2C1, ENABLE);
  
  I2C_Standby_24C();
}

/**
  ******************************************************************************
  *                            I2C_PageWrite_24C()
  *
  * ����   : ��24C��addr��ַ��дno���ֽ�����
  *		   
  * ����   : addr--24C�е�ĳһ��ַ
  *          pBuffer--Ҫд�����ݻ������ĵ�ַ
  *          no--Ҫд����ֽڵĸ���
  *
  *
  * ����ֵ : ��
  * ע�����д��һҳ(����)����24C02��˵no<=8
  ******************************************************************************
  */

void I2C_PageWrite_24C(u8 addr,u8* pBuffer, u8 no)
{
  if(no==0) return ;
  while(I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY));

  /* ��ʼλ */
  I2C_GenerateSTART(I2C1, ENABLE);
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT)); 

#ifdef AT24C01A
  /* ������ַ(д) */
  I2C_Send7bitAddress(I2C1, EEPROM_ADDRESS, I2C_Direction_Transmitter);
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
	
  /* д��ֵַ */
  I2C_SendData(I2C1, addr);
  while(! I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));

#else
  I2C_Send7bitAddress(I2C1, addr<<1, I2C_Direction_Transmitter);
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)); 
#endif
  while(no--)  
  {
    I2C_SendData(I2C1, *pBuffer); 
	pBuffer++; 
	while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
  }
  /* ֹͣλ */
  I2C_GenerateSTOP(I2C1, ENABLE);
}




/**
  ******************************************************************************
  *                            I2C_WriteS_24C()
  *
  * ����   : ��24C��addr��ַ��дno���ֽ�����
  *		   
  * ����   : addr--24C�е�ĳһ��ַ
  *          pBuffer--Ҫд�����ݻ������ĵ�ַ
  *          no--Ҫд����ֽڵĸ���-no 0-256
  *
  *
  * ����ֵ : ��
  * ע�����24c02�������һ��д��8���ֽڣ�
  ******************************************************************************
  */

void I2C_WriteS_24C(u8 addr,u8* pBuffer,  u16 no)
{
  u8 temp;

  /* �Ȱ�ҳ������Ĳ���д�� */
  temp= addr % I2C_PAGESIZE;
  if(temp)
  {
    temp=I2C_PAGESIZE-temp;
	I2C_PageWrite_24C(addr,pBuffer,temp);//��ҳ��������ֽ�д��EEPROM
	no-=temp;
	addr+=temp;
	pBuffer+=temp;
	I2C_Standby_24C();//�ж�EEPROM�Ƿ�æ
  }
  /* ��ҳ���뿪ʼд	*/
  while(no)
  {
	if(no>=I2C_PAGESIZE)
	{
	  I2C_PageWrite_24C(addr,pBuffer,I2C_PAGESIZE);//��ҳ������ֽ�д��EEPROM
	  no-=I2C_PAGESIZE;
	  addr+=I2C_PAGESIZE;
	  pBuffer+=I2C_PAGESIZE;
	  I2C_Standby_24C();//�ж�EEPROM�Ƿ�æ
	}
	else
	{
	  I2C_PageWrite_24C(addr,pBuffer, no);
	  no=0;
	  I2C_Standby_24C();
	}
  }
}

 void I2C_Test(void)
 {
 	u16 i;
 	u8 I2c_Buf[13];
#if Debug_printf
     printf("д�������\n\r");
#endif
 	//��仺��
 	for(i=0;i<=13;i++)
     {   
         I2c_Buf[i]=i;
#if Debug_printf
         printf("0x%2x\t",I2c_Buf[i]);
 
			 if(i%16 == 15)
         {
             printf("\n\r");
         }
#endif   
     }
#if Debug_printf
     printf("\n\r");
#endif
 	I2C_WriteS_24C(5,I2c_Buf,13);	//��I2C_Buf��˳�����������д��EERPOM��  

 	
 	//�建��
 	for(i=0;i<=13;i++)
     {   
 		I2c_Buf[i]=0;
     }
#if Debug_printf
     printf("����������\n\r");
#endif
 	I2C_ReadS_24C(5,I2c_Buf,13);//��EEPROM��������˳�򱣳ֵ�I2C_Buf��  

     //��I2C_Buf�е�����ͨ�����ڴ�ӡ
 	for(i=0;i<13;i++)
 	{	
 		if(I2c_Buf[i]!=i)
 		{
#if Debug_printf
             printf("����:I2C EEPROMд������������ݲ�һ��\n\r");
#endif
		while(1);
 		}
#if Debug_printf
         printf("0x%2X\t", I2c_Buf[i]);
         if(i%16 == 15)
         {
             printf("\n\r");
         }
#endif
 	}
 }
