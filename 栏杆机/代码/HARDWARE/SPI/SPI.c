#include "SPI.h"
#include "os_cpu.h"
#include "sys.h"
#include "delay.h"
#include "includes.h"
/************** GB2312字库字符基地址 *****************/
#define BaseAdd_12X12	0x00000
#define BaseAdd_16X16	0x2C9D0
#define BaseAdd_24X24	0x68190
#define BaseAdd_32X32	0xEDF00
/************** ASCII字符基地址 *****************/
#define ASCIIAdd_5X7	0x1DDF80
#define ASCIIAdd_7X8	0x1DE280
#define ASCIIAdd_6X12	0x1DBE00
#define ASCIIAdd_8X16	0x1DD780
#define ASCIIAdd_12X24	0x1DFF00
#define ASCIIAdd_16X32	0x1E5A50

//--------------------------------------以下为SPI相关函数---------------------------------------//
void Init_SPI1()
{
  GPIO_InitTypeDef  GPIO_InitStructure;
 
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);//使能GPIOB时钟
	  //GPIOB6
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;//
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化
	SPI_CS=1;			//SPI不选中
	SPI1_Init();		//初始化SPI
	SPI1_SetSpeed(SPI_BaudRatePrescaler_128);		//设置4为21M时钟,高速模式
}

//SPI1速度设置函数
//SPI速度=fAPB2/分频系数
//@ref SPI_BaudRate_Prescaler:SPI_BaudRatePrescaler_2~SPI_BaudRatePrescaler_256  
//fAPB2时钟一般为84Mhz：
void SPI1_SetSpeed(u8 SPI_BaudRatePrescaler)
{
  assert_param(IS_SPI_BAUDRATE_PRESCALER(SPI_BaudRatePrescaler));//判断有效性
	SPI1->CR1&=0XFFC7;//位3-5清零，用来设置波特率
	SPI1->CR1|=SPI_BaudRatePrescaler;	//设置SPI1速度 
	SPI_Cmd(SPI1,ENABLE); //使能SPI1
}

//以下是SPI模块的初始化代码，配置成主机模式 						  
//SPI口初始化
//这里针是对SPI1的初始化
void SPI1_Init(void)
{	 
  GPIO_InitTypeDef  GPIO_InitStructure;
  SPI_InitTypeDef  SPI_InitStructure;
	
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);//使能GPIOB时钟
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);//使能SPI1时钟
 
  //GPIOFB3,4,5初始化设置
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5;//PB3~5复用功能输出	
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用功能
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
  GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化
	
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource3,GPIO_AF_SPI1); //PB3复用为 SPI1
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource4,GPIO_AF_SPI1); //PB4复用为 SPI1
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource5,GPIO_AF_SPI1); //PB5复用为 SPI1
 
	//这里只针对SPI口初始化
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_SPI1,ENABLE);//复位SPI1
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_SPI1,DISABLE);//停止复位SPI1

	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  //设置SPI单向或者双向的数据模式:SPI设置为双线双向全双工
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;		//设置SPI工作模式:设置为主SPI
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;		//设置SPI的数据大小:SPI发送接收8位帧结构
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;		//串行同步时钟的空闲状态为低电平
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;	//串行同步时钟的第1个跳变沿（上升或下降）数据被采样
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;		//NSS信号由硬件（NSS管脚）还是软件（使用SSI位）管理:内部NSS信号有SSI位控制
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;		//定义波特率预分频的值:波特率预分频值为256
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;	//指定数据传输从MSB位还是LSB位开始:数据传输从MSB位开始
	SPI_InitStructure.SPI_CRCPolynomial = 7;	//CRC值计算的多项式
	SPI_Init(SPI1, &SPI_InitStructure);  //根据SPI_InitStruct中指定的参数初始化外设SPIx寄存器
 
	SPI_Cmd(SPI1, ENABLE); //使能SPI外设

	//SPI1_ReadWriteByte(0xff);//启动传输		 
} 

//SPI1 读写一个字节
//TxData:要写入的字节
//返回值:读取到的字节
u8 SPI1_ReadWriteByte(u8 TxData)
{		 			 
 
  while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET){}//等待发送区空  
	
	SPI_I2S_SendData(SPI1, TxData); //通过外设SPIx发送一个byte  数据
		
  while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET){} //等待接收完一个byte  
 
	return SPI_I2S_ReceiveData(SPI1); //返回通过SPIx最近接收的数据	
}



