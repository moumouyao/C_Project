/**
  ******************************************************************************
  * 文件名 : bmp.c
  * 作  者 : 陈光
  * 版  本 ：V1.0
  * 时  间 : 2014年6月26日
  * 公  司 ：汉威光电技术有限公司
  * 摘  要 ：本文件包含了BMP位图文件从FLASH中读取，解析和转换成RGB源程序
  ******************************************************************************
  */



/**
  ******************************************************************************
  *                                  头文件
  ******************************************************************************
  */

#include "bmp.h"
#include "includes.h"
#include "ff.h"	
#include "ucos_ii.h"
#include "sw_global.h"
#include "memp.h"

/**
  ******************************************************************************
  *                              BMPTranLedRGBData()
  *
  * 描述   : 把BMP文件转换为LED点阵序列，排列按红(btye)、绿(byte),蓝(byte)或红、绿
	*          由yanse决定
  *		   
  * 参数   : bmp_addr-nor flash中BMP文件地址
	*          yanse-点阵颜色种类2-红.绿;3-红、绿、蓝
	*          buf-转换后数据的缓存地址
  *
  *
  * 返回值 : 无
  *********************************************************************** *******
  */ 
void BMPTranLedRGBData(uint8_t bmp_head,uint8_t bmp_no,uint8_t *buf,uint8_t yanse)
{
  //u32 bmp_addr;
	BITMAPFILEHEADER file_header;   //bmp文件头
	BITMAPINFOHEADER info_header;   //bmp信息头
	uint8_t err;
	int res;  
	FIL fdst;
  FATFS fs;
  UINT br=0;            // File R/W count
	
	BYTE file_name[24];
	res = res;//消除警告
  if(bmp_no<10)
/*****************************************************************************************/
	sprintf((char*)file_name,"0:/bmp/%c0%d.bmp",bmp_head,bmp_no);   // 解析文件名并存储
  else	
	sprintf((char*)file_name,"0:/bmp/%c%d.bmp",bmp_head,bmp_no);   // 解析文件名并存储
	
	OSSemPend(SDSem,0,&err);
	f_mount(0,&fs);		
	
	res = f_open(&fdst,(char*)file_name, FA_OPEN_EXISTING | FA_READ);
	
	res = f_read(&fdst,(u16 *)&file_header, sizeof(BITMAPFILEHEADER), &br);
	
	f_close(&fdst);/*关闭文件 */
	f_mount(0,NULL);
	
	
	f_mount(0,&fs);		
	res = f_open(&fdst,(char*)file_name, FA_OPEN_EXISTING | FA_READ);
	
	res = f_lseek(&fdst,sizeof(BITMAPFILEHEADER));
	
	res = f_read(&fdst,(u16 *)&info_header, sizeof(BITMAPINFOHEADER), &br);

	f_close(&fdst);/*关闭文件 */
	f_mount(0,NULL);
	OSSemPost(SDSem);
//	bmp_addr = bmp_no*BLOCK_SIZE+BMP_LIST;
//	FSMC_NOR_ReadBuffer((u16 *)&file_header, bmp_addr, sizeof(BITMAPFILEHEADER)/2);//读取bmp文件头
//	FSMC_NOR_ReadBuffer((u16 *)&info_header, bmp_addr+sizeof(BITMAPFILEHEADER), sizeof(BITMAPINFOHEADER)/2);//读取bmp信息头
//	printf("\n\r bmp%c%d,bfSize=%2d,位图数据起始位置偏移%d",bmp_head,bmp_no,file_header.bfSize,file_header.bfOffBits);
//	printf("\n\r 位图宽度:%d,位图高度:%d,像素位数:%d",info_header.biWidth,info_header.biHeight,info_header.biBitCount);
//	printf("\n\r 位图压缩类型(0不压缩,1-BI_RLE8,2-BI_RLE4):%d",info_header.biCompression);
//	printf("\n\r 位图size:%d,位图实际使用颜色数:%d",info_header.biSizeImage,info_header.biClrUsed);
	IWDG_ReloadCounter();
//如果不是位图文件或是位图文件不存在应该退出-未做	
// 	FSMC_NOR_ReadBuffer((u16 *)buf,BMP_LIST + BLOCK_SIZE*4, 2048/2);//读取bmp--调试用
	switch(info_header.biBitCount)
	{
    case 1:
			break;
		case 4:
			bmpBitCnt4(&info_header,bmp_head,bmp_no,file_header.bfOffBits,buf,yanse);//
			break;
		case 8:
			bmpBitCnt8(&info_header,bmp_head,bmp_no,file_header.bfOffBits,buf,yanse);//
		  break;
		case 16:
			break;
		case 24:
			bmpBitCnt24(&info_header,bmp_head,bmp_no,file_header.bfOffBits,buf,yanse);//参数：位图信息头指针，位图数据地址，缓冲区指针，颜色
			break;
		default:break;
  }
	
}


