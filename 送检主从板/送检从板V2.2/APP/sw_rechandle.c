#include "sw_rechandle.h"
#include "sw_config.h"
#include "user_crc.h"
#include "i2c.h"
#include "delay.h"
#include "flash.h"
#include "malloc.h"
#include "sw_led.h"
#include "i2c2.h"


static  uint8_t array[3][256];                        //按最多256列

/**															   
  ******************************************************************************
  *                              SlaveAck()
  *
  * 描述   : 从机对主机的应答
  *		   
  * 参数   : *pot-应答数据缓冲区
	*          temp-0或1
  *
  *
  * 返回值 : 无
  ******************************************************************************
  */
void SlaveAckInit (uint8_t *pot,uint8_t temp)
{
  uint32_t crc32;
	uint8_t *p;
	uint16_t temp_sh20 = 0;
	p = pot;//数组起始地址
	pot += 2;            //自留2位存总数据长度
	
	*pot++ = G_IDHost;   //主机地址
	*pot++ = G_IDSlave0; //从机地址0
	*pot++ = 0;          //从机地址1备用
	
	*pot++ = temp;       //应答结果
	*pot++ = 0;          //数据长度高字节	
	*pot++ = 0x9;          //数据长度低字节
	
	//温度
//	temp_sh20 = SHT20_Get_Data(SHT20_CMD_MEASURE_TEMP);
//*pot++ = (uint8_t)(temp_sh20 >> 8);   //温度高八位   
	*pot++ = (uint8_t)temp_sh20; 				//温度低八位
//	temp_sh20 = 0;
//	//湿度
//	temp_sh20 = SHT20_Get_Data(SHT20_CMD_MEASURE_HUMI);
//	*pot++ = (uint8_t)(temp_sh20 >> 8);     //湿度高八位   
	*pot++ = (uint8_t)temp_sh20; 						//湿度低八位							
//	
//	//读取箱门电平 U4 U7 U15没接引脚
	Door1 = GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_1);//U3	控制器故障
	Door2 = GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0);//U5	显示模组故障
	Door3 = GPIO_ReadInputDataBit(GPIOH,GPIO_Pin_2);//U6	显示模组电源故障
	Door4 = GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_6);//U8	单像素管故障
	Door5 = GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_4);//U13	 检测系统故障
	Door6 = GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_0);//U14		防雷器故障
	Door7 = GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_2);//U9 U10220V交流电
	*pot++ = Door1;          //箱门应答结果
	*pot++ = Door2;          
	*pot++ = Door3;          
	*pot++ = Door4; 
	*pot++ = Door5;          
	*pot++ = Door6; 
	*pot++ = Door7;	//220V交流电故障检测，断开为0，正常为1
	
//	
	crc32 = UserHWCRC32(p+2,(pot-p-2));
	*((uint32_t *)pot) = crc32;
//	printf("faultcrc:%d, %d, %d, %d\r\n", *pot, *(pot+1), *(pot+2), *(pot+3));
	pot += 4;
	*((uint16_t *)p) = 0x0013; 
//	printf("故障检测数据长度：%d\r\n", *((uint16_t *)p));
//	*((uint16_t *)p) = (uint16_t)(pot-p-2);//这行代码会进硬件错误
}


/**
  ******************************************************************************
  *                          	SlaveAckOK ()
  *
  * 描述   : 返回坏点数据
  *		   
  * 参数   : 无
  *
  *
  * 返回值 : 无
  ******************************************************************************
  */
uint32_t crc32 = 0;
void Open_DetectionAck(uint8_t num)
{	
	xSemaphoreTake(RS485TranSem,portMAX_DELAY);//获取信号量		
	SLAVE485Out;
	delay_sys_us(200);
//	vTaskDelay(1);
	
	uint8_t *p = NULL;
	uint8_t ackdata[3*32*4+16] = {0};
	uint16_t q = 1;
	p = ackdata;//数组起始地址
	p += 2;            //自留2位存总数据长度
	
	*p++ = G_IDHost;   //主机地址
	*p++ = G_IDSlave0; //从机地址0
	*p++ = 0;          //从机地址1备用
	
	*p++ = 7;       //应答结果
//	*p++ = 0;
//	*p++ = 6;
//	*p++ = 7;
//	*p++ = 8;
if(num == 1){
	*p++ = (2*32 * 4)>>8;	//数据长度
	*p++ = (2*32 * 4)&0xff;	//数据长度
//	

	for(uint8_t i = 0; i < 2; i++){
		for(uint8_t j = 0; j < 32; j++){
			for(uint8_t k = 0; k < 4; k++){
				*p++ = open_data[i][j][k];
//				*p++ = q++;
////				*p++ = 6;
			}
//			
		}
	}
//printf("11\r\n");
}
else{
	*p++ = (32 * 4)>>8;	//数据长度
	*p++ = (32 * 4)&0xff;	//数据长度
//	
		for(uint8_t j = 0; j < 32; j++){
			for(uint8_t k = 0; k < 4; k++){
				*p++ = open_data[2][j][k];
//				*p++ = q++;
//				*p++ = 7;
			}
//			
		}
	
//printf("22\r\n");
}

//	printf("数据长度%d", *(uint16_t *)p);
	
//	
	crc32 = UserHWCRC32(ackdata+2,(p - ackdata-2));
//	printf("校验位%d\r\n", crc32);
	*((uint32_t *)p) = crc32;
//	printf("ledcrc:%d, %d, %d, %d\r\n", *p, *(p+1), *(p+2), *(p+3));
//	printf("ledcrc2:%x\r\n", crc32);//0xD5628D88
	p += 4;
//	*((uint16_t *)p) = 389; 
	*((uint16_t *)ackdata) = (uint16_t)(p - ackdata-2);
//	*((uint16_t *)ackdata) = 394;
//	printf("数据长度 %d\r\n", (uint16_t)(p - ackdata-2));
	

	
	 
	RS485TranConfig(&ackdata[2],*((uint16_t *)ackdata));	//配置UART3准备发送数据
	USART_ITConfig(USART3, USART_IT_TXE, ENABLE);



	
	xSemaphoreGive(RS485TranSem);	//释放信号量
//	delay_sys_us(800);
//	vTaskDelay(1);
//	SLAVE485In;
}




/**
  ******************************************************************************
  *                          RecDataHandle ()
  *
  * 描述   : 处理接收到的数据函数
  *		   
  * 参数   : p:接收到的数据指针
  *
  *
  * 返回值 : 无
  ******************************************************************************
  */
