/**
  ******************************************************************************
  * 文件名 : flash.c
  * 作  者 : 叶中华
  * 版  本 ：V1.0
  * 时  间 : 2012年11月22日
  * 公  司 ：汉威光电技术有限公司
  * 摘  要 ：本文件包含了W25X16模块的初始化、配置、读写操作等。
  * 说  明 ：W25X16资料	--2MByte
  *              页        扇区        块区      
  * 大  小 ：  256Byte	  4kByte	  64kByte
  * 数  量：   8192页	  512扇区	  32块
  ******************************************************************************
  */



/**
  ******************************************************************************
  *                                  头文件
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
  * 描述   : 初始化SPI FLASH的接口、模式配置
  *		   
  * 参数   : 无
  *
  *
  * 返回值 : 无
  ******************************************************************************
  */


void SPI_FlashInit(void)
{
  uint32_t i=0;
	i=i;
	Init_SPI2();		   //初始化SPI
  while(SPI_FlashReadID()!=FLASH_ID)//检测不到W25X16
  {	   
    i=SPI_FlashReadID();
#if Debug_printf
		printf("\n\r ID:%d \r\n",i);
		printf("\n\r 没有读到正确的W25X16芯片ID，请检查硬件连接 \r\n");
#endif
		Delay(0xaFFFF);
		Delay(0xaFFFF);
    GPIO_ResetBits(GPIOF,  GPIO_Pin_7);
		Delay(10);
		GPIO_SetBits(GPIOF,  GPIO_Pin_7);
		Delay(10);

  }
#if Debug_printf
  printf("\n\r 正确的W25X16芯片ID \r\n");
#endif
  SPI_FlashWriteSR(0);
}  

/**
  ******************************************************************************
  *                             SPI_FlashReadSR()
  *
  * 描述   : 读取SPI_FLASH的状态寄存器
  *			 BIT7  6   5   4   3   2   1   0
  *			 SPR   RV  TB BP2 BP1 BP0 WEL BUSY
  *			 SPR:默认0,状态寄存器保护位,配合WP使用
  *			 WEL:写使能锁定
  *			 BUSY:忙标记位(1,忙;0,空闲)
  *			 默认:0x00
  *		   
  * 参数   : 无
  *
  *
  * 返回值 : 无
  ******************************************************************************
  */
u8 SPI_FlashReadSR (void)   
{  
  u8 byte=0;   
  SPI_FLASH_CS=0;;                              //使能器件   
  SPI2_ReadWriteByte(W25X_ReadStatusReg);  //发送读取状态寄存器命令    
  byte = SPI2_ReadWriteByte(0Xff);         //读取一个字节  
  SPI_FLASH_CS=1;;                              //取消片选     
  return byte;   
} 

/**
  ******************************************************************************
  *                             SPI_FlashWritrSR()
  *
  * 描述   : 写SPI_FLASH的状态寄存器--先打开写使能然后才能写入数据
  *			 BIT7  6   5   4   3   2   1   0
  *			 SPR   RV  TB BP2 BP1 BP0 WEL BUSY
  *			 SPR:默认0,状态寄存器保护位,配合WP使用
  *			 WEL:写使能锁定
  *			 BUSY:忙标记位(1,忙;0,空闲)
  *			 默认:0x00
  *			 只有SPR,TB,BP2,BP1,BP0(bit 7,5,4,3,2)可以写!!!
  *		   
  * 参数   : 无
  *
  *
  * 返回值 : 无
  ******************************************************************************
  */
/* 好像有问题，没有进行写使!!!!!! */
void SPI_FlashWriteSR (u8 sr)   
{   
  SPI_FLASH_CS=0;                               //使能器件   
  SPI2_ReadWriteByte(W25X_WriteStatusReg);  //发送写状态寄存器命令    
  SPI2_ReadWriteByte(sr);                   //写入一个字节  
  SPI_FLASH_CS=1;                               //取消片选     	      
}   
 
