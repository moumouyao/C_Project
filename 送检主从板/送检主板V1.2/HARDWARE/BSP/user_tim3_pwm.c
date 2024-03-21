/**
  ******************************************************************************
  * 文件名 : user_tim.c
  * 作  者 : 陈光
  * 版  本 ：V1.0
  * 时  间 : 2014年5月24日
  * 公  司 ：汉威光电技术有限公司
  * 摘  要 ：本文件包含了TIMx模块的初始化、配置等，
  ******************************************************************************
  */
/**
  ******************************************************************************
  *                                  头文件
  ******************************************************************************
  */

#include "lwip_comm.h"
#include "user_tim3_pwm.h"
#include "stdio.h"
#include "sw_global.h"
#include "AD.h"

uint16_t bright_level    = 32;	//亮度级
uint16_t last_bright_level = 32;//保存上次亮度级标志位
uint8_t  bright_level_flag = 0;	//亮度级标志位	
uint8_t  bright_calculate = 32;	//用于手动模式下亮度计算

/**
  ******************************************************************************
  *                              Tim3PwmConfig()
  *
  * 描述   : 配置TIM3为PWM模式
  *		   
  * 参数   : 无
  *
  *
  * 返回值 : 无
  ******************************************************************************
  */


void TIM9_CH1_PWM_Init(uint16_t Period,uint16_t Prescaler,uint16_t CCR4_Val)
{
  /* Tim3 PWM的配置 */ 
  GPIO_InitTypeDef        GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_OCInitTypeDef       TIM_OCInitStructure;
  NVIC_InitTypeDef        NVIC_InitStructure;
	/* 打开GPIOE,TIM9的时钟 */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM9,ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE ,ENABLE);//使能F时钟  
  
  GPIO_PinAFConfig(GPIOE,GPIO_PinSource5,GPIO_AF_TIM9);//GPIOe5复用为定时器9


  
  /* PE5-TIM9_CH1  OEPWM引脚配置 */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽复用输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        //上拉
	GPIO_Init(GPIOE,&GPIO_InitStructure);

	/* Time base configuration */
	/* 定时器0频率为36M/Prescaler/(Period+1) */
	/* 当定时器从0计数到127，即为128次，为一个定时周期 */
	TIM_TimeBaseStructure.TIM_Period = Period;	//自动重装载值
	TIM_TimeBaseStructure.TIM_Prescaler = Prescaler;//定时器分频
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分频系数：不分频
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //向上计数

	TIM_TimeBaseInit(TIM9,&TIM_TimeBaseStructure); //初始化定时器9

  
	//初始化TIM9 Channel 1 PWM模式
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //配置为PWM模式1
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;//比较输出使能
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;//输出极性:TIM输出比较极性低
	TIM_OC1Init(TIM9,&TIM_OCInitStructure);  //根据T指定的参数初始化外设TIM9OC2
	
  /* 设置通道4的电平跳变值，输出另外一个占空比的PWM */
	TIM_OCInitStructure.TIM_Pulse = CCR4_Val;


	TIM_OC1PreloadConfig(TIM9,TIM_OCPreload_Enable);//使能TIM14在CCR1上的预装载寄存器

  TIM_ITConfig(TIM9,TIM_IT_Update,ENABLE); //允许定时器9更新中断
	TIM_ARRPreloadConfig(TIM9,ENABLE);   //ARPE使能 
	/* TIM3 enable counter */
	TIM_Cmd(TIM9,ENABLE);   //使能TIM9
  
  
  
  NVIC_InitStructure.NVIC_IRQChannel=TIM3_IRQn; //定时器3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x01; //抢占优先级1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x03; //子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);

}



//定时器9中断服务函数
void TIM9_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM9,TIM_IT_Update)==SET) //溢出中断
	{
		//lwip_localtime +=2; //加10
    
	}
	TIM_ClearITPendingBit(TIM9,TIM_IT_Update);  //清除中断标志位
}

/**
  ******************************************************************************
  *                              PwmSet()
  *
  * 描述   : 动态设置PWM占空比
  *		   
  * 参数   : uint8_t value--0-99
  *
  *
  * 返回值 : 无
  ******************************************************************************
  */
void PwmSet(uint16_t value)
{
	TIM_OCInitTypeDef TIM_OCInitStructure;
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //配置为PWM模式1
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;

	/* 当计数器计数值小于CCR1_Val时为低电平 */
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;

	/* PWM1 Mode configuration: Channel4 */
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	
	/* 设置通道4的电平跳变值，输出另外一个占空比的PWM */
	TIM_OCInitStructure.TIM_Pulse = value;

	TIM_OC1Init(TIM9,&TIM_OCInitStructure);  
}


//设置亮度等级
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
	BrightRefresh();//更新亮度值
}
				
//调整亮度值
void BrightRefresh(void)
{
	static unsigned char cTime;//2s变化一次
	static uint8_t now_pwm = 128; //128
	/* 备注：亮度调节这块新做，具体的测试还没有进行 亮度PWM*/
	if(BrightValue <= 0)
		BrightValue = 0;
	if(BrightValue >bright_level)
		BrightValue = bright_level-1;	
	
	if((KeyFlag!=0))//调屏时为最大亮度
	{
		PwmSet(128*max_bright_proportion/100);//127
		return ;
	}

	if(BrightMod == 0)     //自动调整亮度
	{

		if((ADC1ConvertedValue>3950 && ADC1ConvertedValue<4050)||(ADC1ConvertedValue ==0))//光敏断路或短路
		{
			LdrSt=1; //光敏故障
      ADC1ConvertedValue = 3500;
		}
		else{
			LdrSt=0;				
		}
		if(abs(ADC1ConvertedValue - BrightAD)<=30) //防止跳变
			ADC1ConvertedValue = BrightAD;

		BrightPwm = (4096 - ADC1ConvertedValue)/32 <=1 ? 1:(4096 - ADC1ConvertedValue)/32;	//自动调整最低亮度
		BrightValue = LowOut(BrightPwm);//低通滤波
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
    //手动调整	BrightValue从0开始故需要加1
//		BrightPwm = (BrightValue+1)*bright_calculate*max_bright_proportion/100;
//		if((BrightValue+1)*bright_calculate*max_bright_proportion%100/10 >5)
//			BrightPwm++;
//		PwmSet(BrightPwm);
//		now_pwm = BrightPwm;	// 此处保存上次的占空值	，修改
//    printf("手动BrightPwm=%d\n",BrightPwm);    
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



