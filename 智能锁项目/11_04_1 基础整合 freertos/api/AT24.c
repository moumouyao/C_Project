#include "AT24.h"

uint8_t AT24_ADDR_W1	= 0XA0;
uint8_t AT24_ADDR_W2	= 0XA2;
uint8_t AT24_ADDR_R1	= 0xA1;
uint8_t AT24_ADDR_R2	= 0xA3;


/**
  * @brief  AT24C04��ʼ��
  * @param  None
  * @retval None
  */
void AT24_init(void){
	
	myi2c_init();
	
}
	
/**
  * @brief  ָ����ַд��һ���ֽ����ݣ�0 ---- 255��
  * @param  uint16_t addr	д�����ݵ�ַ
  * @param	uint8_t data	д���ֽ�
  * @retval д��ɹ�����4
  */
uint8_t AT24_write_byte(uint16_t addr, uint8_t data){
	i2c_start();	//������ʼ�ź�
	myi2c_sendbyte(AT24_ADDR_W1);	//���ʹӻ���ַ
	if(myi2c_recv_ack() == 1){
		i2c_stop();		//����ֹͣλ
		printf("AT24ѰַδӦ��\r\n");
		return 1;
	}
	
	myi2c_sendbyte(addr);	//����Ҫд��ĵ�ַ
	if(myi2c_recv_ack() == 1){
		i2c_stop();		//����ֹͣλ
		printf("AT24�ڲ�ѰַδӦ��\r\n");
		return 2;
	}
	
	myi2c_sendbyte(data);	//����Ҫд�������
	if(myi2c_recv_ack() == 1){
		i2c_stop();		//����ֹͣλ
		printf("AT24д������δӦ��\r\n");
		return 3;
	}
	i2c_stop();		//����ֹͣλ
	printf("AT24д�����ݳɹ�\r\n");
	return  4;
}


/**
  * @brief  ָ����ַ����һ���ֽ����ݣ�0 ---- 255��
  * @param  uint16_t addr	�����ݵ�ַ
  * @retval �ɹ����ض�������
  */

uint8_t AT24_read_byte(uint16_t addr){
	uint8_t read_data = 0;
	i2c_start();	//������ʼ�ź�
	myi2c_sendbyte(AT24_ADDR_W1);	//���ʹӻ���ַ
	if(myi2c_recv_ack() == 1){
		i2c_stop();		//����ֹͣλ
		printf("AT24ѰַδӦ��\r\n");
		return 1;
	}
	
	myi2c_sendbyte(addr);	//����Ҫд��ĵ�ַ
	if(myi2c_recv_ack() == 1){
		i2c_stop();		//����ֹͣλ
		printf("AT24�ڲ�ѰַδӦ��\r\n");
		return 2;
	}
	i2c_stop();		//����ֹͣλ
	i2c_start();	//������ʼ�ź�
	myi2c_sendbyte(AT24_ADDR_R1);	//���ʹӻ���ַ
	if(myi2c_recv_ack() == 1){
		i2c_stop();		//����ֹͣλ
		printf("AT24ѰַδӦ��\r\n");
		return 1;
	}
	read_data = myi2c_recv_byte();
	myi2c_send_ack(1);
	i2c_stop();		//����ֹͣλ
	return read_data;
}


/**
  * @brief  ָ����ַҳд�����ݣ�0 ---- 255��
  * @param  uint16_t addr	д�����ݵ�ַ
  * @param	uint8_t data	д���ֽ��׵�ַ
  * @param	uint8_t num д���ֽڸ���
  * @retval д��ɹ�����4
  */
uint8_t AT24_write_page(uint16_t addr, uint8_t num, uint8_t *data){
	i2c_start();	//������ʼ�ź�
	myi2c_sendbyte(AT24_ADDR_W1);	//���ʹӻ���ַ
	if(myi2c_recv_ack() == 1){
		i2c_stop();		//����ֹͣλ
		printf("AT24ѰַδӦ��\r\n");
		return 1;
	}
	
	myi2c_sendbyte(addr);	//����Ҫд��ĵ�ַ
	if(myi2c_recv_ack() == 1){
		i2c_stop();		//����ֹͣλ
		printf("AT24�ڲ�ѰַδӦ��\r\n");
		return 2;
	}
	while(num--){
		myi2c_sendbyte(*data);	//����Ҫд�������
		if(myi2c_recv_ack() == 1){
			i2c_stop();		//����ֹͣλ
			printf("AT24д������δӦ��\r\n");
			return 3;
		}
		data++;
	}
	
	i2c_stop();		//����ֹͣλ
	printf("AT24д�����ݳɹ�\r\n");
	return  4;
}


