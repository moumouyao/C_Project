#include "Code.h"
#include "sw_global.h"
#include "malloc.h"
#include "memp.h"
#include "sw_crc16.h"
#include "os_cpu.h"
//--------------------------------------����ΪSPI��غ���---------------------------------------//
void init_SPI()
{
  GPIO_InitTypeDef  GPIO_InitStructure;
 
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);//ʹ��GPIOBʱ��
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG, ENABLE);//ʹ��GPIOGʱ��
	  //GPIOB14
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//���
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
  GPIO_Init(GPIOG, &GPIO_InitStructure);//��ʼ��
	  
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;//PB7
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//���
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
  GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ��
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;//
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//
  //GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
  GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ��
	PBout(7) = 1;   //��λ����
	Code_CS=1;			//SPI��ѡ��
	SPI1_Init();		//��ʼ��SPI
	SPI1_SetSpeed(SPI_BaudRatePrescaler_128);		//����4Ϊ21Mʱ��,����ģʽ

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

//--------------------------------------------����IC���-----------------------------------------//

void Rst_Code()//��λ����IC
{
	Code_Rst = 0;//��λ����
	delay_ms(8);
	Code_Rst = 1;
	delay_ms(30);
}
/*
������WriteCode
���ܣ���Codeд��һ������
������*cData:Ҫд�������
			cLong : Ҫд����ֽ���
			Block:�Ƿ���������  =1���� =0������
����ֵ 0=�ɹ�
			 1=ʧ��
*/
unsigned char WriteCode(unsigned char *cData,unsigned short iLong,unsigned char Block)
{
	unsigned short i;
	unsigned char cReturn=1;
	do
	{
		if(Code_Busy != 0)//���IC�������ݾͲ�д��
		{
			//Code_CS=0;//ʹ��Code
			for(i=0;i<iLong;i++)
			{
				SPI1_ReadWriteByte(*(cData+i));//д��һ������
			}
			//Code_CS=1;//ʧ��Code
			cReturn = 0;//д��ɹ�
		}
		else if(!Block)//�����������ͷ�
		{
			OSTimeDly(10);//�ͷ�CPU
			cReturn = 1;//д��ʧ��
		}
	}
	while(Block && cReturn);//���������ѭ������
	return cReturn;
}
/*
������ReadCode(�Զ��޳�֡ͷ�����ݳ���)
���ܣ���Code����һ��
������*cData:����������
      cLong : �����ĳ��ȣ��ֽ�����
			Block:�Ƿ���������  =1���� =0������
����ֵ 0=�ɹ�
			 1=ʧ��
*/
unsigned char ReadCode(unsigned char *cData,unsigned short *iLong,unsigned char Block)
{
	unsigned char cLongH;
	unsigned char cLongL;
	unsigned short i;
	unsigned char cReturn=1;
	do
	{
		if(Code_Busy == 0)//�Ƿ����оƬ�Ѿ�׼������
		{
			delay_us(8);//�ӳ�һ��
			Code_CS=0;//ʹ��Code
			cLongH = SPI1_ReadWriteByte(0x00);//�����ȸ�λ
			cLongL = SPI1_ReadWriteByte(0x00);//�����ȵ�λ
			*iLong = cLongH<<8|cLongL;//�ϳɳ���
			for(i=0;i<*iLong;i++)//��ȡʣ�µ�����
			{
				*(cData+i) = SPI1_ReadWriteByte(0x00);//��ȡ����
			}
			Code_CS=1;//ʧ��Code
			cReturn = 0;//�ɹ�
		}
		else if(!Block)//�����������ͷ�
		{
			OSTimeDly(1);//�ͷ�CPU
			cReturn = 1;//��ȡʧ��
		}
	}
	while(Block && cReturn);
	return cReturn;
}
/*
������EncryptData ��������
������cOriginal:ԭʼ����
			cOlong:ԭʼ���ݳ���
			cDlong:���ܺ�����ݳ���
			Block:�Ƿ���������  =1���� =0������
����ֵ��
			���ܺ������ָ��
*/
unsigned char* EncryptData(unsigned char *cOriginal,unsigned int iOlong,unsigned int *iDlong,unsigned char Block)
{
	unsigned char cTxdHead[8]={0x00,0x00,0x00,0x88,0x00,0x02,0x00};//ǰ��λ���ܳ��ȣ����һλ�ǳ������ݳ���
	unsigned short iTxdLong;//�ܳ���
	unsigned char cCompletion;//��ȫ���� 16������ȫ
	unsigned int cActualLong=0;//��ȫ��ʵ�ʳ���
	unsigned char cMoPaired;//�����IC���͵Ĵ�����1��240���ֽڣ�
	unsigned char cRemainder;//����240������ʣ���
	unsigned char *MoSbuff;//������IC�Ļ���
	unsigned char *MiSbuff;//����IC���ݻ���
	unsigned char *MiSbuffSum[20];//��Ž���IC���ݻ����ָ������(���ݳ�) ������240*20���ֽ�
	unsigned char *MiReturn;//��Ҫ���ص������ܻ�
	unsigned short MiLong;//����IC���ݵĳ��� 
	unsigned char err;//������Ϣ
	//-----ѭ��ϵ������
	unsigned short i;//240����������
	unsigned char j;
	unsigned int k;//ʧ�ܼ�����
	unsigned char z;
	cCompletion = 16 - (iOlong%16);
	cCompletion = cCompletion==16?0:cCompletion;//������Ҫ��ȫ���ֽ���
	cActualLong = iOlong + cCompletion;//���㲹ȫ���ʵ�ʳ���
	cMoPaired = cActualLong/240;//���㷢��240���ֽڵĴ���
	for(i=0;i<cMoPaired;i++)//һ�μ���240���ֽ�
	{
		MoSbuff = mymalloc(SRAMEX,240);//�����ڴ�
		for(j=0;j<240;j++)
		{
			MoSbuff[j] = cOriginal[j+(i*240)];//����Ҫ���͵�����
		}
		cTxdHead[6] = 240;//���ݳ���
		iTxdLong = cTxdHead[6] + 5;//�ܳ���=���ݳ���+5�ֽ�֡ͷ
		cTxdHead[0] = iTxdLong>>8;//�ܳ��ȸ�λ
		cTxdHead[1] = iTxdLong;//�ܳ��ȵ�λ
		Code_CS=0;//ʹ��Code
		WriteCode(cTxdHead,7,Block);//д��֡ͷ
		WriteCode(MoSbuff,240,Block);//д������
		Code_CS=1;//ʧ��Code
		myfree(SRAMEX,MoSbuff);//�ͷ��ڴ�
		//------������Ͽ�ʼ����
		MiSbuff = mymalloc(SRAMEX,255);//�����ڴ�
		k=0;
		do
		{
			k++;
			err = ReadCode(MiSbuff,&MiLong,0);//���ﲻ��������
		}
		while((err==1) && (k<6000));//ѭ������->����ʧ�ܲ���û�г�ʱ
		MiSbuffSum[i] = MiSbuff;//�������ݳ�
	}
	cRemainder = cActualLong - (cMoPaired*240);//�������240������ʣ�������
	if(cRemainder != 0)
	{
		MoSbuff = mymalloc(SRAMEX,cRemainder);//�����ڴ�
		for(j=0;j<cRemainder;j++)
		{
			MoSbuff[j] = ((j+(i*240))>=iOlong)?0:cOriginal[j+(i*240)];//����Ҫ���͵�����(���Ҫд������ݴ���ԭʼ���ݳ��Ⱦ���0��ȫ)
		}
		cTxdHead[6] = cRemainder;//���ݳ���
		iTxdLong = cTxdHead[6] + 5;//�ܳ���=���ݳ���+5�ֽ�֡ͷ
		cTxdHead[0] = iTxdLong>>8;//�ܳ��ȸ�λ
		cTxdHead[1] = iTxdLong;//�ܳ��ȵ�λ
		Code_CS=0;//ʹ��Code
		WriteCode(cTxdHead,7,Block);//д��֡ͷ
		WriteCode(MoSbuff,cRemainder,Block);//д������
		Code_CS=1;//ʧ��Code
		myfree(SRAMEX,MoSbuff);//�ͷ��ڴ�
		//------������Ͽ�ʼ����
		MiSbuff = mymalloc(SRAMEX,255);//�����ڴ�
		k=0;
		do
		{
			k++;
			err = ReadCode(MiSbuff,&MiLong,Block);//�����������IC������
		}
		while((err==1) && (k<6000));//ѭ������->����ʧ�ܲ���û�г�ʱ
		MiSbuffSum[i] = MiSbuff;//�������ݳ�
	}
	//------------�����������
	MiReturn = mymalloc(SRAMEX,cRemainder+(i*240));//�����ڴ�
	*iDlong = cRemainder+(i*240);//���������ܳ���
	for(j=0;j<=i;j++)//�����ݳص����ݰ�˳��д�������ܻ�
	{
		k = j==i?cRemainder:240;//��������ݳ����һ������ �������ݲ���240 ���ǵ���ʣ������
		for(z=0;z<k;z++)
		{
			MiReturn[(j*240) + z] = MiSbuffSum[j][z];//���ݳأ���j�����ݵĵ�k���ֽ�
		}
		myfree(SRAMEX,MiSbuffSum[j]);//�ͷ��ڴ�
	}
	return MiReturn;//���ؽ��յ�������
}

