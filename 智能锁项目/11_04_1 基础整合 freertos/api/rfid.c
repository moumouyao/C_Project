/*
****************************************************************************************
* Copyright: 2008-2019����ӯ��Ƽ����޹�˾��All rights reserved
* File name: None
* Description: None
* Dependent file: None
* Author: weihaoMo
* Version: V1.0
* Date: 2019-11-29
* History: None
* Company web site: http://www.edu118.com/
* Company net schools: http://www.edu118.cn/
* Notice: ������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;�� 
*********************************��Ȩ���У�����ؾ�*********************************
****************************************************************************************
*/

/*
****************************************************************************************
* INCLUDES (ͷ�ļ�����)
****************************************************************************************
*/
#include "rfid.h"
#include "spi3.h"
#include "delay.h"
#include "string.h"
#include "stdio.h"

/*
****************************************************************************************
* CONSTANTS (��������)
****************************************************************************************
*/


/*
****************************************************************************************
* TYPEDEFS (���Ͷ���)
****************************************************************************************
*/


/*
****************************************************************************************
* LOCAL VARIABLES (��̬����)
****************************************************************************************
*/


/*
****************************************************************************************
* LOCAL FUNCTIONS DECLARE (��̬��������)
****************************************************************************************
*/


/*
****************************************************************************************
* LOCAL FUNCTIONS (��̬����)
****************************************************************************************
*/


/*
****************************************************************************************
* PUBLIC FUNCTIONS (ȫ�ֺ���)
****************************************************************************************
*/

/*
****************************************************************************************
* Function: RFID_Init
* Description: ��ʼ��RFID��ʹ�ܽ�
* Input: None
* Output: None
* Return: None
* Author: weihaoMo
* Others: None
* Date of completion: 2019-11-29
* Date of last modify: 2019-11-29
****************************************************************************************
*/
void RFID_Init(void)
{
	//RFID_SPI3_CS  ---PA15
	//RFID_SPI3_RST ---PD2
	SPI3PinInit( );
	
  RCC->APB2ENR |=1<<0 |1<<3;  //ʹ��A��Dʱ��
	
	/*CS-----PA15*/ //������� 
	RFID_CS_GPIO->MODER &=~(0x3<<(RFID_CS_PIN*2));
	RFID_CS_GPIO->MODER |=0X1<<(RFID_CS_PIN*2);
	RFID_CS_GPIO->OTYPER &=~(1<<RFID_CS_PIN);
	RFID_CSH;
	
	/*RST-----PD4*/ //������� 
	RFID_RST_GPIO->MODER &=~(0x3<<(RFID_RST_PIN*2));
	RFID_RST_GPIO->MODER |=0X1<<(RFID_RST_PIN*2);
	RFID_RST_GPIO->OTYPER &=~(1<<RFID_RST_PIN);
	RFID_RSTH;
	
	Reset_RC522();    //��������
	M500PcdConfigISOType( 'A' );
}

/*
****************************************************************************************
* Function: Reset_RC522
* Description: ��λRC522
* Input: None
* Output: None
* Return: None
* Author: weihaoMo
* Others: None
* Date of completion: 2019-11-29
* Date of last modify: 2019-11-29
****************************************************************************************
*/
void Reset_RC522(void)
{
	PcdReset();       //��    �ܣ���λRC522
	PcdAntennaOff();  //�ر�����
	PcdAntennaOn();   //��������
}    

