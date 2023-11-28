#include "AT24.h"

uint8_t AT24_ADDR_W1	= 0XA0;
uint8_t AT24_ADDR_W2	= 0XA2;
uint8_t AT24_ADDR_R1	= 0xA1;
uint8_t AT24_ADDR_R2	= 0xA3;


/**
  * @brief  AT24C04初始化
  * @param  None
  * @retval None
  */
void AT24_init(void){
	
	myi2c_init();
	
}
	
/**
  * @brief  指定地址写入一个字节数据（0 ---- 255）
  * @param  uint16_t addr	写入数据地址
  * @param	uint8_t data	写入字节
  * @retval 写入成功返回4
  */
uint8_t AT24_write_byte(uint16_t addr, uint8_t data){
	i2c_start();	//发送起始信号
	myi2c_sendbyte(AT24_ADDR_W1);	//发送从机地址
	if(myi2c_recv_ack() == 1){
		i2c_stop();		//发送停止位
		printf("AT24寻址未应答\r\n");
		return 1;
	}
	
	myi2c_sendbyte(addr);	//发送要写入的地址
	if(myi2c_recv_ack() == 1){
		i2c_stop();		//发送停止位
		printf("AT24内部寻址未应答\r\n");
		return 2;
	}
	
	myi2c_sendbyte(data);	//发送要写入的数据
	if(myi2c_recv_ack() == 1){
		i2c_stop();		//发送停止位
		printf("AT24写入数据未应答\r\n");
		return 3;
	}
	i2c_stop();		//发送停止位
	printf("AT24写入数据成功\r\n");
	return  4;
}


/**
  * @brief  指定地址读出一个字节数据（0 ---- 255）
  * @param  uint16_t addr	读数据地址
  * @retval 成功返回读出数据
  */

uint8_t AT24_read_byte(uint16_t addr){
	uint8_t read_data = 0;
	i2c_start();	//发送起始信号
	myi2c_sendbyte(AT24_ADDR_W1);	//发送从机地址
	if(myi2c_recv_ack() == 1){
		i2c_stop();		//发送停止位
		printf("AT24寻址未应答\r\n");
		return 1;
	}
	
	myi2c_sendbyte(addr);	//发送要写入的地址
	if(myi2c_recv_ack() == 1){
		i2c_stop();		//发送停止位
		printf("AT24内部寻址未应答\r\n");
		return 2;
	}
	i2c_stop();		//发送停止位
	i2c_start();	//发送起始信号
	myi2c_sendbyte(AT24_ADDR_R1);	//发送从机地址
	if(myi2c_recv_ack() == 1){
		i2c_stop();		//发送停止位
		printf("AT24寻址未应答\r\n");
		return 1;
	}
	read_data = myi2c_recv_byte();
	myi2c_send_ack(1);
	i2c_stop();		//发送停止位
	return read_data;
}


/**
  * @brief  指定地址页写入数据（0 ---- 255）
  * @param  uint16_t addr	写入数据地址
  * @param	uint8_t data	写入字节首地址
  * @param	uint8_t num 写入字节个数
  * @retval 写入成功返回4
  */
uint8_t AT24_write_page(uint16_t addr, uint8_t num, uint8_t *data){
	i2c_start();	//发送起始信号
	myi2c_sendbyte(AT24_ADDR_W1);	//发送从机地址
	if(myi2c_recv_ack() == 1){
		i2c_stop();		//发送停止位
		printf("AT24寻址未应答\r\n");
		return 1;
	}
	
	myi2c_sendbyte(addr);	//发送要写入的地址
	if(myi2c_recv_ack() == 1){
		i2c_stop();		//发送停止位
		printf("AT24内部寻址未应答\r\n");
		return 2;
	}
	while(num--){
		myi2c_sendbyte(*data);	//发送要写入的数据
		if(myi2c_recv_ack() == 1){
			i2c_stop();		//发送停止位
			printf("AT24写入数据未应答\r\n");
			return 3;
		}
		data++;
	}
	
	i2c_stop();		//发送停止位
	printf("AT24写入数据成功\r\n");
	return  4;
}


