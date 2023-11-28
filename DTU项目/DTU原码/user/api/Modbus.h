#ifndef __MODBUS_H_
#define __MODBUS_H_

#include "stm32f10x.h"                  // Device header
#include "RS485.h"

 typedef struct 
 {
 	//作为从机时使用
  	 u8  myadd;        //本设备从机地址
 	u8  rcbuf[100];   //modbus接受缓冲区
 	u8  timout;       //modbus数据持续时间
 	u8  recount;      //modbus端口接收到的数据个数
 	u8  timrun;       //modbus定时器是否计时标志
 	u8  reflag;       //modbus一帧数据接受完成标志位
 	u8  sendbuf[100]; //modbus接发送缓冲区
 	
 	//作为主机添加部分
 	u8 Host_Txbuf[8];	//modbus发送数组	
 	u8 slave_add;		//要匹配的从机设备地址（做主机实验时使用）
 	u8 Host_send_flag;//主机设备发送数据完毕标志位
 	int Host_Sendtime;//发送完一帧数据后时间计数
 	u8 Host_time_flag;//发送时间到标志位，=1表示到发送数据时间了
 	u8 Host_End;//接收数据后处理完毕
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

 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 