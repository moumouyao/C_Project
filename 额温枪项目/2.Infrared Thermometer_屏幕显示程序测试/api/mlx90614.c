#include "mlx90614.h"



#define ACK	 					0
#define	NACK 					1
#define SA						0x00 //从机地址，单个MLX90614时地址为0x00,多个时地址默认为0x5a
#define RAM_ACCESS		0x00 //RAM access command
#define EEPROM_ACCESS	0x20 //EEPROM access command
#define RAM_TOBJ1			0x07 //To1 address in the eeprom

sbit SMBUS_SCK=P2^4;
sbit SMBUS_SDA=P2^5;

#define SMBUS_SCK_H()	    (SMBUS_SCK=1)
#define SMBUS_SCK_L()	    (SMBUS_SCK=0)
#define SMBUS_SDA_H()	    (SMBUS_SDA=1)
#define SMBUS_SDA_L()	    (SMBUS_SDA=0)

#define SMBUS_SDA_PIN()	    (SMBUS_SDA) //读取引脚电平

void Delay5us()		//@24.000MHz
{
	unsigned char i;

	i = 38;
	while (--i);
}


void Delay2us()		//@24.000MHz
{
	unsigned char i;

	i = 14;
	while (--i);
}



void Delay3us()		//@24.000MHz
{
	unsigned char i;

	i = 22;
	while (--i);
}


void Delay6us()		//@24.000MHz
{
	unsigned char i;

	i = 46;
	while (--i);
}



/*******************************************************************************
* 函数名: SMBus_StartBit
* 功能  : 产生起始位
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void SMBus_StartBit(void)
{
    SMBUS_SDA_H();		// Set SDA line
    Delay5us();	    // Wait a few microseconds
    SMBUS_SCK_H();		// Set SCL line
    Delay5us();	    // Generate bus free time between Stop
    SMBUS_SDA_L();		// Clear SDA line
    Delay5us();	    // Hold time after (Repeated) Start
    // Condition. After this period, the first clock is generated.
    //(Thd:sta=4.0us min)
    SMBUS_SCK_L();	    // Clear SCL line
    Delay5us();	    // Wait a few microseconds
}

/*******************************************************************************
* 函数名: SMBus_StopBit
* 功能: Generate STOP condition on SMBus
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void SMBus_StopBit(void)
{
    SMBUS_SCK_L();		// Clear SCL line
    Delay5us();	    // Wait a few microseconds
    SMBUS_SDA_L();		// Clear SDA line
    Delay5us();	    // Wait a few microseconds
    SMBUS_SCK_H();		// Set SCL line
    Delay5us();	    // Stop condition setup time(Tsu:sto=4.0us min)
    SMBUS_SDA_H();		// Set SDA line
}

/*******************************************************************************
* 函数名: SMBus_SendByte
* 功能: Send a byte on SMBus
* Input          : Tx_buffer
* Output         : None
* Return         : None
*******************************************************************************/
u8 SMBus_SendByte(u8 Tx_buffer)
{
    u8	Bit_counter;
    u8	Ack_bit;
    u8	bit_out;

    for(Bit_counter=8; Bit_counter; Bit_counter--)
    {
        if (Tx_buffer&0x80)
        {
            bit_out=1;   // If the current bit of Tx_buffer is 1 set bit_out
        }
        else
        {
            bit_out=0;  // else clear bit_out
        }
        SMBus_SendBit(bit_out);		// Send the current bit on SDA
        Tx_buffer<<=1;				// Get next bit for checking
    }

    Ack_bit=SMBus_ReceiveBit();		// Get acknowledgment bit
    return	Ack_bit;
}

/*******************************************************************************
* 函数名: SMBus_SendBit
* 功能: Send a bit on SMBus 82.5kHz
* Input          : bit_out
* Output         : None
* Return         : None
*******************************************************************************/
void SMBus_SendBit(u8 bit_out)
{
    if(bit_out==0)
    {
        SMBUS_SDA_L();
    }
    else
    {
        SMBUS_SDA_H();
    }
    Delay2us();					// Tsu:dat = 250ns minimum
    SMBUS_SCK_H();					// Set SCL line
    Delay6us();					// High Level of Clock Pulse
    SMBUS_SCK_L();					// Clear SCL line
    Delay3us();					// Low Level of Clock Pulse
//	SMBUS_SDA_H();				    // Master release SDA line ,
    return;
}

/*******************************************************************************
* Function Name  : SMBus_ReceiveBit
* Description    : Receive a bit on SMBus
* Input          : None
* Output         : None
* Return         : Ack_bit
*******************************************************************************/
u8 SMBus_ReceiveBit(void)
{
    u8 Ack_bit;

    SMBUS_SDA_H();          //引脚靠外部电阻上拉，当作输入
		Delay2us();			// High Level of Clock Pulse
    SMBUS_SCK_H();			// Set SCL line
    Delay5us();			// High Level of Clock Pulse
    if (SMBUS_SDA_PIN())
    {
        Ack_bit=1;
    }
    else
    {
        Ack_bit=0;
    }
    SMBUS_SCK_L();			// Clear SCL line
    Delay3us();			// Low Level of Clock Pulse

    return	Ack_bit;
}

/*******************************************************************************
* 函数名: SMBus_ReceiveByte
* 功能: Receive a byte on SMBus
* Input          : ack_nack
* Output         : None
* Return         : RX_buffer
*******************************************************************************/
u8 SMBus_ReceiveByte(u8 ack_nack)
{
    u8 	RX_buffer;
    u8	Bit_Counter;

    for(Bit_Counter=8; Bit_Counter; Bit_Counter--)
    {
        if(SMBus_ReceiveBit())			// Get a bit from the SDA line
        {
            RX_buffer <<= 1;			// If the bit is HIGH save 1  in RX_buffer
            RX_buffer |=0x01;
        }
        else
        {
            RX_buffer <<= 1;			// If the bit is LOW save 0 in RX_buffer
            RX_buffer &=0xfe;
        }
    }
    SMBus_SendBit(ack_nack);			// Sends acknowledgment bit
    return RX_buffer;
}

