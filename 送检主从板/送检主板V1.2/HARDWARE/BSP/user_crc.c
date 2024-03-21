/**
  ******************************************************************************
  * 文件名 : user_crc.c
  * 作  者 : 陈光
  * 版  本 ：V1.0
  * 时  间 : 2014年4月31日
  * 公  司 ：汉威光电技术有限公司
  * 摘  要 ：本文件包含了CRC校验模块
  ******************************************************************************
  */



/**
  ******************************************************************************
  *                                  头文件
  ******************************************************************************
  */

#include  "user_crc.h"


/**
  ******************************************************************************
  *                              User_CRCConfig(void)
  *
  * 描述   : 配置CRC
  *		   
  * 参数   : 无
  *
  *
  * 返回值 : 无
  ******************************************************************************
  */

void User_CRCConfig (void)
{
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_CRC, ENABLE); /* Enable CRC clock */
}


/**
  ******************************************************************************
  *                              User_HWCRC32()
  *
  * 描述   : 配置CRC
  *		   
  * 参数   : 无
  *
  *
  * 返回值 : 无
  ******************************************************************************
  */

uint32_t UserHWCRC32(uint8_t pBuffer[], uint32_t NumOfByte)
{
  uint32_t crc32;
	uint32_t last_data;
  uint32_t NumOfDWord = NumOfByte>>2;
  uint32_t NumOfTailByte =  NumOfByte & 3 ;

  CRC_ResetDR();
  crc32 = CRC_CalcBlockCRC( (uint32_t  *)pBuffer, NumOfDWord );

  switch ( NumOfTailByte )
  {
    case 0:
      return crc32;
    case 1:
      last_data = pBuffer[NumOfByte-1] << 24;
      break;
    case 2:
      last_data = *( (uint16_t *)(&pBuffer[NumOfByte-2]) );
      last_data <<= 16;
      break;
    case 3:
      last_data = *( (uint16_t *)(&pBuffer[NumOfByte-3]) );
      last_data <<= 8;
      last_data += pBuffer[NumOfByte-1]<<24;
      break;
  }
  return CRC_CalcCRC( last_data );
}


/**
  ******************************************************************************
  *                              FrameCRC32()
  *
  * 描述   : 一帧数据的CRC校验，帧头和数据校验
  *		   
  * 参数   : 无
  *
  *
  * 返回值 : 无
  ******************************************************************************
  */
// uint32_t FrameCRC32 (FRAME_Head * frame_head,uint8_t * frame_data)
// {
//   uint32_t crc32;

//   CRC_ResetDR();
//   crc32 = UserHWCRC32(crc32, (uint8_t *) frame_head,LENGTH_FRAME_HEAD );
//   if(frame_head->data_length>0)
//     crc32 = UserHWCRC32(crc32, frame_data,frame_head->data_length );
//   return crc32;
// }