/**
  ******************************************************************************
  *                           SPI_FlashWriteEnable()
  *
  * 描述   : PI_FLASH写使能,将WEL置位
  *			 
  *		   
  * 参数   : 无
  *
  *
  * 返回值 : 无
  ******************************************************************************
  */ 
   
void SPI_FlashWriteEnable (void)   
{
  SPI_FLASH_CS=0;                              //使能器件   
  SPI2_ReadWriteByte(W25X_WriteEnable);    //发送写使能  
  SPI_FLASH_CS=1;                              //取消片选     	      
}
 
 
/**
  ******************************************************************************
  *                           SPI_FlashWriteDisable()
  *
  * 描述   : PI_FLASH写禁止,将WEL清零
  *			 
  *		   
  * 参数   : 无
  *
  *
  * 返回值 : 无
  ******************************************************************************
  */ 
   
void SPI_FlashWriteDisable (void)   
{  
  SPI_FLASH_CS=0;                              //使能器件   
  SPI2_ReadWriteByte(W25X_WriteDisable);   //发送写禁止指令    
  SPI_FLASH_CS=1;                              //取消片选     	      
}

 			    
/**
  ******************************************************************************
  *                           SPI_FlashReadID()
  *
  * 描述   : 读取芯片ID W25X16的ID:0XEF14
  *			 
  *		   
  * 参数   : 无
  *
  *
  * 返回值 : Temp--W25X16的ID:0XEF14
  ******************************************************************************
  */ 