/*
****************************************************************************************
* Function: PcdRequest
* Description: Ѱ��
* Input: req_code[IN]:Ѱ����ʽ
                0x52 = Ѱ��Ӧ�������з���14443A��׼�Ŀ�
                0x26 = Ѱδ��������״̬�Ŀ�
* Output:pTagType[OUT]����Ƭ���ʹ���
                0x4400 = Mifare_UltraLight
                0x0400 = Mifare_One(S50)
                0x0200 = Mifare_One(S70)
                0x0800 = Mifare_Pro(X)
                0x4403 = Mifare_DESFire
* Return: �ɹ�����MI_OK
* Author: weihaoMo
* Others: None
* Date of completion: 2019-11-29
* Date of last modify: 2019-11-29
****************************************************************************************
*/
uint8_t PcdRequest(uint8_t   req_code,uint8_t *pTagType)
{
	uint8_t   status;  
	uint8_t   unLen;
	uint8_t   ucComMF522Buf[MAXRLEN]; 

	ClearBitMask(Status2Reg,0x08);
	WriteRawRC(BitFramingReg,0x07);
	SetBitMask(TxControlReg,0x03);
 
	ucComMF522Buf[0] = req_code;

	status = PcdComMF522(PCD_TRANSCEIVE,ucComMF522Buf,1,ucComMF522Buf,&unLen);

	if ((status == MI_OK) && (unLen == 0x10))
	{    
		*pTagType     = ucComMF522Buf[0];
		*(pTagType+1) = ucComMF522Buf[1];
	}
	else
	{   status = MI_ERR;   }
   
	return status;
}

/*
****************************************************************************************
* Function: PcdAnticoll
* Description: ����ײ
* Input: None
* Output: pSnr[OUT]:��Ƭ���кţ�4�ֽ�
* Return: �ɹ�����MI_OK
* Author: weihaoMo
* Others: None
* Date of completion: 2019-11-29
* Date of last modify: 2019-11-29
****************************************************************************************
*/
uint8_t PcdAnticoll(uint8_t *pSnr)
{
    uint8_t   status;
    uint8_t   i,snr_check=0;
    uint8_t   unLen;
    uint8_t   ucComMF522Buf[MAXRLEN]; 
    

    ClearBitMask(Status2Reg,0x08);
    WriteRawRC(BitFramingReg,0x00);
    ClearBitMask(CollReg,0x80);
 
    ucComMF522Buf[0] = PICC_ANTICOLL1;
    ucComMF522Buf[1] = 0x20;

    status = PcdComMF522(PCD_TRANSCEIVE,ucComMF522Buf,2,ucComMF522Buf,&unLen);

    if (status == MI_OK)
    {
    	 for (i=0; i<4; i++)
         {   
             *(pSnr+i)  = ucComMF522Buf[i];
             snr_check ^= ucComMF522Buf[i];
         }
         if (snr_check != ucComMF522Buf[i])
         {   status = MI_ERR;    }
    }
    
    SetBitMask(CollReg,0x80);
    return status;
}

/*
****************************************************************************************
* Function: PcdSelect
* Description: ѡ����Ƭ
* Input: pSnr[IN]:��Ƭ���кţ�4�ֽ�
* Output: None
* Return: �ɹ�����MI_OK
* Author: weihaoMo
* Others: None
* Date of completion: 2019-11-29
* Date of last modify: 2019-11-29
****************************************************************************************
*/
char PcdSelect(uint8_t *pSnr)
{
    char   status;
    uint8_t   i;
    uint8_t   unLen;
    uint8_t   ucComMF522Buf[MAXRLEN]; 
    
    ucComMF522Buf[0] = PICC_ANTICOLL1;
    ucComMF522Buf[1] = 0x70;
    ucComMF522Buf[6] = 0;
    for (i=0; i<4; i++)
    {
    	ucComMF522Buf[i+2] = *(pSnr+i);
    	ucComMF522Buf[6]  ^= *(pSnr+i);
    }
    CalulateCRC(ucComMF522Buf,7,&ucComMF522Buf[7]);
  
    ClearBitMask(Status2Reg,0x08);

    status = PcdComMF522(PCD_TRANSCEIVE,ucComMF522Buf,9,ucComMF522Buf,&unLen);
    
    if ((status == MI_OK) && (unLen == 0x18))
    {   status = MI_OK;  }
    else
    {   status = MI_ERR;    }

    return status;
}
   
