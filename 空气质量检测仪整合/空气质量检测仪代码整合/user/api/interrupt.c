#include "interrupt.h"
#include "buzzer.h"
#include "led.h"

uint8_t k1 = 0, k2 = 0, k3 = 0, k4 = 0;


/**
  * @brief  ����1�жϷ����������Ʒ�����
  * @param  None
  * @retval None
  */
void EXTI0_IRQHandler(void){
	buzzer_init();
	if(k1 == 0){
		buzzer_enable();
		k1++;
		EXTI_ClearITPendingBit(EXTI_Line0);
		return;
	}
	if(k1 == 1){
		buzzer_disable();
		k1 = 0;
		EXTI_ClearITPendingBit(EXTI_Line0);
	}
	
}

/**
  * @brief  ����2�жϷ�������������ˮ��
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
  * @brief  ���� EXTI4_IRQHandler �Ĳ������
  * @param  None
  * @retval uint8_t val	k2��ֵ
  */
uint8_t get_k2_val(void){
	uint16_t val = k2;
	return val;
}
/**
  * @brief  ����3,����4�жϷ�����
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
  * @brief  ��ʱ��2�жϷ�����
  * @param  None
  * @retval None
  */
void TIM2_IRQHandler(void){
	if(TIM_GetITStatus(TIM2, TIM_IT_Update) == SET){
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
		
		
		buzzer_init();
		//buzzer_turn(BUZZER_PORT, BUZZER_PIN);
		led_turn(GREEN_PORT, GREEN_PIN);
	}
	
}

/**
  * @brief  RTC�Ĵ������жϣ�RGB�Ʒ�ת
  * @param  None
  * @retval None
  */
void RTC_IRQHandler(void){
	if(RTC_GetITStatus(RTC_IT_SEC) == SET){
		RTC_ClearITPendingBit(RTC_IT_SEC);
		led_turn(LED2_PORT, LED2_PIN);
		//led_turn(GREEN_PORT, GREEN_PIN);
	}
	
}




