/**
  ******************************************************************************
  * �ļ��� : sw_controlcenter.c
  * ��  �� : �¹�
  * ��  �� ��V1.0
  * ʱ  �� : 2014��5��11��
  * ��  ˾ ��������缼�����޹�˾
  * ժ  Ҫ �����ļ����������������ͨ��ģ��ĺ����ӿ�
	*
  ******************************************************************************
  */
#include "stm32f4xx.h"
#include "stm32f4xx_rtc.h"
#include "sw_controlcenter.h"
#include "includes.h"
#include "sw_global.h"
#include  "sw_crc16.h"
#include "wdg.h"
#include "24cxx.h"
#include "user_tim3_pwm.h"
#include "usart.h"
#include "ds18b20.h"
#include "rtc.h"
#include "i2c.h"
#include "malloc.h"
#include "lwip_comm.h"
#include "cJSON.h"
uint8_t VERSION[] = "V1.02_F407";

uint8_t  ItemTab[] = {'I','T','E','M'};//4
uint8_t  ItemNoTab[] = {'I','T','E','M','_','N','O','='};//8

uint8_t TempSt;    //�¶ȹ���״̬��
uint8_t LdrSt;     //��������״̬��
uint8_t DoorSt;    //���Ź���״̬��
uint8_t DoorStflag=0;
uint16_t DoorStflag1[20];
uint8_t rgb_buftest[4096];
uint16_t SCREEN_HANG_FH;

void StrListInit (StrList * p)
{
  p->str_type = 0; 	   //�ַ�����  BMP_TYPE   STR_TYPE
  p->posx = 0;         //�ַ�λ��x
  p->posy = 0;         //�ַ�λ��y

  p->front_type = 0;   //��������-���Ρ����塢���塢
  p->front_size = 0;   //�����С-16��24��32��48
  p->space = 0;        //ȱʡʱ�ַ����0
  p->colr = 255; 	     //�ַ���ɫ-��
  p->colg = 255; 	     //�ַ���ɫ-��
  p->colb = 255; 	       //�ַ���ɫ-��
  p->colh = 0;         //�ַ���ɫ-����
  p->bmp_num = NULL;
}


void Control_AckOK(uint8_t * p,uint16_t *len,char *id)
{
	cJSON* cjson_test=NULL;
	
	cjson_test=cJSON_CreateObject();
	
	cJSON_AddStringToObject(cjson_test,"id",id);//����ַ����͵�
	cJSON_AddStringToObject(cjson_test,"status","200");//����ַ����͵�
	cJSON_AddStringToObject(cjson_test,"msg"," ");//����ַ����͵�
	
	char *cjson_data=cJSON_PrintUnformatted(cjson_test);
	memcpy(p,cjson_data,strlen(cjson_data));
	*len=strlen(cjson_data);
	cJSON_Delete(cjson_test);
}

void Control_AckERR(uint8_t * p,uint16_t *len,char *id)
{
	cJSON* cjson_test=NULL;
	
	cjson_test=cJSON_CreateObject();
	
	cJSON_AddStringToObject(cjson_test,"id",id);//����ַ����͵�
	cJSON_AddStringToObject(cjson_test,"status","100");//����ַ����͵�
	cJSON_AddStringToObject(cjson_test,"msg"," ");//����ַ����͵�
	
	char *cjson_data=cJSON_PrintUnformatted(cjson_test);
	memcpy(p,cjson_data,strlen(cjson_data));
	*len=strlen(cjson_data);
	cJSON_Delete(cjson_test);
}


//�ļ������ж�BMP/PLAY.LIST
uint8_t FileTypeJudgment (uint8_t *buffer)
{
	if(*(buffer) == 'b'||*(buffer) == 'B' )//Сд/��д-bmp/bxx.bmp+
  {
    if((*(buffer+1) == 'm'||*(buffer+1) == 'M')&&
			 (*(buffer+2) == 'p'||*(buffer+2) == 'P')&&
		   (*(buffer+3) == 92)&&
		   (*(buffer+7) == '.')&&(*(buffer+8) == 'b'||*(buffer+8) == 'B')
	     &&(*(buffer+9) == 'm'||*(buffer+9) == 'M')&&
		   (*(buffer+10) == 'p'||*(buffer+10) == 'P')&&
		   (*(buffer+11) == '+'))
		return 1;
	}
	else if(*(buffer) == 'p'||*(buffer) == 'P' )	//Сд/��д-play.lst+
  {
    if((*(buffer+1) == 'l'||*(buffer+1) == 'L')&&
			 (*(buffer+2) == 'a'||*(buffer+2) == 'A')&&
		   (*(buffer+3) == 'y'||*(buffer+3) == 'Y')&&
		   (*(buffer+4) == '.'||*(buffer+4) == 'L'||*(buffer+4) == 'l')&&
		   (*(buffer+5) == 'l'||*(buffer+5) == 'L'||*(buffer+5) == 'i'||*(buffer+5) == 'I')&&//Ԥ����Ϣ Ϊ��PLAYLIST
		   (*(buffer+6) == 's'||*(buffer+6) == 'S')&&
		   (*(buffer+7) == 't'||*(buffer+7) == 'T')&&
		   (*(buffer+8) == '+'||*(buffer+8) == '\\'))
		return 2;
	}
	return 0;
}



uint16_t InDex (uint8_t *S,uint16_t Snum,uint8_t *T,uint8_t Tnum,uint16_t pos)
{
  uint16_t i=pos,j=0;
  while(i<Snum && j<Tnum)
  {
    if(*(S+i)==*(T+j) || *(S+i) == *(T+j)+32)
	{i++;j++;}
	else {i=i-j+1;j=0;}
  }
  if(j == Tnum) return i; //�ҵ���ͬ�ַ�������һ��λ��
  else return 0;
}


uint16_t ReadNData(uint8_t *buffer,uint8_t Rnum)   //��ȡ��ʼ��ַ��Ҫ��ȡ���ݵ����λ��
{
  uint8_t temp=0;
  uint8_t i;
  uint16_t ren;
	
  for(i=0;i<Rnum;i++)                   //Rnum=3 ֧��Item0~999
  {
    if(*(buffer+i)>47 && *(buffer+i)<58)//����0~9֮��
			temp++;
	else
	  break;
  }
  switch(temp)
  {
    case 0: /* printf("qing ping"); */
	  return 0xffff;
    case 1: /* printf("item_no = %c",*(buffer+addr_num)); */
      ren = *buffer-48;
	  return ren;		  
    case 2: /* printf("item_no = %c%c",*(buffer+addr_num),*(buffer+addr_num+1)); */
      ren = (*buffer-48)*10+*(buffer+1)-48;
	  return ren;	
    case 3: /* printf("item_no = %c%c%c",*(buffer+addr_num),*(buffer+addr_num+1),*(buffer+addr_num+2)); */
      ren = (*buffer-48)*100+(*(buffer+1)-48)*10+*(buffer+2)-48;
	  return ren;
    case 4: /* printf("item_no = %c%c%c",*(buffer+addr_num),*(buffer+addr_num+1),*(buffer+addr_num+2)); */
      ren = (*buffer-48)*1000+(*(buffer+1)-48)*100+(*(buffer+2)-48)*10+*(buffer+3)-48;
	  return ren;
    case 5: /* printf("item_no = %c%c%c",*(buffer+addr_num),*(buffer+addr_num+1),*(buffer+addr_num+2)); */
      ren = (*buffer-48)*10000+(*(buffer+1)-48)*1000+(*(buffer+2)-48)*100+(*(buffer+3)-48)*10+*(buffer+4)-48;
	  return ren;
    default : break;		  
  }
  return 0xffff;
}

