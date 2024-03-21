/**
  ******************************************************************************
  * �ļ��� : flash.c
  * ��  �� : Ҷ�л�
  * ��  �� ��V1.0
  * ʱ  �� : 2012��11��22��
  * ��  ˾ ��������缼�����޹�˾
  * ժ  Ҫ �����ļ�������W25X16ģ��ĳ�ʼ�������á���д�����ȡ�
  * ˵  �� ��W25X16����	--2MByte
  *              ҳ        ����        ����      
  * ��  С ��  256Byte	  4kByte	  64kByte
  * ��  ����   8192ҳ	  512����	  32��
  ******************************************************************************
  */



/**
  ******************************************************************************
  *                                  ͷ�ļ�
  ******************************************************************************
  */


#include "SPI.h"
#include "flash.h"
#include <stdio.h> 


#if 1
static void Delay(vu32 nCount)
{
  for(; nCount != 0; nCount--);
}
#endif


/**
  ******************************************************************************
  *                             SPI_FlashInit()
  *
  * ����   : ��ʼ��SPI FLASH�Ľӿڡ�ģʽ����
  *		   
  * ����   : ��
  *
  *
  * ����ֵ : ��
  ******************************************************************************
  */


void SPI_FlashInit(void)
{
  uint32_t i=0;
	i=i;
	Init_SPI2();		   //��ʼ��SPI
  while(SPI_FlashReadID()!=FLASH_ID)//��ⲻ��W25X16
  {	   
    i=SPI_FlashReadID();
#if Debug_printf
		printf("\n\r ID:%d \r\n",i);
		printf("\n\r û�ж�����ȷ��W25X16оƬID������Ӳ������ \r\n");
#endif
		Delay(0xaFFFF);
		Delay(0xaFFFF);
    GPIO_ResetBits(GPIOF,  GPIO_Pin_7);
		Delay(10);
		GPIO_SetBits(GPIOF,  GPIO_Pin_7);
		Delay(10);

  }
#if Debug_printf
  printf("\n\r ��ȷ��W25X16оƬID \r\n");
#endif
  SPI_FlashWriteSR(0);
}  

/**
  ******************************************************************************
  *                             SPI_FlashReadSR()
  *
  * ����   : ��ȡSPI_FLASH��״̬�Ĵ���
  *			 BIT7  6   5   4   3   2   1   0
  *			 SPR   RV  TB BP2 BP1 BP0 WEL BUSY
  *			 SPR:Ĭ��0,״̬�Ĵ�������λ,���WPʹ��
  *			 WEL:дʹ������
  *			 BUSY:æ���λ(1,æ;0,����)
  *			 Ĭ��:0x00
  *		   
  * ����   : ��
  *
  *
  * ����ֵ : ��
  ******************************************************************************
  */
u8 SPI_FlashReadSR (void)   
{  
  u8 byte=0;   
  SPI_FLASH_CS=0;;                              //ʹ������   
  SPI2_ReadWriteByte(W25X_ReadStatusReg);  //���Ͷ�ȡ״̬�Ĵ�������    
  byte = SPI2_ReadWriteByte(0Xff);         //��ȡһ���ֽ�  
  SPI_FLASH_CS=1;;                              //ȡ��Ƭѡ     
  return byte;   
} 

/**
  ******************************************************************************
  *                             SPI_FlashWritrSR()
  *
  * ����   : дSPI_FLASH��״̬�Ĵ���--�ȴ�дʹ��Ȼ�����д������
  *			 BIT7  6   5   4   3   2   1   0
  *			 SPR   RV  TB BP2 BP1 BP0 WEL BUSY
  *			 SPR:Ĭ��0,״̬�Ĵ�������λ,���WPʹ��
  *			 WEL:дʹ������
  *			 BUSY:æ���λ(1,æ;0,����)
  *			 Ĭ��:0x00
  *			 ֻ��SPR,TB,BP2,BP1,BP0(bit 7,5,4,3,2)����д!!!
  *		   
  * ����   : ��
  *
  *
  * ����ֵ : ��
  ******************************************************************************
  */
/* ���������⣬û�н���дʹ!!!!!! */
void SPI_FlashWriteSR (u8 sr)   
{   
  SPI_FLASH_CS=0;                               //ʹ������   
  SPI2_ReadWriteByte(W25X_WriteStatusReg);  //����д״̬�Ĵ�������    
  SPI2_ReadWriteByte(sr);                   //д��һ���ֽ�  
  SPI_FLASH_CS=1;                               //ȡ��Ƭѡ     	      
}   
 
