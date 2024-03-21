#include "sw_app.h"
#include "includes.h"
#include "sw_conf.h"
#include "sw_global.h"
#include "i2c.h"
#include "SPI.h"
#include "malloc.h"
#include "memp.h"
#include "delay.h"
#include "usart.h"	
#include "led.h"	
#include "string.h"
#include "led_gpio.h"
#include "wdg.h"

uint8_t zm[SCREEN_HEIGHT][((SCREEN_WIDTH/8)+12)];//组成整平字模

uint8_t  soundplay_msg = 0;   //存放播放信息

uint8_t  PF0_flag = 0;   //存放播放信息

static uint8_t lvjiantou[32][4]={
{0x00,0x00,0x00,0x00},
{0x00,0x00,0x00,0x20},
{0x00,0x00,0x00,0x70},
{0x00,0x00,0x00,0xF8},
{0x00,0x00,0x01,0xFC},
{0x00,0x00,0x03,0xFE},
{0x04,0x00,0x07,0xFC},
{0x0C,0x00,0x0F,0xF8},
{0x1C,0x00,0x1F,0xF0},
{0x3C,0x00,0x3F,0xE0},
{0x7C,0x00,0x7F,0xC0},
{0x7C,0x00,0xFF,0x80},
{0x7C,0x01,0xFF,0x00},
{0x7C,0x03,0xFE,0x00},
{0x7C,0x07,0xFC,0x00},
{0x7C,0x0F,0xF8,0x00},
{0x7C,0x1F,0xF0,0x00},
{0x7C,0x3F,0xE0,0x00},
{0x7C,0x7F,0xC0,0x00},
{0x7C,0xFF,0x80,0x00},
{0x7D,0xFF,0x00,0x00},
{0x7F,0xFE,0x00,0x00},
{0x7F,0xFC,0x00,0x00},
{0x7F,0xF8,0x00,0x00},
{0x7F,0xF0,0x00,0x00},
{0x7F,0xE0,0x00,0x00},
{0x7F,0xFF,0xFF,0xC0},
{0x7F,0xFF,0xFF,0x80},
{0x7F,0xFF,0xFF,0x00},
{0x7F,0xFF,0xFE,0x00},
{0x7F,0xFF,0xFC,0x00},
{0x00,0x00,0x00,0x00},
};
static uint8_t hongcha[32][4]={
{0x00,0x00,0x00,0x00},
{0x04,0x00,0x00,0x20},
{0x0E,0x00,0x00,0x70},
{0x1F,0x00,0x00,0xF8},
{0x3F,0x80,0x01,0xFC},
{0x7F,0xC0,0x03,0xFE},
{0x3F,0xE0,0x07,0xFC},
{0x1F,0xF0,0x0F,0xF8},
{0x0F,0xF8,0x1F,0xF0},
{0x07,0xFC,0x3F,0xE0},
{0x03,0xFE,0x7F,0xC0},
{0x01,0xFF,0xFF,0x80},
{0x00,0xFF,0xFF,0x00},
{0x00,0x7F,0xFE,0x00},
{0x00,0x3F,0xFC,0x00},
{0x00,0x1F,0xF8,0x00},
{0x00,0x1F,0xF8,0x00},
{0x00,0x3F,0xFC,0x00},
{0x00,0x7F,0xFE,0x00},
{0x00,0xFF,0xFF,0x00},
{0x01,0xFF,0xFF,0x80},
{0x03,0xFE,0x7F,0xC0},
{0x07,0xFC,0x3F,0xE0},
{0x0F,0xF8,0x1F,0xF0},
{0x1F,0xF0,0x0F,0xF8},
{0x3F,0xE0,0x07,0xFC},
{0x7F,0xC0,0x03,0xFE},
{0x3F,0x80,0x01,0xFC},
{0x1F,0x00,0x00,0xF8},
{0x0E,0x00,0x00,0x70},
{0x04,0x00,0x00,0x20},
{0x00,0x00,0x00,0x00},
};

//led任务
void key_task(void *pdata)
{
	(void)pdata; 
	uint8_t key_flag=0;
	uint8_t key_flag1=0;
	while(1)
	{
		OSTimeDly(10);
		if(PEin(6)==0)
		{
			OSTimeDly(5);
			if(PEin(6)==0)
			{
				key_flag++;
				while(PEin(6)!=1)
				{
					OSTimeDly(1);
				}
			}
		}
		if(PFin(0)==0)
		{
			PF0_flag=0;
		}
		else
		{
			PF0_flag=1;
		}
		if(key_flag==7)
			key_flag=0;
		if(key_flag!=0&&key_flag1!=key_flag)
		{
			KEY_Mode(key_flag);     //按键处理函数
		}
		key_flag1=key_flag;	
	}
}


//led任务
void led_task(void *pdata)
{
	int s = -5;
	while(1)
	{
		LED0 = !LED0;
		OSTimeDlyHMSM(0,0,1,0);  //延时1s
	}
}
void TestCode(void *pdata)
{
	(void)pdata;		// 'p_arg' 并没有用到，防止编译器提示警告
	StrFrameHeader *frame_head;//原始输入数据流
	INT8U *memp_buf;//报文数据
	StrFrameHeader *mOutHead=NULL; //本函数输出的数据流
	uint8_t err,i;
	uint8_t frame_fun;
	while(1)
	{		
		frame_head = OSQPend(En_Dis_CodeQeue,0,&err);   	//获取接收控制中心数据的消息指针		
		if(frame_head->len <= 4)//数据长度小于4--数据有误
		{
			memp_free(MEMP_PBUF_POOL,(void *)frame_head);	 //释放POOL内存	(用于返回上位机的数据包)			
			return;	
		}

		if(frame_head->type==HEAD_USART2_TYPE)
		{		
			static	uint8_t data[4]={'W','1','\r','\n'};
			printf_usart2(data,4);
		}
		else if(frame_head->type==HEAD_ETH_TYPE)
		{
			memp_buf = memp_malloc(MEMP_PBUF_POOL);//申请一块内存pool
			mOutHead = (StrFrameHeader *)memp_buf;
			static	uint8_t data[4]={'W','1','\r','\n'};
			mOutHead->type = HEAD_ETH_TYPE;
			mOutHead->len=4;
			mOutHead->pdata = memp_buf + sizeof(StrFrameHeader);
			memp_buf =mOutHead->pdata;         //定位接收数据指针
			for(int i=0;i<4;i++)//=======转译报文
			{
				*memp_buf++ = data[i];   
			}		
			err = OSQPost(En_Udp_CodeQeue,(void *)mOutHead);		
		}
		else
		{
			
		}		
		for(uint16_t i=0;i<frame_head->len;i++)
		{
			if(frame_head->pdata[i]=='{')
			{
				i=i+2;
				frame_fun = frame_head->pdata[i];
				switch(frame_fun)
				{
					//显示
#if shihangzuoyi
					case '1':case '2':
					case '3':case '4':		
					case '5':case '6':
					case '7':case '8':	
					case '9':				
						i=i+DisplayMsg2_Get(frame_head->pdata+i);
						break;0 
#endif
#if wuhanghuanhang
					case '1':
					case '2':
					case '3':
					case '4':		
					case '5':			
						i=i+DisplayMsg2_Get(frame_head->pdata+i);
						break;
#endif
					case 'S':
						soundplay_msg = 0;
						if(frame_head->pdata[4] != '}')//语音命令
							SOUND2_Play('S',frame_head->pdata+i);
						if(frame_head->pdata[3] == ']'
							&&frame_head->pdata[4] == '}' &&frame_head->len==5)   //自检命令
						{
	//            PWM_Set(80);//自检
							CHECK_Zijian_S();
						}
						SoundPlay();
						break;
					case 'U':	//检测命令
						CHECK_Jiance_U(frame_head->pdata+i);
						FrameDataLoad();
						break;
					case 'C'://清屏命令
						Clean_All();
						break;
					case 'M'://语音报价
						SOUND2_Play('M',frame_head->pdata+i);
						SoundPlay();
						break;
					case 'W'://状态上报
						break;
					case 'P'://亮度
	//					BRIGHT_Set(frame_head->pdata+i);
						break;
					case 'Y':
	//					PWM_Set(80);//自检
						Clean_All();
						CHOOSE_BPS_Y();
						break;
					default:
						break;
				}
			}
		}
		memp_free(MEMP_PBUF_POOL,(void *)frame_head);	 //释放POOL内存	(用于返回上位机的数据包)			
	}
}

