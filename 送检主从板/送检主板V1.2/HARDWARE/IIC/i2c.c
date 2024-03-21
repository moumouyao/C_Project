/**
  ******************************************************************************
  * �ļ��� : i2c.c
  * ��  �� : �¹�
  * ��  �� ��V1.0
  * ʱ  �� : 2014��6��24��
  * ��  ˾ ��������缼�����޹�˾
  * ժ  Ҫ �����ļ�ΪIO��ģ��I2C����ΪFSMC��I2C1�г�ͻ��24C02�д洢��һЩ����
	*          ���ַ�ϵ����ݶ�������sw_host.h��Ԥ�����
  *
  ******************************************************************************
  */

#include "includes.h"
// #include "i2c.h"
// #include "stdio.h"


#define E2PROM_SCL     GPIO_Pin_6
#define E2PROM_SDA     GPIO_Pin_7



#define SDA_H         GPIO_SetBits( GPIOB, E2PROM_SDA )
#define SDA_L         GPIO_ResetBits( GPIOB, E2PROM_SDA )

#define SCL_H         GPIO_SetBits( GPIOB, E2PROM_SCL )
#define SCL_L         GPIO_ResetBits( GPIOB, E2PROM_SCL )





/**
  ******************************************************************************
  *                                  ͷ�ļ�
  ******************************************************************************
  */
	
	
	




//void Delay(vu32 nCount)
//{
//  for(; nCount != 0; nCount--);
//}


/**************************************************************************
��ʱ
ms����ʱ�ĺ�����
CYCLECOUNTER / 72000000
***************************************************************************/
void DelayMs(uint16_t ms)
{
 uint16_t iq0;
 uint16_t iq1;
 for(iq0 = ms; iq0 > 0; iq0--)
 {
  for(iq1 = 11998; iq1 > 0; iq1--); // ( (6*iq1+9)*iq0+15 ) / 72000000
  
 }
}




void I2C_GPIO_SDAConfIn (void)
{
  GPIO_InitTypeDef  GPIO_InitStructure; 

  /* PB7 SDA */
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;//��������
  GPIO_Init(GPIOB, &GPIO_InitStructure);          
}

void I2C_GPIO_SDAConfOut (void)
{
  GPIO_InitTypeDef  GPIO_InitStructure; 

  /* PB7 SDA */
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT; 
  GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;//��©���
	
  GPIO_Init(GPIOB, &GPIO_InitStructure);          
}


void I2C_GPIOConf (void)
{
  GPIO_InitTypeDef  GPIO_InitStructure; 

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);
    

  /* PB6 SCL */
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_6;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT; 
  GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;//��©���
  GPIO_Init(GPIOB, &GPIO_InitStructure); 
  I2C_GPIO_SDAConfOut(); 	
	
	//E2promWriteByte(EEROM_ITEM_FLAG_ADDR,G_ItemFlag);
}











//===============================================================================
//GPIO ģ��I2C ������EP2ROM IC(AT24C02)
/*************************************************************
*�������ƣ�I2CStart
*�������ܣ�I2C��ʼ�ź�
*���������
*���������
*��           ע��ʱ���߸�ʱ���������ɸߵ��͵����䣬��ʾI2C��ʼ�ź�
**************************************************************/
void I2CStart( void )
{
  SDA_H;
  Delay(25);
  SCL_H;
  Delay(25);
  SDA_L;
  Delay(25);
}

/*************************************************************
*�������ƣ�I2CStop
*�������ܣ�I2Cֹͣ�ź�
*���������
*���������
*��           ע��ʱ���߸�ʱ���������ɵ͵��ߵ����䣬��ʾI2Cֹͣ�ź�
**************************************************************/
void I2CStop( void )
{
  SDA_L;
  Delay(25);
  SCL_H;
  Delay(25);
  SDA_H;
  Delay(25);
}

/*************************************************************
*�������ƣ�I2CSlaveAck
*�������ܣ�I2C�ӻ��豸Ӧ���ѯ
*���������
*���������
*��           ע��
**************************************************************/
unsigned char I2CSlaveAck( void )
{
  unsigned int TimeOut;
  unsigned char RetValue;

  I2C_GPIO_SDAConfIn();
  SCL_H;
  Delay(25);
  TimeOut = 10000;
  while(TimeOut-- > 0)
  {
    if( 1 == GPIO_ReadInputDataBit( GPIOB, E2PROM_SDA ) )
    {
      RetValue = RESET;
    }
    else
    {
      RetValue = SET;
	    break;			
    }
  }
  SCL_L;
  Delay(25);
  I2C_GPIO_SDAConfOut();
  return RetValue;
}

/*************************************************************
*�������ƣ�I2CWriteByte
*�������ܣ�I2Cдһ�ֽ�����
*���������
*���������
*��           ע��
**************************************************************/
void I2CWriteByte( unsigned char byte )
{
  unsigned char i;

  for( i=0; i<8; i++ )
  {
	SCL_L;
	Delay(25);
	if( 0X80 & byte )
      SDA_H;
    else
      SDA_L;
    byte <<= 1;
    Delay(25);
    SCL_H;
	Delay(25);

  }
  SCL_L;
  Delay(25);
  SDA_H;
  Delay(25);
}

