#include "SPI.h"
//1
void init_SPI_GPIO()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin =  SPI1_MOSI| SPI1_SCK;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;  //�����������
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;//��©���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIO_SPI1, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin =  SPI1_MISO;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN; 
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//��������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIO_SPI1, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = SPI1_CS;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;  //�����������
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;//��©���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIO_SPI1, &GPIO_InitStructure);
  GPIO_ResetBits(GPIO_SPI1,SPI1_CS);
}
void init_SPI()
{
	SPI_InitTypeDef SPI_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);      //ʹ��SPI1ʱ��
	
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;//˫��˫��ȫ˫��
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;//����Ϊ��SPI
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;//8λ֡�ṹ
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;//ʱ�����յ�
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;//�ڶ���ʱ����
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;//�ڲ�NSS�ź���SSIλ����
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;//Ԥ��ƵֵΪ2
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_LSB;//���ݴ����LSBλ��ʼ
	SPI_InitStructure.SPI_CRCPolynomial = 7;//����CRCֵ����Ķ���ʽ
	SPI_Init(SPI1,&SPI_InitStructure);//��ʼ������
	SPI_Cmd(SPI1, ENABLE);    //ʹ��SPI����
}
/*******************************************************************************
* Function Name  : SPI1_DMA_Configuration
* Description    : ����SPI1_RX��DMAͨ��2��SPI1_TX��DMAͨ��3
* Input          : None
* Output         : None
* Return         : None
* Attention             :
*******************************************************************************/
void init_SPI_DMA()
{
  DMA_InitTypeDef DMA_InitStructure;
	//=============����DMA��ʼ��
	DMA_DeInit(DMA2_Stream3);//��ʼ��
  DMA_InitStructure.DMA_Channel = DMA_Channel_3;                            //ͨ��ѡ��
  DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)&SPI2->DR;                //DMA�����ַ
  DMA_InitStructure.DMA_Memory0BaseAddr = (u32)pixelBuffer;                 //DMA �洢��0��ַ
  DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;                   //�洢��������ģʽ
  DMA_InitStructure.DMA_BufferSize = Pixel_S1_NUM * 24;                     //���ݴ����� 
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;          //���������ģʽ
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;                   //�洢������ģʽ
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;   //�������ݳ���:8λ
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;           //�洢�����ݳ���:8λ
  DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;                             // ʹ����ͨģʽ 
  DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;                     //�е����ȼ�
  DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;         
  DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;
  DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;               //�洢��ͻ�����δ���
  DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;       //����ͻ�����δ���
  DMA_Init(DMA2_Stream3, &DMA_InitStructure);                               //��ʼ��DMA Stream

 
 
	//=============����DMA��ʼ��
	DMA_DeInit(DMA2_Stream0);//��ʼ��
  DMA_InitStructure.DMA_Channel = DMA_Channel_3;                            //ͨ��ѡ��
  DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)&SPI2->DR;                //DMA�����ַ
  DMA_InitStructure.DMA_Memory0BaseAddr = (u32)pixelBuffer;                 //DMA �洢��0��ַ
  DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;                   //�洢��������ģʽ
  DMA_InitStructure.DMA_BufferSize = Pixel_S1_NUM * 24;                     //���ݴ����� 
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;          //���������ģʽ
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;                   //�洢������ģʽ
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;   //�������ݳ���:8λ
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;           //�洢�����ݳ���:8λ
  DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;                             // ʹ����ͨģʽ 
  DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;                     //�е����ȼ�
  DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;         
  DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;
  DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;               //�洢��ͻ�����δ���
  DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;       //����ͻ�����δ���
  DMA_Init(DMA2_Stream0, &DMA_InitStructure);                               //��ʼ��DMA Stream
}

