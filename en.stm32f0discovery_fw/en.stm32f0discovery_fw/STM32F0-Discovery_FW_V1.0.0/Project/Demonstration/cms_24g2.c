#include "cms_24g2.h"
#include "core_cmInstr.h"

#define REG_BIT_SET    	1
#define REG_BIT_CLEAN  	0

const u8 reg_table_2[7][2] =    
{
//	1M		
		{18,0x48},{34,0x08},{52,0x19},{70,0x0a},{82,0xB0},{85,0xf0},{15,0x0E}
};
static  u8 len,checksum;
//u8 data_buf_length_2 = 15;
u8 tmp_buf_tx2_2[15]={1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};

static void WL1600_Write_Reg_2(u8 reg,u8 value);
static u8 WL1600_Read_Reg_2(u8 reg);
static void WL1600_Write_Reg_Bit_2(u8 reg,u8 bitVal,u8 value);
static u8 WL1600_Read_Reg_Bit_2(u8 reg,u8 bitVal);
static u8 WL1600_Check_2(void);
static void WL1600_Pack_Length_En_2(u8 mode);
static void WL1600_Pkg_Flag_Mask_2(u8 value);
//static void WL1600_Clean_Tx_Fifo_2(void);
static void WL1600_Clean_Rx_Fifo_2(void);
static u8 WL1600_Read_Fifo_Data_2(void);
//static void WL1600_Write_Fifo_Data_2(u8 value);
//static void WL1600_TX_Mode_2(void);
static void WL1600_RX_Mode_2(void);
static u8 WL1600_Pkt_Flag_2(void);
static void WL1600_Clean_Pkg_Flag_2(void);
static u8 WL1600_Crc_Error_Flag_2(void);
static void DelayUs_2(int Time);
static void DelayMs_2(int Time);

static void DelayUs_2(int Time)
{
	int a;
	for(a=0;a<Time;a++)
	{
		//NOP();
		__NOP;
	}
	//while(Time--);
}                  

static void DelayMs_2(int Time)
{
	int a,b;
	for(a=0;a<Time;a++)
	{
		for(b=0;b<5;b++)
		{
		 	DelayUs_2(197);
		}
	}
}

static void WL1600_Write_Reg_2(u8 reg,u8 value)
{/*
	//u8 status;	
   	WL1600_CSN=0;
  	SPI_Send_Dat(reg);
  	SPI_Send_Dat(value);
  	WL1600_CSN=1; 
  	//return(status);*/
	
	SPI_CS_Enable_();
	//SPI_WriteRead(reg);
	//SPI_WriteRead(value);
	SPI_Write(reg);
	SPI_Write(value);
	SPI_CS_Disable_();
}

static u8 WL1600_Read_Reg_2(u8 reg)
{/*
	u8 reg_val;	    
 	WL1600_CSN = 0; 
  	SPI_Send_Dat(reg | 0x80);
  	reg_val = SPI_Receiver_Dat();
  	WL1600_CSN = 1;
  	return(reg_val);*/
	
	u8 readData = 0;
	SPI_CS_Enable_();
	//readData = SPI_WriteRead(reg);
	//readData = SPI_WriteRead(0);
	SPI_Write(reg);
	readData = SPI_Read();
	SPI_CS_Disable_();
	return readData;
}

static void WL1600_Write_Reg_Bit_2(u8 reg,u8 bitVal,u8 value)
{
	u8 reg_val;
	u8 bit_num;
	bit_num = 0x1 << bitVal;
	reg_val = WL1600_Read_Reg_2(reg);
	if(value == REG_BIT_SET)
	{
		reg_val |= bit_num;
	}
	else
	{
		reg_val &= ~bit_num;
	}
	WL1600_Write_Reg_2(reg,reg_val);
}

