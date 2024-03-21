/**
  ******************************************************************************
  * �ļ��� : sw_global.h
  * ��  �� : �¹�
  * ��  �� ��V1.0
  * ʱ  �� : 2014��5��14��
  * ��  ˾ ��������缼�����޹�˾
  * ժ  Ҫ �����ļ�����ȫ�ֱ�������
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
  *                           ȫ�ֱ���������
  ******************************************************************************
  */
#define EnableTest 			0 //�Ƿ�ʹ�����дʼ��
#define EnableCode 			0 //�Ƿ�ʹ�ü���IC
#define EnableLog  			1 //�Ƿ�����־����
#define EnableWhiteList 0 //�Ƿ�������������
#define WhiteListNum 		5 //����������
#define EnableCode      0


#define cJSON_Dbug      1   //json_debug����
//====================2022 6 20 ������������������===============//	 
extern unsigned char g_cTcpClientState;//����ͻ��˵�״̬ 1=������ 0=�ǰ�����
extern unsigned char *g_cIpWhiteList[WhiteListNum];//������
extern unsigned char g_cIp[4];//����IP
extern unsigned short g_cPort;//��ҳ�˵Ķ˿�(��ʼ����)
extern unsigned char g_cGateway[4];
extern unsigned char g_cSysRst;
extern unsigned char *g_cRemot_addr;//Զ��IP��ַ
extern unsigned int g_iFrame_fun;//���Ľ���������
extern unsigned char *g_cNewCodeKey;//����Կ��ָ��
extern unsigned char *g_cSEND_BUF_SIZE;//DMA��������ָ��

#define SEND_BUF_SIZE 8200	//�������ݳ���,��õ���sizeof(TEXT_TO_SEND)+2��������.


#define  CtrlCent_REC_QEUE_LENGTH 2     //CtrlCentRec��Ϣ���г���
#define  UDP_TRAN_QEUE_LENGTH     1     //UDP������Ϣ���г���
#define  UDP_Heart_QEUE_LENGTH     1     //UDP������Ϣ���г���
#define Test_QEUE_LENGTH					2
//#define  USART_TRAN_QEUE_LENGTH   2     //UART1Tran��Ϣ���г���
#define  PLAY_LIST_QEUE_LENGTH    3     //����֡�������Ϣ���г���
#define  RS485_REC_QEUE_LENGTH    2     //RS485Rec��Ϣ���г���

#define En_Dis_Code_QEUE_LENGTH				4     //���ܽ�����Ϣ���г���
#define Dis_En_Code_QEUE_LENGTH				4     //���ܽ�����Ϣ���г���

#define InfmBoardLed_LENGTH				1     //��������Ϣ���г���




////EEROM �ڴ����
////��ÿ��һ֡��play.lst���͹���ʱ��,У����ȷʱ������Ϊ1����������Ϊ0
#define EEROM_ITEM_FLAG_ADDR				0	//1byte �ӻ�FLASH�д�������play.lst��־λ��ַ
//�洢�������ʱ�����һ֡���ݵ����һ�η���У��λ
#define EEROM_FRAME_CRC32_ADDR				1	//4byte �ӻ�FLASH�д�������֡�������һ֡����У��λ��ַ
//������Ҫ���͵�֡�������ʱ�����ñ�־λ��1
#define EEROM_ITEM_UPDATE_FLAG_ADDR			5	//1byte play.lst������ɱ�־λ��ַ
//�������/�ӿ����������/ģ��/IP����������ɱ�־λ��ַ
#define EEROM_DATA_SET_FLAG_ADDR			6	//1byte 
//�������/�ӿ����������/ģ��/IP������ʼ��ַ
#define EEROM_DATA_SET_START_ADDR			7	//69byte
//���ȵȼ���־λ�洢��ַ
#define EEROM_BRIGHT_LEVEL_FLAG_ADDR		  80	//1byte
//������ȱ����洢��ַ
#define EEROM_MAX_BRIGHT_PROPORTION_ADDR	81	//1byte	
//Ԥ�Ʋ��ű��־λ�洢��ַ
#define EEROM_YUZHIFLAG_ADDR				82	//1byte
//Ԥ�Ʋ������
#define EEROM_YUZHIXUHAO_ADDR				83	//2byte
//�������ӷ�ʽ
#define EEROM_NET_MODE_ADDR					85  //1byte
//Զ�˶˿ں�
#define EEROM_REMOTE_PORT_ADDR			86	//2byte
//Զ��IP
#define EEROM_REMOTE_IP_ADDR				88	//4byte
#define EEROM_DATA_SET_REBOOT       127
#define EEROM_DATA_TIME_MONTH       129  //1
//*******������ �������ñ�־λ***********/
//ͨ�Ų�������
#define  EEROM_USART_BPS_DATA_ADDR       130  //1
#define  EEROM_USART_IP_ADDR         131  //1
#define  EEROM_USART_SCREEN_ADDR        132  //1
#define  EEROM_TCP_OR_UDP_ADDR        133  //1
#define  EEROM_IAP_ADDR           134  //1
//����IP��ַ
#define EEROM_Local_IP_ADDR				135	//4byte
//�����˿ں�
#define EEROM_Port_ADDR						140//2byte
//��������
#define EEROM_Gateway_ADDR				142//4Byte
//IP������
#define EEROM_White_1 150//4Byet
#define EEROM_White_2 155//4Byet
#define EEROM_White_3 160//4Byet
#define EEROM_White_4 165//4Byet
#define EEROM_White_5 170//4Byet
//���ڲ�����
#define EEROM_USART_BPS 171//6Byet