void RecDataHandle (uint8_t * p)//接收数据处理
{
	uint8_t test=0;
	uint8_t cmd = p[4];
	BaseType_t xReturn = pdTRUE;
#if Debug_printf
					printf("cmd=%d", cmd);//%d,%d,%d,%d,%d\r\n",p[3],cmd, p[5], p[6], p[7], p[8]);	
#endif
	switch(cmd)
		{
			case 1://完整播放帧表RGB数据
			SlaveAckOK();			//--应答
			test=0;
			xReturn=xQueueSend(FrameStoreMsgQeue,(void *)&p,0);//发送到队列
			if(xReturn!=pdTRUE)
			{
				vTaskDelay(4);
#if Debug_printf
				printf("\n\r 发送消息队列失败：FrameStoreMsgQeue");			
#endif
				if(test++ == 5)
				NVIC_SystemReset();		
				xReturn=xQueueSend(FrameStoreMsgQeue,(void *)&p,0);//发送到队列				
			}	
			break;
		case 2://播放帧号、步数	
			SlaveAckOK();			//--应答
			xReturn=xQueueSend(FrameLoadMbox,(void *)&p,0);//发送到队列
			test = 0;
			if(xReturn!=pdTRUE)
			{
				vTaskDelay(4);
#if Debug_printf
				printf("\n\r 发送消息队列失败：FrameStoreMsgQeue");			
#endif
				if(test++ == 5)
				NVIC_SystemReset();		
				xReturn=xQueueSend(FrameLoadMbox,(void *)&p,0);//发送到队列				
			}		
			break;
			case 3://屏体箱体模组类别参数
				if(*(uint16_t *)(p+2+3) == sizeof(ScreenBoxDataStr))//数据长度为10
				{
					SlaveAckOK(); //应答
					SBoxSetStore(p+7);//传送数据指针
					if(G_FrameFlag == 1)//如果有完整数据
					{		
						SPI_FlashRead(&G_FrameNum,FLASH_FRAME_TAB_ADDR,1);//读取总的帧数
						GetLoadNodeTab(LoadNodeTab,G_FrameNum);           //所有帧加载头表
					}
				SetShowArea();      //设置从机控制显示范围	
				}
			myfree(SRAMIN,(void *)p);	
			break;
		case 5://检查加载数据是否完成
			if(G_LoadDataFlag == 1)//如果数据加载完成则应答
				SlaveAckOK();			//--应答
				myfree(SRAMIN,(void *)p);	
			break;
			
		case 6://返回故障信息
			SlaveAckFault();
			Open_Detection();
			
		break;
		
		case 7://坏点信息
			if(p[7] == 1)
			Open_DetectionAck(1);
			else
			Open_DetectionAck(2);
		break;
			default:
				myfree(SRAMIN,(void *)p);	
			break;
		}
}
/**
  ******************************************************************************
  *                          	SlaveAckOK ()
  *
  * 描述   : 对主机的命令进行应答
  *		   
  * 参数   : 无
  *
  *
  * 返回值 : 无
  ******************************************************************************
  */
void SlaveAckOK (void)
{	
	uint16_t num;
	xSemaphoreTake(RS485TranSem,portMAX_DELAY);//获取信号量		
	SLAVE485Out;
	delay_sys_us(200);
//	vTaskDelay(1);
	num = *(uint16_t *)SlaveAckOKBuf;        //要发送的数据个数
	 
	RS485TranConfig(&SlaveAckOKBuf[2],num);	//配置UART3准备发送数据
	USART_ITConfig(USART3, USART_IT_TXE, ENABLE); 
	
	xSemaphoreGive(RS485TranSem);	//释放信号量
//	delay_sys_us(800);
//	vTaskDelay(1);
//	SLAVE485In;
}



/**
  ******************************************************************************
  *                          	SlaveAckOK ()
  *
  * 描述   : 对主机的命令进行应答
  *		   
  * 参数   : 无
  *
  *
  * 返回值 : 无
  ******************************************************************************
  */
void SlaveAckFault (void)
{	
	uint16_t num;
	xSemaphoreTake(RS485TranSem,portMAX_DELAY);//获取信号量		
	SLAVE485Out;
	delay_sys_us(200);
//	vTaskDelay(1);
	num = *(uint16_t *)SlaveAckOKBuf;        //要发送的数据个数
	 
	RS485TranConfig(&SlaveAckFaultBuf[2],num);	//配置UART3准备发送数据
	USART_ITConfig(USART3, USART_IT_TXE, ENABLE); 
	
	xSemaphoreGive(RS485TranSem);	//释放信号量
//	delay_sys_us(800);
//	vTaskDelay(1);
//	SLAVE485In;
}







/**															   
  ******************************************************************************
  *                              ScreenBoxSet()
  *
  * 描述   : 从机屏体、箱体、模组类别参数存储
  *		   
  * 参数   : 屏体、箱体、模组类别参数指针
  *
  *
  * 返回值 : 无
  ******************************************************************************
  */

void SBoxSetStore (uint8_t *p)
{

	
#if Debug_printf
		ScreenBoxDataStr *pot;
		pot = (ScreenBoxDataStr *)p;

		printf("颜色种类：%d\n\r",pot->Yanse);
		printf("屏体一行点阵数量：%d\n\r",pot->SHang);
		printf("屏体一列点阵的像素数：%d\n\r",pot->SLie);
		printf("箱体一行点阵的像素数：%d\n\r",pot->BHang);
		printf("箱体一列点阵的像素数：%d\n\r",pot->BLie);
		printf("模组类别-1:8X4;2:8X8;3:8X16：%d\n\r",pot->MozuType);
#endif
	
  uint8_t buf[15];
	uint8_t i=0;
	I2C_ReadS_24C(EEROM_SBOX_SET_FLAG_ADDR,&ScreenBoxSetFlag,1);
	if(ScreenBoxSetFlag == 1)
	{
	  I2C_ReadS_24C(EEROM_SBOX_SET_START_ADDR,buf,sizeof(ScreenBoxDataStr));//读取EEROM中参数设置
    for(i=0;i<sizeof(ScreenBoxDataStr);i++)
	  {
      if(buf[i] != p[i])
			  break;
    }
  }
	if(i != sizeof(ScreenBoxDataStr))  //数据不相同或是未设置
	{
    ScreenBoxSetFlag = 0;
		I2C_ByteWrite_24C(EEROM_SBOX_SET_FLAG_ADDR,ScreenBoxSetFlag);//存储清零标志位
#if Debug_printf
		printf("\n\r 存储SBOX_SET_FLAG_CLR \r\n ");
#endif
		I2C_WriteS_24C(EEROM_SBOX_SET_START_ADDR,p,sizeof(ScreenBoxDataStr));//存储参数
		ScreenBoxSetRefresh(p);//更新参数
		ScreenBoxSetFlag = 1;
#if Debug_printf
		printf("\n\r 存储SBOX_SET_FLAG_SET \r\n ");
#endif
		I2C_ByteWrite_24C(EEROM_SBOX_SET_FLAG_ADDR,ScreenBoxSetFlag);//存储置1标志位
  }	
}
/**															   
  ******************************************************************************
  *                              ScreenBoxSet()
  *
  * 描述   : 从机屏体、箱体参数设置
  *		   
  * 参数   : 主机发来的屏体/箱体帧参数指针
  *
  *
  * 返回值 : 无
  ******************************************************************************
  */