void bmpBitCnt4 (BITMAPINFOHEADER *info_p,uint8_t bmp_head,uint8_t bmp_no,u32 data_addr,uint8_t *buf,uint8_t yanse)
{
  u32 biwidth1;//位图存储数据宽度字节数
	u32 rgbwidth;//RGB存储数据宽度位数-必须是字节整数倍
	u32 i,j;
	u8 k,bcnt=0;
	int8_t z;
	uint8_t err;
	u8 *p,test=0,*backup_datap,*datap;
	RGBQUAD *rgbquad_p;
	
	int res;  
	FIL fdst;
  FATFS fs;
  UINT br=0;            // File R/W count
 	BYTE file_name[24];
  if(bmp_no<10)
	sprintf((char*)file_name,"0:/bmp/%c0%d.bmp",bmp_head,bmp_no);   // 解析文件名并存储
  else	
  sprintf((char*)file_name,"0:/bmp/%c%d.bmp",bmp_head,bmp_no);   // 解析文件名并存储
	
	res = res;//消除警告
	*buf++ = info_p->biWidth;   //存储像素宽度
	*buf++ = info_p->biHeight;  //存储像素高度
	biwidth1 = (((info_p->biWidth*info_p->biBitCount)+31)/32)*4;//每一行扫描字节数必须是4的倍数
  rgbwidth = ((info_p->biWidth+7)/8)*8;//RGB数据不足一字节，则补充对齐
	for(j=0;j<rgbwidth/8*info_p->biHeight*yanse;j++) //根据图片大小进行清屏-不足整字节补足
    buf[j] = 0;
	p = memp_malloc(MEMP_PBUF_POOL);//申请一块内存-两用，1、颜色表16*4，2、位图数据
	while(p == NULL)
	{
		test++;
		if(test == 1)
			//printf("\n\r获取内存失败：bmpBitCnt4");
		OSTimeDly(5);
		p = memp_malloc(MEMP_PBUF_POOL);    //申请一块内存用于bmp解析
	}
	OSSemPend(SDSem,0,&err);
  //1-读颜色表
	if(info_p->biClrUsed == 0 || info_p->biClrUsed == 16)
	{
		
	  f_mount(0,&fs);
	
	  res = f_open(&fdst,(char*)file_name, FA_OPEN_EXISTING | FA_READ);
		
		res = f_lseek(&fdst,data_addr-64);
		
		res = f_read(&fdst,(u16 *)p, 64, &br);
		

		f_close(&fdst);/*关闭文件 */	
		f_mount(0,NULL);
//		FSMC_NOR_ReadBuffer((u16 *)p,data_addr-64,64/2);//16个RGBQUAD
		datap = p + 64;//定位位图数据
	}
	else
	{
	  f_mount(0,&fs);		
	
	  res = f_open(&fdst,(char*)file_name, FA_OPEN_EXISTING | FA_READ);
		
		res = f_lseek(&fdst,data_addr-info_p->biClrUsed*4);
		
		res = f_read(&fdst,(u16 *)p, info_p->biClrUsed*4, &br);
		

		f_close(&fdst);/*关闭文件 */
    f_mount(0,NULL);		
//  FSMC_NOR_ReadBuffer((u16 *)p,data_addr-info_p->biClrUsed*4,info_p->biClrUsed*4/2);//info_p->biClrUsed个RGBQUAD
    datap = p + info_p->biClrUsed*4;//定位位图数据
  }
	
	
	rgbquad_p = (RGBQUAD *)p;
	//2-读取位图数据
	backup_datap = datap;
	for(i=info_p->biHeight;i>0;i--)//高度-因为BMP是从下到上，从左到右，我们先从下解析
	{
	  datap = backup_datap;
		
	  f_mount(0,&fs);		
	
	  res = f_open(&fdst,(char*)file_name, FA_OPEN_EXISTING | FA_READ);
		
		res = f_lseek(&fdst,data_addr+(i-1)*biwidth1);
		
		res = f_read(&fdst,(u16 *)datap, biwidth1, &br);
		
		f_close(&fdst);/*关闭文件 */		
		f_mount(0,NULL);
		
//		FSMC_NOR_ReadBuffer((u16 *)datap, data_addr+(i-1)*biwidth1, biwidth1/2);//一次解析一行
		for(j=0;j<rgbwidth/2;j++)//每一个像素点-4位-索引一个颜色表，对应LED红/绿/蓝各一位(bit)
		{
			for(z=1;z>=0;z--)
			{
			  for(k=0;k<yanse;k++)
			  {
          *(buf+k) <<= 1;
				  switch(k)
				  {
            case 0://R
// 							temp = *datap>>(4*z) & 0x0f;
// 						  temp = rgbquad_p[temp].rgbRed;
// 						  if(temp/100 > 0)		
 					    if(rgbquad_p[*datap>>(4*z) & 0x0f].rgbRed/100 > 0)
						    *(buf+k) |= 0x01;
						  break;
					  case 1://G
					    if(rgbquad_p[*datap>>(4*z) & 0x0f].rgbGreen/100 > 0)
						    *(buf+k) |= 0x01;
						  break;
					  case 2://B
					    if(rgbquad_p[*datap>>(4*z) & 0x0f].rgbBlue/100 > 0)
						    *(buf+k) |= 0x01;
						  break;
					  default:break;
          }
        }
				bcnt++;
			  if(bcnt == 8)
			  {
          bcnt = 0;
          buf += yanse;
        }
		  }
      datap++;   //bmp数据指针datap定位
    }				
	}
	OSSemPost(SDSem);
  memp_free(MEMP_PBUF_POOL,(void *)p);//释放POOL内存	                 

//	OSMemPut(MemPoint,p);	  //释放内存
}





