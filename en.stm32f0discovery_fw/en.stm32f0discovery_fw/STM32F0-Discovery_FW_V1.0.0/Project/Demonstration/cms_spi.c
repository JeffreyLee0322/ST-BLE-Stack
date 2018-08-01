#include "cms_spi.h"
#include "stm32f0xx_gpio.h"
#include "stm32f0xx_spi.h"
#include "stm32f0xx_rcc.h"

SPI_InitTypeDef  SPI_InitStructure;
SPI_InitTypeDef  SPI_InitStructure2;
//DMA_InitTypeDef  DMA_InitStructure;
//TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
//TIM_OCInitTypeDef  TIM_OCInitStructure;
uint8_t TxBuffer[] = {5};
//uint8_t RxBuffer [RXBUFFERSIZE];

/*
__IO JOYState_TypeDef PressedButton  = JOY_NONE;
__IO uint32_t CommandTransmitted = 0x00;
__IO uint32_t CommandReceived = 0x00;
__IO uint16_t NumberOfByte = 0x00;
__IO uint16_t PrescalerValue = 0;
__IO uint32_t TimeOut = 0x0;*/

static void SPI_Config(void);
static void SPI2_Config(void);
static void SPI_Delay(int time);

/**
  * @brief  Configures the SPI Peripheral.
  * @param  None
  * @retval None
  */
static void SPI_Config(void)
{
	//EXTI_InitTypeDef  EXTI_InitStructure;
  //NVIC_InitTypeDef  NVIC_InitStructure;
  GPIO_InitTypeDef 	GPIO_InitStructure;

  /* Enable the SPI peripheral */
  RCC_APB2PeriphClockCmd(SPIx_CLK, ENABLE);
  
  /* Enable the DMA peripheral */
  //RCC_AHBPeriphClockCmd(DMAx_CLK, ENABLE);
  
  /* Enable the TIM peripheral */
  //RCC_APB1PeriphClockCmd(TIMx_CLK, ENABLE);

  /* Enable SCK, MOSI, MISO and NSS GPIO clocks */
  RCC_AHBPeriphClockCmd(SPIx_SCK_GPIO_CLK | SPIx_MISO_GPIO_CLK | SPIx_MOSI_GPIO_CLK , ENABLE);
  
  /* Enable TIM DMA trigger clock */ 
  //RCC_AHBPeriphClockCmd(TIMx_TRIGGER_GPIO_CLK, ENABLE);
  
  /* SPI pin mappings */
  GPIO_PinAFConfig(SPIx_SCK_GPIO_PORT, SPIx_SCK_SOURCE, SPIx_SCK_AF);
  GPIO_PinAFConfig(SPIx_MOSI_GPIO_PORT, SPIx_MOSI_SOURCE, SPIx_MOSI_AF);
  GPIO_PinAFConfig(SPIx_MISO_GPIO_PORT, SPIx_MISO_SOURCE, SPIx_MISO_AF);
  //GPIO_PinAFConfig(SPIx_NSS_GPIO_PORT, SPIx_NSS_SOURCE, SPIx_NSS_AF);
  
  /* TIM capture compare pin mapping */
  /*GPIO_PinAFConfig(TIMx_TRIGGER_GPIO_PORT, TIMx_TRIGGER_SOURCE, TIMx_TRIGGER_AF);*/

  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_DOWN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_Level_3;

  /* SPI SCK pin configuration */
  GPIO_InitStructure.GPIO_Pin = SPIx_SCK_PIN;
  GPIO_Init(SPIx_SCK_GPIO_PORT, &GPIO_InitStructure);

  /* SPI  MOSI pin configuration */
  GPIO_InitStructure.GPIO_Pin =  SPIx_MOSI_PIN;
  GPIO_Init(SPIx_MOSI_GPIO_PORT, &GPIO_InitStructure);

  /* SPI MISO pin configuration */
  GPIO_InitStructure.GPIO_Pin = SPIx_MISO_PIN;
  GPIO_Init(SPIx_MISO_GPIO_PORT, &GPIO_InitStructure);

  /* SPI NSS pin configuration */
  //GPIO_InitStructure.GPIO_Pin = SPIx_NSS_PIN;//CSN
  //GPIO_Init(SPIx_NSS_GPIO_PORT, &GPIO_InitStructure);

  /* Configure the TIM channelx capture compare as DMA Trigger */
  //GPIO_InitStructure.GPIO_Pin =  TIMx_TRIGGER_PIN;
  //GPIO_Init(TIMx_TRIGGER_GPIO_PORT, &GPIO_InitStructure);

  /* SPI configuration -------------------------------------------------------*/
  SPI_I2S_DeInit(SPIx);
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
  SPI_Init(SPIx, &SPI_InitStructure);
	
	//SPI CSN PIN: PA15
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_Level_3;
  GPIO_InitStructure.GPIO_Pin = SPIx_NSS_PIN;
  GPIO_Init(SPIx_NSS_GPIO_PORT, &GPIO_InitStructure);
	//SPI CSN PIN Output High: NO SELECT
	GPIO_SetBits(SPIx_NSS_GPIO_PORT, SPIx_NSS_PIN);
	
	//SPI CE PIN: PB0
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_Level_3;
  GPIO_InitStructure.GPIO_Pin = SPIx_CE_PIN;
  GPIO_Init(SPIx_CE_GPIO_PORT, &GPIO_InitStructure);
	//SPI CE PIN Output High: Enable chip
	GPIO_ResetBits(SPIx_CE_GPIO_PORT, SPIx_CE_PIN);
	
	//PKT_IRQ PIN: PB1
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  //GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_DOWN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_Level_3;
  GPIO_InitStructure.GPIO_Pin = PKT1_IRQ_PIN;
  GPIO_Init(PKT1_IRQ__GPIO_PORT, &GPIO_InitStructure);
	
	//FIFO_IRQ PIN: PB2
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  //GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_DOWN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_Level_3;
  GPIO_InitStructure.GPIO_Pin = FIFO1_IRQ_PIN;
  GPIO_Init(FIFO1_IRQ_GPIO_PORT, &GPIO_InitStructure);

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
  SPI_Cmd(SPIx, ENABLE);
  
  /* Disenable NSS output for master mode */
  //SPI_SSOutputCmd(SPIx, DISABLE);
	//SPI_SSOutputCmd(SPIx, ENABLE);
}