u16 SPI_FlashReadID (void)
{
  u16 Temp = 0;	  
  SPI_FLASH_CS=0;				    
  SPI2_ReadWriteByte(0x90);    //发送读取ID命令	    
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
  * 描述   : 读取flash上地址ReadAddr中的NumByteToRead个数据
  *			 
  *		   
  * 参数   : pBuffer--存放数据的内存地址
  *          ReadAddr--要读的flash上的地址
  *          NumByteToRead--读取数据的个数(最大65535)
  *
  *
  * 返回值 : 无
  ******************************************************************************
  */

void SPI_FlashRead(u8* pBuffer,u32 ReadAddr,u16 NumByteToRead)   
{ 
  u16 i; 
  SPI_FLASH_CS=0; ;                               //使能器件   
  SPI2_ReadWriteByte(W25X_ReadData);         //发送读取命令   
  SPI2_ReadWriteByte((u8)((ReadAddr)>>16));  //发送24bit地址    
  SPI2_ReadWriteByte((u8)((ReadAddr)>>8));   
  SPI2_ReadWriteByte((u8)ReadAddr);   
  for(i=0;i<NumByteToRead;i++)
  { 
    pBuffer[i]=SPI2_ReadWriteByte(0XFF);     //循环读数  
  }
  SPI_FLASH_CS=1;                                //取消片选     	      
}  


/**
  ******************************************************************************
  *                           SPI_FlashWritePage()
  *
  * 描述   : SPI在一页(0~65535)内写入少于256个字节的数据
  *			 
  *		   
  * 参数   : pBuffer--存放数据的内存地址
  *          WriteAddr--开始写入flash的地址(24bit)
  *          NumByteToRead--要写入的字节数(最大256),该数不应该超过该页的剩余字节数!!!
  *
  *
  * 返回值 : 无
  ******************************************************************************
  */	 
void SPI_FlashWritePage(u8* pBuffer,u32 WriteAddr,u16 NumByteToWrite)
{
  u16 i;  
  SPI_FlashWriteEnable();                        //SET WEL 
  SPI_FLASH_CS=0;  ;                              //使能器件   
  SPI2_ReadWriteByte(W25X_PageProgram);      //发送写页命令   
  SPI2_ReadWriteByte((u8)((WriteAddr)>>16)); //发送24bit地址    
  SPI2_ReadWriteByte((u8)((WriteAddr)>>8));   
  SPI2_ReadWriteByte((u8)WriteAddr);   
  for(i=0;i<NumByteToWrite;i++)
  {
    SPI2_ReadWriteByte(pBuffer[i]);          //循环写数  
  }
  SPI_FLASH_CS=1;                                //取消片选 
  SPI_FlashWaitBusy();					                 //等待写入结束
} 

/**
  ******************************************************************************
  *                          SPI_FlashWriteNoCheck()
  *
  * 描述   : 1、无检验写SPI FLASH,必须确保所写的地址范围内的数据全部为0XFF,否则在
  *          非0XFF处写入的数据将失败!
  *          2、具有自动换页功能，在指定地址开始写入指定长度的数据,但是要确保地址
  *          不越界!
  *			 
  *		   
  * 参数   : pBuffer--存放数据的内存地址
  *          WriteAddr--开始写入flash的地址(24bit)
  *          NumByteToRead--要写入的字节数(最大65535),
  *
  *
  * 返回值 : 无
  ******************************************************************************
  */

void SPI_FlashWriteNoCheck(u8* pBuffer,u32 WriteAddr,u16 NumByteToWrite)   
{ 			 		 
  u16 pageremain;	   
  pageremain=256-WriteAddr%256;       //单页剩余的字节数		 	    
  if(NumByteToWrite<=pageremain)pageremain=NumByteToWrite;//不大于256个字节
  while(1)
  {	   
    SPI_FlashWritePage(pBuffer,WriteAddr,pageremain);
	if(NumByteToWrite==pageremain)break;     //写入结束了
 	else //NumByteToWrite>pageremain
	{
	  pBuffer+=pageremain;
	  WriteAddr+=pageremain;	

	  NumByteToWrite-=pageremain;			       //减去已经写入了的字节数
	  if(NumByteToWrite>256)pageremain=256;  //一次可以写入256个字节
	  else pageremain=NumByteToWrite; 	     //不够256个字节了
	}
}	    
} 

/**
  ******************************************************************************
  *                          SPI_FlashWrite()
  *
  * 描述   : 写SPI FLASH
  *          在指定地址开始写入指定长度的数据
  *          该函数带擦除操作!
  *			 
  *		   
  * 参数   : pBuffer--存放数据的内存地址
  *          WriteAddr--开始写入flash的地址(24bit)
  *          NumByteToRead--要写入的字节数(最大65535),
  *
  *
  * 返回值 : 无
  ******************************************************************************
  */
    		   
// u8 SPI_FLASH_BUF[4096];
// void SPI_FlashWrite(u8* pBuffer,u32 WriteAddr,u16 NumByteToWrite)   
// { 
//   u32 secpos;
//   u16 secoff;
//   u16 secremain;	   
//   u16 i;  
//   secpos=WriteAddr/4096;//扇区地址 0~511 for w25x16
//   secoff=WriteAddr%4096;//在扇区内的偏移
//   secremain=4096-secoff;//扇区剩余空间大小   
//   if(NumByteToWrite<=secremain)secremain=NumByteToWrite;//不大于4096个字节
//   while(1) 
//   {	
// 	SPI_FlashRead(SPI_FLASH_BUF,secpos*4096,4096);//读出整个扇区的内容
// 	for(i=0;i<secremain;i++)//校验数据
// 	{
// 	  if(SPI_FLASH_BUF[secoff+i]!=0XFF)break;//需要擦除  	  
// 	}
// 	if(i<secremain)//需要擦除
// 	{
// 	  SPI_FlashEraseSector(secpos);//擦除这个扇区
// 	  for(i=0;i<secremain;i++)	   //复制
// 	  {
// 	    SPI_FLASH_BUF[i+secoff]=pBuffer[i];	  
// 	  }
// 	  SPI_FlashWriteNoCheck(SPI_FLASH_BUF,secpos*4096,4096);//写入整个扇区  
// 	}
// 	else 
// 	  SPI_FlashWriteNoCheck(pBuffer,WriteAddr,secremain);//写已经擦除了的,直接写入扇区剩余区间. 				   
// 	if(NumByteToWrite==secremain)break;//写入结束了
// 	else//写入未结束
// 	{
// 	  secpos++;//扇区地址增1
// 	  secoff=0;//偏移位置为0 	 

// 	  pBuffer+=secremain;  //指针偏移
// 	  WriteAddr+=secremain;//写地址偏移	   
// 	  NumByteToWrite-=secremain;				//字节数递减
// 	  if(NumByteToWrite>4096)secremain=4096;	//下一个扇区还是写不完
// 	  else secremain=NumByteToWrite;			//下一个扇区可以写完了
// 	}	 
//   }	 	 
// }


/**
  ******************************************************************************
  *                          SPI_FlashEraseChip()
  *
  * 描述   : 擦除整个芯片
  *          整片擦除时间:
  *          W25X16:25s
  *          W25X32:40s
  *          W25X64:40s	   等待时间超长...
  *			 
  *		   
  * 参数   : 无
  *
  *
  * 返回值 : 无
  ******************************************************************************
  */

void SPI_FlashEraseChip(void)   
{                                             
  SPI_FlashWriteEnable();                  //SET WEL 
  SPI_FlashWaitBusy();
  SPI_FLASH_CS=0;  ;                        //使能器件   
  SPI2_ReadWriteByte(W25X_ChipErase);  //发送片擦除命令  
  SPI_FLASH_CS=1;                          //取消片选     	      
  SPI_FlashWaitBusy();   				           //等待芯片擦除结束

}   

/**
  ******************************************************************************
  *                         SPI_FlashEraseSector()
  *
  * 描述   : 擦除一个扇区
  *          擦除一个扇区的最少时间:150ms
  *			 
  *		   
  * 参数   : Dst_Addr:扇区地址 0~511 for w25x16
  *
  *
  * 返回值 : 无
  ******************************************************************************
  */

void SPI_FlashEraseSector(u32 Dst_Addr)   
{   
  Dst_Addr*=4096;
  SPI_FlashWriteEnable();                      //SET WEL 	 
  SPI_FlashWaitBusy();   
  SPI_FLASH_CS=0;                              //使能器件   
  SPI2_ReadWriteByte(W25X_SectorErase);    //发送扇区擦除指令 
  SPI2_ReadWriteByte((u8)((Dst_Addr)>>16));//发送24bit地址    
  SPI2_ReadWriteByte((u8)((Dst_Addr)>>8));   
  SPI2_ReadWriteByte((u8)Dst_Addr);  
  SPI_FLASH_CS=1;                              //取消片选     	      
  SPI_FlashWaitBusy();   				       //等待擦除完成
}  

/**
  ******************************************************************************
  *                           SPI_FlashWaitBusy()
  *
  * 描述   : 等待空闲
  *			 
  *		   
  * 参数   : 无
  *
  *
  * 返回值 : 无
  ******************************************************************************
  */

void SPI_FlashWaitBusy(void)   
{   
  while ((SPI_FlashReadSR()&0x01)==0x01);   // 等待BUSY位清空
}
  
/**
  ******************************************************************************
  *                           SPI_FlashPowerDown()
  *
  * 描述   : 进入掉电模式
  *			 
  *		   
  * 参数   : 无
  *
  *
  * 返回值 : 无
  ******************************************************************************
  */
void SPI_FlashPowerDown(void)   
{ 
  SPI_FLASH_CS=0;                            //使能器件   
  SPI2_ReadWriteByte(W25X_PowerDown);    //发送掉电命令  
  SPI_FLASH_CS=1;                            //取消片选     	      
//   delay_us(3);                               //等待TPD 
  Delay(30);	
}   
/**
  ******************************************************************************
  *                           SPI_FlashPowerDown()
  *
  * 描述   : 唤醒
  *			 
  *		   
  * 参数   : 无
  *
  *
  * 返回值 : 无
  ******************************************************************************
  */
void SPI_FlashWakeUp(void)   
{  
  SPI_FLASH_CS=0;                                 //使能器件   
  SPI2_ReadWriteByte(W25X_ReleasePowerDown);  //  send W25X_PowerDown command 0xAB    
  SPI_FLASH_CS=1;                                 //取消片选     	      
//   delay_us(3);                                    //等待TRES1
	Delay(20);
}   