/*
������DecryptData ��������
������cOriginal:ԭʼ����
			cOlong:ԭʼ���ݳ���
			cDlong:���ܺ�����ݳ���
			Block:�Ƿ���������  =1���� =0������
����ֵ��
			���ܺ������ָ��
*/
unsigned char* DecryptData(unsigned char *cOriginal,unsigned int iOlong,unsigned int *iDlong,unsigned char Block)
{
	unsigned char cTxdHead[8]={0x00,0x00,0x00,0x88,0x01,0x02,0x00};//ǰ��λ���ܳ��ȣ����һλ�ǳ������ݳ���
	unsigned short iTxdLong;//�ܳ���
	unsigned char cCompletion;//��ȫ���� 16������ȫ
	unsigned int cActualLong=0;//��ȫ��ʵ�ʳ���
	unsigned char cMoPaired;//�����IC���͵Ĵ�����1��240���ֽڣ�
	unsigned char cRemainder;//����240������ʣ���
	unsigned char *MoSbuff;//������IC�Ļ���
	unsigned char *MiSbuff;//����IC���ݻ���
	unsigned char *MiSbuffSum[20];//��Ž���IC���ݻ����ָ������(���ݳ�) ������240*20���ֽ�
	unsigned char *MiReturn;//��Ҫ���ص������ܻ�
	unsigned short MiLong;//����IC���ݵĳ��� 
	unsigned char err;//������Ϣ
	//-----ѭ��ϵ������
	unsigned short i;//240����������
	unsigned char j;
	unsigned int k;//ʧ�ܼ�����
	unsigned char z;
	cCompletion = 16 - (iOlong%16);
	cCompletion = cCompletion==16?0:cCompletion;//������Ҫ��ȫ���ֽ���(���Ķ���16�ı����������ʡ��)
	cActualLong = iOlong + cCompletion;//���㲹ȫ���ʵ�ʳ���
	cMoPaired = cActualLong/240;//���㷢��240���ֽڵĴ���
	for(i=0;i<cMoPaired;i++)//һ�ν���240���ֽ�
	{
		MoSbuff = mymalloc(SRAMEX,240);//�����ڴ�
		for(j=0;j<240;j++)
		{
			MoSbuff[j] = cOriginal[j+(i*240)];//����Ҫ���͵�����
		}
		cTxdHead[6] = 240;//���ݳ���
		iTxdLong = cTxdHead[6] + 5;//�ܳ���=���ݳ���+5�ֽ�֡ͷ
		cTxdHead[0] = iTxdLong>>8;//�ܳ��ȸ�λ
		cTxdHead[1] = iTxdLong;//�ܳ��ȵ�λ
		Code_CS=0;//ʹ��Code
		WriteCode(cTxdHead,7,Block);//д��֡ͷ
		WriteCode(MoSbuff,240,Block);//д������
		Code_CS=1;//ʧ��Code
		myfree(SRAMEX,MoSbuff);//�ͷ��ڴ�
		//------������Ͽ�ʼ����
		MiSbuff = mymalloc(SRAMEX,255);//�����ڴ�
		k=0;
		do
		{
			k++;
			err = ReadCode(MiSbuff,&MiLong,0);//���ﲻ��������
		}
		while((err==1) && (k<6000));//ѭ������->����ʧ�ܲ���û�г�ʱ
		MiSbuffSum[i] = MiSbuff;//�������ݳ�
	}
	cRemainder = cActualLong - (cMoPaired*240);//�������240������ʣ�������
	if(cRemainder != 0)
	{
		MoSbuff = mymalloc(SRAMEX,cRemainder);//�����ڴ�
		for(j=0;j<cRemainder;j++)
		{
			MoSbuff[j] = ((j+(i*240))>=iOlong)?0:cOriginal[j+(i*240)];//����Ҫ���͵�����(���Ҫд������ݴ���ԭʼ���ݳ��Ⱦ���0��ȫ�����ܹ��̿���ʡ��)
		}
		cTxdHead[6] = cRemainder;//���ݳ���
		iTxdLong = cTxdHead[6] + 5;//�ܳ���=���ݳ���+5�ֽ�֡ͷ
		cTxdHead[0] = iTxdLong>>8;//�ܳ��ȸ�λ
		cTxdHead[1] = iTxdLong;//�ܳ��ȵ�λ
		Code_CS=0;//ʹ��Code
		WriteCode(cTxdHead,7,Block);//д��֡ͷ
		WriteCode(MoSbuff,cRemainder,Block);//д������
		Code_CS=1;//ʧ��Code
		myfree(SRAMEX,MoSbuff);//�ͷ��ڴ�
		//------������Ͽ�ʼ����
		MiSbuff = mymalloc(SRAMEX,255);//�����ڴ�
		k=0;
		do
		{
			k++;
			err = ReadCode(MiSbuff,&MiLong,Block);//�����������IC������
		}
		while((err==1) && (k<6000));//ѭ������->����ʧ�ܲ���û�г�ʱ
		MiSbuffSum[i] = MiSbuff;//�������ݳ�
	}
	//------------�����������
	MiReturn = mymalloc(SRAMEX,cRemainder+(i*240));//�����ڴ�
	for(j=0;(MiSbuff[j]!=0x03)&&(j<cRemainder);j++);//�������һ��240�����ĳ���
	j++;
	*iDlong = j+(i*240);//���������ܳ���
	for(j=0;j<=i;j++)//�����ݳص����ݰ�˳��д�������ܻ�
	{
		k = j==i?cRemainder:240;//��������ݳ����һ������ �������ݲ���240 ���ǵ���ʣ������
		for(z=0;z<k;z++)
		{
			MiReturn[(j*240) + z] = MiSbuffSum[j][z];//���ݳأ���j�����ݵĵ�k���ֽ�
		}
		myfree(SRAMEX,MiSbuffSum[j]);//�ͷ��ڴ�
	}
	return MiReturn;//���ؽ��յ�������
}

