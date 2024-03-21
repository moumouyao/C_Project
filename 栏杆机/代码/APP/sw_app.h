#ifndef __SW_HOST_H__
#define __SW_HOST_H__

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"
//数据data取出第i位发送至第x根数据线
#define DRX_Send(data,x,i) if((data&(0x80000000>>i)) !=0){DRX(x,1);}else {DRX(x,0);}
#define DGX_Send(data,x,i) if((data&(0x80000000>>i)) !=0){DGX(x,1);}else {DGX(x,0);}
#define DBX_Send(data,x,i) if((data&(0x80000000>>i)) !=0){DBX(x,1);}else {DBX(x,0);}

//第x根线发送数据y
#define DRX(x,y) switch(x)\
	  {	case 1: R1=y;break;\
			case 2: R2=y;break;\
			case 3: R3=y;break;\
			case 4: R4=y;break;\
			case 5: R5=y;break;\
			case 6: R6=y;break;\
			case 7: R7=y;break;\
			case 8: R8=y;break;\
			case 9: R9=y;break;\
			case 10: R10=y;break;\
			case 11: R11=y;break;\
			case 12: R12=y;break;}
//第x根线发送数据y
#define DGX(x,y) switch(x)\
	  {	case 1: G1=y;break;\
			case 2: G2=y;break;\
			case 3: G3=y;break;\
			case 4: G4=y;break;\
			case 5: G5=y;break;\
			case 6: G6=y;break;\
			case 7: G7=y;break;\
			case 8: G8=y;break;\
			case 9: G9=y;break;\
			case 10: G10=y;break;\
			case 11: G11=y;break;\
			case 12: G12=y;break;}	
		
//第x根线发送数据y
#define DBX(x,y) switch(x)\
	  {	case 1: B1=y;break;\
			case 2: B2=y;break;\
			case 3: B3=y;break;\
			case 4: B4=y;break;\
			case 5: B5=y;break;\
			case 6: B6=y;break;\
			case 7: B7=y;break;\
			case 8: B8=y;break;\
			case 9: B9=y;break;\
			case 10: B10=y;break;\
			case 11: B11=y;break;\
			case 12: B12=y;break;}
		
			


void led_task(void *pdata);
void TestCode(void *pdata);
void key_task(void *pdata);
void Write_RGB(void *pdata);
void TASK_OverDataHandle(void *pdata);

void Select_OneHang (uint8_t num);
uint8_t DisplayMsg2_Get(uint8_t *pdata);
void FrameDataLoad(void);
void ZM_Updata(uint8_t x,uint8_t y,uint8_t *pdata);

void WriteRGB_Data_8(uint16_t i);	
void WriteR_Data(uint32_t red_data[]);
void WriteG_Data(uint32_t red_data[]);
void WriteB_Data(uint32_t red_data[]);

			
u8 BCCheck(u8 *nDataBuf, u8 nLen);//异或检验函数
void SOUND2_Play(uint8_t str,uint8_t *pdata);
void SoundPlay(void);
uint8_t SOUND_Synthesis(void);


void Clean_Hang2(uint8_t hang);
void Clean_All(void); 
void CHECK_Zijian_S(void);
void CHECK_Jiance_U(uint8_t *pdata);
void CHECK_Jiance_Send(uint8_t num);
void CHOOSE_BPS_Y(void);
void KEY_Mode(uint8_t keyvalue);
void GunDong1(uint8_t hang,uint8_t len);
void GunDong(void);
void Send_0(uint8_t k);

#ifdef __cplusplus
}
#endif


#endif

