#include "sw_config.h"

uint8_t	SlaveAckOKBuf[24];//��Ӧ����������
uint8_t	SlaveAckFaultBuf[24];//����6
//RGB���ݻ���
uint8_t    DisplayBuf[3][4096]; 

//�����ӻ���ַ
uint8_t    	G_IDSlave0  = 0;          //�ӻ���ַ
uint8_t    	G_IDHost = 0;             //������ַ

//����״̬���
uint8_t Door1=1;
uint8_t Door2=1;
uint8_t Door3=1;
uint8_t Door4=1;
uint8_t Door5=1;
uint8_t Door6=1;
uint8_t Door7=1;	//����220V���������
//24C02�洢�ı�־λ
uint8_t G_FrameFlag = 0;           //flash�д����������ݱ�־λ
uint32_t G_FrameCRC32 = 0;         //У����

//�ӻ���Ҫ���õĲ������£�
uint8_t    ScreenBoxSetFlag = 0;
ScreenBoxSetStr  G_ScreenSet; //������Ļ�Ĳ���
ScreenBoxSetStr  G_BoxSet;		//�Ӱ�Ĳ���
uint8_t    G_MoZuType = MZ8X8;           //ģ�����0-8X4,1-8X8,2-8X16

//�ӻ�Ӧ��SlaveAckOKBuf-Ϊ�˼ӿ�Ӧ����Ӧ�ٶ�
//uint8_t     SlaveAckOKBuf[20];

uint8_t    G_FrameNum = 0;           //���ű����ܵ�֡��
uint8_t    G_NewFramesFlag = 1;      //����֡���ݱ�־λ
uint8_t    G_LoadDataFlag = 0;       //װ��������ɱ�־λ

LoadDataNode LoadNodeTab[50];   //��Ч֡����ͷ��

unsigned char USART3_BUF[USART3_BUF_SIZE];//DMA������������

//��������ʾ��Χ�ṹ�����
ShowAreaTypedef ShowAreaStru;


//RGB���ݻ���
uint8_t    DisplayBuf[DIS_BUF_CLR][DIS_BUF_LEN];  

//RS485-UART3��Դ����--���Ͳ���
//RS485-UART3��Ϣָ�����飬���г���Ϊ7
uint16_t     RS485TranCnt  = 0;      //RS485�Ѿ��������ݼ�����
uint16_t     RS485TranLength = 0;    //RS485Ҫ���͵����ݳ���
uint8_t    * RS485TranDataPot = 0;   //RS485Ҫ���͵�����ָ��

