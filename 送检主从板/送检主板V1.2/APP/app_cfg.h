#ifndef  __APP_CFG_H__
#define  __APP_CFG_H__


/**
  ******************************************************************************
  *                              �������ȼ�����
  *�����ȼ�:(4-15)
  *         LWIP_TCPIP_THREAD   LWIP_ethernetif_input  
  *         
  ******************************************************************************
  *�����ȼ�:(16-25)	
  *         
  ******************************************************************************
  *�����ȼ�:(26-35)
  *         TASK_LED1           TASK_LED2	
  *  
  ******************************************************************************
  */


/*******************�����������ȼ�*******************/

#define TASK_CCH_PRIO               15    //���տ����������ݴ���
#define TASK_RPL_PRIO               18    //���ݽ��������͸��ӻ�
#define TASK_DIS_CTRL_PRIO          25    //��ʾ��������
#define TASK_LED1_PRIO		          8
#define KEY_LED1_PRIO		          	23


/************����ջ��С����λΪ OS_STK ��************/
#define TASK_LED1_STK_SIZE		     	128
#define LWIP_STK_SIZE		           	128
#define TASK_CCH_STK_SIZE          	4000
#define TASK_RPL_STK_SIZE          	4800
#define TASK_DIS_CTRL_SIZE         	3100
#define TASK_CODE_SIZE             	512
#define KEY_CODE_SIZE             	512
#endif

