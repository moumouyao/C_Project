#include "sw_config.h"

uint8_t	SlaveAckOKBuf[24];//回应主机的数组
uint8_t	SlaveAckFaultBuf[24];//命令6
//RGB数据缓存
uint8_t    DisplayBuf[3][4096]; 

//主机从机地址
uint8_t    	G_IDSlave0  = 0;          //从机地址
uint8_t    	G_IDHost = 0;             //主机地址

//箱门状态监测
uint8_t Door1=1;
uint8_t Door2=1;
uint8_t Door3=1;
uint8_t Door4=1;
uint8_t Door5=1;
uint8_t Door6=1;
uint8_t Door7=1;	//输入220V交流电故障
//24C02存储的标志位
uint8_t G_FrameFlag = 0;           //flash中存有完整数据标志位
uint32_t G_FrameCRC32 = 0;         //校验结果

//从机需要设置的参数如下：
uint8_t    ScreenBoxSetFlag = 0;
ScreenBoxSetStr  G_ScreenSet; //整个屏幕的参数
ScreenBoxSetStr  G_BoxSet;		//从板的参数
uint8_t    G_MoZuType = MZ8X8;           //模组类别：0-8X4,1-8X8,2-8X16

//从机应答SlaveAckOKBuf-为了加快应答响应速度
//uint8_t     SlaveAckOKBuf[20];

uint8_t    G_FrameNum = 0;           //播放表中总的帧数
uint8_t    G_NewFramesFlag = 1;      //更新帧数据标志位
uint8_t    G_LoadDataFlag = 0;       //装载数据完成标志位

LoadDataNode LoadNodeTab[50];   //有效帧加载头表

unsigned char USART3_BUF[USART3_BUF_SIZE];//DMA缓存数据数组

//本箱体显示范围结构体变量
ShowAreaTypedef ShowAreaStru;


//RGB数据缓存
uint8_t    DisplayBuf[DIS_BUF_CLR][DIS_BUF_LEN];  

//RS485-UART3资源变量--发送部分
//RS485-UART3消息指针数组，队列长度为7
uint16_t     RS485TranCnt  = 0;      //RS485已经发的数据计数器
uint16_t     RS485TranLength = 0;    //RS485要发送的数据长度
uint8_t    * RS485TranDataPot = 0;   //RS485要发送的数据指针

