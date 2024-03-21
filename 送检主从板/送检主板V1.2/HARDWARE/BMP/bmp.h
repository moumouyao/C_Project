/**
  ******************************************************************************
  * �ļ��� : bmp.h
  * ��  �� : �¹�
  * ��  �� ��V1.0
  * ʱ  �� : 2014��6��22��
  * ��  ˾ ��������缼�����޹�˾
  * ժ  Ҫ �����ļ�������BMPλͼͷ�ļ�
  ******************************************************************************
  */
#ifndef __BMP_H__
#define __BMP_H__


#ifdef __cplusplus
 extern "C" {
#endif

	 
/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"	  
#include "integer.h"	 
//typedef unsigned char  BYTE;                   /* Unsigned  8 bit quantity */  
//typedef unsigned short WORD;                   /* Unsigned 16 bit quantity */          
//typedef unsigned int   DWORD;                  /* Unsigned 32 bit quantity */    
//typedef signed   int   LONG;                   /* Unsigned 32 bit quantity */

	 
//BMP�ļ���ʽ
/********************************************************************************
 *BMP�ļ����ļ�ͷ(bimap-file header)��λͼ��Ϣͷ(bimap-information header)����ɫ
 *����Ϣ(Ҳ�в�ɫ��(color table))��ͼ�������Ĳ�����ɡ�
 *λͼ���е�˳���Ǵ�ͼ������½ǿ�ʼ����ɨ��ͼ�񣬼������ң����µ��ϣ���ͼ���
 *����ֵ(����ɫ����)һһ��¼������
 *BMP��ʽ�涨ÿ�е��ֽ���������4�������������㲹0
 *  ���ݶ�����          ��Ӧ�ṹ�嶨��          ��С(BYTE)
 *  bmp�ļ�ͷ-1        BITMAPFILEHEADER             14
 *  λͼ��Ϣͷ-2       BITMAPINFOHEADER             40
 *  ��ɫ��-3             	                       ����ɫ���������
 *  λͼ����-4                                   ��ͼ��ߴ����
 ********************************************************************************
 */

	 

	 

//�ļ�ͷ-1
#pragma pack(2)
typedef struct tagBITMAPFILEHEADER
{
  WORD  bfType;      //�ļ�����-Ϊ��ĸASCII��"BM" 42H 4DH
	DWORD bfSize;      //����BMP�ļ���С-4Byte
	DWORD  bfReserved1; //�����ֽڣ�����Ϊ0-2Byte
// 	WORD  bfReserved2; //�����ֽڣ�����Ϊ1-2Byte
	DWORD bfOffBits;   
	//ͼ�����ݵ���ʼλ�ã���������ļ�ͷ+λͼ��Ϣͷ+��ɫ���ƫ������ʾ�����ֽ�Ϊ��λ
}BITMAPFILEHEADER;

#pragma pack()


//λͼ��Ϣͷ-2

#pragma pack(2)
typedef struct tagBITMAPINFOHEADER
{
  DWORD biSize;         //���ṹ��ռ�ֽ��� - 28H 00H 00H 00H
  LONG biWidth;         //λͼ�Ŀ�ȣ�������Ϊ��λ
  LONG biHeight;        //λͼ�ĸ߶ȣ�������Ϊ��λ
  WORD biPlanes;        //Ŀ���豸�ļ��𣬱���Ϊ1 
  WORD biBitCount;      //ÿ�����ص�λ��1/4/8/16/24/32
  DWORD biCompression;  //λͼѹ�����ͣ�������0(��ѹ��), 
	                      //1(BI_RLE8ѹ������)��2(BI_RLE4ѹ������)֮һ
  DWORD biSizeImage;    //λͼ�Ĵ�С�����ֽ�Ϊ��λ-ͼƬ���ݣ�������4�ı���
  LONG biXPelsPerMeter; //λͼˮƽ�ֱ��ʣ�ÿ��������
  LONG biYPelsPerMeter; //λͼ��ֱ�ֱ��ʣ�ÿ��������
  DWORD biClrUsed;      //λͼʵ��ʹ�õ�ɫ���е���ɫ��
  DWORD biClrImportant; //λͼ��ʾ��������Ҫ����ɫ��
} BITMAPINFOHEADER;

#pragma pack()
//��ɫ��/��ɫ��/��ɫ��-3
/**********************************************************************
 *������˵��λͼ�е���ɫ���������ɸ����ÿ��������һ��RGBQUAD����
 *�Ľṹ,����һ����ɫ��RGBQUAD�ṹ���ݵĸ�����λͼ��Ϣͷ�е�biBitCount
 *��ȷ��
 *��biBitCount=1,4,8ʱ���ֱ���2,16,256�����
 *��biBitCount=24ʱ,û�в�ɫ����
 **********************************************************************
 */

//RGBQUAD�ṹ�Ķ������£�

typedef struct tagRGBQUAD
{
  BYTE rgbBlue;     //��ɫ������(ֵ��ΧΪ0-255)
	BYTE rgbGreen;    //��ɫ������(ֵ��ΧΪ0-255)
	BYTE rgbRed;      //��ɫ������(ֵ��ΧΪ0-255)
	BYTE rgbReserved; //����,����Ϊ0
} RGBQUAD;
/************************************************************************
 *ע:RGBQUAD���ݽṹ�У�������һ�������ֶ�rgbReserved,���������κ���ɫ��
 *��Ϊ�̶���ֵ0,ͬʱ��RGBQUAD�ṹ�ж������ɫֵ��,��ɫ����ɫ����ɫ������
 *˳����һ������ɫͼ���ļ�����ɫ��������˳��ǡ���෴����:ĳ��λͼ�е�һ
 *�����ص����ɫ������Ϊ��00,00,ff,00�������ʾ�õ�Ϊ��ɫ����������ɫ
 ************************************************************************
 */

//��BMP�ļ�ת��ΪLED��������
extern void BMPTranLedRGBData(uint8_t bmp_head,uint8_t bmp_no,uint8_t *buf,uint8_t yanse);


extern void bmpBitCnt4 (BITMAPINFOHEADER *info_p,uint8_t bmp_head,uint8_t bmp_no,u32 data_addr,u8 *buf,u8 yanse);
extern void bmpBitCnt8 (BITMAPINFOHEADER *info_p,uint8_t bmp_head,uint8_t bmp_no,u32 data_addr,u8 *buf,u8 yanse);
extern void bmpBitCnt24(BITMAPINFOHEADER *info_p,uint8_t bmp_head,uint8_t bmp_no,u32 data_addr,u8 *buf,u8 yanse);//������λͼ��Ϣͷָ��,λͼ���ݵ�ַ��������ָ��,��ɫ
	 

uint8_t ReadBMPInfo(char* file_name,BITMAPFILEHEADER *file_header,BITMAPINFOHEADER *info_header);
void BMP24RGBToItem(char* file_name,BITMAPINFOHEADER info_header,BITMAPFILEHEADER file_header,uint16_t posx,uint16_t posy);
void BMP4_8RGBToItem(char* file_name,BITMAPINFOHEADER info_header,BITMAPFILEHEADER file_header,uint16_t posx,uint16_t posy);


#ifdef __cplusplus
}
#endif


#endif

