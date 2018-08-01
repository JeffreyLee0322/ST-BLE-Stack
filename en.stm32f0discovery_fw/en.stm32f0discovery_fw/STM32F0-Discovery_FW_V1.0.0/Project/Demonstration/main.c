/**
  ******************************************************************************
  * @file    main.c 
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    23-March-2012
  * @brief   Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2012 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */ 

/* Includes ------------------------------------------------------------------*/
#include "main.h"

//#include "uart.h"
//#include "i2c.h"
//#inlcude "delay.h"
#include "cms_spi.h"
#include "cms_24g.h"
#include "cms_24g2.h"

#include "string.h"

/** @addtogroup STM32F0-Discovery_Demo
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static __IO uint32_t TimingDelay;
uint8_t BlinkSpeed = 0;

//#define TEST_I2C

void EXTI0_Config(void);

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/




void adv_init_format_get(uint8_t *adv_format);
void whitening_init(uint8_t channel_index);
uint8_t whitening_output(void);
void whitening_decode(uint8_t *data, uint8_t length);

void check_crc(uint8_t *data, uint8_t length);
void crc_update(uint8_t input_data_bit);
uint32_t arr_to_crc(void);
void crc_init(void);

void adv_data_to_lsb(void);
/*****************************************************BLE: Device->Phone**************************************************/
typedef struct adv_header
{
  uint8_t ADV_PDU_Type:4;
  uint8_t ADV_RFU:2;
  uint8_t ADV_TxAdd:1;
  uint8_t ADV_RxAdd:1;
  uint8_t ADV_Length:6;
  uint8_t ADV_RFU_:2;
}ADV_PDU_HEADER;

typedef struct adv_ind_pdu
{
  uint8_t ADV_MAC_Address[6];
  //uint8_t ADV_PDU_Data[31];
  uint8_t ADV_BlutoothType[3];//len+type+value
  //uint8_t ADV_LocalName[6];//len+type+name=6
  uint8_t ADV_MenufacData[7];//len+type+value=7
  uint8_t ADV_CRC[3];
}ADV_IND_PDU_DATA;

uint8_t whitening_reg[7] = {0, };
uint8_t crc_reg[24] = {0, };
uint8_t ADV_DATA[50] = {1,1,1,1,1,1,1,1,1,1,1};
uint8_t mac_address[6] = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06};//HUAWEI P9: 0x67A4765C8AA8
//Packet Sniffer: 0x8E89BED6
//BLE SIG: 10001110100010011011111011010110b (0x8E89BED6)   LSB Forward
//uint8_t access_address[4] = {0xD6, 0xBE, 0x89, 0x8E};
uint8_t access_address[4] = {0xD6, 0xBE, 0x89, 0x8E};
uint8_t test_crc[3] = {0x23, 0x34, 0x22};
//uint8_t local_name[6] = {0x04, 0x09, 0x4D, 0x4D, 0x53, 0x2D};
uint8_t local_name[6] = {0x04, 0x09, 0xAA, 0xAB, 0xAC, 0xAD};
uint8_t test_ble_type[3] = {0x02, 0x01, 0x06};
uint8_t test_Menufacture_Data[7] = {0x06, 0xFF, 0x5B, 0x37, 0x01, 0x02, 0x03};

void adv_init_format_get(uint8_t *adv_format)
{
  ADV_PDU_HEADER adv_pdu_header;
  ADV_IND_PDU_DATA adv_ind_pdu_data;
  
  adv_pdu_header.ADV_PDU_Type = 0x02; //0x00:ADV_IND 0x02:NON_CONN
  adv_pdu_header.ADV_RFU = 0x00;
  adv_pdu_header.ADV_TxAdd = 0x01;
  adv_pdu_header.ADV_RxAdd = 0x00;
  adv_pdu_header.ADV_Length = 0x10;//6-37 0ctets=12 Bytes
  adv_pdu_header.ADV_RFU_ = 0x00;
  
  memcpy(adv_ind_pdu_data.ADV_MAC_Address, mac_address, 6);//Set ADV Mac address
  //memset(adv_ind_pdu_data.ADV_PDU_Data, 0, 31);
  memcpy(adv_ind_pdu_data.ADV_BlutoothType, test_ble_type, 3);
  //memcpy(adv_ind_pdu_data.ADV_LocalName, local_name, 6);
  memcpy(adv_ind_pdu_data.ADV_MenufacData, test_Menufacture_Data, 7);
  memcpy(adv_ind_pdu_data.ADV_CRC, test_crc, 3);
  
  //1. Packet
  /*adv_format[0] = 0x55; //Preamble
  memcpy(adv_format+1, access_address, 4);
  memcpy(adv_format+5, &adv_pdu_header, 2);
  //memcpy(adv_format+7, &adv_ind_pdu_data, 37);//Mac_address + PDU + CRC
  memcpy(adv_format+7, &adv_ind_pdu_data, 6+7+3+3);//Only Mac_address + BLE_Type + Local_name + CRC*/
  
  //2. Packet
  memcpy(adv_format, &adv_pdu_header, 2);
  //memcpy(adv_format+7, &adv_ind_pdu_data, 37);//Mac_address + PDU + CRC
  memcpy(adv_format+2, &adv_ind_pdu_data, 6+3+3+7);//Only Mac_address + BLE_Type + Local_name + CRC
  
  // (AA) (D6 BE 89 8E) (40 06) (A8 8A 5C 76 A4 63) + CRC
}

