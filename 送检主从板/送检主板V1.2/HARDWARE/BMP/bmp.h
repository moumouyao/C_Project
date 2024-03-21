/**
  ******************************************************************************
  * 文件名 : bmp.h
  * 作  者 : 陈光
  * 版  本 ：V1.0
  * 时  间 : 2014年6月22日
  * 公  司 ：汉威光电技术有限公司
  * 摘  要 ：本文件包含了BMP位图头文件
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

	 
//BMP文件格式
/********************************************************************************
 *BMP文件由文件头(bimap-file header)、位图信息头(bimap-information header)、颜色
 *表信息(也叫彩色表(color table))和图形数据四部分组成。
 *位图阵列的顺序是从图像的左下角开始逐行扫描图像，即从左到右，从下到上，将图像的
 *像素值(或颜色索引)一一记录下来。
 *BMP格式规定每行的字节数必须是4的整数倍，不足补0
 *  数据段名称          对应结构体定义          大小(BYTE)
 *  bmp文件头-1        BITMAPFILEHEADER             14
 *  位图信息头-2       BITMAPINFOHEADER             40
 *  颜色表-3             	                       由颜色索引表决定
 *  位图数据-4                                   由图像尺寸决定
 ********************************************************************************
 */

	 

	 

//文件头-1
#pragma pack(2)
typedef struct tagBITMAPFILEHEADER
{
  WORD  bfType;      //文件类型-为字母ASCII码"BM" 42H 4DH
	DWORD bfSize;      //整个BMP文件大小-4Byte
	DWORD  bfReserved1; //保留字节，必须为0-2Byte
// 	WORD  bfReserved2; //保留字节，必须为1-2Byte
	DWORD bfOffBits;   
	//图像数据的起始位置，以相对于文件头+位图信息头+彩色表的偏移量表示，以字节为单位
}BITMAPFILEHEADER;

#pragma pack()


//位图信息头-2

#pragma pack(2)
typedef struct tagBITMAPINFOHEADER
{
  DWORD biSize;         //本结构所占字节数 - 28H 00H 00H 00H
  LONG biWidth;         //位图的宽度，以像素为单位
  LONG biHeight;        //位图的高度，以像素为单位
  WORD biPlanes;        //目标设备的级别，必须为1 
  WORD biBitCount;      //每个像素的位数1/4/8/16/24/32
  DWORD biCompression;  //位图压缩类型，必须是0(不压缩), 
	                      //1(BI_RLE8压缩类型)或2(BI_RLE4压缩类型)之一
  DWORD biSizeImage;    //位图的大小，以字节为单位-图片数据，必须是4的倍数
  LONG biXPelsPerMeter; //位图水平分辨率，每米像素数
  LONG biYPelsPerMeter; //位图垂直分辨率，每米像素数
  DWORD biClrUsed;      //位图实际使用调色板中的颜色数
  DWORD biClrImportant; //位图显示过程中重要的颜色数
} BITMAPINFOHEADER;

#pragma pack()
//彩色表/颜色表/调色板-3
/**********************************************************************
 *它用于说明位图中的颜色，它有若干个表项，每个表项是一个RGBQUAD类型
 *的结构,定义一种颜色。RGBQUAD结构数据的个数由位图信息头中的biBitCount
 *来确定
 *当biBitCount=1,4,8时，分别有2,16,256个表项；
 *当biBitCount=24时,没有彩色表项
 **********************************************************************
 */

//RGBQUAD结构的定义如下：

typedef struct tagRGBQUAD
{
  BYTE rgbBlue;     //蓝色的亮度(值范围为0-255)
	BYTE rgbGreen;    //绿色的亮度(值范围为0-255)
	BYTE rgbRed;      //红色的亮度(值范围为0-255)
	BYTE rgbReserved; //保留,必须为0
} RGBQUAD;
/************************************************************************
 *注:RGBQUAD数据结构中，增加了一个保留字段rgbReserved,它不代表任何颜色必
 *须为固定的值0,同时，RGBQUAD结构中定义的颜色值中,红色、绿色和蓝色的排列
 *顺序与一般的真彩色图像文件的颜色数据排列顺序恰好相反，即:某个位图中的一
 *个像素点的颜色的描述为“00,00,ff,00”，则表示该点为红色，而不是蓝色
 ************************************************************************
 */

//把BMP文件转换为LED点阵序列
extern void BMPTranLedRGBData(uint8_t bmp_head,uint8_t bmp_no,uint8_t *buf,uint8_t yanse);


extern void bmpBitCnt4 (BITMAPINFOHEADER *info_p,uint8_t bmp_head,uint8_t bmp_no,u32 data_addr,u8 *buf,u8 yanse);
extern void bmpBitCnt8 (BITMAPINFOHEADER *info_p,uint8_t bmp_head,uint8_t bmp_no,u32 data_addr,u8 *buf,u8 yanse);
extern void bmpBitCnt24(BITMAPINFOHEADER *info_p,uint8_t bmp_head,uint8_t bmp_no,u32 data_addr,u8 *buf,u8 yanse);//参数：位图信息头指针,位图数据地址，缓冲区指针,颜色
	 

uint8_t ReadBMPInfo(char* file_name,BITMAPFILEHEADER *file_header,BITMAPINFOHEADER *info_header);
void BMP24RGBToItem(char* file_name,BITMAPINFOHEADER info_header,BITMAPFILEHEADER file_header,uint16_t posx,uint16_t posy);
void BMP4_8RGBToItem(char* file_name,BITMAPINFOHEADER info_header,BITMAPFILEHEADER file_header,uint16_t posx,uint16_t posy);


#ifdef __cplusplus
}
#endif


#endif

