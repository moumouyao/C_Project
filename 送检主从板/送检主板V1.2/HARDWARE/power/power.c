#include "power.h"
#include "includes.h"
#include "sw_global.h"
#include "memp.h" 
#include "wdg.h"
#include "cJSON.h"
#include "ctrlcenter_app.h"

void Power_Gpio(void)
{
   //PD12--DUMP
  GPIO_InitTypeDef GPIO_InitStructure;
  RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOD, ENABLE);

  GPIO_InitStructure.GPIO_Pin= GPIO_Pin_12;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOD,&GPIO_InitStructure);      
}


//掉电检测
void EXTI_Power_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure; 
	EXTI_InitTypeDef EXTI_InitStructure;

/* config the extiline clock and AFIO clock  */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD,ENABLE);
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
	
/* EXTI line gpio configPD8- MOINTOR */	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Mode= GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOD, &GPIO_InitStructure);

	
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOD, EXTI_PinSource8);    
	EXTI_InitStructure.EXTI_Line = EXTI_Line8;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;//下降沿触发
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);

}


//控制外接继电器开关
void Control_Power(cJSON *json)
{
	uint8_t value;
	value=cjson_get_number(json,"value");
	//0：关闭，1：打开
	if(value==1)
	{
    GPIO_SetBits(GPIOE,GPIO_Pin_3);	
    screenflag=1;
	}
	else
	{
		GPIO_ResetBits(GPIOE,GPIO_Pin_3);
    screenflag=0;
	}
  IWDG_Feed();	
	
	cJSON_Delete(json);	
}


