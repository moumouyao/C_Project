#include "SPI.h"
#include "sys.h"
#include "delay.h"


//--------------------------------------以下为SPI相关函数---------------------------------------//
void Init_SPI2()
{
  GPIO_InitTypeDef  GPIO_InitStructure;
 
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOI, ENABLE);//使能GPIOI时钟
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;//
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOI, &GPIO_InitStructure);//初始化
	SPI_CS=1;			//SPI不选中
	SPI2_Init();		//初始化SPI
	SPI2_SetSpeed(SPI_BaudRatePrescaler_128);		//设置4为21M时钟,高速模式
}

//SPI1速度设置函数
//SPI速度=fAPB2/分频系数
//@ref SPI_BaudRate_Prescaler:SPI_BaudRatePrescaler_2~SPI_BaudRatePrescaler_256  
//fAPB2时钟一般为84Mhz：
void SPI2_SetSpeed(u8 SPI_BaudRatePrescaler)
{
  assert_param(IS_SPI_BAUDRATE_PRESCALER(SPI_BaudRatePrescaler));//判断有效性
	SPI2->CR1&=0XFFC7;//位3-5清零，用来设置波特率
	SPI2->CR1|=SPI_BaudRatePrescaler;	//设置SPI1速度 
	SPI_Cmd(SPI2,ENABLE); //使能SPI1
}

//以下是SPI模块的初始化代码，配置成主机模式 						  
//SPI口初始化
//这里针是对SPI1的初始化
void SPI2_Init(void)
{	 
  GPIO_InitTypeDef  GPIO_InitStructure;
  SPI_InitTypeDef  SPI_InitStructure;
	
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOI, ENABLE);//使能GPIOI时钟
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);//使能SPI2时钟
 
  //GPIOI 1~3初始化设置
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3;//PI1~3复用功能输出	
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用功能
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
  GPIO_Init(GPIOI, &GPIO_InitStructure);//初始化
	
	GPIO_PinAFConfig(GPIOI,GPIO_PinSource1,GPIO_AF_SPI2); //PI1复用为 SPI2
	GPIO_PinAFConfig(GPIOI,GPIO_PinSource2,GPIO_AF_SPI2); //PI2复用为 SPI2
	GPIO_PinAFConfig(GPIOI,GPIO_PinSource3,GPIO_AF_SPI2); //PI3复用为 SPI2
 
//	//这里只针对SPI口初始化
//	RCC_APB1PeriphResetCmd(RCC_APB1Periph_SPI2,ENABLE);//复位SPI1
//	RCC_APB1PeriphResetCmd(RCC_APB1Periph_SPI2,DISABLE);//停止复位SPI1
 
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex; //SPI2设置为两线全双工
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;	            //设置SPI2为主模式
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;         //SPI发送接收8位帧结构
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;	 		          //串行时钟在不操作时，时钟为高电平
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;		          //第二个时钟沿开始采样数据
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;			            //NSS信号由软件（使用SSI位）管理
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_8; //定义波特率预分频的值:波特率预分频值为8
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;				//数据传输从MSB位开始
	SPI_InitStructure.SPI_CRCPolynomial = 7;						      //CRC值计算的多项式
	SPI_Init(SPI2, &SPI_InitStructure);
	/* Enable SPI2  */
	
	SPI_Cmd(SPI2, ENABLE); //使能SPI外设 
} 

//SPI2 读写一个字节
//TxData:要写入的字节
//返回值:读取到的字节
uint8_t SPI2_ReadWriteByte(uint8_t TxData)
{		 			 
 
  while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET){}//等待发送区空  
	
	SPI_I2S_SendData(SPI2, TxData); //通过外设SPIx发送一个byte  数据
		
  while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET){} //等待接收完一个byte  
 
	return SPI_I2S_ReceiveData(SPI2); //返回通过SPIx最近接收的数据	
}

