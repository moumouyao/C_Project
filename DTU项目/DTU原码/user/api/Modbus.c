#include "Modbus.h"

MODBUS modbus;
uint16_t Reg[] ={	0x0001,
									0x0012,
									0x0013,
									0x0004,
									0x0025,
									0x0036,
									0x0007,
									0X0008,
								};//reg是提前定义好的寄存器和寄存器数据，要读取和改写的部分内容

uint16_t modbus_data[5] = {0};

 //参数1从机地址，参数2起始地址，参数3寄存器个数
 void Host_Read03_slave(uint8_t slave,uint16_t StartAddr,uint16_t num)
 {
// 	int j;
 	uint16_t crc;//计算的CRC校验位
 	modbus.slave_add=slave;//这是先把从机地址存储下来，后面接收数据处理时会用到
 	modbus.Host_Txbuf[0]=slave;//这是要匹配的从机地址
 	modbus.Host_Txbuf[1]=0x03;//功能码
 	modbus.Host_Txbuf[2]=StartAddr/256;//起始地址高位
 	modbus.Host_Txbuf[3]=StartAddr%256;//起始地址低位
 	modbus.Host_Txbuf[4]=num/256;//寄存器个数高位
 	modbus.Host_Txbuf[5]=num%256;//寄存器个数低位
 	crc=Modbus_CRC16(&modbus.Host_Txbuf[0],6); //获取CRC校验位
 	modbus.Host_Txbuf[6]=crc%256;//寄存器个数高位
 	modbus.Host_Txbuf[7]=crc/256;//寄存器个数低位
 	//发送数据包装完毕(共8个字节)
 	//开始发送数据
 	RS485_TX_ENABLE();//使能485控制端(启动发送)  

 	usart_send_array(USART2, modbus.Host_Txbuf, 8);
	for(uint8_t i = 0; i < 8; i++){
		printf("%x\t", modbus.Host_Txbuf[i]);
	}
 	RS485_RX_ENABLE();//失能485控制端（改为接收）
 }

 
  //主机接收从机的消息进行处理功能码0x03
 void HOST_ModbusRX()
 {
 	u16 crc,rccrc;//计算crc和接收到的crc
 
   if(modbus.reflag == 0)  //如果接收未完成则返回空
 	{
 	   return;
 	}
 	//接收数据结束
 	
 	//（数组中除了最后两位CRC校验位其余全算）
 	crc = Modbus_CRC16(&modbus.rcbuf[0],modbus.recount-2); //获取CRC校验位
//	crc = modbus.rcbuf[modbus.recount-2]*256+modbus.rcbuf[modbus.recount-1];//计算读取的CRC校验位
 	rccrc = modbus.rcbuf[modbus.recount-1]*256+modbus.rcbuf[modbus.recount-2];//计算读取的CRC校验位
 	
 	if(crc == rccrc) //CRC检验成功 开始分析包
 	{	
 	   if(modbus.rcbuf[0] == modbus.slave_add)  // 检查地址是是对应从机发过来的
 		 {
 			 if(modbus.rcbuf[1]==3)//功能码时03
 		      Host_Func3();//这是读取寄存器的有效数据位进行计算
 		 }
 		 
 	}	
 	 modbus.recount = 0;//接收计数清零
    modbus.reflag = 0; //接收标志清零
 	
 }
 
 
  void Host_Func3(void)
 {
 	int i;
 	int count=(int)modbus.rcbuf[2];//这是数据个数
 	uint8_t j = 0;
// 	printf("从机返回 %d 个寄存器数据：\r\n",count/2);
 	for(i=0;i<count;i=i+2)
 	{
 		printf("Temp_Hbit= %d Temp_Lbit= %d temp= %d\r\n",
		(int)modbus.rcbuf[3+i],(int)modbus.rcbuf[4+i],(int)modbus.rcbuf[4+i]+((int)modbus.rcbuf[3+i])*256);
		modbus_data[j] = (int)modbus.rcbuf[4+i]+((int)modbus.rcbuf[3+i])*256;
		j++;
 	}
 }


 // Modbus初始化函数
 void Modbus_Init()
 {
   modbus.myadd = 0x02; //从机设备地址为2
   modbus.timrun = 0;    //modbus定时器停止计算
 	modbus.slave_add=0x01;//主机要匹配的从机地址（本设备作为主机时）
 }

 
 
 // Modbus事件处理函数
 void Modbus_Event()
 {
 	uint16_t crc,rccrc;//crc和接收到的crc
 	//没有收到数据包
   if(modbus.reflag == 0)  //如果接收未完成则返回空
 	{
 	   return;
 	}
//	printf("接收到数据包     ");
//	printf("%s\r\n", modbus.rcbuf);
 	//收到数据包(接收完成)
 	//通过读到的数据帧计算CRC
 	//参数1是数组首地址，参数2是要计算的长度（除了CRC校验位其余全算）
 	crc = Modbus_CRC16(&modbus.rcbuf[0],modbus.recount-2); //获取CRC校验位
//	printf("校验位 %x\r\n", crc);
 	// 读取数据帧的CRC
//	crc = modbus.rcbuf[modbus.recount-2]*256+modbus.rcbuf[modbus.recount-1];//计算读取的CRC校验位
 	rccrc = modbus.rcbuf[modbus.recount-1]*256+modbus.rcbuf[modbus.recount-2];//计算读取的CRC校验位
 	//等价于下面这条语句
 	//rccrc=modbus.rcbuf[modbus.recount-1]|(((u16)modbus.rcbuf[modbus.recount-2])<<8);//获取接收到的CRC
 	if(crc == rccrc) //CRC检验成功 开始分析包
 	{	
 	   if(modbus.rcbuf[0] == modbus.myadd)  // 检查地址是否时自己的地址
 		 {
 		   switch(modbus.rcbuf[1])   //分析modbus功能码
 			 {
 			   case 0:             break;
 				 case 1:             break;
 				 case 2:             break;
 				 case 3:      Modbus_Func3();      break;//这是读取寄存器的数据
 				 case 4:             break;
 				 case 5:             break;
         case 6:      Modbus_Func6();      break;//这是写入单个寄存器数据
 				 case 7:             break;
 				 case 8:             break;
 				 case 9:             break;
 				 case 16:     Modbus_Func16(); 			break;//写入多个寄存器数据
 			 }
 		 }
 		 else if(modbus.rcbuf[0] == 0) //广播地址不予回应
 		 {
 		    
 		 }	 
 	}	
 	 modbus.recount = 0;//接收计数清零
    modbus.reflag = 0; //接收标志清零
 }

 
 
 /*
 ********************************************************************************
 主机：03
 01  03      00 01     00 01          D5 CA	从地址01开始读读取一个寄存器的数据内容
 ID 功能码  起始地址  读取寄存器的个数
 从机返回：
 01  03       02       00 03          F8 45 返回了两个字节的数据，数据是00 03
 ID  功能码  几个字节  返回的数据内容
 
 ********************************************************************************
 */
 // Modbus 3号功能码函数
 // Modbus 主机读取寄存器值
 void Modbus_Func3(void)
 {
   u16 Regadd,Reglen,crc;
 	u8 i,j;	
//	 printf("进入数据包发送函数\r\n");
 	//得到要读取寄存器的首地址
 	Regadd = modbus.rcbuf[2]*256+modbus.rcbuf[3];//读取的首地址
 	//得到要读取寄存器的数据长度
 	Reglen = modbus.rcbuf[4]*256+modbus.rcbuf[5];//读取的寄存器个数
 	//发送回应数据包
 	i = 0;
 	modbus.sendbuf[i++] = modbus.myadd;      //ID号：发送本机设备地址
 	modbus.sendbuf[i++] = 0x03;              //发送功能码
   modbus.sendbuf[i++] = ((Reglen*2)%256);   //返回字节个数
 	for(j=0;j<Reglen;j++)                    //返回数据
 	{
 		//reg是提前定义好的16位数组（模仿寄存器）
 	  modbus.sendbuf[i++] = Reg[Regadd+j]/256;//高位数据
 		modbus.sendbuf[i++] = Reg[Regadd+j]%256;//低位数据
 	}
 	crc = Modbus_CRC16(modbus.sendbuf,i);    //计算要返回数据的CRC
 	modbus.sendbuf[i++] = crc%256;//校验位低位
 	modbus.sendbuf[i++] = crc/256;//校验位高位
 	//数据包打包完成
 	// 开始返回Modbus数据
 	
 	RS485_TX_ENABLE();//这是开启485发送

	usart_send_array(USART2, modbus.sendbuf, i);
//	printf("发送成功\r\n");
	
 	RS485_RX_ENABLE();//这里是关闭485发送
 }

 
 
  // Modbus 6号功能码函数
 // Modbus 主机写入寄存器值
 void Modbus_Func6(void)  
 {
   u16 Regadd;//地址16位
 	u16 val;//值
 	u16 i,crc,j;
 	i=0;
   Regadd=modbus.rcbuf[2]*256+modbus.rcbuf[3];  //得到要修改的地址 
 	val=modbus.rcbuf[4]*256+modbus.rcbuf[5];     //修改后的值（要写入的数据）
 	Reg[Regadd]=val;  //修改本设备相应的寄存器
 	
 	//以下为回应主机
 	modbus.sendbuf[i++]=modbus.myadd;//本设备地址
  modbus.sendbuf[i++]=0x06;        //功能码 
  modbus.sendbuf[i++]=Regadd/256;//写入的地址
 	modbus.sendbuf[i++]=Regadd%256;
 	modbus.sendbuf[i++]=val/256;//写入的数值
 	modbus.sendbuf[i++]=val%256;
 	crc=Modbus_CRC16(modbus.sendbuf,i);//获取crc校验位
  modbus.sendbuf[6]= crc%256; //crc校验位加入包中
	modbus.sendbuf[7]=	crc/256;
	//数据发送包打包完毕
//	printf("校验位 %x\r\n", modbus.sendbuf[6]);
//	printf("校验位 %x\r\n", modbus.sendbuf[7]);

	RS485_TX_ENABLE();//使能485控制端(启动发送)  
	usart_send_array(USART2, modbus.sendbuf, 9);
	RS485_RX_ENABLE();//失能485控制端（改为接收）
 }

 
 
 //这是往多个寄存器器中写入数据
 //功能码0x10指令即十进制16
 void Modbus_Func16(void)
 {
 		u16 Regadd;//地址16位
 		u16 Reglen;
 		u16 i,crc,j;
 		
 		Regadd=modbus.rcbuf[2]*256+modbus.rcbuf[3];  //要修改内容的起始地址
 		Reglen = modbus.rcbuf[4]*256+modbus.rcbuf[5];//读取的寄存器个数
 		for(i=0;i<Reglen;i++)//往寄存器中写入数据
 		{
 			//接收数组的第七位开始是数据
 			Reg[Regadd+i]=modbus.rcbuf[7+i*2]*256+modbus.rcbuf[8+i*2];//对寄存器一次写入数据
//			printf("修改后的数据 %x", Reg[Regadd+i]);
 		}
 		//写入数据完毕，接下来需要进行打包回复数据了
 		
 		//以下为回应主机内容
 		//内容=接收数组的前6位+两位的校验位
 		modbus.sendbuf[0]=modbus.rcbuf[0];//本设备地址
 		modbus.sendbuf[1]=modbus.rcbuf[1];  //功能码 
 		modbus.sendbuf[2]=modbus.rcbuf[2];//写入的地址
 		modbus.sendbuf[3]=modbus.rcbuf[3];
 		modbus.sendbuf[4]=modbus.rcbuf[4];
 		modbus.sendbuf[5]=modbus.rcbuf[5];
 		crc=Modbus_CRC16(modbus.sendbuf,6);//获取crc校验位
 		modbus.sendbuf[6]= crc%256; //crc校验位加入包中
 		modbus.sendbuf[7]=	crc/256;
 		//数据发送包打包完毕
// 		printf("校验位 %x\r\n", modbus.sendbuf[6]);
//		printf("校验位 %x\r\n", modbus.sendbuf[7]);
		
 		RS485_TX_ENABLE();//使能485控制端(启动发送)  
 		usart_send_array(USART2, modbus.sendbuf, 9);
 		RS485_RX_ENABLE();//失能485控制端（改为接收）
 }

 
 
 
 //CRC计算
unsigned int Modbus_CRC16(unsigned char *data, unsigned int len)
{
    unsigned int i, j, tmp, CRC16;

    CRC16 = 0xFFFF;             //CRC寄存器初始值
    for (i = 0; i < len; i++)
    {
        CRC16 ^= data[i];
        for (j = 0; j < 8; j++)
        {
            tmp = (unsigned int)(CRC16 & 0x0001);
            CRC16 >>= 1;
            if (tmp == 1)
            {
                CRC16 ^= 0xA001;    //异或多项式
            }
        }
    }
/*根据需要对结果进行处理*/
//    data[i++] = (unsigned char) (CRC16 & 0x00FF);
//    data[i++] = (unsigned char) ((CRC16 & 0xFF00)>>8);
    return CRC16;
}

 
 
 
 
 
 
 
 
 
 
 
 
 
 
 