/**
  ******************************************************************************
  *                           SPI_FlashWriteEnable()
  *
  * ����   : PI_FLASHдʹ��,��WEL��λ
  *			 
  *		   
  * ����   : ��
  *
  *
  * ����ֵ : ��
  ******************************************************************************
  */ 
   
void SPI_FlashWriteEnable (void)   
{
  SPI_FLASH_CS=0;                              //ʹ������   
  SPI2_ReadWriteByte(W25X_WriteEnable);    //����дʹ��  
  SPI_FLASH_CS=1;                              //ȡ��Ƭѡ     	      
}
 
 
/**
  ******************************************************************************
  *                           SPI_FlashWriteDisable()
  *
  * ����   : PI_FLASHд��ֹ,��WEL����
  *			 
  *		   
  * ����   : ��
  *
  *
  * ����ֵ : ��
  ******************************************************************************
  */ 
   
void SPI_FlashWriteDisable (void)   
{  
  SPI_FLASH_CS=0;                              //ʹ������   
  SPI2_ReadWriteByte(W25X_WriteDisable);   //����д��ָֹ��    
  SPI_FLASH_CS=1;                              //ȡ��Ƭѡ     	      
}

 			    
/**
  ******************************************************************************
  *                           SPI_FlashReadID()
  *
  * ����   : ��ȡоƬID W25X16��ID:0XEF14
  *			 
  *		   
  * ����   : ��
  *
  *
  * ����ֵ : Temp--W25X16��ID:0XEF14
  ******************************************************************************
  */ 

u16 SPI_FlashReadID (void)
{
  u16 Temp = 0;	  
  SPI_FLASH_CS=0;				    
  SPI2_ReadWriteByte(0x90);    //���Ͷ�ȡID����	    
  SPI2_ReadWriteByte(0x00); 	    
  SPI2_ReadWriteByte(0x00); 	    
  SPI2_ReadWriteByte(0x00); 	 			   
  Temp|=SPI2_ReadWriteByte(0xFF)<<8;  
  Temp|=SPI2_ReadWriteByte(0xFF);	 
  SPI_FLASH_CS=1;				    
  return Temp;
}   		    

/**
  ******************************************************************************
  *                           SPI_FlashRead()
  *
  * ����   : ��ȡflash�ϵ�ַReadAddr�е�NumByteToRead������
  *			 
  *		   
  * ����   : pBuffer--������ݵ��ڴ��ַ
  *          ReadAddr--Ҫ����flash�ϵĵ�ַ
  *          NumByteToRead--��ȡ���ݵĸ���(���65535)
  *
  *
  * ����ֵ : ��
  ******************************************************************************
  */

void SPI_FlashRead(u8* pBuffer,u32 ReadAddr,u16 NumByteToRead)   
{ 
  u16 i; 
  SPI_FLASH_CS=0; ;                               //ʹ������   
  SPI2_ReadWriteByte(W25X_ReadData);         //���Ͷ�ȡ����   
  SPI2_ReadWriteByte((u8)((ReadAddr)>>16));  //����24bit��ַ    
  SPI2_ReadWriteByte((u8)((ReadAddr)>>8));   
  SPI2_ReadWriteByte((u8)ReadAddr);   
  for(i=0;i<NumByteToRead;i++)
  { 
    pBuffer[i]=SPI2_ReadWriteByte(0XFF);     //ѭ������  
  }
  SPI_FLASH_CS=1;                                //ȡ��Ƭѡ     	      
}  


/**
  ******************************************************************************
  *                           SPI_FlashWritePage()
  *
  * ����   : SPI��һҳ(0~65535)��д������256���ֽڵ�����
  *			 
  *		   
  * ����   : pBuffer--������ݵ��ڴ��ַ
  *          WriteAddr--��ʼд��flash�ĵ�ַ(24bit)
  *          NumByteToRead--Ҫд����ֽ���(���256),������Ӧ�ó�����ҳ��ʣ���ֽ���!!!
  *
  *
  * ����ֵ : ��
  ******************************************************************************
  */	 
