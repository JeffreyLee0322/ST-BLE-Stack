#include "cms_24g.h"
#include "core_cmInstr.h"
#include "cms_spi.h"

#define REG_BIT_SET    	1
#define REG_BIT_CLEAN  	0

#define REG_NUM         28

const u8 reg_table[][2] =
{
//	1M
		//{18,0x48},{34,0x08},{52,0x19},{70,0x0a},{82,0xB0},{85,0xf0},{15,0x0E}

//	1M		CH(2426) 2426->0X18 2480->0X4E
		//{18,0x48},{34,0x08},{52,0x19},{70,0x0a},{82,0xB0},{85,0xf0},{15,0x18},

//1M BLE Config(Receive)
#if 0
		//{64, 0x68},//preamble 4Byte
		{64, 0x48},//preamble 3Byte
		
		/*{73, 0xD6},
		{72, 0xBE},
		{79, 0x89},
		{78, 0x8E},*/
		
		{73, 0x20},
		{72, 0xC0},
		{79, 0x40},
		{78, 0x80},
		
		{41, 0x10}, //Disable Trailer
		{81, 0x00},
#endif

#if 0  //BLE Send
		{64, 0x68},//Len
		
		{73, 0x6B},//0xD6
		{72, 0x7D},//0xBE
		{79, 0x91},//0x89
		{78, 0x71},//0x8E
		
		{41, 0x10},//Disable Trailer & Config MSB/LSB
		{81, 0x00},//SYNCDATA THREAD
#endif
		
#if 1
		{0,  0x6F},//6F  7F
    {1,  0xE8},
    {2,  0x56},//56  57
    {3,  0x0D},
    {4,  0xC4}, // 
    {8,  0x25}, //
    {9,  0x41},//41  14
    {10, 0x57},
    {11, 0x3B},
    {16, 0x84},
		
		{17,0x5A},//3A
		{18,0x0C},
		{34,0x48},//08
		{41,0x10},
		{45,0x00},
		{46,0x01},
		{52,0x19},//19
		{53,0x40},
		
		{64, 0x6A},//0x0A
		//BLE Access_address: 0x8E89BED6
		{72, 0xBE},//BE
		{73, 0xD6},//D6 
		{78, 0x8E},//8E
		{79, 0x89}, //89
		/*{73,0x04},//SYNC Data
		{72,0x03},
		{79,0x02},
		{78,0x01},*/
		{81, 0x42},//0x44  FIFO_Full_ Threshold _L & SYNCWORD_ Threshold
		
		{82, 0x31},
    {66, 0x60},
    {67, 0xA0},
    {68, 0x80},
#endif

};

u8 data_buf_length = 21;
u8 tmp_buf_tx2[50]={1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};

static u8 len,checksum;

u8 test_read_reg = 0;
u8 test_write_reg = 0;
u8 test_count = 0;

u8 send_count = 0;

static void WL1600_Write_Reg(u8 reg,u8 value);
static u8 WL1600_Read_Reg(u8 reg);
static void WL1600_Write_Reg_Bit(u8 reg,u8 bitVal,u8 value);
static u8 WL1600_Read_Reg_Bit(u8 reg,u8 bitVal);
static u8 WL1600_Check(void);
static void WL1600_Pack_Length_En(u8 mode);
static void WL1600_Pkg_Flag_Mask(u8 value);
static void WL1600_Clean_Tx_Fifo(void);
static void WL1600_Clean_Rx_Fifo(void);
static u8 WL1600_Read_Fifo_Data(void);
static void WL1600_Write_Fifo_Data(u8 value);
static void WL1600_TX_Mode(void);
static void WL1600_RX_Mode(void);
static void WL1600_IDLE_Mode(void);
static void WL1600_Sleep(void);
static u8 WL1600_Pkt_Flag(void);
static void WL1600_Clean_Pkg_Flag(void);
static u8 WL1600_Crc_Error_Flag(void);
static void WL1600_Set_Channel(u8 channel);
static void DelayUs(int Time);
static void DelayMs(int Time);

static void DelayUs(int Time)
{
	int a;
	for(a=0;a<Time;a++)
	{
		__NOP;
	}
}                  

static void DelayMs(int Time)
{
	int a,b;
	for(a=0;a<Time;a++)
	{
		for(b=0;b<5;b++)
		{
		 	DelayUs(197);
		}
	}
}

static void WL1600_Write_Reg(u8 reg,u8 value)
{/*
	//u8 status;	
   	WL1600_CSN=0;
  	SPI_Send_Dat(reg);
  	SPI_Send_Dat(value);
  	WL1600_CSN=1; 
  	//return(status);*/
	
	u16 data = (u16)(value & 0xffff) << 8;
	u16 sendData = data | reg;
	
	SPI_CS_Enable_();
	SPI_WriteRead(sendData);
	//SPI_WriteRead(value);
	//SPI_Write(reg);
	//SPI_Write(value);
	SPI_CS_Disable_();
}

