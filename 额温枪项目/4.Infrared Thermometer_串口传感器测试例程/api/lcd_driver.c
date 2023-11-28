#include "lcd_driver.h"
#include "tim.h"

u16 LCD_Color = BLACK;
u16 LCD_BackColor = WHITE;

/******************************************************************************
      函数说明：LCD串行数据写入函数
      入口数据：dat  要写入的串行数据
      返回值：  无
******************************************************************************/
void LCD_Writ_Bus(u8 dat) 
{	
	u8 i;			  
	for(i=0;i<8;i++)
	{			  
		OLED_SCLK_Clr();
		if(dat&0x80)
		   OLED_SDIN_Set();
		else 
		   OLED_SDIN_Clr();
		OLED_SCLK_Set();
		dat<<=1;   
	}			
}


/******************************************************************************
      函数说明：LCD写入数据
      入口数据：dat 写入的数据
      返回值：  无
******************************************************************************/
void LCD_WR_DATA8(u8 dat)
{
	OLED_DC_Set();//写数据
	LCD_Writ_Bus(dat);
}


/******************************************************************************
      函数说明：LCD写入数据
      入口数据：dat 写入的数据
      返回值：  无
******************************************************************************/
void LCD_WR_DATA(u16 dat)
{
	OLED_DC_Set();//写数据
	LCD_Writ_Bus(dat>>8);
	LCD_Writ_Bus(dat);
}


/******************************************************************************
      函数说明：LCD写入命令
      入口数据：dat 写入的命令
      返回值：  无
******************************************************************************/
void LCD_WR_REG(u8 dat)
{
	OLED_DC_Clr();//写命令
	LCD_Writ_Bus(dat);
}


void LCD_GPIOConfig(void)
{
	//管脚全部拉高，释放总线
	OLED_SCLK_Set();OLED_SDIN_Set();OLED_RST_Set();
	OLED_DC_Set();OLED_CS_Clr();
	//关闭背光
	OLED_BLK_Clr();
}

void LCD_Config(void)
{
	OLED_RST_Clr();
	delay_ms(200);
	OLED_RST_Set();
	delay_ms(20);
	OLED_BLK_Set();
	
	//************* Start Initial Sequence **********// 
	LCD_WR_REG(0x36);
	if(USE_HORIZONTAL==0)LCD_WR_DATA8(0x00);
	else if(USE_HORIZONTAL==1)LCD_WR_DATA8(0xC0);
	else if(USE_HORIZONTAL==2)LCD_WR_DATA8(0x70);
	else LCD_WR_DATA8(0xA0);

	LCD_WR_REG(0x3A); 
	LCD_WR_DATA8(0x05);
	LCD_WR_REG(0xB2);
	LCD_WR_DATA8(0x0C);
	LCD_WR_DATA8(0x0C);
	LCD_WR_DATA8(0x00);
	LCD_WR_DATA8(0x33);
	LCD_WR_DATA8(0x33); 
	LCD_WR_REG(0xB7); 
	LCD_WR_DATA8(0x35);  
	LCD_WR_REG(0xBB);
	LCD_WR_DATA8(0x19);
	LCD_WR_REG(0xC0);
	LCD_WR_DATA8(0x2C);
	LCD_WR_REG(0xC2);
	LCD_WR_DATA8(0x01);
	LCD_WR_REG(0xC3);
	LCD_WR_DATA8(0x12);   
	LCD_WR_REG(0xC4);
	LCD_WR_DATA8(0x20);  
	LCD_WR_REG(0xC6); 
	LCD_WR_DATA8(0x0F);    
	LCD_WR_REG(0xD0); 
	LCD_WR_DATA8(0xA4);
	LCD_WR_DATA8(0xA1);
	LCD_WR_REG(0xE0);
	LCD_WR_DATA8(0xD0);
	LCD_WR_DATA8(0x04);
	LCD_WR_DATA8(0x0D);
	LCD_WR_DATA8(0x11);
	LCD_WR_DATA8(0x13);
	LCD_WR_DATA8(0x2B);
	LCD_WR_DATA8(0x3F);
	LCD_WR_DATA8(0x54);
	LCD_WR_DATA8(0x4C);
	LCD_WR_DATA8(0x18);
	LCD_WR_DATA8(0x0D);
	LCD_WR_DATA8(0x0B);
	LCD_WR_DATA8(0x1F);
	LCD_WR_DATA8(0x23);
	LCD_WR_REG(0xE1);
	LCD_WR_DATA8(0xD0);
	LCD_WR_DATA8(0x04);
	LCD_WR_DATA8(0x0C);
	LCD_WR_DATA8(0x11);
	LCD_WR_DATA8(0x13);
	LCD_WR_DATA8(0x2C);
	LCD_WR_DATA8(0x3F);
	LCD_WR_DATA8(0x44);
	LCD_WR_DATA8(0x51);
	LCD_WR_DATA8(0x2F);
	LCD_WR_DATA8(0x1F);
	LCD_WR_DATA8(0x1F);
	LCD_WR_DATA8(0x20);
	LCD_WR_DATA8(0x23);
	LCD_WR_REG(0x21); 
	LCD_WR_REG(0x11); 
	LCD_WR_REG(0x29);
}

void LCD_Init(void)
{
	LCD_GPIOConfig();
	LCD_Config();
	LCD_Clear(WHITE);
	//打开背光
	OLED_BLK_Set();
}

