#include "wdg.h"
#include "stm32f4xx.h"
//看门狗驱动代码
//分频数:0~7（只有低三位有效）
//分频因子=4*2^prer。但是最大只能为256
//rlr重装载寄存器值.（低12位有效）
//时间计算：不准确时钟在17~47Khz之间按32Khz算
//Timeout=((4*2^prer)*rlr)/32 (ms).

//初始化独立看门狗
void IWDG_Init(void)
{
//1.解锁独立看门狗的寄存器
	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);
	//2.进行32分频，独立看门狗的硬件时钟频率=32KHz/32=1000Hz
	IWDG_SetPrescaler(IWDG_Prescaler_32);
	//3.设置独立看门狗的计数值,1.5秒钟的时间到达
	IWDG_SetReload(2000-1);
	//4.将独立看门狗加上写保护
	IWDG_ReloadCounter();
        //5.使能独立看门狗
	IWDG_Enable();
}
	
	
	
//喂狗操作
void IWDG_Feed(void)
{
 IWDG_ReloadCounter();
}

