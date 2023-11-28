/*
****************************************************************************************
* Copyright: 2008-2019，信盈达科技有限公司，All rights reserved
* File name: None
* Description: None
* Dependent file: None
* Author: weihaoMo
* Version: V1.0
* Date: 2019-11-29
* History: None
* Company web site: http://www.edu118.com/
* Company net schools: http://www.edu118.cn/
* Notice: 本程序只供学习使用，未经作者许可，不得用于其它任何用途。 
*********************************版权所有，盗版必究*********************************
****************************************************************************************
*/

/*
****************************************************************************************
* INCLUDES (头文件包含)
****************************************************************************************
*/
#include "rfid.h"
#include "spi3.h"
#include "delay.h"
#include "string.h"
#include "stdio.h"

/*
****************************************************************************************
* CONSTANTS (常量定义)
****************************************************************************************
*/


/*
****************************************************************************************
* TYPEDEFS (类型定义)
****************************************************************************************
*/


/*
****************************************************************************************
* LOCAL VARIABLES (静态变量)
****************************************************************************************
*/


/*
****************************************************************************************
* LOCAL FUNCTIONS DECLARE (静态函数声明)
****************************************************************************************
*/


/*
****************************************************************************************
* LOCAL FUNCTIONS (静态函数)
****************************************************************************************
*/


/*
****************************************************************************************
* PUBLIC FUNCTIONS (全局函数)
****************************************************************************************
*/

/*
****************************************************************************************
* Function: RFID_Init
* Description: 初始化RFID的使能脚
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
	
  RCC->APB2ENR |=1<<0 |1<<3;  //使能A、D时钟
	
	/*CS-----PA15*/ //推挽输出 
	RFID_CS_GPIO->MODER &=~(0x3<<(RFID_CS_PIN*2));
	RFID_CS_GPIO->MODER |=0X1<<(RFID_CS_PIN*2);
	RFID_CS_GPIO->OTYPER &=~(1<<RFID_CS_PIN);
	RFID_CSH;
	
	/*RST-----PD4*/ //推挽输出 
	RFID_RST_GPIO->MODER &=~(0x3<<(RFID_RST_PIN*2));
	RFID_RST_GPIO->MODER |=0X1<<(RFID_RST_PIN*2);
	RFID_RST_GPIO->OTYPER &=~(1<<RFID_RST_PIN);
	RFID_RSTH;
	
	Reset_RC522();    //开启天线
	M500PcdConfigISOType( 'A' );
}

