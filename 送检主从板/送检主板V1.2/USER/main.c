#include "bsp.h"
#include "ucos_ii.h"
#include "app_cfg.h"
#include "lwip_app.h"
#include "app.h"
#include "ctrlcenter_app.h"
#include "host_app.h"
#include "dis_app.h"
#include "sw_global.h"
#include "lwip_comm.h" 
#include "includes.h"
#include "lwipopts.h"
#include "tcp_server_demo.h"
#include "udp_demo.h"
#include "stmflash.h"
#include "iap.h" 
#include "malloc.h"
#include "httpd.h"
#include "Code.h"


//�����ջ
	OS_STK task_led1_stk[TASK_LED1_STK_SIZE]; //ϵͳ�ȶ�ջ
	OS_STK task_cch_stk[TASK_CCH_STK_SIZE];   //����ջ - ���ݴ���
	OS_STK task_rpl_stk[TASK_RPL_STK_SIZE];       //����ջ--���ݽ���
	OS_STK task_dis_ctrl_stk[TASK_DIS_CTRL_SIZE]; //����ջ--������ʾ
	OS_STK task_Code_stk[TASK_CODE_SIZE];//���ܽ��������ջ
	OS_STK key_Code_stk[KEY_CODE_SIZE];//���������ջ
int main(void)
{
	SCB->VTOR = FLASH_BASE | 0x10000;//����ƫ����
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//����ϵͳ�ж����ȼ�����2
  BSP_Init();
	OSInit(); 					//UCOS��ʼ��
  
    /* ����UdpTran��Ϣ���� */
  UdpTranMsgQeue = OSQCreate(&UdpTranMsgQeueTab[0],UDP_TRAN_QEUE_LENGTH);
 /* ����CtrlCentRec��Ϣ���� */
  CtrlCentRecMsgQeue = OSQCreate(&CtrlCentRecMsgQeueTab[0],CtrlCent_REC_QEUE_LENGTH);
  /* ����PlayListMsgQeue��Ϣ���� */
  PlayListMsgQeue = OSQCreate(&PlayListMsgQeueTab[0],PLAY_LIST_QEUE_LENGTH);
  /* ����RS485Rec��Ϣ���� */
  RS485RecMsgQeue = OSQCreate(&RS485RecMsgQeueTab[0],RS485_REC_QEUE_LENGTH); 

 /* ����InfmBoard_ErrQeue��Ϣ���� */
  InfmBoard_ErrQeue = OSQCreate(&UdpHearbeatQeueTab[0],UDP_Heart_QEUE_LENGTH);
	
 /* ����InfmBoard_ErrQeue��Ϣ���� */
  InfmBoardLed_ErrQeue = OSQCreate(&UdpHearbeatQeueTab[0],UDP_Heart_QEUE_LENGTH);
	
	/*�������ܽ�����Ϣ����*/
#if EnableCode
	En_Dis_CodeQeue = OSQCreate(&En_Dis_CodeQeueTab[0],En_Dis_Code_QEUE_LENGTH);//������λ��->��λ��
	Dis_En_CodeQeue = OSQCreate(&Dis_En_CodeQeueTab[0],Dis_En_Code_QEUE_LENGTH);//������λ��->��λ��
#endif
  /* ����RS485Sem��Դ�ź��� */
  RS485Sem = OSSemCreate(1); 
//  //===============��Դ�ź���=============//
  UsartTranSem = OSSemCreate(1);     //����UART1������Դ�ź���
  SDSem = OSSemCreate(1);
	HearbeatSem = OSSemCreate(1);		//�����������ź���
	while(lwip_comm_init()); 	//lwip��ʼ��
	//=============��ʼ������=====================//
	if(TCPorUDPFlag)
	{
		while(tcp_server_init());//��ʼ��TCP�߳�	
	}
	else
	{
		while(udp_demo_init());//��ʼ��UDP�߳�
	}
	
	httpd_init();  //��ʼ��HTTP

  //���ݴ���
  OSTaskCreate(Task_CtrlCenterHandle,(void *)0,
	          &task_cch_stk[TASK_CCH_STK_SIZE-1], TASK_CCH_PRIO);

	//���ݽ���
  OSTaskCreate(Task_ReadPlayList,(void *)0,
	               &task_rpl_stk[TASK_RPL_STK_SIZE-1], TASK_RPL_PRIO);

  //������ʾ
  OSTaskCreate(Task_DisplayCtrl,(void *)0,		  	
              &task_dis_ctrl_stk[TASK_DIS_CTRL_SIZE-1], TASK_DIS_CTRL_PRIO);
	
  //LEDϵͳ��
  OSTaskCreate(Task_LED1,(void *)0,
	          &task_led1_stk[TASK_LED1_STK_SIZE-1], TASK_LED1_PRIO);
	
  //ETC�������Ʋ���Ԥ�Ʋ��ű�
  OSTaskCreate(Task_KEY,(void *)0,
	          &key_Code_stk[KEY_CODE_SIZE-1], KEY_LED1_PRIO);
#if EnableCode
  	//����  ���ܽ���
	OSTaskCreate(TestCode,(void *)0,
						&task_Code_stk[TASK_CODE_SIZE-1],9);
#endif
	OSStart(); //����UCOS
 
	return 0;
}