void ScreenBoxSetRefresh (uint8_t *p)
{
  ScreenBoxDataStr *pot;
	pot = (ScreenBoxDataStr *)p;
	G_ScreenSet.YANSE = pot->Yanse;
	G_ScreenSet.HANG = pot->SHang;
	G_ScreenSet.HBYTE = G_ScreenSet.HANG>>3;
	G_ScreenSet.LIE = pot->SLie;
	G_ScreenSet.FANHUI = G_ScreenSet.LIE>>3;
	G_ScreenSet.BYTENUM = G_ScreenSet.HBYTE * G_ScreenSet.LIE;

	G_BoxSet.YANSE = pot->Yanse;
	G_BoxSet.HANG = pot->BHang;
	G_BoxSet.HBYTE = G_BoxSet.HANG>>3;
	G_BoxSet.LIE = pot->BLie;
	G_BoxSet.FANHUI = G_BoxSet.LIE>>3;
	G_BoxSet.BYTENUM = G_BoxSet.HBYTE * G_ScreenSet.LIE;
	
  G_MoZuType = pot->MozuType;   //模组类别	
}

/**															   
  ******************************************************************************
  *                              FrameStoreHandle()
  *
  * 描述   : 数据存储处理
  *		   
  * 参数   : p:主机发来的参数指针
  *
  *
  * 返回值 : 无
  ******************************************************************************
  */
void FrameStoreHandle (uint8_t *p) 
{
#if Debug_printf
		printf("\n\r FLASH存储数据");
#endif
	uint16_t data_length;             //数据长度
	FrameFileHead head;  
	static FrameFileHead last_head;
	static uint32_t flash_addr;       //FLASH存储地址记录
	static uint16_t flash_sector;	    //FLASH存储扇区记录
	
	ReadFrameFileHead((p+7),&head);   //读取文件头
	data_length = *(uint16_t *)(p+5)-4; //读取数据长度-不包括文件头
	if(head.frame_no == 0 && head.data_no == 0) //第0帧数据,第0部分
	{
		G_FrameFlag = 0;              //存储完整数据标志位-清零		
		I2C_ByteWrite_24C(EEROM_FRAME_FLAG_ADDR,0);//存储置1标志位//FLASH中存有完整帧数据标志位地址
		flash_addr = FLASH_FRAME_FILE_START_ADDR;     //初始化存储地址//存放全部帧文件的起始地址	
		flash_sector = FLASH_FRAME_FILE_START_SECTOR; //初始化扇区
		SPI_FlashEraseSector(flash_sector++);         //擦除扇区
		SPI_FlashEraseSector(0);                      //擦除扇区0
		SPI_FlashWriteNoCheck(&head.frame_no,FLASH_FRAME_TAB_ADDR+5*head.frame_no+1,1);//存储帧号 总共的帧表起始地址，记录着帧总数、各个帧号和所在地址	
		SPI_FlashWriteNoCheck((uint8_t *)&flash_addr,FLASH_FRAME_TAB_ADDR+5*head.frame_no+2,4);//存储地址
  }
	else
	{
		if(head.frame_no == last_head.frame_no) 
		{
			if(head.data_no == last_head.data_no) //发送重复
			{
#if Debug_printf
			  printf("\n\r 数据发送重复，不进行存储");
#endif
				return;        
      }
			else if(head.data_no%2 == 0) //本扇区已存满
			{
        flash_addr = flash_sector*4096;      //存放全部帧文件的起始地址	
				SPI_FlashEraseSector(flash_sector++);//擦除扇区   先擦除后加加
      }		
    }	
		else  //新帧数据
		{
			flash_addr = flash_sector*4096;      //
      SPI_FlashEraseSector(flash_sector++);//擦除扇区
			SPI_FlashWriteNoCheck(&head.frame_no,FLASH_FRAME_TAB_ADDR+5*head.frame_no+1,1);//存储帧号
		  SPI_FlashWriteNoCheck((uint8_t *)&flash_addr,FLASH_FRAME_TAB_ADDR+5*head.frame_no+2,4);//存储地址
    }
  }
	SPI_FlashWriteNoCheck(p+11,flash_addr,data_length);//开始存储数据
	flash_addr += data_length;
	
	last_head.data_no = head.data_no; 
	last_head.data_num = head.data_num;
	last_head.frame_no = head.frame_no; 
	last_head.frame_num = head.frame_num;
	//最后一部分数据
	if(head.frame_no == (head.frame_num-1) && head.data_no == (head.data_num-1)) 
	{
    SPI_FlashWriteNoCheck(&head.frame_num,FLASH_FRAME_TAB_ADDR,1);//存储总帧数
		G_FrameNum = head.frame_num;
		G_NewFramesFlag = 1;          //更新帧数据标志位
		G_FrameFlag = 1;              //存储完整数据标志位-置1
		I2C_ByteWrite_24C(EEROM_FRAME_FLAG_ADDR,1);   //EEROM中标志位置1-表示数据存储完整  
		if(ScreenBoxSetFlag == 1)//箱体屏体参数设置
		  GetLoadNodeTab(LoadNodeTab,G_FrameNum);       //所有帧加载头表
  }	
}

/**															   
  ******************************************************************************
  *                              FrameStoreHandle()
  *
  * 描述   : 读取帧的文件头
  *		   
  * 参数   : p:文件头地址
  *          head:要存储的文件头结构体
  *
  *
  * 返回值 : 无
  ******************************************************************************
  */
void ReadFrameFileHead(uint8_t *p,FrameFileHead *head)
{
  head->frame_num = *p++;
	head->frame_no = *p++;
	head->data_num = *p++;
	head->data_no = *p++;
}

/**															   
  ******************************************************************************
  *                              GetLoadNodeTab()
  *
  * 描述   : 建立加载要显示的所有帧头表：帧号+FLASH地址+步数
  *		   
  * 参数   : tab:帧头表指针
  *          num:总的帧数
  *
  *
  * 返回值 : 无
  ******************************************************************************
  */
