/**
  ******************************************************************************
  * �ļ��� : bmp.c
  * ��  �� : �¹�
  * ��  �� ��V1.0
  * ʱ  �� : 2014��6��26��
  * ��  ˾ ��������缼�����޹�˾
  * ժ  Ҫ �����ļ�������BMPλͼ�ļ���FLASH�ж�ȡ��������ת����RGBԴ����
  ******************************************************************************
  */



/**
  ******************************************************************************
  *                                  ͷ�ļ�
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
  * ����   : ��BMP�ļ�ת��ΪLED�������У����а���(btye)����(byte),��(byte)��졢��
	*          ��yanse����
  *		   
  * ����   : bmp_addr-nor flash��BMP�ļ���ַ
	*          yanse-������ɫ����2-��.��;3-�졢�̡���
	*          buf-ת�������ݵĻ����ַ
  *
  *
  * ����ֵ : ��
  *********************************************************************** *******
  */ 
void BMPTranLedRGBData(uint8_t bmp_head,uint8_t bmp_no,uint8_t *buf,uint8_t yanse)
{
  //u32 bmp_addr;
	BITMAPFILEHEADER file_header;   //bmp�ļ�ͷ
	BITMAPINFOHEADER info_header;   //bmp��Ϣͷ
	uint8_t err;
	int res;  
	FIL fdst;
  FATFS fs;
  UINT br=0;            // File R/W count
	
	BYTE file_name[24];
	res = res;//��������
  if(bmp_no<10)
/*****************************************************************************************/
	sprintf((char*)file_name,"0:/bmp/%c0%d.bmp",bmp_head,bmp_no);   // �����ļ������洢
  else	
	sprintf((char*)file_name,"0:/bmp/%c%d.bmp",bmp_head,bmp_no);   // �����ļ������洢
	
	OSSemPend(SDSem,0,&err);
	f_mount(0,&fs);		
	
	res = f_open(&fdst,(char*)file_name, FA_OPEN_EXISTING | FA_READ);
	
	res = f_read(&fdst,(u16 *)&file_header, sizeof(BITMAPFILEHEADER), &br);
	
	f_close(&fdst);/*�ر��ļ� */
	f_mount(0,NULL);
	
	
	f_mount(0,&fs);		
	res = f_open(&fdst,(char*)file_name, FA_OPEN_EXISTING | FA_READ);
	
	res = f_lseek(&fdst,sizeof(BITMAPFILEHEADER));
	
	res = f_read(&fdst,(u16 *)&info_header, sizeof(BITMAPINFOHEADER), &br);

	f_close(&fdst);/*�ر��ļ� */
	f_mount(0,NULL);
	OSSemPost(SDSem);
//	bmp_addr = bmp_no*BLOCK_SIZE+BMP_LIST;
//	FSMC_NOR_ReadBuffer((u16 *)&file_header, bmp_addr, sizeof(BITMAPFILEHEADER)/2);//��ȡbmp�ļ�ͷ
//	FSMC_NOR_ReadBuffer((u16 *)&info_header, bmp_addr+sizeof(BITMAPFILEHEADER), sizeof(BITMAPINFOHEADER)/2);//��ȡbmp��Ϣͷ
//	printf("\n\r bmp%c%d,bfSize=%2d,λͼ������ʼλ��ƫ��%d",bmp_head,bmp_no,file_header.bfSize,file_header.bfOffBits);
//	printf("\n\r λͼ���:%d,λͼ�߶�:%d,����λ��:%d",info_header.biWidth,info_header.biHeight,info_header.biBitCount);
//	printf("\n\r λͼѹ������(0��ѹ��,1-BI_RLE8,2-BI_RLE4):%d",info_header.biCompression);
//	printf("\n\r λͼsize:%d,λͼʵ��ʹ����ɫ��:%d",info_header.biSizeImage,info_header.biClrUsed);
	IWDG_ReloadCounter();
//�������λͼ�ļ�����λͼ�ļ�������Ӧ���˳�-δ��	
// 	FSMC_NOR_ReadBuffer((u16 *)buf,BMP_LIST + BLOCK_SIZE*4, 2048/2);//��ȡbmp--������
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
			bmpBitCnt24(&info_header,bmp_head,bmp_no,file_header.bfOffBits,buf,yanse);//������λͼ��Ϣͷָ�룬λͼ���ݵ�ַ��������ָ�룬��ɫ
			break;
		default:break;
  }
	
}


