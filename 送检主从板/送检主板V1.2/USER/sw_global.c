/**
  ******************************************************************************
  * �ļ��� : sw_global.c
  * ��  �� : �¹�
  * ��  �� ��V1.0
  * ʱ  �� : 2014��7��29��
  * ��  ˾ ��������缼�����޹�˾
  * ժ  Ҫ �����ļ�������������Ҫ�õ���ȫ�ֱ���
	*
  ******************************************************************************
  */
#include "includes.h"
#include "sw_global.h"
#include "malloc.h"

//====================2022 6 20 ������������������===============//
unsigned char g_cTcpClientState=1;//����ͻ��˵�״̬ 1=������ 0=�ǰ�����
unsigned char *g_cIpWhiteList[WhiteListNum];//������(ָ��ָ�������Ϊ4*8bit �ֱ��ӦIP��ַ�ĸ��ֽ�)
unsigned char g_cIp[4] = {192,168,1,5};//��ҳ�޸ĵ�IP(��ʼ����)
unsigned short g_cPort = 8001;//��ҳ�˵Ķ˿�(��ʼ����)
unsigned char g_cGateway[4] = {192,168,1,1};//���أ���ʼ���ã�
unsigned char g_cSysRst=0;//ϵͳ��Ҫ��λ��־λ����ϵͳ�Ƶ�λ�ø�λ��
unsigned char *g_cRemot_addr;//Զ��IP��ַ
unsigned int g_iFrame_fun=0xffff;//���Ľ���������
unsigned char *g_cNewCodeKey=NULL;//����Կ��ָ��
unsigned char *g_cSEND_BUF_SIZE;//DMA��������ָ��



////����֡�����RGB����-֡��+ͣ��ʱ��+��ʾ��ʽ+�ƶ��ٶ�+RGB����
ITEM_STRUCT   ItemBuffer;


uint32_t sys_runtime = 0;
uint8_t  slave_rst_flag=0;//�Ӱ���ϸ�λ��־λ
uint8_t  max_bright_proportion = 60;//������Ȱٷֱ�
uint8_t  ScreenRatio;//���峤�����
uint32_t slave_communication= 0;

uint8_t checkself_flag = 40;//�������Ӱ�������Ϊȫ����ʾ

//24C02�洢�ı�־λ
uint8_t  G_ItemFlag = 0;            //flash�д�������play.lst���ݱ�־λ
INT8U    G_ItemUpdateFlag = 0;      //֡���ݸ��µ��ӻ���ϱ�־λ
uint32_t G_FrameCRC32 = 0;          //У����

uint8_t  G_ItemUpdateCnt = 0;      //����֡���¼���
uint8_t  G_TranFrameCnt = 0;        //����֡������
uint8_t  G_DisplayFlag = 0;         //��ʼ��ʾ��־λ
uint8_t  temporary_flag = 0;			//��ʱ���ű�
uint16_t temporary_time = 0;			//��ʱ���ű�ı�ʶ
uint32_t temporary_runtime = 0;		//�岥ʱ��
//�鱨����Ҫ���õĲ������£�--ÿ���ϵ��ʼ��ʱ�����EEPROM�д洢����Щ����
//����Щ���ݻ���и���-������EEPROM�еĵ�ַ����sw_host.h�ж���
//����������������Ľ���ͨ�ŵı���




INT8U    HostAddrH = '0';
INT8U    HostAddrL = '0';             //�����ڿ������Ĵ��ĵ�ַ-Ĭ��
INT8U    UDPResetFlag=0 ;

ScreenBoxSetStr  G_ScreenSet;         //�ṹ����sw_host.h�ж��� //������Ļ
ScreenBoxSetStr  G_BoxSet;            //�ṹ����sw_host.h�ж��� //ĳ���Ӱ�
INT8U    G_MoZuType = 1;              //ģ�����0-8X4,1-8X8,2-8X16

INT32U   G_BaudRate = 9600;         //���������UART1ͨ�Ų�����-Ĭ��

INT8U    IPAddress[4] = {192,168,1,5};   //IP��ַ-Ĭ��
INT8U    NetMask[4] = {255,255,255,0};  //��������-Ĭ��
INT8U    GetWay[4] = {192,168,1,1};       //����-Ĭ��
INT16U   LocalPort = 8001;                //UDP�˿ں�-Ĭ��


INT8U    G_SlaveNum = 0;              //�ӻ�����
INT8U	   KeyFlag = 0; 
INT8U    G_DataSetFlag = 0;           //�������ñ�־λ--ֻ�ڸտ�ʼ�ϵ�ʱ��BSP.C�н��и���
INT8U    G_UpCpuFlag=0;
INT8U	   Keydebug =0;

//����������ӿ���������ͨ�ŵĵ�ַ

   

INT8U  G_IDHost = 0;
INT8U  host_Flag=0;               //Ӳ������
INT8U  YuZhiFlag=0;               //Ԥ�Ʋ��ű�־λ
INT32U YuZhiXuHao=0;             //Ԥ���ļ����
INT16U last_CRC=1;
INT16U load_CRC=0;