//=================����Ϊ���ܽ��ܽ���==================//

void TestCode(void *p_arg)
{
	(void)p_arg;                				// 'p_arg' ��û���õ�����ֹ��������ʾ����
	StrFrameHeader *mInHead=NULL; //�����������������
	StrFrameHeader *mOutHead=NULL; //�����������������
	unsigned char err;
	unsigned char *cOutData;//���ܺ��ԭ��
	static unsigned char cOutError[]={0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff};//�����Ļظ�
	unsigned int iOutLong;//���ܺ��ԭ�ĳ���
	INT8U *memp_buf;//��������
	unsigned int i;
	unsigned char cUartFlag=0;//���ڱ�־λ
		while(1)
		{
			//==================��֡���ݺϳ����׼��ȥ����
			if(*mInHead->pdata != 0x02)//�ж�����֡ͷ
			{
				cOutData = cOutError;//ָ��ظ�����
				iOutLong = 8;
				memp_free(MEMP_PBUF_POOL,(void *)mInHead);//�ͷ�POOL�ڴ�
			}
			else//������ȷ
			{
				for(i=0;i<mInHead->len;i++)//����֡ͷ��֡βֻ��������
				{
					mInHead->pdata[i] = mInHead->pdata[i+1];
				}
				mInHead->len -= 2;//��ȥ֡ͷ֡β�ĳ���
				cOutData = DecryptData(mInHead->pdata,mInHead->len,&iOutLong,1);//�����ȴ��������
				memp_free(MEMP_PBUF_POOL,(void *)mInHead);//�ͷ�POOL�ڴ�
			}
			
			//================��������׼��ת��������������
			memp_buf = memp_malloc(MEMP_PBUF_POOL);//����һ���ڴ�pool
			mOutHead = (StrFrameHeader *)memp_buf;
			mOutHead->type = HEAD_ETH_TYPE;
			mOutHead->pdata = memp_buf + sizeof(StrFrameHeader);

			memcpy(mOutHead->pdata,cOutData,iOutLong);
			mOutHead->len=iOutLong;					

			myfree(SRAMEX,cOutData);//�ͷ��ڴ�(��������)
			do
			{
				err = OSQPost(CtrlCentRecMsgQeue,(void *)mOutHead);//�ѿ��������ݷ��͸�������
			}
			while(err != OS_ERR_NONE);//�����ȴ��������

			//===================�����Ǵ������������������============================//
			mInHead = OSQPend(UdpTranMsgQeue,0,&err);   	//�����ȴ����մ�������
			cOutData = EncryptData(mInHead->pdata,mInHead->len,&iOutLong,1);//�����ȴ��������
			memp_free(MEMP_PBUF_POOL,(void *)mInHead);//�ͷ�POOL�ڴ�
			//========��������׼��ת������������
			memp_buf = memp_malloc(MEMP_PBUF_POOL);//����һ���ڴ�pool
			mOutHead = (StrFrameHeader *)memp_buf;
			mOutHead->type = HEAD_ETH_TYPE;
			mOutHead->pdata = memp_buf + sizeof(StrFrameHeader);
			memp_buf =mOutHead->pdata;         //��λ��������ָ��

			memcpy(mOutHead->pdata,cOutData,iOutLong);
			mOutHead->len=iOutLong;	

			myfree(SRAMEX,cOutData);//�ͷ��ڴ�(��������)
			if(cUartFlag==0)//������Ǵ�������
			{
				do
				{
					err = OSQPost(Dis_En_CodeQeue,(void *)mOutHead);//�ѿ��������ݷ��͸���������
				}
				while(err != OS_ERR_NONE);//�����ȴ��������
			}
			else
			{
				for(i=0;i<mOutHead->len;i++)
				{
				printf("%c",mOutHead->pdata[i]);//��������
				}
				memp_free(MEMP_PBUF_POOL,(void *)mOutHead);//�ͷ�POOL�ڴ�
			}
			//==================�ж��Ƿ���Ҫ�޸���Կ========================//
			if(g_cNewCodeKey != NULL)//�������Կָ�������ݣ�������Ҫ�޸���Կ
			{		
				AlterCodeKey(0);//�޸���Կ
				myfree(SRAMEX,g_cNewCodeKey);//�ͷ��ڴ�
				g_cNewCodeKey = NULL;//ָ���
			}
		}
	
}

