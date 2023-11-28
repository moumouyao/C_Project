#ifndef __LVGL_APP_H_
#define __LVGL_APP_H_

#include "stm32f10x.h"                  // Device header
#include "lvgl.h"
#include "lv_port_disp.h"
#include "stdio.h"
#include "RTC.h"
#include "kqm.h"
#include "string.h"
#include "kqm.h"
#include "dht11.h"
#include "ADC.h"
#include "key.h"
#include "Delay.h"
#include "gui_guider.h"
#include "esp8266.h"

void font_test(void);
void btn_event_test(void);
void wifi_set(void);
void setup_animation();

#endif