//CtrlCent��Ϣָ�����飬���г���Ϊ4
void     * CtrlCentRecMsgQeueTab[CtrlCent_REC_QEUE_LENGTH];// __attribute__((at(0X69000000+MEM2_MAX_SIZE+MEM2_ALLOC_TABLE_SIZE))); 
OS_EVENT * CtrlCentRecMsgQeue;      //CtrlCent�������ݵ���Ϣ����



//UdpTranMsgQeue��Ϣָ�����飬���г���Ϊ4
void     * UdpTranMsgQeueTab[UDP_TRAN_QEUE_LENGTH]; 
OS_EVENT * UdpTranMsgQeue;          //UdpTranMsg��Ϣ����

//UdpHearbeatQeue��Ϣָ�����飬���г���Ϊ1
void     * UdpHearbeatQeueTab[UDP_Heart_QEUE_LENGTH]; 
OS_EVENT * InfmBoard_ErrQeue;          //UdpTranMsg��Ϣ����

//TestQueue��Ϣָ������ ���г���Ϊ4
void     * TestQueueTab[Test_QEUE_LENGTH];
OS_EVENT * TestQueue;

//InfmBoardLed_ErrQeue��Ϣָ������ ���г���Ϊ1
void     * InfmBoardLed_ErrTab[InfmBoardLed_LENGTH];
OS_EVENT * InfmBoardLed_ErrQeue;


//UART1��Դ����--���Ͳ���
//UART1��Ϣָ�����飬���г���Ϊ5
//void     * UsartTranMsgQeueTab[USART_TRAN_QEUE_LENGTH]; 
OS_EVENT * UsartTranSem;           //UART1������Դ�ź�������������ʹ��UART1����
//OS_EVENT * UsartTranMsgQeue;       //UART1�������ݵ���Ϣ����
INT16U     UsartTranCnt  = 0;      //Uart1�Ѿ��������ݼ�����
INT16S     UsartTranLength = 0;    //Uart1Ҫ���͵����ݳ���
INT8U    * UsartTranDataPot = 0;   //Uart1Ҫ���͵�����ָ��


//RS485-UART2��Դ����--���ղ���
//RS485-UART2��Ϣָ�����飬���г���Ϊ7
void     * RS485RecMsgQeueTab[RS485_REC_QEUE_LENGTH]; 
OS_EVENT * RS485RecMsgQeue;        //UART1�������ݵ���Ϣ����




//RS485-UART2��Դ����--���Ͳ���
//RS485-UART2��Ϣָ�����飬���г���Ϊ7
//void     * RS485TranMsgQeueTab[RS485_TRAN_QEUE_LENGTH]; 
OS_EVENT * RS485TranMsgQeue;       //RS485�������ݵ���Ϣ����
OS_EVENT * RS485Sem;               //RS485��Դ�ź���
OS_EVENT * SDSem;
OS_EVENT * HearbeatSem;							//�������ź���
INT16U     RS485TranCnt  = 0;      //RS485�Ѿ��������ݼ�����
INT16S     RS485TranLength = 0;    //RS485Ҫ���͵����ݳ���
INT8U    * RS485TranDataPot = 0;   //RS485Ҫ���͵�����ָ��

void     * R485DataTab[2];
OS_EVENT * R485Data;



//����֡�����
//����֡�������Ϣָ�����飬���г���Ϊ5
void     * PlayListMsgQeueTab[PLAY_LIST_QEUE_LENGTH]; 
OS_EVENT * PlayListMsgQeue;        //����֡����Ϣ����

//���ܽ���
void     * En_Dis_CodeQeueTab[En_Dis_Code_QEUE_LENGTH];
OS_EVENT * En_Dis_CodeQeue;//���ܽ�����Ϣ����
void     * Dis_En_CodeQeueTab[Dis_En_Code_QEUE_LENGTH];
OS_EVENT * Dis_En_CodeQeue;//���ܽ�����Ϣ����


//�鱨�����Ȳ���-�������Ӧ�ô洢��24C02��
//���ȵ������жϴ�����SysTick_Handler����
uint8_t		  BrightMod = 0;            //���ȵ��ڷ�ʽ 0-�Զ�/1-�ֶ�
int					BrightValue = 0;          //�ֶ���������ֵ 0-100 
uint16_t		BrightPwm = 0;            //pwmռ��ֵ0-128     
uint16_t    SUV_pwm=0;

//��������ź���
uint8_t  AnaSem_flag=0;
uint8_t  Ana_flag=0;
uint8_t resetok[10];

uint8_t   screenflag=1;
uint32_t  offset;
uint8_t   downele_flag=0;
uint8_t   spare_flag=0; 
uint8_t   keystate;
uint8_t   serverflag=0;
uint32_t  server_run[2] = {0,100}; //��ѯ��������
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
uint8_t   TcpReciveFlag = 0; //tcp���մ����ݰ� ��־λ


unsigned char g_cTxtUp;//���±�־λ