void GetCodeKeyAck(StrFrameHeader *frame_head,unsigned char* p,unsigned short*len)//������Կ���ظ���λ��
{
	unsigned char *cData;//���ĵ�����ָ��
	unsigned char i;//���ڼ���
	unsigned char *p1;//ָ��P,���ڼ��������ܳ���
	unsigned short crc16;//CRCУ��
	
	cData = frame_head->pdata;//����
	p1 = p;
	g_cNewCodeKey = mymalloc(SRAMEX,16);//�����ڴ�
	for(i=0;i<16;i++)
	{
		g_cNewCodeKey[i] = cData[i+5];//д����Կ����
	}
	i+=(2+5);//ָ������֡β
	if(cData[i] == 0x03)//�ж�֡β
	{
		//д��ظ��ɹ��ı���
		*p++ = 0x02;         //֡ͷ
		*p++ = HostAddrH;    //����������ַ��λ
		*p++ = HostAddrL;    //����������ַ��λ
		*p++ = '0';
	}
	else//���Ĵ���ظ�ʧ��
	{
		//д��ظ�ʧ�ܵı���
		*p++ = 0x02;         //֡ͷ
		*p++ = HostAddrH;    //����������ַ��λ
		*p++ = HostAddrL;    //����������ַ��λ
		*p++ = '1';
		myfree(SRAMEX,g_cNewCodeKey);//�ͷ��ڴ�
		g_cNewCodeKey = NULL;//ָ���
	}
	//=======CRC����
	crc16 = SWCRC16(p1+1,p-p1-1);
	*p++ = (uint8_t)(crc16>>8); //У����ֽ���ǰ
	*p++ = (uint8_t)(crc16);    //У����ֽ��ں�
	*p++ = 0x03;              //֡β
  *len = p-p1;   //���ݸ�����֡ͷ+��ַ+����+crc+֡δ
}

