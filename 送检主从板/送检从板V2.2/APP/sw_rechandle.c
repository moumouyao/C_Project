#include "sw_rechandle.h"
#include "sw_config.h"
#include "user_crc.h"
#include "i2c.h"
#include "delay.h"
#include "flash.h"
#include "malloc.h"
#include "sw_led.h"
#include "i2c2.h"


static  uint8_t array[3][256];                        //�����256��

/**															   
  ******************************************************************************
  *                              SlaveAck()
  *
  * ����   : �ӻ���������Ӧ��
  *		   
  * ����   : *pot-Ӧ�����ݻ�����
	*          temp-0��1
  *
  *
  * ����ֵ : ��
  ******************************************************************************
  */
void SlaveAckInit (uint8_t *pot,uint8_t temp)
{
  uint32_t crc32;
	uint8_t *p;
	uint16_t temp_sh20 = 0;
	p = pot;//������ʼ��ַ
	pot += 2;            //����2λ�������ݳ���
	
	*pot++ = G_IDHost;   //������ַ
	*pot++ = G_IDSlave0; //�ӻ���ַ0
	*pot++ = 0;          //�ӻ���ַ1����
	
	*pot++ = temp;       //Ӧ����
	*pot++ = 0;          //���ݳ��ȸ��ֽ�	
	*pot++ = 0x9;          //���ݳ��ȵ��ֽ�
	
	//�¶�
//	temp_sh20 = SHT20_Get_Data(SHT20_CMD_MEASURE_TEMP);
//*pot++ = (uint8_t)(temp_sh20 >> 8);   //�¶ȸ߰�λ   
	*pot++ = (uint8_t)temp_sh20; 				//�¶ȵͰ�λ
//	temp_sh20 = 0;
//	//ʪ��
//	temp_sh20 = SHT20_Get_Data(SHT20_CMD_MEASURE_HUMI);
//	*pot++ = (uint8_t)(temp_sh20 >> 8);     //ʪ�ȸ߰�λ   
	*pot++ = (uint8_t)temp_sh20; 						//ʪ�ȵͰ�λ							
//	
//	//��ȡ���ŵ�ƽ U4 U7 U15û������
	Door1 = GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_1);//U3	����������
	Door2 = GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0);//U5	��ʾģ�����
	Door3 = GPIO_ReadInputDataBit(GPIOH,GPIO_Pin_2);//U6	��ʾģ���Դ����
	Door4 = GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_6);//U8	�����عܹ���
	Door5 = GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_4);//U13	 ���ϵͳ����
	Door6 = GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_0);//U14		����������
	Door7 = GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_2);//U9 U10220V������
	*pot++ = Door1;          //����Ӧ����
	*pot++ = Door2;          
	*pot++ = Door3;          
	*pot++ = Door4; 
	*pot++ = Door5;          
	*pot++ = Door6; 
	*pot++ = Door7;	//220V��������ϼ�⣬�Ͽ�Ϊ0������Ϊ1
	
//	
	crc32 = UserHWCRC32(p+2,(pot-p-2));
	*((uint32_t *)pot) = crc32;
//	printf("faultcrc:%d, %d, %d, %d\r\n", *pot, *(pot+1), *(pot+2), *(pot+3));
	pot += 4;
	*((uint16_t *)p) = 0x0013; 
//	printf("���ϼ�����ݳ��ȣ�%d\r\n", *((uint16_t *)p));
//	*((uint16_t *)p) = (uint16_t)(pot-p-2);//���д�����Ӳ������
}


/**
  ******************************************************************************
  *                          	SlaveAckOK ()
  *
  * ����   : ���ػ�������
  *		   
  * ����   : ��
  *
  *
  * ����ֵ : ��
  ******************************************************************************
  */