void Write_RGB(void *pdata)
{
	(void)pdata; 
	while(1)
	{	
		uint16_t i;
		for(i=0;i<8;i++)
		{
#if 1
//			Select_OneHang(i);
//			LCPII=0;
//			WriteRGB_Data_8(i);
//			LCPII=1;		
//			TIM_SetCompare3(TIM1,0);
//			OSTimeDly(2);
//			TIM_SetCompare3(TIM1,99);
//			delay_us(200);
		LCPII=0;
		WriteRGB_Data_8(i);

		Select_OneHang(i);
		LCPII=1;
    OSTimeDly(1);		
		delay_us(800);
			

		LCPII=0;
    Send_0(i);
    Select_OneHang(i);
		LCPII=1;
		IWDG_Feed();
#else //全部亮
			Select_OneHang(i);
			LCPII=0;
			WriteRGB_Data_1_8(i);
			LCPII=1;	
			TIM_SetCompare3(TIM1,0);
			OSTimeDly(2);
#endif
/*
		LCPII(LCP_ON);
		Send_RGData(i);
    OSTimeDly(1);
		Select_OneHang(i);
		
    LCPII(LCP_OFF);
    
    LCPII(LCP_ON);
    Send_0(i);
    Select_OneHang(i);
    LCPII(LCP_OFF);
 */
		}		
	}
}


DisPlay_Data display_data;

void TASK_OverDataHandle(void *pdata)
{
	(void)pdata; 
	while(1)
	{
		OSTimeDly(400);
		GunDong();
	}
}	

uint16_t zuoyi[4];

void GunDong()
{	
	INT8U err;
	OSSemPend(ZMSem,0,&err);//请求信号量
	int i=0;	
	int hang;
	for(hang=0;hang<3;hang++)
	{
		if(zuoyi[hang]>0x60){
			for(i=0;i<=19;i++)
				{	
					zm[hang*16][i]=zm[hang*16][i]<<1|((zm[hang*16][i+1]&0x80)>>7);
					zm[hang*16+1][i]=zm[hang*16+1][i]<<1|((zm[hang*16+1][i+1]&0x80)>>7);
					zm[hang*16+2][i]=zm[hang*16+2][i]<<1|((zm[hang*16+2][i+1]&0x80)>>7);
					zm[hang*16+3][i]=zm[hang*16+3][i]<<1|((zm[hang*16+3][i+1]&0x80)>>7);
					zm[hang*16+4][i]=zm[hang*16+4][i]<<1|((zm[hang*16+4][i+1]&0x80)>>7);
					zm[hang*16+5][i]=zm[hang*16+5][i]<<1|((zm[hang*16+5][i+1]&0x80)>>7);
					zm[hang*16+6][i]=zm[hang*16+6][i]<<1|((zm[hang*16+6][i+1]&0x80)>>7);
					zm[hang*16+7][i]=zm[hang*16+7][i]<<1|((zm[hang*16+7][i+1]&0x80)>>7);
					zm[hang*16+8][i]=zm[hang*16+8][i]<<1|((zm[hang*16+8][i+1]&0x80)>>7);
					zm[hang*16+9][i]=zm[hang*16+9][i]<<1|((zm[hang*16+9][i+1]&0x80)>>7);
					zm[hang*16+10][i]=zm[hang*16+10][i]<<1|((zm[hang*16+10][i+1]&0x80)>>7);
					zm[hang*16+11][i]=zm[hang*16+11][i]<<1|((zm[hang*16+11][i+1]&0x80)>>7);
					zm[hang*16+12][i]=zm[hang*16+12][i]<<1|((zm[hang*16+12][i+1]&0x80)>>7);
					zm[hang*16+13][i]=zm[hang*16+13][i]<<1|((zm[hang*16+13][i+1]&0x80)>>7);
					zm[hang*16+14][i]=zm[hang*16+14][i]<<1|((zm[hang*16+14][i+1]&0x80)>>7);
					zm[hang*16+15][i]=zm[hang*16+15][i]<<1|((zm[hang*16+15][i+1]&0x80)>>7);
				}
			zuoyi[hang]--;
		}
	}
	OSSemPost(ZMSem);//释放信号量	
}

uint8_t DisplayMsg2_Get(uint8_t *pdata)
{
	uint8_t i=0,j=0;
	display_data.hangnum = pdata[0]-48-1;
	if(display_data.hangnum==0&&pdata[1]!=']')
	{
	display_data.hangnum=pdata[1]-48-1+10;
	i=3;
	}else
	i=2;//跳到数据开始地址
	while(pdata[i]!='}')
	{
		display_data.pdata[j++] = pdata[i++];
	}
	display_data.len = j;
	if(pdata[i+1]=='<')
	{
		i+=2;
	}
	zuoyi[display_data.hangnum]=display_data.len*8;	
	FrameDataLoad();
	memset(display_data.pdata,0,1024);
	display_data.hangnum=0;
	display_data.len=0;
	return i;
}
/**
  ******************************************************************************
  * 函数名 : Task_FrameDataLoad()
  * 描述   : 字符点阵下载
  * 参数   : 无
  * 返回值 : 无
  ******************************************************************************
**/