void AlterCodeKey(unsigned char cmd)
{
	OS_CPU_SR cpu_sr;
	unsigned char cCommandErasure[] ={0x00,0x05,0x80,0x0E,0x00,0x00,0x00};//��������
	unsigned char cCommandNewFile[]={0x00,0x0C,0x80,0xE0,0x00,0x00,0x07,0x3F,0x03,0x00,0xFF,0xF0,0xFF,0xFF};//�����ļ�ָ��
	unsigned char cCommandEnCodeKey[]={0x00,0x1A,0x80,0xD4,0x01,0x02,0x15,0x30,0xF0,0xFA,0x01,0x04,\
	0x01,0x23,0x45,0x67,0x89,0xab,0xcd,0xef,0xfe,0xdc,0xba,0x98,0x76,0x54,0x32,0x10};//д��SM4�����ܳ�
	unsigned char cCommandDisCodeKey[]={0x00,0x1A,0x80,0xD4,0x01,0x02,0x15,0x31,0xF0,0xFA,0x01,0x04,\
	0x01,0x23,0x45,0x67,0x89,0xab,0xcd,0xef,0xfe,0xdc,0xba,0x98,0x76,0x54,0x32,0x10};//д��SM4�����ܳ�
	unsigned char *cReData;//����COdeоƬ������
	unsigned short iLong;//���ݳ���
	unsigned char err;
	unsigned int i;
	
	//============�������ܳ�===============//
	if(cmd == 0)//����д��
	{
		for(i=0;i<16;i++)
		{
			cCommandEnCodeKey[i+12] = g_cNewCodeKey[i];
			cCommandDisCodeKey[i+12] = g_cNewCodeKey[i];
		}
	}
	cReData = mymalloc(SRAMEX,20);//�����ڴ�
	OS_ENTER_CRITICAL(); //���ж�

	//======================д���������==================//
	Code_CS=0;//ʹ��Code
	WriteCode(cCommandErasure,7,1);//д������
	Code_CS=1;//ʧ��Code
	i=0;
	do
	{
		i++;
		err = ReadCode(cReData,&iLong,0);//���ﲻ��������
	}
	while((err==1) && (i<6000));//ѭ������->����ʧ�ܲ���û�г�ʱ
	delay_ms(100);
	Rst_Code();//��λһ��IC
	//==============д�봴���ļ�ָ��================//
	Code_CS=0;//ʹ��Code
	WriteCode(cCommandNewFile,14,1);//д������
	Code_CS=1;//ʧ��Code
	i=0;
	do
	{
		i++;
		err = ReadCode(cReData,&iLong,0);//���ﲻ��������
	}
	while((err==1) && (i<6000));//ѭ������->����ʧ�ܲ���û�г�ʱ
	delay_ms(100);
	Rst_Code();//��λһ��IC
	//============д������ܳ�==================//
	Code_CS=0;//ʹ��Code
	WriteCode(cCommandEnCodeKey,28,1);//д������
	Code_CS=1;//ʧ��Code
	i=0;
	do
	{
		i++;
		err = ReadCode(cReData,&iLong,0);//���ﲻ��������
	}
	while((err==1) && (i<6000));//ѭ������->����ʧ�ܲ���û�г�ʱ
	delay_ms(100);
	Rst_Code();//��λһ��IC
	//============д������ܳ�==================//
	Code_CS=0;//ʹ��Code
	WriteCode(cCommandDisCodeKey,28,1);//д������
	Code_CS=1;//ʧ��Code
	i=0;
	do
	{
		i++;
		err = ReadCode(cReData,&iLong,0);//���ﲻ��������
	}
	while((err==1) && (i<6000));//ѭ������->����ʧ�ܲ���û�г�ʱ
	delay_ms(100);
	Rst_Code();//��λһ��IC
	myfree(SRAMEX,cReData);//�ͷ��ڴ�
	OS_EXIT_CRITICAL();  //���ж�
}