int16_t ReadNData_S(uint8_t *buffer,uint8_t Rnum)   //��ȡ��ʼ��ַ��Ҫ��ȡ���ݵ����λ��
{
  uint8_t temp=0;
  uint8_t flag = 0;
  uint8_t *buffer_temp = buffer;
  uint8_t i;
  int16_t ren=0;
	
  for(i=0;i<Rnum;i++)                   //Rnum=3 ֧��Item0~999
  {
    if((i == 0) && (*buffer == '-'))
    {
      flag = 1;
      buffer++;
      continue;
    }
    if(*(buffer_temp+i)>='0' && *(buffer_temp+i)<='9')//����0~9֮��
			temp++;
    else
      break;
  }

  switch(temp)
  {
    case 0: /* printf("qing ping"); */
      return 0;
    case 1: /* printf("item_no = %c",*(buffer+addr_num)); */
      ren = *buffer-48;
      break;
    case 2: /* printf("item_no = %c%c",*(buffer+addr_num),*(buffer+addr_num+1)); */
      ren = (*buffer-48)*10+*(buffer+1)-48;
      break;
    case 3: /* printf("item_no = %c%c%c",*(buffer+addr_num),*(buffer+addr_num+1),*(buffer+addr_num+2)); */
      ren = (*buffer-48)*100+(*(buffer+1)-48)*10+*(buffer+2)-48;
      break;
    case 4: /* printf("item_no = %c%c%c",*(buffer+addr_num),*(buffer+addr_num+1),*(buffer+addr_num+2)); */
      ren = (*buffer-48)*1000+(*(buffer+1)-48)*100+(*(buffer+2)-48)*10+*(buffer+3)-48;
      break;
    case 5: /* printf("item_no = %c%c%c",*(buffer+addr_num),*(buffer+addr_num+1),*(buffer+addr_num+2)); */
      ren = (*buffer-48)*10000+(*(buffer+1)-48)*1000+(*(buffer+2)-48)*100+(*(buffer+3)-48)*10+*(buffer+4)-48;
      break;
    default : break;		  
  }
  
  if(flag)
  {
    ren = 0 - ren;
  }
  
  return ren;
}
////��ȡ�ܹ��ж���֡����
uint16_t ReadItemNo (uint8_t *p,uint16_t num)//����ָ�룬���ݸ���
{
  uint16_t item_no = 0;
	uint16_t addr;//,k=0;
	
	addr = InDex(p,num,ItemNoTab,8,0);
	
	if(addr != 0)
	{
	  item_no = ReadNData(p+addr,3);//��ȡ�����������ֵ
	  if(item_no == 0xffff)
		  return 0;
	}
	else 
		return 0;
	return item_no;
}

uint8_t MoveNBit (uint16_t temp)
{
  if(temp < 10)
    return 1;
  else if(temp < 100)
    return 2;
  else if(temp < 1000)
    return 3;
  else if(temp < 10000)
    return 4;
  else if(temp < 100000)
    return 5;
	else
		return 0;
}



//��ȡһ֡���ݷŵ�ItemBuffer��--���ض�ȡ֡β����һλƫ����
uint16_t ReadOneItem (uint8_t *p,uint16_t num)
{
	uint8_t *pot;
	uint16_t addr,temp;
//	uint8_t k=0;
	pot = p;
	//��λһ��ITEM
	addr = InDex(p,num,ItemTab,4,0);//��λITEM��һλ,SD����ΪСд
	p += addr;
	//��λ����ITEM0=0,22,0��0��ָ��
	
	if(addr != 0)
	{
		temp = ReadNData(p,3);//��ȡ֡���� �ڼ�֡:Item��
	//	printf("fr no %d\n\r",temp);
		if(temp != 0xffff)
		{
			ItemBuffer.itemlist.itemyyy = temp;//���������0-500--�ڼ�������
			p +=MoveNBit(temp);
			p++;   //ȥ��'='��,ָ��'='����һλ
			temp = ReadNData(p,5);//��ȡͣ��ʱ�� (2-30000)*10ms ����100ת��ΪS
			//printf("delay time %d\n\r",temp);
			if(temp != 0xffff)
			{
				ItemBuffer.itemlist.delay = temp/100;
				p +=MoveNBit(temp);
			}
			else
			{
				return 0;
			}
			p++; //ȥ��','��,ָ��','����һλ
			while(*p==0x0D||*p==0x0A||*p==0x20)//�����ո�
			{
////				printf("p++\n\r");
				p++;
			}
			temp = ReadNData(p,2);//��ȡ��ʾ��ʽ
		//	printf("dismod %d\n\r",temp);
			if(temp != 0xffff)
			{
				if(temp<10)
					ItemBuffer.itemlist.mod = temp*10+temp;
				else
					ItemBuffer.itemlist.mod = temp;
				p +=MoveNBit(temp);
			}
			else
			{
				return 0;
			}
			p++;//ȥ��'='��,ָ��'='����һλ
			while(*p==0x0D||*p==0x0A||*p==0x20)//�����ո�
				p++;
			temp = ReadNData(p,2);//��ȡ�ƶ��ٶ�
			//printf("speed %d\n\r",temp);
			if(temp != 0xffff)
			{
				ItemBuffer.itemlist.param = temp;
				p +=MoveNBit(temp);
			}
			else
			{
				return 0;
			}
			p++;	//ȥ��','��,ָ��','����һλ	
			while(*p==0x0D||*p==0x0A||*p==0x20)//�����ո�
			{
////				printf("p++\n\r");
				p++;
			}	
			//printf("index %d\n\r",addr);
			//��λ���� ���Ų����ĵ�һ�� \ \fs3232��f��ָ��
			//for(k=0;k<3;k++)
			//printf("%c",p[k]);
		}
		else
		{
				return 0;
		}
// 		else
// 			ReadOneItem(p,num-addr);
	}
	else
	{
		return 0;
	}
	//printf("\n\r ����֡ 1 �ף�");
	//��ʼ���� ���Ų��� L-(0-0,22,0)
	p = ReadNString(p,(num-(p-pot)));//num-(p-pot)ָ���Ǵ�ʱpָ���Ժ�����ݵĴ�С
	if(p==NULL)
	{
		return 0;
	}
	else
	{
		//printf("p-pot is %d\n\r",p-pot);
		return (p-pot);//����֡�Ĵ�С
	}
}


