#include  "user_crc.h"
/**
  ******************************************************************************
  *                              User_CRCConfig(void)
  *
  * ����   : ����CRC
  *		   
  * ����   : ��
  *
  *
  * ����ֵ : ��
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
  * ����   : ����CRC
  *		   
  * ����   : ��
  *
  *
  * ����ֵ : ��
  ******************************************************************************
  */

uint32_t UserHWCRC32(uint8_t pBuffer[], uint32_t NumOfByte)
{
  uint32_t crc32;
	uint32_t last_data;
  uint32_t NumOfDWord = NumOfByte>>2;
  uint32_t NumOfTailByte =  NumOfByte & 3 ;
	
	//��λCRC�������ݼĴ���
  CRC_ResetDR();
	//����32λCRCУ������
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
	//����32λCRCУ������
  return CRC_CalcCRC( last_data );
}
