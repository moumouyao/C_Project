#include "sw_led.h"
#include "delay.h"
#include "FreeRTOS.h"
#include "task.h"
#include "sw_config.h"
/*
 *
 *��ʼ��ID�����Լ���ȡ�Ӱ�ID
 *
 */
uint8_t Read_IDSlave (void)
{

	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD,ENABLE); //ʹ��GPIOD��ʱ��
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN;//����
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_DOWN;  
	GPIO_Init(GPIOD,&GPIO_InitStructure);
		
	uint16_t i;
	uint8_t c;
	i=(GPIO_ReadInputData(GPIOD))&0x00ff;      //8λID
	c=(uint8_t)i;
	return c;

}
/*
 *
 *HUB50_1�����IO��ʼ��
 *
 */
void Sw_Led_Out_Init (void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE); //ʹ��GPIOA��ʱ��
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_8|GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_OUT;//���
	GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;  //�������
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_NOPULL;  
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_100MHz; //����GPIO
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	GPIO_ResetBits(GPIOA,GPIO_Pin_8|GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12); 

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE); //ʹ��GPIOB��ʱ��
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_OUT;//���
	GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;  //�������
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_NOPULL;  
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_100MHz; //����GPIO
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	GPIO_ResetBits(GPIOB,GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15);

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC,ENABLE); //ʹ��GPIOB��ʱ��
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_6|GPIO_Pin_8|GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_OUT;//���
	GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;  //�������
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_NOPULL;  
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_100MHz; //����GPIO
	GPIO_Init(GPIOC,&GPIO_InitStructure);
	GPIO_ResetBits(GPIOC,GPIO_Pin_6|GPIO_Pin_8|GPIO_Pin_9);

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD,ENABLE);
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_OUT;//���
	GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;  //�������
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_NOPULL;  
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_100MHz; //����GPIO
	GPIO_Init(GPIOD,&GPIO_InitStructure);
	GPIO_ResetBits(GPIOD,GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15); 

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG,ENABLE);
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_7|GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_OUT;//���
	GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;  //�������
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_NOPULL;  
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_100MHz; //����GPIO
	GPIO_Init(GPIOG,&GPIO_InitStructure);
	GPIO_ResetBits(GPIOG,GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_7|GPIO_Pin_8);
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOH,ENABLE);
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_OUT;//���
	GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;  //�������
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_NOPULL;  
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_100MHz; //����GPIO
	GPIO_Init(GPIOH,&GPIO_InitStructure);
	GPIO_ResetBits(GPIOH,GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12);
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOI,ENABLE);
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_4|GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_OUT;//���
	GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;  //�������
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_NOPULL;  
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_100MHz; //����GPIO
	GPIO_Init(GPIOI,&GPIO_InitStructure);
	GPIO_ResetBits(GPIOI,GPIO_Pin_4|GPIO_Pin_5);
}

/*
 *
 *HUB50_2������IO��ʼ��
 *
 */
void Sw_Led_Int_Init (void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC,ENABLE); //ʹ��GPIOC��ʱ��
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN;//���
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_UP;  
	GPIO_Init(GPIOC,&GPIO_InitStructure);

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE,ENABLE); //ʹ��GPIOE��ʱ��
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN;//���
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_UP;  
	GPIO_Init(GPIOE,&GPIO_InitStructure);

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF,ENABLE); //ʹ��GPIOF��ʱ��
	GPIO_InitStructure.GPIO_Pin=0x0800;//PF0 PF1 PF2 PF3 PF4 PF5 PF6 PF7 PF8 PF9 PF10
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN;//���
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_UP;  
	GPIO_Init(GPIOF,&GPIO_InitStructure);
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOI,ENABLE); //ʹ��GPIOI��ʱ��
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN;//���
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_UP;  
	GPIO_Init(GPIOI,&GPIO_InitStructure);
	
}


/*
 *
 *8X8��ģ�����
 *
 */