void GetLoadNodeTab(LoadDataNode *tab,uint8_t num)           //所有帧加载头表
{
  uint8_t i;
	uint16_t step = 0;
	for(i=0;i<num;i++)
	{
    tab[i].frame_no = i;//帧号
    SPI_FlashRead((uint8_t *)&tab[i].addr,FLASH_FRAME_TAB_ADDR+5*i+2,4);//读取地址	
    SPI_FlashRead(&tab[i].mod,tab[i].addr+2,1);//显示方式
    switch(tab[i].mod/10)  //移入
	  {
      case 1://立即显示
			  step = 0;
		    tab[i].in_step = 0;	
			  break;
		  case 2://左移
			  step = SCREEN_HANG-1;
		    tab[i].in_step = SCREEN_HANG-1;
			  break;
		  case 3://右移
			  step = SCREEN_HANG-1;
		    tab[i].in_step = SCREEN_HANG-1;
			  break;
		  case 4://上移
			  step = SCREEN_LIE-1;
		    tab[i].in_step = SCREEN_LIE-1;
			  break;
		  case 5://下移
			  step = SCREEN_LIE-1;
		    tab[i].in_step = SCREEN_LIE-1;
			  break;
		  case 6://横百叶窗 
			  step = 7;
		    tab[i].in_step = 7;
			  break;
		  case 7://竖百叶窗
			  step = 7;
		    tab[i].in_step = 7;
			  break;
		  case 8://闪烁
			  step = 0;
		    tab[i].in_step = 0;
			  break;
		  default:
				step = 0;
			  tab[i].in_step = 0;
			  break;
    }
	  switch(tab[i].mod%10)  //移出
	  {
      case 1://立即显示
			  step += 1; 
		    tab[i].out_step = 0;
			  break;
		  case 2://左移
			  step += SCREEN_HANG;
		    tab[i].out_step = SCREEN_HANG-1;
			  break;
		  case 3://右移
			  step += SCREEN_HANG;
		    tab[i].out_step = SCREEN_HANG-1;
			  break;
		  case 4://上移
			  step += SCREEN_LIE;
		    tab[i].out_step = SCREEN_LIE-1;
			  break;
		  case 5://下移
			  step += SCREEN_LIE;
		    tab[i].out_step = SCREEN_LIE-1;
			  break;
		  case 6://横百叶窗 
			  step += 8;
		    tab[i].out_step = 7;
			  break;
		  case 7://竖百叶窗
			  step += 8;
		    tab[i].out_step = 7;
			  break;
		  case 8://闪烁
			  step += 1;
		    tab[i].out_step = 0;
			  break;
		  default:
				step += 1;
			  tab[i].out_step = 0;
			  break;
    }
    tab[i].step = step;		
  }
}

void GetSetEEPROM (void)
{
  uint8_t buf[15];
	/* 读取已存储完整数据标志位  */
//  I2C_Standby_24C();
  I2C_ReadS_24C(EEROM_FRAME_FLAG_ADDR,&G_FrameFlag,1);
  if(G_FrameFlag == 1)	 //如果设置了，则读取校验数据
  {
    I2C_ReadS_24C(EEROM_FRAME_CRC32_ADDR,(uint8_t *)&G_FrameCRC32,4);
#if Debug_printf
    printf("\n\r 已完整存储数据 \r\n ");
		printf("校验结果：0x%8x",G_FrameCRC32);
#endif 
	}
	else
	{
#if Debug_printf
    printf("\n\r 数据存储不完整 \r\n ");
#endif
    G_FrameCRC32 = 0;
  }
  I2C_ReadS_24C(EEROM_SBOX_SET_FLAG_ADDR,&ScreenBoxSetFlag,1);
  if(ScreenBoxSetFlag == 1)
	{
#if Debug_printf
    printf("\n\r 已完整存储屏体箱体参数 \r\n ");
#endif
		I2C_ReadS_24C(EEROM_SBOX_SET_START_ADDR,buf,sizeof(ScreenBoxDataStr));//读取参数
		ScreenBoxSetRefresh(buf);//更新参数
		SetShowArea();           //设置从机控制显示范围
  }		
}


/**															   
  ******************************************************************************
  *                              LoadDataHandle()
  *
  * 描述   : 把要显示的数据加载到缓冲区中
  *		   
  * 参数   : p:要写入缓冲区的指针
  *          p_node:要加载的结点指针
  *
  *
  * 返回值 : 无
  ******************************************************************************
  */