void bmpBitCnt8 (BITMAPINFOHEADER *info_p,uint8_t bmp_head,uint8_t bmp_no,u32 data_addr,u8 *buf,u8 yanse)
{
  u32 biwidth1;//位图存储数据宽度字节数
	u32 rgbwidth;//RGB存储数据宽度位数-必须是字节整数倍
	u32 i,j;
	u8 k,bcnt=0;
	u8 *p,test=0,*backup_datap,*datap;
	RGBQUAD *rgbquad_p;
	uint8_t err;
	int res;  
	FIL fdst;
  FATFS fs;
  UINT br=0;            // File R/W count
	res = res;//消除警告
	BYTE file_name[24];
	
  if(bmp_no<10)
	sprintf((char*)file_name,"0:/bmp/%c0%d.bmp",bmp_head,bmp_no);   // 解析文件名并存储
  else	
  sprintf((char*)file_name,"0:/bmp/%c%d.bmp",bmp_head,bmp_no);   // 解析文件名并存储
	
	*buf++ = info_p->biWidth;   //存储像素宽度
	*buf++ = info_p->biHeight;  //存储像素高度
	biwidth1 = (((info_p->biWidth*info_p->biBitCount)+31)/32)*4;//每一行扫描字节数必须是4的倍数
	rgbwidth = ((info_p->biWidth+7)/8)*8;
	for(j=0;j<rgbwidth/8*info_p->biHeight*yanse;j++)//根据图片大小进行清屏-不足整字节补足
    buf[j] = 0;
	p = memp_malloc(MEMP_PBUF_POOL);//申请一块内存-两用，1、颜色表256*4，2、位图数据
	while(p == NULL)
	{
		test++;
		if(test == 1)
			//printf("\n\r获取内存失败：bmpBitCnt8");
		OSTimeDly(5);
		p = memp_malloc(MEMP_PBUF_POOL);     //申请一块内存用于bmp解析
	}
  //1-读颜色表
	OSSemPend(SDSem,0,&err);
	if(info_p->biClrUsed == 0 || info_p->biClrUsed == 256)
	{
	  f_mount(0,&fs);		
	
	  res = f_open(&fdst,(char*)file_name, FA_OPEN_EXISTING | FA_READ);
		
		res = f_lseek(&fdst,data_addr-1024);
		
		res = f_read(&fdst,(u16 *)p, 1024, &br);
		
		f_close(&fdst);/*关闭文件 */	
		f_mount(0,NULL);
		
//		FSMC_NOR_ReadBuffer((u16 *)p,data_addr-1024,1024/2);//256个RGBQUAD
		datap = p + 1024;//定位位图数据
	}
	else
	{
	  f_mount(0,&fs);		
	
	  res = f_open(&fdst,(char*)file_name, FA_OPEN_EXISTING | FA_READ);
		
		res = f_lseek(&fdst,data_addr-info_p->biClrUsed*4);
		
		res = f_read(&fdst,(u16 *)p, info_p->biClrUsed*4, &br);
		
		f_close(&fdst);/*关闭文件 */	
		f_mount(0,NULL);
//    FSMC_NOR_ReadBuffer((u16 *)p,data_addr-info_p->biClrUsed*4,info_p->biClrUsed*4/2);//info_p->biClrUsed个RGBQUAD
    datap = p + info_p->biClrUsed*4;//定位位图数据
  }
	rgbquad_p = (RGBQUAD *)p;
	//2-读取位图数据
	backup_datap = datap;
	for(i=info_p->biHeight;i>0;i--)//高度-因为BMP是从下到上，从左到右，我们先从下解析
	{
	  datap = backup_datap;
		
	  f_mount(0,&fs);		
	
	  res = f_open(&fdst,(char*)file_name, FA_OPEN_EXISTING | FA_READ);
		
		res = f_lseek(&fdst,data_addr+(i-1)*biwidth1);
		
		res = f_read(&fdst,(u16 *)datap, biwidth1, &br);
		
		f_close(&fdst);/*关闭文件 */	
		f_mount(0,NULL);
		
//		FSMC_NOR_ReadBuffer((u16 *)datap, data_addr+(i-1)*biwidth1, biwidth1/2);//一次解析一行
		for(j=0;j<rgbwidth;j++)//每一个像素点-1个字节-索引一个颜色表，对应LED红/绿/蓝各一位(bit)
		{
			for(k=0;k<yanse;k++)
			{
        *(buf+k) <<= 1;
				switch(k)
				{
          case 0://R
					  if(rgbquad_p[*datap].rgbRed/100 > 0)
						  *(buf+k) |= 0x01;	
						break;
					case 1://G
					  if(rgbquad_p[*datap].rgbGreen/100 > 0)
						  *(buf+k) |= 0x01;
						break;
					case 2://B
					  if(rgbquad_p[*datap].rgbBlue/100 > 0)
						  *(buf+k) |= 0x01;
						break;
					default:break;
        }
      }
			datap++;   //bmp数据指针datap定位
			bcnt++;
			if(bcnt == 8)
			{
        bcnt = 0;
				buf += yanse;
      }
    }		
	}
	OSSemPost(SDSem);
	memp_free(MEMP_PBUF_POOL,(void *)p);
//	OSMemPut(MemPoint,p);	  //释放内存
}