void bmpBitCnt4 (BITMAPINFOHEADER *info_p,uint8_t bmp_head,uint8_t bmp_no,u32 data_addr,uint8_t *buf,uint8_t yanse)
{
  u32 biwidth1;//λͼ�洢���ݿ���ֽ���
	u32 rgbwidth;//RGB�洢���ݿ��λ��-�������ֽ�������
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
	sprintf((char*)file_name,"0:/bmp/%c0%d.bmp",bmp_head,bmp_no);   // �����ļ������洢
  else	
  sprintf((char*)file_name,"0:/bmp/%c%d.bmp",bmp_head,bmp_no);   // �����ļ������洢
	
	res = res;//��������
	*buf++ = info_p->biWidth;   //�洢���ؿ��
	*buf++ = info_p->biHeight;  //�洢���ظ߶�
	biwidth1 = (((info_p->biWidth*info_p->biBitCount)+31)/32)*4;//ÿһ��ɨ���ֽ���������4�ı���
  rgbwidth = ((info_p->biWidth+7)/8)*8;//RGB���ݲ���һ�ֽڣ��򲹳����
	for(j=0;j<rgbwidth/8*info_p->biHeight*yanse;j++) //����ͼƬ��С��������-�������ֽڲ���
    buf[j] = 0;
	p = memp_malloc(MEMP_PBUF_POOL);//����һ���ڴ�-���ã�1����ɫ��16*4��2��λͼ����
	while(p == NULL)
	{
		test++;
		if(test == 1)
			//printf("\n\r��ȡ�ڴ�ʧ�ܣ�bmpBitCnt4");
		OSTimeDly(5);
		p = memp_malloc(MEMP_PBUF_POOL);    //����һ���ڴ�����bmp����
	}
	OSSemPend(SDSem,0,&err);
  //1-����ɫ��
	if(info_p->biClrUsed == 0 || info_p->biClrUsed == 16)
	{
		
	  f_mount(0,&fs);
	
	  res = f_open(&fdst,(char*)file_name, FA_OPEN_EXISTING | FA_READ);
		
		res = f_lseek(&fdst,data_addr-64);
		
		res = f_read(&fdst,(u16 *)p, 64, &br);
		

		f_close(&fdst);/*�ر��ļ� */	
		f_mount(0,NULL);
//		FSMC_NOR_ReadBuffer((u16 *)p,data_addr-64,64/2);//16��RGBQUAD
		datap = p + 64;//��λλͼ����
	}
	else
	{
	  f_mount(0,&fs);		
	
	  res = f_open(&fdst,(char*)file_name, FA_OPEN_EXISTING | FA_READ);
		
		res = f_lseek(&fdst,data_addr-info_p->biClrUsed*4);
		
		res = f_read(&fdst,(u16 *)p, info_p->biClrUsed*4, &br);
		

		f_close(&fdst);/*�ر��ļ� */
    f_mount(0,NULL);		
//  FSMC_NOR_ReadBuffer((u16 *)p,data_addr-info_p->biClrUsed*4,info_p->biClrUsed*4/2);//info_p->biClrUsed��RGBQUAD
    datap = p + info_p->biClrUsed*4;//��λλͼ����
  }
	
	
	rgbquad_p = (RGBQUAD *)p;
	//2-��ȡλͼ����
	backup_datap = datap;
	for(i=info_p->biHeight;i>0;i--)//�߶�-��ΪBMP�Ǵ��µ��ϣ������ң������ȴ��½���
	{
	  datap = backup_datap;
		
	  f_mount(0,&fs);		
	
	  res = f_open(&fdst,(char*)file_name, FA_OPEN_EXISTING | FA_READ);
		
		res = f_lseek(&fdst,data_addr+(i-1)*biwidth1);
		
		res = f_read(&fdst,(u16 *)datap, biwidth1, &br);
		
		f_close(&fdst);/*�ر��ļ� */		
		f_mount(0,NULL);
		
//		FSMC_NOR_ReadBuffer((u16 *)datap, data_addr+(i-1)*biwidth1, biwidth1/2);//һ�ν���һ��
		for(j=0;j<rgbwidth/2;j++)//ÿһ�����ص�-4λ-����һ����ɫ����ӦLED��/��/����һλ(bit)
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
      datap++;   //bmp����ָ��datap��λ
    }				
	}
	OSSemPost(SDSem);
  memp_free(MEMP_PBUF_POOL,(void *)p);//�ͷ�POOL�ڴ�	                 

