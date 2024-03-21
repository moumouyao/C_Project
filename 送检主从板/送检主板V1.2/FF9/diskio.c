///*-----------------------------------------------------------------------*/
///* Low level disk I/O module skeleton for FatFs     (C)ChaN, 2007        */
///*-----------------------------------------------------------------------*/
///* This is a stub disk I/O module that acts as front end of the existing */
///* disk I/O modules and attach it to FatFs module with common interface. */
///*-----------------------------------------------------------------------*/

#include "diskio.h"
#include "stm32f4xx.h"
#include "sdio_sdcard.h"
#include "sw_global.h"
#include "delay.h"
#include "dis_app.h"
//#define BLOCK_SIZE            512 /* Block Size in Bytes */



///*-----------------------------------------------------------------------*/
///* Inidialize a Drive                                                    */

//DSTATUS disk_initialize (
//	BYTE drv				/* Physical drive nmuber (0..) */
//)
//{
//	SD_Error  Status;
//	/* Supports only single drive */
//	if (drv)
//	{
//		return STA_NOINIT;
//	}
///*-------------------------- SD Init ----------------------------- */
//  Status = SD_Init();
//	if (Status!=SD_OK )
//	{
//		return STA_NOINIT;
//	}
//	else
//	{
//		return RES_OK;
//	}

//}



///*-----------------------------------------------------------------------*/
///* Return Disk Status                                                    */

//DSTATUS disk_status (
//	BYTE drv		/* Physical drive nmuber (0..) */
//)
//{
//	return RES_OK;
//}



///*-----------------------------------------------------------------------*/
///* Read Sector(s)                                                        */

//DRESULT disk_read (
//	BYTE drv,		/* Physical drive nmuber (0..) */
//	BYTE *buff,		/* Data buffer to store read data */
//	DWORD sector,	/* Sector address (LBA) */
//	BYTE count		/* Number of sectors to read (1..255) */
//)
//{

//	if (count > 1)
//	{
//		SD_ReadMultiBlocks(buff, sector*BLOCK_SIZE, BLOCK_SIZE, count);
//	
//			  /* Check if the Transfer is finished */
//	     SD_WaitReadOperation();  //ѭ����ѯdma�����Ƿ����
//	
//	    /* Wait until end of DMA transfer */
//	    while(SD_GetStatus() != 0);

//	}
//	else
//	{
//		
//		SD_ReadBlock(buff, sector*BLOCK_SIZE, BLOCK_SIZE);

//			  /* Check if the Transfer is finished */
//	     SD_WaitReadOperation();  //ѭ����ѯdma�����Ƿ����
//	
//	    /* Wait until end of DMA transfer */
//	    while(SD_GetStatus() != 0);

//	}
//	return RES_OK;
//}



///*-----------------------------------------------------------------------*/
///* Write Sector(s)                                                       */

//#if _READONLY == 0
//DRESULT disk_write (
//	BYTE drv,			/* Physical drive nmuber (0..) */
//	const BYTE *buff,	/* Data to be written */
//	DWORD sector,		/* Sector address (LBA) */
//	BYTE count			/* Number of sectors to write (1..255) */
//)
//{
//	if (count > 1)
//	{
//		SD_WriteMultiBlocks((uint8_t *)buff, sector*BLOCK_SIZE, BLOCK_SIZE, count);
//		
//		  /* Check if the Transfer is finished */
//	  	 SD_WaitWriteOperation();	   //�ȴ�dma�������
//	    while(SD_GetStatus() != 0); //�ȴ�sdio��sd���������
//	}
//	else
//	{
//		SD_WriteBlock((uint8_t *)buff,sector*BLOCK_SIZE, BLOCK_SIZE);
//		
//		  /* Check if the Transfer is finished */
//	   	SD_WaitWriteOperation();	   //�ȴ�dma�������
//	  while(SD_GetStatus() != 0); //�ȴ�sdio��sd���������
//	}
//	return RES_OK;
//}
//#endif /* _READONLY */




///*-----------------------------------------------------------------------*/
///* Miscellaneous Functions                                               */

