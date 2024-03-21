#ifndef _SW_LED_H
#define _SW_LED_H

#include "stm32f4xx.h"
#include "sys.h"



//////////////////////////////LED输出引脚//////////////////////////////////////////////////////

#define RD0    	PHout(10)	  
#define RD1    	PBout(13)	  
#define RD2    	PDout( 9)	  
#define RD3    	PDout(13)	  
#define RD4    	PGout( 3)	  
#define RD5    	PGout( 7)	  
#define RD6    	PCout( 8)	  
#define RD7    	PAout(10)	  

#define GD0    	PHout(11)	  
#define GD1    	PBout(14)	  
#define GD2    	PDout(10)	  
#define GD3    	PDout(14)	  
#define GD4    	PGout( 4)	  
#define GD5    	PGout( 8)	  
#define GD6    	PCout( 9)	  
#define GD7    	PAout(11)	

#define BD0    	PHout(12)	  
#define BD1    	PBout(15)	  
#define BD2    	PDout(11)	  
#define BD3    	PDout(15)	  
#define BD4    	PGout( 5)	  
#define BD5    	PCout( 6)	  
#define BD6    	PAout( 8)	  
#define BD7    	PAout(12)	


#define CKA    	PIout( 4)	    //时钟
#define STB    	PIout( 5)			//锁存
//数据data取出第i位发送至第x根数据线
#define DRX_Send(data,x,i) if((data&(0x80000000>>i)) !=0){DRX(x,1);}else {DRX(x,0);}
#define DGX_Send(data,x,i) if((data&(0x80000000>>i)) !=0){DGX(x,1);}else {DGX(x,0);}
#define DBX_Send(data,x,i) if((data&(0x80000000>>i)) !=0){DBX(x,1);}else {DBX(x,0);}

//第x根线发送数据y
#define DRX(x,y) switch(x)\
	  {	case 1: RD0=y;break;\
			case 2: RD1=y;break;\
			case 3: RD2=y;break;\
			case 4: RD3=y;break;\
			case 5: RD4=y;break;\
			case 6: RD5=y;break;\
			case 7: RD6=y;break;\
			case 8: RD7=y;break;}
//第x根线发送数据y
#define DGX(x,y) switch(x)\
	  {	case 1: GD0=y;break;\
			case 2: GD1=y;break;\
			case 3: GD2=y;break;\
			case 4: GD3=y;break;\
			case 5: GD4=y;break;\
			case 6: GD5=y;break;\
			case 7: GD6=y;break;\
			case 8: GD7=y;break;}	
		
//第x根线发送数据y
#define DBX(x,y) switch(x)\
	  {	case 1: BD0=y;break;\
			case 2: BD1=y;break;\
			case 3: BD2=y;break;\
			case 4: BD3=y;break;\
			case 5: BD4=y;break;\
			case 6: BD5=y;break;\
			case 7: BD6=y;break;\
			case 8: BD7=y;break;}
		
//////////////////////////////LED输入引脚//////////////////////////////////////////////////////
//#define RD8    	PEin( 4)	  
//#define RD9    	PCin(13)	  
//#define RD10	  PIin(10)	  
//#define RD11    PFin( 2)	  
//#define RD12    PFin( 6)	  
//#define RD13    PFin(10)	  
//#define RD14   	PCin( 3)	  
//#define RD15    PHin( 2)	  

//#define GD8    	PEin( 5)	  
//#define GD9    	PCin(14)	  
//#define GD10	  PIin(11)	  
//#define GD11    PFin( 3)	  
//#define GD12    PFin( 7)	  
//#define GD13    PCin( 0)	  
//#define GD14   	PAin( 0)	  
//#define GD15    PHin( 3)		

//#define BD8    	PEin( 6)	  
//#define BD9    	PCin(15)	  
//#define BD10	  PFin( 0)	  
//#define BD11    PFin( 4)	  
//#define BD12    PFin( 8)	  
//#define BD13    PCin( 1)	  
//#define BD14   	PAin( 1)	  
//#define BD15    PHin( 4)	



#define R0_IN		PCin( 0)	
#define G0_IN		PCin( 1)	
#define B0_IN		PCin( 2)	
#define R1_IN		PFin( 8)	
#define G1_IN		PFin( 9)	
#define B1_IN		PFin(10)	
#define R2_IN		PFin( 5)	
#define G2_IN		PFin( 6)
#define B2_IN		PFin( 7)
#define R3_IN		PFin( 2)
#define G3_IN		PFin( 3)
#define B3_IN		PFin( 4)
#define R4_IN		PIin(11)
#define G4_IN		PFin( 0)
#define B4_IN		PFin( 1)
#define R5_IN		PCin(15)
#define G5_IN		PIin( 9)
#define B5_IN		PIin(10)
#define R6_IN		PIin( 8)
#define G6_IN		PCin(13)
#define B6_IN		PCin(14)
#define R7_IN		PEin(4)
#define G7_IN		PEin(5)
#define B7_IN		PEin(6)

extern uint8_t open_data[3][32][4];


extern uint8_t Read_IDSlave (void);
extern void Sw_Led_Out_Init (void);
extern void Sw_Led_Int_Init (void);
extern void SendOneUnitData_8X8 (uint32_t r_data[],uint32_t g_data[],uint32_t b_data[]);
extern void All_Sdo_Out(uint8_t value);
extern void SetShowArea (void);
extern void LedDisplay (void);
extern void SendOneUnitData (uint8_t hangunit);//上下，左右
void Open_Detection(void);
#endif