//	OSMemPut(MemPoint,p);	  //�ͷ��ڴ�
}





void bmpBitCnt8 (BITMAPINFOHEADER *info_p,uint8_t bmp_head,uint8_t bmp_no,u32 data_addr,u8 *buf,u8 yanse)
{
  u32 biwidth1;//λͼ�洢���ݿ���ֽ���
	u32 rgbwidth;//RGB�洢���ݿ��λ��-�������ֽ�������
	u32 i,j;
	u8 k,bcnt=0;
	u8 *p,test=0,*backup_datap,*datap;
	RGBQUAD *rgbquad_p;
	uint8_t err;
	int res;  
	FIL fdst;
  FATFS fs;
  UINT br=0;            // File R/W count
	res = res;//��������
	BYTE file_name[24];
	
  if(bmp_no<10)
	sprintf((char*)file_name,"0:/bmp/%c0%d.bmp",bmp_head,bmp_no);   // �����ļ������洢
  else	
  sprintf((char*)file_name,"0:/bmp/%c%d.bmp",bmp_head,bmp_no);   // �����ļ������洢
	
	*buf++ = info_p->biWidth;   //�洢���ؿ��
	*buf++ = info_p->biHeight;  //�洢���ظ߶�
	biwidth1 = (((info_p->biWidth*info_p->biBitCount)+31)/32)*4;//ÿһ��ɨ���ֽ���������4�ı���
	rgbwidth = ((info_p->biWidth+7)/8)*8;
	for(j=0;j<rgbwidth/8*info_p->biHeight*yanse;j++)//����ͼƬ��С��������-�������ֽڲ���
    buf[j] = 0;
	p = memp_malloc(MEMP_PBUF_POOL);//����һ���ڴ�-���ã�1����ɫ��256*4��2��λͼ����
	while(p == NULL)
	{
		test++;
		if(test == 1)
			//printf("\n\r��ȡ�ڴ�ʧ�ܣ�bmpBitCnt8");
		OSTimeDly(5);
		p = memp_malloc(MEMP_PBUF_POOL);     //����һ���ڴ�����bmp����
	}
  //1-����ɫ��
	OSSemPend(SDSem,0,&err);
	if(info_p->biClrUsed == 0 || info_p->biClrUsed == 256)
	{
	  f_mount(0,&fs);		
	
	  res = f_open(&fdst,(char*)file_name, FA_OPEN_EXISTING | FA_READ);
		
		res = f_lseek(&fdst,data_addr-1024);
		
		res = f_read(&fdst,(u16 *)p, 1024, &br);
		
		f_close(&fdst);/*�ر��ļ� */	
		f_mount(0,NULL);
		
//		FSMC_NOR_ReadBuffer((u16 *)p,data_addr-1024,1024/2);//256��RGBQUAD
		datap = p + 1024;//��λλͼ����
	}
	else
	{
	  f_mount(0,&fs);		
	
	  res = f_open(&fdst,(char*)file_name, FA_OPEN_EXISTING | FA_READ);
		
		res = f_lseek(&fdst,data_addr-info_p->biClrUsed*4);
		
		res = f_read(&fdst,(u16 *)p, info_p->biClrUsed*4, &br);
		
		f_close(&fdst);/*�ر��ļ� */	
		f_mount(0,NULL);
//    FSMC_NOR_ReadBuffer((u16 *)p,data_addr-info_p->biClrUsed*4,info_p->biClrUsed*4/2);//info_p->biClrUsed��RGBQUAD
    datap = p + info_p->biClrUsed*4;//��λλͼ����
  }
	rgbquad_p = (RGBQUAD *)p;
	//2-��ȡλͼ����
	backup_datap = datap;
	for(i=info_p->biHeight;i>0;i--)//�߶�-��ΪBMP�Ǵ��µ��ϣ������ң������ȴ��½���
	{
	  datap = backup_datap;
		
	  f_mount(0,&fs);		
	
	  res = f_open(&fdst,(char*)file_name, FA_OPEN_EXISTING | FA_READ);
		
		res = f_lseek(&fdst,data_addr+(i-1)*biwidth1);
		
		res = f_read(&fdst,(u16 *)datap, biwidth1, &br);
		
		f_close(&fdst);/*�ر��ļ� */	
		f_mount(0,NULL);
		
//		FSMC_NOR_ReadBuffer((u16 *)datap, data_addr+(i-1)*biwidth1, biwidth1/2);//һ�ν���һ��
		for(j=0;j<rgbwidth;j++)//ÿһ�����ص�-1���ֽ�-����һ����ɫ����ӦLED��/��/����һλ(bit)
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
			datap++;   //bmp����ָ��datap��λ
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
//	OSMemPut(MemPoint,p);	  //�ͷ��ڴ�
}



void bmpBitCnt24 (BITMAPINFOHEADER *info_p,uint8_t bmp_head,uint8_t bmp_no,u32 data_addr,u8 *buf,u8 yanse)//������λͼ���ݵ�ַ��������ָ��
{
  u32 biwidth1,lastdata;//λͼ�洢���ݿ���ֽ���
	u32 rgbwidth;//RGB�洢���ݿ��λ��-�������ֽ�������
	u32 i,j,bt;
	u8 k,bcnt=0,rt;
	u8 p[7000];//,test=0;
	uint8_t err;
	int res;  
	FIL fdst;
  FATFS fs;
  UINT br=0;            // File R/W count
	res = res;//��ֹ����
	BYTE file_name[24];
	if(bmp_no<10)
	sprintf((char*)file_name,"0:/bmp/%c0%d.bmp",bmp_head,bmp_no);   // �����ļ������洢
  else	
  sprintf((char*)file_name,"0:/bmp/%c%d.bmp",bmp_head,bmp_no);   // �����ļ������洢
	
	*buf++ = info_p->biWidth;   //�洢���ؿ��
	*buf++ = info_p->biHeight;  //�洢���ظ߶�
	biwidth1 = (((info_p->biWidth*info_p->biBitCount)+31)/32)*4;//ÿһ��ɨ���ֽ��� ������4�ı���
	rgbwidth = ((info_p->biWidth+7)/8)*8;//RGB�洢���ݿ��λ��-�������ֽ�������
	for(j=0;j<rgbwidth/8*info_p->biHeight*yanse;j++)//����ͼƬ��С��������-�������ֽڲ���
    buf[j] = 0;
//	p = memp_malloc(MEMP_PBUF_POOL);
//	//p = OSMemGet(MemPoint,&err);//����һ���ڴ�
//	while(p == NULL)
//	{
//		test++;
//		if(test == 1)
//			printf("\n\r��ȡ�ڴ�ʧ�ܣ�bmpBitCnt24");
//		OSTimeDly(5);
//		p = memp_malloc(MEMP_PBUF_POOL);
//		//p = OSMemGet(MemPoint,&err);     //����һ���ڴ�����bmp����
//	}
//	backupp = p;
	OSSemPend(SDSem,0,&err);
	for(i=info_p->biHeight;i>0;i--)//�߶�-��ΪBMP�Ǵ��µ��ϣ������ң������ȴ��½���
	{
  //  p = backupp;
		
//		//FSMC_NOR_ReadBuffer((u16 *)p, data_addr+(i-1)*biwidth1, biwidth1/2);//һ�ν���һ��
		if(biwidth1<510)
		{
			f_mount(0,&fs);		
			
			res = f_open(&fdst,(char*)file_name, FA_OPEN_EXISTING | FA_READ);
			
			res = f_lseek(&fdst,data_addr+(i-1)*biwidth1);
		
			res = f_read(&fdst,p, biwidth1, &br);
			

			f_close(&fdst);/*�ر��ļ� */
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
			
			f_close(&fdst);/*�ر��ļ� */
			f_mount(0, NULL);	 
		 }
		if(lastdata!=0)
		 {
			f_mount(0,&fs);		
			
			res = f_open(&fdst,(char*)file_name, FA_OPEN_EXISTING | FA_READ);
			
			res = f_lseek(&fdst,data_addr+(i-1)*biwidth1+bt*510);
		
			res = f_read(&fdst,p+bt*510, lastdata, &br);
			
			f_close(&fdst);/*�ر��ļ� */
			f_mount(0, NULL);	 
		 }
			
		}

		for(j=0,bt=0;j<rgbwidth;j++)//ÿһ�����ص�-�����ֽڣ���ӦLED��/��/���һλ(bit)
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
			bt += 3;   //bmpָ��p��λ
    }
		
	for(bt=0;bt<7000;bt++)
	p[bt]=0;
  }
	OSSemPost(SDSem);
 // memp_free(MEMP_PBUF_POOL,(void *)p);
	//OSMemPut(MemPoint,backupp);	  //�ͷ��ڴ�
}

