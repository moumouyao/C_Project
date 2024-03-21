#include "Code.h"
#include "sw_global.h"
#include "malloc.h"
#include "memp.h"
#include "sw_crc16.h"
#include "os_cpu.h"
//--------------------------------------以下为SPI相关函数---------------------------------------//
void init_SPI()
{
  GPIO_InitTypeDef  GPIO_InitStructure;
 
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);//使能GPIOB时钟
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG, ENABLE);//使能GPIOG时钟
	  //GPIOB14
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//输出
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
  GPIO_Init(GPIOG, &GPIO_InitStructure);//初始化
	  
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;//PB7
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//输出
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
  GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;//
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//
  //GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
  GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化
	PBout(7) = 1;   //复位拉高
	Code_CS=1;			//SPI不选中
	SPI1_Init();		//初始化SPI
	SPI1_SetSpeed(SPI_BaudRatePrescaler_128);		//设置4为21M时钟,高速模式

}
//以下是SPI模块的初始化代码，配置成主机模式 						  
//SPI口初始化
//这里针是对SPI1的初始化
void SPI1_Init(void)
{	 
  GPIO_InitTypeDef  GPIO_InitStructure;
  SPI_InitTypeDef  SPI_InitStructure;
	
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);//使能GPIOB时钟
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);//使能SPI1时钟
 
  //GPIOFB3,4,5初始化设置
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5;//PB3~5复用功能输出	
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用功能
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
  GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化
	
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource3,GPIO_AF_SPI1); //PB3复用为 SPI1
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource4,GPIO_AF_SPI1); //PB4复用为 SPI1
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource5,GPIO_AF_SPI1); //PB5复用为 SPI1
 
	//这里只针对SPI口初始化
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_SPI1,ENABLE);//复位SPI1
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_SPI1,DISABLE);//停止复位SPI1

	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  //设置SPI单向或者双向的数据模式:SPI设置为双线双向全双工
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;		//设置SPI工作模式:设置为主SPI
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;		//设置SPI的数据大小:SPI发送接收8位帧结构
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;		//串行同步时钟的空闲状态为低电平
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;	//串行同步时钟的第1个跳变沿（上升或下降）数据被采样
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;		//NSS信号由硬件（NSS管脚）还是软件（使用SSI位）管理:内部NSS信号有SSI位控制
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;		//定义波特率预分频的值:波特率预分频值为256
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;	//指定数据传输从MSB位还是LSB位开始:数据传输从MSB位开始
	SPI_InitStructure.SPI_CRCPolynomial = 7;	//CRC值计算的多项式
	SPI_Init(SPI1, &SPI_InitStructure);  //根据SPI_InitStruct中指定的参数初始化外设SPIx寄存器
 
	SPI_Cmd(SPI1, ENABLE); //使能SPI外设

	//SPI1_ReadWriteByte(0xff);//启动传输		 
}   
//SPI1速度设置函数
//SPI速度=fAPB2/分频系数
//@ref SPI_BaudRate_Prescaler:SPI_BaudRatePrescaler_2~SPI_BaudRatePrescaler_256  
//fAPB2时钟一般为84Mhz：
void SPI1_SetSpeed(u8 SPI_BaudRatePrescaler)
{
  assert_param(IS_SPI_BAUDRATE_PRESCALER(SPI_BaudRatePrescaler));//判断有效性
	SPI1->CR1&=0XFFC7;//位3-5清零，用来设置波特率
	SPI1->CR1|=SPI_BaudRatePrescaler;	//设置SPI1速度 
	SPI_Cmd(SPI1,ENABLE); //使能SPI1
}
//SPI1 读写一个字节
//TxData:要写入的字节
//返回值:读取到的字节
u8 SPI1_ReadWriteByte(u8 TxData)
{		 			 
 
  while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET){}//等待发送区空  
	
	SPI_I2S_SendData(SPI1, TxData); //通过外设SPIx发送一个byte  数据
		
  while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET){} //等待接收完一个byte  
 
	return SPI_I2S_ReceiveData(SPI1); //返回通过SPIx最近接收的数据	
}

//--------------------------------------------解码IC相关-----------------------------------------//

