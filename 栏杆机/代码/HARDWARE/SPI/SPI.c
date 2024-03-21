#include "SPI.h"
#include "os_cpu.h"
#include "sys.h"
#include "delay.h"
#include "includes.h"
/************** GB2312�ֿ��ַ�����ַ *****************/
#define BaseAdd_12X12	0x00000
#define BaseAdd_16X16	0x2C9D0
#define BaseAdd_24X24	0x68190
#define BaseAdd_32X32	0xEDF00
/************** ASCII�ַ�����ַ *****************/
#define ASCIIAdd_5X7	0x1DDF80
#define ASCIIAdd_7X8	0x1DE280
#define ASCIIAdd_6X12	0x1DBE00
#define ASCIIAdd_8X16	0x1DD780
#define ASCIIAdd_12X24	0x1DFF00
#define ASCIIAdd_16X32	0x1E5A50

//--------------------------------------����ΪSPI��غ���---------------------------------------//
void Init_SPI1()
{
  GPIO_InitTypeDef  GPIO_InitStructure;
 
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);//ʹ��GPIOBʱ��
	  //GPIOB6
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;//
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ��
	SPI_CS=1;			//SPI��ѡ��
	SPI1_Init();		//��ʼ��SPI
	SPI1_SetSpeed(SPI_BaudRatePrescaler_128);		//����4Ϊ21Mʱ��,����ģʽ
}

//SPI1�ٶ����ú���
//SPI�ٶ�=fAPB2/��Ƶϵ��
//@ref SPI_BaudRate_Prescaler:SPI_BaudRatePrescaler_2~SPI_BaudRatePrescaler_256  
//fAPB2ʱ��һ��Ϊ84Mhz��
void SPI1_SetSpeed(u8 SPI_BaudRatePrescaler)
{
  assert_param(IS_SPI_BAUDRATE_PRESCALER(SPI_BaudRatePrescaler));//�ж���Ч��
	SPI1->CR1&=0XFFC7;//λ3-5���㣬�������ò�����
	SPI1->CR1|=SPI_BaudRatePrescaler;	//����SPI1�ٶ� 
	SPI_Cmd(SPI1,ENABLE); //ʹ��SPI1
}

//������SPIģ��ĳ�ʼ�����룬���ó�����ģʽ 						  
//SPI�ڳ�ʼ��
//�������Ƕ�SPI1�ĳ�ʼ��
void SPI1_Init(void)
{	 
  GPIO_InitTypeDef  GPIO_InitStructure;
  SPI_InitTypeDef  SPI_InitStructure;
	
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);//ʹ��GPIOBʱ��
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);//ʹ��SPI1ʱ��
 
  //GPIOFB3,4,5��ʼ������
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5;//PB3~5���ù������	
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//���ù���
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
  GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ��
	
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource3,GPIO_AF_SPI1); //PB3����Ϊ SPI1
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource4,GPIO_AF_SPI1); //PB4����Ϊ SPI1
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource5,GPIO_AF_SPI1); //PB5����Ϊ SPI1
 
	//����ֻ���SPI�ڳ�ʼ��
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_SPI1,ENABLE);//��λSPI1
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_SPI1,DISABLE);//ֹͣ��λSPI1

	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  //����SPI�������˫�������ģʽ:SPI����Ϊ˫��˫��ȫ˫��
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;		//����SPI����ģʽ:����Ϊ��SPI
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;		//����SPI�����ݴ�С:SPI���ͽ���8λ֡�ṹ
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;		//����ͬ��ʱ�ӵĿ���״̬Ϊ�͵�ƽ
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;	//����ͬ��ʱ�ӵĵ�1�������أ��������½������ݱ�����
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;		//NSS�ź���Ӳ����NSS�ܽţ����������ʹ��SSIλ������:�ڲ�NSS�ź���SSIλ����
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;		//���岨����Ԥ��Ƶ��ֵ:������Ԥ��ƵֵΪ256
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;	//ָ�����ݴ����MSBλ����LSBλ��ʼ:���ݴ����MSBλ��ʼ
	SPI_InitStructure.SPI_CRCPolynomial = 7;	//CRCֵ����Ķ���ʽ
	SPI_Init(SPI1, &SPI_InitStructure);  //����SPI_InitStruct��ָ���Ĳ�����ʼ������SPIx�Ĵ���
 
	SPI_Cmd(SPI1, ENABLE); //ʹ��SPI����

	//SPI1_ReadWriteByte(0xff);//��������		 
} 