static u8 WL1600_Read_Reg(u8 reg)
{/*
	u8 reg_val;	    
 	WL1600_CSN = 0; 
  	SPI_Send_Dat(reg | 0x80);
  	reg_val = SPI_Receiver_Dat();
  	WL1600_CSN = 1;
  	return(reg_val);*/
	
	u16 data = (u16)(reg & 0xffff);
	u16 sendData = (data | 0x0080);
	
	u8 readData = 0;
	SPI_CS_Enable_();
	readData = SPI_WriteRead(sendData);
	//readData = SPI_WriteRead(0);
	//SPI_Write(reg);
	//readData = SPI_Read();
	SPI_CS_Disable_();
	return readData;
}

static void WL1600_Write_Reg_Bit(u8 reg,u8 bitVal,u8 value)
{
	u8 reg_val;
	u8 bit_num;
	bit_num = 0x1 << bitVal;
	reg_val = WL1600_Read_Reg(reg);
	if(value == REG_BIT_SET)
	{
		reg_val |= bit_num;
	}
	else
	{
		reg_val &= ~bit_num;
	}
	WL1600_Write_Reg(reg,reg_val);
}

static u8 WL1600_Read_Reg_Bit(u8 reg,u8 bitVal)
{
	u8 reg_val;
	u8 bit_num;
	bit_num = 0x1 << bitVal;	
	reg_val = WL1600_Read_Reg(reg);
	if((reg_val&bit_num) == 0x0)
	{
		return REG_BIT_CLEAN;
	}
	else
	{
		return REG_BIT_SET;
	}
}
static u8 WL1600_Check(void)
{
 		u8 i = 0;
		SPI_CS_Enable_();
		//DelayUs(20);
    DelayMs(20);
		SPI_CS_Disable_();
		DelayMs(210);
		//WL1600_Write_Reg(94,0x80);   //3 SPI MODE
		//DelayMs(210);
		for (i = 0; i < REG_NUM; ++i)
		{
			WL1600_Write_Reg(reg_table[i][0],reg_table[i][1]);
			if(i!=2)
			{
				test_count = i;
				test_write_reg = reg_table[i][1];
				test_read_reg = WL1600_Read_Reg(reg_table[i][0]);
				if(reg_table[i][1] != test_read_reg)
				{
					return 1;
				}
			}
		}
		return 0;
}

static void WL1600_Pack_Length_En(u8 mode)
{
	WL1600_Write_Reg_Bit(82,5,mode);
}

static void WL1600_Pkg_Flag_Mask(u8 value)
{
	WL1600_Write_Reg_Bit(33,1,value);
}

static void WL1600_Clean_Tx_Fifo(void)
{
	WL1600_Write_Reg_Bit(104,7,REG_BIT_SET);
}

static void WL1600_Clean_Rx_Fifo(void)
{
	WL1600_Write_Reg_Bit(105,7,REG_BIT_SET);
}

static u8 WL1600_Read_Fifo_Data(void)
{
	return WL1600_Read_Reg(100);
}

static void WL1600_Write_Fifo_Data(u8 value)
{
	WL1600_Write_Reg(100,value);
}

static void WL1600_TX_Mode(void)
{
	WL1600_Write_Reg_Bit(15,7,REG_BIT_CLEAN);	
	WL1600_Write_Reg_Bit(14,0,REG_BIT_SET);
}

static void WL1600_RX_Mode(void)
{
	WL1600_Write_Reg_Bit(14,0,REG_BIT_CLEAN);
	WL1600_Write_Reg_Bit(15,7,REG_BIT_SET);	
}

static void WL1600_IDLE_Mode(void)
{

}

static void WL1600_Sleep(void)
{

}

static u8 WL1600_Pkt_Flag(void)                        
{
	return WL1600_Read_Reg_Bit(97,6);
}

static void WL1600_Clean_Pkg_Flag(void)
{
	WL1600_Write_Reg_Bit(37,1,REG_BIT_SET);		
}

static u8 WL1600_Crc_Error_Flag(void)
{
	return WL1600_Read_Reg_Bit(96,7);
}

static void WL1600_Set_Channel(u8 channel)
{
    u8 val = 0;
    if(channel <= 78)
    {
        val = WL1600_Read_Reg(15);
        val = (val & 0x80) + channel;
        WL1600_Write_Reg(15, val);
    }
}

