#ifndef __FLASH_H__
#define __FLASH_H__

#ifdef __cplusplus
 extern "C" {
#endif
#include "sys.h"

#define	SPI_FLASH_CS PIout(0)  //ѡ��FLASH

//W25X16��д
#define FLASH_ID 0XEF14
//ָ���
#define W25X_WriteEnable		0x06 	  //дʹ��
#define W25X_WriteDisable		0x04 	  //д����
#define W25X_ReadStatusReg		0x05 	  //��ȡ״̬�Ĵ���
#define W25X_WriteStatusReg		0x01 	  //д״̬�Ĵ���
#define W25X_ReadData			0x03 	  //������
#define W25X_FastReadData		0x0B 	  //���
#define W25X_FastReadDual		0x3B 	  //���˫���
#define W25X_PageProgram		0x02 	  //ҳ���
#define W25X_BlockErase			0xD8 	  //�����
#define W25X_SectorErase		0x20 	  //��������
#define W25X_ChipErase			0xC7 	  //оƬ����
#define W25X_PowerDown			0xB9 	  //����
#define W25X_ReleasePowerDown	0xAB 
#define W25X_DeviceID			0xAB 
#define W25X_ManufactDeviceID	0x90 	  //����/����ID
#define W25X_JedecDeviceID		0x9F

extern void SPI_FlashInit(void);
extern u8   SPI_FlashReadSR (void);
extern void SPI_FlashWriteSR (u8 sr);
extern void SPI_FlashWriteEnable (void);
extern void SPI_FlashWriteDisable (void);
extern u16  SPI_FlashReadID (void);
extern void SPI_FlashRead(u8* pBuffer,u32 ReadAddr,u16 NumByteToRead);
extern void SPI_FlashWritePage(u8* pBuffer,u32 WriteAddr,u16 NumByteToWrite);
extern void SPI_FlashWriteNoCheck(u8* pBuffer,u32 WriteAddr,u16 NumByteToWrite);
// extern void SPI_FlashWrite(u8* pBuffer,u32 WriteAddr,u16 NumByteToWrite);
extern void SPI_FlashEraseChip(void);
extern void SPI_FlashEraseSector(u32 Dst_Addr);
extern void SPI_FlashWaitBusy(void);
extern void SPI_FlashPowerDown(void);
extern void SPI_FlashWakeUp(void);







#ifdef __cplusplus
}
#endif


#endif