void SendOneUnitData_8X8 (uint32_t r_data[],uint32_t g_data[],uint32_t b_data[])
{
	for(int j=0;j<BOX_HANG/8;j++)  //8X8��ģ�� 
	{
		for(int i=0;i<32;i++)
		{
			DRX_Send(r_data[0],1,i);
			DRX_Send(r_data[1],2,i);
			DRX_Send(r_data[2],3,i);
			DRX_Send(r_data[3],4,i);
			DRX_Send(r_data[4],5,i);
			DRX_Send(r_data[5],6,i);
			DRX_Send(r_data[6],7,i);
			DRX_Send(r_data[7],8,i);		

			DGX_Send(g_data[0],1,i);
			DGX_Send(g_data[1],2,i);
			DGX_Send(g_data[2],3,i);
			DGX_Send(g_data[3],4,i);
			DGX_Send(g_data[4],5,i);
			DGX_Send(g_data[5],6,i);
			DGX_Send(g_data[6],7,i);
			DGX_Send(g_data[7],8,i);	
			
			DBX_Send(b_data[0],1,i);
			DBX_Send(b_data[1],2,i);
			DBX_Send(b_data[2],3,i);
			DBX_Send(b_data[3],4,i);
			DBX_Send(b_data[4],5,i);
			DBX_Send(b_data[5],6,i);
			DBX_Send(b_data[6],7,i);
			DBX_Send(b_data[7],8,i);	
			CKA=1;
			delay_sys_us(30);						
			CKA=0;
		}
	}
}
/*
 *
 *SDO�������
 *
 */
void All_Sdo_Out(uint8_t value)
{
	RD0=value;
	RD1=value;
	RD2=value;
	RD3=value;
	RD4=value;
	RD5=value;
	RD6=value;
	RD7=value; 

	GD0=value;
	GD1=value;
	GD2=value;
	GD3=value;
	GD4=value;
	GD5=value;
	GD6=value;
	GD7=value; 
	
	BD0=value;
	BD1=value;
	BD2=value;
	BD3=value;
	BD4=value;
	BD5=value;
	BD6=value;
	BD7=value; 
	
	CKA=value;
	STB=value;
}
///*
// *
// *��·���
// *
// */

//uint8_t open_data[32][4]={0};
//void Open_Detection()
//{

//	TIM_Cmd(TIM3,DISABLE); 	//�رն�ʱ��
//	All_Sdo_Out(0);					//ȫ����SDO����
//	
//	taskENTER_CRITICAL(); 	//�����ٽ���
//	
//	//���뿪·����ʱ��
//	delay_sys_us(20);		//��ʱ650ns ���� ʱ�䲻��̫׼ȷ ������
//	STB=1;									//LE����
//	CKA=1;									//CLK����
//	delay_sys_us(20);  //���25ns ���� ʱ�䲻��̫׼ȷ ������
//	CKA=0;									//CLK����
//	STB=0;									//LE����
//	delay_sys_us(20);  //��ʱ650ns���ݲŻᷢ�͵��������� ���� ʱ�䲻��̫׼ȷ ������
//	//���ֵ�һ���������ݵ͵�ƽ�ܶ�  ���Գ��Խ����ݷ��͵���������ʱ���ӳ���δ����
//	
//	//���Խ����ݷ��͵���������ʱ���ӳ�
//	delay_sys_us(20);
//	
//	/*
//	��ȡ����
//	
//	
//	*/
//	
//	
//	for(int i=0;i<64;i++)
//		{
//			CKA=1;
//			delay_sys_us(20);		
//			//��ȡ��������ݣ����ݾ��ǻ�����Ϣ			
//			CKA=0;
//			delay_sys_us(20);	
//		}
//	//CLK��� ʹ�����ݷ��͵�����
//		
//	taskEXIT_CRITICAL(); 		//�˳��ٽ���	
//		
//}



/*
 *
 *��·���
 *
 */

uint8_t open_data[3][32][4]={0xff};

