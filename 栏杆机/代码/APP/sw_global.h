#ifndef __SW_GLOBAL_H__
#define __SW_GLOBAL_H__

#ifdef __cplusplus
 extern "C" {
#endif

#include "os_cpu.h" 
#include "ucos_ii.h"


#define shihangzuoyi 		0		//十行左移
#define wuhanghuanhang 	1 	//五行换行
	 

#define  HEAD_ETH_TYPE  		1
#define  HEAD_USART2_TYPE		2
#define  HEAD_KEY_TYPE		3

//屏幕宽高点阵大小
#define SCREEN_HEIGHT 192	//高像素
#define SCREEN_WIDTH  96

//宽像素

#define HZ_DATA_LEN 32
#define ZF_DATA_LEN 16


#define RED    1
#define GREEN  2
#define YELLOW 3
#define BLUE   4

#define HZ     1
#define ZF     2 

extern uint8_t zm[SCREEN_HEIGHT][(SCREEN_WIDTH/8)+12];//组成整平字模
extern uint8_t  checkflag; //U标志位

extern OS_EVENT * ZMSem;               //字模数组资源信号量

extern unsigned char hongchalvjiantou_flag;//继电器控制红色还是绿色字体

#define USART2_BUF_SIZE 1024	//发送数据长度,最好等于sizeof(TEXT_TO_SEND)+2的整数倍.

#define En_Dis_Code_QEUE_LENGTH				4     //加密解密消息队列长度
#define Dis_En_Code_QEUE_LENGTH				4     //加密解密消息队列长度
#define En_Udp_Code_QEUE_LENGTH				4     //加密解密消息队列长度


//EEROM 内存管理

//*******串口屏 屏参设置标志位***********/
//通信参数设置
#define EEROM_USART_BSP_ADDR       0	//3byte
//本机IP地址
#define EEROM_Local_IP_ADDR				3	//4byte
//本机端口号
#define EEROM_Port_ADDR						7	//2byte
//本机网关
#define EEROM_Gateway_ADDR				9	//4Byte
//子网掩码
#define EEROM_Netmask_ADDR						13	//4Byte
//远端IP
#define EEROM_REMOTE_IP_ADDR				17	//4byte
//远端端口号
#define EEROM_REMOTE_PORT_ADDR			21	//2byte

extern void     * En_Dis_CodeQeueTab[En_Dis_Code_QEUE_LENGTH];
extern OS_EVENT * En_Dis_CodeQeue;//上位机发送至解析报文消息队列

extern void     * En_Udp_CodeQeueTab[En_Udp_Code_QEUE_LENGTH];
extern OS_EVENT * En_Udp_CodeQeue;//解析报文任务发送到UDP消息队列
#endif