void adv_data_to_lsb(void)
{
	uint8_t tmp = 0, i = 0, j = 0;
	uint8_t adv_tmp = 0;
	
	for(i = 0; i < 21; i++)
	{
			adv_tmp = 0;
			for(j = 0; j < 8; j++)
			{
				tmp = ADV_DATA[i] >> j;
				if(tmp & 0x01)
				{
					adv_tmp |= (0x01 << (7 - j));
				}
			}
			ADV_DATA[i] = adv_tmp;
	}
}

void whitening_init(uint8_t channel_index)
{
	uint8_t i;
	whitening_reg[0] = 1;
	
	for (i = 1; i < 7; i++)
	{
		whitening_reg[i] = (channel_index >> (6 - i)) & 0x01;
	}
}

uint8_t whitening_output(void)
{
	uint8_t temp = whitening_reg[3] ^ whitening_reg[6];
	
	whitening_reg[3] = whitening_reg[2];
	whitening_reg[2] = whitening_reg[1];
	whitening_reg[1] = whitening_reg[0];
	whitening_reg[0] = whitening_reg[6];
	whitening_reg[6] = whitening_reg[5];
	whitening_reg[5] = whitening_reg[4];
	whitening_reg[4] = temp;
	
	return whitening_reg[0];
}

void whitening_decode(uint8_t *data, uint8_t length)
{
	uint8_t data_index = 0;
	uint8_t data_input = 0;
	uint8_t data_bit = 0;
	uint8_t data_output = 0;
	uint8_t bit_index = 0;
	
	for (data_index = 0; data_index < length; data_index++)
	{
		data_input = data[data_index];
		data_bit = 0;
		data_output = 0;
		
		for (bit_index = 0; bit_index < 8; bit_index++)
		{
			data_bit = (data_input >> (bit_index)) & 0x01;
			
			data_bit ^= whitening_output();
			
			data_output += (data_bit << (bit_index));
		}
		
		data[data_index] = data_output;
	}
}
/****************************************************BLE END******************************************************************/


/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
int main(void)
{
	uint32_t calc_crc = 0;
  RCC_ClocksTypeDef RCC_Clocks;
  
  /* Configure LED3 and LED4 on STM32F0-Discovery */
  //STM_EVAL_LEDInit(LED3);
 // STM_EVAL_LEDInit(LED4);
  
  /* Initialize User_Button on STM32F0-Discovery */
  //STM_EVAL_PBInit(BUTTON_USER, BUTTON_MODE_GPIO);
  
  /* SysTick end of count event each 1ms */
  RCC_GetClocksFreq(&RCC_Clocks);
  SysTick_Config(RCC_Clocks.HCLK_Frequency / 1000);
   
  /* Initiate Blink Speed variable */ 
 // BlinkSpeed = 1;
  
  //uart_init();
	//EXTI0_Config();
	
	/****************************************************Device->Phone**********************************************************/
#if 0
  //BLE Packet Format (16 Bytes)
  adv_init_format_get(ADV_DATA);
  //Preamble & access_code (5 Bytes)//
	//adv_data_to_lsb();
  
  crc_init();
  check_crc(ADV_DATA, 21 - 3);
  calc_crc = arr_to_crc();
  ADV_DATA[18] = (uint8_t)(calc_crc&0xff);
  ADV_DATA[19] = (uint8_t)((calc_crc>>8)&0xff);
  ADV_DATA[20] = (uint8_t)((calc_crc>>16)&0xff);
  
  whitening_init(38);
  whitening_decode(ADV_DATA, 21);
#endif
/****************************************************Device->Phone End**********************************************************/

	cms_spi_init();
	//cms_spi2_init();
	
	//rf_init_2();
	//rf_init();
  
  while(1)
  {
		//rf_send(ADV_DATA, 21);
		//rf_receive();
    //STM_EVAL_LEDOn(LED4);
    
#ifdef TEST_I2C
    //usart_putchar(0x01);
#endif
    
#if 0
    /* Check if the user button is pressed */
    if(STM_EVAL_PBGetState(BUTTON_USER)== SET)
    {
      /* BlinkSpeed: 1 -> 2 -> 0, then re-cycle */
      /* Turn on LD4 Blue LED during 1s each time User button is pressed */
      STM_EVAL_LEDOn(LED4);
      
      /* wait for 1s */
      Delay(1000);
      
      /* Turn off LD4 Blue LED after 1s each time User button is pressed */
      STM_EVAL_LEDOff(LED4);
      
      /* Increment the blink speed counter */
      BlinkSpeed++;
      
      /* Default value for blink speed counter */
      if(BlinkSpeed == 3)
      {  
        BlinkSpeed = 0;
      }
    }
    
    /* Test on blink speed */
    if(BlinkSpeed == 2)
    {
      /* LED3 toggles each 100 ms */
      STM_EVAL_LEDToggle(LED3);
      
      /* maintain LED3 status for 100ms */
      Delay(100);
    }
    else if(BlinkSpeed == 1)
    {
      /* LED3 toggles each 200 ms */
      STM_EVAL_LEDToggle(LED3);
      
      /* maintain LED3 status for 200ms */
      Delay(200);
    }
    else
    {  
      /* LED3 Off */
      STM_EVAL_LEDOff(LED3);
    }
#endif
  }
}