void Open_Detection(void)
{
	
	TIM_Cmd(TIM1,DISABLE); 	//�رն�ʱ��
	All_Sdo_Out(0);					//ȫ����SDO����
	
	taskENTER_CRITICAL(); 	//�����ٽ���
	
	//���뿪·����ʱ��
	delay_sys_us(20);		//��ʱ650ns ���� ʱ�䲻��̫׼ȷ ������
	STB=1;									//LE����
	CKA=1;									//CLK����
	delay_sys_us(20);  //���25ns ���� ʱ�䲻��̫׼ȷ ������
	CKA=0;									//CLK����
	STB=0;									//LE����
	delay_sys_us(20);  //��ʱ650ns���ݲŻᷢ�͵��������� ���� ʱ�䲻��̫׼ȷ ������
	//���ֵ�һ���������ݵ͵�ƽ�ܶ�  ���Գ��Խ����ݷ��͵���������ʱ���ӳ���δ����
	
	//���Խ����ݷ��͵���������ʱ���ӳ�
	delay_sys_us(20);
	
	/*
	��ȡ����
	*/
	
	
//P10��P31.25��һ�� �������Ƽ�λ��һ��	
/*P10 ����
		for(int i=1;i<=8;i++)
		{
			//��ȡ��������ݣ����ݾ��ǻ�����Ϣ	
			data[0]|=R0_IN<<(8-i);
			data[1]|=G0_IN<<(8-i);
			data[2]|=B0_IN<<(8-i);
			CKA=1;
			delay_sys_us(20);		
			CKA=0;
			delay_sys_us(20);	
		}
*/
//P31.25 ��P10�ϲ���������δ��31.25����
for(int hangunit=0;hangunit<ShowAreaStru.HangUint;hangunit++)//ѡ��һ���еĵڼ�����Ԫ��***��*****************************
	{
	//CLK��� ʹ�����ݷ��͵�����
		
		for(int i=0;i<=8;i++)
		{
			//open_data[3][32][4]
			//��ȡ��������ݣ����ݾ��ǻ�����Ϣ	
			open_data[0][1][hangunit]|=R0_IN<<i;
			open_data[1][1][hangunit]|=G0_IN<<i;
			open_data[2][1][hangunit]|=B0_IN<<i;
			open_data[0][5][hangunit]|=R1_IN<<i;
			open_data[1][5][hangunit]|=G1_IN<<i;
			open_data[2][5][hangunit]|=B1_IN<<i;
			
			open_data[0][9][hangunit]|=R2_IN<<i;
			open_data[1][9][hangunit]|=G2_IN<<i;
			open_data[2][9][hangunit]|=B2_IN<<i;
			open_data[0][13][hangunit]|=R3_IN<<i;
			open_data[1][13][hangunit]|=G3_IN<<i;
			open_data[2][13][hangunit]|=B3_IN<<i;
			
			open_data[0][17][hangunit]|=R4_IN<<i;
			open_data[1][17][hangunit]|=G4_IN<<i;
			open_data[2][17][hangunit]|=B4_IN<<i;
			open_data[0][21][hangunit]|=R5_IN<<i;
			open_data[1][21][hangunit]|=G5_IN<<i;
			open_data[2][21][hangunit]|=B5_IN<<i;
			
			open_data[0][25][hangunit]|=R6_IN<<i;
			open_data[1][25][hangunit]|=G6_IN<<i;
			open_data[2][25][hangunit]|=B6_IN<<i;
			open_data[0][29][hangunit]|=R7_IN<<i;
			open_data[1][29][hangunit]|=G7_IN<<i;
			open_data[2][29][hangunit]|=B7_IN<<i;
			CKA=1;
			delay_sys_us(20);		
			CKA=0;
			delay_sys_us(20);	
		}

		for(int i=0;i<=8;i++)
		{
			//open_data[3][32][4]
			//��ȡ��������ݣ����ݾ��ǻ�����Ϣ	
			open_data[0][0][hangunit]|=R0_IN<<i;
			open_data[1][0][hangunit]|=G0_IN<<i;
			open_data[2][0][hangunit]|=B0_IN<<i;
			open_data[0][4][hangunit]|=R1_IN<<i;
			open_data[1][4][hangunit]|=G1_IN<<i;
			open_data[2][4][hangunit]|=B1_IN<<i;
			
			open_data[0][8][hangunit]|=R2_IN<<i;
			open_data[1][8][hangunit]|=G2_IN<<i;
			open_data[2][8][hangunit]|=B2_IN<<i;
			open_data[0][12][hangunit]|=R3_IN<<i;
			open_data[1][12][hangunit]|=G3_IN<<i;
			open_data[2][12][hangunit]|=B3_IN<<i;
			
			open_data[0][16][hangunit]|=R4_IN<<i;
			open_data[1][16][hangunit]|=G4_IN<<i;
			open_data[2][16][hangunit]|=B4_IN<<i;
			open_data[0][20][hangunit]|=R5_IN<<i;
			open_data[1][20][hangunit]|=G5_IN<<i;
			open_data[2][20][hangunit]|=B5_IN<<i;
			
			open_data[0][24][hangunit]|=R6_IN<<i;
			open_data[1][24][hangunit]|=G6_IN<<i;
			open_data[2][24][hangunit]|=B6_IN<<i;
			open_data[0][28][hangunit]|=R7_IN<<i;
			open_data[1][28][hangunit]|=G7_IN<<i;
			open_data[2][28][hangunit]|=B7_IN<<i;
			CKA=1;
			delay_sys_us(20);		
			CKA=0;
			delay_sys_us(20);	
		}
		
		for(int i=0;i<=8;i++)
		{
			//open_data[3][32][4]
			//��ȡ��������ݣ����ݾ��ǻ�����Ϣ	
			open_data[0][3][hangunit]|=R0_IN<<i;
			open_data[1][3][hangunit]|=G0_IN<<i;
			open_data[2][3][hangunit]|=B0_IN<<i;
			open_data[0][7][hangunit]|=R1_IN<<i;
			open_data[1][7][hangunit]|=G1_IN<<i;
			open_data[2][7][hangunit]|=B1_IN<<i;
			
			open_data[0][11][hangunit]|=R2_IN<<i;
			open_data[1][11][hangunit]|=G2_IN<<i;
			open_data[2][11][hangunit]|=B2_IN<<i;
			open_data[0][15][hangunit]|=R3_IN<<i;
			open_data[1][15][hangunit]|=G3_IN<<i;
			open_data[2][15][hangunit]|=B3_IN<<i;
			
			open_data[0][19][hangunit]|=R4_IN<<i;
			open_data[1][19][hangunit]|=G4_IN<<i;
			open_data[2][19][hangunit]|=B4_IN<<i;
			open_data[0][23][hangunit]|=R5_IN<<i;
			open_data[1][23][hangunit]|=G5_IN<<i;
			open_data[2][23][hangunit]|=B5_IN<<i;
			
			open_data[0][27][hangunit]|=R6_IN<<i;
			open_data[1][27][hangunit]|=G6_IN<<i;
			open_data[2][27][hangunit]|=B6_IN<<i;
			open_data[0][31][hangunit]|=R7_IN<<i;
			open_data[1][31][hangunit]|=G7_IN<<i;
			open_data[2][31][hangunit]|=B7_IN<<i;
			CKA=1;
			delay_sys_us(20);		
			CKA=0;
			delay_sys_us(20);	
		}
		
		for(int i=0;i<=8;i++)
		{
			//open_data[3][32][4]
			//��ȡ��������ݣ����ݾ��ǻ�����Ϣ	
			open_data[0][2][hangunit]|=R0_IN<<i;
			open_data[1][2][hangunit]|=G0_IN<<i;
			open_data[2][2][hangunit]|=B0_IN<<i;
			open_data[0][6][hangunit]|=R1_IN<<i;
			open_data[1][6][hangunit]|=G1_IN<<i;
			open_data[2][6][hangunit]|=B1_IN<<i;
			
			open_data[0][10][hangunit]|=R2_IN<<i;
			open_data[1][10][hangunit]|=G2_IN<<i;
			open_data[2][10][hangunit]|=B2_IN<<i;
			open_data[0][14][hangunit]|=R3_IN<<i;
			open_data[1][14][hangunit]|=G3_IN<<i;
			open_data[2][14][hangunit]|=B3_IN<<i;
			
			open_data[0][18][hangunit]|=R4_IN<<i;
			open_data[1][18][hangunit]|=G4_IN<<i;
			open_data[2][18][hangunit]|=B4_IN<<i;
			open_data[0][22][hangunit]|=R5_IN<<i;
			open_data[1][22][hangunit]|=G5_IN<<i;
			open_data[2][22][hangunit]|=B5_IN<<i;
			
			open_data[0][26][hangunit]|=R6_IN<<i;
			open_data[1][26][hangunit]|=G6_IN<<i;
			open_data[2][26][hangunit]|=B6_IN<<i;
			open_data[0][30][hangunit]|=R7_IN<<i;
			open_data[1][30][hangunit]|=G7_IN<<i;
			open_data[2][30][hangunit]|=B7_IN<<i;
			CKA=1;
			delay_sys_us(20);		
			CKA=0;
			delay_sys_us(20);	
		}

	}
		
	taskEXIT_CRITICAL(); 		//�˳��ٽ���	
		
}






	
void SetShowArea (void)
{
  uint16_t num_hang=0;  //�ӻ�������
  uint16_t num_lie=0;   //�ӻ�������
  
  num_hang = SCREEN_HANG/BOX_HANG;
  num_lie = SCREEN_LIE/BOX_LIE;
	
	num_lie=num_lie;
  if(G_MoZuType == MZ8X16)//8X16ģ��
		ShowAreaStru.HangUint = BOX_HANG/16;//����һ�е�Ԫ����
	else
		ShowAreaStru.HangUint = BOX_HBYTE;//����һ�е�Ԫ����
	if(G_MoZuType == MZ8X4)
	  ShowAreaStru.LieUint = BOX_LIE/4; //����һ�е�Ԫ����
	else
		ShowAreaStru.LieUint = BOX_FANHUI;//����һ�е�Ԫ����
		
  ShowAreaStru.LieStart = ((G_IDSlave0-1)/num_hang)*BOX_LIE;    //�����ؼ���
  ShowAreaStru.HByteStart = ((G_IDSlave0-1)%num_hang)*BOX_HBYTE;//���ֽڼ��� 
  
  ShowAreaStru.LieEnd = ShowAreaStru.LieStart + BOX_LIE;   //�޸�2014.7.14****************************************
  ShowAreaStru.HByteEnd = ShowAreaStru.HByteStart + BOX_HBYTE;

}