void Rst_Code()//复位解码IC
{
	Code_Rst = 0;//复位操作
	delay_ms(8);
	Code_Rst = 1;
	delay_ms(30);
}
/*
函数：WriteCode
功能：向Code写入一段数据
参数：*cData:要写入的数据
			cLong : 要写入的字节数
			Block:是否阻塞进程  =1阻塞 =0不阻塞
返回值 0=成功
			 1=失败
*/
unsigned char WriteCode(unsigned char *cData,unsigned short iLong,unsigned char Block)
{
	unsigned short i;
	unsigned char cReturn=1;
	do
	{
		if(Code_Busy != 0)//如果IC内有数据就不写入
		{
			//Code_CS=0;//使能Code
			for(i=0;i<iLong;i++)
			{
				SPI1_ReadWriteByte(*(cData+i));//写入一个数据
			}
			//Code_CS=1;//失能Code
			cReturn = 0;//写入成功
		}
		else if(!Block)//不阻塞进程释放
		{
			OSTimeDly(10);//释放CPU
			cReturn = 1;//写入失败
		}
	}
	while(Block && cReturn);//如果阻塞就循环发送
	return cReturn;
}
/*
函数：ReadCode(自动剔除帧头的数据长度)
功能：向Code读出一段
参数：*cData:读出的数据
      cLong : 读出的长度（字节数）
			Block:是否阻塞进程  =1阻塞 =0不阻塞
返回值 0=成功
			 1=失败
*/
unsigned char ReadCode(unsigned char *cData,unsigned short *iLong,unsigned char Block)
{
	unsigned char cLongH;
	unsigned char cLongL;
	unsigned short i;
	unsigned char cReturn=1;
	do
	{
		if(Code_Busy == 0)//是否解码芯片已经准备好了
		{
			delay_us(8);//延迟一下
			Code_CS=0;//使能Code
			cLongH = SPI1_ReadWriteByte(0x00);//读长度高位
			cLongL = SPI1_ReadWriteByte(0x00);//读长度低位
			*iLong = cLongH<<8|cLongL;//合成长度
			for(i=0;i<*iLong;i++)//读取剩下的数据
			{
				*(cData+i) = SPI1_ReadWriteByte(0x00);//读取数据
			}
			Code_CS=1;//失能Code
			cReturn = 0;//成功
		}
		else if(!Block)//不阻塞进程释放
		{
			OSTimeDly(1);//释放CPU
			cReturn = 1;//读取失败
		}
	}
	while(Block && cReturn);
	return cReturn;
}
/*
函数：EncryptData 加密数据
参数：cOriginal:原始数据
			cOlong:原始数据长度
			cDlong:加密后的数据长度
			Block:是否阻塞进程  =1阻塞 =0不阻塞
返回值：
			加密后的数据指针
*/
unsigned char* EncryptData(unsigned char *cOriginal,unsigned int iOlong,unsigned int *iDlong,unsigned char Block)
{
	unsigned char cTxdHead[8]={0x00,0x00,0x00,0x88,0x00,0x02,0x00};//前两位是总长度，最后一位是长度数据长度
	unsigned short iTxdLong;//总长度
	unsigned char cCompletion;//补全长度 16倍数补全
	unsigned int cActualLong=0;//补全后实际长度
	unsigned char cMoPaired;//向加密IC发送的次数（1次240个字节）
	unsigned char cRemainder;//除下240整倍数剩余的
	unsigned char *MoSbuff;//发送至IC的缓存
	unsigned char *MiSbuff;//接收IC数据缓存
	unsigned char *MiSbuffSum[20];//存放接收IC数据缓存的指针数组(数据池) 最大接收240*20个字节
	unsigned char *MiReturn;//需要返回的数据总汇
	unsigned short MiLong;//读出IC数据的长度 
	unsigned char err;//错误信息
	//-----循环系数定义
	unsigned short i;//240整倍数计数
	unsigned char j;
	unsigned int k;//失败检测计数
	unsigned char z;
	cCompletion = 16 - (iOlong%16);
	cCompletion = cCompletion==16?0:cCompletion;//计算需要补全的字节数
	cActualLong = iOlong + cCompletion;//计算补全后的实际长度
	cMoPaired = cActualLong/240;//计算发送240个字节的次数
	for(i=0;i<cMoPaired;i++)//一次加密240个字节
	{
		MoSbuff = mymalloc(SRAMEX,240);//申请内存
		for(j=0;j<240;j++)
		{
			MoSbuff[j] = cOriginal[j+(i*240)];//复制要发送的内容
		}
		cTxdHead[6] = 240;//数据长度
		iTxdLong = cTxdHead[6] + 5;//总长度=数据长度+5字节帧头
		cTxdHead[0] = iTxdLong>>8;//总长度高位
		cTxdHead[1] = iTxdLong;//总长度地位
		Code_CS=0;//使能Code
		WriteCode(cTxdHead,7,Block);//写入帧头
		WriteCode(MoSbuff,240,Block);//写入内容
		Code_CS=1;//失能Code
		myfree(SRAMEX,MoSbuff);//释放内存
		//------发送完毕开始接收
		MiSbuff = mymalloc(SRAMEX,255);//申请内存
		k=0;
		do
		{
			k++;
			err = ReadCode(MiSbuff,&MiLong,0);//这里不阻塞进程
		}
		while((err==1) && (k<6000));//循环条件->接收失败并且没有超时
		MiSbuffSum[i] = MiSbuff;//放入数据池
	}
	cRemainder = cActualLong - (cMoPaired*240);//计算除下240整倍数剩余的数量
	if(cRemainder != 0)
	{
		MoSbuff = mymalloc(SRAMEX,cRemainder);//申请内存
		for(j=0;j<cRemainder;j++)
		{
			MoSbuff[j] = ((j+(i*240))>=iOlong)?0:cOriginal[j+(i*240)];//复制要发送的内容(如果要写入的内容大于原始数据长度就用0补全)
		}
		cTxdHead[6] = cRemainder;//数据长度
		iTxdLong = cTxdHead[6] + 5;//总长度=数据长度+5字节帧头
		cTxdHead[0] = iTxdLong>>8;//总长度高位
		cTxdHead[1] = iTxdLong;//总长度地位
		Code_CS=0;//使能Code
		WriteCode(cTxdHead,7,Block);//写入帧头
		WriteCode(MoSbuff,cRemainder,Block);//写入内容
		Code_CS=1;//失能Code
		myfree(SRAMEX,MoSbuff);//释放内存
		//------发送完毕开始接收
		MiSbuff = mymalloc(SRAMEX,255);//申请内存
		k=0;
		do
		{
			k++;
			err = ReadCode(MiSbuff,&MiLong,Block);//这里接收来自IC的数据
		}
		while((err==1) && (k<6000));//循环条件->接收失败并且没有超时
		MiSbuffSum[i] = MiSbuff;//放入数据池
	}
	//------------接收密文完毕
	MiReturn = mymalloc(SRAMEX,cRemainder+(i*240));//申请内存
	*iDlong = cRemainder+(i*240);//计算密文总长度
	for(j=0;j<=i;j++)//将数据池的数据按顺序写入数据总汇
	{
		k = j==i?cRemainder:240;//如果是数据池最后一组数据 该组数据不满240 而是等于剩余数据
		for(z=0;z<k;z++)
		{
			MiReturn[(j*240) + z] = MiSbuffSum[j][z];//数据池：第j组数据的第k个字节
		}
		myfree(SRAMEX,MiSbuffSum[j]);//释放内存
	}
	return MiReturn;//返回接收到的密文
}

