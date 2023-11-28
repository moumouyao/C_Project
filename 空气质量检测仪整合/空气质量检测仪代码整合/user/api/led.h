#ifndef __LED_H
#define __LED_H
#include "stm32f10x.h"                  // Device header
//LED灯宏定义
#define LED1 1
#define LED2 2
#define LED3 3
#define LED4 4
//led IO口
#define  LED1_PORT   GPIOE
#define  LED2_PORT   GPIOE
#define  LED3_PORT   GPIOE
#define  LED4_PORT   GPIOE
#define  RED_PORT   GPIOA
#define  GREEN_PORT   GPIOA
#define  BLUE_PORT   GPIOA
//LED PIN口
#define  LED1_PIN    1<<2
#define  LED2_PIN    1<<3
#define  LED3_PIN    1<<4
#define  LED4_PIN    1<<5
#define  RED_PIN		1<<8
#define GREEN_PIN		1<<7
#define BLUE_PIN		1<<6
//RGB灯宏定义
#define RED_LED 3
#define GREEN_LED 2
#define BLUE_LED 1

void RGB_init(void);
void RGB_enable(uint32_t t);
void RGB_disable(uint32_t t);
void led_init(uint32_t t);
void led_enable(uint32_t t);
void led_disable(uint32_t t);
void waterfall_light(void);
void led_turn(GPIO_TypeDef *led_port,uint16_t ledpin);
#endif