extern uint8_t  spare_flag;      //��ͬ���� Ԥ�ò��ű� ��־λ
extern uint32_t sys_runtime;
extern uint8_t  slave_rst_flag;
extern uint8_t  max_bright_proportion;
extern uint32_t slave_communication;
extern uint8_t  checkself_flag;
extern uint8_t  temporary_flag;
extern uint16_t temporary_time;
extern uint32_t temporary_runtime;
extern uint8_t  ScreenRatio;;	


////�鱨����Ҫ���õĲ������£�--ÿ���ϵ��ʼ��ʱ�����FLASH�д洢����Щ����
////����Щ���ݻ���и���
////����������������Ľ���ͨ�ŵı���
extern   ScreenBoxSetStr  G_ScreenSet;  //�ṹ����sw_host.h�ж���
extern   ScreenBoxSetStr  G_BoxSet;     //�ṹ����sw_host.h�ж���
extern   INT8U	HostAddrH;
extern   INT8U	HostAddrL;            //�����ڿ������Ĵ��ĵ�ַ
extern   INT8U	G_MoZuType;           //ģ�����0-8X4,1-8X8,2-8X16

extern   INT32U	G_BaudRate;           //���������UART1ͨ�Ų�����-Ĭ��

extern   INT8U	IPAddress[];          //IP��ַ-Ĭ��
extern   INT8U	NetMask[];            //��������-Ĭ��
extern   INT8U	GetWay[];             //����-Ĭ��
extern   INT16U	LocalPort;              //UDP�˿ں�-Ĭ��

extern   uint8_t	PlayNum;
extern   uint8_t	NOWPlayNum;
extern   INT8U	G_SlaveNum;           //�ӻ�����
extern   INT8U	G_DataSetFlag;        //�������ñ�־λ
extern   INT8U	G_UpCpuFlag;          //������ʾ��������
extern   INT8U	KeyFlag;               //��ֵ
extern   INT8U	Keydebug;              //�����־


////����������ӿ���������ͨ�ŵĵ�ַ
extern	INT8U    G_IDHost;
extern	INT8U    UDPResetFlag;
extern	INT8U    host_Flag;
extern	uint16_t SCREEN_HANG_FH; //������
extern	uint16_t last_CRC; //������
extern	uint16_t load_CRC; //������


extern   void     * RS485RecMsgQeueTab[];
extern   void     * RS485TranMsgQeueTab[]; 
extern   void     * CtrlCentRecMsgQeueTab[]; 
extern   OS_EVENT * CtrlCentRecMsgQeue;     //CtrlCent�������ݵ���Ϣ����

extern   OS_EVENT * UsartTranSem;           //UART1��Դ�ź�������������ʹ��UART1


extern   void     * UdpTranMsgQeueTab[]; 
extern   OS_EVENT * UdpTranMsgQeue;         //UdpTranMsg��Ϣ����

