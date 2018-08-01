#include "hal_spi.h"
#include "stm32f0xx_gpio.h"
#include "stm32f0xx_spi.h"
#include "stm32f0xx_rcc.h"


static void BLE_SPI_Config(void);
static void BLE_SPI_Delay(int time);

/**
  * @brief  Configures the SPI Peripheral.
  * @param  None
  * @retval None
  */
static void BLE_SPI_Config(void)
{
	//EXTI_InitTypeDef  EXTI_InitStructure;
  //NVIC_InitTypeDef  NVIC_InitStructure;
  GPIO_InitTypeDef 	GPIO_InitStructure;
	SPI_InitTypeDef  SPI_InitStructure;

  /* Enable the SPI peripheral */
  RCC_APB2PeriphClockCmd(BLE_SPIx_CLK, ENABLE);
  
  /* Enable the DMA peripheral */
  //RCC_AHBPeriphClockCmd(DMAx_CLK, ENABLE);
  
  /* Enable the TIM peripheral */
  //RCC_APB1PeriphClockCmd(TIMx_CLK, ENABLE);

  /* Enable SCK, MOSI, MISO and NSS GPIO clocks */
  RCC_AHBPeriphClockCmd(BLE_SPIx_SCK_GPIO_CLK | BLE_SPIx_MISO_GPIO_CLK | BLE_SPIx_MOSI_GPIO_CLK , ENABLE);
  
  /* Enable TIM DMA trigger clock */ 
  //RCC_AHBPeriphClockCmd(TIMx_TRIGGER_GPIO_CLK, ENABLE);
  
  /* SPI pin mappings */
  GPIO_PinAFConfig(BLE_SPIx_SCK_GPIO_PORT, BLE_SPIx_SCK_SOURCE, BLE_SPIx_SCK_AF);
  GPIO_PinAFConfig(BLE_SPIx_MOSI_GPIO_PORT, BLE_SPIx_MOSI_SOURCE, BLE_SPIx_MOSI_AF);
  GPIO_PinAFConfig(BLE_SPIx_MISO_GPIO_PORT, BLE_SPIx_MISO_SOURCE, BLE_SPIx_MISO_AF);
  //GPIO_PinAFConfig(SPIx_NSS_GPIO_PORT, SPIx_NSS_SOURCE, SPIx_NSS_AF);
  
  /* TIM capture compare pin mapping */
  /*GPIO_PinAFConfig(TIMx_TRIGGER_GPIO_PORT, TIMx_TRIGGER_SOURCE, TIMx_TRIGGER_AF);*/

  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_DOWN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_Level_3;

  /* SPI SCK pin configuration */
  GPIO_InitStructure.GPIO_Pin = BLE_SPIx_SCK_PIN;
  GPIO_Init(BLE_SPIx_SCK_GPIO_PORT, &GPIO_InitStructure);

  /* SPI  MOSI pin configuration */
  GPIO_InitStructure.GPIO_Pin =  BLE_SPIx_MOSI_PIN;
  GPIO_Init(BLE_SPIx_MOSI_GPIO_PORT, &GPIO_InitStructure);

  /* SPI MISO pin configuration */
  GPIO_InitStructure.GPIO_Pin = BLE_SPIx_MISO_PIN;
  GPIO_Init(BLE_SPIx_MISO_GPIO_PORT, &GPIO_InitStructure);

  /* SPI NSS pin configuration */
  //GPIO_InitStructure.GPIO_Pin = SPIx_NSS_PIN;//CSN
  //GPIO_Init(SPIx_NSS_GPIO_PORT, &GPIO_InitStructure);

  /* Configure the TIM channelx capture compare as DMA Trigger */
  //GPIO_InitStructure.GPIO_Pin =  TIMx_TRIGGER_PIN;
  //GPIO_Init(TIMx_TRIGGER_GPIO_PORT, &GPIO_InitStructure);

  /* SPI configuration -------------------------------------------------------*/
  SPI_I2S_DeInit(BLE_SPIx);
  SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
  SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
  SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
  SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
  SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
  SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_8;
  SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
  SPI_InitStructure.SPI_CRCPolynomial = 7;

  /* DMA Configuration -------------------------------------------------------*/
  /*DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
  DMA_InitStructure.DMA_MemoryDataSize =  DMA_MemoryDataSize_Byte;
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
  DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
  DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;*/

	/* Initializes the SPI communication */
  SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
  SPI_Init(BLE_SPIx, &SPI_InitStructure);
	
	//SPI CSN PIN: PA15
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_Level_3;
  GPIO_InitStructure.GPIO_Pin = BLE_SPIx_NSS_PIN;
  GPIO_Init(BLE_SPIx_NSS_GPIO_PORT, &GPIO_InitStructure);
	//SPI CSN PIN Output High: NO SELECT
	GPIO_SetBits(BLE_SPIx_NSS_GPIO_PORT, BLE_SPIx_NSS_PIN);
	
	//SPI CE PIN: PB0
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_Level_3;
  GPIO_InitStructure.GPIO_Pin = BLE_SPIx_CE_PIN;
  GPIO_Init(BLE_SPIx_CE_GPIO_PORT, &GPIO_InitStructure);
	//SPI CE PIN Output High: Enable chip
	GPIO_ResetBits(BLE_SPIx_CE_GPIO_PORT, BLE_SPIx_CE_PIN);
	
	//PKT_IRQ PIN: PB1
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  //GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_DOWN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_Level_3;
  GPIO_InitStructure.GPIO_Pin = BLE_PKT1_IRQ_PIN;
  GPIO_Init(BLE_PKT1_IRQ__GPIO_PORT, &GPIO_InitStructure);
	
	//FIFO_IRQ PIN: PB2
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  //GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_DOWN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_Level_3;
  GPIO_InitStructure.GPIO_Pin = BLE_FIFO1_IRQ_PIN;
  GPIO_Init(BLE_FIFO1_IRQ_GPIO_PORT, &GPIO_InitStructure);

#if 0
	//Config interrupt
	/* Connect EXTI0 Line to PB1 pin */
  SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOB, EXTI_PinSource1);
	/* Connect EXTI2 Line to PB2 pin */
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOB, EXTI_PinSource2);

  /* Configure EXTI0 line */
  EXTI_InitStructure.EXTI_Line = EXTI_Line0;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);

  /* Enable and set EXTI0 Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = EXTI0_1_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPriority = 0x00;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
#endif

	/* Initialize the FIFO threshold */
  //SPI_RxFIFOThresholdConfig(SPIx, SPI_RxFIFOThreshold_QF);
  
  /* TIM configuration ------------------------------------------------------*/
  //TIM_Config(); 
  
  /* Enable the SPI peripheral */
  SPI_Cmd(BLE_SPIx, ENABLE);
  
  /* Disenable NSS output for master mode */
  //SPI_SSOutputCmd(SPIx, DISABLE);
	//SPI_SSOutputCmd(SPIx, ENABLE);
}

