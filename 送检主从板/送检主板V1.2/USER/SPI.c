#include "SPI.h"
//1
void init_SPI_GPIO()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin =  SPI1_MOSI| SPI1_SCK;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;  //复用推挽输出
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;//开漏输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIO_SPI1, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin =  SPI1_MISO;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN; 
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉输入
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIO_SPI1, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = SPI1_CS;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;  //复用推挽输出
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;//开漏输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIO_SPI1, &GPIO_InitStructure);
  GPIO_ResetBits(GPIO_SPI1,SPI1_CS);
}
void init_SPI()
{
	SPI_InitTypeDef SPI_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);      //使能SPI1时钟
	
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;//双线双向全双工
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;//设置为主SPI
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;//8位帧结构
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;//时钟悬空低
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;//第二个时钟沿
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;//内部NSS信号由SSI位控制
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;//预分频值为2
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_LSB;//数据传输从LSB位开始
	SPI_InitStructure.SPI_CRCPolynomial = 7;//用于CRC值计算的多项式
	SPI_Init(SPI1,&SPI_InitStructure);//初始化配置
	SPI_Cmd(SPI1, ENABLE);    //使能SPI外设
}
/*******************************************************************************
* Function Name  : SPI1_DMA_Configuration
* Description    : 配置SPI1_RX的DMA通道2，SPI1_TX的DMA通道3
* Input          : None
* Output         : None
* Return         : None
* Attention             :
*******************************************************************************/
void init_SPI_DMA()
{
  DMA_InitTypeDef DMA_InitStructure;
	//=============发送DMA初始化
	DMA_DeInit(DMA2_Stream3);//初始化
  DMA_InitStructure.DMA_Channel = DMA_Channel_3;                            //通道选择
  DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)&SPI2->DR;                //DMA外设地址
  DMA_InitStructure.DMA_Memory0BaseAddr = (u32)pixelBuffer;                 //DMA 存储器0地址
  DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;                   //存储器到外设模式
  DMA_InitStructure.DMA_BufferSize = Pixel_S1_NUM * 24;                     //数据传输量 
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;          //外设非增量模式
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;                   //存储器增量模式
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;   //外设数据长度:8位
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;           //存储器数据长度:8位
  DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;                             // 使用普通模式 
  DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;                     //中等优先级
  DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;         
  DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;
  DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;               //存储器突发单次传输
  DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;       //外设突发单次传输
  DMA_Init(DMA2_Stream3, &DMA_InitStructure);                               //初始化DMA Stream

 
 
	//=============接收DMA初始化
	DMA_DeInit(DMA2_Stream0);//初始化
  DMA_InitStructure.DMA_Channel = DMA_Channel_3;                            //通道选择
  DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)&SPI2->DR;                //DMA外设地址
  DMA_InitStructure.DMA_Memory0BaseAddr = (u32)pixelBuffer;                 //DMA 存储器0地址
  DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;                   //存储器到外设模式
  DMA_InitStructure.DMA_BufferSize = Pixel_S1_NUM * 24;                     //数据传输量 
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;          //外设非增量模式
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;                   //存储器增量模式
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;   //外设数据长度:8位
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;           //存储器数据长度:8位
  DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;                             // 使用普通模式 
  DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;                     //中等优先级
  DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;         
  DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;
  DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;               //存储器突发单次传输
  DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;       //外设突发单次传输
  DMA_Init(DMA2_Stream0, &DMA_InitStructure);                               //初始化DMA Stream
}