/*****************************����Ϊ�Ľ��汾*****2019.12********************************************/
void BMP24RGBToItem(char* file_name,BITMAPINFOHEADER info_header,BITMAPFILEHEADER file_header,uint16_t posx,uint16_t posy)
{
	uint8_t err;
	int res;  
	FIL fdst;
	FATFS fs;
	UINT br=0;            // File R/W count
	u32 biwidth1;//λͼ�洢���ݿ���ֽ���
	uint16_t i,j,k;
	uint16_t posx_temp;
	uint8_t *p[3],bgr[3];
	uint16_t biWidth,biHeight;
	
	posx_temp = posx;
	
	biWidth = info_header.biWidth;   //�洢���ؿ��
	biHeight= info_header.biHeight;  //�洢���ظ߶�
	
	biwidth1 = (((info_header.biWidth*info_header.biBitCount)+31)/32)*4;//ÿһ��ɨ���ֽ��� ������4�ı���
	
	OSSemPend(SDSem,0,&err);
	f_mount(0,&fs);

	for(i=0;i<biHeight;i++)
	{
		if(posy >= SCREEN_LIE)	break;		//�߽紦��
		
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
			f_close(&fdst);/*�ر��ļ� */
			f_mount(0,NULL);
			OSSemPost(SDSem);
			return;
		}
		
		posx = posx_temp;

		for(j=0;j<biWidth;j++)
		{
			if(posx >= SCREEN_HANG)	break;//�߽紦��
			
			p[2] = (posx>>3)+ItemBuffer.rgb_buf[0]+posy*SCREEN_HBYTE;//���ֽڶ�λ	//R
			p[1] = (posx>>3)+ItemBuffer.rgb_buf[1]+posy*SCREEN_HBYTE;//���ֽڶ�λ	//G
			p[0] = (posx>>3)+ItemBuffer.rgb_buf[2]+posy*SCREEN_HBYTE;//���ֽڶ�λ	//B
			
			res = f_read(&fdst,bgr,3,&br);
			if(res!=FR_OK)
			{
				f_close(&fdst);/*�ر��ļ� */
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
	u32 biwidth1;//λͼ�洢���ݿ���ֽ���
	uint16_t i,j;
	uint16_t posx_temp;
	uint8_t *p[3],rgb,*pot;
	RGBQUAD *rgbquad_p;
	uint16_t biWidth,biHeight;//,bitcnt=0;
	u32 bmpPosByte;
	uint8_t byte_H,byte_L;
	
	pot = memp_malloc(MEMP_PBUF_POOL);       //����һ���ڴ�����BMP���ݽ���--��ɫ��
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

		biWidth = info_header.biWidth/2;   //�洢���ؿ��
	}else if(info_header.biBitCount == 8)
	{
		if(info_header.biClrUsed == 0 || info_header.biClrUsed == 256)
			bmpPosByte = 256*4;
		else
			bmpPosByte = info_header.biClrUsed*4;
		
		biWidth = info_header.biWidth;   //�洢���ؿ��
	}

	biHeight= info_header.biHeight;  //�洢���ظ߶�
		
	posx_temp = posx;
	
	biwidth1 = (((info_header.biWidth*info_header.biBitCount)+31)/32)*4;//ÿһ��ɨ���ֽ��� ������4�ı���
	
	OSSemPend(SDSem,0,&err);
	f_mount(0,&fs);
	//1-����ɫ��
	res = f_open(&fdst,(char*)file_name, FA_OPEN_EXISTING | FA_READ);
	if(res!=FR_OK)
	{
		f_mount(0,NULL);
    memp_free(MEMP_PBUF_POOL,(void *)pot);//�ͷ�POOL�ڴ�	                 

//		OSMemPut(MemPoint,pot);
		OSSemPost(SDSem);
		return;
	}
	res = f_lseek(&fdst,file_header.bfOffBits-bmpPosByte);
	if(res!=FR_OK)
	{
		f_close(&fdst);
		f_mount(0,NULL);
    memp_free(MEMP_PBUF_POOL,(void *)pot);//�ͷ�POOL�ڴ�	                 
    
//		OSMemPut(MemPoint,pot);
		OSSemPost(SDSem);
		return;
	}
	
	res = f_read(&fdst,pot,bmpPosByte,&br);
	if(res!=FR_OK)
	{
		f_close(&fdst);/*�ر��ļ� */
		f_mount(0,NULL);
    memp_free(MEMP_PBUF_POOL,(void *)pot);//�ͷ�POOL�ڴ�	                   
//		OSMemPut(MemPoint,pot);	  //�ͷ��ڴ�
		OSSemPost(SDSem);
		return;
	}
	
	rgbquad_p = (RGBQUAD *)pot;
	
	for(i=0;i<biHeight;i++)
	{
		if(posy >= SCREEN_LIE)	break;		//�߽紦��
		
		res = f_open(&fdst,(char*)file_name, FA_OPEN_EXISTING | FA_READ);
		if(res!=FR_OK)
		{
			f_mount(0,NULL);
      memp_free(MEMP_PBUF_POOL,(void *)pot);//�ͷ�POOL�ڴ�	                 
//			OSMemPut(MemPoint,pot);	  //�ͷ��ڴ�
			OSSemPost(SDSem);
			return;
		}
		
		res = f_lseek(&fdst,file_header.bfOffBits+(biHeight-i-1)*biwidth1);
		if(res!=FR_OK)
		{
			f_close(&fdst);/*�ر��ļ� */
			f_mount(0,NULL);
      memp_free(MEMP_PBUF_POOL,(void *)pot);//�ͷ�POOL�ڴ�	                 
//			OSMemPut(MemPoint,pot);	  //�ͷ��ڴ�
			OSSemPost(SDSem);
			return;
		}
		
		posx = posx_temp;

		for(j=0;j<biWidth;j++)
		{
			if(posx >= SCREEN_HANG)	break;//�߽紦��
			
			p[0] = (posx>>3)+ItemBuffer.rgb_buf[0]+posy*SCREEN_HBYTE;//���ֽڶ�λ	//R
			p[1] = (posx>>3)+ItemBuffer.rgb_buf[1]+posy*SCREEN_HBYTE;//���ֽڶ�λ	//G
			p[2] = (posx>>3)+ItemBuffer.rgb_buf[2]+posy*SCREEN_HBYTE;//���ֽڶ�λ	//B
			
			res = f_read(&fdst,&rgb,1,&br);
			if(res!=FR_OK)
			{
				f_close(&fdst);/*�ر��ļ� */
				f_mount(0,NULL);
        memp_free(MEMP_PBUF_POOL,(void *)pot);//�ͷ�POOL�ڴ�	                 
//				OSMemPut(MemPoint,pot);	  //�ͷ��ڴ�
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
				if(posx >= SCREEN_HANG)	break;//�߽紦��
				p[0] = (posx>>3)+ItemBuffer.rgb_buf[0]+posy*SCREEN_HBYTE;//���ֽڶ�λ	//R
				p[1] = (posx>>3)+ItemBuffer.rgb_buf[1]+posy*SCREEN_HBYTE;//���ֽڶ�λ	//G
				p[2] = (posx>>3)+ItemBuffer.rgb_buf[2]+posy*SCREEN_HBYTE;//���ֽڶ�λ	//B
				
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
  memp_free(MEMP_PBUF_POOL,(void *)pot);//�ͷ�POOL�ڴ�	                 
//	OSMemPut(MemPoint,pot);	  //�ͷ��ڴ�
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
		f_close(&fdst);/*�ر��ļ� */
		f_mount(0,NULL);
		OSSemPost(SDSem);
		return 1;
	}
	
	res = f_read(&fdst,(u16 *)info_header, sizeof(BITMAPINFOHEADER), &br);
	if(res!=FR_OK)
	{
		f_close(&fdst);/*�ر��ļ� */
		f_mount(0,NULL);
		OSSemPost(SDSem);
		return 1;
	}
	f_close(&fdst);/*�ر��ļ� */
	f_mount(0,NULL);
	OSSemPost(SDSem);
	return 0;
}
