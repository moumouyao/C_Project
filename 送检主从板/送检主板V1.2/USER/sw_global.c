/**
  ******************************************************************************
  * 文件名 : sw_global.c
  * 作  者 : 陈光
  * 版  本 ：V1.0
  * 时  间 : 2014年7月29日
  * 公  司 ：汉威光电技术有限公司
  * 摘  要 ：本文件包含了我们需要用到的全局变量
	*
  ******************************************************************************
  */
#include "includes.h"
#include "sw_global.h"
#include "malloc.h"

//====================2022 6 20 白名单功能新增变量===============//
unsigned char g_cTcpClientState=1;//接入客户端的状态 1=白名单 0=非白名单
unsigned char *g_cIpWhiteList[WhiteListNum];//白名单(指针指向的数组为4*8bit 分别对应IP地址四个字节)
unsigned char g_cIp[4] = {192,168,1,5};//网页修改的IP(初始化用)
unsigned short g_cPort = 8001;//网页端的端口(初始化用)
unsigned char g_cGateway[4] = {192,168,1,1};//网关（初始化用）
unsigned char g_cSysRst=0;//系统需要复位标志位，在系统灯的位置复位。
unsigned char *g_cRemot_addr;//远程IP地址
unsigned int g_iFrame_fun=0xffff;//报文解析功能码
unsigned char *g_cNewCodeKey=NULL;//新密钥的指针
unsigned char *g_cSEND_BUF_SIZE;//DMA缓存数据指针



////播放帧表解析RGB缓存-帧号+停留时间+显示方式+移动速度+RGB缓存
ITEM_STRUCT   ItemBuffer;


uint32_t sys_runtime = 0;
uint8_t  slave_rst_flag=0;//从板故障复位标志位
uint8_t  max_bright_proportion = 60;//最大亮度百分比
uint8_t  ScreenRatio;//屏体长宽比例
uint32_t slave_communication= 0;

uint8_t checkself_flag = 40;//大于最大从版数量即为全屏显示

//24C02存储的标志位
uint8_t  G_ItemFlag = 0;            //flash中存有完整play.lst数据标志位
INT8U    G_ItemUpdateFlag = 0;      //帧数据更新到从机完毕标志位
uint32_t G_FrameCRC32 = 0;          //校验结果

uint8_t  G_ItemUpdateCnt = 0;      //发送帧更新计数
uint8_t  G_TranFrameCnt = 0;        //发送帧计数器
uint8_t  G_DisplayFlag = 0;         //开始显示标志位
uint8_t  temporary_flag = 0;			//临时播放表
uint16_t temporary_time = 0;			//临时播放表的标识
uint32_t temporary_runtime = 0;		//插播时间
//情报板需要设置的参数如下：--每次上电初始化时候如果EEPROM中存储有这些数据
//则这些数据会进行更新-具体在EEPROM中的地址是在sw_host.h中定义
//主控制器与控制中心进行通信的变量




INT8U    HostAddrH = '0';
INT8U    HostAddrL = '0';             //主机在控制中心处的地址-默认
INT8U    UDPResetFlag=0 ;

ScreenBoxSetStr  G_ScreenSet;         //结构体在sw_host.h中定义 //整个屏幕
ScreenBoxSetStr  G_BoxSet;            //结构体在sw_host.h中定义 //某个从板
INT8U    G_MoZuType = 1;              //模组类别：0-8X4,1-8X8,2-8X16

INT32U   G_BaudRate = 9600;         //与控制中心UART1通信波特率-默认

INT8U    IPAddress[4] = {192,168,1,5};   //IP地址-默认
INT8U    NetMask[4] = {255,255,255,0};  //子网掩码-默认
INT8U    GetWay[4] = {192,168,1,1};       //网关-默认
INT16U   LocalPort = 8001;                //UDP端口号-默认


INT8U    G_SlaveNum = 0;              //从机个数
INT8U	   KeyFlag = 0; 
INT8U    G_DataSetFlag = 0;           //参数设置标志位--只在刚开始上电时候BSP.C中进行更新
INT8U    G_UpCpuFlag=0;
INT8U	   Keydebug =0;

//主控制器与从控制器进行通信的地址

   

INT8U  G_IDHost = 0;
INT8U  host_Flag=0;               //硬件故障
INT8U  YuZhiFlag=0;               //预制播放标志位
INT32U YuZhiXuHao=0;             //预置文件序号
INT16U last_CRC=1;
INT16U load_CRC=0;



//CtrlCent消息指针数组，队列长度为4
void     * CtrlCentRecMsgQeueTab[CtrlCent_REC_QEUE_LENGTH];// __attribute__((at(0X69000000+MEM2_MAX_SIZE+MEM2_ALLOC_TABLE_SIZE))); 
OS_EVENT * CtrlCentRecMsgQeue;      //CtrlCent接收数据的消息队列