//uint8_t zf_num[10]={0,0,0,0,0,0,0,0,0,0};
//uint8_t hz_num[10]={0,0,0,0,0,0,0,0,0,0};
uint8_t  checkflag  = 0;   //自检标志位
ZM_DATA  zm_data;

uint8_t  hz_msb=0;           //存放汉字高八位	
uint8_t  hz_lsb=0;           //存放汉字第八位

void FrameDataLoad(void)
{
#if shihangzuoyi
	uint8_t num=0;//第几个字模
	uint16_t i;
	Clean_Hang2(display_data.hangnum+1);//清屏
	for(i=0;i<display_data.len;i++)
	{	
		hz_msb = display_data.pdata[i];	//汉字字模高八位
		hz_lsb = display_data.pdata[i+1];//汉字字模低八位
		if(GB2312_16X16(hz_msb,hz_lsb,zm_data.HZ_Data) == 1)	//汉字
		{
			i++;
			zm_data.HZorZF = HZ;
			if(checkflag == 0)
			{
				ZM_Updata(display_data.hangnum,num,zm_data.HZ_Data);
			}
			else if(checkflag==1)//U命令，全屏点亮一个汉子
			{
				CHECK_Jiance_Send(i);
				checkflag = 0;
			}
		num+=2;
		}else//字符
		{
			zm_data.HZorZF = ZF;    
			ASCII_8X16(display_data.pdata[i],zm_data.ZF_Data);
			if(checkflag == 0)
			{
				ZM_Updata(display_data.hangnum,num,zm_data.ZF_Data);			
			}
			else if(checkflag==1)//U命令，全屏点亮两个字符
			{
				ASCII_8X16(display_data.pdata[i+1],zm_data.ZF_Data1);
				CHECK_Jiance_Send(i);
				checkflag = 0;
				return;
			}
			num++;
		}
	}
#endif

#if wuhanghuanhang
	uint8_t num=0;//第几个字模
	uint16_t i;
	for(i=0;i<display_data.len;i++)
	{	
		hz_msb = display_data.pdata[i];	//汉字字模高八位
		hz_lsb = display_data.pdata[i+1];//汉字字模低八位
		if(GB2312_16X16(hz_msb,hz_lsb,zm_data.HZ_Data) == 1)	//汉字
		{
			i++;
			zm_data.HZorZF = HZ;
			if(checkflag == 0)
			{
			if(num<12)
				ZM_Updata(display_data.hangnum*2,num,zm_data.HZ_Data);
			else
				ZM_Updata(display_data.hangnum*2+1,num-12,zm_data.HZ_Data);
			}
			else if(checkflag==1)//U命令，全屏点亮一个汉子
			{
				CHECK_Jiance_Send(i);
				checkflag = 0;
			}
		num+=2;
		}else//字符
		{
			zm_data.HZorZF = ZF;    
			ASCII_8X16(display_data.pdata[i],zm_data.ZF_Data);
			if(checkflag == 0)
			{
			if(num<12)
				ZM_Updata(display_data.hangnum*2,num,zm_data.ZF_Data);	
			else
				ZM_Updata(display_data.hangnum*2+1,num-12,zm_data.ZF_Data);
			}
			else if(checkflag==1)//U命令，全屏点亮两个字符
			{
				ASCII_8X16(display_data.pdata[i+1],zm_data.ZF_Data1);
				CHECK_Jiance_Send(i);
				checkflag = 0;
				return;
			}
			num++;
		}
	}
#endif
}
//第x行第y个字
void ZM_Updata(uint8_t x,uint8_t y,uint8_t *pdata)
{
	uint8_t i;
	uint8_t *ptemp = pdata;
	INT8U err;
	if(y > ((SCREEN_WIDTH/8)+30))//超出空间大小退出
		return;
	OSSemPend(ZMSem,0,&err);//请求信号量	
	for(i=16*x;i<16*(x+1);i++)
	{
			if(zm_data.HZorZF == HZ)
			{			
				zm[i][y]  = *ptemp++;
				zm[i][y+1]= *ptemp++;			
			}
			else if(zm_data.HZorZF ==  ZF)
				zm[i][y] = *ptemp++;
	}
	OSSemPost(ZMSem);//释放信号量		
}
///////////////////////////////////////////////////////////////////////////////////////////////////
/*模拟38译码器行选通*/
void Select_OneHang (uint8_t num)
{
	uint16_t data;
	data=GPIO_ReadInputData(GPIOE);
	data &= 0xfff8;              // 置PE0 1 2 3位为0
	data |= ((uint16_t)num);
	GPIO_Write(GPIOE,data); 
}


