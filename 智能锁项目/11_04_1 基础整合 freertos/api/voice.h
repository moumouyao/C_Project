#ifndef __VOICE_H_
#define __VOICE_H_

#include "stm32f4xx.h"                  // Device header
#include "delay.h"
#include "usart.h"

#define	Sound_off		0X00						//����
#define	Chang_Adm_Password	0X01		//�޸Ĺ���Ա����
#define	Set_door_password		0X02		//���ÿ�������	
#define	Register_fingerprint	0X03	//�Ǽ�ָ��
#define	Registration_card		0X04		//�Ǽǿ�Ƭ
#define	Delete_all_fingerprints	0X06//ɾ������ָ��
#define	Delete_specified_fingerprint	0X07	//ɾ��ָ��ָ��
#define	factory_data_reset	0X08	//�ָ���������
#define	Set_volume	0X09						//��������
#define	Delete_all_cards	0X0a		//ɾ�������ſ�
#define	Delete_specified_card	0X0b//ɾ��ָ���ſ�
#define	new_password		0X0d		//������������
#define new_password_again	0X0e//���ٴ�����������
#define	 put_finger	0X16				//�����ָ
#define	Welcome_home	0X18		//��ӭ�ؼ�
#define	opening_failure		0X19//����ʧ��
#define	dudu	0X1d		//�
#define	dingdong	0X1e		
#define Password_inconsistency	0X10




void send_voi_cmd(uint8_t cmd);
void voice_init(void);

#endif