//UdpTranMsgQeue消息指针数组，队列长度为4
void     * UdpTranMsgQeueTab[UDP_TRAN_QEUE_LENGTH]; 
OS_EVENT * UdpTranMsgQeue;          //UdpTranMsg消息队列

//UdpHearbeatQeue消息指针数组，队列长度为1
void     * UdpHearbeatQeueTab[UDP_Heart_QEUE_LENGTH]; 
OS_EVENT * InfmBoard_ErrQeue;          //UdpTranMsg消息队列

//TestQueue消息指针数组 队列长度为4
void     * TestQueueTab[Test_QEUE_LENGTH];
OS_EVENT * TestQueue;

//InfmBoardLed_ErrQeue消息指针数组 队列长度为1
void     * InfmBoardLed_ErrTab[InfmBoardLed_LENGTH];
OS_EVENT * InfmBoardLed_ErrQeue;


//UART1资源变量--发送部分
//UART1消息指针数组，队列长度为5
//void     * UsartTranMsgQeueTab[USART_TRAN_QEUE_LENGTH]; 
OS_EVENT * UsartTranSem;           //UART1发送资源信号量，用来申请使用UART1发送
//OS_EVENT * UsartTranMsgQeue;       //UART1接收数据的消息队列
INT16U     UsartTranCnt  = 0;      //Uart1已经发的数据计数器
INT16S     UsartTranLength = 0;    //Uart1要发送的数据长度
INT8U    * UsartTranDataPot = 0;   //Uart1要发送的数据指针


//RS485-UART2资源变量--接收部分
//RS485-UART2消息指针数组，队列长度为7
void     * RS485RecMsgQeueTab[RS485_REC_QEUE_LENGTH]; 
OS_EVENT * RS485RecMsgQeue;        //UART1接收数据的消息队列




//RS485-UART2资源变量--发送部分
//RS485-UART2消息指针数组，队列长度为7
//void     * RS485TranMsgQeueTab[RS485_TRAN_QEUE_LENGTH]; 
OS_EVENT * RS485TranMsgQeue;       //RS485接收数据的消息队列
OS_EVENT * RS485Sem;               //RS485资源信号量
OS_EVENT * SDSem;
OS_EVENT * HearbeatSem;							//心跳包信号量
INT16U     RS485TranCnt  = 0;      //RS485已经发的数据计数器
INT16S     RS485TranLength = 0;    //RS485要发送的数据长度
INT8U    * RS485TranDataPot = 0;   //RS485要发送的数据指针

void     * R485DataTab[2];
OS_EVENT * R485Data;



//播放帧表解析
//播放帧表解析消息指针数组，队列长度为5
void     * PlayListMsgQeueTab[PLAY_LIST_QEUE_LENGTH]; 
OS_EVENT * PlayListMsgQeue;        //播放帧表消息队列

//加密解密
void     * En_Dis_CodeQeueTab[En_Dis_Code_QEUE_LENGTH];
OS_EVENT * En_Dis_CodeQeue;//加密解密消息队列
void     * Dis_En_CodeQeueTab[Dis_En_Code_QEUE_LENGTH];
OS_EVENT * Dis_En_CodeQeue;//加密解密消息队列


//情报板亮度参数-这个参数应该存储在24C02中
//亮度调节在中断处理中SysTick_Handler进行
uint8_t		  BrightMod = 0;            //亮度调节方式 0-自动/1-手动
int					BrightValue = 0;          //手动调节亮度值 0-100 
uint16_t		BrightPwm = 0;            //pwm占空值0-128     
uint16_t    SUV_pwm=0;

//解析打断信号量
uint8_t  AnaSem_flag=0;
uint8_t  Ana_flag=0;
uint8_t resetok[10];

uint8_t   screenflag=1;
uint32_t  offset;
uint8_t   downele_flag=0;
uint8_t   spare_flag=0; 
uint8_t   keystate;
uint8_t   serverflag=0;
uint32_t  server_run[2] = {0,100}; //轮询检测服务器
uint16_t   keyrun[2]={0,10};
INT8U     Hostdata1 ;
INT8U     Hostdata2 ;    
uint8_t   errtype;
uint16_t  Period =127;
uint16_t  Prescaler = 128;
uint8_t   StoreFlag =0;
uint32_t  Tcpdatalen =0;

uint8_t   rec_2[20]={0};
uint8_t   recflag = 0;
uint8_t   usartflag = 0;

uint8_t   TCPorUDPFlag = 1;   //tcp ---1    udp---0
uint8_t   TcpReciveFlag = 0; //tcp接收大数据包 标志位


unsigned char g_cTxtUp;//更新标志位