uint32_t crc32 = 0;
void Open_DetectionAck(uint8_t num)
{	
	xSemaphoreTake(RS485TranSem,portMAX_DELAY);//��ȡ�ź���		
	SLAVE485Out;
	delay_sys_us(200);
//	vTaskDelay(1);
	
	uint8_t *p = NULL;
	uint8_t ackdata[3*32*4+16] = {0};
	uint16_t q = 1;
	p = ackdata;//������ʼ��ַ
	p += 2;            //����2λ�������ݳ���
	
	*p++ = G_IDHost;   //������ַ
	*p++ = G_IDSlave0; //�ӻ���ַ0
	*p++ = 0;          //�ӻ���ַ1����
	
	*p++ = 7;       //Ӧ����
//	*p++ = 0;
//	*p++ = 6;
//	*p++ = 7;
//	*p++ = 8;
if(num == 1){
	*p++ = (2*32 * 4)>>8;	//���ݳ���
	*p++ = (2*32 * 4)&0xff;	//���ݳ���
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
	*p++ = (32 * 4)>>8;	//���ݳ���
	*p++ = (32 * 4)&0xff;	//���ݳ���
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

//	printf("���ݳ���%d", *(uint16_t *)p);
	
//	
	crc32 = UserHWCRC32(ackdata+2,(p - ackdata-2));
//	printf("У��λ%d\r\n", crc32);
	*((uint32_t *)p) = crc32;
//	printf("ledcrc:%d, %d, %d, %d\r\n", *p, *(p+1), *(p+2), *(p+3));
//	printf("ledcrc2:%x\r\n", crc32);//0xD5628D88
	p += 4;
//	*((uint16_t *)p) = 389; 
	*((uint16_t *)ackdata) = (uint16_t)(p - ackdata-2);
//	*((uint16_t *)ackdata) = 394;
//	printf("���ݳ��� %d\r\n", (uint16_t)(p - ackdata-2));
	

	
	 
	RS485TranConfig(&ackdata[2],*((uint16_t *)ackdata));	//����UART3׼����������
	USART_ITConfig(USART3, USART_IT_TXE, ENABLE);



	
	xSemaphoreGive(RS485TranSem);	//�ͷ��ź���
//	delay_sys_us(800);
//	vTaskDelay(1);
//	SLAVE485In;
}




/**
  ******************************************************************************
  *                          RecDataHandle ()
  *
  * ����   : ������յ������ݺ���
  *		   
  * ����   : p:���յ�������ָ��
  *
  *
  * ����ֵ : ��
  ******************************************************************************
  */
void RecDataHandle (uint8_t * p)//�������ݴ���
{
	uint8_t test=0;
	uint8_t cmd = p[4];
	BaseType_t xReturn = pdTRUE;
#if Debug_printf
					printf("cmd=%d", cmd);//%d,%d,%d,%d,%d\r\n",p[3],cmd, p[5], p[6], p[7], p[8]);	
#endif
	switch(cmd)
		{
			case 1://��������֡��RGB����
			SlaveAckOK();			//--Ӧ��
			test=0;
			xReturn=xQueueSend(FrameStoreMsgQeue,(void *)&p,0);//���͵�����
			if(xReturn!=pdTRUE)
			{
				vTaskDelay(4);
#if Debug_printf
				printf("\n\r ������Ϣ����ʧ�ܣ�FrameStoreMsgQeue");			
#endif
				if(test++ == 5)
				NVIC_SystemReset();		
				xReturn=xQueueSend(FrameStoreMsgQeue,(void *)&p,0);//���͵�����				
			}	
			break;
		case 2://����֡�š�����	
			SlaveAckOK();			//--Ӧ��
			xReturn=xQueueSend(FrameLoadMbox,(void *)&p,0);//���͵�����
			test = 0;
			if(xReturn!=pdTRUE)
			{
				vTaskDelay(4);
#if Debug_printf
				printf("\n\r ������Ϣ����ʧ�ܣ�FrameStoreMsgQeue");			
#endif
				if(test++ == 5)
				NVIC_SystemReset();		
				xReturn=xQueueSend(FrameLoadMbox,(void *)&p,0);//���͵�����				
			}		
			break;
			case 3://��������ģ��������
				if(*(uint16_t *)(p+2+3) == sizeof(ScreenBoxDataStr))//���ݳ���Ϊ10
				{
					SlaveAckOK(); //Ӧ��
					SBoxSetStore(p+7);//��������ָ��
					if(G_FrameFlag == 1)//�������������
					{		
						SPI_FlashRead(&G_FrameNum,FLASH_FRAME_TAB_ADDR,1);//��ȡ�ܵ�֡��
						GetLoadNodeTab(LoadNodeTab,G_FrameNum);           //����֡����ͷ��
					}
				SetShowArea();      //���ôӻ�������ʾ��Χ	
				}
			myfree(SRAMIN,(void *)p);	
			break;
		case 5://�����������Ƿ����
			if(G_LoadDataFlag == 1)//������ݼ��������Ӧ��
				SlaveAckOK();			//--Ӧ��
				myfree(SRAMIN,(void *)p);	
			break;
			
		case 6://���ع�����Ϣ
			SlaveAckFault();
			Open_Detection();
			
		break;
		
		case 7://������Ϣ
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
  * ����   : ���������������Ӧ��
  *		   
  * ����   : ��
  *
  *
  * ����ֵ : ��
  ******************************************************************************
  */
void SlaveAckOK (void)
{	
	uint16_t num;
	xSemaphoreTake(RS485TranSem,portMAX_DELAY);//��ȡ�ź���		
	SLAVE485Out;
	delay_sys_us(200);
//	vTaskDelay(1);
	num = *(uint16_t *)SlaveAckOKBuf;        //Ҫ���͵����ݸ���
	 
	RS485TranConfig(&SlaveAckOKBuf[2],num);	//����UART3׼����������
	USART_ITConfig(USART3, USART_IT_TXE, ENABLE); 
	
	xSemaphoreGive(RS485TranSem);	//�ͷ��ź���
//	delay_sys_us(800);
//	vTaskDelay(1);
//	SLAVE485In;
}



/**
  ******************************************************************************
  *                          	SlaveAckOK ()
  *
  * ����   : ���������������Ӧ��
  *		   
  * ����   : ��
  *
  *
  * ����ֵ : ��
  ******************************************************************************
  */
void SlaveAckFault (void)
{	
	uint16_t num;
	xSemaphoreTake(RS485TranSem,portMAX_DELAY);//��ȡ�ź���		
	SLAVE485Out;
	delay_sys_us(200);
//	vTaskDelay(1);
	num = *(uint16_t *)SlaveAckOKBuf;        //Ҫ���͵����ݸ���
	 
	RS485TranConfig(&SlaveAckFaultBuf[2],num);	//����UART3׼����������
	USART_ITConfig(USART3, USART_IT_TXE, ENABLE); 
	
	xSemaphoreGive(RS485TranSem);	//�ͷ��ź���
//	delay_sys_us(800);
//	vTaskDelay(1);
//	SLAVE485In;
}







/**															   
  ******************************************************************************
  *                              ScreenBoxSet()
  *
  * ����   : �ӻ����塢���塢ģ���������洢
  *		   
  * ����   : ���塢���塢ģ��������ָ��
  *
  *
  * ����ֵ : ��
  ******************************************************************************
  */

void SBoxSetStore (uint8_t *p)
{

	
#if Debug_printf
		ScreenBoxDataStr *pot;
		pot = (ScreenBoxDataStr *)p;

		printf("��ɫ���ࣺ%d\n\r",pot->Yanse);
		printf("����һ�е���������%d\n\r",pot->SHang);
		printf("����һ�е������������%d\n\r",pot->SLie);
		printf("����һ�е������������%d\n\r",pot->BHang);
		printf("����һ�е������������%d\n\r",pot->BLie);
		printf("ģ�����-1:8X4;2:8X8;3:8X16��%d\n\r",pot->MozuType);
#endif
	
  uint8_t buf[15];
	uint8_t i=0;
	I2C_ReadS_24C(EEROM_SBOX_SET_FLAG_ADDR,&ScreenBoxSetFlag,1);
	if(ScreenBoxSetFlag == 1)
	{
	  I2C_ReadS_24C(EEROM_SBOX_SET_START_ADDR,buf,sizeof(ScreenBoxDataStr));//��ȡEEROM�в�������
    for(i=0;i<sizeof(ScreenBoxDataStr);i++)
	  {
      if(buf[i] != p[i])
			  break;
    }
  }
	if(i != sizeof(ScreenBoxDataStr))  //���ݲ���ͬ����δ����
	{
    ScreenBoxSetFlag = 0;
		I2C_ByteWrite_24C(EEROM_SBOX_SET_FLAG_ADDR,ScreenBoxSetFlag);//�洢�����־λ
#if Debug_printf
		printf("\n\r �洢SBOX_SET_FLAG_CLR \r\n ");
#endif
		I2C_WriteS_24C(EEROM_SBOX_SET_START_ADDR,p,sizeof(ScreenBoxDataStr));//�洢����
		ScreenBoxSetRefresh(p);//���²���
		ScreenBoxSetFlag = 1;
#if Debug_printf
		printf("\n\r �洢SBOX_SET_FLAG_SET \r\n ");
#endif
		I2C_ByteWrite_24C(EEROM_SBOX_SET_FLAG_ADDR,ScreenBoxSetFlag);//�洢��1��־λ
  }	
}
/**															   
  ******************************************************************************
  *                              ScreenBoxSet()
  *
  * ����   : �ӻ����塢�����������
  *		   
  * ����   : ��������������/����֡����ָ��
  *
  *
  * ����ֵ : ��
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
	
  G_MoZuType = pot->MozuType;   //ģ�����	
}

/**															   
  ******************************************************************************
  *                              FrameStoreHandle()
  *
  * ����   : ���ݴ洢����
  *		   
  * ����   : p:���������Ĳ���ָ��
  *
  *
  * ����ֵ : ��
  ******************************************************************************
  */
void FrameStoreHandle (uint8_t *p) 
{
#if Debug_printf
		printf("\n\r FLASH�洢����");
#endif
	uint16_t data_length;             //���ݳ���
	FrameFileHead head;  
	static FrameFileHead last_head;
	static uint32_t flash_addr;       //FLASH�洢��ַ��¼
	static uint16_t flash_sector;	    //FLASH�洢������¼
	
	ReadFrameFileHead((p+7),&head);   //��ȡ�ļ�ͷ
	data_length = *(uint16_t *)(p+5)-4; //��ȡ���ݳ���-�������ļ�ͷ
	if(head.frame_no == 0 && head.data_no == 0) //��0֡����,��0����
	{
		G_FrameFlag = 0;              //�洢�������ݱ�־λ-����		
		I2C_ByteWrite_24C(EEROM_FRAME_FLAG_ADDR,0);//�洢��1��־λ//FLASH�д�������֡���ݱ�־λ��ַ
		flash_addr = FLASH_FRAME_FILE_START_ADDR;     //��ʼ���洢��ַ//���ȫ��֡�ļ�����ʼ��ַ	
		flash_sector = FLASH_FRAME_FILE_START_SECTOR; //��ʼ������
		SPI_FlashEraseSector(flash_sector++);         //��������
		SPI_FlashEraseSector(0);                      //��������0
		SPI_FlashWriteNoCheck(&head.frame_no,FLASH_FRAME_TAB_ADDR+5*head.frame_no+1,1);//�洢֡�� �ܹ���֡����ʼ��ַ����¼��֡����������֡�ź����ڵ�ַ	
		SPI_FlashWriteNoCheck((uint8_t *)&flash_addr,FLASH_FRAME_TAB_ADDR+5*head.frame_no+2,4);//�洢��ַ
  }
	else
	{
		if(head.frame_no == last_head.frame_no) 
		{
			if(head.data_no == last_head.data_no) //�����ظ�
			{
#if Debug_printf
			  printf("\n\r ���ݷ����ظ��������д洢");
#endif
				return;        
      }
			else if(head.data_no%2 == 0) //�������Ѵ���
			{
        flash_addr = flash_sector*4096;      //���ȫ��֡�ļ�����ʼ��ַ	
				SPI_FlashEraseSector(flash_sector++);//��������   �Ȳ�����Ӽ�
      }		
    }	
		else  //��֡����
		{
			flash_addr = flash_sector*4096;      //
      SPI_FlashEraseSector(flash_sector++);//��������
			SPI_FlashWriteNoCheck(&head.frame_no,FLASH_FRAME_TAB_ADDR+5*head.frame_no+1,1);//�洢֡��
		  SPI_FlashWriteNoCheck((uint8_t *)&flash_addr,FLASH_FRAME_TAB_ADDR+5*head.frame_no+2,4);//�洢��ַ
    }
  }
	SPI_FlashWriteNoCheck(p+11,flash_addr,data_length);//��ʼ�洢����
	flash_addr += data_length;
	
	last_head.data_no = head.data_no; 
	last_head.data_num = head.data_num;
	last_head.frame_no = head.frame_no; 
	last_head.frame_num = head.frame_num;
	//���һ��������
	if(head.frame_no == (head.frame_num-1) && head.data_no == (head.data_num-1)) 
	{
    SPI_FlashWriteNoCheck(&head.frame_num,FLASH_FRAME_TAB_ADDR,1);//�洢��֡��
		G_FrameNum = head.frame_num;
		G_NewFramesFlag = 1;          //����֡���ݱ�־λ
		G_FrameFlag = 1;              //�洢�������ݱ�־λ-��1
		I2C_ByteWrite_24C(EEROM_FRAME_FLAG_ADDR,1);   //EEROM�б�־λ��1-��ʾ���ݴ洢����  
		if(ScreenBoxSetFlag == 1)//���������������
		  GetLoadNodeTab(LoadNodeTab,G_FrameNum);       //����֡����ͷ��
  }	
}

/**															   
  ******************************************************************************
  *                              FrameStoreHandle()
  *
  * ����   : ��ȡ֡���ļ�ͷ
  *		   
  * ����   : p:�ļ�ͷ��ַ
  *          head:Ҫ�洢���ļ�ͷ�ṹ��
  *
  *
  * ����ֵ : ��
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
  * ����   : ��������Ҫ��ʾ������֡ͷ��֡��+FLASH��ַ+����
  *		   
  * ����   : tab:֡ͷ��ָ��
  *          num:�ܵ�֡��
  *
  *
  * ����ֵ : ��
  ******************************************************************************
  */
void GetLoadNodeTab(LoadDataNode *tab,uint8_t num)           //����֡����ͷ��
{
  uint8_t i;
	uint16_t step = 0;
	for(i=0;i<num;i++)
	{
    tab[i].frame_no = i;//֡��
    SPI_FlashRead((uint8_t *)&tab[i].addr,FLASH_FRAME_TAB_ADDR+5*i+2,4);//��ȡ��ַ	
    SPI_FlashRead(&tab[i].mod,tab[i].addr+2,1);//��ʾ��ʽ
    switch(tab[i].mod/10)  //����
	  {
      case 1://������ʾ
			  step = 0;
		    tab[i].in_step = 0;	
			  break;
		  case 2://����
			  step = SCREEN_HANG-1;
		    tab[i].in_step = SCREEN_HANG-1;
			  break;
		  case 3://����
			  step = SCREEN_HANG-1;
		    tab[i].in_step = SCREEN_HANG-1;
			  break;
		  case 4://����
			  step = SCREEN_LIE-1;
		    tab[i].in_step = SCREEN_LIE-1;
			  break;
		  case 5://����
			  step = SCREEN_LIE-1;
		    tab[i].in_step = SCREEN_LIE-1;
			  break;
		  case 6://���Ҷ�� 
			  step = 7;
		    tab[i].in_step = 7;
			  break;
		  case 7://����Ҷ��
			  step = 7;
		    tab[i].in_step = 7;
			  break;
		  case 8://��˸
			  step = 0;
		    tab[i].in_step = 0;
			  break;
		  default:
				step = 0;
			  tab[i].in_step = 0;
			  break;
    }
	  switch(tab[i].mod%10)  //�Ƴ�
	  {
      case 1://������ʾ
			  step += 1; 
		    tab[i].out_step = 0;
			  break;
		  case 2://����
			  step += SCREEN_HANG;
		    tab[i].out_step = SCREEN_HANG-1;
			  break;
		  case 3://����
			  step += SCREEN_HANG;
		    tab[i].out_step = SCREEN_HANG-1;
			  break;
		  case 4://����
			  step += SCREEN_LIE;
		    tab[i].out_step = SCREEN_LIE-1;
			  break;
		  case 5://����
			  step += SCREEN_LIE;
		    tab[i].out_step = SCREEN_LIE-1;
			  break;
		  case 6://���Ҷ�� 
			  step += 8;
		    tab[i].out_step = 7;
			  break;
		  case 7://����Ҷ��
			  step += 8;
		    tab[i].out_step = 7;
			  break;
		  case 8://��˸
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
	/* ��ȡ�Ѵ洢�������ݱ�־λ  */
//  I2C_Standby_24C();
  I2C_ReadS_24C(EEROM_FRAME_FLAG_ADDR,&G_FrameFlag,1);
  if(G_FrameFlag == 1)	 //��������ˣ����ȡУ������
  {
    I2C_ReadS_24C(EEROM_FRAME_CRC32_ADDR,(uint8_t *)&G_FrameCRC32,4);
#if Debug_printf
    printf("\n\r �������洢���� \r\n ");
		printf("У������0x%8x",G_FrameCRC32);
#endif 
	}
	else
	{
#if Debug_printf
    printf("\n\r ���ݴ洢������ \r\n ");
#endif
    G_FrameCRC32 = 0;
  }
  I2C_ReadS_24C(EEROM_SBOX_SET_FLAG_ADDR,&ScreenBoxSetFlag,1);
  if(ScreenBoxSetFlag == 1)
	{
#if Debug_printf
    printf("\n\r �������洢����������� \r\n ");
#endif
		I2C_ReadS_24C(EEROM_SBOX_SET_START_ADDR,buf,sizeof(ScreenBoxDataStr));//��ȡ����
		ScreenBoxSetRefresh(buf);//���²���
		SetShowArea();           //���ôӻ�������ʾ��Χ
  }		
}


/**															   
  ******************************************************************************
  *                              LoadDataHandle()
  *
  * ����   : ��Ҫ��ʾ�����ݼ��ص���������
  *		   
  * ����   : p:Ҫд�뻺������ָ��
  *          p_node:Ҫ���صĽ��ָ��
  *
  *
  * ����ֵ : ��
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
     case 1://������ʾ
			step = 1;		 
      if(p_node->step < step)
        LoadDotaNowIn(p,LoadNodeTab[p_node->frame_no].addr,p_node->step);
			else
				out_flg = 1;
		  if(G_FrameFlag == 0)//����RGB���ݷ��͹���
        return;			
			break;
		case 2://����
			step = 2*SCREEN_HANG - (ShowAreaStru.HByteEnd<<3);//Ҫ��ʾ����ȫ�����뱾������Ҫ����
		  temp = SCREEN_HANG - (ShowAreaStru.HByteEnd<<3);  //���Ƶ��ﱾ������Ҫ����
		  if(flg == 1)//ǰ����֡������ʾ
			{
        p_node->step += SCREEN_HANG;
				p_node->frame_no -= 1;
      }
		  if(p_node->step < temp)  //û�е��ﱾ����
        ClrDisplayBuf();       //���㻺����
      else if(p_node->step < step)//��ʾ�������ڽ���
			{
				if((p_node->step == SCREEN_HANG-1) && (p_node->frame_no < G_FrameNum-1))//�������һ֡����
				{
				  if(LoadNodeTab[p_node->frame_no+1].mod == LoadNodeTab[p_node->frame_no].mod)//��Ҫ������ʾ
						flg = 1;//������ʾ��־λ
					else
						flg = 0;
				}
				LoadDotaLeftIn(p,LoadNodeTab[p_node->frame_no].addr,p_node->step-temp);
				if(p_node->step == step -1)//�����������
					flg = 0;
			}
      else
        out_flg = 1;
		  if(G_FrameFlag == 0){//����RGB���ݷ��͹���
				 ClrDisplayBuf();
        return;
	 	}			
			break;
		case 3://����
			step = SCREEN_HANG + (ShowAreaStru.HByteStart<<3);//ȫ�����ݴﵽ���������貽��
		  temp = ShowAreaStru.HByteStart<<3;//���Ƶ��ﱾ������Ҫ����
		  if(flg == 1)//ǰ����֡������ʾ
			{
        p_node->step += SCREEN_HANG;
				p_node->frame_no -= 1;
      }		  
		  if(p_node->step < temp)  //û�е��ﱾ����
				ClrDisplayBuf();       //���㻺����
      else if(p_node->step < step)//��ʾ�������ڽ���
			{
        if((p_node->step == SCREEN_HANG-1) && (p_node->frame_no < G_FrameNum-1))//�������һ֡����
				{
				  if(LoadNodeTab[p_node->frame_no+1].mod == LoadNodeTab[p_node->frame_no].mod)//��Ҫ������ʾ
						flg = 1;//������ʾ��־λ
					else
						flg = 0;
				}
				LoadDotaRightIn(p,LoadNodeTab[p_node->frame_no].addr,p_node->step-temp);
				if(p_node->step == step -1)//�����������
					flg = 0;
			}
      else
        out_flg = 1;
		  if(G_FrameFlag == 0)//����RGB���ݷ��͹���
        return;	
			break;
		case 4://����
			step = SCREEN_LIE*2 - ShowAreaStru.LieEnd;//ȫ�����ݴﵽ���������貽��
		  temp = SCREEN_LIE - ShowAreaStru.LieEnd;             //���Ƶ��ﱾ������Ҫ����
		  if(flg == 1)//ǰ����֡������ʾ
			{
        p_node->step += SCREEN_LIE;
				p_node->frame_no -= 1;
      }
		  if(p_node->step < temp)  //û�е��ﱾ����
				ClrDisplayBuf();       //���㻺����
      else if(p_node->step < step)//��ʾ�������ڽ���
			{
        if((p_node->step == SCREEN_LIE-1) && (p_node->frame_no < G_FrameNum-1))//�������һ֡����
				{
				  if(LoadNodeTab[p_node->frame_no+1].mod == LoadNodeTab[p_node->frame_no].mod)//��Ҫ������ʾ
						flg = 1;//������ʾ��־λ
					else
						flg = 0;
				}
				LoadDotaUpIn(p,LoadNodeTab[p_node->frame_no].addr,p_node->step-temp);
				if(p_node->step == step -1)//�����������
					flg = 0;
			}
      else
        out_flg = 1;
		  if(G_FrameFlag == 0)//����RGB���ݷ��͹���
        return;	
			break;
		case 5://����
			step = SCREEN_LIE + ShowAreaStru.LieStart;//ȫ�����ݴﵽ���������貽��
		  temp = ShowAreaStru.LieStart;             //���Ƶ��ﱾ������Ҫ����
		  if(flg == 1)//ǰ����֡������ʾ
			{
        p_node->step += SCREEN_LIE;
				p_node->frame_no -= 1;
      }
		  if(p_node->step < temp)  //û�е��ﱾ����
				ClrDisplayBuf();       //���㻺����			
      else if(p_node->step < step)//��ʾ�������ڽ���
			{
        if((p_node->step == SCREEN_LIE-1) && (p_node->frame_no < G_FrameNum-1))//�������һ֡����
				{
				  if(LoadNodeTab[p_node->frame_no+1].mod == LoadNodeTab[p_node->frame_no].mod)//��Ҫ������ʾ
						flg = 1;//������ʾ��־λ
					else
						flg = 0;
				}
			 LoadDotaDownIn(p,LoadNodeTab[p_node->frame_no].addr,p_node->step-temp);
			 if(p_node->step == step -1)//�����������
					flg = 0;
			}
      else
        out_flg = 1;
		  if(G_FrameFlag == 0)//����RGB���ݷ��͹���
        return;	
			break;
		case 6://���Ҷ�� 
			step = 8;
      if(p_node->step < step)
        LoadDotaHengIn(p,LoadNodeTab[p_node->frame_no].addr,p_node->step);	
      else
        out_flg = 1;
		  if(G_FrameFlag == 0)//����RGB���ݷ��͹���
        return;				
			break;
		case 7://����Ҷ��
			step = 8;
      if(p_node->step < step)
        LoadDotaShuIn(p,LoadNodeTab[p_node->frame_no].addr,p_node->step);
      else
        out_flg = 1;
		  if(G_FrameFlag == 0)//����RGB���ݷ��͹���
        return;	
			break;
		case 8://��˸
			step = 1;
      if(p_node->step < step)
        LoadDotaNowIn(p,LoadNodeTab[p_node->frame_no].addr,p_node->step);
			else
				out_flg = 1;
		  if(G_FrameFlag == 0)  //����RGB���ݷ��͹���
        return;	
			break;
		default:break;
  }
	if(out_flg == 1)
	{
		switch(LoadNodeTab[p_node->frame_no].mod%10)  //�Ƴ�
		{
			case 1://������ʾ 
				ClrDisplayBuf();    //����
				break;
			case 2://����
				LoadDotaLeftOut(p,LoadNodeTab[p_node->frame_no].addr,p_node->step-step);
				break;
			case 3://����
				LoadDotaRightOut(p,LoadNodeTab[p_node->frame_no].addr,p_node->step-step);
				break;
			case 4://����
				LoadDotaUpOut(p,LoadNodeTab[p_node->frame_no].addr,p_node->step-step);
				break;
			case 5://����
				LoadDotaDownOut(p,LoadNodeTab[p_node->frame_no].addr,p_node->step-step);
				break;
			case 6://���Ҷ�� 
				LoadDotaHengOut(p,LoadNodeTab[p_node->frame_no].addr,p_node->step-step);
				break;
			case 7://����Ҷ��
				LoadDotaShuOut(p,LoadNodeTab[p_node->frame_no].addr,p_node->step-step);
				break;
			case 8://��˸
				ClrDisplayBuf();  //����
				break;
			default:break;
		}
  }	
}
/**															   
  ******************************************************************************
  *                              LoadDotaNowIn()
  *
  * ����   : ������Ҫ��ʾ������ȫ���͵���������
  *		   
  * ����   : buf:��������ַ
  *          addr:Ҫ����֡���ݵĵ�ַ
  *          step:Ҫ�������ݵĲ���
  *
  *
  * ����ֵ : ��
  ******************************************************************************
  */
void LoadDotaNowIn (uint8_t buf[][DIS_BUF_LEN],uint32_t addr,uint16_t step)
{
  uint8_t i;
  uint8_t j;
	for(i=0;i<SCREEN_YANSE;i++)
	{
    for(j=0;j<BOX_LIE;j++)//��ȡ�������ÿ�е�����
		  SPI_FlashRead(buf[i]+j*BOX_HBYTE,
		    addr+4+i*SCREEN_BYTE_NUM+(j+ShowAreaStru.LieStart)*SCREEN_HBYTE+
		    ShowAreaStru.HByteStart,BOX_HBYTE);
		if(G_FrameFlag == 0)//����RGB���ݷ��͹���
       return;	
	}
}

/**															   
  ******************************************************************************
  *                            LoadDotaLeftIn()
  *
  * ����   : ������Ҫ��ʾ�����������𲽴��뻺����0-127(���ڿ��Ϊ128�ĵ���)
  *		   
  * ����   : buf:��������ַ
  *          addr:Ҫ���ص����ݵ�ַ
  *          step:Ҫ�������ݵĲ���
  *
  *
  * ����ֵ : ��
  ******************************************************************************
  */
void LoadDotaLeftIn (uint8_t buf[][DIS_BUF_LEN],uint32_t addr,uint16_t step)
{
  uint8_t i;
	int16_t k;
	uint16_t j;
	uint16_t start_addr = 0;
  uint16_t byte_cnt;  //ÿ����Ҫ�����ֽ���	
	uint8_t bit_cnt;   //��Ҫ����λ��

	typedef union 
	{
		uint16_t data16;
		uint8_t  data8[2];
	} dataUN;
  dataUN dataunion;
	
	byte_cnt = step>>3;//����ӿ������ֽ���
	if(byte_cnt <= BOX_HBYTE)
		start_addr = 0;
	else
	{
    byte_cnt = BOX_HBYTE;
 	  start_addr = (step>>3) - BOX_HBYTE;
  }
	bit_cnt = step%8;
  if(bit_cnt == 0)//���ֽ�
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
	if(G_FrameFlag == 0)//����RGB���ݷ��͹���
    return;	
	for(i=0;i<SCREEN_YANSE;i++)
	{
		for(j=0;j<BOX_LIE;j++)
		{
			for(k=0;k<BOX_HBYTE-1;k++)  //���ֽ�(���һ�ֽڵ�������)
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
  * ����   : ������Ҫ��ʾ�������������Ƴ�������
  *		   
  * ����   : buf:��������ַ
  *          step:Ҫ�������ݵĲ���
  *
  *
  * ����ֵ : ��
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

	if(G_FrameFlag == 0)//����RGB���ݷ��͹���
    return;	
	if(step >= BOX_HANG-1)//ȫ���Ƴ�
	{
    ClrDisplayBuf();  //����
  }
	else
	{
	  for(i=0;i<BOX_YANSE;i++)
	  {
		  for(j=0;j<BOX_LIE;j++)
		  {
			  for(k=0;k<BOX_HBYTE-1;k++)  //���ֽ�(���һ�ֽڵ�������)
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
  * ����   : ������Ҫ��ʾ�����������ƴ��뻺����
  *		   
  * ����   : buf:��������ַ
  *          addr:Ҫ���ص����ݵ�ַ
  *          step:Ҫ�������ݵĲ���
  *
  *
  * ����ֵ : ��
  ******************************************************************************
  */
void LoadDotaRightIn (uint8_t buf[][DIS_BUF_LEN],uint32_t addr,uint16_t step)
{
  uint8_t i;
	int16_t k;
	uint16_t j;	
	uint16_t start_addr; //Ҫ��ʾ���ַ���ʼλ��
  uint16_t byte_cnt;   //ÿ����Ҫ�����ֽ���	
	uint8_t bit_cnt;     //��Ҫ����λ��
	
	typedef union 
	{
		uint16_t data16;
		uint8_t  data8[2];
	} dataUN;
  dataUN dataunion;
	
  //�ƶ�λ�õ��ﱾ����  ���������ݺ�Ҫ��ʾ�����Ƿ����
	byte_cnt = step>>3;	
	start_addr = SCREEN_HBYTE - byte_cnt-1;
	byte_cnt = byte_cnt>BOX_HBYTE? BOX_HBYTE:byte_cnt;	
	bit_cnt = step%8;

  if(bit_cnt == 0)
	{
		for(i=0;i<SCREEN_YANSE;i++)//��ȡ��ʼ������
		{
			for(j=0;j<BOX_LIE;j++)
			 SPI_FlashRead(&array[i][j],
			 addr+4+i*SCREEN_BYTE_NUM+(j+ShowAreaStru.LieStart)*SCREEN_HBYTE+start_addr,1);
	  }
	}
  if(G_FrameFlag == 0)//����RGB���ݷ��͹���
    return;
	for(i=0;i<SCREEN_YANSE;i++)
	{
		for(j=0;j<BOX_LIE;j++)
		{
			for(k=BOX_HBYTE-1;k>0;k--)  //���ֽ�(���һ�ֽڵ�������)
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
  * ����   : ������Ҫ��ʾ�������������Ƴ�������
  *		   
  * ����   :  buf:��������ַ
  *          step:Ҫ�������ݵĲ���
  *
  *
  * ����ֵ : ��
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

	if(G_FrameFlag == 0)//����RGB���ݷ��͹���
    return;
	if(step >= BOX_HANG-1)//ȫ���Ƴ�
	{
    ClrDisplayBuf();  //����
  }
	else
	{
	  for(i=0;i<BOX_YANSE;i++)
	  {
		  for(j=0;j<BOX_LIE;j++)
		  {
        for(k=BOX_HBYTE-1;k>0;k--)  //֡�ֽ�
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
  * ����   : ������Ҫ��ʾ�������������Ƴ�������
  *		   
  * ����   : s:�ο�����������ָ��
  *          p:Ҫ���صĻ�����ָ��
  *          p_node:Ҫ��ʾ�Ľ��ָ��
  *
  *
  * ����ֵ : ��
  ******************************************************************************
  */
void LoadDotaDownIn (uint8_t buf[][DIS_BUF_LEN],uint32_t addr,uint16_t step)
{
  uint8_t i;
	uint16_t j,z;
	uint8_t *sp,*dp;
	uint16_t start_lie = 0;
	
	start_lie = SCREEN_LIE-1-step;//������������
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
	
	if(G_FrameFlag == 0)//����RGB���ݷ��͹���
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
  * ����   : ������Ҫ��ʾ�������������Ƴ�������
  *		   
  * ����   : s:�ο�����������ָ��
  *          p:Ҫ���صĻ�����ָ��
  *          p_node:Ҫ��ʾ�Ľ��ָ��
  *
  *
  * ����ֵ : ��
  ******************************************************************************
  */
void LoadDotaDownOut (uint8_t buf[][DIS_BUF_LEN],uint32_t addr,uint16_t step)
{
  uint8_t i;
	uint16_t j,z;
	uint8_t *sp,*dp;
	
	//���㻺����
	if(step >= BOX_LIE-1)
	{
	  ClrDisplayBuf();
		return;
	}
	if(G_FrameFlag == 0)//����RGB���ݷ��͹���
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
  * ����   : ������Ҫ��ʾ�����ݴ������뻺����
  *		   
  * ����   : s:�ο�����������ָ��
  *          p:Ҫ���صĻ�����ָ��
  *          p_node:Ҫ��ʾ�Ľ��ָ��
  *
  *
  * ����ֵ : ��
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

	if(G_FrameFlag == 0)//����RGB���ݷ��͹���
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
  * ����   : ������Ҫ��ʾ�������������Ƴ�������
  *		   
  * ����   : s:�ο�����������ָ��
  *          p:Ҫ���صĻ�����ָ��
  *          p_node:Ҫ��ʾ�Ľ��ָ��
  *
  *
  * ����ֵ : ��
  ******************************************************************************
  */
void LoadDotaUpOut (uint8_t buf[][DIS_BUF_LEN],uint32_t addr,uint16_t step)
{
  uint8_t i;
	uint16_t j,z;
	uint8_t *sp,*dp;
	
	//���㻺����
	if(step >= BOX_LIE-1)
	{
	  ClrDisplayBuf();
		return;
	}
	if(G_FrameFlag == 0)//����RGB���ݷ��͹���
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
  * ����   : ���Ҷ��-���ֽ�Ϊ��λ
  *		   
  * ����   : s:�ο�����������ָ��
  *          p:Ҫ���صĻ�����ָ��
  *          p_node:Ҫ��ʾ�Ľ��ָ��
  *
  *
  * ����ֵ : ��
  ******************************************************************************
  */
void LoadDotaHengIn (uint8_t buf[][DIS_BUF_LEN],uint32_t addr,uint16_t step)
{
  uint8_t i,k;
	uint16_t j;
	
	//���㻺����
  ClrDisplayBuf();
	if(G_FrameFlag == 0)//����RGB���ݷ��͹���
    return;			
  for(i=0;i<SCREEN_YANSE;i++)//��ɫ
  {
    for(k=0;k<BOX_LIE;k+=8)//���ֽ�
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
  * ����   : ���Ҷ��-���ֽ�Ϊ��λ
  *		   
  * ����   : s:�ο�����������ָ��
  *          p:Ҫ���صĻ�����ָ��
  *          p_node:Ҫ��ʾ�Ľ��ָ��
  *
  *
  * ����ֵ : ��
  ******************************************************************************
  */
void LoadDotaHengOut (uint8_t buf[][DIS_BUF_LEN],uint32_t addr,uint16_t step)
{
  uint8_t i,k;
	uint16_t j;
	
  //���������ݺ�Ҫ��ʾ�����Ƿ����
  for(i=0;i<SCREEN_YANSE;i++)//��ɫ
  {
    for(k=0;k<BOX_LIE;k+=8)//���ֽ�
	  {
		  for(j=0;j<BOX_HBYTE;j++)//����
				buf[i][(step+k)*BOX_HBYTE+j] = 0;
    }
		if(G_FrameFlag == 0)//����RGB���ݷ��͹���
      return;	
  }    
}

/**															   
  ******************************************************************************
  *                            LoadDotaShuIn()
  *
  * ����   : ����Ҷ��-���ֽ�Ϊ��λ
  *		   
  * ����   : s:�ο�����������ָ��
  *          p:Ҫ���صĻ�����ָ��
  *          p_node:Ҫ��ʾ�Ľ��ָ��
  *
  *
  * ����ֵ : ��
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
	if(G_FrameFlag == 0)//����RGB���ݷ��͹���
    return;	
  if(step < 7)	//Ϊ7ʱȫ����ʾ������Ҫ��װ����
	{
	  data = shuIN_tab[step];
		for(i=0;i<BOX_YANSE;i++)//��ɫ
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
  * ����   : ����Ҷ��-���ֽ�Ϊ��λ
  *		   
  * ����   : s:�ο�����������ָ��
  *          p:Ҫ���صĻ�����ָ��
  *          p_node:Ҫ��ʾ�Ľ��ָ��
  *
  *
  * ����ֵ : ��
  ******************************************************************************
  */
const uint8_t shuOUT_tab[] = {0x7f,0x3f,0x1f,0x0f,0x07,0x03,0x01,0x00};
void LoadDotaShuOut (uint8_t buf[][DIS_BUF_LEN],uint32_t addr,uint16_t step)
{
  uint8_t i,k;
	uint16_t j;
	uint8_t data;
	uint8_t *datap;
	
	if(step == 7)//���һ��������
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
		if(G_FrameFlag == 0)//����RGB���ݷ��͹���
      return;	
	  for(i=0;i<SCREEN_YANSE;i++)//��ɫ
    {
      datap = buf[i];
		  for(j=0;j<BOX_BYTE_NUM;j++)
		    *datap++ &= data;
    } 
  }
}


//������ʾ������
void ClrDisplayBuf (void)
{
	uint16_t j;
  uint8_t i,*pot;	
	//���㻺����
	for(i=0;i<SCREEN_YANSE;i++)
	{
		pot = (uint8_t *)&DisplayBuf[i];
		for(j=0;j<BOX_BYTE_NUM;j++)
			*pot++ = 0;
		if(G_FrameFlag == 0)//����RGB���ݷ��͹���
      return;	
	}  
}

