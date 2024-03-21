/**
  ******************************************************************************
  * �ļ��� : user_tim.c
  * ��  �� : �¹�
  * ��  �� ��V1.0
  * ʱ  �� : 2014��5��24��
  * ��  ˾ ��������缼�����޹�˾
  * ժ  Ҫ �����ļ�������TIMxģ��ĳ�ʼ�������õȣ�
  ******************************************************************************
  */
/**
  ******************************************************************************
  *                                  ͷ�ļ�
  ******************************************************************************
  */

#include "lwip_comm.h"
#include "user_tim3_pwm.h"
#include "stdio.h"
#include "sw_global.h"
#include "AD.h"

uint16_t bright_level    = 32;	//���ȼ�
uint16_t last_bright_level = 32;//�����ϴ����ȼ���־λ
uint8_t  bright_level_flag = 0;	//���ȼ���־λ	
uint8_t  bright_calculate = 32;	//�����ֶ�ģʽ�����ȼ���

/**
  ******************************************************************************
  *                              Tim3PwmConfig()
  *
  * ����   : ����TIM3ΪPWMģʽ
  *		   
  * ����   : ��
  *
  *
  * ����ֵ : ��
  ******************************************************************************
  */


void TIM9_CH1_PWM_Init(uint16_t Period,uint16_t Prescaler,uint16_t CCR4_Val)
{
  /* Tim3 PWM������ */ 
  GPIO_InitTypeDef        GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_OCInitTypeDef       TIM_OCInitStructure;
  NVIC_InitTypeDef        NVIC_InitStructure;
	/* ��GPIOE,TIM9��ʱ�� */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM9,ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE ,ENABLE);//ʹ��Fʱ��  
  
  GPIO_PinAFConfig(GPIOE,GPIO_PinSource5,GPIO_AF_TIM9);//GPIOe5����Ϊ��ʱ��9


  
  /* PE5-TIM9_CH1  OEPWM�������� */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//���츴�����
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        //����
	GPIO_Init(GPIOE,&GPIO_InitStructure);

	/* Time base configuration */
	/* ��ʱ��0Ƶ��Ϊ36M/Prescaler/(Period+1) */
	/* ����ʱ����0������127����Ϊ128�Σ�Ϊһ����ʱ���� */
	TIM_TimeBaseStructure.TIM_Period = Period;	//�Զ���װ��ֵ
	TIM_TimeBaseStructure.TIM_Prescaler = Prescaler;//��ʱ����Ƶ
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //����ʱ�ӷ�Ƶϵ��������Ƶ
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //���ϼ���

	TIM_TimeBaseInit(TIM9,&TIM_TimeBaseStructure); //��ʼ����ʱ��9

  
	//��ʼ��TIM9 Channel 1 PWMģʽ
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //����ΪPWMģʽ1
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;//�Ƚ����ʹ��
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;//�������:TIM����Ƚϼ��Ե�
	TIM_OC1Init(TIM9,&TIM_OCInitStructure);  //����Tָ���Ĳ�����ʼ������TIM9OC2
	
  /* ����ͨ��4�ĵ�ƽ����ֵ���������һ��ռ�ձȵ�PWM */
	TIM_OCInitStructure.TIM_Pulse = CCR4_Val;


	TIM_OC1PreloadConfig(TIM9,TIM_OCPreload_Enable);//ʹ��TIM14��CCR1�ϵ�Ԥװ�ؼĴ���

  TIM_ITConfig(TIM9,TIM_IT_Update,ENABLE); //����ʱ��9�����ж�
	TIM_ARRPreloadConfig(TIM9,ENABLE);   //ARPEʹ�� 
	/* TIM3 enable counter */
	TIM_Cmd(TIM9,ENABLE);   //ʹ��TIM9
  
  
  
  NVIC_InitStructure.NVIC_IRQChannel=TIM3_IRQn; //��ʱ��3�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x01; //��ռ���ȼ�1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x03; //�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);

}



//��ʱ��9�жϷ�����
void TIM9_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM9,TIM_IT_Update)==SET) //����ж�
	{
		//lwip_localtime +=2; //��10
    
	}
	TIM_ClearITPendingBit(TIM9,TIM_IT_Update);  //����жϱ�־λ
}

/**
  ******************************************************************************
  *                              PwmSet()
  *
  * ����   : ��̬����PWMռ�ձ�
  *		   
  * ����   : uint8_t value--0-99
  *
  *
  * ����ֵ : ��
  ******************************************************************************
  */