void bmpBitCnt24 (BITMAPINFOHEADER *info_p,uint8_t bmp_head,uint8_t bmp_no,u32 data_addr,u8 *buf,u8 yanse)//参数：位图数据地址，缓冲区指针
{
  u32 biwidth1,lastdata;//位图存储数据宽度字节数
	u32 rgbwidth;//RGB存储数据宽度位数-必须是字节整数倍
	u32 i,j,bt;
	u8 k,bcnt=0,rt;
	u8 p[7000];//,test=0;
	uint8_t err;
	int res;  
	FIL fdst;
  FATFS fs;
  UINT br=0;            // File R/W count
	res = res;//防止警告
	BYTE file_name[24];
	if(bmp_no<10)
	sprintf((char*)file_name,"0:/bmp/%c0%d.bmp",bmp_head,bmp_no);   // 解析文件名并存储
  else	
  sprintf((char*)file_name,"0:/bmp/%c%d.bmp",bmp_head,bmp_no);   // 解析文件名并存储
	
	*buf++ = info_p->biWidth;   //存储像素宽度
	*buf++ = info_p->biHeight;  //存储像素高度
	biwidth1 = (((info_p->biWidth*info_p->biBitCount)+31)/32)*4;//每一行扫描字节数 必须是4的倍数
	rgbwidth = ((info_p->biWidth+7)/8)*8;//RGB存储数据宽度位数-必须是字节整数倍
	for(j=0;j<rgbwidth/8*info_p->biHeight*yanse;j++)//根据图片大小进行清屏-不足整字节补足
    buf[j] = 0;
//	p = memp_malloc(MEMP_PBUF_POOL);
//	//p = OSMemGet(MemPoint,&err);//申请一块内存
//	while(p == NULL)
//	{
//		test++;
//		if(test == 1)
//			printf("\n\r获取内存失败：bmpBitCnt24");
//		OSTimeDly(5);
//		p = memp_malloc(MEMP_PBUF_POOL);
//		//p = OSMemGet(MemPoint,&err);     //申请一块内存用于bmp解析
//	}
//	backupp = p;
	OSSemPend(SDSem,0,&err);
	for(i=info_p->biHeight;i>0;i--)//高度-因为BMP是从下到上，从左到右，我们先从下解析
	{
  //  p = backupp;
		
//		//FSMC_NOR_ReadBuffer((u16 *)p, data_addr+(i-1)*biwidth1, biwidth1/2);//一次解析一行
		if(biwidth1<510)
		{
			f_mount(0,&fs);		
			
			res = f_open(&fdst,(char*)file_name, FA_OPEN_EXISTING | FA_READ);
			
			res = f_lseek(&fdst,data_addr+(i-1)*biwidth1);
		
			res = f_read(&fdst,p, biwidth1, &br);
			

			f_close(&fdst);/*关闭文件 */
			f_mount(0,NULL);
		}
		else
		{
			rt=biwidth1/510;
		  lastdata=biwidth1-rt*510;
			
		 for(bt=0;bt<rt;bt++)
		 {
			f_mount(0,&fs);;		
			
			res = f_open(&fdst,(char*)file_name, FA_OPEN_EXISTING | FA_READ);
			
			res = f_lseek(&fdst,data_addr+(i-1)*biwidth1+bt*510);
		
			res = f_read(&fdst,p+bt*510, 510, &br);
			
			f_close(&fdst);/*关闭文件 */
			f_mount(0, NULL);	 
		 }
		if(lastdata!=0)
		 {
			f_mount(0,&fs);		
			
			res = f_open(&fdst,(char*)file_name, FA_OPEN_EXISTING | FA_READ);
			
			res = f_lseek(&fdst,data_addr+(i-1)*biwidth1+bt*510);
		
			res = f_read(&fdst,p+bt*510, lastdata, &br);
			
			f_close(&fdst);/*关闭文件 */
			f_mount(0, NULL);	 
		 }
			
		}

		for(j=0,bt=0;j<rgbwidth;j++)//每一个像素点-三个字节，对应LED蓝/绿/红各一位(bit)
		{
			for(k=3;k>3-yanse;k--)
			{
        *(buf+3-k) <<= 1;
				if(p[bt+k-1]/128 > 0)
					*(buf+3-k) |= 0x01;
      }
      bcnt++;
      if(bcnt == 8)
      {
         bcnt = 0;
			buf += yanse;
      }
			bt += 3;   //bmp指针p定位
    }
		
	for(bt=0;bt<7000;bt++)
	p[bt]=0;
  }
	OSSemPost(SDSem);
 // memp_free(MEMP_PBUF_POOL,(void *)p);
	//OSMemPut(MemPoint,backupp);	  //释放内存
}