void hal_spi_init(void)
{
	BLE_SPI_Config();
	
  /* TIM Capture Compare DMA Request enable */
  //TIM_DMACmd(TIMx, TIMx_DMA_CHANNEL, ENABLE);
	
	/* DMA channel Rx of SPI Configuration */
  /*DMA_InitStructure.DMA_BufferSize = (uint16_t)1;
  DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)SPIx_DR_ADDRESS;
  DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t) &CommandReceived;
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
  DMA_InitStructure.DMA_Priority = DMA_Priority_High;
  DMA_Init(SPIx_RX_DMA_CHANNEL, &DMA_InitStructure);*/
		
	/* Enable the SPI Rx DMA request */
  //SPI_I2S_DMACmd(SPIx, SPI_I2S_DMAReq_Rx, ENABLE);
    
  //CommandTransmitted = 0x00;
  //CommandReceived = 0x00;
    
  /* Clear the RxBuffer */
  //Fill_Buffer(RxBuffer, TXBUFFERSIZE);
		
	/* Enable the DMA channel */
  //DMA_Cmd(SPIx_RX_DMA_CHANNEL, ENABLE);
	/*
	while(1)
	{
			//while(!((SPIx->SR)&0x2));//Tx buffer not empty
			//SPI_SendData8(SPIx, 0x05);
			SPI_CS_Enable_();
			while(!((SPIx->SR)&0x2));//Tx buffer not empty
			SPI_SendData8(SPIx, 0x05);
			//SPI_Delay(500);
			SPI_CS_Disable_();
			SPI_Delay(5000);
	}*/
}

void BLE_SPI_CS_Enable_(void)
{
    /* Reset PA15 */
    GPIO_ResetBits(GPIOA, BLE_SPIx_NSS_PIN);
}

void BLE_SPI_CS_Disable_(void)
{
   /* Set PA15 */
   GPIO_SetBits(GPIOA, BLE_SPIx_NSS_PIN);
}

unsigned char BLE_SPI_WriteRead(unsigned short SendData)
{
	uint16_t data = 0;
	while(!((BLE_SPIx->SR)&0x2));//Tx buffer not empty
	//SPI_SendData8(SPIx, SendData);
	SPI_I2S_SendData16(BLE_SPIx, SendData);
	while(!((BLE_SPIx->SR)&0x1));//Rx buffer empty
	//return SPI_ReceiveData8(SPIx); 
	data = SPI_I2S_ReceiveData16(BLE_SPIx);
	return (uint8_t)(data>>8);
}

void BLE_SPI_Write(unsigned char SendData)
{
	while(!((BLE_SPIx->SR)&0x2));//Tx buffer not empty
	SPI_SendData8(BLE_SPIx, SendData);
}

unsigned char BLE_SPI_Read(void)
{
	while(!((BLE_SPIx->SR)&0x1));//Rx buffer empty
	return SPI_ReceiveData8(BLE_SPIx);
}

void BLE_SPI_CE_SetHigh(void)
{
	//PB0
	GPIO_SetBits(GPIOB, BLE_SPIx_CE_PIN);
}

void BLE_SPI_CE_SetLow(void)
{
	//PB0
	GPIO_ResetBits(GPIOB, BLE_SPIx_CE_PIN);
}

static void BLE_SPI_Delay(int time)
{
  if (time != 0x00)
  {
    time--;
  }
}