void PwmSet(uint16_t value)
{
	TIM_OCInitTypeDef TIM_OCInitStructure;
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //����ΪPWMģʽ1
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;

	/* ������������ֵС��CCR1_ValʱΪ�͵�ƽ */
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;

	/* PWM1 Mode configuration: Channel4 */
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	
	/* ����ͨ��4�ĵ�ƽ����ֵ���������һ��ռ�ձȵ�PWM */
	TIM_OCInitStructure.TIM_Pulse = value;

	TIM_OC1Init(TIM9,&TIM_OCInitStructure);  
}


//�������ȵȼ�
void BRIGHT_Level_Set(uint8_t pdata)
{	
	switch(pdata)
	{
		case 0:
			bright_level = 32;
    	bright_calculate = 128/bright_level;
      Period = 127;
      Prescaler = 128;
			break;
		case 1:
			bright_level = 64;
    	bright_calculate = 128/bright_level;
      Period =127;
      Prescaler =128;
			break;
		case 2:      
			bright_level = 1024;
    	bright_calculate = 1;
      Period =1024;
      Prescaler =0;
			break;
		default:break;
	}

//	printf("\r\nbright_calculate =  %d\r\n",bright_calculate);

	
	if(last_bright_level < bright_level)
		BrightValue = BrightValue*(bright_level/last_bright_level);
	else if(last_bright_level > bright_level)
		BrightValue = BrightValue/(last_bright_level/bright_level);
	
	last_bright_level = bright_level;
  TIM9_CH1_PWM_Init(Period,Prescaler,0);
   
  
//  printf("Period=%d Prescaler=%d",Period,Prescaler);
//  printf("BrightValue=%d",BrightValue);
	BrightRefresh();//��������ֵ
}
				
//��������ֵ
void BrightRefresh(void)
{
	static unsigned char cTime;//2s�仯һ��
	static uint8_t now_pwm = 128; //128
	/* ��ע�����ȵ����������������Ĳ��Ի�û�н��� ����PWM*/
	if(BrightValue <= 0)
		BrightValue = 0;
	if(BrightValue >bright_level)
		BrightValue = bright_level-1;	
	
	if((KeyFlag!=0))//����ʱΪ�������
	{
		PwmSet(128*max_bright_proportion/100);//127
		return ;
	}

	if(BrightMod == 0)     //�Զ���������
	{

		if((ADC1ConvertedValue>3950 && ADC1ConvertedValue<4050)||(ADC1ConvertedValue ==0))//������·���·
		{
			LdrSt=1; //��������
      ADC1ConvertedValue = 3500;
		}
		else{
			LdrSt=0;				
		}
		if(abs(ADC1ConvertedValue - BrightAD)<=30) //��ֹ����
			ADC1ConvertedValue = BrightAD;

		BrightPwm = (4096 - ADC1ConvertedValue)/32 <=1 ? 1:(4096 - ADC1ConvertedValue)/32;	//�Զ������������
		BrightValue = LowOut(BrightPwm);//��ͨ�˲�
		BrightPwm = BrightValue;
		
		if(BrightPwm != now_pwm)
		{
			if(cTime >= 15)
			{
				cTime = 0;
				if(BrightPwm<=30)  
				{
					if(bright_level == 1024)
						PwmSet(((BrightPwm<2) ? 1:(BrightPwm*max_bright_proportion/100))*1024/128);
					else
						PwmSet(((BrightPwm<2) ? 1:(BrightPwm*max_bright_proportion/100)));
					SUV_pwm=(BrightPwm<2) ? 1:(BrightPwm*max_bright_proportion/100);
				}
				if(BrightPwm>30)
				{
					if(bright_level == 1024)
						PwmSet(((BrightPwm<2) ? 1:(BrightPwm*max_bright_proportion/100))*1024/128);
					else
						PwmSet(((BrightPwm<2) ? 1:(BrightPwm*max_bright_proportion/100)));
					SUV_pwm=BrightPwm*max_bright_proportion/100;
				}
				now_pwm = BrightPwm;
				BrightAD=ADC1ConvertedValue;
			}
			else
			{
				cTime++;
			}
		}
	}
	else
	{
    //�ֶ�����	BrightValue��0��ʼ����Ҫ��1
//		BrightPwm = (BrightValue+1)*bright_calculate*max_bright_proportion/100;
//		if((BrightValue+1)*bright_calculate*max_bright_proportion%100/10 >5)
//			BrightPwm++;
//		PwmSet(BrightPwm);
//		now_pwm = BrightPwm;	// �˴������ϴε�ռ��ֵ	���޸�
//    printf("�ֶ�BrightPwm=%d\n",BrightPwm);    
	}
}
#define LowConstant  0.2
#define LowConstants 0.8
unsigned char LowOut(unsigned char cmd)
{
	static double dLastData;
	double dOut;
	dOut = cmd*LowConstant + dLastData*LowConstants;
	dLastData = dOut;
	return (unsigned char)dOut;
}