/*
****************************************************************************************
* Function: Reset_RC522
* Description: 复位RC522
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
	PcdReset();       //功    能：复位RC522
	PcdAntennaOff();  //关闭天线
	PcdAntennaOn();   //开启天线
}    

/*
****************************************************************************************
* Function: PcdRequest
* Description: 寻卡
* Input: req_code[IN]:寻卡方式
                0x52 = 寻感应区内所有符合14443A标准的卡
                0x26 = 寻未进入休眠状态的卡
* Output:pTagType[OUT]：卡片类型代码
                0x4400 = Mifare_UltraLight
                0x0400 = Mifare_One(S50)
                0x0200 = Mifare_One(S70)
                0x0800 = Mifare_Pro(X)
                0x4403 = Mifare_DESFire
* Return: 成功返回MI_OK
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
* Description: 防冲撞
* Input: None
* Output: pSnr[OUT]:卡片序列号，4字节
* Return: 成功返回MI_OK
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
* Description: 选定卡片
* Input: pSnr[IN]:卡片序列号，4字节
* Output: None
* Return: 成功返回MI_OK
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
* Description: 验证卡片密码
* Input: auth_mode[IN]: 密码验证模式
                 0x60 = 验证A密钥
                 0x61 = 验证B密钥 
								addr[IN]：块地址
* Output: pKey[OUT]：密码
					pSnr[OUT]：卡片序列号，4字节
* Return: 成功返回MI_OK
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
* Description: 读取M1卡一块数据
* Input: addr[IN]：块地址
* Output: p [OUT]：读出的数据，16字节
* Return: 成功返回MI_OK
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
* Description: 写数据到M1卡一块
* Input: addr[IN]：块地址
* Output: p [OUT]：读出的数据，16字节
* Return: 成功返回MI_OK
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
* Description: 命令卡片进入休眠状态
* Input: None
* Output: None
* Return: 成功返回MI_OK
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
* Description: 用MF522计算CRC16函数
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
* Description: 复位RC522
* Input: None
* Output: None
* Return: 成功返回MI_OK
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
    
	WriteRawRC(ModeReg,0x3D);            //和Mifare卡通讯，CRC初始值0x6363
	WriteRawRC(TReloadRegL,30);           
	WriteRawRC(TReloadRegH,0);
	WriteRawRC(TModeReg,0x8D);
	WriteRawRC(TPrescalerReg,0x3E);
	
	WriteRawRC(TxAutoReg,0x40);//必须要
   
   return MI_OK;
}

/*
****************************************************************************************
* Function: M500PcdConfigISOType
* Description: 设置RC632的工作方式
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
* Description: 读RC632寄存器
* Input: Address[IN]:寄存器地址
* Output: None
* Return: 读出的值
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
* Description: 写RC632寄存器
* Input: Address[IN]:寄存器地址
				 value[IN]:写入的值
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
* Description: 置RC522寄存器位
* Input: reg[IN]:寄存器地址
				 mask[IN]:置位值
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
* Description: 清RC522寄存器位
* Input: reg[IN]:寄存器地址
				 mask[IN]:清位值
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
* Description: 通过RC522和ISO14443卡通讯
* Input: Command[IN]:RC522命令字
				 pIn [IN]:通过RC522发送到卡片的数据
				 InLenByte[IN]:发送数据的字节长度
* Output: pOut [OUT]:接收到的卡片返回数据
					*pOutLenBit[OUT]:返回数据的位长度
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
    ClearBitMask(ComIrqReg,0x80);	//清所有中断位
    WriteRawRC(CommandReg,PCD_IDLE);
    SetBitMask(FIFOLevelReg,0x80);	 	//清FIFO缓存
    
    for (i=0; i<InLenByte; i++)
    {   WriteRawRC(FIFODataReg, pIn [i]);    }
    WriteRawRC(CommandReg, Command);	  
//   	 n = ReadRawRC(CommandReg);
    
    if (Command == PCD_TRANSCEIVE)
    {    SetBitMask(BitFramingReg,0x80);  }	 //开始传送
    										 
      i = 600;//根据时钟频率调整，操作M1卡最大等待时间25ms
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
* Description: 开启天线
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
* Description: 关闭天线
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
* Description: 读取卡号
* Input: None
* Output: id[OUT]卡号
* Return: 成功返回MI_OK
* Author: weihaoMo
* Others: None
* Date of completion: 2019-11-29
* Date of last modify: 2019-11-29
****************************************************************************************
*/
char Read_IC_Card_ID(uint8_t *id)
{
    unsigned char CT[2];//卡类型
    if(PcdRequest(PICC_REQALL,CT)==MI_OK)//寻卡成功
    {
       
      if(PcdAnticoll(id)==MI_OK)/*防冲撞成功*/
      {
        
        if(PcdSelect(id)==MI_OK)//选卡成功
        {
          return MI_OK;
         }
      }
    }
   return MI_ERR;
}
/***********************************************************************************************
***********************************************************************************************/

uint8_t CardId[5]={0}; //卡号
uint8_t CardType[2];   //卡类型
uint8_t RFID[16];			 //存放RFID 
//uint8_t writeDataBuf[16]={255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255}; //写入的数据

uint8_t total=0; 
uint8_t KEY[6]={0xff,0xff,0xff,0xff,0xff,0xff}; //卡密码-初始密码--白卡的出厂密码--
uint8_t MIMA_1[16]={88,88,88,88,88,88,0xff,0x07,0x80,0x29,88,88,88,88,88,88}; //扇区1的密码 格式(A密码 控制位  B密码 )
uint8_t MIMA_2[16]={88,88,88,88,88,88};//扇区1的A密码

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
* Description: 寻卡--防冲撞--选卡
* Input: None
* Output: pTagType[OUT]：卡片类型代码(做输出参数)
            0x4400 = Mifare_UltraLight
            0x0400 = Mifare_One(S50)
            0x0200 = Mifare_One(S70)
            0x0800 = Mifare_Pro(X)
            0x4403 = Mifare_DESFire
					pSnr[OUT]:卡片序列号，4字节(做输出参数)