void EXTI0_Config(void)
{
  EXTI_InitTypeDef   EXTI_InitStructure;
  GPIO_InitTypeDef   GPIO_InitStructure;
  NVIC_InitTypeDef   NVIC_InitStructure;
  
  /* Enable GPIOA clock */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);

  /* Configure PA0 pin as input floating */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  /* Enable SYSCFG clock */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
  
  /* Connect EXTI0 Line to PA0 pin */
  SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource0);

  /* Configure EXTI0 line */
  EXTI_InitStructure.EXTI_Line = EXTI_Line0;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;//EXTI_Trigger_Falling;
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);

  /* Enable and set EXTI0 Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = EXTI0_1_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPriority = 0x00;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}

/**
  * @brief  Inserts a delay time.
  * @param  nTime: specifies the delay time length, in 1 ms.
  * @retval None
  */
void Delay(__IO uint32_t nTime)
{
  TimingDelay = nTime;

  while(TimingDelay != 0);
}

/**
  * @brief  Decrements the TimingDelay variable.
  * @param  None
  * @retval None
  */
void TimingDelay_Decrement(void)
{
  if (TimingDelay != 0x00)
  { 
    TimingDelay--;
  }
}

/*************************************************************CRC****************************************************************/
void crc_init(void)
{
	uint8_t i = 0;
	uint32_t crc = 0x00555555;
	
	for (i = 0; i < 24; i++)
	{
		crc_reg[i] = (crc >> i) & 0x00000001;
	}
}

uint32_t arr_to_crc(void)
{
	uint8_t i = 0;
	uint32_t output_data = 0;
	
	for (i = 0; i < 24; i++)
	{
		output_data |= (crc_reg[i] << (23 - i));
	}
	
	return output_data;
}

void crc_update(uint8_t input_data_bit)
{

	uint8_t temp = crc_reg[23] ^ input_data_bit;
	
	crc_reg[23] = crc_reg[22];
	crc_reg[22] = crc_reg[21];
	crc_reg[21] = crc_reg[20];
	crc_reg[20] = crc_reg[19];
	crc_reg[19] = crc_reg[18];
	crc_reg[18] = crc_reg[17];
	crc_reg[17] = crc_reg[16];
	crc_reg[16] = crc_reg[15];
	crc_reg[15] = crc_reg[14];
	crc_reg[14] = crc_reg[13];
	crc_reg[13] = crc_reg[12];
	crc_reg[12] = crc_reg[11];
	crc_reg[11] = crc_reg[10];
	crc_reg[10] = crc_reg[9] ^ temp;
	crc_reg[9] = crc_reg[8] ^ temp;
	crc_reg[8] = crc_reg[7];
	crc_reg[7] = crc_reg[6];
	crc_reg[6] = crc_reg[5] ^ temp;
	crc_reg[5] = crc_reg[4];
	crc_reg[4] = crc_reg[3] ^ temp;
	crc_reg[3] = crc_reg[2] ^ temp;
	crc_reg[2] = crc_reg[1];
	crc_reg[1] = crc_reg[0] ^ temp;
	crc_reg[0] = temp;
}

void check_crc(uint8_t *data, uint8_t length)
{
	uint8_t data_input, data_index, data_bit, bit_index;
	
	for (data_index = 0; data_index < length; data_index++)
	{
		data_input = data[data_index];
		data_bit = 0;
		
		for (bit_index = 0; bit_index < 8; bit_index++)
		{
			data_bit = (data_input >> (bit_index)) & 0x01;
			crc_update(data_bit);
		}
	}
}
/************************************************************CRC END************************************************************************/


#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *   where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {}
}
#endif

/**
  * @}
  */


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