/*
函数：DecryptData 解密数据
参数：cOriginal:原始数据
			cOlong:原始数据长度
			cDlong:解密后的数据长度
			Block:是否阻塞进程  =1阻塞 =0不阻塞
返回值：
			解密后的数据指针
*/
unsigned char* DecryptData(unsigned char *cOriginal,unsigned int iOlong,unsigned int *iDlong,unsigned char Block)
{
	unsigned char cTxdHead[8]={0x00,0x00,0x00,0x88,0x01,0x02,0x00};//前两位是总长度，最后一位是长度数据长度
	unsigned short iTxdLong;//总长度
	unsigned char cCompletion;//补全长度 16倍数补全
	unsigned int cActualLong=0;//补全后实际长度
	unsigned char cMoPaired;//向加密IC发送的次数（1次240个字节）
	unsigned char cRemainder;//除下240整倍数剩余的
	unsigned char *MoSbuff;//发送至IC的缓存
	unsigned char *MiSbuff;//接收IC数据缓存
	unsigned char *MiSbuffSum[20];//存放接收IC数据缓存的指针数组(数据池) 最大接收240*20个字节
	unsigned char *MiReturn;//需要返回的数据总汇
	unsigned short MiLong;//读出IC数据的长度 
	unsigned char err;//错误信息
	//-----循环系数定义
	unsigned short i;//240整倍数计数
	unsigned char j;
	unsigned int k;//失败检测计数
	unsigned char z;
	cCompletion = 16 - (iOlong%16);
	cCompletion = cCompletion==16?0:cCompletion;//计算需要补全的字节数(密文都是16的倍数此项可以省略)
	cActualLong = iOlong + cCompletion;//计算补全后的实际长度
	cMoPaired = cActualLong/240;//计算发送240个字节的次数
	for(i=0;i<cMoPaired;i++)//一次解密240个字节
	{
		MoSbuff = mymalloc(SRAMEX,240);//申请内存
		for(j=0;j<240;j++)
		{
			MoSbuff[j] = cOriginal[j+(i*240)];//复制要发送的内容
		}
		cTxdHead[6] = 240;//数据长度
		iTxdLong = cTxdHead[6] + 5;//总长度=数据长度+5字节帧头
		cTxdHead[0] = iTxdLong>>8;//总长度高位
		cTxdHead[1] = iTxdLong;//总长度地位
		Code_CS=0;//使能Code
		WriteCode(cTxdHead,7,Block);//写入帧头
		WriteCode(MoSbuff,240,Block);//写入内容
		Code_CS=1;//失能Code
		myfree(SRAMEX,MoSbuff);//释放内存
		//------发送完毕开始接收
		MiSbuff = mymalloc(SRAMEX,255);//申请内存
		k=0;
		do
		{
			k++;
			err = ReadCode(MiSbuff,&MiLong,0);//这里不阻塞进程
		}
		while((err==1) && (k<6000));//循环条件->接收失败并且没有超时
		MiSbuffSum[i] = MiSbuff;//放入数据池
	}
	cRemainder = cActualLong - (cMoPaired*240);//计算除下240整倍数剩余的数量
	if(cRemainder != 0)
	{
		MoSbuff = mymalloc(SRAMEX,cRemainder);//申请内存
		for(j=0;j<cRemainder;j++)
		{
			MoSbuff[j] = ((j+(i*240))>=iOlong)?0:cOriginal[j+(i*240)];//复制要发送的内容(如果要写入的内容大于原始数据长度就用0补全，解密过程可以省略)
		}
		cTxdHead[6] = cRemainder;//数据长度
		iTxdLong = cTxdHead[6] + 5;//总长度=数据长度+5字节帧头
		cTxdHead[0] = iTxdLong>>8;//总长度高位
		cTxdHead[1] = iTxdLong;//总长度地位
		Code_CS=0;//使能Code
		WriteCode(cTxdHead,7,Block);//写入帧头
		WriteCode(MoSbuff,cRemainder,Block);//写入内容
		Code_CS=1;//失能Code
		myfree(SRAMEX,MoSbuff);//释放内存
		//------发送完毕开始接收
		MiSbuff = mymalloc(SRAMEX,255);//申请内存
		k=0;
		do
		{
			k++;
			err = ReadCode(MiSbuff,&MiLong,Block);//这里接收来自IC的数据
		}
		while((err==1) && (k<6000));//循环条件->接收失败并且没有超时
		MiSbuffSum[i] = MiSbuff;//放入数据池
	}
	//------------接收明文完毕
	MiReturn = mymalloc(SRAMEX,cRemainder+(i*240));//申请内存
	for(j=0;(MiSbuff[j]!=0x03)&&(j<cRemainder);j++);//计算最后一个240的明文长度
	j++;
	*iDlong = j+(i*240);//计算明文总长度
	for(j=0;j<=i;j++)//将数据池的数据按顺序写入数据总汇
	{
		k = j==i?cRemainder:240;//如果是数据池最后一组数据 该组数据不满240 而是等于剩余数据
		for(z=0;z<k;z++)
		{
			MiReturn[(j*240) + z] = MiSbuffSum[j][z];//数据池：第j组数据的第k个字节
		}
		myfree(SRAMEX,MiSbuffSum[j]);//释放内存
	}
	return MiReturn;//返回接收到的密文
}

