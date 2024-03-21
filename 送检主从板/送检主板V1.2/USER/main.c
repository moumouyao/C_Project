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


//任务堆栈
	OS_STK task_led1_stk[TASK_LED1_STK_SIZE]; //系统等堆栈
	OS_STK task_cch_stk[TASK_CCH_STK_SIZE];   //定义栈 - 数据处理
	OS_STK task_rpl_stk[TASK_RPL_STK_SIZE];       //定义栈--数据解析
	OS_STK task_dis_ctrl_stk[TASK_DIS_CTRL_SIZE]; //定义栈--控制显示
	OS_STK task_Code_stk[TASK_CODE_SIZE];//加密解密任务堆栈
	OS_STK key_Code_stk[KEY_CODE_SIZE];//按键任务堆栈
int main(void)
{
	SCB->VTOR = FLASH_BASE | 0x10000;//设置偏移量
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置系统中断优先级分组2
  BSP_Init();
	OSInit(); 					//UCOS初始化
  
    /* 创建UdpTran消息队列 */
  UdpTranMsgQeue = OSQCreate(&UdpTranMsgQeueTab[0],UDP_TRAN_QEUE_LENGTH);
 /* 创建CtrlCentRec消息队列 */
  CtrlCentRecMsgQeue = OSQCreate(&CtrlCentRecMsgQeueTab[0],CtrlCent_REC_QEUE_LENGTH);
  /* 创建PlayListMsgQeue消息队列 */
  PlayListMsgQeue = OSQCreate(&PlayListMsgQeueTab[0],PLAY_LIST_QEUE_LENGTH);
  /* 创建RS485Rec消息队列 */
  RS485RecMsgQeue = OSQCreate(&RS485RecMsgQeueTab[0],RS485_REC_QEUE_LENGTH); 

 /* 创建InfmBoard_ErrQeue消息队列 */
  InfmBoard_ErrQeue = OSQCreate(&UdpHearbeatQeueTab[0],UDP_Heart_QEUE_LENGTH);
	
 /* 创建InfmBoard_ErrQeue消息队列 */
  InfmBoardLed_ErrQeue = OSQCreate(&UdpHearbeatQeueTab[0],UDP_Heart_QEUE_LENGTH);
	
	/*创建加密解密消息队列*/
#if EnableCode
	En_Dis_CodeQeue = OSQCreate(&En_Dis_CodeQeueTab[0],En_Dis_Code_QEUE_LENGTH);//用于上位机->下位机
	Dis_En_CodeQeue = OSQCreate(&Dis_En_CodeQeueTab[0],Dis_En_Code_QEUE_LENGTH);//用于下位机->上位机
#endif
  /* 创建RS485Sem资源信号量 */
  RS485Sem = OSSemCreate(1); 
//  //===============资源信号量=============//
  UsartTranSem = OSSemCreate(1);     //创建UART1发送资源信号量
  SDSem = OSSemCreate(1);
	HearbeatSem = OSSemCreate(1);		//创建心跳包信号量
	while(lwip_comm_init()); 	//lwip初始化
	//=============初始化进程=====================//
	if(TCPorUDPFlag)
	{
		while(tcp_server_init());//初始化TCP线程	
	}
	else
	{
		while(udp_demo_init());//初始化UDP线程
	}
	
	httpd_init();  //初始化HTTP

  //数据处理
  OSTaskCreate(Task_CtrlCenterHandle,(void *)0,
	          &task_cch_stk[TASK_CCH_STK_SIZE-1], TASK_CCH_PRIO);

	//数据解析
  OSTaskCreate(Task_ReadPlayList,(void *)0,
	               &task_rpl_stk[TASK_RPL_STK_SIZE-1], TASK_RPL_PRIO);

  //控制显示
  OSTaskCreate(Task_DisplayCtrl,(void *)0,		  	
              &task_dis_ctrl_stk[TASK_DIS_CTRL_SIZE-1], TASK_DIS_CTRL_PRIO);
	
  //LED系统灯
  OSTaskCreate(Task_LED1,(void *)0,
	          &task_led1_stk[TASK_LED1_STK_SIZE-1], TASK_LED1_PRIO);
	
  //ETC按键控制播放预制播放表
  OSTaskCreate(Task_KEY,(void *)0,
	          &key_Code_stk[KEY_CODE_SIZE-1], KEY_LED1_PRIO);
#if EnableCode
  	//任务  加密解密
	OSTaskCreate(TestCode,(void *)0,
						&task_Code_stk[TASK_CODE_SIZE-1],9);
#endif
	OSStart(); //开启UCOS
 
	return 0;
}