/**
  * @brief  随机写
  * @param  uint8_t *pBuffer	写入数据的首地址
  * @param  uint32_t WriteAddr	写入地址
  * @param  uint16_t NumByteToWrite	数据长度
  * @retval None
  */
void AT24_WriteBuffer(uint8_t *pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite){
	uint8_t NumOfPage = 0, NumOfSingle = 0, Addr = 0, count = 0, temp = 0;
	Addr = WriteAddr % 16;	//判断地址是否为整页
	count = 16 - Addr;			//当前页剩余字节数
	NumOfPage =  NumByteToWrite / 16;	//需要的整页数
	NumOfSingle = NumByteToWrite % 16;	//除整页剩余的字节数
	
	if (Addr == 0) /*整页开始  */
  {
    if (NumOfPage == 0) /*所写数据不够一整页，直接调用页编程函数 */
    {
      AT24_write_page(WriteAddr, NumByteToWrite, pBuffer);
    }
    else /*所写数据超过一页*/
    {
      while (NumOfPage--)	//整页写
      {
        AT24_write_page(WriteAddr, 16, pBuffer);
        WriteAddr +=  16;
        pBuffer += 16;
      }

      AT24_write_page(WriteAddr, NumOfSingle, pBuffer);	//除整页之外剩余的
    }
  }
  else /*不是整页开始写  */
  {
    if (NumOfPage == 0) /*所写不到一页 */
    {
      if (NumOfSingle > count) /*所需空间大于当前页所剩空间*/
      {
        temp = NumOfSingle - count;	//当前页写完之后剩余量

        AT24_write_page(WriteAddr, count, pBuffer);	//在当前页写，写满
        WriteAddr +=  count;
        pBuffer += count;

        AT24_write_page(WriteAddr, temp, pBuffer);	//剩余写入下一页
      }
      else
      {
        AT24_write_page(WriteAddr, NumByteToWrite, pBuffer);	//直接写当前页
      }
    }
    else /*写入数据量大于一页 */
    {
      NumByteToWrite -= count;	//写满当前页所剩数据
      NumOfPage =  NumByteToWrite / 16;	//要写入的整页
      NumOfSingle = NumByteToWrite % 16;	//写完整页剩余的字节

      AT24_write_page(WriteAddr, count, pBuffer);//把当前页写满
      WriteAddr +=  count;
      pBuffer += count;

      while (NumOfPage--)	//写整页
      {
        AT24_write_page(WriteAddr, 16, pBuffer);
        WriteAddr +=  16;
        pBuffer += 16;
      }

      if (NumOfSingle != 0)	//写剩余不满一页的字节
      {
        AT24_write_page(WriteAddr, NumOfSingle, pBuffer);
      }
    }
  }
}

/**
  * @brief  随便读
  * @param  None
  * @retval None
  */
uint8_t AT24_readBuffer(uint16_t addr, uint16_t num, uint8_t *recvdata){
	i2c_start();	//发送起始信号
	myi2c_sendbyte(AT24_ADDR_W1);	//发送从机地址
		Delay_us(10);
	if(myi2c_recv_ack() == 1){
		i2c_stop();		//发送停止位
		printf("AT24器件寻址未应答\r\n");
		return 1;
	}

	myi2c_sendbyte(addr);	//发送要写入的地址
	if(myi2c_recv_ack() == 1){
		i2c_stop();		//发送停止位
		printf("AT24内部寻址未应答\r\n");
		return 2;
	}
	i2c_stop();		//发送停止位
	i2c_start();	//发送起始信号
	myi2c_sendbyte(AT24_ADDR_R1);	//发送从机地址
	if(myi2c_recv_ack() == 1){
		i2c_stop();		//发送停止位
		printf("AT24器件2寻址未应答\r\n");
		return 1;
	}
	while(num--){
		*recvdata = myi2c_recv_byte();
		myi2c_send_ack(0);
		recvdata++;
		Delay_us(5);
	}
	myi2c_send_ack(1);
	i2c_stop();		//发送停止位
	return num;
}