////��һ֡�����е��ַ����ݲ��ڻ��������������
// void ReadNString(uint8_t *buffer,uint16_t addr_num)  //������ʼ��ַ��������ʼ��ַƫ��
uint8_t * ReadNString (uint8_t *p,uint16_t num)
{
  uint16_t i,j; 
  uint8_t *pot;
  uint8_t buf[1024]; //�ַ�������--�����������С��Ҫע�� ����ʵ����Ҫ����****************************************
  StrList strlist;	 //��ǰ�༭���ַ���
  pot = p;
//  printf("num is %d\n\r",num);
  for(i=0;i<1024;i++)
  {
    buf[i]=0; 
  }
  StrListInit(&strlist);    //��ʼ����ǰ�༭�ַ���
//	////////////////////////////////////////////////////
//		while(*p==0x0D||*p==0x0A||*p==0x20)//�����ո�
//		 p++;
//	////////////////////////////////////////////////////		
  while(*p != 0x0D && *p != 0x0A)//ȡҪ��ʾ��ͼƬ/���ָ�ʽ/ASCII 0x0D��0x0A�ǿո���ַ�
  {
		if((!memcmp(p,"ITEM",4))||(!memcmp(p,"item",4))||(!memcmp(p,"Item",4)))
			break;
		if(p-pot>=num)
			break;
		//printf("ReadNString \n\r");
		if(*p == 92) //92 ASCII ��'\'
		{
			p++;
			switch(*p)
			{
				case 'C':  //�ַ���ʾλ��
					p++;     //��λ������
					strlist.posx = ReadNData(p,3);
					p += 3;
					strlist.posy = ReadNData(p,3);
					p += 3;
//          printf("C\n\r");
					break;
				case 'B':  //bmp
					strlist.str_type = BMP_TYPE;
				  p ++;                                //��ַ�洢bmp�ļ�ͷ��ĸ
				  strlist.bmp_head = *p;
					p ++;   //��λ�������ַ�
					strlist.bmp_num = ReadNData(p,2);	   //��ַ�洢bmp�ļ���ͼ������
					p += 2;
					TC_RGBdata(&strlist,0,0); //���RGB����-��һ��������Ч
					StrListInit(&strlist);    //��ʼ����ǰ�༭�ַ���
////					printf("B\n\r");
					break;
				case 'f':  //����
					p++;   //��λ�������ַ�
					strlist.front_type = *p;
					p++;   //��λ�������ַ�
					strlist.front_size = ReadNData(p,2);
					p += 4;
//				 printf("f\n\r");
					break;
				case 'S':  //������
					p++;   //��λ�������ַ�
					strlist.space = ReadNData_S(p,2);
					p += 2;
////					printf("S\n\r");
					break;
				case 'c':  //�ַ���ɫ
					strlist.str_type = STR_TYPE;
					p++;   //��λ�������ַ�
				if(*p=='t')p++;//    \ct ͸��ɫ���ޣ�
				else
					{
            strlist.colr = ReadNData(p,3); //��
            p += 3;
            strlist.colg = ReadNData(p,3); //��
            p += 3;
            strlist.colb = ReadNData(p,3); //��
            p += 3;
            strlist.colh = ReadNData(p,3); //����
            p += 3;
//						printf("color\n\r");
					}
//          printf("\r\n r=%d,g=%d,b=%d",strlist.colr,strlist.colg,strlist.colb);
				  break;
				case 's':  //�ַ���Ӱ��ɫ�����ޣ�
					p++;   //��λ�������ַ�
					if(*p=='t')
						p++;// \bt ͸��ɫ���ޣ�
					else
						p += 12;
					break;
				case 'y':  //�����ޣ�
					p++;   //��λ�������ַ�
					//strlist.space = ReadNData(p,2);
					p += 1;
					break;	
				case 'N':  //��˸���������ޣ�
					p++;   //��λ�������ַ�
					//strlist.space = ReadNData(p,2);
					p += 2;
					break;
				case 'r':  //��˸�������꣨���ޣ�
					p++;   //��λ�������ַ�
					//strlist.space = ReadNData(p,2);
					p += 12;
					break;					
				case 'b':  //�ַ�������ɫ�����ޣ�
					p++;   //��λ�������ַ�
				if(*p=='t')p++;// \bt ͸��ɫ���ޣ�
					else
					p += 12;
					break;
				default : break;
			}
	  }else
		{
						//printf("������\n\r");	
//			    	while(*p == 0x20)//�����ո�
//		            p++;if()
//						break;
			    i = 0;
					while((*p != 0x0D && *p != 0x0A))//ȡҪ��ʾ��ͼƬ/���ָ�ʽ ֻҪ��Ϊ�ո���ִ��ѭ��
					{
						if((!memcmp(p,"ITEM",4))||(!memcmp(p,"item",4))\
					||(!memcmp(p,"Item",4)))
						{
////							printf("break\n\r");
						break;
						}
						//printf("p-pot %d\n\r",p-pot);
						if((p-pot>=num))
							break;
						
//						IWDG_ReloadCounter();
//				    printf("%c",*(buffer+*addr_num));
						buf[i++] = *p++;  //��ȡ�ַ� 
						if(*p == 92 && (*(p+1) == 'f'             //92���ַ�'\'
							 || *(p+1) == 'B' || *(p+1) == 'C' 
							 || *(p+1) == 'S' || *(p+1) == 'c'))//�ַ�'\'
						{
							break;
						}
					}
					TC_RGBdata(&strlist,buf,i);//���RGB����
					StrListInit(&strlist);     //��ʼ����ǰ�༭�ַ���
          
					for(j=0;j<i;j++)           //����ʹ�ù����ڴ�
					{
//            printf("data  %x\n\r",buf[j]);
						buf[j]=0; 
					}
//					printf("data size is %d\n\r",i);
		}
  }
	return p;
}




//��ȡ��ģ
void TC_StrData (StrList *strlist,uint8_t *buf,uint8_t num,uint8_t *rgb_p)
{
  uint8_t *p,k;
	uint8_t hz_buf[1024];//!!!!!!!!!!!!!!!--�����������С��Ҫע��
	uint8_t qm,wm,err;
	uint8_t space_bit = 0;  //���λ��
	uint8_t flag24 = 0;
	uint8_t hbtye = 0;
  
	uint16_t i,j;
	uint16_t posx,posy;
	uint16_t xcnt,ycnt;
  uint16_t hz_byte;//,bt;//һ�����ֶ����ֽ�
	uint16_t asc_byte;//һ���ַ������ֽ�
//  u32 flash_hzk_addr,flash_asc_addr;
	uint32_t addr;
	
	int res;  
	FIL fdst_hanzi,fdst_zifu;
	FATFS fs;
	UINT br=0;            // File R/W count
	
	BYTE file_name[24];
  
  uint8_t daxiao=0;
  uint8_t asc_byte1=0;
	//flash_asc_addr = GetASCAddr(strlist->front_type,strlist->front_size);
	//*************************************************************************************
	//����20�ֺŴ�С
	if(strlist->front_size==20)
	{hz_byte = (24)*(24>>3); 
	daxiao=20;
	}
	else if(strlist->front_size==18)
	{hz_byte = (24)*(24>>3); 
	daxiao=18;
	}
		else
	hz_byte = (strlist->front_size)*(strlist->front_size>>3);    //һ�������ֽ���
	
	if(strlist->front_size == 24||strlist->front_size == 20)//24*16-���⴦��
	{
		flag24 = 1;
		asc_byte = 0x30;   //һ���ַ��ֽ���
	}
	else
	asc_byte = (strlist->front_size)*(strlist->front_size>>4);   //һ���ַ��ֽ���
	posx = strlist->posx;  //�ַ�λ��x y
	posy = strlist->posy;
	//*****************************************************************************************
	
	OSSemPend(SDSem,0,&err);
	f_mount(0,&fs);	
	if(strlist->front_type=='h'||strlist->front_type=='k'||strlist->front_type=='s'||strlist->front_type=='f'||strlist->front_type=='l')
		sprintf((char*)file_name,"0:/font/HZK%d%c.bin",strlist->front_size,strlist->front_type);   // �������ֿ��ļ���
	else
		sprintf((char*)file_name,"0:/font/HZK%ds.bin",strlist->front_size);   // �������ֿ��ļ���	
	
	res = f_open(&fdst_hanzi,(char*)file_name, FA_OPEN_EXISTING | FA_READ);	
if(res!=FR_OK)
{
	f_mount(0,NULL);
	OSSemPost(SDSem);
	return;
}
	sprintf((char*)file_name,"0:/font/ASC%d.bin",strlist->front_size);   // ����ASC�ļ���				
	res = f_open(&fdst_zifu,(char*)file_name, FA_OPEN_EXISTING | FA_READ);		
if(res!=FR_OK)
{
	f_mount(0,NULL);
	OSSemPost(SDSem);
	return;
}	
//*********************************************************************
	//����20�ֺŴ�С
if(strlist->front_size==20)
	strlist->front_size=24;
//**************************************************************
	for(i=0;i<num;i++)
	{
		if(buf[i]>160)   //����
		{
			xcnt = posx;
			ycnt = posy;
			if(daxiao==20||daxiao==18)
			{
			qm = buf[i]-0xA1;   //����
			wm = buf[++i]-0xA1; //λ��
				
			addr=(qm*94+wm)*hz_byte;
			}
			else
				{	
			qm = buf[i]-160;   //����
			wm = buf[++i]-160; //λ��
			
			//�ҵ��ֿ��к��ֵĵ�ַ
			addr = (94*(qm-1)+wm-1)*hz_byte;     //�����ں��ֿ���λ��
			
			//��ȡ���ֵ���-���ֿ��к��ֵ�����16λ���ݴ洢
			 
			}
			res = f_lseek(&fdst_hanzi,addr);		
			res = f_read(&fdst_hanzi,(u16 *)hz_buf, hz_byte, &br);

			hbtye = (strlist->front_size>>3);
			for(j=0;j<hz_byte;j +=hbtye)	     //����һ��������
			{
				xcnt = posx;                   //X���������ʼ��
				for(k=0;k<hbtye ;k++)          //һ�ж����ֽ�
				{
					if(xcnt >= SCREEN_HANG || ycnt >= SCREEN_LIE)   //�߽�����
					{
						break;
					}
					p = rgb_p+ycnt*SCREEN_HBYTE+(xcnt>>3);          //���ֽڶ�λ
					space_bit = xcnt%8;
					if(space_bit == 0)                              //����Ҫ��λ
					  *p = hz_buf[j+k];
					else                                            //��Ҫ��λ
					{
						*p = *p|(hz_buf[j+k]>>space_bit);     //ȡ��8-space_bitλ
						if(xcnt+8-space_bit >= SCREEN_HANG)           //�߽�����
						{
							break;
						}
						*(p+1) = *(p+1)|(hz_buf[j+k]<<(8-space_bit)); //ȡ��space_bitλ
					}
					xcnt += 8;				
				}
			  ycnt++;				
			}
			posx += strlist->front_size+strlist->space;
		}
		 else
		{
			xcnt = posx;
			ycnt = posy;	
			if(buf[i+1] == 'n')
			{
				posx = strlist->posx;
				posy +=  strlist->front_size;
				i++;
				continue;
			}
			else
				if(daxiao==20)
				{asc_byte1=48;
				addr = (buf[i]-0x20)*asc_byte1; 
				}
			else if(buf[i] == 92)
			{	
				if(daxiao==20)
				{asc_byte1=48;
				addr = (buf[i]-0x20)*asc_byte1; 
				}
				else
				addr = 92*asc_byte;                //�ַ�'\'���ֿ���λ�� 
			}
			else
				if(daxiao==20)
				{asc_byte1=48;
				addr = (buf[i]-0x20)*asc_byte1; 
				}
				else
				addr = buf[i]*asc_byte;      	

			res = f_lseek(&fdst_zifu,addr);		
			res = f_read(&fdst_zifu,(u16 *)hz_buf, asc_byte, &br);			
			if(flag24 == 1 )
				hbtye = (strlist->front_size>>4)+1;
			else
				hbtye = (strlist->front_size>>4);

			for(j=0;j<asc_byte;j +=hbtye)	     //�� 
			{
				xcnt = posx;
				for(k=0;k<hbtye;k++)           //һ�ж����ֽ�
				{
					if(xcnt >= SCREEN_HANG || ycnt >= SCREEN_LIE)   //�߽�����
					{
							break;
					}
					p = rgb_p+ycnt*SCREEN_HBYTE+(xcnt>>3);          //���ֽڶ�λ
					space_bit = xcnt%8;
					if(space_bit == 0)                              //����Ҫ��λ
						*p = hz_buf[j+k];
					else                                            //��Ҫ��λ
					{
						*p = *p|(hz_buf[j+k]>>space_bit);     //ȡ��8-space_bitλ
						if(xcnt+8-space_bit >= SCREEN_HANG)           //�߽�����
						{
							break;
						}
						*(p+1) = *(p+1)|(hz_buf[j+k]<<(8-space_bit)); //ȡ��space_bitλ
					}
					xcnt += 8;
					IWDG_ReloadCounter();	
					}
					ycnt++;
				}
			posx +=strlist->front_size/2 + strlist->space;
		}
	}
	f_close(&fdst_hanzi);/*�ر��ļ� */
	f_close(&fdst_zifu);
	f_mount(0,NULL);
	OSSemPost(SDSem);
}