void LoadDataHandle (uint8_t p[][DIS_BUF_LEN],LoadDataNode *p_node)
{
  uint8_t out_flg = 0;
	uint16_t step = 0;
	uint16_t temp;
static uint8_t flg = 0; 
	
	switch(LoadNodeTab[p_node->frame_no].mod/10)
	{
     case 1://立即显示
			step = 1;		 
      if(p_node->step < step)
        LoadDotaNowIn(p,LoadNodeTab[p_node->frame_no].addr,p_node->step);
			else
				out_flg = 1;
		  if(G_FrameFlag == 0)//有新RGB数据发送过来
        return;			
			break;
		case 2://左移
			step = 2*SCREEN_HANG - (ShowAreaStru.HByteEnd<<3);//要显示内容全部进入本箱体需要步数
		  temp = SCREEN_HANG - (ShowAreaStru.HByteEnd<<3);  //左移到达本箱体需要步数
		  if(flg == 1)//前后俩帧连续显示
			{
        p_node->step += SCREEN_HANG;
				p_node->frame_no -= 1;
      }
		  if(p_node->step < temp)  //没有到达本箱体
        ClrDisplayBuf();       //清零缓冲区
      else if(p_node->step < step)//显示内容正在进入
			{
				if((p_node->step == SCREEN_HANG-1) && (p_node->frame_no < G_FrameNum-1))//不是最后一帧数据
				{
				  if(LoadNodeTab[p_node->frame_no+1].mod == LoadNodeTab[p_node->frame_no].mod)//需要连续显示
						flg = 1;//连续显示标志位
					else
						flg = 0;
				}
				LoadDotaLeftIn(p,LoadNodeTab[p_node->frame_no].addr,p_node->step-temp);
				if(p_node->step == step -1)//进入箱体完毕
					flg = 0;
			}
      else
        out_flg = 1;
		  if(G_FrameFlag == 0){//有新RGB数据发送过来
				 ClrDisplayBuf();
        return;
	 	}			
			break;
		case 3://右移
			step = SCREEN_HANG + (ShowAreaStru.HByteStart<<3);//全部数据达到本箱体所需步数
		  temp = ShowAreaStru.HByteStart<<3;//右移到达本箱体需要步数
		  if(flg == 1)//前后俩帧连续显示
			{
        p_node->step += SCREEN_HANG;
				p_node->frame_no -= 1;
      }		  
		  if(p_node->step < temp)  //没有到达本箱体
				ClrDisplayBuf();       //清零缓冲区
      else if(p_node->step < step)//显示内容正在进入
			{
        if((p_node->step == SCREEN_HANG-1) && (p_node->frame_no < G_FrameNum-1))//不是最后一帧数据
				{
				  if(LoadNodeTab[p_node->frame_no+1].mod == LoadNodeTab[p_node->frame_no].mod)//需要连续显示
						flg = 1;//连续显示标志位
					else
						flg = 0;
				}
				LoadDotaRightIn(p,LoadNodeTab[p_node->frame_no].addr,p_node->step-temp);
				if(p_node->step == step -1)//进入箱体完毕
					flg = 0;
			}
      else
        out_flg = 1;
		  if(G_FrameFlag == 0)//有新RGB数据发送过来
        return;	
			break;
		case 4://上移
			step = SCREEN_LIE*2 - ShowAreaStru.LieEnd;//全部数据达到本箱体所需步数
		  temp = SCREEN_LIE - ShowAreaStru.LieEnd;             //上移到达本箱体需要步数
		  if(flg == 1)//前后俩帧连续显示
			{
        p_node->step += SCREEN_LIE;
				p_node->frame_no -= 1;
      }
		  if(p_node->step < temp)  //没有到达本箱体
				ClrDisplayBuf();       //清零缓冲区
      else if(p_node->step < step)//显示内容正在进入
			{
        if((p_node->step == SCREEN_LIE-1) && (p_node->frame_no < G_FrameNum-1))//不是最后一帧数据
				{
				  if(LoadNodeTab[p_node->frame_no+1].mod == LoadNodeTab[p_node->frame_no].mod)//需要连续显示
						flg = 1;//连续显示标志位
					else
						flg = 0;
				}
				LoadDotaUpIn(p,LoadNodeTab[p_node->frame_no].addr,p_node->step-temp);
				if(p_node->step == step -1)//进入箱体完毕
					flg = 0;
			}
      else
        out_flg = 1;
		  if(G_FrameFlag == 0)//有新RGB数据发送过来
        return;	
			break;
		case 5://下移
			step = SCREEN_LIE + ShowAreaStru.LieStart;//全部数据达到本箱体所需步数
		  temp = ShowAreaStru.LieStart;             //下移到达本箱体需要步数
		  if(flg == 1)//前后俩帧连续显示
			{
        p_node->step += SCREEN_LIE;
				p_node->frame_no -= 1;
      }
		  if(p_node->step < temp)  //没有到达本箱体
				ClrDisplayBuf();       //清零缓冲区			
      else if(p_node->step < step)//显示内容正在进入
			{
        if((p_node->step == SCREEN_LIE-1) && (p_node->frame_no < G_FrameNum-1))//不是最后一帧数据
				{
				  if(LoadNodeTab[p_node->frame_no+1].mod == LoadNodeTab[p_node->frame_no].mod)//需要连续显示
						flg = 1;//连续显示标志位
					else
						flg = 0;
				}
			 LoadDotaDownIn(p,LoadNodeTab[p_node->frame_no].addr,p_node->step-temp);
			 if(p_node->step == step -1)//进入箱体完毕
					flg = 0;
			}
      else
        out_flg = 1;
		  if(G_FrameFlag == 0)//有新RGB数据发送过来
        return;	
			break;
		case 6://横百叶窗 
			step = 8;
      if(p_node->step < step)
        LoadDotaHengIn(p,LoadNodeTab[p_node->frame_no].addr,p_node->step);	
      else
        out_flg = 1;
		  if(G_FrameFlag == 0)//有新RGB数据发送过来
        return;				
			break;
		case 7://竖百叶窗
			step = 8;
      if(p_node->step < step)
        LoadDotaShuIn(p,LoadNodeTab[p_node->frame_no].addr,p_node->step);
      else
        out_flg = 1;
		  if(G_FrameFlag == 0)//有新RGB数据发送过来
        return;	
			break;
		case 8://闪烁
			step = 1;
      if(p_node->step < step)
        LoadDotaNowIn(p,LoadNodeTab[p_node->frame_no].addr,p_node->step);
			else
				out_flg = 1;
		  if(G_FrameFlag == 0)  //有新RGB数据发送过来
        return;	
			break;
		default:break;
  }
	if(out_flg == 1)
	{
		switch(LoadNodeTab[p_node->frame_no].mod%10)  //移出
		{
			case 1://立即显示 
				ClrDisplayBuf();    //清屏
				break;
			case 2://左移
				LoadDotaLeftOut(p,LoadNodeTab[p_node->frame_no].addr,p_node->step-step);
				break;
			case 3://右移
				LoadDotaRightOut(p,LoadNodeTab[p_node->frame_no].addr,p_node->step-step);
				break;
			case 4://上移
				LoadDotaUpOut(p,LoadNodeTab[p_node->frame_no].addr,p_node->step-step);
				break;
			case 5://下移
				LoadDotaDownOut(p,LoadNodeTab[p_node->frame_no].addr,p_node->step-step);
				break;
			case 6://横百叶窗 
				LoadDotaHengOut(p,LoadNodeTab[p_node->frame_no].addr,p_node->step-step);
				break;
			case 7://竖百叶窗
				LoadDotaShuOut(p,LoadNodeTab[p_node->frame_no].addr,p_node->step-step);
				break;
			case 8://闪烁
				ClrDisplayBuf();  //清屏
				break;
			default:break;
		}
  }	
}
/**															   
  ******************************************************************************
  *                              LoadDotaNowIn()
  *
  * 描述   : 立即把要显示的数据全部送到缓冲区中
  *		   
  * 参数   : buf:缓冲区地址
  *          addr:要加载帧数据的地址
  *          step:要加载数据的步数
  *
  *
  * 返回值 : 无
  ******************************************************************************
  */
void LoadDotaNowIn (uint8_t buf[][DIS_BUF_LEN],uint32_t addr,uint16_t step)
{
  uint8_t i;
  uint8_t j;
	for(i=0;i<SCREEN_YANSE;i++)
	{
    for(j=0;j<BOX_LIE;j++)//读取这个箱体每行的数据
		  SPI_FlashRead(buf[i]+j*BOX_HBYTE,
		    addr+4+i*SCREEN_BYTE_NUM+(j+ShowAreaStru.LieStart)*SCREEN_HBYTE+
		    ShowAreaStru.HByteStart,BOX_HBYTE);
		if(G_FrameFlag == 0)//有新RGB数据发送过来
       return;	
	}
}