static void SPI2_Config(void)
{
	EXTI_InitTypeDef  EXTI_InitStructure;
  NVIC_InitTypeDef  NVIC_InitStructure;
  GPIO_InitTypeDef GPIO_InitStructure;

  /* Enable the SPI peripheral */
  RCC_APB1PeriphClockCmd(SPI2x_CLK, ENABLE);

  /* Enable SCK, MOSI, MISO and NSS GPIO clocks */
  RCC_AHBPeriphClockCmd(SPI2x_SCK_GPIO_CLK | SPI2x_MISO_GPIO_CLK | SPI2x_MOSI_GPIO_CLK | SPI2x_NSS_GPIO_CLK , ENABLE);
	RCC_AHBPeriphClockCmd(GPIOC_PERIPHREAL_CLOCK, ENABLE);

  /* SPI pin mappings */
  GPIO_PinAFConfig(SPI2x_SCK_GPIO_PORT, SPI2x_SCK_SOURCE, SPI2x_SCK_AF);
  GPIO_PinAFConfig(SPI2x_MOSI_GPIO_PORT, SPI2x_MOSI_SOURCE, SPI2x_MOSI_AF);
  GPIO_PinAFConfig(SPI2x_MISO_GPIO_PORT, SPI2x_MISO_SOURCE, SPI2x_MISO_AF);
  GPIO_PinAFConfig(SPI2x_NSS_GPIO_PORT, SPI2x_NSS_SOURCE, SPI2x_NSS_AF);

  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_DOWN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_Level_3;

  /* SPI SCK pin configuration */
  GPIO_InitStructure.GPIO_Pin = SPI2x_SCK_PIN;
  GPIO_Init(SPI2x_SCK_GPIO_PORT, &GPIO_InitStructure);

  /* SPI  MOSI pin configuration */
  GPIO_InitStructure.GPIO_Pin =  SPI2x_MOSI_PIN;
  GPIO_Init(SPI2x_MOSI_GPIO_PORT, &GPIO_InitStructure);

  /* SPI MISO pin configuration */
  GPIO_InitStructure.GPIO_Pin = SPI2x_MISO_PIN;
  GPIO_Init(SPI2x_MISO_GPIO_PORT, &GPIO_InitStructure);

  /* SPI NSS pin configuration */
  GPIO_InitStructure.GPIO_Pin = SPI2x_NSS_PIN;
  GPIO_Init(SPI2x_NSS_GPIO_PORT, &GPIO_InitStructure);

  /* SPI configuration -------------------------------------------------------*/
  SPI_I2S_DeInit(SPI2x);
  SPI_InitStructure2.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
  SPI_InitStructure2.SPI_DataSize = SPI_DataSize_8b;
  SPI_InitStructure2.SPI_CPOL = SPI_CPOL_Low;
  SPI_InitStructure2.SPI_CPHA = SPI_CPHA_1Edge;
  SPI_InitStructure2.SPI_NSS = SPI_NSS_Soft;
  SPI_InitStructure2.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_16;
  SPI_InitStructure2.SPI_FirstBit = SPI_FirstBit_MSB;
  SPI_InitStructure2.SPI_CRCPolynomial = 7;
	
	/* Initializes the SPI communication */
  SPI_InitStructure2.SPI_Mode = SPI_Mode_Master;
  SPI_Init(SPI2x, &SPI_InitStructure2);
	
	//SPI2 CE PIN: PC7
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_Level_3;
  GPIO_InitStructure.GPIO_Pin = SPI2x_CE_PIN;
  GPIO_Init(SPI2x_CE_GPIO_PORT, &GPIO_InitStructure);
	//SPI CE PIN Output High: Enable chip
	GPIO_SetBits(SPI2x_CE_GPIO_PORT, SPI2x_CE_PIN);
	
	//PKT2_IRQ PIN: PC8
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  //GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_DOWN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_Level_3;
  GPIO_InitStructure.GPIO_Pin = PKT2_IRQ_PIN;
  GPIO_Init(PKT2_IRQ__GPIO_PORT, &GPIO_InitStructure);
	
	//FIFO2_IRQ PIN: PC9
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  //GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_DOWN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_Level_3;
  GPIO_InitStructure.GPIO_Pin = FIFO2_IRQ_PIN;
  GPIO_Init(FIFO2_IRQ_GPIO_PORT, &GPIO_InitStructure);
	
	//Config interrupt
	/* Connect EXTI8 Line to PC8 pin */
  SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOC, EXTI_PinSource8);
	/* Connect EXTI9 Line to PC9 pin */
  SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOC, EXTI_PinSource9);

  /* Configure EXTI8 line */
  EXTI_InitStructure.EXTI_Line = EXTI_Line8;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);
	
	/* Configure EXTI9 line */
  EXTI_InitStructure.EXTI_Line = EXTI_Line9;
  EXTI_Init(&EXTI_InitStructure);

  /* Enable and set EXTI4-15 Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = EXTI4_15_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPriority = 0x00;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
	
  /* Enable the SPI peripheral */
  SPI_Cmd(SPI2x, ENABLE);
  
  /* Enable NSS output for master mode */
  SPI_SSOutputCmd(SPI2x, ENABLE);
}