////���RGB����-�ַ���ʽ���ַ����������ַ���Ŀ
void TC_RGBdata (StrList *strlist,uint8_t *buf,uint16_t num)
{
//	uint8_t test;
//	uint16_t i,j,k=0;
//	uint8_t *p,err;
	switch(strlist->str_type)
  {
    case BMP_TYPE:		 //λͼ
    TC_BmpItemOnebyOne(strlist);
	    break;
	  case STR_TYPE:		 //�ַ�
      
	    if(strlist->colr > 0)  //����ɫ
	    {
        
				TC_StrData(strlist,buf,num,ItemBuffer.rgb_buf[0]);// ����������
			}
			if(strlist->colg > 0)  //�����ɫ
			{		
       
				TC_StrData(strlist,buf,num,ItemBuffer.rgb_buf[1]);// ����������
			}
			if(SCREEN_YANSE == 3 && strlist->colb > 0) //�����ɫ
			{
        
				TC_StrData(strlist,buf,num,ItemBuffer.rgb_buf[2]);// ����������
			}
	  break;
  }
}



void ClrItemBuffer (void)
{
  uint16_t i,j;
	uint8_t *p;
	p = (uint8_t *)&ItemBuffer.itemlist;
	j = sizeof(ItemList);
	for(i=0;i<j;i++)
	{
    *p++ = 0;
  }
	for(i=0;i<SCREEN_YANSE;i++)
	{
    p = ItemBuffer.rgb_buf[i];
		for(j=0;j<SCREEN_BYTE_NUM;j++)
		  *p++ = 0;
  }
}

/*һ���ֽ�RGBһ���ֽ�RGB����*/
void TC_BmpItemOnebyOne(StrList *strlist)
{
//  uint16_t biWidth,biHeight;

	
	uint8_t err;//,test=0;
	BITMAPFILEHEADER file_header;   //bmp�ļ�ͷ
	BITMAPINFOHEADER info_header;   //bmp��Ϣͷ
	
	BYTE file_name[24];
	

	
	if(strlist->bmp_num<10)
		sprintf((char*)file_name,"0:/bmp/%c0%d.bmp",strlist->bmp_head,strlist->bmp_num);   // �����ļ������洢
  else	
		sprintf((char*)file_name,"0:/bmp/%c%d.bmp",strlist->bmp_head,strlist->bmp_num);   // �����ļ������洢
	
	err = ReadBMPInfo((char*)file_name,&file_header,&info_header);
	if(err)	return;

	if(info_header.biBitCount == 24)
	{
		BMP24RGBToItem((char*)file_name,info_header,file_header,strlist->posx,strlist->posy);
	}else if(info_header.biBitCount == 4 || info_header.biBitCount == 8)
	{
		BMP4_8RGBToItem((char*)file_name,info_header,file_header,strlist->posx,strlist->posy);
	}
}



//�����鱨�嵱ǰ��ʾ���ݣ�1.55�汾�޸ģ�
void InfmBoard_RetDisplayInfm (uint8_t *p,uint16_t *len, char* id)
{
	int res;  
	FIL fdst;
  FATFS fs;
  UINT br=0, bw=0,j=0,s;            // File R/W count 
	cJSON* cjson_test=NULL;
	
	BYTE file_name[20];
	uint16_t size;//,file_num;
	uint8_t pdata[2048]={0}, temp[256];
  uint8_t * p1;
	uint8_t err;//,temp_num;
	uint8_t  NOWItemTab[5],NOWItemTab1[6];
	uint16_t addr;
	p1 = p;
	
	cjson_test=cJSON_CreateObject();

	if(temporary_flag == 0)
		sprintf((char*)file_name,"0:/PLAY.LST");   // �ļ����洢
	else
		sprintf((char*)file_name,"0:/PLAYL.LST");   // �ļ����洢
  
  OSSemPend(SDSem,0,&err);
  f_mount(0,&fs);
  res = f_open(&fdst,(char*)file_name, FA_OPEN_EXISTING | FA_READ);
  if(res!=FR_OK)
  {
  f_mount(0,NULL);
  OSSemPost(SDSem);
  return;
  }
//	 res = f_lseek(&fdst,26);	
	 if(temporary_flag == 0)
	 {
		 res = f_read(&fdst,(uint8_t *)pdata, fdst.fsize, &br);
		 size = fdst.fsize;
	 }
	 else
	 {
 		 res = f_read(&fdst,(uint8_t *)pdata, fdst.fsize, &br);
		 size = fdst.fsize;
	 }   
		f_close(&fdst);/*�ر��ļ� */
		f_mount(0,NULL);
	 
//	 Json_playlist(pdata, size, p1);
	 
		OSSemPost(SDSem);
		
		if(NOWPlayNum<10)
		{
			NOWItemTab[0]='I';
			NOWItemTab[1]='t';
			NOWItemTab[2]='e';
			NOWItemTab[3]='m';
			NOWItemTab[4]=NOWPlayNum+48;
			
		 addr = InDex(pdata,size,NOWItemTab,5,0);
			
		}else
		{
			s=NOWPlayNum+1;
			NOWItemTab1[0]='I';
			NOWItemTab1[1]='t';
			NOWItemTab1[2]='e';
			NOWItemTab1[3]='m';
			NOWItemTab1[4]=NOWPlayNum/10+48;
			NOWItemTab1[5]=NOWPlayNum%10+48;
			addr = InDex(pdata,size,NOWItemTab1,6,0);
		}

	////����֡����
 	if(NOWPlayNum<10)
	{
		if(NOWPlayNum<PlayNum-1)//�Ƿ�Ϊ���һ֡
		{
	   NOWItemTab[4]=NOWItemTab[4]+1;
	   j=InDex(pdata,size,NOWItemTab,5,0);//��һ֡��ʵ��ַ
			
		 for(bw=0;bw<j-addr - 5 - 1;bw++)
	     temp[bw]=pdata[addr + bw + 1];
		}else
		{
		j=size;
		for(bw=0;bw<j - addr - 1;bw++)
	  temp[bw]=pdata[bw + addr + 1];	
		}
	}else//��ǰ֡������10
	{
		if(NOWPlayNum<PlayNum-1)
		{
			NOWItemTab1[4]=s/10+48;	
			NOWItemTab1[5]=s%10+48;	
			j=InDex(pdata,size,NOWItemTab1,6,0);
			for(bw=0;bw<j-addr - 6;bw++)
			temp[bw]=pdata[addr + bw + 1];
		}else
		{
		j=size;
		for(bw = 0;bw<j;bw++)
	  temp[bw]=pdata[bw + 1];	
		}
	}
//	temp[bw+1] = 0;
	cJSON_AddStringToObject(cjson_test,"screen",(const char*)temp);
	//base64����ת��
//	char *temp2 = NULL;
//	
//	temp2 = (char *)malloc((bw / 3 + 1) * 4);
//	char temp2[512] = {0};
//	*len = encode((char*)temp, bw, (char*)temp2);
	
	char* temp2 = NULL;
	temp2 = malloc(encode_length(bw));
	*len = encode((char*)temp, bw, (char*)temp2);

	
	//ƴ��Cjson����
	cJSON_AddStringToObject(cjson_test,"id",id);//����ַ����͵�
	cJSON_AddStringToObject(cjson_test,"status","200");//����ַ����͵�
	cJSON_AddStringToObject(cjson_test,"pic",(const char*)temp2);//����ַ����͵�
	free(temp2);
	
	char *cjson_data=cJSON_PrintUnformatted(cjson_test);
	memcpy(p1,cjson_data,strlen(cjson_data));
	*len=strlen(cjson_data);
	cJSON_Delete(cjson_test);
 
}