static u8 WL1600_Read_Reg_Bit_2(u8 reg,u8 bitVal)
{
	u8 reg_val;
	u8 bit_num;
	bit_num = 0x1 << bitVal;	
	reg_val = WL1600_Read_Reg_2(reg);
	if((reg_val&bit_num) == 0x0)
	{
		return REG_BIT_CLEAN;
	}
	else
	{
		return REG_BIT_SET;
	}
}
static u8 WL1600_Check_2(void)
{
 		u8 i = 0;
		//WL1600_CSN=0;//WL1600_CE=0;
		SPI_CE_SetLow();
		//DelayUs(20);
    DelayMs_2(10);
		//WL1600_CSN=1;//WL1600_CE=1;
		SPI_CE_SetHigh();
		DelayMs_2(210);
		//WL1600_Write_Reg(94,0x80);   //3 SPI MODE
		//DelayMs(210);
		for (i = 0; i <7; ++i)
		{
			WL1600_Write_Reg_2(reg_table_2[i][0],reg_table_2[i][1]);
			if(i!=2)
			{
				if(reg_table_2[i][1] != WL1600_Read_Reg_2(reg_table_2[i][0]))
				{					
					return 1;
				}
			}
		}
		return 0;
}

static void WL1600_Pack_Length_En_2(u8 mode)
{
	WL1600_Write_Reg_Bit_2(82,5,mode);
}

static void WL1600_Pkg_Flag_Mask_2(u8 value)
{
	WL1600_Write_Reg_Bit_2(33,1,value);
}

/*static void WL1600_Clean_Tx_Fifo_2(void)
{
	WL1600_Write_Reg_Bit_2(104,7,REG_BIT_SET);
}*/

static void WL1600_Clean_Rx_Fifo_2(void)
{
	WL1600_Write_Reg_Bit_2(105,7,REG_BIT_SET);
}

static u8 WL1600_Read_Fifo_Data_2(void)
{
	return WL1600_Read_Reg_2(100);
}

/*static void WL1600_Write_Fifo_Data_2(u8 value)
{          
	WL1600_Write_Reg_2(100,value);
}*/

/*static void WL1600_TX_Mode_2(void)
{
	WL1600_Write_Reg_Bit_2(15,7,REG_BIT_CLEAN);	
	WL1600_Write_Reg_Bit_2(14,0,REG_BIT_SET);
}*/

void WL1600_RX_Mode_2(void)
{
	WL1600_Write_Reg_Bit_2(14,0,REG_BIT_CLEAN);
	WL1600_Write_Reg_Bit_2(15,7,REG_BIT_SET);	
}	

static u8 WL1600_Pkt_Flag_2(void)                        
{
	return WL1600_Read_Reg_Bit_2(97,6);
}

static void WL1600_Clean_Pkg_Flag_2(void)
{
	WL1600_Write_Reg_Bit_2(37,1,REG_BIT_SET);		
}

static u8 WL1600_Crc_Error_Flag_2(void)
{
	return WL1600_Read_Reg_Bit_2(96,7);
}

void rf_init_2()
{
	u8 i = 0;
	
	//WL1600_CSN = 1;
	SPI_CE_SetHigh();
  WL1600_Write_Reg_2(32,0x04);
/*	
	while(WL1600_Check_Init())
	{
		DelayMs(2);
		LED2=!LED2;						
	}
*/
	DelayMs_2(10);
	while(1 == WL1600_Check_2());
	//{
		//DelayMs_2(2);
		//LED1=!LED1;						
	//}
	
	WL1600_Pack_Length_En_2(1);
	WL1600_Pkg_Flag_Mask_2(REG_BIT_SET);
	

#if 0
	while(1)
	{
		while(1)
		{
       WL1600_RX_Mode_2();
       i=15;
       while(i)
       {
          DelayUs_2(100);
          if(WL1600_Pkt_Flag_2() == 1) break;
          i--;
       }
      //if(WL1600_Pkt_Flag() == 1)     	
			if(i>0)
			{
        checksum=0;
        len=0;
				WL1600_Clean_Pkg_Flag_2();
				if(WL1600_Crc_Error_Flag_2() == 0)
				{
          len=WL1600_Read_Fifo_Data_2();
					for(i=0;i<len;i++)
					{
						tmp_buf_tx2_2[i]= WL1600_Read_Fifo_Data_2();
            checksum += tmp_buf_tx2_2[i];
					}	
          //if(checksum==120)
						//LED1=!LED1;
          //else LED2=!LED2;
				}
       //WL1600_Clean_Tx_Fifo_2();
       WL1600_Clean_Rx_Fifo_2();
			}
		}
	}
#endif
}

void rf_receive_2()
{
	WL1600_RX_Mode_2();
}