//SPI1 ��дһ���ֽ�
//TxData:Ҫд����ֽ�
//����ֵ:��ȡ�����ֽ�
u8 SPI1_ReadWriteByte(u8 TxData)
{		 			 
 
  while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET){}//�ȴ���������  
	
	SPI_I2S_SendData(SPI1, TxData); //ͨ������SPIx����һ��byte  ����
		
  while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET){} //�ȴ�������һ��byte  
 
	return SPI_I2S_ReceiveData(SPI1); //����ͨ��SPIx������յ�����	
}



void GenitopZk_Address(u32 Address) 
{
	u8 AddH, AddM, AddL;
	
	AddH = Address >> 16;
	AddM = Address >> 8;
	AddL = Address;
	
	SPI1_ReadWriteByte(0x03);	//һ���ȡ
	SPI1_ReadWriteByte(AddH);
	SPI1_ReadWriteByte(AddM);
	SPI1_ReadWriteByte(AddL);
	
//	SPI2_ReadWriteByte(0x0B);	//���ٶ�ȡ
//	SPI2_ReadWriteByte(AddH);
//	SPI2_ReadWriteByte(AddM);
//	SPI2_ReadWriteByte(AddL);
//	SPI2_ReadWriteByte(0xff);
}

/*
 *��ȡGB2312 8x16ASCII  ����֤
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
 *��ȡGB2312 16x16����  ����֤
 */
uint8_t GB2312_16X16(u8 MSB,u8 LSB,u8 *BUF)
{
	u8 i;

	u32 BaseAdd;	//�ֿ����ַ
	u32 Address;	//�ַ���оƬ�еĵ�ַ

	BaseAdd = BaseAdd_16X16;
	
	//�����ַ���ֵַ
	if(MSB >= 0xA1 && MSB <= 0xA9 && LSB >= 0xA1){
		Address = ((MSB - 0xA1) * 94 + (LSB - 0xA1)) * 32 + BaseAdd;
	}
	else if(MSB >= 0xB0 && MSB <= 0xF7 && LSB >= 0xA1){
		Address = ((MSB - 0xB0) * 94 + (LSB - 0xA1) + 846) * 32 + BaseAdd;
	}
	else{
	return 0;
	}

	//��ȡ�ַ���������
	SPI_CS=0;
	GenitopZk_Address(Address);
	for(i = 0; i < 32; i++){
		BUF[i] = SPI1_ReadWriteByte(0xff);
	}
	SPI_CS=1;
	return 1;
}



/*
 *��ȡGB2312 12X24ASCII  
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
 *��ȡGB2312 16x16����  ����֤
 */
uint8_t GB2312_24X24(u8 MSB,u8 LSB,u8 *BUF)
{
	u8 i;

	u32 BaseAdd;	//�ֿ����ַ
	u32 Address;	//�ַ���оƬ�еĵ�ַ

	BaseAdd = BaseAdd_24X24;
	
	//�����ַ���ֵַ
	if(MSB >= 0xA1 && MSB <= 0xA9 && LSB >= 0xA1){
		Address = ((MSB - 0xA1) * 94 + (LSB - 0xA1)) * 72 + BaseAdd;
	}
	else if(MSB >= 0xB0 && MSB <= 0xF7 && LSB >= 0xA1){
		Address = ((MSB - 0xB0) * 94 + (LSB - 0xA1) + 846) * 72 + BaseAdd;
	}
	else{
	return 1;
	}

	//��ȡ�ַ���������
	SPI_CS=0;
	GenitopZk_Address(Address);
	for(i = 0; i < 72; i++){
		BUF[i] = SPI1_ReadWriteByte(0xff);
	}
	SPI_CS=1;
	return 0;
}