//������Ϣ
void InfmBoard_ErrAll(uint8_t * p,uint16_t *len, char* id)
{
//	uint16_t  crc16;
//	uint8_t errflag = 0;
//	uint32_t j;
	cJSON* cjson_test=NULL;
	cjson_test=cJSON_CreateObject();
	uint8_t* ackp = NULL, err;

	cJSON_AddStringToObject(cjson_test,"id",id);//����ַ����͵�
	
	ackp = OSQPend(InfmBoard_ErrQeue,100,&err);//��ȡ������Ϣ
	
	/*����һ���ߵ�ƽ����*/
	 
//	if(LdrSt==1)//��������
//	{
////		errflag = 1;
//		cJSON_AddNumberToObject(cjson_test,"photoSensitive", 1);
//	}
//	else{
//		cJSON_AddNumberToObject(cjson_test,"photoSensitive", 0);
//	}

if(err != OS_ERR_NONE){
	cJSON_AddStringToObject(cjson_test,"status","100");
	char *cjson_data=cJSON_PrintUnformatted(cjson_test);
	memcpy(p,cjson_data,strlen(cjson_data));
	*len=strlen(cjson_data);
	cJSON_Delete(cjson_test);
	return ;
}

	cJSON_AddStringToObject(cjson_test,"status","200");
	
	if(*(ackp + 2 + 6)>70)//�¶ȹ���,�����¶ȴ���70���϶ȹ���
	{
//		errflag = 1;
		cJSON_AddNumberToObject(cjson_test,"temperature", 1);
	}
	else{
		cJSON_AddNumberToObject(cjson_test,"temperature", 0);
	}
	
	
		if(*(ackp + 2 + 8) == 1)//����������
	{
//		errflag = 1;
		cJSON_AddNumberToObject(cjson_test,"internalFault", 1);
	}
	else{
		cJSON_AddNumberToObject(cjson_test,"internalFault", 0);
	}
	
	
	
		if(*(ackp + 2 + 9) == 1)//��ʾģ������
	{
//		errflag = 1;
		cJSON_AddNumberToObject(cjson_test,"moduleFault", 1);
	}
	else{
		cJSON_AddNumberToObject(cjson_test,"moduleFault", 0);
	}
	
	
		if(*(ackp + 2 + 10) == 1)//��ʾģ���Դ����
	{
//		errflag = 1;
		cJSON_AddNumberToObject(cjson_test,"powerFault", 1);
	}
	else{
		cJSON_AddNumberToObject(cjson_test,"powerFault", 0);
	}	

	
	
		if(*(ackp + 2 + 11) == 1)//�����عܹ���
	{
//		errflag = 1;
		cJSON_AddNumberToObject(cjson_test,"pixelFault", 1);
	}
	else{
		cJSON_AddNumberToObject(cjson_test,"pixelFault", 0);
	}
	
	
		if(*(ackp + 2 + 12) == 1)//���ϵͳ����
	{
//		errflag = 1;
		cJSON_AddNumberToObject(cjson_test,"checkSystemFault", 1);
	}
	else{
		cJSON_AddNumberToObject(cjson_test,"checkSystemFault", 0);
	}
	
	
		if(*(ackp + 2 + 13) == 1)//����������
	{
//		errflag = 1;
		cJSON_AddNumberToObject(cjson_test,"spdFault", 1);
	}
	else{
		cJSON_AddNumberToObject(cjson_test,"spdFault", 0);
	}
	

		if(*(ackp + 2 + 14) == 1)//���������
	{
//		errflag = 1;
		cJSON_AddNumberToObject(cjson_test,"acPowerFault", 1);
	}
	else{
		cJSON_AddNumberToObject(cjson_test,"acPowerFault", 0);
	}
	
	
//		if(*(ackp + 2 + 14) == 1)//���������
//	{
////		errflag = 1;
//		cJSON_AddNumberToObject(cjson_test,"acPowerFault", 1);
//	}
//	else{
//		cJSON_AddNumberToObject(cjson_test,"acPowerFault", 0);
//	}
//	
//	
//	
//	if(DoorSt>0)//���Ź���
//	{
////		errflag = 1;
//		cJSON_AddNumberToObject(cjson_test,"doorOpenFault", 1);
//	}
//	else{
//		cJSON_AddNumberToObject(cjson_test,"doorOpenFault", 0);
//	}
//	
//	if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_8)==0){//�����عܹ���
//		if(downele_flag==1){
////			errflag = 1;
//			cJSON_AddNumberToObject(cjson_test,"pixelFault", 1);
//		}
//		else{
//		cJSON_AddNumberToObject(cjson_test,"pixelFault", 0);
//		}
//	}
//	
//	if(host_Flag==1){//��ʾģ�����
////		errflag = 1;
//		cJSON_AddNumberToObject(cjson_test,"moduleFault", 1);
//	}
//	else{
//		cJSON_AddNumberToObject(cjson_test,"pixelFault", 0);
//		}
//	
//		//����������
//		//��ʾģ���Դ����
//		//���ϵͳ����
//		//����220V���������
//		//����������

	char *cjson_data=cJSON_PrintUnformatted(cjson_test);
	memcpy(p,cjson_data,strlen(cjson_data));
	*len=strlen(cjson_data);
	cJSON_Delete(cjson_test);
	memp_free(MEMP_PBUF_POOL,ackp);//�ͷ�POOL�ڴ�	
}

//����Ԥ�ò��ű�
void InfmBoard_LoadYuZhiFile(int filetype, char* filename, char * p,uint16_t len)
{
	FRESULT res;  
	FIL fdst;
	FATFS fs;
	UINT bw=0;            // File R/W count 
	char file_name[24] = {0};
	
//	uint8_t * p1;
	uint8_t err;
  
	OSSemPend(SDSem,0,&err);
	res = f_mount(0,&fs);
	if(res!=FR_OK)
	{
		f_mount(0,NULL);
		OSSemPost(SDSem);
		return;
	}
		
	//�����ļ�����ƴ�ӵ�ַ
	switch(filetype){
		case 0://���ű�
			sprintf(file_name, "0:/playlist/%s", filename);//û�п��Ǹ�λ����Ҫ��������
			break;
		case 1://ͼƬ
			sprintf(file_name, "0:/bm/%s.bmp", filename);//û�п��Ǹ�λ����Ҫ�������������ܻ�������
			break;
		case 2://�ֿ�
			sprintf(file_name, "0:/font/%s.bin", filename);//û�п��Ǹ�λ����Ҫ��������
			break;
		case 3://��Ƶ���ͣ���ûд��
			break;
		default:
			return;
	}
	
	res = f_open(&fdst,(char*)file_name, FA_CREATE_ALWAYS | FA_WRITE);//ֱ���ǽ��յ����ļ���
if(res!=FR_OK)
{
	f_mount(0,NULL);
	OSSemPost(SDSem);
	return;
}
	res = f_write(&fdst,p,len,&bw);//ֱ�Ӱ��ļ���Ϣȫ�����棬��֪���Բ���
	f_close(&fdst);/*�ر��ļ� */
	f_mount(0,NULL);
	OSSemPost(SDSem);	

}