void SPI_FlashWritePage(u8* pBuffer,u32 WriteAddr,u16 NumByteToWrite)
{
  u16 i;  
  SPI_FlashWriteEnable();                        //SET WEL 
  SPI_FLASH_CS=0;  ;                              //ʹ������   
  SPI2_ReadWriteByte(W25X_PageProgram);      //����дҳ����   
  SPI2_ReadWriteByte((u8)((WriteAddr)>>16)); //����24bit��ַ    
  SPI2_ReadWriteByte((u8)((WriteAddr)>>8));   
  SPI2_ReadWriteByte((u8)WriteAddr);   
  for(i=0;i<NumByteToWrite;i++)
  {
    SPI2_ReadWriteByte(pBuffer[i]);          //ѭ��д��  
  }
  SPI_FLASH_CS=1;                                //ȡ��Ƭѡ 
  SPI_FlashWaitBusy();					                 //�ȴ�д�����
} 

/**
  ******************************************************************************
  *                          SPI_FlashWriteNoCheck()
  *
  * ����   : 1���޼���дSPI FLASH,����ȷ����д�ĵ�ַ��Χ�ڵ�����ȫ��Ϊ0XFF,������
  *          ��0XFF��д������ݽ�ʧ��!
  *          2�������Զ���ҳ���ܣ���ָ����ַ��ʼд��ָ�����ȵ�����,����Ҫȷ����ַ
  *          ��Խ��!
  *			 
  *		   
  * ����   : pBuffer--������ݵ��ڴ��ַ
  *          WriteAddr--��ʼд��flash�ĵ�ַ(24bit)
  *          NumByteToRead--Ҫд����ֽ���(���65535),
  *
  *
  * ����ֵ : ��
  ******************************************************************************
  */

void SPI_FlashWriteNoCheck(u8* pBuffer,u32 WriteAddr,u16 NumByteToWrite)   
{ 			 		 
  u16 pageremain;	   
  pageremain=256-WriteAddr%256;       //��ҳʣ����ֽ���		 	    
  if(NumByteToWrite<=pageremain)pageremain=NumByteToWrite;//������256���ֽ�
  while(1)
  {	   
    SPI_FlashWritePage(pBuffer,WriteAddr,pageremain);
	if(NumByteToWrite==pageremain)break;     //д�������
 	else //NumByteToWrite>pageremain
	{
	  pBuffer+=pageremain;
	  WriteAddr+=pageremain;	

	  NumByteToWrite-=pageremain;			       //��ȥ�Ѿ�д���˵��ֽ���
	  if(NumByteToWrite>256)pageremain=256;  //һ�ο���д��256���ֽ�
	  else pageremain=NumByteToWrite; 	     //����256���ֽ���
	}
}	    
} 

/**
  ******************************************************************************
  *                          SPI_FlashWrite()
  *
  * ����   : дSPI FLASH
  *          ��ָ����ַ��ʼд��ָ�����ȵ�����
  *          �ú�������������!
  *			 
  *		   
  * ����   : pBuffer--������ݵ��ڴ��ַ
  *          WriteAddr--��ʼд��flash�ĵ�ַ(24bit)
  *          NumByteToRead--Ҫд����ֽ���(���65535),
  *
  *
  * ����ֵ : ��
  ******************************************************************************
  */
    		   
// u8 SPI_FLASH_BUF[4096];
// void SPI_FlashWrite(u8* pBuffer,u32 WriteAddr,u16 NumByteToWrite)   
// { 
//   u32 secpos;
//   u16 secoff;
//   u16 secremain;	   
//   u16 i;  
//   secpos=WriteAddr/4096;//������ַ 0~511 for w25x16
//   secoff=WriteAddr%4096;//�������ڵ�ƫ��
//   secremain=4096-secoff;//����ʣ��ռ��С   
//   if(NumByteToWrite<=secremain)secremain=NumByteToWrite;//������4096���ֽ�
//   while(1) 
//   {	
// 	SPI_FlashRead(SPI_FLASH_BUF,secpos*4096,4096);//������������������
// 	for(i=0;i<secremain;i++)//У������
// 	{
// 	  if(SPI_FLASH_BUF[secoff+i]!=0XFF)break;//��Ҫ����  	  
// 	}
// 	if(i<secremain)//��Ҫ����
// 	{
// 	  SPI_FlashEraseSector(secpos);//�����������
// 	  for(i=0;i<secremain;i++)	   //����
// 	  {
// 	    SPI_FLASH_BUF[i+secoff]=pBuffer[i];	  
// 	  }
// 	  SPI_FlashWriteNoCheck(SPI_FLASH_BUF,secpos*4096,4096);//д����������  
// 	}
// 	else 
// 	  SPI_FlashWriteNoCheck(pBuffer,WriteAddr,secremain);//д�Ѿ������˵�,ֱ��д������ʣ������. 				   
// 	if(NumByteToWrite==secremain)break;//д�������
// 	else//д��δ����
// 	{
// 	  secpos++;//������ַ��1
// 	  secoff=0;//ƫ��λ��Ϊ0 	 