void WriteRGB_Data_8(uint16_t i)
{	
	uint32_t red_data[6][12]={0};
	INT8U err;
/*
{[1]		车  型:      1型}<10>
{[2]		金  额:     95元}<10>
{[3]		总  重：    65吨}<10>
{[4]		超  重：    10吨}<10>
	red_data[0][0]  前面是第几个字 后面是第几个IO口
*/

if(PF0_flag==0){
hongchalvjiantou_flag=0;
red_data[2][0]=(uint32_t)lvjiantou[i+8][0]<<24|(uint32_t)lvjiantou[i+8][1]<<16|(uint32_t)lvjiantou[i][0]<<8|(uint32_t)lvjiantou[i][1];
red_data[3][0]=(uint32_t)lvjiantou[i+8][2]<<24|(uint32_t)lvjiantou[i+8][3]<<16|(uint32_t)lvjiantou[i][2]<<8|(uint32_t)lvjiantou[i][3];
red_data[2][1]=(uint32_t)lvjiantou[i+24][0]<<24|(uint32_t)lvjiantou[i+24][1]<<16|(uint32_t)lvjiantou[i+16][0]<<8|(uint32_t)lvjiantou[i+16][1];
red_data[3][1]=(uint32_t)lvjiantou[i+24][2]<<24|(uint32_t)lvjiantou[i+24][3]<<16|(uint32_t)lvjiantou[i+16][2]<<8|(uint32_t)lvjiantou[i+16][3];
	if(i==0)
	{
red_data[2][0]&=0xffff0000;
red_data[3][0]&=0xffff0000;
	}
}
else{
hongchalvjiantou_flag=1;
red_data[2][0]=(uint32_t)hongcha[i+8][0]<<24|(uint32_t)hongcha[i+8][1]<<16|(uint32_t)hongcha[i][0]<<8|(uint32_t)hongcha[i][1];
red_data[3][0]=(uint32_t)hongcha[i+8][2]<<24|(uint32_t)hongcha[i+8][3]<<16|(uint32_t)hongcha[i][2]<<8|(uint32_t)hongcha[i][3];
red_data[2][1]=(uint32_t)hongcha[i+24][0]<<24|(uint32_t)hongcha[i+24][1]<<16|(uint32_t)hongcha[i+16][0]<<8|(uint32_t)hongcha[i+16][1];
red_data[3][1]=(uint32_t)hongcha[i+24][2]<<24|(uint32_t)hongcha[i+24][3]<<16|(uint32_t)hongcha[i+16][2]<<8|(uint32_t)hongcha[i+16][3];
}
	
	OSSemPend(ZMSem,0,&err);//请求信号量			
	red_data[0][2]=(uint32_t)zm[i+8][0]<<24|(uint32_t)zm[i+8][1]<<16|(uint32_t)zm[i][0]<<8|(uint32_t)zm[i][1];
	red_data[1][2]=(uint32_t)zm[i+8][2]<<24|(uint32_t)zm[i+8][3]<<16|(uint32_t)zm[i][2]<<8|(uint32_t)zm[i][3];
	red_data[2][2]=(uint32_t)zm[i+8][4]<<24|(uint32_t)zm[i+8][5]<<16|(uint32_t)zm[i][4]<<8|(uint32_t)zm[i][5];
	red_data[3][2]=(uint32_t)zm[i+8][6]<<24|(uint32_t)zm[i+8][7]<<16|(uint32_t)zm[i][6]<<8|(uint32_t)zm[i][7];
	red_data[4][2]=(uint32_t)zm[i+8][8]<<24|(uint32_t)zm[i+8][9]<<16|(uint32_t)zm[i][8]<<8|(uint32_t)zm[i][9];
	red_data[5][2]=(uint32_t)zm[i+8][10]<<24|(uint32_t)zm[i+8][11]<<16|(uint32_t)zm[i][10]<<8|(uint32_t)zm[i][11];

	red_data[0][3]=(uint32_t)zm[i+24][0]<<24|(uint32_t)zm[i+24][1]<<16|(uint32_t)zm[i+16][0]<<8|(uint32_t)zm[i+16][1];
	red_data[1][3]=(uint32_t)zm[i+24][2]<<24|(uint32_t)zm[i+24][3]<<16|(uint32_t)zm[i+16][2]<<8|(uint32_t)zm[i+16][3];
	red_data[2][3]=(uint32_t)zm[i+24][4]<<24|(uint32_t)zm[i+24][5]<<16|(uint32_t)zm[i+16][4]<<8|(uint32_t)zm[i+16][5];
	red_data[3][3]=(uint32_t)zm[i+24][6]<<24|(uint32_t)zm[i+24][7]<<16|(uint32_t)zm[i+16][6]<<8|(uint32_t)zm[i+16][7];
	red_data[4][3]=(uint32_t)zm[i+24][8]<<24|(uint32_t)zm[i+24][9]<<16|(uint32_t)zm[i+16][8]<<8|(uint32_t)zm[i+16][9];
	red_data[5][3]=(uint32_t)zm[i+24][10]<<24|(uint32_t)zm[i+24][11]<<16|(uint32_t)zm[i+16][10]<<8|(uint32_t)zm[i+16][11];

	red_data[0][4]=(uint32_t)zm[i+40][0]<<24|(uint32_t)zm[i+40][1]<<16|(uint32_t)zm[i+32][0]<<8|(uint32_t)zm[i+32][1];
	red_data[1][4]=(uint32_t)zm[i+40][2]<<24|(uint32_t)zm[i+40][3]<<16|(uint32_t)zm[i+32][2]<<8|(uint32_t)zm[i+32][3];
	red_data[2][4]=(uint32_t)zm[i+40][4]<<24|(uint32_t)zm[i+40][5]<<16|(uint32_t)zm[i+32][4]<<8|(uint32_t)zm[i+32][5];
	red_data[3][4]=(uint32_t)zm[i+40][6]<<24|(uint32_t)zm[i+40][7]<<16|(uint32_t)zm[i+32][6]<<8|(uint32_t)zm[i+32][7];
	red_data[4][4]=(uint32_t)zm[i+40][8]<<24|(uint32_t)zm[i+40][9]<<16|(uint32_t)zm[i+32][8]<<8|(uint32_t)zm[i+32][9];
	red_data[5][4]=(uint32_t)zm[i+40][10]<<24|(uint32_t)zm[i+40][11]<<16|(uint32_t)zm[i+32][10]<<8|(uint32_t)zm[i+32][11];

	red_data[0][5]=(uint32_t)zm[i+56][0]<<24|(uint32_t)zm[i+56][1]<<16|(uint32_t)zm[i+48][0]<<8|(uint32_t)zm[i+48][1];
	red_data[1][5]=(uint32_t)zm[i+56][2]<<24|(uint32_t)zm[i+56][3]<<16|(uint32_t)zm[i+48][2]<<8|(uint32_t)zm[i+48][3];
	red_data[2][5]=(uint32_t)zm[i+56][4]<<24|(uint32_t)zm[i+56][5]<<16|(uint32_t)zm[i+48][4]<<8|(uint32_t)zm[i+48][5];
	red_data[3][5]=(uint32_t)zm[i+56][6]<<24|(uint32_t)zm[i+56][7]<<16|(uint32_t)zm[i+48][6]<<8|(uint32_t)zm[i+48][7];
	red_data[4][5]=(uint32_t)zm[i+56][8]<<24|(uint32_t)zm[i+56][9]<<16|(uint32_t)zm[i+48][8]<<8|(uint32_t)zm[i+48][9];
	red_data[5][5]=(uint32_t)zm[i+56][10]<<24|(uint32_t)zm[i+56][11]<<16|(uint32_t)zm[i+48][10]<<8|(uint32_t)zm[i+48][11];
	
	red_data[0][6]=(uint32_t)zm[i+72][0]<<24|(uint32_t)zm[i+72][1]<<16|(uint32_t)zm[i+64][0]<<8|(uint32_t)zm[i+64][1];
	red_data[1][6]=(uint32_t)zm[i+72][2]<<24|(uint32_t)zm[i+72][3]<<16|(uint32_t)zm[i+64][2]<<8|(uint32_t)zm[i+64][3];
	red_data[2][6]=(uint32_t)zm[i+72][4]<<24|(uint32_t)zm[i+72][5]<<16|(uint32_t)zm[i+64][4]<<8|(uint32_t)zm[i+64][5];
	red_data[3][6]=(uint32_t)zm[i+72][6]<<24|(uint32_t)zm[i+72][7]<<16|(uint32_t)zm[i+64][6]<<8|(uint32_t)zm[i+64][7];
	red_data[4][6]=(uint32_t)zm[i+72][8]<<24|(uint32_t)zm[i+72][9]<<16|(uint32_t)zm[i+64][8]<<8|(uint32_t)zm[i+64][9];
	red_data[5][6]=(uint32_t)zm[i+72][10]<<24|(uint32_t)zm[i+72][11]<<16|(uint32_t)zm[i+64][10]<<8|(uint32_t)zm[i+64][11];
	
	red_data[0][7]=(uint32_t)zm[i+88][0]<<24|(uint32_t)zm[i+88][1]<<16|(uint32_t)zm[i+80][0]<<8|(uint32_t)zm[i+80][1];
	red_data[1][7]=(uint32_t)zm[i+88][2]<<24|(uint32_t)zm[i+88][3]<<16|(uint32_t)zm[i+80][2]<<8|(uint32_t)zm[i+80][3];
	red_data[2][7]=(uint32_t)zm[i+88][4]<<24|(uint32_t)zm[i+88][5]<<16|(uint32_t)zm[i+80][4]<<8|(uint32_t)zm[i+80][5];
	red_data[3][7]=(uint32_t)zm[i+88][6]<<24|(uint32_t)zm[i+88][7]<<16|(uint32_t)zm[i+80][6]<<8|(uint32_t)zm[i+80][7];
	red_data[4][7]=(uint32_t)zm[i+88][8]<<24|(uint32_t)zm[i+88][9]<<16|(uint32_t)zm[i+80][8]<<8|(uint32_t)zm[i+80][9];
	red_data[5][7]=(uint32_t)zm[i+88][10]<<24|(uint32_t)zm[i+88][11]<<16|(uint32_t)zm[i+80][10]<<8|(uint32_t)zm[i+80][11];

	red_data[0][8]=(uint32_t)zm[i+104][0]<<24|(uint32_t)zm[i+104][1]<<16|(uint32_t)zm[i+96][0]<<8|(uint32_t)zm[i+96][1];
	red_data[1][8]=(uint32_t)zm[i+104][2]<<24|(uint32_t)zm[i+104][3]<<16|(uint32_t)zm[i+96][2]<<8|(uint32_t)zm[i+96][3];
	red_data[2][8]=(uint32_t)zm[i+104][4]<<24|(uint32_t)zm[i+104][5]<<16|(uint32_t)zm[i+96][4]<<8|(uint32_t)zm[i+96][5];
	red_data[3][8]=(uint32_t)zm[i+104][6]<<24|(uint32_t)zm[i+104][7]<<16|(uint32_t)zm[i+96][6]<<8|(uint32_t)zm[i+96][7];
	red_data[4][8]=(uint32_t)zm[i+104][8]<<24|(uint32_t)zm[i+104][9]<<16|(uint32_t)zm[i+96][8]<<8|(uint32_t)zm[i+96][9];
	red_data[5][8]=(uint32_t)zm[i+104][10]<<24|(uint32_t)zm[i+104][11]<<16|(uint32_t)zm[i+96][10]<<8|(uint32_t)zm[i+96][11];

	red_data[0][9]=(uint32_t)zm[i+120][0]<<24|(uint32_t)zm[i+120][1]<<16|(uint32_t)zm[i+112][0]<<8|(uint32_t)zm[i+112][1];
	red_data[1][9]=(uint32_t)zm[i+120][2]<<24|(uint32_t)zm[i+120][3]<<16|(uint32_t)zm[i+112][2]<<8|(uint32_t)zm[i+112][3];
	red_data[2][9]=(uint32_t)zm[i+120][4]<<24|(uint32_t)zm[i+120][5]<<16|(uint32_t)zm[i+112][4]<<8|(uint32_t)zm[i+112][5];
	red_data[3][9]=(uint32_t)zm[i+120][6]<<24|(uint32_t)zm[i+120][7]<<16|(uint32_t)zm[i+112][6]<<8|(uint32_t)zm[i+112][7];
	red_data[4][9]=(uint32_t)zm[i+120][8]<<24|(uint32_t)zm[i+120][9]<<16|(uint32_t)zm[i+112][8]<<8|(uint32_t)zm[i+112][9];
	red_data[5][9]=(uint32_t)zm[i+120][10]<<24|(uint32_t)zm[i+120][11]<<16|(uint32_t)zm[i+112][10]<<8|(uint32_t)zm[i+112][11];

	red_data[0][10]=(uint32_t)zm[i+136][0]<<24|(uint32_t)zm[i+136][1]<<16|(uint32_t)zm[i+128][0]<<8|(uint32_t)zm[i+128][1];
	red_data[1][10]=(uint32_t)zm[i+136][2]<<24|(uint32_t)zm[i+136][3]<<16|(uint32_t)zm[i+128][2]<<8|(uint32_t)zm[i+128][3];
	red_data[2][10]=(uint32_t)zm[i+136][4]<<24|(uint32_t)zm[i+136][5]<<16|(uint32_t)zm[i+128][4]<<8|(uint32_t)zm[i+128][5];
	red_data[3][10]=(uint32_t)zm[i+136][6]<<24|(uint32_t)zm[i+136][7]<<16|(uint32_t)zm[i+128][6]<<8|(uint32_t)zm[i+128][7];
	red_data[4][10]=(uint32_t)zm[i+136][8]<<24|(uint32_t)zm[i+136][9]<<16|(uint32_t)zm[i+128][8]<<8|(uint32_t)zm[i+128][9];
	red_data[5][10]=(uint32_t)zm[i+136][10]<<24|(uint32_t)zm[i+136][11]<<16|(uint32_t)zm[i+128][10]<<8|(uint32_t)zm[i+128][11];

	red_data[0][11]=(uint32_t)zm[i+152][0]<<24|(uint32_t)zm[i+152][1]<<16|(uint32_t)zm[i+144][0]<<8|(uint32_t)zm[i+144][1];
	red_data[1][11]=(uint32_t)zm[i+152][2]<<24|(uint32_t)zm[i+152][3]<<16|(uint32_t)zm[i+144][2]<<8|(uint32_t)zm[i+144][3];
	red_data[2][11]=(uint32_t)zm[i+152][4]<<24|(uint32_t)zm[i+152][5]<<16|(uint32_t)zm[i+144][4]<<8|(uint32_t)zm[i+144][5];
	red_data[3][11]=(uint32_t)zm[i+152][6]<<24|(uint32_t)zm[i+152][7]<<16|(uint32_t)zm[i+144][6]<<8|(uint32_t)zm[i+144][7];
	red_data[4][11]=(uint32_t)zm[i+152][8]<<24|(uint32_t)zm[i+152][9]<<16|(uint32_t)zm[i+144][8]<<8|(uint32_t)zm[i+144][9];
	red_data[5][11]=(uint32_t)zm[i+152][10]<<24|(uint32_t)zm[i+152][11]<<16|(uint32_t)zm[i+144][10]<<8|(uint32_t)zm[i+144][11];
	OSSemPost(ZMSem);//释放信号量	
	WriteR_Data(red_data[0]);
	WriteR_Data(red_data[1]);
	WriteR_Data(red_data[2]);
	WriteR_Data(red_data[3]);
	WriteR_Data(red_data[4]);
	WriteR_Data(red_data[5]);
}