/*****************************以下为改进版本*****2019.12********************************************/
void BMP24RGBToItem(char* file_name,BITMAPINFOHEADER info_header,BITMAPFILEHEADER file_header,uint16_t posx,uint16_t posy)
{
	uint8_t err;
	int res;  
	FIL fdst;
	FATFS fs;
	UINT br=0;            // File R/W count
	u32 biwidth1;//位图存储数据宽度字节数
	uint16_t i,j,k;
	uint16_t posx_temp;
	uint8_t *p[3],bgr[3];
	uint16_t biWidth,biHeight;
	
	posx_temp = posx;
	
	biWidth = info_header.biWidth;   //存储像素宽度
	biHeight= info_header.biHeight;  //存储像素高度
	
	biwidth1 = (((info_header.biWidth*info_header.biBitCount)+31)/32)*4;//每一行扫描字节数 必须是4的倍数
	
	OSSemPend(SDSem,0,&err);
	f_mount(0,&fs);

	for(i=0;i<biHeight;i++)
	{
		if(posy >= SCREEN_LIE)	break;		//边界处理
		
		res = f_open(&fdst,(char*)file_name, FA_OPEN_EXISTING | FA_READ);
		if(res!=FR_OK)
		{
			f_mount(0,NULL);
			OSSemPost(SDSem);
			return;
		}
		
		res = f_lseek(&fdst,file_header.bfOffBits+(biHeight-i-1)*biwidth1);
		if(res!=FR_OK)
		{
			f_close(&fdst);/*关闭文件 */
			f_mount(0,NULL);
			OSSemPost(SDSem);
			return;
		}
		
		posx = posx_temp;

		for(j=0;j<biWidth;j++)
		{
			if(posx >= SCREEN_HANG)	break;//边界处理
			
			p[2] = (posx>>3)+ItemBuffer.rgb_buf[0]+posy*SCREEN_HBYTE;//按字节定位	//R
			p[1] = (posx>>3)+ItemBuffer.rgb_buf[1]+posy*SCREEN_HBYTE;//按字节定位	//G
			p[0] = (posx>>3)+ItemBuffer.rgb_buf[2]+posy*SCREEN_HBYTE;//按字节定位	//B
			
			res = f_read(&fdst,bgr,3,&br);
			if(res!=FR_OK)
			{
				f_close(&fdst);/*关闭文件 */
				f_mount(0,NULL);
				OSSemPost(SDSem);
				return;
			}
			
			for(k=0;k<3;k++)
			{
				if(bgr[k]>=128)
				{
					*p[k] |= 1<<(7-posx%8);
				}else
				{
					*p[k] &= ~(1<<(7-posx%8));
				}
			}
			posx++;
		}
		f_close(&fdst);
		posy++;
	}
	f_mount(0, NULL);	 
	OSSemPost(SDSem);
	return;
}	