/*
****************************************************************************************
* Function: PcdAuthState
* Description: ��֤��Ƭ����
* Input: auth_mode[IN]: ������֤ģʽ
                 0x60 = ��֤A��Կ
                 0x61 = ��֤B��Կ 
								addr[IN]�����ַ
* Output: pKey[OUT]������
					pSnr[OUT]����Ƭ���кţ�4�ֽ�
* Return: �ɹ�����MI_OK
* Author: weihaoMo
* Others: None
* Date of completion: 2019-11-29
* Date of last modify: 2019-11-29
****************************************************************************************
*/
char PcdAuthState(uint8_t   auth_mode,uint8_t   addr,uint8_t *pKey,uint8_t *pSnr)
{
    char   status;
    uint8_t   unLen;
//     uint8_t   i;
		uint8_t	 ucComMF522Buf[MAXRLEN]; 

    ucComMF522Buf[0] = auth_mode;
    ucComMF522Buf[1] = addr;
//    for (i=0; i<6; i++)
//    {    ucComMF522Buf[i+2] = *(pKey+i);   }
//    for (i=0; i<6; i++)
//    {    ucComMF522Buf[i+8] = *(pSnr+i);   }
    memcpy(&ucComMF522Buf[2], pKey, 6); 
    memcpy(&ucComMF522Buf[8], pSnr, 4); 
    
    status = PcdComMF522(PCD_AUTHENT,ucComMF522Buf,12,ucComMF522Buf,&unLen);
    if ((status != MI_OK) || (!(ReadRawRC(Status2Reg) & 0x08)))
    {   status = MI_ERR;   }
    
    return status;
}

/*
****************************************************************************************
* Function: PcdRead
* Description: ��ȡM1��һ������
* Input: addr[IN]�����ַ
* Output: p [OUT]�����������ݣ�16�ֽ�
* Return: �ɹ�����MI_OK
* Author: weihaoMo
* Others: None
* Date of completion: 2019-11-29
* Date of last modify: 2019-11-29
****************************************************************************************
*/
char PcdRead(uint8_t   addr,uint8_t *p )
{
    char   status;
    uint8_t   unLen;
    uint8_t   i,ucComMF522Buf[MAXRLEN]; 

    ucComMF522Buf[0] = PICC_READ;
    ucComMF522Buf[1] = addr;
    CalulateCRC(ucComMF522Buf,2,&ucComMF522Buf[2]);
   
    status = PcdComMF522(PCD_TRANSCEIVE,ucComMF522Buf,4,ucComMF522Buf,&unLen);
    if ((status == MI_OK) && (unLen == 0x90))
 //   {   memcpy(p , ucComMF522Buf, 16);   }
    {
        for (i=0; i<16; i++)
        {    *(p +i) = ucComMF522Buf[i];   }
    }
    else
    {   status = MI_ERR;   }
    
    return status;
}

/*
****************************************************************************************
* Function: PcdWrite
* Description: д���ݵ�M1��һ��
* Input: addr[IN]�����ַ
* Output: p [OUT]�����������ݣ�16�ֽ�
* Return: �ɹ�����MI_OK
* Author: weihaoMo
* Others: None
* Date of completion: 2019-11-29
* Date of last modify: 2019-11-29
****************************************************************************************
*/
char PcdWrite(uint8_t   addr,uint8_t *p )
{
    char   status;
    uint8_t   unLen;
    uint8_t   i,ucComMF522Buf[MAXRLEN]; 
    
    ucComMF522Buf[0] = PICC_WRITE;
    ucComMF522Buf[1] = addr;
    CalulateCRC(ucComMF522Buf,2,&ucComMF522Buf[2]);
 
    status = PcdComMF522(PCD_TRANSCEIVE,ucComMF522Buf,4,ucComMF522Buf,&unLen);

    if ((status != MI_OK) || (unLen != 4) || ((ucComMF522Buf[0] & 0x0F) != 0x0A))
    {   status = MI_ERR;   }
        
    if (status == MI_OK)
    {
        //memcpy(ucComMF522Buf, p , 16);
        for (i=0; i<16; i++)
        {    
        	ucComMF522Buf[i] = *(p +i);   
        }
        CalulateCRC(ucComMF522Buf,16,&ucComMF522Buf[16]);

        status = PcdComMF522(PCD_TRANSCEIVE,ucComMF522Buf,18,ucComMF522Buf,&unLen);
        if ((status != MI_OK) || (unLen != 4) || ((ucComMF522Buf[0] & 0x0F) != 0x0A))
        {   status = MI_ERR;   }
    }
    
    return status;
}