//=================以下为加密解密进程==================//

void TestCode(void *p_arg)
{
	(void)p_arg;                				// 'p_arg' 并没有用到，防止编译器提示警告
	StrFrameHeader *mInHead=NULL; //本函数输入的数据流
	StrFrameHeader *mOutHead=NULL; //本函数输出的数据流
	unsigned char err;
	unsigned char *cOutData;//解密后的原文
	static unsigned char cOutError[]={0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff};//错误报文回复
	unsigned int iOutLong;//解密后的原文长度
	INT8U *memp_buf;//报文数据
	unsigned int i;
	unsigned char cUartFlag=0;//串口标志位
		while(1)
		{
			//==================断帧数据合成完毕准备去处理
			if(*mInHead->pdata != 0x02)//判断密文帧头
			{
				cOutData = cOutError;//指向回复错误
				iOutLong = 8;
				memp_free(MEMP_PBUF_POOL,(void *)mInHead);//释放POOL内存
			}
			else//密文正确
			{
				for(i=0;i<mInHead->len;i++)//消除帧头和帧尾只保留密文
				{
					mInHead->pdata[i] = mInHead->pdata[i+1];
				}
				mInHead->len -= 2;//减去帧头帧尾的长度
				cOutData = DecryptData(mInHead->pdata,mInHead->len,&iOutLong,1);//阻塞等待解密完成
				memp_free(MEMP_PBUF_POOL,(void *)mInHead);//释放POOL内存
			}
			
			//================整理数据准备转发到任务处理任务
			memp_buf = memp_malloc(MEMP_PBUF_POOL);//申请一块内存pool
			mOutHead = (StrFrameHeader *)memp_buf;
			mOutHead->type = HEAD_ETH_TYPE;
			mOutHead->pdata = memp_buf + sizeof(StrFrameHeader);

			memcpy(mOutHead->pdata,cOutData,iOutLong);
			mOutHead->len=iOutLong;					

			myfree(SRAMEX,cOutData);//释放内存(明文数据)
			do
			{
				err = OSQPost(CtrlCentRecMsgQeue,(void *)mOutHead);//把拷贝的数据发送给任务处理
			}
			while(err != OS_ERR_NONE);//阻塞等待发送完毕

			//===================以下是处理来自任务处理的数据============================//
			mInHead = OSQPend(UdpTranMsgQeue,0,&err);   	//阻塞等待接收处理数据
			cOutData = EncryptData(mInHead->pdata,mInHead->len,&iOutLong,1);//阻塞等待加密完成
			memp_free(MEMP_PBUF_POOL,(void *)mInHead);//释放POOL内存
			//========整理数据准备转发到网络任务
			memp_buf = memp_malloc(MEMP_PBUF_POOL);//申请一块内存pool
			mOutHead = (StrFrameHeader *)memp_buf;
			mOutHead->type = HEAD_ETH_TYPE;
			mOutHead->pdata = memp_buf + sizeof(StrFrameHeader);
			memp_buf =mOutHead->pdata;         //定位接收数据指针

			memcpy(mOutHead->pdata,cOutData,iOutLong);
			mOutHead->len=iOutLong;	

			myfree(SRAMEX,cOutData);//释放内存(密文数据)
			if(cUartFlag==0)//如果不是串口数据
			{
				do
				{
					err = OSQPost(Dis_En_CodeQeue,(void *)mOutHead);//把拷贝的数据发送给网络任务
				}
				while(err != OS_ERR_NONE);//阻塞等待发送完毕
			}
			else
			{
				for(i=0;i<mOutHead->len;i++)
				{
				printf("%c",mOutHead->pdata[i]);//发送数据
				}
				memp_free(MEMP_PBUF_POOL,(void *)mOutHead);//释放POOL内存
			}
			//==================判断是否需要修改密钥========================//
			if(g_cNewCodeKey != NULL)//如果新密钥指针有数据，代表需要修改密钥
			{		
				AlterCodeKey(0);//修改密钥
				myfree(SRAMEX,g_cNewCodeKey);//释放内存
				g_cNewCodeKey = NULL;//指向空
			}
		}
	
}