void BMP4_8RGBToItem(char* file_name,BITMAPINFOHEADER info_header,BITMAPFILEHEADER file_header,uint16_t posx,uint16_t posy)
{
	uint8_t err;
	int res;  
	FIL fdst;
	FATFS fs;
	UINT br=0;            // File R/W count
	u32 biwidth1;//位图存储数据宽度字节数
	uint16_t i,j;
	uint16_t posx_temp;
	uint8_t *p[3],rgb,*pot;
	RGBQUAD *rgbquad_p;
	uint16_t biWidth,biHeight;//,bitcnt=0;
	u32 bmpPosByte;
	uint8_t byte_H,byte_L;
	
	pot = memp_malloc(MEMP_PBUF_POOL);       //申请一块内存用于BMP数据解析--调色板
	while(pot == NULL)
	{
		OSTimeDly(1);
		pot = memp_malloc(MEMP_PBUF_POOL);
	}

	if(info_header.biBitCount == 4)						
	{
		if(info_header.biClrUsed == 0 || info_header.biClrUsed == 16)
			bmpPosByte = 16*4;
		else
			bmpPosByte = info_header.biClrUsed*4;

		biWidth = info_header.biWidth/2;   //存储像素宽度
	}else if(info_header.biBitCount == 8)
	{
		if(info_header.biClrUsed == 0 || info_header.biClrUsed == 256)
			bmpPosByte = 256*4;
		else
			bmpPosByte = info_header.biClrUsed*4;
		
		biWidth = info_header.biWidth;   //存储像素宽度
	}

	biHeight= info_header.biHeight;  //存储像素高度
		
	posx_temp = posx;
	
	biwidth1 = (((info_header.biWidth*info_header.biBitCount)+31)/32)*4;//每一行扫描字节数 必须是4的倍数
	
	OSSemPend(SDSem,0,&err);
	f_mount(0,&fs);
	//1-读颜色表
	res = f_open(&fdst,(char*)file_name, FA_OPEN_EXISTING | FA_READ);
	if(res!=FR_OK)
	{
		f_mount(0,NULL);
    memp_free(MEMP_PBUF_POOL,(void *)pot);//释放POOL内存	                 

//		OSMemPut(MemPoint,pot);
		OSSemPost(SDSem);
		return;
	}
	res = f_lseek(&fdst,file_header.bfOffBits-bmpPosByte);
	if(res!=FR_OK)
	{
		f_close(&fdst);
		f_mount(0,NULL);
    memp_free(MEMP_PBUF_POOL,(void *)pot);//释放POOL内存	                 
    
//		OSMemPut(MemPoint,pot);
		OSSemPost(SDSem);
		return;
	}
	
	res = f_read(&fdst,pot,bmpPosByte,&br);
	if(res!=FR_OK)
	{
		f_close(&fdst);/*关闭文件 */
		f_mount(0,NULL);
    memp_free(MEMP_PBUF_POOL,(void *)pot);//释放POOL内存	                   
//		OSMemPut(MemPoint,pot);	  //释放内存
		OSSemPost(SDSem);
		return;
	}
	
	rgbquad_p = (RGBQUAD *)pot;
	
	for(i=0;i<biHeight;i++)
	{
		if(posy >= SCREEN_LIE)	break;		//边界处理
		
		res = f_open(&fdst,(char*)file_name, FA_OPEN_EXISTING | FA_READ);
		if(res!=FR_OK)
		{
			f_mount(0,NULL);
      memp_free(MEMP_PBUF_POOL,(void *)pot);//释放POOL内存	                 
//			OSMemPut(MemPoint,pot);	  //释放内存
			OSSemPost(SDSem);
			return;
		}
		
		res = f_lseek(&fdst,file_header.bfOffBits+(biHeight-i-1)*biwidth1);
		if(res!=FR_OK)
		{
			f_close(&fdst);/*关闭文件 */
			f_mount(0,NULL);
      memp_free(MEMP_PBUF_POOL,(void *)pot);//释放POOL内存	                 
//			OSMemPut(MemPoint,pot);	  //释放内存
			OSSemPost(SDSem);
			return;
		}
		
		posx = posx_temp;

		for(j=0;j<biWidth;j++)
		{
			if(posx >= SCREEN_HANG)	break;//边界处理
			
			p[0] = (posx>>3)+ItemBuffer.rgb_buf[0]+posy*SCREEN_HBYTE;//按字节定位	//R
			p[1] = (posx>>3)+ItemBuffer.rgb_buf[1]+posy*SCREEN_HBYTE;//按字节定位	//G
			p[2] = (posx>>3)+ItemBuffer.rgb_buf[2]+posy*SCREEN_HBYTE;//按字节定位	//B
			
			res = f_read(&fdst,&rgb,1,&br);
			if(res!=FR_OK)
			{
				f_close(&fdst);/*关闭文件 */
				f_mount(0,NULL);
        memp_free(MEMP_PBUF_POOL,(void *)pot);//释放POOL内存	                 
//				OSMemPut(MemPoint,pot);	  //释放内存
				OSSemPost(SDSem);
				return;
			}
			
			if(info_header.biBitCount == 4)						
			{
				byte_H = rgb >> 4;
				byte_L = rgb & 0x0f;
				
				if(rgbquad_p[byte_H].rgbRed >= 100)
					*p[0] |= 1<<(7-posx%8);			
				else
					*p[0] &= ~(1<<(7-posx%8));
				if(rgbquad_p[byte_H].rgbGreen >= 100)
					*p[1] |= 1<<(7-posx%8);			
				else
					*p[1] &= ~(1<<(7-posx%8));
				if(rgbquad_p[byte_H].rgbBlue >= 100)
					*p[2] |= 1<<(7-posx%8);			
				else
					*p[2] &= ~(1<<(7-posx%8));
				
				posx++;
				if(posx >= SCREEN_HANG)	break;//边界处理
				p[0] = (posx>>3)+ItemBuffer.rgb_buf[0]+posy*SCREEN_HBYTE;//按字节定位	//R
				p[1] = (posx>>3)+ItemBuffer.rgb_buf[1]+posy*SCREEN_HBYTE;//按字节定位	//G
				p[2] = (posx>>3)+ItemBuffer.rgb_buf[2]+posy*SCREEN_HBYTE;//按字节定位	//B
				
				if(rgbquad_p[byte_L].rgbRed >= 100)
					*p[0] |= 1<<(7-posx%8);			
				else
					*p[0] &= ~(1<<(7-posx%8));
				if(rgbquad_p[byte_L].rgbGreen >= 100)
					*p[1] |= 1<<(7-posx%8);			
				else
					*p[1] &= ~(1<<(7-posx%8));
				if(rgbquad_p[byte_L].rgbBlue >= 100)
					*p[2] |= 1<<(7-posx%8);			
				else
					*p[2] &= ~(1<<(7-posx%8));
				
				posx++;
			}else if(info_header.biBitCount == 8)
			{
				if(rgbquad_p[rgb].rgbRed / 100 > 0)
					*p[0] |= 1<<(7-posx%8);			
				else
					*p[0] &= ~(1<<(7-posx%8));
				
				if(rgbquad_p[rgb].rgbGreen / 100 > 0)
					*p[1] |= 1<<(7-posx%8);			
				else
					*p[1] &= ~(1<<(7-posx%8));
				
				if(rgbquad_p[rgb].rgbBlue / 100 > 0)
					*p[2] |= 1<<(7-posx%8);			
				else
					*p[2] &= ~(1<<(7-posx%8));
				posx++;
			}
		}
		f_close(&fdst);
		posy++;
	}
	f_mount(0, NULL);	
  memp_free(MEMP_PBUF_POOL,(void *)pot);//释放POOL内存	                 
//	OSMemPut(MemPoint,pot);	  //释放内存
	OSSemPost(SDSem);
	return;
}