/*************************************************************
*�������ƣ�I2CReadByte
*�������ܣ�I2C��һ�ֽ�����
*���������
*���������
*��           ע��
**************************************************************/
unsigned char I2CReadByte( void )
{
  unsigned char i;
  unsigned char ReadValue = 0;
  unsigned char bit;
  SCL_L;
  Delay(25);
  I2C_GPIO_SDAConfIn();
  for( i=0; i<8; i++ )
  {
    SCL_H;
    Delay(25);
    if( SET == GPIO_ReadInputDataBit( GPIOB, E2PROM_SDA ) )
      bit = 0X01;
    else
      bit = 0x00;
      
    ReadValue = (ReadValue<<1)|bit;
    SCL_L;
    Delay(25);
  }
  I2C_GPIO_SDAConfOut();
  SDA_H;
  Delay(25);
  return ReadValue;
}

/*************************************************************
*�������ƣ�E2promWriteByte
*�������ܣ�E2PROMָ����ַдһ�ֽ�����
*���������addr  E2PROM��ַ
                             data  д�������
*���������SET: д��������RESET:д�����
*��           ע��
**************************************************************/
unsigned char E2promWriteByte( unsigned char addr, unsigned char data )
{
  //asm("CPSID I");  //���ж�
#if OS_CRITICAL_METHOD == 3u                     /* Allocate storage for CPU status register           */
  OS_CPU_SR  cpu_sr = 0u;
#endif
	OS_ENTER_CRITICAL();   //����ȫ���жϱ�־,�����ж�/* Tell uC/OS-II that we are starting an ISR*/
  
  I2CStart();

  I2CWriteByte( 0xA0 );
  if( RESET == I2CSlaveAck() )
  {
    return RESET;
  }
  I2CWriteByte( addr );
  if( RESET == I2CSlaveAck() )
  {
    return RESET;
  }
  I2CWriteByte( data );
  if( RESET == I2CSlaveAck() )
  {
    return RESET;
  }
  I2CStop();
  OS_EXIT_CRITICAL();	   //�ָ�ȫ���жϱ�־
  DelayMs(12+5);             //дһ���ֽں���Ҫ��ʱ12ms!!!!!!!!//////////
  //asm("CPSIE I");  //���ж�

  return SET;
}

/*************************************************************
*�������ƣ�E2promReadByte
*�������ܣ�E2PROMָ����ַ��һ�ֽ�����
*���������addr  E2PROM��ַ
*������������ض���������
*��           ע��
**************************************************************/
unsigned char E2promReadByte( unsigned char addr )
{
  unsigned char ReadValue;

#if OS_CRITICAL_METHOD == 3u                     /* Allocate storage for CPU status register           */
  OS_CPU_SR  cpu_sr = 0u;
#endif
	OS_ENTER_CRITICAL();   //����ȫ���жϱ�־,�����ж�/* Tell uC/OS-II that we are starting an ISR*/
  
	
  I2CStart();

  I2CWriteByte( 0xA0 );
  if( RESET == I2CSlaveAck() )
  {
    return RESET;
  }
  I2CWriteByte( addr );
  if( RESET == I2CSlaveAck() )
  {
    return RESET;
  }
  I2CStart();
  I2CWriteByte( 0xA1 );
  if( RESET == I2CSlaveAck() )
  {
    return RESET;
  }
  ReadValue = I2CReadByte();
  I2CStop();
  OS_EXIT_CRITICAL();	   //�ָ�ȫ���жϱ�־

  return ReadValue;  
}



// void I2C_Test(void)
// {
// 	u16 i;
// 	u8 err,I2c_Buf[256];
// 	
//   printf("д�������\n\r");
//     
// 	//��仺��
// 	for(i=0;i<=255;i++)
//   {   
//     I2c_Buf[i]=255-i;
//     printf("0x%x\t",I2c_Buf[i]);
//     if(i%16 == 15)
//     {
//       printf("\n\r");
//     }
//   }
//   printf("\n\r"); 
//   for(i=0;i<=255;i++)
//   {
//     err = E2promWriteByte(i,I2c_Buf[i]);	//��I2C_Buf��˳�����������д��EERPOM�� 	
//   } 

// 	//�建��
// 	for(i=0;i<=255;i++)
//   {   
// 		I2c_Buf[i]=0;
//   }
//     
//   printf("����������\n\r");
// 	for(i=0;i<=255;i++)
// 	  I2c_Buf[i] = E2promReadByte(i);//��EEPROM��������˳�򱣳ֵ�I2C_Buf��  

//     //��I2C_Buf�е�����ͨ�����ڴ�ӡ
// 	for(i=0;i<256;i++)
// 	{	
// 		if(I2c_Buf[i]!=255-i)
// 		{
//       printf("����:I2C EEPROMд������������ݲ�һ��\n\r");
// 			while(1);
// 		}
//     printf("0x%X\t", I2c_Buf[i]);
//     if(i%16 == 15)
//     {
//        printf("\n\r");
//     }
// 	}
// }