void GetCodeKeyAck(StrFrameHeader *frame_head,unsigned char* p,unsigned short*len)//接收密钥并回复上位机
{
	unsigned char *cData;//报文的数据指针
	unsigned char i;//用于计数
	unsigned char *p1;//指向P,用于计算数据总长度
	unsigned short crc16;//CRC校验
	
	cData = frame_head->pdata;//数据
	p1 = p;
	g_cNewCodeKey = mymalloc(SRAMEX,16);//申请内存
	for(i=0;i<16;i++)
	{
		g_cNewCodeKey[i] = cData[i+5];//写入密钥数据
	}
	i+=(2+5);//指向数据帧尾
	if(cData[i] == 0x03)//判断帧尾
	{
		//写入回复成功的报文
		*p++ = 0x02;         //帧头
		*p++ = HostAddrH;    //主控制器地址高位
		*p++ = HostAddrL;    //主控制器地址低位
		*p++ = '0';
	}
	else//报文错误回复失败
	{
		//写入回复失败的报文
		*p++ = 0x02;         //帧头
		*p++ = HostAddrH;    //主控制器地址高位
		*p++ = HostAddrL;    //主控制器地址低位
		*p++ = '1';
		myfree(SRAMEX,g_cNewCodeKey);//释放内存
		g_cNewCodeKey = NULL;//指向空
	}
	//=======CRC检验
	crc16 = SWCRC16(p1+1,p-p1-1);
	*p++ = (uint8_t)(crc16>>8); //校验高字节在前
	*p++ = (uint8_t)(crc16);    //校验低字节在后
	*p++ = 0x03;              //帧尾
  *len = p-p1;   //数据个数：帧头+地址+数据+crc+帧未
}