extern void     * UdpHearbeatQeueTab[UDP_Heart_QEUE_LENGTH]; 
extern OS_EVENT * InfmBoard_ErrQeue;          //���Ͻ�����Ϣ����

extern void     * InfmBoardLed_ErrTab[InfmBoardLed_LENGTH];
extern OS_EVENT * InfmBoardLed_ErrQeue;

//���ܽ���
extern   void     * En_Dis_CodeQeueTab[En_Dis_Code_QEUE_LENGTH];
extern   OS_EVENT * En_Dis_CodeQeue;//���ܽ�����Ϣ����
extern   void     * Dis_En_CodeQeueTab[Dis_En_Code_QEUE_LENGTH];
extern   OS_EVENT * Dis_En_CodeQeue;//���ܽ�����Ϣ����


//extern   OS_EVENT * UsartTranMsgQeue;       //�������ݵ���Ϣ����	 
extern   INT16U     UsartTranCnt;           //�Ѿ��������ݼ�����
extern   INT16S     UsartTranLength;        //Ҫ���͵����ݳ���
extern   uint8_t  * UsartTranDataPot;       //Ҫ���͵�����ָ��	
//	 
extern   OS_EVENT * RS485RecMsgQeue;        //RS485�������ݵ���Ϣ����
extern   OS_EVENT * RS485TranMsgQeue;       //RS485�������ݵ���Ϣ����
extern   OS_EVENT * RS485Sem;               //RS485������Դ�ź���
extern   OS_EVENT * SDSem;
extern 	 OS_EVENT * HearbeatSem;
extern   INT16U     RS485TranCnt;           //RS485�Ѿ��������ݼ�����
extern   INT16S     RS485TranLength;        //RS485Ҫ���͵����ݳ���
extern   uint8_t  * RS485TranDataPot;       //RS485Ҫ���͵�����ָ��
extern   INT8U      G_TranFrameSlaveCnt;  //485������ɴӻ�������
extern   uint8_t  AnaSem_flag;
extern   uint8_t  Ana_flag;					//���ڽ�����־λ


extern  void     * R485DataTab[];
extern   OS_EVENT * R485Data;


////����֡�����
////����֡�������Ϣָ�����飬���г���Ϊ5
extern   void     * PlayListMsgQeueTab[]; 
extern   OS_EVENT * PlayListMsgQeue;        //����֡����Ϣ����
extern   BYTE Itemlisttab[800]; //����֡������



////�鱨�����Ȳ���-�������Ӧ�ô洢��24C02��
extern	uint8_t		BrightMod;           //���ȵ��ڷ�ʽ 0-�Զ�/1-�ֶ�
extern	int				BrightValue;         //�ֶ���������ֵ 0-100
extern	uint16_t	BrightPwm;           //pwmռ��ֵ0-128  
extern	uint16_t	BrightAD;           //AD 

extern  uint16_t	SUV_pwm;                 //����pwmֵ ����
extern  uint8_t 	YuZhiFlag;               //Ԥ�Ʋ��ű�־λ
extern  INT32U 	  YuZhiXuHao;             //Ԥ���ļ����

////����֡�����RGB����-֡��+ͣ��ʱ��+��ʾ��ʽ+�ƶ��ٶ�+RGB����
extern   ITEM_STRUCT   ItemBuffer;


////24C02�洢�ı�־λ
extern   uint8_t  G_ItemFlag;            //flash�д�������play.lst���ݱ�־λ
extern   uint8_t  G_ItemUpdateFlag;      //֡���ݸ��µ��ӻ���ϱ�־λ
extern   uint32_t G_FrameCRC32;          //У����

extern uint8_t  G_ItemUpdateCnt ;         //����֡���¼���
extern   uint8_t  G_TranFrameCnt;        //����֡������
extern   uint8_t  G_DisplayFlag;         //��ʼ��ʾ��־λ

 




//���ϱ�־λ
extern uint8_t TempSt;    //�¶ȹ���״̬��
extern uint8_t LdrSt;     //��������״̬��
extern uint8_t DoorSt;    //���Ź���״̬��
extern uint8_t DoorStflag;    //���Ź��ϱ�־λ

extern uint8_t resetok[10];


extern uint16_t 	DoorStflag1[];    //���Ź��ϱ�־λ


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

extern unsigned char g_cTxtUp;//���±�־λ

#endif