//DRESULT disk_ioctl (
//	BYTE drv,		/* Physical drive nmuber (0..) */
//	BYTE ctrl,		/* Control code */
//	void *buff		/* Buffer to send/receive control data */
//)
//{
//	return RES_OK;
//}
//							 
///*-----------------------------------------------------------------------*/
///* Get current time                                                      */
///*-----------------------------------------------------------------------*/ 
//DWORD get_fattime(void)
//{

// 	return 0;

//} 





























/*-----------------------------------------------------------------------*/
/* Low level disk I/O module skeleton for FatFs     (C)ChaN, 2013        */
/*-----------------------------------------------------------------------*/
/* If a working storage control module is available, it should be        */
/* attached to the FatFs via a glue function rather than modifying it.   */
/* This is an example of glue functions to attach various exsisting      */
/* storage control module to the FatFs module with a defined API.        */
/*-----------------------------------------------------------------------*/

#include "diskio.h"		/* FatFs lower layer API */
#include "malloc.h"

//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32F407������
//FATFS�ײ�(diskio) ��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2014/5/15
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	 


#define SD_CARD	 0  //SD��,���Ϊ0
#define EX_FLASH 1	//�ⲿflash,���Ϊ1

#define FLASH_SECTOR_SIZE 	512			  
//����W25Q128
//ǰ12M�ֽڸ�fatfs��,12M�ֽں�,���ڴ���ֿ�,�ֿ�ռ��3.09M.	ʣ�ಿ��,���ͻ��Լ���	 			    
u16	    FLASH_SECTOR_COUNT=2048*12;	//W25Q1218,ǰ12M�ֽڸ�FATFSռ��
#define FLASH_BLOCK_SIZE   	8     	//ÿ��BLOCK��8������

//��ʼ������
DSTATUS disk_initialize (
	BYTE pdrv				/* Physical drive nmuber (0..) */
)
{
	u8 res=0;	    
	switch(pdrv)
	{
		case SD_CARD://SD��
			res=SD_Init();//SD����ʼ�� 
  			break;
		case EX_FLASH://�ⲿflash
//			W25QXX_Init();
			FLASH_SECTOR_COUNT=2048*12;//W25Q1218,ǰ12M�ֽڸ�FATFSռ�� 
 			break;
		default:
			res=1; 
	}		 
	if(res)return  STA_NOINIT;
	else return 0; //��ʼ���ɹ�
}  

//��ô���״̬
DSTATUS disk_status (
	BYTE pdrv		/* Physical drive nmuber (0..) */
)
{ 
	return 0;
} 

//������
//drv:���̱��0~9
//*buff:���ݽ��ջ����׵�ַ
//sector:������ַ
//count:��Ҫ��ȡ��������
DRESULT disk_read (
	BYTE pdrv,		/* Physical drive nmuber (0..) */
	BYTE *buff,		/* Data buffer to store read data */
	DWORD sector,	/* Sector address (LBA) */
	UINT count		/* Number of sectors to read (1..128) */
)
{
	u8 res=0; 
    if (!count)return RES_PARERR;//count���ܵ���0�����򷵻ز�������		 	 
	switch(pdrv)
	{
		case SD_CARD://SD��
			res=SD_ReadDisk(buff,sector,count);	 
			while(res)//������
			{
				SD_Init();	//���³�ʼ��SD��
				res=SD_ReadDisk(buff,sector,count);	
				//printf("sd rd error:%d\r\n",res);
			}
			break;
		case EX_FLASH://�ⲿflash
			for(;count>0;count--)
			{
//				W25QXX_Read(buff,sector*FLASH_SECTOR_SIZE,FLASH_SECTOR_SIZE);
				sector++;
				buff+=FLASH_SECTOR_SIZE;
			}
			res=0;
			break;
		default:
			res=1; 
	}
   //������ֵ����SPI_SD_driver.c�ķ���ֵת��ff.c�ķ���ֵ
    if(res==0x00)return RES_OK;	 
    else return RES_ERROR;	   
}