/**
  * @brief  ���д
  * @param  uint8_t *pBuffer	д�����ݵ��׵�ַ
  * @param  uint32_t WriteAddr	д���ַ
  * @param  uint16_t NumByteToWrite	���ݳ���
  * @retval None
  */
void AT24_WriteBuffer(uint8_t *pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite){
	uint8_t NumOfPage = 0, NumOfSingle = 0, Addr = 0, count = 0, temp = 0;
	Addr = WriteAddr % 16;	//�жϵ�ַ�Ƿ�Ϊ��ҳ
	count = 16 - Addr;			//��ǰҳʣ���ֽ���
	NumOfPage =  NumByteToWrite / 16;	//��Ҫ����ҳ��
	NumOfSingle = NumByteToWrite % 16;	//����ҳʣ����ֽ���
	
	if (Addr == 0) /*��ҳ��ʼ  */
  {
    if (NumOfPage == 0) /*��д���ݲ���һ��ҳ��ֱ�ӵ���ҳ��̺��� */
    {
      AT24_write_page(WriteAddr, NumByteToWrite, pBuffer);
    }
    else /*��д���ݳ���һҳ*/
    {
      while (NumOfPage--)	//��ҳд
      {
        AT24_write_page(WriteAddr, 16, pBuffer);
        WriteAddr +=  16;
        pBuffer += 16;
      }

      AT24_write_page(WriteAddr, NumOfSingle, pBuffer);	//����ҳ֮��ʣ���
    }
  }
  else /*������ҳ��ʼд  */
  {
    if (NumOfPage == 0) /*��д����һҳ */
    {
      if (NumOfSingle > count) /*����ռ���ڵ�ǰҳ��ʣ�ռ�*/
      {
        temp = NumOfSingle - count;	//��ǰҳд��֮��ʣ����

        AT24_write_page(WriteAddr, count, pBuffer);	//�ڵ�ǰҳд��д��
        WriteAddr +=  count;
        pBuffer += count;

        AT24_write_page(WriteAddr, temp, pBuffer);	//ʣ��д����һҳ
      }
      else
      {
        AT24_write_page(WriteAddr, NumByteToWrite, pBuffer);	//ֱ��д��ǰҳ
      }
    }
    else /*д������������һҳ */
    {
      NumByteToWrite -= count;	//д����ǰҳ��ʣ����
      NumOfPage =  NumByteToWrite / 16;	//Ҫд�����ҳ
      NumOfSingle = NumByteToWrite % 16;	//д����ҳʣ����ֽ�

      AT24_write_page(WriteAddr, count, pBuffer);//�ѵ�ǰҳд��
      WriteAddr +=  count;
      pBuffer += count;

      while (NumOfPage--)	//д��ҳ
      {
        AT24_write_page(WriteAddr, 16, pBuffer);
        WriteAddr +=  16;
        pBuffer += 16;
      }

      if (NumOfSingle != 0)	//дʣ�಻��һҳ���ֽ�
      {
        AT24_write_page(WriteAddr, NumOfSingle, pBuffer);
      }
    }
  }
}

/**
  * @brief  ����
  * @param  None
  * @retval None
  */
uint8_t AT24_readBuffer(uint16_t addr, uint16_t num, uint8_t *recvdata){
	i2c_start();	//������ʼ�ź�
	myi2c_sendbyte(AT24_ADDR_W1);	//���ʹӻ���ַ
		Delay_us(10);
	if(myi2c_recv_ack() == 1){
		i2c_stop();		//����ֹͣλ
		printf("AT24����ѰַδӦ��\r\n");
		return 1;
	}

	myi2c_sendbyte(addr);	//����Ҫд��ĵ�ַ
	if(myi2c_recv_ack() == 1){
		i2c_stop();		//����ֹͣλ
		printf("AT24�ڲ�ѰַδӦ��\r\n");
		return 2;
	}
	i2c_stop();		//����ֹͣλ
	i2c_start();	//������ʼ�ź�
	myi2c_sendbyte(AT24_ADDR_R1);	//���ʹӻ���ַ
	if(myi2c_recv_ack() == 1){
		i2c_stop();		//����ֹͣλ
		printf("AT24����2ѰַδӦ��\r\n");
		return 1;
	}
	while(num--){
		*recvdata = myi2c_recv_byte();
		myi2c_send_ack(0);
		recvdata++;
		Delay_us(5);
	}
	myi2c_send_ack(1);
	i2c_stop();		//����ֹͣλ
	return num;
}









