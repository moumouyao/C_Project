#ifndef __VOICE_H_
#define __VOICE_H_

#include "stm32f4xx.h"                  // Device header
#include "delay.h"
#include "usart.h"

#define	Sound_off		0X00						//静音
#define	Chang_Adm_Password	0X01		//修改管理员密码
#define	Set_door_password		0X02		//设置开门密码	
#define	Register_fingerprint	0X03	//登记指纹
#define	Registration_card		0X04		//登记卡片
#define	Delete_all_fingerprints	0X06//删除所有指纹
#define	Delete_specified_fingerprint	0X07	//删除指定指纹
#define	factory_data_reset	0X08	//恢复出厂设置
#define	Set_volume	0X09						//设置音量
#define	Delete_all_cards	0X0a		//删除所有门卡
#define	Delete_specified_card	0X0b//删除指定门卡
#define	new_password		0X0d		//请输入新密码
#define new_password_again	0X0e//请再次输入新密码
#define	 put_finger	0X16				//请放手指
#define	Welcome_home	0X18		//欢迎回家
#define	opening_failure		0X19//开门失败
#define	dudu	0X1d		//嘟
#define	dingdong	0X1e		
#define Password_inconsistency	0X10




void send_voi_cmd(uint8_t cmd);
void voice_init(void);

#endif