* Return: 成功返回MI_OK
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

   if(PcdRequest(PICC_REQALL,pTagType)==MI_OK)//寻卡成功
   {
		 printf("寻卡成功\r\n");
      STA |=0x01;
      if(PcdAnticoll(pSnr)==MI_OK)/*防冲撞成功*/
      {
				printf("防冲撞成功\r\n");
				STA |=0x02;
		
        if(PcdSelect(pSnr)==MI_OK)//选卡成功
        {
					printf("选卡成功\r\n");
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
* Description: 修改卡密码
* Input:  auth_mode ：验证密码的类型。（分为PICC_AUTHENT1A和PICC_AUTHENT1B）
			    addr      ：密码存放的地址。（密码放在每个扇区的第3个块。第一个扇区比较特殊。）
					  一般填：3、7、11.....规律
					*Src_Key    ：原密码 
					*New_Key    ：新密码
					*pSnr       ：卡号
* Output: None
* Return: 成功返回MI_OK
* Author: weihaoMo
* Others: None
* Date of completion: 2019-11-29
* Date of last modify: 2019-11-29
****************************************************************************************
*/
int Card_Passworld(uint8_t auth_mode,uint8_t addr,uint8_t *Src_Key,uint8_t *New_Key,uint8_t *pSnr)
{
  int status;

  /*1. 寻卡--防冲撞--选卡*/
  status=Identificate_Card(CardType,pSnr); 
  /*2. 验证卡密码*/
  if(status==MI_OK)
  {
    status=PcdAuthState(auth_mode,addr,Src_Key,pSnr);   //验证卡片密码 形参参数：验证方式，块地址，密码，卡许列号
  }
  /*3. 写数据到卡*/
  if(status==MI_OK)
  {
    status=PcdWrite(addr,New_Key); //写数据到第addr块，New_Key写入的数据值。
  }
  return status;
}

/*
****************************************************************************************
* Function: WriteCardData
* Description: 写数据到指定块
* Input:      auth_mode ：验证密码的类型。（分为PICC_AUTHENT1A和PICC_AUTHENT1B）
					    addr      ：数据存放的地址。每个扇区的0、1、2块是存放数据。3是存放密码。
					一般填：0、1、2 、4、5、6、8、9、10----
					  *Src_Key    ：原密码 
					  *data       ：新写入的数据
					  *pSnr       ：卡号
* Output: None
* Return: 成功返回MI_OK
* Author: weihaoMo
* Others: 数据一般格式：u8 writeDataBuf[16]={255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255}; //写入的金额
* Date of completion: 2019-11-29
* Date of last modify: 2019-11-29
****************************************************************************************
*/
uint8_t WriteCardData(uint8_t auth_mode,uint8_t addr,uint8_t *Src_Key,uint8_t *data,uint8_t *pSnr)
{
  int status=MI_ERR;

  /*1. 寻卡--防冲撞--选卡*/
  status=Identificate_Card(CardType,pSnr); 
  /*2. 验证卡密码*/
  if(status==MI_OK)
  {
    status =PcdAuthState(0x60,addr/4*4+3,KEY,pSnr);   //验证卡片密码 形参参数：验证方式，块地址，密码，卡许列号
  }
  if(status==MI_OK)
  {
    printf("密码验证成功!\r\n");
  }
  /*3. 写数据到卡*/
  if(status==MI_OK)
  {
    status=PcdWrite(addr,data); //写数据到第addr块，data入的数据值。
  }
  if(status==MI_OK)
  {
    printf("数据写入OK!\r\n");
  }
  return status;
}

/*
****************************************************************************************
* Function: ReadCardData
* Description: 读数据到指定块
* Input:     auth_mode ：验证密码的类型。（分为PICC_AUTHENT1A和PICC_AUTHENT1B）
				     addr      ：数据存放的地址。每个扇区的0、1、2块是存放数据。3是存放密码。
				一般填：0、1、2 、4、5、6、8、9、10
				    *Src_Key    ：原密码 
				    *data       ：读出的数据
				    *pSnr       ：卡号
* Output: None
* Return: 成功返回MI_OK
* Author: weihaoMo
* Others: 数据一般格式：u8 writeDataBuf[16]={255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255}; //写入的金额
* Date of completion: 2019-11-29
* Date of last modify: 2019-11-29
****************************************************************************************
*/
uint8_t ReadCardData(uint8_t auth_mode,uint8_t addr,uint8_t *Src_Key,uint8_t *data,uint8_t *pSnr)
{
  int status;
	
  /*1. 寻卡--防冲撞--选卡*/
  status=Identificate_Card(CardType,pSnr);  
  /*2. 验证卡密码*/
  if(status==MI_OK)
  {
		printf("卡类型:");
		print_info(CardType,2);//打印类型
		printf("卡号:");
		print_info(pSnr,4);//打印卡号		
		
    status=PcdAuthState(auth_mode,addr/4*4+3,KEY,pSnr);   //验证卡片密码 形参参数：验证方式，块地址，密码，卡许列号
    printf("验证卡密码成功\r\n");
  }
  /*3. 读出数据*/
  if(status==MI_OK)
  {
    status=PcdRead(addr,data); //从第addr块读出数据值。
  }
  if(status==MI_OK)
  {
    printf("数据读出成功!\r\n");
  }
  return status;
}