/*******************************************************************************
* 函数名: SMBus_Delay
* 功能: 延时  一次循环约1us
* Input          : time
* Output         : None
* Return         : None
*******************************************************************************/
//void SMBus_Delay(u16 time)
//{
//    u16 i, j;
//    for (i=0; i<4; i++)
//    {
//        for (j=0; j<time; j++);
//    }
//}

/*******************************************************************************
* 函数名: SMBus_Init
* 功能: SMBus初始化
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void SMBus_Init()
{
	SMBUS_SCK_H();
	SMBUS_SDA_H();
}

/*******************************************************************************
 * 函数名: SMBus_ReadMemory
 * 功能: READ DATA FROM RAM/EEPROM
 * Input          : slaveAddress, command
 * Return         : Data
*******************************************************************************/
u16 SMBus_ReadMemory(u8 slaveAddress, u8 command)
{
    u16 dataa;			// Data storage (DataH:DataL)
    u8 Pec;				// PEC byte storage
    u8 DataL=0;			// Low data byte storage
    u8 DataH=0;			// High data byte storage
    u8 arr[6];			// Buffer for the sent bytes
    u8 PecReg;			// Calculated PEC byte storage
    u8 ErrorCounter;	// Defines the number of the attempts for communication with MLX90614

    ErrorCounter=0x00;				// Initialising of ErrorCounter
		slaveAddress <<= 1;	//2-7位表示从机地址
	
    do
    {
repeat:
        SMBus_StopBit();			    //If slave send NACK stop comunication
        --ErrorCounter;				    //Pre-decrement ErrorCounter
        if(!ErrorCounter) 			    //ErrorCounter=0?
        {
            break;					    //Yes,go out from do-while{}
        }

        SMBus_StartBit();				//Start condition
        if(SMBus_SendByte(slaveAddress))//Send SlaveAddress 最低位Wr=0表示接下来写命令
        {
            goto	repeat;			    //Repeat comunication again
        }
        if(SMBus_SendByte(command))	    //Send command
        {
            goto	repeat;		    	//Repeat comunication again
        }

        SMBus_StartBit();					//Repeated Start condition
        if(SMBus_SendByte(slaveAddress+1))	//Send SlaveAddress 最低位Rd=1表示接下来读数据
        {
            goto	repeat;             	//Repeat comunication again
        }

        DataL = SMBus_ReceiveByte(ACK);	//Read low data,master must send ACK
        DataH = SMBus_ReceiveByte(ACK); //Read high data,master must send ACK
        Pec = SMBus_ReceiveByte(NACK);	//Read PEC byte, master must send NACK
        SMBus_StopBit();				//Stop condition

        arr[5] = slaveAddress;		//
        arr[4] = command;			//
        arr[3] = slaveAddress+1;	//Load array arr
        arr[2] = DataL;				//
        arr[1] = DataH;				//
        arr[0] = 0;					//
        PecReg=PEC_Calculation(arr);//Calculate CRC
    }
    while(PecReg != Pec);		//If received and calculated CRC are equal go out from do-while{}

		dataa = (DataH<<8) | DataL;	//data=DataH:DataL
    return dataa;
}

/*******************************************************************************
* 函数名: PEC_calculation
* 功能 : 数据校验
* Input          : pec[]
* Output         : None
* Return         : pec[0]-this byte contains calculated crc value
*******************************************************************************/
u8 PEC_Calculation(u8 pec[])
{
    u8 	crc[6];
    u8	BitPosition=47;
    u8	shift;
    u8	i;
    u8	j;
    u8	temp;

    do
    {
        /*Load pattern value 0x000000000107*/
        crc[5]=0;
        crc[4]=0;
        crc[3]=0;
        crc[2]=0;
        crc[1]=0x01;
        crc[0]=0x07;

        /*Set maximum bit position at 47 ( six bytes byte5...byte0,MSbit=47)*/
        BitPosition=47;

        /*Set shift position at 0*/
        shift=0;

        /*Find first "1" in the transmited message beginning from the MSByte byte5*/
        i=5;
        j=0;
        while((pec[i]&(0x80>>j))==0 && i>0)
        {
            BitPosition--;
            if(j<7)
            {
                j++;
            }
            else
            {
                j=0x00;
                i--;
            }
        }/*End of while */

        /*Get shift value for pattern value*/
        shift=BitPosition-8;

        /*Shift pattern value */
        while(shift)
        {
            for(i=5; i<0xFF; i--)
            {
                if((crc[i-1]&0x80) && (i>0))
                {
                    temp=1;
                }
                else
                {
                    temp=0;
                }
                crc[i]<<=1;
                crc[i]+=temp;
            }/*End of for*/
            shift--;
        }/*End of while*/

        /*Exclusive OR between pec and crc*/
        for(i=0; i<=5; i++)
        {
            pec[i] ^=crc[i];
        }/*End of for*/
    }
    while(BitPosition>8); /*End of do-while*/

    return pec[0];
}

 /*******************************************************************************
 * 函数名: SMBus_ReadTemp
 * 功能: 计算并返回温度值
 * Return         : SMBus_ReadMemory(0x00, 0x07)*0.02-273.15
*******************************************************************************/
float SMBus_ReadTemp(void)
{   
	return SMBus_ReadMemory(SA, RAM_ACCESS|RAM_TOBJ1)*0.02-273.15;
}

/*********************************END OF FILE*********************************/