// 	  pBuffer+=secremain;  //ָ��ƫ��
// 	  WriteAddr+=secremain;//д��ַƫ��	   
// 	  NumByteToWrite-=secremain;				//�ֽ����ݼ�
// 	  if(NumByteToWrite>4096)secremain=4096;	//��һ����������д����
// 	  else secremain=NumByteToWrite;			//��һ����������д����
// 	}	 
//   }	 	 
// }


/**
  ******************************************************************************
  *                          SPI_FlashEraseChip()
  *
  * ����   : ��������оƬ
  *          ��Ƭ����ʱ��:
  *          W25X16:25s
  *          W25X32:40s
  *          W25X64:40s	   �ȴ�ʱ�䳬��...
  *			 
  *		   
  * ����   : ��
  *
  *
  * ����ֵ : ��
  ******************************************************************************
  */

void SPI_FlashEraseChip(void)   
{                                             
  SPI_FlashWriteEnable();                  //SET WEL 
  SPI_FlashWaitBusy();
  SPI_FLASH_CS=0;  ;                        //ʹ������   
  SPI2_ReadWriteByte(W25X_ChipErase);  //����Ƭ��������  
  SPI_FLASH_CS=1;                          //ȡ��Ƭѡ     	      
  SPI_FlashWaitBusy();   				           //�ȴ�оƬ��������

}   

/**
  ******************************************************************************
  *                         SPI_FlashEraseSector()
  *
  * ����   : ����һ������
  *          ����һ������������ʱ��:150ms
  *			 
  *		   
  * ����   : Dst_Addr:������ַ 0~511 for w25x16
  *
  *
  * ����ֵ : ��
  ******************************************************************************
  */

void SPI_FlashEraseSector(u32 Dst_Addr)   
{   
  Dst_Addr*=4096;
  SPI_FlashWriteEnable();                      //SET WEL 	 
  SPI_FlashWaitBusy();   
  SPI_FLASH_CS=0;                              //ʹ������   
  SPI2_ReadWriteByte(W25X_SectorErase);    //������������ָ�� 
  SPI2_ReadWriteByte((u8)((Dst_Addr)>>16));//����24bit��ַ    
  SPI2_ReadWriteByte((u8)((Dst_Addr)>>8));   
  SPI2_ReadWriteByte((u8)Dst_Addr);  
  SPI_FLASH_CS=1;                              //ȡ��Ƭѡ     	      
  SPI_FlashWaitBusy();   				       //�ȴ��������
}  

/**
  ******************************************************************************
  *                           SPI_FlashWaitBusy()
  *
  * ����   : �ȴ�����
  *			 
  *		   
  * ����   : ��
  *
  *
  * ����ֵ : ��
  ******************************************************************************
  */

void SPI_FlashWaitBusy(void)   
{   
  while ((SPI_FlashReadSR()&0x01)==0x01);   // �ȴ�BUSYλ���
}
  
/**
  ******************************************************************************
  *                           SPI_FlashPowerDown()
  *
  * ����   : �������ģʽ
  *			 
  *		   
  * ����   : ��
  *
  *
  * ����ֵ : ��
  ******************************************************************************
  */
void SPI_FlashPowerDown(void)   
{ 
  SPI_FLASH_CS=0;                            //ʹ������   
  SPI2_ReadWriteByte(W25X_PowerDown);    //���͵�������  
  SPI_FLASH_CS=1;                            //ȡ��Ƭѡ     	      
//   delay_us(3);                               //�ȴ�TPD 
  Delay(30);	
}   
/**
  ******************************************************************************
  *                           SPI_FlashPowerDown()
  *
  * ����   : ����
  *			 
  *		   
  * ����   : ��
  *
  *
  * ����ֵ : ��
  ******************************************************************************
  */
void SPI_FlashWakeUp(void)   
{  
  SPI_FLASH_CS=0;                                 //ʹ������   
  SPI2_ReadWriteByte(W25X_ReleasePowerDown);  //  send W25X_PowerDown command 0xAB    
  SPI_FLASH_CS=1;                                 //ȡ��Ƭѡ     	      
//   delay_us(3);                                    //�ȴ�TRES1
	Delay(20);
}   




