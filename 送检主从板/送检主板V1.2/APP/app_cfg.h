#ifndef  __APP_CFG_H__
#define  __APP_CFG_H__


/**
  ******************************************************************************
  *                              任务优先级安排
  *高优先级:(4-15)
  *         LWIP_TCPIP_THREAD   LWIP_ethernetif_input  
  *         
  ******************************************************************************
  *中优先级:(16-25)	
  *         
  ******************************************************************************
  *低优先级:(26-35)
  *         TASK_LED1           TASK_LED2	
  *  
  ******************************************************************************
  */


/*******************设置任务优先级*******************/

#define TASK_CCH_PRIO               15    //接收控制中心数据处理
#define TASK_RPL_PRIO               18    //数据解析并发送给从机
#define TASK_DIS_CTRL_PRIO          25    //显示控制任务
#define TASK_LED1_PRIO		          8
#define KEY_LED1_PRIO		          	23


/************设置栈大小（单位为 OS_STK ）************/
#define TASK_LED1_STK_SIZE		     	128
#define LWIP_STK_SIZE		           	128
#define TASK_CCH_STK_SIZE          	4000
#define TASK_RPL_STK_SIZE          	4800
#define TASK_DIS_CTRL_SIZE         	3100
#define TASK_CODE_SIZE             	512
#define KEY_CODE_SIZE             	512
#endif

