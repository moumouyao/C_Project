#ifndef __UDP_DEMO_H
#define __UDP_DEMO_H
#include "sys.h"
#include "includes.h"

//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK STM32F407开发板
//NETCONN API编程方式的UDP测试代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//创建日期:2014/8/15
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved									  
//*******************************************************************************
//修改信息
//无
////////////////////////////////////////////////////////////////////////////////// 	   
 
 
#define UDP_DEMO_RX_BUFSIZE		1024	//定义udp最大接收数据长度
#define UDP_DEMO_PORT			8089	//定义udp连接的本地端口号
#define LWIP_SEND_DATA			0X80    //定义有数据发送

extern u8 udp_flag;		//UDP数据发送标志位
INT8U udp_demo_init(void);
#endif

