/**
  ******************************************************************************
  * 文件名 : sw_conf.h
  * 作  者 : 陈光
  * 版  本 ：V1.0
  * 时  间 : 2014年5月14日
  * 公  司 ：汉威光电技术有限公司
  * 摘  要 ：本文件包含了系统配置模块,全局变量
  ******************************************************************************
  */

#ifndef __SW_CONF_H__
#define __SW_CONF_H__

#ifdef __cplusplus
 extern "C" {
#endif


	 
/**
  ******************************************************************************
  *                           配置箱体屏体参数
  ******************************************************************************
  */

#define SCREEN_YANSE	       G_ScreenSet.YANSE	      //屏体LED颜色种类
#define SCREEN_HANG     		 G_ScreenSet.HANG	    		//屏体一行有多少个点    	 
#define SCREEN_LIE					 G_ScreenSet.LIE		    	//屏体一列有多少个点
	 
#define SCREEN_HBYTE    		 G_ScreenSet.HBYTE	          //屏体一行有多少字节
#define SCREEN_FANHUI				 G_ScreenSet.FANHUI	          //屏体一列有多少字节
#define SCREEN_BYTE_NUM			 G_ScreenSet.BYTENUM	        //屏体一共有多少字节


#define BOX_YANSE    	       G_BoxSet.YANSE	      //屏体LED颜色种类

#define BOX_HANG					   G_BoxSet.HANG		  //箱体一行有多少个点
#define BOX_LIE						   G_BoxSet.LIE		    //箱体一列有多少个点

#define BOX_HBYTE					   G_BoxSet.HBYTE		    //箱体一行有多少字节
#define BOX_FANHUI				   G_BoxSet.FANHUI		    //箱体一列有多少字节	 

#define SLAVE_NUM            G_SlaveNum        //从机个数

	 

#define  DIS_BUF_CLR          3       //RGB数据缓冲区颜色种类
#define  DIS_BUF_LEN          4096   //RGB数据缓冲区长度--根据实际应用而定 32*36个箱体 


#ifdef __cplusplus 
}
#endif


#endif

