#include "interrupt.h"

#include "led.h"

uint8_t k1 = 0, k2 = 0, k3 = 0, k4 = 0;


/**
  * @brief  按键1中断服务函数，控制蜂鸣器
  * @param  None
  * @retval None
  */
void EXTI0_IRQHandler(void){

	if(k1 == 0){
	
		k1++;
		EXTI_ClearITPendingBit(EXTI_Line0);
		return;
	}
	if(k1 == 1){

		k1 = 0;
		EXTI_ClearITPendingBit(EXTI_Line0);
	}
	
}

/**
  * @brief  按键2中断服务函数，控制流水灯
  * @param  None
  * @retval None
  */
void EXTI4_IRQHandler(void){
	RGB_init();
	if(k2 == 0){
		RGB_enable(RED);
		k2 = 1;
	}
	else{
		RGB_disable(RED);
		k2 = 0;
	}
	EXTI_ClearITPendingBit(EXTI_Line4);
}

/**
  * @brief  返回 EXTI4_IRQHandler 的操作结果
  * @param  None
  * @retval uint8_t val	k2的值
  */
uint8_t get_k2_val(void){
	uint16_t val = k2;
	return val;
}
/**
  * @brief  按键3,按键4中断服务函数
  * @param  None
  * @retval None
  */
void EXTI9_5_IRQHandler(void){
	RGB_init();
	if(EXTI_GetITStatus(EXTI_Line5) == SET){
			if(k3 == 0){
			RGB_enable(GREEN);
			k3 = 1;
		}
		else{
			RGB_disable(GREEN);
			k3 = 0;
		}
		EXTI_ClearITPendingBit(EXTI_Line5);
		}
	if(EXTI_GetITStatus(EXTI_Line6) == SET){
		if(k4 == 0){
			RGB_enable(BLUE);
			k4 = 1;
		}
		else{
			RGB_disable(BLUE);
			k4 = 0;
		}
		EXTI_ClearITPendingBit(EXTI_Line6);
		}
	
}

/**
  * @brief  定时器2中断服务函数
  * @param  None
  * @retval None
  */
//void TIM2_IRQHandler(void){
//	if(TIM_GetITStatus(TIM2, TIM_IT_Update) == SET){
//		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
//		
//		

//		//buzzer_turn(BUZZER_PORT, BUZZER_PIN);
//		led_turn(GREEN_PORT, GREEN_PIN);
//	}
//	
//}
 // Modbus 定时器中断函数 1ms中断一次
 void TIM2_IRQHandler(void)   //TIM3中断
 {
 	
 	if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET) //检查指定的TIM中断发生与否:TIM 中断源 
 	{
 		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);  //清除TIMx的中断待处理位:TIM 中断源 
 		if(modbus.timrun != 0)//运行时间！=0表明
 		 {
 		  modbus.timout++;
 		  if(modbus.timout >=8)
 		  {
 		   modbus.timrun = 0;
 			 modbus.reflag = 1;//接收数据完毕
			modbus.Host_send_flag = 1;
 		  }
 			
 		 }
 		 modbus.Host_Sendtime++;//发送完上一帧后的时间计数
 		 if(modbus.Host_Sendtime>1000)//距离发送上一帧数据1s了
 			{
 				//1s时间到
 				modbus.Host_time_flag=1;//发送数据标志位置1
 				
 			}
 	}
 }