void AlterCodeKey(unsigned char cmd)
{
	OS_CPU_SR cpu_sr;
	unsigned char cCommandErasure[] ={0x00,0x05,0x80,0x0E,0x00,0x00,0x00};//擦除命令
	unsigned char cCommandNewFile[]={0x00,0x0C,0x80,0xE0,0x00,0x00,0x07,0x3F,0x03,0x00,0xFF,0xF0,0xFF,0xFF};//创建文件指令
	unsigned char cCommandEnCodeKey[]={0x00,0x1A,0x80,0xD4,0x01,0x02,0x15,0x30,0xF0,0xFA,0x01,0x04,\
	0x01,0x23,0x45,0x67,0x89,0xab,0xcd,0xef,0xfe,0xdc,0xba,0x98,0x76,0x54,0x32,0x10};//写入SM4加密密匙
	unsigned char cCommandDisCodeKey[]={0x00,0x1A,0x80,0xD4,0x01,0x02,0x15,0x31,0xF0,0xFA,0x01,0x04,\
	0x01,0x23,0x45,0x67,0x89,0xab,0xcd,0xef,0xfe,0xdc,0xba,0x98,0x76,0x54,0x32,0x10};//写入SM4解密密匙
	unsigned char *cReData;//接收COde芯片的数据
	unsigned short iLong;//数据长度
	unsigned char err;
	unsigned int i;
	
	//============读入新密匙===============//
	if(cmd == 0)//正常写入
	{
		for(i=0;i<16;i++)
		{
			cCommandEnCodeKey[i+12] = g_cNewCodeKey[i];
			cCommandDisCodeKey[i+12] = g_cNewCodeKey[i];
		}
	}
	cReData = mymalloc(SRAMEX,20);//申请内存
	OS_ENTER_CRITICAL(); //关中断

	//======================写入擦除命令==================//
	Code_CS=0;//使能Code
	WriteCode(cCommandErasure,7,1);//写入数据
	Code_CS=1;//失能Code
	i=0;
	do
	{
		i++;
		err = ReadCode(cReData,&iLong,0);//这里不阻塞进程
	}
	while((err==1) && (i<6000));//循环条件->接收失败并且没有超时
	delay_ms(100);
	Rst_Code();//复位一下IC
	//==============写入创建文件指令================//
	Code_CS=0;//使能Code
	WriteCode(cCommandNewFile,14,1);//写入数据
	Code_CS=1;//失能Code
	i=0;
	do
	{
		i++;
		err = ReadCode(cReData,&iLong,0);//这里不阻塞进程
	}
	while((err==1) && (i<6000));//循环条件->接收失败并且没有超时
	delay_ms(100);
	Rst_Code();//复位一下IC
	//============写入加密密匙==================//
	Code_CS=0;//使能Code
	WriteCode(cCommandEnCodeKey,28,1);//写入数据
	Code_CS=1;//失能Code
	i=0;
	do
	{
		i++;
		err = ReadCode(cReData,&iLong,0);//这里不阻塞进程
	}
	while((err==1) && (i<6000));//循环条件->接收失败并且没有超时
	delay_ms(100);
	Rst_Code();//复位一下IC
	//============写入解密密匙==================//
	Code_CS=0;//使能Code
	WriteCode(cCommandDisCodeKey,28,1);//写入数据
	Code_CS=1;//失能Code
	i=0;
	do
	{
		i++;
		err = ReadCode(cReData,&iLong,0);//这里不阻塞进程
	}
	while((err==1) && (i<6000));//循环条件->接收失败并且没有超时
	delay_ms(100);
	Rst_Code();//复位一下IC
	myfree(SRAMEX,cReData);//释放内存
	OS_EXIT_CRITICAL();  //开中断
}

