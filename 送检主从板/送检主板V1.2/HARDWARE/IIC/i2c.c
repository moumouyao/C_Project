/**
  ******************************************************************************
  * 文件名 : i2c.c
  * 作  者 : 陈光
  * 版  本 ：V1.0
  * 时  间 : 2014年6月24日
  * 公  司 ：汉威光电技术有限公司
  * 摘  要 ：本文件为IO口模拟I2C，因为FSMC与I2C1有冲突，24C02中存储的一些数据
	*          其地址上的内容定义是在sw_host.h中预定义的
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
  *                                  头文件
  ******************************************************************************
  */
	
	
	




//void Delay(vu32 nCount)
//{
//  for(; nCount != 0; nCount--);
//}


/**************************************************************************
延时
ms：延时的毫秒数
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
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;//浮空输入
  GPIO_Init(GPIOB, &GPIO_InitStructure);          
}

void I2C_GPIO_SDAConfOut (void)
{
  GPIO_InitTypeDef  GPIO_InitStructure; 

  /* PB7 SDA */
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT; 
  GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;//开漏输出
	
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
  GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;//开漏输出
  GPIO_Init(GPIOB, &GPIO_InitStructure); 
  I2C_GPIO_SDAConfOut(); 	
	
	//E2promWriteByte(EEROM_ITEM_FLAG_ADDR,G_ItemFlag);
}











//===============================================================================
//GPIO 模拟I2C ，操作EP2ROM IC(AT24C02)
/*************************************************************
*函数名称：I2CStart
*函数功能：I2C开始信号
*输入参数：
*输出参数：
*备           注：时钟线高时，数据线由高到低的跳变，表示I2C开始信号
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
*函数名称：I2CStop
*函数功能：I2C停止信号
*输入参数：
*输出参数：
*备           注：时钟线高时，数据线由低到高的跳变，表示I2C停止信号
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
*函数名称：I2CSlaveAck
*函数功能：I2C从机设备应答查询
*输入参数：
*输出参数：
*备           注：
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
*函数名称：I2CWriteByte
*函数功能：I2C写一字节数据
*输入参数：
*输出参数：
*备           注：
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
*函数名称：I2CReadByte
*函数功能：I2C读一字节数据
*输入参数：
*输出参数：
*备           注：
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
*函数名称：E2promWriteByte
*函数功能：E2PROM指定地址写一字节数据
*输入参数：addr  E2PROM地址
                             data  写入的数据
*输出参数：SET: 写入正常；RESET:写入错误
*备           注：
**************************************************************/
unsigned char E2promWriteByte( unsigned char addr, unsigned char data )
{
  //asm("CPSID I");  //关中断
#if OS_CRITICAL_METHOD == 3u                     /* Allocate storage for CPU status register           */
  OS_CPU_SR  cpu_sr = 0u;
#endif
	OS_ENTER_CRITICAL();   //保存全局中断标志,关总中断/* Tell uC/OS-II that we are starting an ISR*/
  
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
  OS_EXIT_CRITICAL();	   //恢复全局中断标志
  DelayMs(12+5);             //写一个字节后需要延时12ms!!!!!!!!//////////
  //asm("CPSIE I");  //关中断

  return SET;
}

/*************************************************************
*函数名称：E2promReadByte
*函数功能：E2PROM指定地址读一字节数据
*输入参数：addr  E2PROM地址
*输出参数：返回读出的数据
*备           注：
**************************************************************/
unsigned char E2promReadByte( unsigned char addr )
{
  unsigned char ReadValue;

#if OS_CRITICAL_METHOD == 3u                     /* Allocate storage for CPU status register           */
  OS_CPU_SR  cpu_sr = 0u;
#endif
	OS_ENTER_CRITICAL();   //保存全局中断标志,关总中断/* Tell uC/OS-II that we are starting an ISR*/
  
	
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
  OS_EXIT_CRITICAL();	   //恢复全局中断标志

  return ReadValue;  
}



// void I2C_Test(void)
// {
// 	u16 i;
// 	u8 err,I2c_Buf[256];
// 	
//   printf("写入的数据\n\r");
//     
// 	//填充缓冲
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
//     err = E2promWriteByte(i,I2c_Buf[i]);	//将I2C_Buf中顺序递增的数据写入EERPOM中 	
//   } 

// 	//清缓冲
// 	for(i=0;i<=255;i++)
//   {   
// 		I2c_Buf[i]=0;
//   }
//     
//   printf("读出的数据\n\r");
// 	for(i=0;i<=255;i++)
// 	  I2c_Buf[i] = E2promReadByte(i);//将EEPROM读出数据顺序保持到I2C_Buf中  

//     //将I2C_Buf中的数据通过串口打印
// 	for(i=0;i<256;i++)
// 	{	
// 		if(I2c_Buf[i]!=255-i)
// 		{
//       printf("错误:I2C EEPROM写入与读出的数据不一致\n\r");
// 			while(1);
// 		}
//     printf("0x%X\t", I2c_Buf[i]);
//     if(i%16 == 15)
//     {
//        printf("\n\r");
//     }
// 	}
// }