/******************************************************************************
      函数说明：设置起始和结束地址
      入口数据：x1,x2 设置列的起始和结束地址
                y1,y2 设置行的起始和结束地址
      返回值：  无
******************************************************************************/
void LCD_Address_Set(u16 x1,u16 y1,u16 x2,u16 y2)
{
	if(USE_HORIZONTAL==1)
	{
		LCD_WR_REG(0x2a);//列地址设置
		LCD_WR_DATA(x1);
		LCD_WR_DATA(x2);
		LCD_WR_REG(0x2b);//行地址设置
		LCD_WR_DATA(y1+80);
		LCD_WR_DATA(y2+80);
		LCD_WR_REG(0x2c);//储存器写
	}
	//把适配代码去掉，省内存
//	if(USE_HORIZONTAL==0)
//	{
//		LCD_WR_REG(0x2a);//列地址设置
//		LCD_WR_DATA(x1);
//		LCD_WR_DATA(x2);
//		LCD_WR_REG(0x2b);//行地址设置
//		LCD_WR_DATA(y1);
//		LCD_WR_DATA(y2);
//		LCD_WR_REG(0x2c);//储存器写
//	}
//	else if(USE_HORIZONTAL==1)
//	{
//		LCD_WR_REG(0x2a);//列地址设置
//		LCD_WR_DATA(x1);
//		LCD_WR_DATA(x2);
//		LCD_WR_REG(0x2b);//行地址设置
//		LCD_WR_DATA(y1+80);
//		LCD_WR_DATA(y2+80);
//		LCD_WR_REG(0x2c);//储存器写
//	}
//	else if(USE_HORIZONTAL==2)
//	{
//		LCD_WR_REG(0x2a);//列地址设置
//		LCD_WR_DATA(x1);
//		LCD_WR_DATA(x2);
//		LCD_WR_REG(0x2b);//行地址设置
//		LCD_WR_DATA(y1);
//		LCD_WR_DATA(y2);
//		LCD_WR_REG(0x2c);//储存器写
//	}
//	else
//	{
//		LCD_WR_REG(0x2a);//列地址设置
//		LCD_WR_DATA(x1+80);
//		LCD_WR_DATA(x2+80);
//		LCD_WR_REG(0x2b);//行地址设置
//		LCD_WR_DATA(y1);
//		LCD_WR_DATA(y2);
//		LCD_WR_REG(0x2c);//储存器写
//	}
}
/******************************************************************************
      函数说明：LCD清屏函数
      入口数据：无
      返回值：  无
******************************************************************************/
void LCD_Clear(u16 Color)
{
	u16 i,j;  	
	LCD_Address_Set(0,0,LCD_W-1,LCD_H-1);
	for(i=0;i<LCD_W;i++)
	{
		for (j=0;j<LCD_H;j++)
		{
			LCD_WR_DATA(Color);	 			 
		}
	}
}



//void LCD_DrewPoint(u16 x,u16 y,u16 color)
//{
//	LCD_Address_Set(x,y,1,1);
//	LCD_WR_DATA(color);	
//}




extern code unsigned char zf_8_16[][16];
void LCD_ShowString(u16 x, u16 y, u16 fc, u16 bc, u8 *p)
{
	u16 i=0, j=0;
	u8 temp = 0;
	while(*p != '\0') {
		LCD_Address_Set(x, y, x+8-1, y+16-1);
		x+=8;
		if(x > (LCD_W-8)) {
			x = 0;
			y +=16;
		} 	
		for(i=0; i<16; i++) {//一个字体字模16字节
			temp = zf_8_16[(*p)-' '][i];
			for(j=0; j<8; j++) {
				if(temp & (0x01<<(7-j)))
					LCD_WR_DATA(fc);
				else 
					LCD_WR_DATA(bc);
			}
		}
		p++;
	}
}


extern code unsigned char smg_32_64[][256];
void LCD_ShowSmg(u16 x, u16 y, u16 fc, u16 bc, u8 *p)
{
	u16 i=0, j=0;
	u8 temp = 0;
	while(*p != '\0') {
		LCD_Address_Set(x, y, x+32-1, y+64-1);
		x+=32;
		if(x > (LCD_W-32)) {
			x = 0;
			y +=64;
		} 	
		for(i=0; i<256; i++) {//一个字体字模256字节
			if(*p >= '.' && *p <= '9')
				temp = smg_32_64[(*p)-'.'][i];
			else
				temp = smg_32_64[12][i];
			for(j=0; j<8; j++) {
				if(temp & (0x01<<(7-j)))
					LCD_WR_DATA(fc);
				else 
					LCD_WR_DATA(bc);
			}
		}
		p++;
	}
}



/*
col -- 开始的列
page - 开始的页
width - 图片的宽度
high -- 图片的高度
p    - 指向要显示的图片数组
*/
void LCD_ShowPicture(uint16_t col,uint16_t page,uint16_t width,uint16_t high,uint8_t *p)
{
	u16 i,j;
  u16 color = 0;	
	LCD_Address_Set(col,page,col+width-1,page+high-1);
	for(i=0;i<width;i++)
	{
		for (j=0;j<high;j++)
		{
			color = *p<<8 | *(p+1);
			LCD_WR_DATA(color);
		  p+=2;
		}
	}
}


void LCD_ClearRect(u8 x_start,u8 y_start,u8 width,u8 high,u16 color)
{
	u16 i,j;  	
	LCD_Address_Set(x_start,y_start,x_start+width-1,y_start+high-1);
	for(i=0;i<width;i++)
	{
		for (j=0;j<high;j++)
		{
			LCD_WR_DATA(color);	 			 
		}
	}
}

