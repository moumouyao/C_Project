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
	
	//复位CRC计算数据寄存器
  CRC_ResetDR();
	//计算32位CRC校验数据
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
	//计算32位CRC校验数据
  return CRC_CalcCRC( last_data );
}
