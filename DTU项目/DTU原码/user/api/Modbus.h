#ifndef __MODBUS_H_
#define __MODBUS_H_

#include "stm32f10x.h"                  // Device header
#include "RS485.h"

 typedef struct 
 {
 	//��Ϊ�ӻ�ʱʹ��
  	 u8  myadd;        //���豸�ӻ���ַ
 	u8  rcbuf[100];   //modbus���ܻ�����
 	u8  timout;       //modbus���ݳ���ʱ��
 	u8  recount;      //modbus�˿ڽ��յ������ݸ���
 	u8  timrun;       //modbus��ʱ���Ƿ��ʱ��־
 	u8  reflag;       //modbusһ֡���ݽ�����ɱ�־λ
 	u8  sendbuf[100]; //modbus�ӷ��ͻ�����
 	
 	//��Ϊ������Ӳ���
 	u8 Host_Txbuf[8];	//modbus��������	
 	u8 slave_add;		//Ҫƥ��Ĵӻ��豸��ַ��������ʵ��ʱʹ�ã�
 	u8 Host_send_flag;//�����豸����������ϱ�־λ
 	int Host_Sendtime;//������һ֡���ݺ�ʱ�����
 	u8 Host_time_flag;//����ʱ�䵽��־λ��=1��ʾ����������ʱ����
 	u8 Host_End;//�������ݺ������
 }MODBUS;

extern MODBUS modbus;
extern uint16_t modbus_data[5];
 
void Host_Func3(void);
void Modbus_Func3(void);
void Host_Read03_slave(uint8_t slave,uint16_t StartAddr,uint16_t num);
void HOST_ModbusRX(void);
void Modbus_Init(void);
void Modbus_Event(void);
void Modbus_Func6(void) ;
void Modbus_Func16(void);
unsigned int Modbus_CRC16(unsigned char *data, unsigned int len);

#endif

 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 