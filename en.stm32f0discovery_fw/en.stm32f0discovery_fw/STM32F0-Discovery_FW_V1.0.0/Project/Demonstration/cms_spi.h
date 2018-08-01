#ifndef CMS_SPI_H
#define CMS_SPI_H

#include "stm32f0xx.h"
#include "stm32f0_discovery.h"

#define SPIx_DR_ADDRESS                  0x4001300C
/*#define SPIx_TX_DMA_CHANNEL              DMA1_Channel3
#define SPIx_TX_DMA_FLAG_TC              DMA1_FLAG_TC3
#define SPIx_TX_DMA_FLAG_GL              DMA1_FLAG_GL3
#define SPIx_RX_DMA_CHANNEL              DMA1_Channel2
#define SPIx_RX_DMA_FLAG_TC              DMA1_FLAG_TC2
#define SPIx_RX_DMA_FLAG_GL              DMA1_FLAG_GL2

#define DMAx_CLK                         RCC_AHBPeriph_DMA1*/

#define SPIx                             SPI1
#define SPIx_CLK                         RCC_APB2Periph_SPI1
#define SPIx_IRQn                        SPI1_IRQn
#define SPIx_IRQHandler                  SPI1_IRQHandler

#define SPIx_SCK_PIN                     GPIO_Pin_5
#define SPIx_SCK_GPIO_PORT               GPIOA
#define SPIx_SCK_GPIO_CLK                RCC_AHBPeriph_GPIOA
#define SPIx_SCK_SOURCE                  GPIO_PinSource5
#define SPIx_SCK_AF                      GPIO_AF_0

#define SPIx_MISO_PIN                    GPIO_Pin_4
#define SPIx_MISO_GPIO_PORT              GPIOB
#define SPIx_MISO_GPIO_CLK               RCC_AHBPeriph_GPIOB
#define SPIx_MISO_SOURCE                 GPIO_PinSource4
#define SPIx_MISO_AF                     GPIO_AF_0

#define SPIx_MOSI_PIN                    GPIO_Pin_7
#define SPIx_MOSI_GPIO_PORT              GPIOA
#define SPIx_MOSI_GPIO_CLK               RCC_AHBPeriph_GPIOA
#define SPIx_MOSI_SOURCE                 GPIO_PinSource7
#define SPIx_MOSI_AF                     GPIO_AF_0


#define SPIx_NSS_PIN                     GPIO_Pin_15
#define SPIx_NSS_GPIO_PORT               GPIOA
#define SPIx_NSS_GPIO_CLK                RCC_AHBPeriph_GPIOA
#define SPIx_NSS_SOURCE                  GPIO_PinSource15
#define SPIx_NSS_AF                      GPIO_AF_0





#define SPI2x                             SPI2
#define SPI2x_CLK                         RCC_APB1Periph_SPI2
#define SPI2x_IRQn                        SPI2_IRQn
#define SPI2x_IRQHandler                  SPI2_IRQHandler

#define SPI2x_SCK_PIN                     GPIO_Pin_13
#define SPI2x_SCK_GPIO_PORT               GPIOB
#define SPI2x_SCK_GPIO_CLK                RCC_AHBPeriph_GPIOB
#define SPI2x_SCK_SOURCE                  GPIO_PinSource13
#define SPI2x_SCK_AF                      GPIO_AF_0

#define SPI2x_MISO_PIN                    GPIO_Pin_14
#define SPI2x_MISO_GPIO_PORT              GPIOB
#define SPI2x_MISO_GPIO_CLK               RCC_AHBPeriph_GPIOB
#define SPI2x_MISO_SOURCE                 GPIO_PinSource14
#define SPI2x_MISO_AF                     GPIO_AF_0

#define SPI2x_MOSI_PIN                    GPIO_Pin_15
#define SPI2x_MOSI_GPIO_PORT              GPIOB
#define SPI2x_MOSI_GPIO_CLK               RCC_AHBPeriph_GPIOB
#define SPI2x_MOSI_SOURCE                 GPIO_PinSource15
#define SPI2x_MOSI_AF                     GPIO_AF_0

#define SPI2x_NSS_PIN                     GPIO_Pin_12
#define SPI2x_NSS_GPIO_PORT               GPIOB
#define SPI2x_NSS_GPIO_CLK                RCC_AHBPeriph_GPIOB
#define SPI2x_NSS_SOURCE                  GPIO_PinSource12
#define SPI2x_NSS_AF                      GPIO_AF_0

//#define SPI_DATASIZE                     SPI_DataSize_8b
#define SPI_DATAMASK                     (uint8_t)0xFF


/* TIM Trigger Configuration */
/*#define TIMx                             TIM2
#define TIMx_CLK                         RCC_APB1Periph_TIM2
#define TIMx_DMA_CHANNEL                 TIM_DMA_CC2

#define TIMx_TRIGGER_PIN                 GPIO_Pin_1
#define TIMx_TRIGGER_GPIO_PORT           GPIOA
#define TIMx_TRIGGER_GPIO_CLK            RCC_AHBPeriph_GPIOA
#define TIMx_TRIGGER_SOURCE              GPIO_PinSource1
#define TIMx_TRIGGER_AF                  GPIO_AF_2
#define TIMx_CHANNEL_INIT                TIM_OC2Init
#define TIMx_CHANNEL_DMA_CHANNEL         DMA1_Channel3
#define TIMx_CHANNEL_DMA_FLAG_TC         DMA1_FLAG_TC3
#define TIMx_CHANNEL_DMA_FLAG_GL         DMA1_FLAG_GL3*/

#define TXBUFFERSIZE                     (countof(TxBuffer) - 1)
#define RXBUFFERSIZE                     TXBUFFERSIZE


#define GPIOA_PERIPHREAL_CLOCK           RCC_AHBPeriph_GPIOA
#define GPIOB_PERIPHREAL_CLOCK           RCC_AHBPeriph_GPIOB
#define GPIOC_PERIPHREAL_CLOCK           RCC_AHBPeriph_GPIOC


#define SPIx_CE_PIN                      GPIO_Pin_0
#define SPIx_CE_GPIO_PORT                GPIOB
#define PKT1_IRQ_PIN                     GPIO_Pin_1
#define PKT1_IRQ__GPIO_PORT              GPIOB
#define FIFO1_IRQ_PIN                    GPIO_Pin_2
#define FIFO1_IRQ_GPIO_PORT              GPIOB


#define SPI2x_CE_PIN                     GPIO_Pin_7
#define SPI2x_CE_GPIO_PORT               GPIOC
#define PKT2_IRQ_PIN                     GPIO_Pin_8
#define PKT2_IRQ__GPIO_PORT              GPIOC
#define FIFO2_IRQ_PIN                    GPIO_Pin_9
#define FIFO2_IRQ_GPIO_PORT              GPIOC



void cms_spi_init(void);
void SPI_CS_Enable_(void);
void SPI_CS_Disable_(void);
//unsigned char SPI_WriteRead(unsigned char SendData);
unsigned char SPI_WriteRead(unsigned short SendData);
void SPI_CE_SetHigh(void);
void SPI_CE_SetLow(void);

void cms_spi2_init(void);
void SPI2_CS_Enable_(void);
void SPI2_CS_Disable_(void);
unsigned char SPI2_WriteRead(unsigned char SendData);
void SPI2_CE_SetHigh(void);
void SPI2_CE_SetLow(void);

void SPI_Write(unsigned char SendData);
unsigned char SPI_Read(void);


#endif