unsigned char hongchalvjiantou_flag=0;
/*
 red_data[12] 0是R1的前32个数据 1是R2的前32个数据 以此类推
 */
void WriteR_Data(uint32_t red_data[])
{
		uint8_t i;
		for(i=0;i<32;i++)
		{
			SCPII=0;
//			if(hongchalvjiantou_flag==1){
//			DRX_Send(red_data[0],1,i);
//			DRX_Send(red_data[1],2,i);
//			DRX_Send(red_data[2],3,i);
//			DRX_Send(red_data[3],4,i);
//			DRX_Send(red_data[4],5,i);
//			DRX_Send(red_data[5],6,i);
//			DRX_Send(red_data[6],7,i);
//			DRX_Send(red_data[7],8,i);
//			DRX_Send(red_data[8],9,i);
//			DRX_Send(red_data[9],10,i);
//			DRX_Send(red_data[10],11,i);
//			DRX_Send(red_data[11],12,i);	
//			}
//			else{
//			DGX_Send(red_data[0],1,i);
//			DGX_Send(red_data[1],2,i);
//			DGX_Send(red_data[2],3,i);
//			DGX_Send(red_data[3],4,i);
//			DGX_Send(red_data[4],5,i);
//			DGX_Send(red_data[5],6,i);
//			DGX_Send(red_data[6],7,i);
//			DGX_Send(red_data[7],8,i);
//			DGX_Send(red_data[8],9,i);
//			DGX_Send(red_data[9],10,i);
//			DGX_Send(red_data[10],11,i);
//			DGX_Send(red_data[11],12,i);	
//			}
			if(hongchalvjiantou_flag==1){
			DRX_Send(red_data[0],1,i);
			DRX_Send(red_data[1],2,i);
			}
			else{
			DGX_Send(red_data[0],1,i);
			DGX_Send(red_data[1],2,i);
			}
			
			DGX_Send(red_data[2],3,i);
			DGX_Send(red_data[3],4,i);
			DGX_Send(red_data[4],5,i);
			DGX_Send(red_data[5],6,i);
			DGX_Send(red_data[6],7,i);
			DGX_Send(red_data[7],8,i);
			DGX_Send(red_data[8],9,i);
			DGX_Send(red_data[9],10,i);
			DGX_Send(red_data[10],11,i);
			DGX_Send(red_data[11],12,i);	
			SCPII=1;
			
			uint8_t k=4;
			while(k--)
			{
				for(int o=10;o>0;o--);
			}
	}
}