//�ϴ��ļ�
void upload_dev_file(cJSON *json){
	char *filename;
	int fileType = 0;
	char* fileContext = NULL;
	uint16_t textsize = 0;
	char *text = NULL;
	
	fileContext=cjson_get_string(json,"fileContext");
	fileType=cjson_get_number(json,"fileType");
	filename=cjson_get_string(json,"fileName");
	
	//����
	if(is_base64(fileContext))//base64����
	{
		textsize = strlen(fileContext) / 4 * 3;
		text = malloc(textsize);
		decode(fileContext, strlen(fileContext), text, textsize);
	}
	else if(is_url_encoded(fileContext))//URL����
	{
		textsize = strlen(fileContext);
		text = malloc(textsize);
		urldecode(fileContext, textsize, text);
	}
	else//δ֪����
	{
//		return;
	}
	
	//�жϽ����ʼ۵���������,��������д��SD��
	InfmBoard_LoadYuZhiFile(fileType, filename, text, textsize);
	
	free(text);
}


//�ļ�����
uint16_t download_dev_file(cJSON *json, uint8_t * p,uint16_t *len, char* id){
	char *filename;
	int fileType = 0;
//	char* fileContext = NULL;
	uint16_t textsize = 0;
	char text[512] = {0};
	
	fileType=cjson_get_number(json,"fileType");
	filename=cjson_get_string(json,"fileName");
	
	//��ȡ�ļ�����
		textsize = InfmBoard_Read_file_text(fileType, filename, text);
		
		//ƴ��Ӧ������
		InfmBoard_Read_File_Ack(filename, p, len, text, textsize, id);
	return textsize;

	
}

//��ȡԤ�ò��ű�
//uint16_t InfmBoard_Read_file_text(int filetype, char* filename, char * text,uint16_t textsize, uint8_t * p,uint16_t *len)
extern uint8_t read_fime_num;
uint16_t InfmBoard_Read_file_text(int filetype, char* filename, char* text)
{
	

int res;  
	FIL fdst;
  FATFS fs;
  UINT br=0;//, bw=0,j=0,s;            // File R/W count 
//	cJSON* cjson_test=NULL;
	
	char file_name[20];
	uint16_t size;//,file_num;
	uint8_t err;//,temp_num;

	
	
		//�����ļ�����ƴ�ӵ�ַ
	switch(filetype){
		case 0://���ű�
			sprintf(file_name, "0:/playlist/%s", filename);//û�п��Ǹ�λ����Ҫ��������
			break;
		case 1://ͼƬ
			sprintf(file_name, "0:/bmp/%s.BMP", filename);//û�п��Ǹ�λ����Ҫ�������������ܻ�������
			break;
		case 2://�ֿ�
			sprintf(file_name, "0:/font/%s.bin", filename);//û�п��Ǹ�λ����Ҫ��������
			break;
		default:
			return 0;
	}

//		sprintf((char*)file_name,"0:/playlist/082");   // �ļ����洢
  
  OSSemPend(SDSem,0,&err);
  f_mount(0,&fs);
  res = f_open(&fdst,(char*)file_name, FA_OPEN_EXISTING | FA_READ);
  if(res!=FR_OK)
  {
  f_mount(0,NULL);
  OSSemPost(SDSem);
  return 0;
  }
	 res = f_lseek(&fdst,read_fime_num * 512);
	if(res!=FR_OK)
  {
  f_mount(0,NULL);
  OSSemPost(SDSem);
//	printf("ƫ��ʧ��\r\n");
  return 0;
  }
//	printf("ƫ���� %d\r\n",read_fime_num * 512);

		 res = f_read(&fdst,(uint8_t *)text, 512, &br);
//	printf("br = %d\r\n", br);
	if(res!=FR_OK)
  {
  f_mount(0,NULL);
  OSSemPost(SDSem);
	printf("��ȡʧ��\r\n");
  return 0;
  }
		 size = br;
	  
	 
		f_close(&fdst);/*�ر��ļ� */
		f_mount(0,NULL);
		OSSemPost(SDSem);
	 return size;


}


//�ļ����ػظ�
//Ӧ�����ݵ�ַ Ӧ�����ݳ��� �ļ����ݵ�ַ �ļ����ݳ��� ��Ϣ���
void InfmBoard_Read_File_Ack(char* filename,uint8_t * p,uint16_t *len, char* filetext, uint16_t filelen, char* id)
{
//	uint16_t  crc16;
//	uint8_t i,k,errflag = 0;
//	uint32_t j;
	char name[24] = {0};
	cJSON* cjson_test=NULL;
	cjson_test=cJSON_CreateObject();
	
	sprintf(name, "0:/%s", filename);
//	cJSON_AddStringToObject(cjson_test,"name",name);
	cJSON_AddStringToObject(cjson_test,"id",id);//����ַ����͵�
	cJSON_AddNumberToObject(cjson_test,"status",200);//����ַ����͵�
	cJSON_AddStringToObject(cjson_test,"msg","BASE64");//����ַ����͵�
	cJSON_AddStringToObject(cjson_test,"screen",filetext);
	
	
	//�������ʽת��Ϊbase64
//	char* temp = NULL;
//	temp = malloc((filelen / 3 + 1) * 4);
//	strncpy(temp, (const char*)filetext, filelen);
//	filelen = encode(filetext, filelen,temp);
	char* temp2 = NULL;
////	uint16_t size = 0;
////	size = encode_length(filelen);
	temp2 = malloc(encode_length(filelen));
	*len = encode((char*)filetext, filelen, (char*)temp2);
	
	cJSON_AddStringToObject(cjson_test,"fileContext",temp2);//����ַ����͵�
	free(temp2);
//	cJSON_AddNumberToObject(cjson_test,"len",filelen);
	//������ת��Ϊ�ַ���
	char *cjson_data=cJSON_PrintUnformatted(cjson_test);
	memcpy(p,cjson_data,strlen(cjson_data));
	*len=strlen(cjson_data);
	cJSON_Delete(cjson_test);
}

//�ļ�ɾ��
uint8_t delete_dev_file(cJSON *json){
	char *filename, *file_name;
	int fileType = 0;
	FRESULT res;
//	char *text = NULL;
	uint8_t err;
	FATFS fs;
	
	fileType=cjson_get_number(json,"fileType");
	filename=cjson_get_string(json,"fileName");
	
	//�����ļ�����ƴ�ӵ�ַ
	switch(fileType){
		case 0://���ű�
			sprintf(file_name, "0:/playlist/%s", filename);//û�п��Ǹ�λ����Ҫ��������
			break;
		case 1://ͼƬ
			sprintf(file_name, "0:/bm/%s.bmp", filename);//û�п��Ǹ�λ����Ҫ�������������ܻ�������
			break;
		case 2://�ֿ�
			sprintf(file_name, "0:/font/%s.bin", filename);//û�п��Ǹ�λ����Ҫ��������
			break;
		default:
			return 0;
	}
	
	OSSemPend(SDSem,0,&err);
	f_mount(0,&fs);//�����ļ�ϵͳ
	res = f_unlink(file_name);//ɾ���ļ�
	f_mount(0,NULL);//ж���ļ�ϵͳ
	OSSemPost(SDSem);	
	
	if(res == FR_OK)
		return 1;
	else
		return 0;
	
}