/**															   
  ******************************************************************************
  *                            LoadDotaLeftIn()
  *
  * 描述   : 立即把要显示的数据左移逐步存入缓冲区0-127(对于宽度为128的点阵)
  *		   
  * 参数   : buf:缓冲区地址
  *          addr:要加载的数据地址
  *          step:要加载数据的步数
  *
  *
  * 返回值 : 无
  ******************************************************************************
  */
void LoadDotaLeftIn (uint8_t buf[][DIS_BUF_LEN],uint32_t addr,uint16_t step)
{
  uint8_t i;
	int16_t k;
	uint16_t j;
	uint16_t start_addr = 0;
  uint16_t byte_cnt;  //每行需要填充的字节数	
	uint8_t bit_cnt;   //需要填充的位数

	typedef union 
	{
		uint16_t data16;
		uint8_t  data8[2];
	} dataUN;
  dataUN dataunion;
	
	byte_cnt = step>>3;//进入从控区域字节数
	if(byte_cnt <= BOX_HBYTE)
		start_addr = 0;
	else
	{
    byte_cnt = BOX_HBYTE;
 	  start_addr = (step>>3) - BOX_HBYTE;
  }
	bit_cnt = step%8;
  if(bit_cnt == 0)//整字节
	{
		for(i=0;i<SCREEN_YANSE;i++)
		{
			for(j=0;j<BOX_LIE;j++)
			{
				SPI_FlashRead(&array[i][j],
					addr+4+i*SCREEN_BYTE_NUM+(j+ShowAreaStru.LieStart)*SCREEN_HBYTE + 
				  start_addr+byte_cnt,1);
			}
	  }
	}
	if(G_FrameFlag == 0)//有新RGB数据发送过来
    return;	
	for(i=0;i<SCREEN_YANSE;i++)
	{
		for(j=0;j<BOX_LIE;j++)
		{
			for(k=0;k<BOX_HBYTE-1;k++)  //行字节(最后一字节单独处理)
			{
				dataunion.data8[1]=buf[i][j*BOX_HBYTE+k];
				dataunion.data8[0]=buf[i][j*BOX_HBYTE+k+1];
				dataunion.data16<<=1;
				buf[i][j*BOX_HBYTE+k]=dataunion.data8[1];
			}
			dataunion.data8[1]=buf[i][j*BOX_HBYTE+BOX_HBYTE-1];
			dataunion.data8[0]=array[i][j];
			dataunion.data16<<=1;
			array[i][j]<<=1;
			buf[i][j*BOX_HBYTE+BOX_HBYTE-1]=dataunion.data8[1];
		}
	}
}



/**															   
  ******************************************************************************
  *                            LoadDotaLeftOut()
  *
  * 描述   : 立即把要显示的数据左移逐步移出缓冲区
  *		   
  * 参数   : buf:缓冲区地址
  *          step:要加载数据的步数
  *
  *
  * 返回值 : 无
  ******************************************************************************
  */
void LoadDotaLeftOut (uint8_t buf[][DIS_BUF_LEN],uint32_t addr,uint16_t step)
{
  uint8_t i;
	int16_t k;
	uint16_t j;
	

	typedef union 
	{
		uint16_t data16;
		uint8_t  data8[2];
	} dataUN;
  dataUN dataunion;

	if(G_FrameFlag == 0)//有新RGB数据发送过来
    return;	
	if(step >= BOX_HANG-1)//全部移出
	{
    ClrDisplayBuf();  //清屏
  }
	else
	{
	  for(i=0;i<BOX_YANSE;i++)
	  {
		  for(j=0;j<BOX_LIE;j++)
		  {
			  for(k=0;k<BOX_HBYTE-1;k++)  //行字节(最后一字节单独处理)
			  {
				  dataunion.data8[1]=buf[i][j*BOX_HBYTE+k];
				  dataunion.data8[0]=buf[i][j*BOX_HBYTE+k+1];
				  dataunion.data16<<=1;
				  buf[i][j*BOX_HBYTE+k]=dataunion.data8[1];
			  }
			  dataunion.data8[1]=buf[i][j*BOX_HBYTE+BOX_HBYTE-1];
			  dataunion.data8[0]=0;
			  dataunion.data16<<=1;
			  buf[i][j*BOX_HBYTE+BOX_HBYTE-1]=dataunion.data8[1];
		  }
	  }
  }	
}


/**															   
  ******************************************************************************
  *                            LoadDotaRightIn()
  *
  * 描述   : 立即把要显示的数据逐步右移存入缓冲区
  *		   
  * 参数   : buf:缓冲区地址
  *          addr:要加载的数据地址
  *          step:要加载数据的步数
  *
  *
  * 返回值 : 无
  ******************************************************************************
  */
void LoadDotaRightIn (uint8_t buf[][DIS_BUF_LEN],uint32_t addr,uint16_t step)
{
  uint8_t i;
	int16_t k;
	uint16_t j;	
	uint16_t start_addr; //要显示的字符起始位置
  uint16_t byte_cnt;   //每行需要填充的字节数	
	uint8_t bit_cnt;     //需要填充的位数
	
	typedef union 
	{
		uint16_t data16;
		uint8_t  data8[2];
	} dataUN;
  dataUN dataunion;
	
  //移动位置到达本箱体  缓存中数据和要显示数据是否相近
	byte_cnt = step>>3;	
	start_addr = SCREEN_HBYTE - byte_cnt-1;
	byte_cnt = byte_cnt>BOX_HBYTE? BOX_HBYTE:byte_cnt;	
	bit_cnt = step%8;

  if(bit_cnt == 0)
	{
		for(i=0;i<SCREEN_YANSE;i++)//读取开始的数据
		{
			for(j=0;j<BOX_LIE;j++)
			 SPI_FlashRead(&array[i][j],
			 addr+4+i*SCREEN_BYTE_NUM+(j+ShowAreaStru.LieStart)*SCREEN_HBYTE+start_addr,1);
	  }
	}
  if(G_FrameFlag == 0)//有新RGB数据发送过来
    return;
	for(i=0;i<SCREEN_YANSE;i++)
	{
		for(j=0;j<BOX_LIE;j++)
		{
			for(k=BOX_HBYTE-1;k>0;k--)  //行字节(最后一字节单独处理)
			{
				dataunion.data8[1]=buf[i][j*BOX_HBYTE+k-1];
				dataunion.data8[0]=buf[i][j*BOX_HBYTE+k];
				dataunion.data16>>=1;
				buf[i][j*BOX_HBYTE+k]=dataunion.data8[0];
			}
			dataunion.data8[0]=buf[i][j*BOX_HBYTE];
			dataunion.data8[1]=array[i][j];
			dataunion.data16>>=1;
			array[i][j]>>=1;
			buf[i][j*BOX_HBYTE]=dataunion.data8[0];
		}
	}
}