/*
 ged_data[12] 0是R1的前32个数据 1是R2的前32个数据 以此类推
 */
void WriteG_Data(uint32_t red_data[])
{
		uint8_t i;
		for(i=0;i<32;i++)
		{
			SCPII=0;				
			DGX_Send(red_data[0],1,i);
			DGX_Send(red_data[1],2,i);
			DGX_Send(red_data[2],3,i);
			DGX_Send(red_data[3],4,i);
			DGX_Send(red_data[4],5,i);
			DGX_Send(red_data[5],6,i);
			DGX_Send(red_data[6],7,i);
			DGX_Send(red_data[7],8,i);
			DGX_Send(red_data[8],9,i);
			DGX_Send(red_data[9],10,i);
			DGX_Send(red_data[10],11,i);
			DGX_Send(red_data[11],12,i);			

			SCPII=1;
			uint8_t k=10;
			while(k--)
			{
				for(int o=8;o>0;o--);
			}
	}
}
/*
 bed_data[12] 0是R1的前32个数据 1是R2的前32个数据 以此类推
 */
void WriteB_Data(uint32_t red_data[])
{
		uint8_t i;
		for(i=0;i<32;i++)
		{
			SCPII=0;				
			
			DBX_Send(red_data[0],1,i);
			DBX_Send(red_data[1],2,i);
			DBX_Send(red_data[2],3,i);
			DBX_Send(red_data[3],4,i);
			DBX_Send(red_data[4],5,i);
			DBX_Send(red_data[5],6,i);
			DBX_Send(red_data[6],7,i);
			DBX_Send(red_data[7],8,i);
			DBX_Send(red_data[8],9,i);
			DBX_Send(red_data[9],10,i);
			DBX_Send(red_data[10],11,i);
			DBX_Send(red_data[11],12,i);	
			SCPII=1;
			uint8_t k=10;
			while(k--)
			{
				for(int o=8;o>0;o--);
			}
	}
}
///////////////////////////////////////////////////////////////////////////////
uint8_t  soundmode     = 0;   //语音播报播放类型：1价格，2语音命令
USARTSEND usart3_send;
MONEY money={0,0};            //用于存放金额
/**
  ******************************************************************************
  * 函数名 : TASK_SoundPlay()
  * 描述   : 语音播放任务
  * 参数   : 无
  * 返回值 : 无
  ******************************************************************************
**/
void SOUND2_Play(uint8_t str,uint8_t *pdata)
{
	uint8_t i=0;
	uint8_t changeflag = 0;//金额非整数标志位
	if(str == 'M')
	{
		money.fen=0;money.jiao=0;money.integer=0;
		changeflag=0;
		for(i=2;pdata[i]!='}';i++)
		{
			if(pdata[i] == '.')
			{
				changeflag=1;
				continue;
			}
			else
			{
				if(changeflag == 0)
					money.integer = money.integer*10 + pdata[i]-48;
				else
				{
					money.jiao  = pdata[i]  -48;
					if(pdata[i+1] != '}')
						money.fen = pdata[i+1]-48;
					break;
				}	
			}
		}
		soundmode=1;			
	}
	else if(str == 'S')
	{
		soundplay_msg = pdata[2]-48; 
		soundmode=2;
		checkflag = 0;	
	}
}