/*
****************************************************************************************
* Function: PcdHalt
* Description: ���Ƭ��������״̬
* Input: None
* Output: None
* Return: �ɹ�����MI_OK
* Author: weihaoMo
* Others: None
* Date of completion: 2019-11-29
* Date of last modify: 2019-11-29
****************************************************************************************
*/
char PcdHalt(void)
{
//    uint8_t   status;
    uint8_t   unLen;
    uint8_t   ucComMF522Buf[MAXRLEN]; 

    ucComMF522Buf[0] = PICC_HALT;
    ucComMF522Buf[1] = 0;
    CalulateCRC(ucComMF522Buf,2,&ucComMF522Buf[2]);
 
//    status = PcdComMF522(PCD_TRANSCEIVE,ucComMF522Buf,4,ucComMF522Buf,&unLen);
		PcdComMF522(PCD_TRANSCEIVE,ucComMF522Buf,4,ucComMF522Buf,&unLen);
    return MI_OK;
}

/*
****************************************************************************************
* Function: CalulateCRC
* Description: ��MF522����CRC16����
* Input: None
* Output: None
* Return: None
* Author: weihaoMo
* Others: None
* Date of completion: 2019-11-29
* Date of last modify: 2019-11-29
****************************************************************************************
*/
void CalulateCRC(uint8_t *pIn ,uint8_t   len,uint8_t *pOut )
{
    uint8_t   i,n;
    ClearBitMask(DivIrqReg,0x04);
    WriteRawRC(CommandReg,PCD_IDLE);
    SetBitMask(FIFOLevelReg,0x80);
    for (i=0; i<len; i++)
    {   WriteRawRC(FIFODataReg, *(pIn +i));   }
    WriteRawRC(CommandReg, PCD_CALCCRC);
    i = 0xFF;
    do 
    {
        n = ReadRawRC(DivIrqReg);
        i--;
    }
    while ((i!=0) && !(n&0x04));
    pOut [0] = ReadRawRC(CRCResultRegL);
    pOut [1] = ReadRawRC(CRCResultRegM);
}

/*
****************************************************************************************
* Function: PcdReset
* Description: ��λRC522
* Input: None
* Output: None
* Return: �ɹ�����MI_OK
* Author: weihaoMo
* Others: None
* Date of completion: 2019-11-29
* Date of last modify: 2019-11-29
****************************************************************************************
*/
uint8_t PcdReset(void)
{
	//PORTD|=(1<<RC522RST);
	RFID_RSTH;
  Delay_us(2);

	//PORTD&=~(1<<RC522RST);
	RFID_RSTL;
   Delay_us(2);

	//PORTD|=(1<<RC522RST);
	RFID_RSTH;
  Delay_us(2);

  WriteRawRC(CommandReg,PCD_RESETPHASE);
	WriteRawRC(CommandReg,PCD_RESETPHASE);
  Delay_us(2);
    
	WriteRawRC(ModeReg,0x3D);            //��Mifare��ͨѶ��CRC��ʼֵ0x6363
	WriteRawRC(TReloadRegL,30);           
	WriteRawRC(TReloadRegH,0);
	WriteRawRC(TModeReg,0x8D);
	WriteRawRC(TPrescalerReg,0x3E);
	
	WriteRawRC(TxAutoReg,0x40);//����Ҫ
   
   return MI_OK;
}