//д����
//drv:���̱��0~9
//*buff:���������׵�ַ
//sector:������ַ
//count:��Ҫд���������
#if _USE_WRITE
DRESULT disk_write (
	BYTE pdrv,			/* Physical drive nmuber (0..) */
	const BYTE *buff,	/* Data to be written */
	DWORD sector,		/* Sector address (LBA) */
	UINT count			/* Number of sectors to write (1..128) */
)
{
	u8 res=0;  
    if (!count)return RES_PARERR;//count���ܵ���0�����򷵻ز�������		 	 
	switch(pdrv)
	{
		case SD_CARD://SD��
			res=SD_WriteDisk((u8*)buff,sector,count);
			while(res)//д����
			{
				SD_Init();	//���³�ʼ��SD��
				res=SD_WriteDisk((u8*)buff,sector,count);	
				//printf("sd wr error:%d\r\n",res);
			}
			break;
		case EX_FLASH://�ⲿflash
			for(;count>0;count--)
			{										    
//				W25QXX_Write((u8*)buff,sector*FLASH_SECTOR_SIZE,FLASH_SECTOR_SIZE);
				sector++;
				buff+=FLASH_SECTOR_SIZE;
			}
			res=0;
			break;
		default:
			res=1; 
	}
    //������ֵ����SPI_SD_driver.c�ķ���ֵת��ff.c�ķ���ֵ
    if(res == 0x00)return RES_OK;	 
    else return RES_ERROR;	
}
#endif


//����������Ļ��
 //drv:���̱��0~9
 //ctrl:���ƴ���
 //*buff:����/���ջ�����ָ��
#if _USE_IOCTL
DRESULT disk_ioctl (
	BYTE pdrv,		/* Physical drive nmuber (0..) */
	BYTE cmd,		/* Control code */
	void *buff		/* Buffer to send/receive control data */
)
{
	DRESULT res;						  			     
	if(pdrv==SD_CARD)//SD��
	{
	    switch(cmd)
	    {
		    case CTRL_SYNC:
				res = RES_OK; 
		        break;	 
		    case GET_SECTOR_SIZE:
				*(DWORD*)buff = 512; 
		        res = RES_OK;
		        break;	 
		    case GET_BLOCK_SIZE:
				*(WORD*)buff = SDCardInfo.CardBlockSize;
		        res = RES_OK;
		        break;	 
		    case GET_SECTOR_COUNT:
		        *(DWORD*)buff = SDCardInfo.CardCapacity/512;
		        res = RES_OK;
		        break;
		    default:
		        res = RES_PARERR;
		        break;
	    }
	}else if(pdrv==EX_FLASH)	//�ⲿFLASH  
	{
	    switch(cmd)
	    {
		    case CTRL_SYNC:
				res = RES_OK; 
		        break;	 
		    case GET_SECTOR_SIZE:
		        *(WORD*)buff = FLASH_SECTOR_SIZE;
		        res = RES_OK;
		        break;	 
		    case GET_BLOCK_SIZE:
		        *(WORD*)buff = FLASH_BLOCK_SIZE;
		        res = RES_OK;
		        break;	 
		    case GET_SECTOR_COUNT:
		        *(DWORD*)buff = FLASH_SECTOR_COUNT;
		        res = RES_OK;
		        break;
		    default:
		        res = RES_PARERR;
		        break;
	    }
	}else res=RES_ERROR;//�����Ĳ�֧��
    return res;
}
#endif
//���ʱ��
//User defined function to give a current time to fatfs module      */
//31-25: Year(0-127 org.1980), 24-21: Month(1-12), 20-16: Day(1-31) */                                                                                                                                                                                                                                          
//15-11: Hour(0-23), 10-5: Minute(0-59), 4-0: Second(0-29 *2) */                                                                                                                                                                                                                                                
DWORD get_fattime (void)
{				 
	return 0;
}			 
//��̬�����ڴ�
void *ff_memalloc (UINT size)			
{
	return (void*)mymalloc(0,size);
}
//�ͷ��ڴ�
void ff_memfree (void* mf)		 
{
	myfree(0,mf);
}