void cms_spi_init(void)
{
	SPI_Config();
	
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

void cms_spi2_init(void)
{
	SPI2_Config();
	
	/*while(1)
	{
			while(!((SPI2x->SR)&0x2));//Tx buffer not empty
			SPI_SendData8(SPI2x, 0x05);
			
			SPI_Delay(100);
	}*/
}

void SPI_CS_Enable_(void)
{
    /* Reset PA15 */
    GPIO_ResetBits(GPIOA, SPIx_NSS_PIN);
}

void SPI2_CS_Enable_(void)
{
    /* Reset PB12 */
    GPIO_ResetBits(GPIOB, SPI2x_NSS_PIN);
}

void SPI_CS_Disable_(void)
{
   /* Set PA15 */
   GPIO_SetBits(GPIOA, SPIx_NSS_PIN);
}

void SPI2_CS_Disable_(void)
{
   /* Set PB12 */
   GPIO_SetBits(GPIOB, SPI2x_NSS_PIN);
}

unsigned char SPI_WriteRead(unsigned short SendData)
{
	uint16_t data = 0;
	while(!((SPIx->SR)&0x2));//Tx buffer not empty
	//SPI_SendData8(SPIx, SendData);
	SPI_I2S_SendData16(SPIx, SendData);
	while(!((SPIx->SR)&0x1));//Rx buffer empty
	//return SPI_ReceiveData8(SPIx); 
	data = SPI_I2S_ReceiveData16(SPIx);
	return (uint8_t)(data>>8);
}

void SPI_Write(unsigned char SendData)
{
	while(!((SPIx->SR)&0x2));//Tx buffer not empty
	SPI_SendData8(SPIx, SendData);
}

unsigned char SPI_Read()
{
	while(!((SPIx->SR)&0x1));//Rx buffer empty
	return SPI_ReceiveData8(SPIx);
}

unsigned char SPI2_WriteRead(unsigned char SendData)
{
	while(!((SPI2x->SR)&0x2));//Tx buffer not empty
	SPI_SendData8(SPI2x, SendData);
	while(!((SPI2x->SR)&0x1));//Rx buffer empty
	return SPI_ReceiveData8(SPI2x);
}

void SPI_CE_SetHigh(void)
{
	//PB0
	GPIO_SetBits(GPIOB, SPIx_CE_PIN);
}

void SPI2_CE_SetHigh(void)
{
	//PC7
	GPIO_SetBits(GPIOC, SPI2x_CE_PIN);
}

void SPI_CE_SetLow(void)
{
	//PB0
	GPIO_ResetBits(GPIOB, SPIx_CE_PIN);
}

void SPI2_CE_SetLow(void)
{
	//PC7
	GPIO_ResetBits(GPIOC, SPI2x_CE_PIN);
}

static void SPI_Delay(int time)
{
  if (time != 0x00)
  {
    time--;
  }
}