/*
****************************************************************************************
* Function: M500PcdConfigISOType
* Description: ����RC632�Ĺ�����ʽ
* Input: None
* Output: None
* Return: None
* Author: weihaoMo
* Others: None
* Date of completion: 2019-11-29
* Date of last modify: 2019-11-29
****************************************************************************************
*/
char M500PcdConfigISOType(uint8_t   type)
{
   if (type == 'A')                     //ISO14443_A
   { 
			ClearBitMask(Status2Reg,0x08);
			WriteRawRC(ModeReg,0x3D);//3F
			WriteRawRC(RxSelReg,0x86);//84
			WriteRawRC(RFCfgReg,0x7F);   //4F
			WriteRawRC(TReloadRegL,30);//tmoLength);// TReloadVal = 'h6a =tmoLength(dec) 
			WriteRawRC(TReloadRegH,0);
			WriteRawRC(TModeReg,0x8D);
			WriteRawRC(TPrescalerReg,0x3E);
			Delay_us(2);
			PcdAntennaOn();
   }
   else{ return 1; }
   
   return MI_OK;
}

/*
****************************************************************************************
* Function: ReadRawRC
* Description: ��RC632�Ĵ���
* Input: Address[IN]:�Ĵ�����ַ
* Output: None
* Return: ������ֵ
* Author: weihaoMo
* Others: None
* Date of completion: 2019-11-29
* Date of last modify: 2019-11-29
****************************************************************************************
*/
uint8_t ReadRawRC(uint8_t   Address)
{
		uint8_t   ucAddr;
		uint8_t   ucResult=0;
		RFID_CSL;
		ucAddr = ((Address<<1)&0x7E)|0x80;

		SPI3TransferByte(ucAddr);
		ucResult=SPI3TransferByte(0);
		RFID_CSH;
		return ucResult;
}

/*
****************************************************************************************
* Function: WriteRawRC
* Description: дRC632�Ĵ���
* Input: Address[IN]:�Ĵ�����ַ
				 value[IN]:д���ֵ
* Output: None
* Return: None
* Author: weihaoMo
* Others: None
* Date of completion: 2019-11-29
* Date of last modify: 2019-11-29
****************************************************************************************
*/
void WriteRawRC(uint8_t   Address, uint8_t   value)
{  
    uint8_t   ucAddr;
//	uint8_t tmp;

		RFID_CSL;
    ucAddr = ((Address<<1)&0x7E);

		SPI3TransferByte(ucAddr);
		SPI3TransferByte(value);
		RFID_CSH;

// 		tmp=ReadRawRC(Address);

// 		if(value!=tmp)
// 		printf("wrong\n");
}

/*
****************************************************************************************
* Function: SetBitMask
* Description: ��RC522�Ĵ���λ
* Input: reg[IN]:�Ĵ�����ַ
				 mask[IN]:��λֵ
* Output: None
* Return: None
* Author: weihaoMo
* Others: None
* Date of completion: 2019-11-29
* Date of last modify: 2019-11-29
****************************************************************************************
*/
void SetBitMask(uint8_t   reg,uint8_t   mask)  
{
    char   tmp = 0x0;
    tmp = ReadRawRC(reg);
    WriteRawRC(reg,tmp | mask);  // set bit mask
}

/*
****************************************************************************************
* Function: ClearBitMask
* Description: ��RC522�Ĵ���λ
* Input: reg[IN]:�Ĵ�����ַ
				 mask[IN]:��λֵ
* Output: None
* Return: None
* Author: weihaoMo
* Others: None
* Date of completion: 2019-11-29
* Date of last modify: 2019-11-29
****************************************************************************************
*/
void ClearBitMask(uint8_t   reg,uint8_t   mask)  
{
    char   tmp = 0x0;
    tmp = ReadRawRC(reg);
    WriteRawRC(reg, tmp & ~mask);  // clear bit mask
} 