void GenitopZk_Address(u32 Address) 
{
	u8 AddH, AddM, AddL;
	
	AddH = Address >> 16;
	AddM = Address >> 8;
	AddL = Address;
	
	SPI1_ReadWriteByte(0x03);	//一般读取
	SPI1_ReadWriteByte(AddH);
	SPI1_ReadWriteByte(AddM);
	SPI1_ReadWriteByte(AddL);
	
//	SPI2_ReadWriteByte(0x0B);	//快速读取
//	SPI2_ReadWriteByte(AddH);
//	SPI2_ReadWriteByte(AddM);
//	SPI2_ReadWriteByte(AddL);
//	SPI2_ReadWriteByte(0xff);
}

/*
 *读取GB2312 8x16ASCII  已验证
 */
void ASCII_8X16(u8 ASCIICode, u8 *BUF)
{
	u8 i;
	u32 BaseAdd;
	u32 Address;
	
	BaseAdd = ASCIIAdd_8X16;
	
	if(ASCIICode >= 0x20 && ASCIICode <= 0x7E){
		Address = (ASCIICode - 0x20) * 16 + BaseAdd;
	}

	SPI_CS=0;

	GenitopZk_Address(Address);
	for(i = 0; i < 16 ; i++){
		BUF[i] = SPI1_ReadWriteByte(0xFF);
	}
	SPI_CS=1;
}
/*
 *读取GB2312 16x16汉字  已验证
 */
uint8_t GB2312_16X16(u8 MSB,u8 LSB,u8 *BUF)
{
	u8 i;

	u32 BaseAdd;	//字库基地址
	u32 Address;	//字符在芯片中的地址

	BaseAdd = BaseAdd_16X16;
	
	//计算字符地址值
	if(MSB >= 0xA1 && MSB <= 0xA9 && LSB >= 0xA1){
		Address = ((MSB - 0xA1) * 94 + (LSB - 0xA1)) * 32 + BaseAdd;
	}
	else if(MSB >= 0xB0 && MSB <= 0xF7 && LSB >= 0xA1){
		Address = ((MSB - 0xB0) * 94 + (LSB - 0xA1) + 846) * 32 + BaseAdd;
	}
	else{
	return 0;
	}

	//获取字符点阵数据
	SPI_CS=0;
	GenitopZk_Address(Address);
	for(i = 0; i < 32; i++){
		BUF[i] = SPI1_ReadWriteByte(0xff);
	}
	SPI_CS=1;
	return 1;
}



/*
 *读取GB2312 12X24ASCII  
 */
void ASCII_12X24(u8 ASCIICode, u8 *BUF)
{
	u8 i;
	u32 BaseAdd;
	u32 Address;
	
	BaseAdd = ASCIIAdd_12X24;
	
	if(ASCIICode >= 0x20 && ASCIICode <= 0x7E){
		Address = (ASCIICode - 0x20) * 48 + BaseAdd;
	}

	SPI_CS=0;

	GenitopZk_Address(Address);
	for(i = 0; i < 48 ; i++){
		BUF[i] = SPI1_ReadWriteByte(0xFF);
	}
	SPI_CS=1;
}
/*
 *读取GB2312 16x16汉字  已验证
 */
uint8_t GB2312_24X24(u8 MSB,u8 LSB,u8 *BUF)
{
	u8 i;

	u32 BaseAdd;	//字库基地址
	u32 Address;	//字符在芯片中的地址

	BaseAdd = BaseAdd_24X24;
	
	//计算字符地址值
	if(MSB >= 0xA1 && MSB <= 0xA9 && LSB >= 0xA1){
		Address = ((MSB - 0xA1) * 94 + (LSB - 0xA1)) * 72 + BaseAdd;
	}
	else if(MSB >= 0xB0 && MSB <= 0xF7 && LSB >= 0xA1){
		Address = ((MSB - 0xB0) * 94 + (LSB - 0xA1) + 846) * 72 + BaseAdd;
	}
	else{
	return 1;
	}

	//获取字符点阵数据
	SPI_CS=0;
	GenitopZk_Address(Address);
	for(i = 0; i < 72; i++){
		BUF[i] = SPI1_ReadWriteByte(0xff);
	}
	SPI_CS=1;
	return 0;
}