//�ļ��б���Ϣ
void Dev_File_Dir(cJSON *json, uint8_t * p,uint16_t *len, char* id){
char dir_name[16] = {0};
	int fileType = 0;
	FRESULT res;
//	char *text = NULL;
	uint8_t err;
	FATFS fs;
	DIR dir;
	FILINFO fileinfo;
	cJSON* cjson_test=NULL;
	cJSON* filename_array = NULL;
	cjson_test=cJSON_CreateObject();
	filename_array=cJSON_CreateArray();
	
	fileType=cjson_get_number(json,"fileType");
	
	//�����ļ����ͻ�ȡ�ļ����ƣ���ʱ��֪�����ĸ��ļ���ȡ
	switch(fileType){
		case 0://���ű�
			strcpy(dir_name, "0:/playlist");
			break;
		case 1://ͼƬ
			strcpy(dir_name, "0:/bmp");
			break;
		case 2://�ֿ�
			strcpy(dir_name, "0:/font");
			break;
		default:
			return;
	}
	
	OSSemPend(SDSem,0,&err);
	res = f_mount(0,&fs);//�����ļ�ϵͳ
	if(res == FR_OK){
		res = f_opendir(&dir, dir_name);
		if(res == FR_OK){
			while(1){
				res = f_readdir(&dir, &fileinfo);
				if(res != FR_OK || fileinfo.fname[0] == 0)//��ȡ�ļ����µ��ļ���
					break;
				cJSON_AddItemToArray(filename_array, cJSON_CreateString(fileinfo.fname));
			}
			
		}
	}
	
	f_mount(0,NULL);//ж���ļ�ϵͳ
	OSSemPost(SDSem);	
	
	cJSON_AddStringToObject(cjson_test,"id",id);//����ַ����͵�
	cJSON_AddNumberToObject(cjson_test,"status",200);//����ַ����͵�
	cJSON_AddStringToObject(cjson_test,"msg"," ");//����ַ����͵�
	cJSON_AddItemToObject(cjson_test,"fileList",filename_array);//��ʱ������ļ����ƴ������ȡ
	
	//������ת��Ϊ�ַ���
	char *cjson_data=cJSON_PrintUnformatted(cjson_test);
	memcpy(p,cjson_data,strlen(cjson_data));
	*len=strlen(cjson_data);
	cJSON_Delete(cjson_test);
	
}

//���ű�json��ʽƴ��
void Json_playlist(char* list, uint16_t len, char* p){
	uint16_t addr = NULL, f_num = 0, temp = 0;
	uint16_t i = 0;
	uint8_t num = 0;
	char yanse[20] = {0}, text[24] = {0};
	char ItemTab[7] = {'I', 't', 'e', 'm', 0, 0, 0};
    cJSON* cjson_playlist = cJSON_CreateObject(); // ���ű�
    cJSON* scene_array = cJSON_CreateArray(); // ��������
    cJSON* regisons_array = cJSON_CreateArray(); // �����б�����
    cJSON* items_array = NULL;
	  cJSON* items_object = NULL;
	  cJSON* duration_object = NULL;
	  cJSON* backGround_object = NULL;
	  cJSON* transition_object = NULL;
	  cJSON* content_object = NULL;
	  cJSON* regison_object = NULL;
	
#if cJSON_Dbug
	printf("list %s\r\n", list);
#endif

    // ���ű������ �ļ����� �ļ��汾
    cJSON_AddStringToObject(cjson_playlist, "fileType", "playTable");
    cJSON_AddStringToObject(cjson_playlist, "version", "1");
		
//	if(InDex((uint8_t*)list,len,(uint8_t*)"\\C",8,0)){//�ı����ű�
	if(strstr(list,"\\C") != NULL){//�ı����ű�
		//��ȡ�б�֡��
		addr = InDex((uint8_t*)list,len,(uint8_t*)"Item_no=",8,0);
		for( i = addr; list[i]>= 48 && list[i]<= 57; i++){
			f_num = f_num*10 + list[i] - 48;
		}
		
#if cJSON_Dbug
	printf("֡��%d\r\n", f_num);
#endif
		//��֡����
		for(uint16_t m = 0; m < f_num; m++){
			items_array = cJSON_CreateArray(); // �ز��б�����
			items_object = cJSON_CreateObject();
			duration_object = cJSON_CreateObject(); // ����ʱ�����
			backGround_object = cJSON_CreateObject(); // ��������
			transition_object = cJSON_CreateObject(); // ��������
			content_object = cJSON_CreateObject(); // �ı����ݶ���
			regison_object = cJSON_CreateObject();
			if(m < 10){
				ItemTab[4] = m + 48;
				ItemTab[5] = '=';
			}
			else{
				ItemTab[4]=m/10+48;	
				ItemTab[5]=m%10+48;
				ItemTab[6]='=';
			}
			
#if cJSON_Dbug
	printf("tab %s, %d\r\n", ItemTab, strlen(ItemTab));
#endif
			i = 0;
			addr = InDex((uint8_t*)list,len,(uint8_t*)ItemTab,strlen(ItemTab),0);
			for(i = addr; list[i]>= 48 && list[i]<= 57; i++){
				temp = temp * 10 + list[i] - 48;
			}
			
#if cJSON_Dbug
	printf("delay %d\r\n", temp);
#endif
			cJSON_AddNumberToObject(duration_object, "total", temp);//�ܳ���ʱ��
			
			//����
			temp = 0;
			for(i = i + 1; list[i]>= 48 && list[i]<= 57; i++){
				temp = temp * 10 + list[i] - 48;
			}
#if cJSON_Dbug
	printf("type %d\r\n", temp);
#endif
			
			cJSON_AddNumberToObject(transition_object, "type", temp);
			temp = 0;
			for(i = i + 1; list[i]>= 48 && list[i]<= 57; i++){
				temp = temp * 10 + list[i] - 48;
			}
#if cJSON_Dbug
	printf("speed %d\r\n", temp);
#endif
			
			cJSON_AddNumberToObject(transition_object, "speed", temp);
			
			//������ɫ
			num = 0;
//			printf("2.addr %d, i %d\r\n", addr, i);
//			addr = InDex((uint8_t*)list ,len - i,(uint8_t*)"\\c",2,i);
			char* addr2 = strstr(list + i, "\\c");
			addr = addr2 - list + 2;
//			printf("��ɫ %s, ���� %d\r\n", "\\c", strlen("\\c"));
//			printf("3.addr %d, i %d\r\n", addr, i);
			for(i = addr; list[i]>= 48 && list[i]<= 57; i++){
				yanse[num] = list[i];
				num++;
//				printf("addr %d, i %d\r\n", addr, i);
				if(num == 3 || num == 7 || num == 11){
					yanse[num] = '.';
					num++;
				}
			}
#if cJSON_Dbug
	printf("backColor %s\r\n", yanse);
#endif
			
			cJSON_AddStringToObject(backGround_object, "backColor", yanse);
			
		//�����ı�
			num = 0;
//			printf("4.addr %d, i %d\r\n", addr, i);
			for(i = i; list[i] != '\r'; i++){
				text[num] = list[i];
				num++;
			}
#if cJSON_Dbug
	printf("text %s\r\n", text);
#endif
			
			cJSON_AddStringToObject(content_object, "text", text);
			
			
			// �������б����������
    cJSON_AddStringToObject(regison_object, "id", "id");
    cJSON_AddNumberToObject(regison_object, "layer", 1);
    cJSON_AddNumberToObject(regison_object, "x", 100);
    cJSON_AddNumberToObject(regison_object, "y", 125);
    cJSON_AddNumberToObject(regison_object, "width", 126);
    cJSON_AddNumberToObject(regison_object, "heigth", 127);
    cJSON_AddNumberToObject(regison_object, "regionLoop", 1);

    // �����ز��б�
    cJSON_AddNumberToObject(items_object, "type", 0);
    cJSON_AddNumberToObject(items_object, "align", 0);
    cJSON_AddNumberToObject(items_object, "fspace", 0);
    cJSON_AddNumberToObject(items_object, "lspace", 0);

    
    cJSON_AddItemToObject(duration_object, "delay", cJSON_CreateNull());
    cJSON_AddItemToObject(duration_object, "playCount", cJSON_CreateNull());
    cJSON_AddItemToObject(items_object, "duration", duration_object);

    
    cJSON_AddItemToObject(items_object, "backGround", backGround_object);

    
    cJSON_AddItemToObject(items_object, "transition", transition_object);

    
    cJSON_AddItemToObject(items_object, "content", content_object);
		
		cJSON_AddItemToArray(items_array, items_object);

//    cJSON_AddItemToArray(regison_object, items_array);
		cJSON_AddItemToObject(regison_object, "items", items_array);
    cJSON_AddItemToArray(regisons_array, regison_object);
		}
	}
//	else if(InDex((uint8_t*)list,len,(uint8_t*)"\\B",2,0) != 0){//ͼƬ���ű�
	else if(strstr(list,"\\C") != NULL){//ͼƬ���ű�
				//��ȡ�б�֡��
		addr = InDex((uint8_t*)list,len,(uint8_t*)"Item_no=",8,0);
		for( i = addr; list[i]>= 48 && list[i]<= 57; i++){
			f_num = f_num*10 + list[i] - 48;
		}
#if cJSON_Dbug
	printf("֡��%d\r\n", f_num);
#endif
		
		//��֡����
		for(uint16_t m = 0; m < f_num; m++){
			items_array = cJSON_CreateArray(); // �ز��б�����
			items_object = cJSON_CreateObject();
			duration_object = cJSON_CreateObject(); // ����ʱ�����
			backGround_object = cJSON_CreateObject(); // ��������
			transition_object = cJSON_CreateObject(); // ��������
			content_object = cJSON_CreateObject(); // �ı����ݶ���
			regison_object = cJSON_CreateObject();
			if(m < 10){
				ItemTab[4] = m + 48;
				ItemTab[5] = '=';
			}
			else{
				ItemTab[4]=m/10+48;	
				ItemTab[5]=m%10+48;
				ItemTab[6]='=';
			}
#if cJSON_Dbug
	printf("tab %s, %d\r\n", ItemTab, strlen(ItemTab));
#endif
			
			i = 0;
			addr = InDex((uint8_t*)list,len,(uint8_t*)ItemTab,strlen(ItemTab),0);
//			printf("1.addr %d, i %d\r\n", addr, i);
			for(i = addr; list[i]>= 48 && list[i]<= 57; i++){
				temp = temp * 10 + list[i] - 48;
			}
#if cJSON_Dbug
	printf("delay %d\r\n", temp);
#endif
			
			cJSON_AddNumberToObject(duration_object, "total", temp);//�ܳ���ʱ��
			
			//����
			temp = 0;
			for(i = i + 1; list[i]>= 48 && list[i]<= 57; i++){
				temp = temp * 10 + list[i] - 48;
			}
#if cJSON_Dbug
	printf("type %d\r\n", temp);
#endif
			
			cJSON_AddNumberToObject(transition_object, "type", temp);
			temp = 0;
			for(i = i + 1; list[i]>= 48 && list[i]<= 57; i++){
				temp = temp * 10 + list[i] - 48;
			}
#if cJSON_Dbug
	printf("speed %d\r\n", temp);
#endif
			
			cJSON_AddNumberToObject(transition_object, "speed", temp);
			
			//ͼƬ�ļ�����
			num = 0;
//			printf("2.addr %d, i %d\r\n", addr, i);
//			addr = InDex((uint8_t*)list ,len - i,(uint8_t*)"\\c",2,i);
			char* addr2 = strstr(list + i, "\\B");
			addr = addr2 - list + 2;
			for(i = i; list[i] != '\r'; i++){
				text[num] = list[i];
				num++;
			}
#if cJSON_Dbug
	printf("file %s\r\n", text);
#endif
			
			cJSON_AddStringToObject(content_object, "file", text);
			
			
			// �������б����������
    
    cJSON_AddStringToObject(regison_object, "id", "id");
    cJSON_AddNumberToObject(regison_object, "layer", 1);
    cJSON_AddNumberToObject(regison_object, "x", 100);
    cJSON_AddNumberToObject(regison_object, "y", 125);
    cJSON_AddNumberToObject(regison_object, "width", 126);
    cJSON_AddNumberToObject(regison_object, "heigth", 127);
    cJSON_AddNumberToObject(regison_object, "regionLoop", 1);

    // �����ز��б�
    cJSON_AddNumberToObject(items_object, "type", 0);
    cJSON_AddNumberToObject(items_object, "align", 0);
    cJSON_AddNumberToObject(items_object, "fspace", 0);
    cJSON_AddNumberToObject(items_object, "lspace", 0);
   
    cJSON_AddItemToObject(duration_object, "delay", cJSON_CreateNull());
    cJSON_AddItemToObject(duration_object, "playCount", cJSON_CreateNull());
    cJSON_AddItemToObject(items_object, "duration", duration_object);
    
    cJSON_AddItemToObject(items_object, "transition", transition_object);

    cJSON_AddItemToObject(items_object, "content", content_object);

    cJSON_AddItemToArray(regison_object, items_array);
    cJSON_AddItemToArray(regisons_array, regison_object);
		}
	}
	else if(InDex((uint8_t*)list,len,(uint8_t*)"\\V",2,0) != 0){//��Ƶ���ű�(δ���)
		return;
	}
	else{
		return;
	}
		            

    

    cJSON_AddItemToArray(scene_array, regisons_array);

    cJSON_AddItemToObject(cjson_playlist, "scense", scene_array);

    // ������ת��Ϊ�ַ���
    char* cjson_data = cJSON_PrintUnformatted(cjson_playlist);
    cJSON_Delete(cjson_playlist);
	memcpy(p,cjson_data,strlen(cjson_data));

#if cJSON_Dbug
	printf("%s\r\n", cjson_data);
#endif
    
}



