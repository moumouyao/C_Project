/**
  ******************************************************************************
  * �ļ��� : user_crc.c
  * ��  �� : �¹�
  * ��  �� ��V1.0
  * ʱ  �� : 2014��4��31��
  * ��  ˾ ��������缼�����޹�˾
  * ժ  Ҫ �����ļ�������CRCУ��ģ��
  ******************************************************************************
  */



/**
  ******************************************************************************
  *                                  ͷ�ļ�
  ******************************************************************************
  */

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
  * ����   : һ֡���ݵ�CRCУ�飬֡ͷ������У��
  *		   
  * ����   : ��
  *
  *
  * ����ֵ : ��
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