//语音模块异或校验
u8 BCCheck(u8 *nDataBuf, u8 nLen)//异或检验函数
{
	u8 nSum = 0;
	u8 i = 0;
	for(i=0; i<nLen; i++)
	{
		nSum ^= nDataBuf[i];//异或检验函数
	}
	return nSum;
}
/**
  ******************************************************************************
  * 函数名 : TASK_SoundPlay()
  * 描述   : 语音播放任务
  * 参数   : 无
  * 返回值 : 无
  ******************************************************************************
**/
void SoundPlay(void)
{	
	uint8_t sound_buf[120];
	memset(sound_buf,0,sizeof(sound_buf));
	usart3_send.cnt=0;
	usart3_send.lenth=0;
	usart3_send.pdata = sound_buf;
	SOUND_Synthesis();
	
	printf_usart3(usart3_send.pdata,usart3_send.lenth);
}

uint8_t SOUND_Synthesis(void)
{
	uint8_t num,sound_num = 0;
	uint8_t i=0,j=0,k;
	uint8_t data[20];
	if(soundmode == 1)//报价
	{
		if(money.integer>=10000)
			num = 5;
		else if(money.integer>=1000)
			num = 4;
		else if(money.integer>=100)
			num = 3;
		else if(money.integer>=10)
			num = 2;
		else if(money.integer>0)
			num = 1;
		else
			num = 0;

			data[j++]=0x05;//已交费
				
		while(i<num)
		{
			switch(num-i)
			{
				case 1://元
					if((money.integer%10)!=0)
						data[j++]=money.integer%10+0x0a;
					break;
				case 2://十
					if(money.integer/10%10!=0)
					{
						data[j++]=money.integer/10%10+0x0a;
						data[j++] = 0x14;
					}
					else if(money.integer%100!=0&&data[j-1]!=0x0a)
						data[j++]=0x0a;
					break;
				case 3://百
					if(money.integer/100%10!=0)
					{
						data[j++]=money.integer/100%10+0x0a;
						data[j++] = 0x15;
					}
					else if(money.integer%1000!=0&&data[j-1]!=0x0a)
						data[j++]=0x0a;
					break;
				case 4://千
					if(money.integer/1000%10!=0)
					{
						data[j++]=money.integer/1000%10+0x0a;
						data[j++] = 0x16;
					}
					else if(money.integer%10000!=0&&data[j-1]!=0x0a)
						data[j++]=0x0a;
					break;
				case 5://万
					data[j++]=money.integer/10000%10+0x0a;
					data[j++] = 0x17;
					break;
			}
			i++;
		}
		data[j++] = 0x09;
		if(money.jiao != 0 && num <4)
		{
			data[j++] = money.jiao+0x0a;
			data[j++] = 8;//角
		}
		if(money.fen != 0 && num <3)
		{
			data[j++] = money.fen+0x0a;
			data[j++] = 7;//分				
		}
		i=0;k=0;
		while(i<j)
		{
			usart3_send.pdata[k++]=0x7e;
			usart3_send.pdata[k++]=0x05;
			usart3_send.pdata[k++]=0x41;
			usart3_send.pdata[k++]=0x00;
			usart3_send.pdata[k++]=data[i];
			usart3_send.pdata[k++]=BCCheck(&(usart3_send.pdata[7*i+1]),usart3_send.pdata[7*i+1]-1);
			usart3_send.pdata[k++]=0xef;
			sound_num++;
			i++;
		}
		usart3_send.lenth=k;		
	}
	else if(soundmode == 2)//指定语音
	{
		usart3_send.lenth=7;
		usart3_send.pdata[0]=0x7e;
		usart3_send.pdata[1]=0x05;
		usart3_send.pdata[2]=0x41;
		usart3_send.pdata[3]=0x00;
		usart3_send.pdata[4]=soundplay_msg;
		usart3_send.pdata[5]=BCCheck(&usart3_send.pdata[1],usart3_send.pdata[1]-1);
		usart3_send.pdata[6]=0xef;
		sound_num = 1;
	}
	else if(soundmode ==3)
	{
		usart3_send.lenth=5;
		usart3_send.pdata[0]=0x7e;
		usart3_send.pdata[1]=0x03;
		usart3_send.pdata[2]=0x0E;
		usart3_send.pdata[3]=0x0D;
		usart3_send.pdata[4]=0xef;
		sound_num = 1;
	}
	soundmode = 0;
	return sound_num;
}
/////////////////////////////////////////////////////////////////////
/***************************************************
	函 数 名：Clean_All
	函数功能：清空除去指示灯所在行之外的行字模
	传    参：无
	返 回 值：无
***************************************************/
void Clean_All(void) 
{
	uint8_t i;
	memset(display_data.pdata,0,512);
	display_data.len=0;
		for(i=0;i<11;i++)
		{
			Clean_Hang2(i);
		}
}

void Clean_Hang2(uint8_t hang)
{
	uint8_t i=0,j=0;
	
		for(i=((hang-1)*16);i<(hang*16);i++)
		{
		for	(j=0;j<((SCREEN_WIDTH/8)+30);j++)
			zm[i][j]=0x00;
		}
}

void CHECK_Zijian_S(void)
{
	display_data.hangnum = 0;
	memcpy(display_data.pdata,"郑州汉威光电一行",16);
	display_data.len = 16;
	FrameDataLoad();	

	display_data.hangnum = 1;
	memcpy(display_data.pdata,"郑州汉威光电二行",16);
	display_data.len = 16;
	FrameDataLoad();	

	display_data.hangnum = 2;
	memcpy(display_data.pdata,"郑州汉威光电三行",16);
	display_data.len = 16;
	FrameDataLoad();	

	display_data.hangnum = 3;
	memcpy(display_data.pdata,"郑州汉威光电四行",16);
	display_data.len = 16;
	FrameDataLoad();	

	display_data.hangnum = 4;
	memcpy(display_data.pdata,"郑州汉威光电五行",16);
	display_data.len = 16;
	FrameDataLoad();	

//	display_data.hangnum = 5;
//	memcpy(display_data.pdata,"汉威光电六行",12);
//	display_data.len = 12;
//	FrameDataLoad();	

//	display_data.hangnum = 6;
//	memcpy(display_data.pdata,"汉威光电七行",12);
//	display_data.len = 12;
//	FrameDataLoad();	

//	display_data.hangnum = 7;
//	memcpy(display_data.pdata,"汉威光电八行",12);
//	display_data.len = 12;
//	FrameDataLoad();	

//	display_data.hangnum = 8;
//	memcpy(display_data.pdata,"汉威光电九行",12);
//	display_data.len = 12;
//	FrameDataLoad();	

//	display_data.hangnum = 9;
//	memcpy(display_data.pdata,"汉威光电十行",12);
//	display_data.len = 12;
//	FrameDataLoad();	
}

void CHECK_Jiance_U(uint8_t *pdata)
{
	uint8_t i=0;
	for(i=0;pdata[2+i]!='}';i++)
		display_data.pdata[i]=pdata[2+i];
	display_data.len = i;
	if(display_data.len > 4)
		display_data.len = 4;
	checkflag = 1;
}

