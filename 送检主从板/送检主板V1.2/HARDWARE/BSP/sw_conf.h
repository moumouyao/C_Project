/**
  ******************************************************************************
  * �ļ��� : sw_conf.h
  * ��  �� : �¹�
  * ��  �� ��V1.0
  * ʱ  �� : 2014��5��14��
  * ��  ˾ ��������缼�����޹�˾
  * ժ  Ҫ �����ļ�������ϵͳ����ģ��,ȫ�ֱ���
  ******************************************************************************
  */

#ifndef __SW_CONF_H__
#define __SW_CONF_H__

#ifdef __cplusplus
 extern "C" {
#endif


	 
/**
  ******************************************************************************
  *                           ���������������
  ******************************************************************************
  */

#define SCREEN_YANSE	       G_ScreenSet.YANSE	      //����LED��ɫ����
#define SCREEN_HANG     		 G_ScreenSet.HANG	    		//����һ���ж��ٸ���    	 
#define SCREEN_LIE					 G_ScreenSet.LIE		    	//����һ���ж��ٸ���
	 
#define SCREEN_HBYTE    		 G_ScreenSet.HBYTE	          //����һ���ж����ֽ�
#define SCREEN_FANHUI				 G_ScreenSet.FANHUI	          //����һ���ж����ֽ�
#define SCREEN_BYTE_NUM			 G_ScreenSet.BYTENUM	        //����һ���ж����ֽ�


#define BOX_YANSE    	       G_BoxSet.YANSE	      //����LED��ɫ����

#define BOX_HANG					   G_BoxSet.HANG		  //����һ���ж��ٸ���
#define BOX_LIE						   G_BoxSet.LIE		    //����һ���ж��ٸ���

#define BOX_HBYTE					   G_BoxSet.HBYTE		    //����һ���ж����ֽ�
#define BOX_FANHUI				   G_BoxSet.FANHUI		    //����һ���ж����ֽ�	 

#define SLAVE_NUM            G_SlaveNum        //�ӻ�����

	 

#define  DIS_BUF_CLR          3       //RGB���ݻ�������ɫ����
#define  DIS_BUF_LEN          4096   //RGB���ݻ���������--����ʵ��Ӧ�ö��� 32*36������ 


#ifdef __cplusplus 
}
#endif


#endif