uint8_t ReadBMPInfo(char* file_name,BITMAPFILEHEADER *file_header,BITMAPINFOHEADER *info_header)
{
	int res;  
	FIL fdst;
  FATFS fs;
  UINT br=0;            // File R/W count
	uint8_t err;
	
	OSSemPend(SDSem,0,&err);
	f_mount(0,&fs);		
	
	res = f_open(&fdst,(char*)file_name, FA_OPEN_EXISTING | FA_READ);
	if(res!=FR_OK)
	{
		f_mount(0,NULL);
		OSSemPost(SDSem);
		return 1;
	}
	
	res = f_read(&fdst,(u16 *)file_header, sizeof(BITMAPFILEHEADER), &br);
	if(res!=FR_OK)
	{
		f_close(&fdst);/*关闭文件 */
		f_mount(0,NULL);
		OSSemPost(SDSem);
		return 1;
	}
	
	res = f_read(&fdst,(u16 *)info_header, sizeof(BITMAPINFOHEADER), &br);
	if(res!=FR_OK)
	{
		f_close(&fdst);/*关闭文件 */
		f_mount(0,NULL);
		OSSemPost(SDSem);
		return 1;
	}
	f_close(&fdst);/*关闭文件 */
	f_mount(0,NULL);
	OSSemPost(SDSem);
	return 0;
}