void rf_init()
{
	u8 i = 0;
	
	SPI_CS_Disable_();
	DelayMs(10);
  WL1600_Write_Reg(32,0x04);
	SPI_CS_Disable_();

	DelayMs(20);
	while(WL1600_Check())
	{
		DelayMs(2);				
	}
	
	WL1600_Pack_Length_En(1);
	WL1600_Pkg_Flag_Mask(REG_BIT_SET);
	
	//Jeffrey add @2018-07-25
  WL1600_Write_Reg(82,0x31);
  WL1600_Write_Reg(95,0x80|0x24);
  WL1600_Write_Reg(105,0x80);
  //WL1600_Write_Reg_Bit(37,1,1);
  WL1600_Set_Channel(24);
	
	#if 0 //Receive info
	while(1)
	{
		while(1)
		{
       WL1600_RX_Mode();
       i=15;
       while(i)
       {
					len=0;
          DelayUs(100);
          if(WL1600_Pkt_Flag() == 1) 
					{
							len=WL1600_Read_Fifo_Data();
							for(i=0;i<len;i++)
							{
								tmp_buf_tx2[i]= WL1600_Read_Fifo_Data();
							}	
							DelayUs(1);
							break;
					}
          i--;
       }
   	
			/*if(i>0)
			{
        checksum=0;
        len=0;
				WL1600_Clean_Pkg_Flag();
				if(WL1600_Crc_Error_Flag() == 0)
				{
          len=WL1600_Read_Fifo_Data();
					for(i=0;i<len;i++)
					{
						tmp_buf_tx2[i]= WL1600_Read_Fifo_Data();
            checksum += tmp_buf_tx2[i];
					}	
					DelayUs(1);
				}
       WL1600_Clean_Tx_Fifo();
       WL1600_Clean_Rx_Fifo();
			}*/
			 WL1600_Clean_Tx_Fifo();
       WL1600_Clean_Rx_Fifo();
		}
	}
#endif
}

void rf_send(u8 *data, u8 num)
{
	u8 i = 0;

	while(1)//Send
	{
		//For No Packet len case
		/*DelayMs(10);
		SPI_CS_Disable_();
		DelayMs(10);*/
	 
		WL1600_Clean_Tx_Fifo();
    //  WL1600_Clean_Rx_Fifo();//???fifo
		//WL1600_Write_Fifo_Data(data_buf_length);
		//send_count++;
		//if(send_count > 100) send_count = 0;
		//tmp_buf_tx2[0] = send_count;
		
		for(i=0;i<data_buf_length ;i++)
		{
				//WL1600_Write_Fifo_Data(tmp_buf_tx2[i]);
				WL1600_Write_Fifo_Data(data[i]);
		}

		WL1600_TX_Mode();
		
/*		DelayMs(200);
		
		//Set 2.4g Chip IDLE mode
		WL1600_IDLE_Mode();
		
		//Set 2.4g Chip Sleep
		WL1600_Sleep();
		
		DelayMs(1000);*/

		while(1)
		{
        DelayUs(100);
				if(WL1600_Pkt_Flag() == 1)
				{
					send_count++;
					if(send_count > 100) send_count = 0;
					
					WL1600_Clean_Pkg_Flag();
					break;
				}
		}

    DelayMs(1000);
    //DelayMs(5000);
		//DelayMs(5000);
		//DelayMs(5000);
    //DelayMs(5000);
		//DelayMs(5000);
		//DelayMs(5000);
    //DelayMs(5000);
		//DelayMs(5000);
	}
}

void rf_receive()
{
		u8 i = 0;
	
#if 1
	while(1)
	{
		while(1)
		{
       WL1600_RX_Mode();
       i=15;
       while(i)
       {
					len=0;
          DelayUs(100);
          if(WL1600_Pkt_Flag() == 1) 
					{
							len=WL1600_Read_Fifo_Data();
							for(i=0;i<len;i++)
							{
								tmp_buf_tx2[i]= WL1600_Read_Fifo_Data();
							}	
							break;
					}
          i--;
       }
   	
			if(i>0)
			{
        checksum=0;
        len=0;
				WL1600_Clean_Pkg_Flag();
				if(WL1600_Crc_Error_Flag() == 0)
				{
          len=WL1600_Read_Fifo_Data();
					for(i=0;i<len;i++)
					{
						tmp_buf_tx2[i]= WL1600_Read_Fifo_Data();
            checksum += tmp_buf_tx2[i];
					}	
          //if(checksum==120)
						//LED1=!LED1;
          //else LED2=!LED2;
				}
       WL1600_Clean_Tx_Fifo();
       WL1600_Clean_Rx_Fifo();
			}
		}
	}
#endif
}