/*
****************************************************************************************
* Function: PcdComMF522
* Description: ͨ��RC522��ISO14443��ͨѶ
* Input: Command[IN]:RC522������
				 pIn [IN]:ͨ��RC522���͵���Ƭ������
				 InLenByte[IN]:�������ݵ��ֽڳ���
* Output: pOut [OUT]:���յ��Ŀ�Ƭ��������
					*pOutLenBit[OUT]:�������ݵ�λ����
* Return: None
* Author: weihaoMo
* Others: None
* Date of completion: 2019-11-29
* Date of last modify: 2019-11-29
****************************************************************************************
*/
char PcdComMF522(uint8_t   Command, 
                 uint8_t *pIn , 
                 uint8_t   InLenByte,
                 uint8_t *pOut , 
                 uint8_t *pOutLenBit)
{
    char   status = MI_ERR;
    uint8_t   irqEn   = 0x00;
    uint8_t   waitFor = 0x00;
    uint8_t   lastBits;
    uint8_t   n;
    uint16_t   i;
    switch (Command)
    {
        case PCD_AUTHENT:
													irqEn   = 0x12;
													waitFor = 0x10;
													break;
				case PCD_TRANSCEIVE:
													irqEn   = 0x77;
													waitFor = 0x30;
													break;
				default:	break;
    }
   
    WriteRawRC(ComIEnReg,irqEn|0x80);
    ClearBitMask(ComIrqReg,0x80);	//�������ж�λ
    WriteRawRC(CommandReg,PCD_IDLE);
    SetBitMask(FIFOLevelReg,0x80);	 	//��FIFO����
    
    for (i=0; i<InLenByte; i++)
    {   WriteRawRC(FIFODataReg, pIn [i]);    }
    WriteRawRC(CommandReg, Command);	  
//   	 n = ReadRawRC(CommandReg);
    
    if (Command == PCD_TRANSCEIVE)
    {    SetBitMask(BitFramingReg,0x80);  }	 //��ʼ����
    										 
      i = 600;//����ʱ��Ƶ�ʵ���������M1�����ȴ�ʱ��25ms
// 	  i = 100000;
    do 
    {
        n = ReadRawRC(ComIrqReg);
        i--;
    }
    while ((i!=0) && !(n&0x01) && !(n&waitFor));
    ClearBitMask(BitFramingReg,0x80);

    if (i!=0)
    {    
        if(!(ReadRawRC(ErrorReg)&0x1B))
        {
            status = MI_OK;
            if (n & irqEn & 0x01)
            {   status = MI_NOTAGERR;   }
            if (Command == PCD_TRANSCEIVE)
            {
               	n = ReadRawRC(FIFOLevelReg);
              	lastBits = ReadRawRC(ControlReg) & 0x07;
                if (lastBits)
                {   *pOutLenBit = (n-1)*8 + lastBits;   }
                else
                {   *pOutLenBit = n*8;   }
                if (n == 0)
                {   n = 1;    }
                if (n > MAXRLEN)
                {   n = MAXRLEN;   }
                for (i=0; i<n; i++)
                {   pOut [i] = ReadRawRC(FIFODataReg);    }
            }
        }
        else
        {   status = MI_ERR;   }
        
    }
   

    SetBitMask(ControlReg,0x80);           // stop timer now
    WriteRawRC(CommandReg,PCD_IDLE); 
    return status;
}

/*
****************************************************************************************
* Function: PcdAntennaOn
* Description: ��������
* Input: None
* Output: None
* Return: None
* Author: weihaoMo
* Others: None
* Date of completion: 2019-11-29
* Date of last modify: 2019-11-29
****************************************************************************************
*/
void PcdAntennaOn(void)
{
    uint8_t   i;
    i = ReadRawRC(TxControlReg);
    if (!(i & 0x03))
    {
        SetBitMask(TxControlReg, 0x03);
    }
}

/*
****************************************************************************************
* Function: PcdAntennaOff
* Description: �ر�����
* Input: None
* Output: None
* Return: None
* Author: weihaoMo
* Others: None
* Date of completion: 2019-11-29
* Date of last modify: 2019-11-29
****************************************************************************************
*/
void PcdAntennaOff(void)
{
	ClearBitMask(TxControlReg, 0x03);
}


