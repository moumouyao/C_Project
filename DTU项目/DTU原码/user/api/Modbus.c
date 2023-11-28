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
								};//reg����ǰ����õļĴ����ͼĴ������ݣ�Ҫ��ȡ�͸�д�Ĳ�������

uint16_t modbus_data[5] = {0};

 //����1�ӻ���ַ������2��ʼ��ַ������3�Ĵ�������
 void Host_Read03_slave(uint8_t slave,uint16_t StartAddr,uint16_t num)
 {
// 	int j;
 	uint16_t crc;//�����CRCУ��λ
 	modbus.slave_add=slave;//�����ȰѴӻ���ַ�洢����������������ݴ���ʱ���õ�
 	modbus.Host_Txbuf[0]=slave;//����Ҫƥ��Ĵӻ���ַ
 	modbus.Host_Txbuf[1]=0x03;//������
 	modbus.Host_Txbuf[2]=StartAddr/256;//��ʼ��ַ��λ
 	modbus.Host_Txbuf[3]=StartAddr%256;//��ʼ��ַ��λ
 	modbus.Host_Txbuf[4]=num/256;//�Ĵ���������λ
 	modbus.Host_Txbuf[5]=num%256;//�Ĵ���������λ
 	crc=Modbus_CRC16(&modbus.Host_Txbuf[0],6); //��ȡCRCУ��λ
 	modbus.Host_Txbuf[6]=crc%256;//�Ĵ���������λ
 	modbus.Host_Txbuf[7]=crc/256;//�Ĵ���������λ
 	//�������ݰ�װ���(��8���ֽ�)
 	//��ʼ��������
 	RS485_TX_ENABLE();//ʹ��485���ƶ�(��������)  

 	usart_send_array(USART2, modbus.Host_Txbuf, 8);
	for(uint8_t i = 0; i < 8; i++){
		printf("%x\t", modbus.Host_Txbuf[i]);
	}
 	RS485_RX_ENABLE();//ʧ��485���ƶˣ���Ϊ���գ�
 }

 
  //�������մӻ�����Ϣ���д�������0x03
 void HOST_ModbusRX()
 {
 	u16 crc,rccrc;//����crc�ͽ��յ���crc
 
   if(modbus.reflag == 0)  //�������δ����򷵻ؿ�
 	{
 	   return;
 	}
 	//�������ݽ���
 	
 	//�������г��������λCRCУ��λ����ȫ�㣩
 	crc = Modbus_CRC16(&modbus.rcbuf[0],modbus.recount-2); //��ȡCRCУ��λ
//	crc = modbus.rcbuf[modbus.recount-2]*256+modbus.rcbuf[modbus.recount-1];//�����ȡ��CRCУ��λ
 	rccrc = modbus.rcbuf[modbus.recount-1]*256+modbus.rcbuf[modbus.recount-2];//�����ȡ��CRCУ��λ
 	
 	if(crc == rccrc) //CRC����ɹ� ��ʼ������
 	{	
 	   if(modbus.rcbuf[0] == modbus.slave_add)  // ����ַ���Ƕ�Ӧ�ӻ���������
 		 {
 			 if(modbus.rcbuf[1]==3)//������ʱ03
 		      Host_Func3();//���Ƕ�ȡ�Ĵ�������Ч����λ���м���
 		 }
 		 
 	}	
 	 modbus.recount = 0;//���ռ�������
    modbus.reflag = 0; //���ձ�־����
 	
 }
 
 
  void Host_Func3(void)
 {
 	int i;
 	int count=(int)modbus.rcbuf[2];//�������ݸ���
 	uint8_t j = 0;
// 	printf("�ӻ����� %d ���Ĵ������ݣ�\r\n",count/2);
 	for(i=0;i<count;i=i+2)
 	{
 		printf("Temp_Hbit= %d Temp_Lbit= %d temp= %d\r\n",
		(int)modbus.rcbuf[3+i],(int)modbus.rcbuf[4+i],(int)modbus.rcbuf[4+i]+((int)modbus.rcbuf[3+i])*256);
		modbus_data[j] = (int)modbus.rcbuf[4+i]+((int)modbus.rcbuf[3+i])*256;
		j++;
 	}
 }


 // Modbus��ʼ������
 void Modbus_Init()
 {
   modbus.myadd = 0x02; //�ӻ��豸��ַΪ2
   modbus.timrun = 0;    //modbus��ʱ��ֹͣ����
 	modbus.slave_add=0x01;//����Ҫƥ��Ĵӻ���ַ�����豸��Ϊ����ʱ��
 }

 
 
 // Modbus�¼�������
 void Modbus_Event()
 {
 	uint16_t crc,rccrc;//crc�ͽ��յ���crc
 	//û���յ����ݰ�
   if(modbus.reflag == 0)  //�������δ����򷵻ؿ�
 	{
 	   return;
 	}
//	printf("���յ����ݰ�     ");
//	printf("%s\r\n", modbus.rcbuf);
 	//�յ����ݰ�(�������)
 	//ͨ������������֡����CRC
 	//����1�������׵�ַ������2��Ҫ����ĳ��ȣ�����CRCУ��λ����ȫ�㣩
 	crc = Modbus_CRC16(&modbus.rcbuf[0],modbus.recount-2); //��ȡCRCУ��λ
//	printf("У��λ %x\r\n", crc);
 	// ��ȡ����֡��CRC
//	crc = modbus.rcbuf[modbus.recount-2]*256+modbus.rcbuf[modbus.recount-1];//�����ȡ��CRCУ��λ
 	rccrc = modbus.rcbuf[modbus.recount-1]*256+modbus.rcbuf[modbus.recount-2];//�����ȡ��CRCУ��λ
 	//�ȼ��������������
 	//rccrc=modbus.rcbuf[modbus.recount-1]|(((u16)modbus.rcbuf[modbus.recount-2])<<8);//��ȡ���յ���CRC
 	if(crc == rccrc) //CRC����ɹ� ��ʼ������
 	{	
 	   if(modbus.rcbuf[0] == modbus.myadd)  // ����ַ�Ƿ�ʱ�Լ��ĵ�ַ
 		 {
 		   switch(modbus.rcbuf[1])   //����modbus������
 			 {
 			   case 0:             break;
 				 case 1:             break;
 				 case 2:             break;
 				 case 3:      Modbus_Func3();      break;//���Ƕ�ȡ�Ĵ���������
 				 case 4:             break;
 				 case 5:             break;
         case 6:      Modbus_Func6();      break;//����д�뵥���Ĵ�������
 				 case 7:             break;
 				 case 8:             break;
 				 case 9:             break;
 				 case 16:     Modbus_Func16(); 			break;//д�����Ĵ�������
 			 }
 		 }
 		 else if(modbus.rcbuf[0] == 0) //�㲥��ַ�����Ӧ
 		 {
 		    
 		 }	 
 	}	
 	 modbus.recount = 0;//���ռ�������
    modbus.reflag = 0; //���ձ�־����
 }

 
 
 /*
 ********************************************************************************
 ������03
 01  03      00 01     00 01          D5 CA	�ӵ�ַ01��ʼ����ȡһ���Ĵ�������������
 ID ������  ��ʼ��ַ  ��ȡ�Ĵ����ĸ���
 �ӻ����أ�
 01  03       02       00 03          F8 45 �����������ֽڵ����ݣ�������00 03
 ID  ������  �����ֽ�  ���ص���������
 
 ********************************************************************************
 */
 // Modbus 3�Ź����뺯��
 // Modbus ������ȡ�Ĵ���ֵ
 void Modbus_Func3(void)
 {
   u16 Regadd,Reglen,crc;
 	u8 i,j;	
//	 printf("�������ݰ����ͺ���\r\n");
 	//�õ�Ҫ��ȡ�Ĵ������׵�ַ
 	Regadd = modbus.rcbuf[2]*256+modbus.rcbuf[3];//��ȡ���׵�ַ
 	//�õ�Ҫ��ȡ�Ĵ��������ݳ���
 	Reglen = modbus.rcbuf[4]*256+modbus.rcbuf[5];//��ȡ�ļĴ�������
 	//���ͻ�Ӧ���ݰ�
 	i = 0;
 	modbus.sendbuf[i++] = modbus.myadd;      //ID�ţ����ͱ����豸��ַ
 	modbus.sendbuf[i++] = 0x03;              //���͹�����
   modbus.sendbuf[i++] = ((Reglen*2)%256);   //�����ֽڸ���
 	for(j=0;j<Reglen;j++)                    //��������
 	{
 		//reg����ǰ����õ�16λ���飨ģ�¼Ĵ�����
 	  modbus.sendbuf[i++] = Reg[Regadd+j]/256;//��λ����
 		modbus.sendbuf[i++] = Reg[Regadd+j]%256;//��λ����
 	}
 	crc = Modbus_CRC16(modbus.sendbuf,i);    //����Ҫ�������ݵ�CRC
 	modbus.sendbuf[i++] = crc%256;//У��λ��λ
 	modbus.sendbuf[i++] = crc/256;//У��λ��λ
 	//���ݰ�������
 	// ��ʼ����Modbus����
 	
 	RS485_TX_ENABLE();//���ǿ���485����

	usart_send_array(USART2, modbus.sendbuf, i);
//	printf("���ͳɹ�\r\n");
	
 	RS485_RX_ENABLE();//�����ǹر�485����
 }

 
 
  // Modbus 6�Ź����뺯��
 // Modbus ����д��Ĵ���ֵ
 void Modbus_Func6(void)  
 {
   u16 Regadd;//��ַ16λ
 	u16 val;//ֵ
 	u16 i,crc,j;
 	i=0;
   Regadd=modbus.rcbuf[2]*256+modbus.rcbuf[3];  //�õ�Ҫ�޸ĵĵ�ַ 
 	val=modbus.rcbuf[4]*256+modbus.rcbuf[5];     //�޸ĺ��ֵ��Ҫд������ݣ�
 	Reg[Regadd]=val;  //�޸ı��豸��Ӧ�ļĴ���
 	
 	//����Ϊ��Ӧ����
 	modbus.sendbuf[i++]=modbus.myadd;//���豸��ַ
  modbus.sendbuf[i++]=0x06;        //������ 
  modbus.sendbuf[i++]=Regadd/256;//д��ĵ�ַ
 	modbus.sendbuf[i++]=Regadd%256;
 	modbus.sendbuf[i++]=val/256;//д�����ֵ
 	modbus.sendbuf[i++]=val%256;
 	crc=Modbus_CRC16(modbus.sendbuf,i);//��ȡcrcУ��λ
  modbus.sendbuf[6]= crc%256; //crcУ��λ�������
	modbus.sendbuf[7]=	crc/256;
	//���ݷ��Ͱ�������
//	printf("У��λ %x\r\n", modbus.sendbuf[6]);
//	printf("У��λ %x\r\n", modbus.sendbuf[7]);

	RS485_TX_ENABLE();//ʹ��485���ƶ�(��������)  
	usart_send_array(USART2, modbus.sendbuf, 9);
	RS485_RX_ENABLE();//ʧ��485���ƶˣ���Ϊ���գ�
 }

 
 
 //����������Ĵ�������д������
 //������0x10ָ�ʮ����16
 void Modbus_Func16(void)
 {
 		u16 Regadd;//��ַ16λ
 		u16 Reglen;
 		u16 i,crc,j;
 		
 		Regadd=modbus.rcbuf[2]*256+modbus.rcbuf[3];  //Ҫ�޸����ݵ���ʼ��ַ
 		Reglen = modbus.rcbuf[4]*256+modbus.rcbuf[5];//��ȡ�ļĴ�������
 		for(i=0;i<Reglen;i++)//���Ĵ�����д������
 		{
 			//��������ĵ���λ��ʼ������
 			Reg[Regadd+i]=modbus.rcbuf[7+i*2]*256+modbus.rcbuf[8+i*2];//�ԼĴ���һ��д������
//			printf("�޸ĺ������ %x", Reg[Regadd+i]);
 		}
 		//д��������ϣ���������Ҫ���д���ظ�������
 		
 		//����Ϊ��Ӧ��������
 		//����=���������ǰ6λ+��λ��У��λ
 		modbus.sendbuf[0]=modbus.rcbuf[0];//���豸��ַ
 		modbus.sendbuf[1]=modbus.rcbuf[1];  //������ 
 		modbus.sendbuf[2]=modbus.rcbuf[2];//д��ĵ�ַ
 		modbus.sendbuf[3]=modbus.rcbuf[3];
 		modbus.sendbuf[4]=modbus.rcbuf[4];
 		modbus.sendbuf[5]=modbus.rcbuf[5];
 		crc=Modbus_CRC16(modbus.sendbuf,6);//��ȡcrcУ��λ
 		modbus.sendbuf[6]= crc%256; //crcУ��λ�������
 		modbus.sendbuf[7]=	crc/256;
 		//���ݷ��Ͱ�������
// 		printf("У��λ %x\r\n", modbus.sendbuf[6]);
//		printf("У��λ %x\r\n", modbus.sendbuf[7]);
		
 		RS485_TX_ENABLE();//ʹ��485���ƶ�(��������)  
 		usart_send_array(USART2, modbus.sendbuf, 9);
 		RS485_RX_ENABLE();//ʧ��485���ƶˣ���Ϊ���գ�
 }

 
 
 
 //CRC����
unsigned int Modbus_CRC16(unsigned char *data, unsigned int len)
{
    unsigned int i, j, tmp, CRC16;

    CRC16 = 0xFFFF;             //CRC�Ĵ�����ʼֵ
    for (i = 0; i < len; i++)
    {
        CRC16 ^= data[i];
        for (j = 0; j < 8; j++)
        {
            tmp = (unsigned int)(CRC16 & 0x0001);
            CRC16 >>= 1;
            if (tmp == 1)
            {
                CRC16 ^= 0xA001;    //������ʽ
            }
        }
    }
/*������Ҫ�Խ�����д���*/
//    data[i++] = (unsigned char) (CRC16 & 0x00FF);
//    data[i++] = (unsigned char) ((CRC16 & 0xFF00)>>8);
    return CRC16;
}

 
 
 
 
 
 
 
 
 
 
 
 
 
 
 