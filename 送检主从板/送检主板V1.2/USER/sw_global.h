/**
  ******************************************************************************
  * 文件名 : sw_global.h
  * 作  者 : 陈光
  * 版  本 ：V1.0
  * 时  间 : 2014年5月14日
  * 公  司 ：汉威光电技术有限公司
  * 摘  要 ：本文件包含全局变量声明
  ******************************************************************************
  */

#ifndef __SW_GLOBAL_H__
#define __SW_GLOBAL_H__

#ifdef __cplusplus
 extern "C" {
#endif
#include "sw_controlcenter.h"
#include "os_cpu.h" 
#include "ucos_ii.h"
#include "ff.h"	 


/**
  ******************************************************************************
  *                           全局变量声明区
  ******************************************************************************
  */
#define EnableTest 			0 //是否使能敏感词检测
#define EnableCode 			0 //是否使用加密IC
#define EnableLog  			1 //是否开启日志功能
#define EnableWhiteList 0 //是否开启白名单功能
#define WhiteListNum 		5 //白名单数量
#define EnableCode      0


#define cJSON_Dbug      1   //json_debug调试
//====================2022 6 20 白名单功能新增变量===============//	 
extern unsigned char g_cTcpClientState;//接入客户端的状态 1=白名单 0=非白名单
extern unsigned char *g_cIpWhiteList[WhiteListNum];//白名单
extern unsigned char g_cIp[4];//本机IP
extern unsigned short g_cPort;//网页端的端口(初始化用)
extern unsigned char g_cGateway[4];
extern unsigned char g_cSysRst;
extern unsigned char *g_cRemot_addr;//远程IP地址
extern unsigned int g_iFrame_fun;//报文解析功能码
extern unsigned char *g_cNewCodeKey;//新密钥的指针
extern unsigned char *g_cSEND_BUF_SIZE;//DMA缓存数据指针

#define SEND_BUF_SIZE 8200	//发送数据长度,最好等于sizeof(TEXT_TO_SEND)+2的整数倍.


#define  CtrlCent_REC_QEUE_LENGTH 2     //CtrlCentRec消息队列长度
#define  UDP_TRAN_QEUE_LENGTH     1     //UDP发送消息队列长度
#define  UDP_Heart_QEUE_LENGTH     1     //UDP发送消息队列长度
#define Test_QEUE_LENGTH					2
//#define  USART_TRAN_QEUE_LENGTH   2     //UART1Tran消息队列长度
#define  PLAY_LIST_QEUE_LENGTH    3     //播放帧表解析消息队列长度
#define  RS485_REC_QEUE_LENGTH    2     //RS485Rec消息队列长度

#define En_Dis_Code_QEUE_LENGTH				4     //加密解密消息队列长度
#define Dis_En_Code_QEUE_LENGTH				4     //加密解密消息队列长度

#define InfmBoardLed_LENGTH				1     //坏点检测消息队列长度




////EEROM 内存管理
////当每次一帧新play.lst发送过来时侯,校验正确时候设置为1，否则设置为0
#define EEROM_ITEM_FLAG_ADDR				0	//1byte 从机FLASH中存有完整play.lst标志位地址
//存储发送完成时，最后一帧数据的最后一次发送校验位
#define EEROM_FRAME_CRC32_ADDR				1	//4byte 从机FLASH中存有完整帧数据最后一帧数据校验位地址
//所有需要发送的帧发送完成时，设置标志位置1
#define EEROM_ITEM_UPDATE_FLAG_ADDR			5	//1byte play.lst更新完成标志位地址
//屏体参数/从控制区域参数/模组/IP参数设置完成标志位地址
#define EEROM_DATA_SET_FLAG_ADDR			6	//1byte 
//屏体参数/从控制区域参数/模组/IP参数起始地址
#define EEROM_DATA_SET_START_ADDR			7	//69byte
//亮度等级标志位存储地址
#define EEROM_BRIGHT_LEVEL_FLAG_ADDR		  80	//1byte
//最大亮度比例存储地址
#define EEROM_MAX_BRIGHT_PROPORTION_ADDR	81	//1byte	
//预制播放表标志位存储地址
#define EEROM_YUZHIFLAG_ADDR				82	//1byte
//预制播放序号
#define EEROM_YUZHIXUHAO_ADDR				83	//2byte
//网络连接方式
#define EEROM_NET_MODE_ADDR					85  //1byte
//远端端口号
#define EEROM_REMOTE_PORT_ADDR			86	//2byte
//远端IP
#define EEROM_REMOTE_IP_ADDR				88	//4byte
#define EEROM_DATA_SET_REBOOT       127
#define EEROM_DATA_TIME_MONTH       129  //1
//*******串口屏 屏参设置标志位***********/
//通信参数设置
#define  EEROM_USART_BPS_DATA_ADDR       130  //1
#define  EEROM_USART_IP_ADDR         131  //1
#define  EEROM_USART_SCREEN_ADDR        132  //1
#define  EEROM_TCP_OR_UDP_ADDR        133  //1
#define  EEROM_IAP_ADDR           134  //1
//本机IP地址
#define EEROM_Local_IP_ADDR				135	//4byte
//本机端口号
#define EEROM_Port_ADDR						140//2byte
//本机网关
#define EEROM_Gateway_ADDR				142//4Byte
//IP白名单
#define EEROM_White_1 150//4Byet
#define EEROM_White_2 155//4Byet
#define EEROM_White_3 160//4Byet
#define EEROM_White_4 165//4Byet
#define EEROM_White_5 170//4Byet
//串口波特率
#define EEROM_USART_BPS 171//6Byet

extern uint8_t  spare_flag;      //不同按键 预置播放表 标志位
extern uint32_t sys_runtime;
extern uint8_t  slave_rst_flag;
extern uint8_t  max_bright_proportion;
extern uint32_t slave_communication;
extern uint8_t  checkself_flag;
extern uint8_t  temporary_flag;
extern uint16_t temporary_time;
extern uint32_t temporary_runtime;
extern uint8_t  ScreenRatio;;	


////情报板需要设置的参数如下：--每次上电初始化时候如果FLASH中存储有这些数据
////则这些数据会进行更新
////主控制器与控制中心进行通信的变量
extern   ScreenBoxSetStr  G_ScreenSet;  //结构体在sw_host.h中定义
extern   ScreenBoxSetStr  G_BoxSet;     //结构体在sw_host.h中定义
extern   INT8U	HostAddrH;
extern   INT8U	HostAddrL;            //主机在控制中心处的地址
extern   INT8U	G_MoZuType;           //模组类别：0-8X4,1-8X8,2-8X16

extern   INT32U	G_BaudRate;           //与控制中心UART1通信波特率-默认

extern   INT8U	IPAddress[];          //IP地址-默认
extern   INT8U	NetMask[];            //子网掩码-默认
extern   INT8U	GetWay[];             //网关-默认
extern   INT16U	LocalPort;              //UDP端口号-默认

extern   uint8_t	PlayNum;
extern   uint8_t	NOWPlayNum;
extern   INT8U	G_SlaveNum;           //从机个数
extern   INT8U	G_DataSetFlag;        //参数设置标志位
extern   INT8U	G_UpCpuFlag;          //重新显示正常内容
extern   INT8U	KeyFlag;               //键值
extern   INT8U	Keydebug;              //纠错标志


////主控制器与从控制器进行通信的地址
extern	INT8U    G_IDHost;
extern	INT8U    UDPResetFlag;
extern	INT8U    host_Flag;
extern	uint16_t SCREEN_HANG_FH; //返回行
extern	uint16_t last_CRC; //返回行
extern	uint16_t load_CRC; //返回行


extern   void     * RS485RecMsgQeueTab[];
extern   void     * RS485TranMsgQeueTab[]; 
extern   void     * CtrlCentRecMsgQeueTab[]; 
extern   OS_EVENT * CtrlCentRecMsgQeue;     //CtrlCent接收数据的消息队列

extern   OS_EVENT * UsartTranSem;           //UART1资源信号量，用来申请使用UART1


extern   void     * UdpTranMsgQeueTab[]; 
extern   OS_EVENT * UdpTranMsgQeue;         //UdpTranMsg消息队列

extern void     * UdpHearbeatQeueTab[UDP_Heart_QEUE_LENGTH]; 
extern OS_EVENT * InfmBoard_ErrQeue;          //故障建册消息队列

extern void     * InfmBoardLed_ErrTab[InfmBoardLed_LENGTH];
extern OS_EVENT * InfmBoardLed_ErrQeue;

//加密解密
extern   void     * En_Dis_CodeQeueTab[En_Dis_Code_QEUE_LENGTH];
extern   OS_EVENT * En_Dis_CodeQeue;//加密解密消息队列
extern   void     * Dis_En_CodeQeueTab[Dis_En_Code_QEUE_LENGTH];
extern   OS_EVENT * Dis_En_CodeQeue;//加密解密消息队列


//extern   OS_EVENT * UsartTranMsgQeue;       //发送数据的消息队列	 
extern   INT16U     UsartTranCnt;           //已经发的数据计数器
extern   INT16S     UsartTranLength;        //要发送的数据长度
extern   uint8_t  * UsartTranDataPot;       //要发送的数据指针	
//	 
extern   OS_EVENT * RS485RecMsgQeue;        //RS485接收数据的消息队列
extern   OS_EVENT * RS485TranMsgQeue;       //RS485发送数据的消息队列
extern   OS_EVENT * RS485Sem;               //RS485发送资源信号量
extern   OS_EVENT * SDSem;
extern 	 OS_EVENT * HearbeatSem;
extern   INT16U     RS485TranCnt;           //RS485已经发的数据计数器
extern   INT16S     RS485TranLength;        //RS485要发送的数据长度
extern   uint8_t  * RS485TranDataPot;       //RS485要发送的数据指针
extern   INT8U      G_TranFrameSlaveCnt;  //485发送完成从机计数器
extern   uint8_t  AnaSem_flag;
extern   uint8_t  Ana_flag;					//正在解析标志位


extern  void     * R485DataTab[];
extern   OS_EVENT * R485Data;


////播放帧表解析
////播放帧表解析消息指针数组，队列长度为5
extern   void     * PlayListMsgQeueTab[]; 
extern   OS_EVENT * PlayListMsgQeue;        //播放帧表消息队列
extern   BYTE Itemlisttab[800]; //播放帧表缓存区



////情报板亮度参数-这个参数应该存储在24C02中
extern	uint8_t		BrightMod;           //亮度调节方式 0-自动/1-手动
extern	int				BrightValue;         //手动调节亮度值 0-100
extern	uint16_t	BrightPwm;           //pwm占空值0-128  
extern	uint16_t	BrightAD;           //AD 

extern  uint16_t	SUV_pwm;                 //亮度pwm值 传递
extern  uint8_t 	YuZhiFlag;               //预制播放标志位
extern  INT32U 	  YuZhiXuHao;             //预置文件序号

////播放帧表解析RGB缓存-帧号+停留时间+显示方式+移动速度+RGB缓存
extern   ITEM_STRUCT   ItemBuffer;


////24C02存储的标志位
extern   uint8_t  G_ItemFlag;            //flash中存有完整play.lst数据标志位
extern   uint8_t  G_ItemUpdateFlag;      //帧数据更新到从机完毕标志位
extern   uint32_t G_FrameCRC32;          //校验结果

extern uint8_t  G_ItemUpdateCnt ;         //发送帧更新计数
extern   uint8_t  G_TranFrameCnt;        //发送帧计数器
extern   uint8_t  G_DisplayFlag;         //开始显示标志位

 




//故障标志位
extern uint8_t TempSt;    //温度故障状态字
extern uint8_t LdrSt;     //光敏故障状态字
extern uint8_t DoorSt;    //箱门故障状态字
extern uint8_t DoorStflag;    //箱门故障标志位

extern uint8_t resetok[10];


extern uint16_t 	DoorStflag1[];    //箱门故障标志位


extern uint8_t    screenflag;
extern uint8_t    downele_flag;
extern uint32_t 	  offset;

extern uint8_t  	keystate;


extern uint8_t   errtype;
extern uint16_t  Period ;
extern uint16_t  Prescaler ;
extern uint8_t   StoreFlag;
extern uint32_t  Tcpdatalen;
extern INT8U    Hostdata1 ;
extern INT8U    Hostdata2 ;



extern uint8_t   rec_2[20];
extern uint8_t   recflag;
extern uint8_t   usartflag;
extern uint8_t   TCPorUDPFlag;
extern uint8_t   TcpReciveFlag;
//#ifdef __cplusplus
//}
//#endif

extern unsigned char g_cTxtUp;//更新标志位

#endif