/*
****************************************************************************************
* Function: Read_IC_Card_ID
* Description: ��ȡ����
* Input: None
* Output: id[OUT]����
* Return: �ɹ�����MI_OK
* Author: weihaoMo
* Others: None
* Date of completion: 2019-11-29
* Date of last modify: 2019-11-29
****************************************************************************************
*/
char Read_IC_Card_ID(uint8_t *id)
{
    unsigned char CT[2];//������
    if(PcdRequest(PICC_REQALL,CT)==MI_OK)//Ѱ���ɹ�
    {
       
      if(PcdAnticoll(id)==MI_OK)/*����ײ�ɹ�*/
      {
        
        if(PcdSelect(id)==MI_OK)//ѡ���ɹ�
        {
          return MI_OK;
         }
      }
    }
   return MI_ERR;
}
/***********************************************************************************************
***********************************************************************************************/

uint8_t CardId[5]={0}; //����
uint8_t CardType[2];   //������
uint8_t RFID[16];			 //���RFID 
//uint8_t writeDataBuf[16]={255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255}; //д�������

uint8_t total=0; 
uint8_t KEY[6]={0xff,0xff,0xff,0xff,0xff,0xff}; //������-��ʼ����--�׿��ĳ�������--
uint8_t MIMA_1[16]={88,88,88,88,88,88,0xff,0x07,0x80,0x29,88,88,88,88,88,88}; //����1������ ��ʽ(A���� ����λ  B���� )
uint8_t MIMA_2[16]={88,88,88,88,88,88};//����1��A����

void print_info(uint8_t *p,int cnt)
{
  int i;
  for(i=0;i<cnt;i++)
  {
		if(!i)
			printf("0x%02X ",p[i]);
		else
			printf("%02X ",p[i]);
  }
  printf("\r\n");
}

/*
****************************************************************************************
* Function: Identificate_Card
* Description: Ѱ��--����ײ--ѡ��
* Input: None
* Output: pTagType[OUT]����Ƭ���ʹ���(���������)
            0x4400 = Mifare_UltraLight
            0x0400 = Mifare_One(S50)
            0x0200 = Mifare_One(S70)
            0x0800 = Mifare_Pro(X)
            0x4403 = Mifare_DESFire
					pSnr[OUT]:��Ƭ���кţ�4�ֽ�(���������)
* Return: �ɹ�����MI_OK
* Author: weihaoMo
* Others: None
* Date of completion: 2019-11-29
* Date of last modify: 2019-11-29
****************************************************************************************
*/
uint8_t Identificate_Card(uint8_t *pTagType,uint8_t *pSnr)
{
  uint8_t status=MI_ERR;
	uint8_t STA=0;

   if(PcdRequest(PICC_REQALL,pTagType)==MI_OK)//Ѱ���ɹ�
   {
		 printf("Ѱ���ɹ�\r\n");
      STA |=0x01;
      if(PcdAnticoll(pSnr)==MI_OK)/*����ײ�ɹ�*/
      {
				printf("����ײ�ɹ�\r\n");
				STA |=0x02;
		
        if(PcdSelect(pSnr)==MI_OK)//ѡ���ɹ�
        {
					printf("ѡ���ɹ�\r\n");
					STA |=0x04;
        }
      }
   }

	 
	 
	 if(STA==0x07)
		 status=MI_OK;
	 
  return status;
}

/*
****************************************************************************************
* Function: Card_Passworld
* Description: �޸Ŀ�����
* Input:  auth_mode ����֤��������͡�����ΪPICC_AUTHENT1A��PICC_AUTHENT1B��
			    addr      �������ŵĵ�ַ�����������ÿ�������ĵ�3���顣��һ�������Ƚ����⡣��
					  һ���3��7��11.....����
					*Src_Key    ��ԭ���� 
					*New_Key    ��������
					*pSnr       ������
* Output: None
* Return: �ɹ�����MI_OK
* Author: weihaoMo
* Others: None
* Date of completion: 2019-11-29
* Date of last modify: 2019-11-29
****************************************************************************************
*/
int Card_Passworld(uint8_t auth_mode,uint8_t addr,uint8_t *Src_Key,uint8_t *New_Key,uint8_t *pSnr)
{
  int status;

  /*1. Ѱ��--����ײ--ѡ��*/
  status=Identificate_Card(CardType,pSnr); 
  /*2. ��֤������*/
  if(status==MI_OK)
  {
    status=PcdAuthState(auth_mode,addr,Src_Key,pSnr);   //��֤��Ƭ���� �ββ�������֤��ʽ�����ַ�����룬�����к�
  }
  /*3. д���ݵ���*/
  if(status==MI_OK)
  {
    status=PcdWrite(addr,New_Key); //д���ݵ���addr�飬New_Keyд�������ֵ��
  }
  return status;
}