void LedDisplay (void)
{
		u16 hangunit;//�е�Ԫ��
				STB=0;
		for(hangunit=0;hangunit<ShowAreaStru.HangUint;hangunit++)//ѡ��һ���еĵڼ�����Ԫ��***��*****************************
			{
				SendOneUnitData(hangunit);	 //��һ����Ԫ������ ��ʾ�е�Ԫ�塢��ʾ�е�Ԫ��
			}		
			STB=1;
			delay_sys_us(5);	
			STB=0;
}


void SendOneUnitData (uint8_t hangunit)//���£�����
{
//**********************************************************************************************
/*
RGB���ݻ���  uint8_t    DisplayBuf[3][4096];  3 ����������ɫ  4096�������ݳ���
1 -10
11-20
21-30
Ӧ����ȡ���ұ��Ǹ�ģ��� ���� Ȼ�����������ȡ��ֱ����һ����Ȼ�������һ��
�˴Ӱ���Ϊ�л������������������У���������DisplayBuf��4096�����Գ������1024����
�����ط�Χ 32X1024
*/

	uint32_t buffer[3][8]={0};//3 ������ɫ 01 �ǵ�һ������������ 23�ǵڶ������������� 45�ǵ��������������� 67�ǵ���������������
	
	for(uint8_t lieunit =0;lieunit<(ShowAreaStru.LieUint * 2);lieunit+=2)	//����75�������������ߣ�����һ��ȡ��������
	{
		
			for(uint8_t j=0;j<BOX_YANSE;j++)
			{
				buffer[j][lieunit] = 		DisplayBuf[j][hangunit+(lieunit*4+2)*BOX_HBYTE]<<24|DisplayBuf[j][hangunit+(lieunit*4+3)*BOX_HBYTE]<<16|
																DisplayBuf[j][hangunit+(lieunit*4+0)*BOX_HBYTE]<<8|DisplayBuf[j][hangunit+(lieunit*4+1)*BOX_HBYTE];
				
				buffer[j][lieunit+1] =	DisplayBuf[j][hangunit+(lieunit*4+6)*BOX_HBYTE]<<24|DisplayBuf[j][hangunit+(lieunit*4+7)*BOX_HBYTE]<<16|
																DisplayBuf[j][hangunit+(lieunit*4+4)*BOX_HBYTE]<<8|DisplayBuf[j][hangunit+(lieunit*4+5)*BOX_HBYTE];
			}

	}
		switch(G_MoZuType) //ѡ��ģ��
		{
			case MZ8X8://1:8X8
					SendOneUnitData_8X8(buffer[0],buffer[1],buffer[2]);
					//ֻ������ĳһ�����е�Ԫ�������
					//�����������еķ������һ�������
			break;
			default:break;
		}

}