//������Ϣ�ϴ�
void InfmBoardLed_Err_ACK(uint8_t * p,uint16_t *len, char* id)
{

	cJSON* cjson_test=NULL;
	cjson_test=cJSON_CreateObject();
	uint8_t* ackp = NULL, err, *temp = NULL;
	uint16_t size = 0;

	cJSON_AddStringToObject(cjson_test,"id",id);//����ַ����͵�
	
	/*************��һ������*************/
	GetSlaveLedErrData(1);
	ackp = OSQPend(InfmBoardLed_ErrQeue,100,&err);//��ȡ������Ϣ
	if(err != OS_ERR_NONE)
//		if(ackp != NULL)
		{	//�жϲ�ƴ��json����
		cJSON_AddStringToObject(cjson_test,"status","100");//����ַ����͵�
		cJSON_AddStringToObject(cjson_test,"badPixelMap", "");//����ַ����͵�
	}
	else{
//		size = *(uint16_t* )(ackp + 2 + 4);//�������ݴ�С
		size = 256;
//		printf("size = %d\r\n", size);
		cJSON_AddStringToObject(cjson_test,"status","200");//����ַ����͵�
		temp = (uint8_t*)malloc(400);
		for(uint16_t i = 0; i < size; i++){
			*(temp +i) = *(ackp + 2 + 6 + i);
		}
//		cJSON_AddStringToObject(cjson_test,"badPixelMap", (const char*)temp);//����ַ����͵�
//		free(temp);
//		temp = NULL;
		memp_free(MEMP_PBUF_POOL,ackp);//�ͷ�POOL�ڴ�
		ackp = NULL;
	}
	
	/**********�ڶ�������***********/
//	printf("�ڶ���\r\n");
	GetSlaveLedErrData(2);
	ackp = OSQPend(InfmBoardLed_ErrQeue,100,&err);//��ȡ������Ϣ
	
//	if(size == 0 || ackp == NULL)
	if(err != OS_ERR_NONE)
//		if(ackp != NULL)
		{	//�жϲ�ƴ��json����
		cJSON_AddStringToObject(cjson_test,"status","100");//����ַ����͵�
		cJSON_AddStringToObject(cjson_test,"badPixelMap", "");//����ַ����͵�
	}
	else{
//		size = *(uint16_t* )(ackp + 2 + 4);//�������ݴ�С
//		size = 128;
//		printf("size = %d\r\n", size);
////		cJSON_AddStringToObject(cjson_test,"status","200");//����ַ����͵�
//		temp = malloc(size);
		for(uint16_t i = 256; i < 384; i++){
			*(temp +i) = *(ackp + 2 + 6 + i - 256);
		}
		cJSON_AddStringToObject(cjson_test,"badPixelMap", (const char*)temp);//����ַ����͵�
		free(temp);
		temp = NULL;
		memp_free(MEMP_PBUF_POOL,ackp);//�ͷ�POOL�ڴ�
		ackp = NULL;
	}

	char *cjson_data=cJSON_PrintUnformatted(cjson_test);
//	printf("cjson_data = %s\r\n", cjson_data);
	memcpy(p,cjson_data,strlen(cjson_data));
	*len=strlen(cjson_data);
	cJSON_Delete(cjson_test);
	
}