/**															   
  ******************************************************************************
  *                            LoadDotaRightOut()
  *
  * 描述   : 立即把要显示的数据左移逐步移出缓冲区
  *		   
  * 参数   :  buf:缓冲区地址
  *          step:要加载数据的步数
  *
  *
  * 返回值 : 无
  ******************************************************************************
  */
void LoadDotaRightOut (uint8_t buf[][DIS_BUF_LEN],uint32_t addr,uint16_t step)
{
  uint8_t i;
	int16_t k;
	uint16_t j;

	typedef union 
	{
		uint16_t data16;
		uint8_t  data8[2];
	} dataUN;
  dataUN dataunion;

	if(G_FrameFlag == 0)//有新RGB数据发送过来
    return;
	if(step >= BOX_HANG-1)//全部移出
	{
    ClrDisplayBuf();  //清屏
  }
	else
	{
	  for(i=0;i<BOX_YANSE;i++)
	  {
		  for(j=0;j<BOX_LIE;j++)
		  {
        for(k=BOX_HBYTE-1;k>0;k--)  //帧字节
			  {
				  dataunion.data8[1]=buf[i][j*BOX_HBYTE+k-1];
				  dataunion.data8[0]=buf[i][j*BOX_HBYTE+k];
				  dataunion.data16>>=1;
				  buf[i][j*BOX_HBYTE+k]=dataunion.data8[0];
			  }
			  dataunion.data8[0]=buf[i][j*BOX_HBYTE];
			  dataunion.data8[1]=0;
			  dataunion.data16>>=1;
			  buf[i][j*BOX_HBYTE]=dataunion.data8[0];		
	    }
		}
  }	 
}


/**															   
  ******************************************************************************
  *                            LoadDotaDownIn()
  *
  * 描述   : 立即把要显示的数据左移逐步移出缓冲区
  *		   
  * 参数   : s:参考缓冲区数据指针
  *          p:要加载的缓冲区指针
  *          p_node:要显示的结点指针
  *
  *
  * 返回值 : 无
  ******************************************************************************
  */
void LoadDotaDownIn (uint8_t buf[][DIS_BUF_LEN],uint32_t addr,uint16_t step)
{
  uint8_t i;
	uint16_t j,z;
	uint8_t *sp,*dp;
	uint16_t start_lie = 0;
	
	start_lie = SCREEN_LIE-1-step;//加载新入数据
	for(i=0;i<SCREEN_YANSE;i++)
	{
    for(j=BOX_LIE-1;j>0;j--)
	  {
      sp = &buf[i][(j-1)*BOX_HBYTE];
			dp = &buf[i][j*BOX_HBYTE];
			for(z=0;z<BOX_HBYTE;z++)
			  *dp++ = *sp++;
    }
  }
	
	if(G_FrameFlag == 0)//有新RGB数据发送过来
    return;		
  for(i=0;i<SCREEN_YANSE;i++)
  {
		  SPI_FlashRead(buf[i],
			  addr+4+i*SCREEN_BYTE_NUM+start_lie*SCREEN_HBYTE+ShowAreaStru.HByteStart,BOX_HBYTE);
	}
}

/**															   
  ******************************************************************************
  *                            LoadDotaDownOut()
  *
  * 描述   : 立即把要显示的数据左移逐步移出缓冲区
  *		   
  * 参数   : s:参考缓冲区数据指针
  *          p:要加载的缓冲区指针
  *          p_node:要显示的结点指针
  *
  *
  * 返回值 : 无
  ******************************************************************************
  */
void LoadDotaDownOut (uint8_t buf[][DIS_BUF_LEN],uint32_t addr,uint16_t step)
{
  uint8_t i;
	uint16_t j,z;
	uint8_t *sp,*dp;
	
	//清零缓冲区
	if(step >= BOX_LIE-1)
	{
	  ClrDisplayBuf();
		return;
	}
	if(G_FrameFlag == 0)//有新RGB数据发送过来
    return;		
	for(i=0;i<SCREEN_YANSE;i++)
	{
    for(j=BOX_LIE-1;j>0;j--)
	  {
      sp = &buf[i][(j-1)*BOX_HBYTE];
			dp = &buf[i][j*BOX_HBYTE];
			for(z=0;z<BOX_HBYTE;z++)
			  *dp++ = *sp++;
    }
		dp = &buf[i][0];
		for(z=0;z<BOX_HBYTE;z++)
		  *dp++ = 0;
		  
  }

}


/**															   
  ******************************************************************************
  *                            LoadDotaUpIn()
  *
  * 描述   : 立即把要显示的数据从下移入缓冲区
  *		   
  * 参数   : s:参考缓冲区数据指针
  *          p:要加载的缓冲区指针
  *          p_node:要显示的结点指针
  *
  *
  * 返回值 : 无
  ******************************************************************************
  */
void LoadDotaUpIn (uint8_t buf[][DIS_BUF_LEN],uint32_t addr,uint16_t step)
{
  uint8_t i;
	uint16_t j,z;
	uint8_t *sp,*dp;
	
	
	for(i=0;i<SCREEN_YANSE;i++)
	{
    for(j=0;j<BOX_LIE-1;j++)
	  {
      sp = &buf[i][(j+1)*BOX_HBYTE];
			dp = &buf[i][j*BOX_HBYTE];
			for(z=0;z<BOX_HBYTE;z++)
			  *dp++ = *sp++;
    }
  }

	if(G_FrameFlag == 0)//有新RGB数据发送过来
    return;	
  for(i=0;i<SCREEN_YANSE;i++)
  {
		  SPI_FlashRead(buf[i]+(BOX_LIE-1)*BOX_HBYTE,
			  addr+4+i*SCREEN_BYTE_NUM+step*SCREEN_HBYTE+ShowAreaStru.HByteStart,BOX_HBYTE);
	}
	
}

/**															   
  ******************************************************************************
  *                            LoadDotaUpOut()
  *
  * 描述   : 立即把要显示的数据左移逐步移出缓冲区
  *		   
  * 参数   : s:参考缓冲区数据指针
  *          p:要加载的缓冲区指针
  *          p_node:要显示的结点指针
  *
  *
  * 返回值 : 无
  ******************************************************************************
  */
