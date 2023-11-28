#ifndef _LCD_DRIVER_H_
#define _LCD_DRIVER_H_

#include <STC8.H>

//#include "config.h"
#include "font.h"


#define u16 unsigned int
#define u8 unsigned char

extern u16 LCD_Color;
extern u16 LCD_BackColor;


#define delay_ms Delay_ms

#define USE_HORIZONTAL 1  //设置横屏或者竖屏显示 0或1为竖屏 2或3为横屏


#if USE_HORIZONTAL==0||USE_HORIZONTAL==1
#define LCD_W 240
#define LCD_H 240
#else
#define LCD_W 240
#define LCD_H 240
#endif

sbit LCD_RST=P2^0;
sbit LCD_BLK=P4^4;
sbit LCD_SCL=P4^3;
sbit LCD_DC=P4^2;
sbit LCD_CS=P4^1;
sbit LCD_SDA=P4^0;

#define OLED_RST_Clr()  (LCD_RST=0)//RES
#define OLED_RST_Set()  (LCD_RST=1)

#define OLED_CS_Clr()  (LCD_CS=0)//CS
#define OLED_CS_Set()  (LCD_CS=1)

#define OLED_SCLK_Clr() (LCD_SCL=0)//SCL
#define OLED_SCLK_Set() (LCD_SCL=1)

#define OLED_DC_Clr()   (LCD_DC=0)//DC
#define OLED_DC_Set()   (LCD_DC=1)

#define OLED_SDIN_Clr() (LCD_SDA=0)//DIN
#define OLED_SDIN_Set() (LCD_SDA=1)

#define OLED_BLK_Clr()  (LCD_BLK=0)//BLK
#define OLED_BLK_Set()  (LCD_BLK=1)


//颜色
#define WHITE         	 0xFFFF
#define BLACK         	 0x0000	  
#define BLUE           	 0x001F  
#define BRED             0XF81F
#define GRED 			       0XFFE0
#define GBLUE			       0X07FF
#define RED           	 0xF800
#define MAGENTA       	 0xF81F
#define GREEN         	 0x07E0
#define CYAN          	 0x7FFF
#define YELLOW        	 0xFFE0

#define BROWN 			     0XBC40 //棕色
#define BRRED 			     0XFC07 //棕红色
#define GRAY  			     0X8430 //灰色
#define DARKBLUE      	 0X01CF	//深蓝色
#define LIGHTBLUE      	 0X7D7C	//浅蓝色  
#define GRAYBLUE       	 0X5458 //灰蓝色 
#define LIGHTGREEN     	 0X841F //浅绿色
#define LGRAY 			     0XC618 //浅灰色(PANNEL),窗体背景色
#define LGRAYBLUE        0XA651 //浅灰蓝色(中间层颜色)
#define LBBLUE           0X2B12 //浅棕蓝色(选择条目的反色)

void LCD_Writ_Bus(u8 dat);
void LCD_WR_DATA8(u8 dat);
void LCD_WR_DATA(u16 dat);
void LCD_WR_REG(u8 dat);
void LCD_Address_Set(u16 x1,u16 y1,u16 x2,u16 y2);

void LCD_Config(void);
void LCD_GPIOConfig(void);
void LCD_Init(void);
void LCD_Clear(u16 Color);
void LCD_ClearRect(u8 x_start,u8 y_start,u8 width,u8 high,u16 color);
//void LCD_ShowxXx(u16 x,u16 y,u16 width,u16 high,char *p);
void LCD_ShowString(u16 x, u16 y, u16 fc, u16 bc, u8 *p);
void LCD_ShowPicture(uint16_t col,uint16_t page,uint16_t width,uint16_t high,uint8_t *p);
void LCD_ShowSmg(u16 x, u16 y, u16 fc, u16 bc, u8 *p);
#endif