void CHECK_Jiance_Send(uint8_t num)
{
	uint8_t i,temp,j;	
		temp=0;
	if(zm_data.HZorZF == HZ)
	{
		for(i=0;i<11;i++)
		{
			Clean_Hang2(i);
		}
		for(i=temp;i<10;i++)
		{
			ZM_Updata(i,0,zm_data.HZ_Data);
			ZM_Updata(i,2,zm_data.HZ_Data);
			ZM_Updata(i,4,zm_data.HZ_Data);
			ZM_Updata(i,6,zm_data.HZ_Data);
			ZM_Updata(i,8,zm_data.HZ_Data);
			ZM_Updata(i,10,zm_data.HZ_Data);
		}	
	}
	if(zm_data.HZorZF == ZF)
	{
		for(i=0;i<11;i++)
		{
			Clean_Hang2(i);
		}
		for(i=0;i<10;i++)
		{
			for(j=0;j<6;j++)//根据屏幕大小以及字体大小更改
			{
				ZM_Updata(i,2*j,zm_data.ZF_Data);
				ZM_Updata(i,2*j+1,zm_data.ZF_Data1);
			}
		}				
	}
}

void CHOOSE_BPS_Y(void)
{

display_data.hangnum = 0;
memcpy(display_data.pdata,"汉威光电",8);
display_data.len = 8;
FrameDataLoad();	

display_data.hangnum = 1;
memcpy(display_data.pdata,"波特率",6);
display_data.len = 6;
FrameDataLoad();	

display_data.hangnum = 2;
memcpy(display_data.pdata,"    9600",8);
display_data.len = 8;
FrameDataLoad();	

display_data.hangnum = 3;
memcpy(display_data.pdata,"版本:V1.0.0",11);
display_data.len = 11;
FrameDataLoad();	
}


/*
****************************************
	函数名：		KEY_Mode()
	函数功能：	按键功能处理函数
****************************************
*/
void KEY_Mode(uint8_t keyvalue)
{
	static StrFrameHeader *frame_head;
	INT8U *memp_buf;//报文数据
	INT8U err;
	switch(keyvalue)
	{
	 case 1:
			memp_buf = memp_malloc(MEMP_PBUF_POOL);;//申请一块内存
			frame_head = (StrFrameHeader *)memp_buf;
			frame_head->pdata = memp_buf + sizeof(StrFrameHeader);
			frame_head->type = HEAD_KEY_TYPE;
			frame_head->len = 0;
			memcpy(frame_head->pdata,"{[S]}",5);
			frame_head->len = 5;
			do
			{
				err = OSQPost(En_Dis_CodeQeue,(void *)frame_head);//把拷贝的数据发送给加密解密任务
			}
				while(err != OS_ERR_NONE);
			break;
		case 2:
			memp_buf = memp_malloc(MEMP_PBUF_POOL);;//申请一块内存
			frame_head = (StrFrameHeader *)memp_buf;
			frame_head->pdata = memp_buf + sizeof(StrFrameHeader);
			frame_head->type = HEAD_KEY_TYPE;
			frame_head->len = 0;
			memcpy(frame_head->pdata,"{[S]4}",6);
			frame_head->len = 6;
			do
			{
				err = OSQPost(En_Dis_CodeQeue,(void *)frame_head);//把拷贝的数据发送给加密解密任务
			}
				while(err != OS_ERR_NONE);
			break;
		case 3:
			memp_buf = memp_malloc(MEMP_PBUF_POOL);;//申请一块内存
			frame_head = (StrFrameHeader *)memp_buf;
			frame_head->pdata = memp_buf + sizeof(StrFrameHeader);
			frame_head->type = HEAD_KEY_TYPE;
			frame_head->len = 0;
			memcpy(frame_head->pdata,"{[Y]}",5);
			frame_head->len = 5;
			do
			{
				err = OSQPost(En_Dis_CodeQeue,(void *)frame_head);//把拷贝的数据发送给加密解密任务
			}
				while(err != OS_ERR_NONE);
			break;
	case 4:
			memp_buf = memp_malloc(MEMP_PBUF_POOL);;//申请一块内存
			frame_head = (StrFrameHeader *)memp_buf;
			frame_head->pdata = memp_buf + sizeof(StrFrameHeader);
			frame_head->type = HEAD_KEY_TYPE;
			frame_head->len = 0;
			memcpy(frame_head->pdata,"{[U]■}",7);
			frame_head->len = 7;
			do
			{
				err = OSQPost(En_Dis_CodeQeue,(void *)frame_head);//把拷贝的数据发送给加密解密任务
			}
				while(err != OS_ERR_NONE);
			break;
		case 5:
			memp_buf = memp_malloc(MEMP_PBUF_POOL);;//申请一块内存
			frame_head = (StrFrameHeader *)memp_buf;
			memcpy(display_data.pdata,"/0",512);
			frame_head->pdata = memp_buf + sizeof(StrFrameHeader);
			frame_head->type = HEAD_KEY_TYPE;
			frame_head->len = 0;
			memcpy(frame_head->pdata,"{[1]蓝豫A12345  车型:1      }<10>{[2]收费:0.00元             }<10>{[3]蓝豫AD54321 车型:2      }<10>{[4]收费:0.00元             }<10>{[5]欢迎通行高速公路        }<10>",165);
			frame_head->len = 165;
			do
			{
				err = OSQPost(En_Dis_CodeQeue,(void *)frame_head);//把拷贝的数据发送给加密解密任务
			}
				while(err != OS_ERR_NONE);
			break;
		case 6:
			memp_buf = memp_malloc(MEMP_PBUF_POOL);;//申请一块内存
			frame_head = (StrFrameHeader *)memp_buf;
			memcpy(display_data.pdata,"/0",512);
			frame_head->pdata = memp_buf + sizeof(StrFrameHeader);
			frame_head->type = HEAD_KEY_TYPE;
			frame_head->len = 0;
			memcpy(frame_head->pdata,"{[C]}",5);
			frame_head->len = 5;
			do
			{
				err = OSQPost(En_Dis_CodeQeue,(void *)frame_head);//把拷贝的数据发送给加密解密任务
			}
				while(err != OS_ERR_NONE);
			break;
	}
}
//消除字体旁边阴影影响
void Send_0(uint8_t k)
{
	uint32_t red_data[6][12]={0};
	WriteR_Data(red_data[0]);
	WriteR_Data(red_data[1]);
	WriteR_Data(red_data[2]);
	WriteR_Data(red_data[3]);
	WriteR_Data(red_data[4]);
	WriteR_Data(red_data[5]);	
}