/*
****************************************************************************************
* Function: WriteCardData
* Description: д���ݵ�ָ����
* Input:      auth_mode ����֤��������͡�����ΪPICC_AUTHENT1A��PICC_AUTHENT1B��
					    addr      �����ݴ�ŵĵ�ַ��ÿ��������0��1��2���Ǵ�����ݡ�3�Ǵ�����롣
					һ���0��1��2 ��4��5��6��8��9��10----
					  *Src_Key    ��ԭ���� 
					  *data       ����д�������
					  *pSnr       ������
* Output: None
* Return: �ɹ�����MI_OK
* Author: weihaoMo
* Others: ����һ���ʽ��u8 writeDataBuf[16]={255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255}; //д��Ľ��
* Date of completion: 2019-11-29
* Date of last modify: 2019-11-29
****************************************************************************************
*/
uint8_t WriteCardData(uint8_t auth_mode,uint8_t addr,uint8_t *Src_Key,uint8_t *data,uint8_t *pSnr)
{
  int status=MI_ERR;

  /*1. Ѱ��--����ײ--ѡ��*/
  status=Identificate_Card(CardType,pSnr); 
  /*2. ��֤������*/
  if(status==MI_OK)
  {
    status =PcdAuthState(0x60,addr/4*4+3,KEY,pSnr);   //��֤��Ƭ���� �ββ�������֤��ʽ�����ַ�����룬�����к�
  }
  if(status==MI_OK)
  {
    printf("������֤�ɹ�!\r\n");
  }
  /*3. д���ݵ���*/
  if(status==MI_OK)
  {
    status=PcdWrite(addr,data); //д���ݵ���addr�飬data�������ֵ��
  }
  if(status==MI_OK)
  {
    printf("����д��OK!\r\n");
  }
  return status;
}

/*
****************************************************************************************
* Function: ReadCardData
* Description: �����ݵ�ָ����
* Input:     auth_mode ����֤��������͡�����ΪPICC_AUTHENT1A��PICC_AUTHENT1B��
				     addr      �����ݴ�ŵĵ�ַ��ÿ��������0��1��2���Ǵ�����ݡ�3�Ǵ�����롣
				һ���0��1��2 ��4��5��6��8��9��10
				    *Src_Key    ��ԭ���� 
				    *data       ������������
				    *pSnr       ������
* Output: None
* Return: �ɹ�����MI_OK
* Author: weihaoMo
* Others: ����һ���ʽ��u8 writeDataBuf[16]={255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255}; //д��Ľ��
* Date of completion: 2019-11-29
* Date of last modify: 2019-11-29
****************************************************************************************
*/
uint8_t ReadCardData(uint8_t auth_mode,uint8_t addr,uint8_t *Src_Key,uint8_t *data,uint8_t *pSnr)
{
  int status;
	
  /*1. Ѱ��--����ײ--ѡ��*/
  status=Identificate_Card(CardType,pSnr);  
  /*2. ��֤������*/
  if(status==MI_OK)
  {
		printf("������:");
		print_info(CardType,2);//��ӡ����
		printf("����:");
		print_info(pSnr,4);//��ӡ����		
		
    status=PcdAuthState(auth_mode,addr/4*4+3,KEY,pSnr);   //��֤��Ƭ���� �ββ�������֤��ʽ�����ַ�����룬�����к�
    printf("��֤������ɹ�\r\n");
  }
  /*3. ��������*/
  if(status==MI_OK)
  {
    status=PcdRead(addr,data); //�ӵ�addr���������ֵ��
  }
  if(status==MI_OK)
  {
    printf("���ݶ����ɹ�!\r\n");
  }
  return status;
}