void LoadDotaUpOut (uint8_t buf[][DIS_BUF_LEN],uint32_t addr,uint16_t step)
{
  uint8_t i;
	uint16_t j,z;
	uint8_t *sp,*dp;
	
	//清零缓冲区
	if(step >= BOX_LIE-1)
	{
	  ClrDisplayBuf();
		return;
	}
	if(G_FrameFlag == 0)//有新RGB数据发送过来
    return;	

	for(i=0;i<SCREEN_YANSE;i++)
	{
    for(j=0;j<BOX_LIE-1;j++)
	  {
      sp = &buf[i][(j+1)*BOX_HBYTE];
			dp = &buf[i][j*BOX_HBYTE];
			for(z=0;z<BOX_HBYTE;z++)
			  *dp++ = *sp++;
    }
		dp = &buf[i][(BOX_LIE-1)*BOX_HBYTE];
		for(z=0;z<BOX_HBYTE;z++)
			*dp++ = 0;		
  }	
}

/**															   
  ******************************************************************************
  *                            LoadDotaHengIn()
  *
  * 描述   : 横百叶窗-以字节为单位
  *		   
  * 参数   : s:参考缓冲区数据指针
  *          p:要加载的缓冲区指针
  *          p_node:要显示的结点指针
  *
  *
  * 返回值 : 无
  ******************************************************************************
  */
void LoadDotaHengIn (uint8_t buf[][DIS_BUF_LEN],uint32_t addr,uint16_t step)
{
  uint8_t i,k;
	uint16_t j;
	
	//清零缓冲区
  ClrDisplayBuf();
	if(G_FrameFlag == 0)//有新RGB数据发送过来
    return;			
  for(i=0;i<SCREEN_YANSE;i++)//颜色
  {
    for(k=0;k<BOX_LIE;k+=8)//列字节
	  {
      for(j=0;j<=step;j++)
		    SPI_FlashRead(buf[i]+(k+j)*BOX_HBYTE,
			    addr+4+i*SCREEN_BYTE_NUM+(k+j+ShowAreaStru.LieStart)*SCREEN_HBYTE+ShowAreaStru.HByteStart,BOX_HBYTE);
    }
  }    
}



/**															   
  ******************************************************************************
  *                            LoadDotaHengOut()
  *
  * 描述   : 横百叶窗-以字节为单位
  *		   
  * 参数   : s:参考缓冲区数据指针
  *          p:要加载的缓冲区指针
  *          p_node:要显示的结点指针
  *
  *
  * 返回值 : 无
  ******************************************************************************
  */
void LoadDotaHengOut (uint8_t buf[][DIS_BUF_LEN],uint32_t addr,uint16_t step)
{
  uint8_t i,k;
	uint16_t j;
	
  //缓存中数据和要显示数据是否相近
  for(i=0;i<SCREEN_YANSE;i++)//颜色
  {
    for(k=0;k<BOX_LIE;k+=8)//列字节
	  {
		  for(j=0;j<BOX_HBYTE;j++)//清屏
				buf[i][(step+k)*BOX_HBYTE+j] = 0;
    }
		if(G_FrameFlag == 0)//有新RGB数据发送过来
      return;	
  }    
}

/**															   
  ******************************************************************************
  *                            LoadDotaShuIn()
  *
  * 描述   : 竖百叶窗-以字节为单位
  *		   
  * 参数   : s:参考缓冲区数据指针
  *          p:要加载的缓冲区指针
  *          p_node:要显示的结点指针
  *
  *
  * 返回值 : 无
  ******************************************************************************
  */
const uint8_t shuIN_tab[] = {0x80,0xc0,0xe0,0xf0,0xf8,0xfc,0xfe,0xff};
void LoadDotaShuIn (uint8_t buf[][DIS_BUF_LEN],uint32_t addr,uint16_t step)
{
  uint8_t i,k;
	uint16_t j;
	uint8_t data;
	uint8_t *datap;
	
	for(i=0;i<BOX_YANSE;i++)
	{
    for(k=0;k<BOX_LIE;k++)
		  SPI_FlashRead(buf[i]+k*BOX_HBYTE,
	      addr+4+i*SCREEN_BYTE_NUM+(k+ShowAreaStru.LieStart)*SCREEN_HBYTE+ShowAreaStru.HByteStart,BOX_HBYTE);    
  }
	if(G_FrameFlag == 0)//有新RGB数据发送过来
    return;	
  if(step < 7)	//为7时全部显示，不需要组装数据
	{
	  data = shuIN_tab[step];
		for(i=0;i<BOX_YANSE;i++)//颜色
    {
      datap = buf[i];
		  for(j=0;j<BOX_BYTE_NUM;j++)
		    *datap++ &= data;
    }
  }	
}



/**															   
  ******************************************************************************
  *                            LoadDotaShuOut()
  *
  * 描述   : 竖百叶窗-以字节为单位
  *		   
  * 参数   : s:参考缓冲区数据指针
  *          p:要加载的缓冲区指针
  *          p_node:要显示的结点指针
  *
  *
  * 返回值 : 无
  ******************************************************************************
  */
const uint8_t shuOUT_tab[] = {0x7f,0x3f,0x1f,0x0f,0x07,0x03,0x01,0x00};
void LoadDotaShuOut (uint8_t buf[][DIS_BUF_LEN],uint32_t addr,uint16_t step)
{
  uint8_t i,k;
	uint16_t j;
	uint8_t data;
	uint8_t *datap;
	
	if(step == 7)//最后一步，清零
		ClrDisplayBuf();
	else
	{
	  data = shuOUT_tab[step];
	  for(i=0;i<SCREEN_YANSE;i++)
	  {
      for(k=0;k<BOX_LIE;k++)
		    SPI_FlashRead(buf[i]+k*BOX_HBYTE,
	        addr+4+i*SCREEN_BYTE_NUM+(k+ShowAreaStru.LieStart)*SCREEN_HBYTE+ShowAreaStru.HByteStart,BOX_HBYTE);    
    }
		if(G_FrameFlag == 0)//有新RGB数据发送过来
      return;	
	  for(i=0;i<SCREEN_YANSE;i++)//颜色
    {
      datap = buf[i];
		  for(j=0;j<BOX_BYTE_NUM;j++)
		    *datap++ &= data;
    } 
  }
}


//清零显示缓冲区
void ClrDisplayBuf (void)
{
	uint16_t j;
  uint8_t i,*pot;	
	//清零缓冲区
	for(i=0;i<SCREEN_YANSE;i++)
	{
		pot = (uint8_t *)&DisplayBuf[i];
		for(j=0;j<BOX_BYTE_NUM;j++)
			*pot++ = 0;
		if(G_FrameFlag == 0)//有新RGB数据发送过来
      return;	
	}  
}

