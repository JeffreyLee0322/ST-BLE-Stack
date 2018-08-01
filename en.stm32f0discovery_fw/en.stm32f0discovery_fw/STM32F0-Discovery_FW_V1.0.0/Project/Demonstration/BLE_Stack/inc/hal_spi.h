#ifndef HAL_SPI_H
#define HAL_SPI_H

#include "userConfig.h"
#include "stm32f0_discovery.h"

#define BLE_SPIx_DR_ADDRESS                  0x4001300C

#define BLE_SPIx                             SPI1
#define BLE_SPIx_CLK                         RCC_APB2Periph_SPI1
#define BLE_SPIx_IRQn                        SPI1_IRQn
#define BLE_SPIx_IRQHandler                  SPI1_IRQHandler

#define BLE_SPIx_SCK_PIN                     GPIO_Pin_5
#define BLE_SPIx_SCK_GPIO_PORT               GPIOA
#define BLE_SPIx_SCK_GPIO_CLK                RCC_AHBPeriph_GPIOA
#define BLE_SPIx_SCK_SOURCE                  GPIO_PinSource5
#define BLE_SPIx_SCK_AF                      GPIO_AF_0

#define BLE_SPIx_MISO_PIN                    GPIO_Pin_4
#define BLE_SPIx_MISO_GPIO_PORT              GPIOB
#define BLE_SPIx_MISO_GPIO_CLK               RCC_AHBPeriph_GPIOB
#define BLE_SPIx_MISO_SOURCE                 GPIO_PinSource4
#define BLE_SPIx_MISO_AF                     GPIO_AF_0

#define BLE_SPIx_MOSI_PIN                    GPIO_Pin_7
#define BLE_SPIx_MOSI_GPIO_PORT              GPIOA
#define BLE_SPIx_MOSI_GPIO_CLK               RCC_AHBPeriph_GPIOA
#define BLE_SPIx_MOSI_SOURCE                 GPIO_PinSource7
#define BLE_SPIx_MOSI_AF                     GPIO_AF_0

#define BLE_SPIx_NSS_PIN                     GPIO_Pin_15
#define BLE_SPIx_NSS_GPIO_PORT               GPIOA
#define BLE_SPIx_NSS_GPIO_CLK                RCC_AHBPeriph_GPIOA
#define BLE_SPIx_NSS_SOURCE                  GPIO_PinSource15
#define BLE_SPIx_NSS_AF                      GPIO_AF_0



//#define SPI_DATASIZE                     SPI_DataSize_8b
#define BLE_SPI_DATAMASK                     (uint8_t)0xFF


#define BLE_TXBUFFERSIZE                     (countof(TxBuffer) - 1)
#define BLE_RXBUFFERSIZE                     TXBUFFERSIZE

#define BLE_SPIx_CE_PIN                      GPIO_Pin_0
#define BLE_SPIx_CE_GPIO_PORT                GPIOB
#define BLE_PKT1_IRQ_PIN                     GPIO_Pin_1
#define BLE_PKT1_IRQ__GPIO_PORT              GPIOB
#define BLE_FIFO1_IRQ_PIN                    GPIO_Pin_2
#define BLE_FIFO1_IRQ_GPIO_PORT              GPIOB


void hal_spi_init(void);
void BLE_SPI_CS_Enable_(void);
void BLE_SPI_CS_Disable_(void);
//unsigned char SPI_WriteRead(unsigned char SendData);
unsigned char BLE_SPI_WriteRead(unsigned short SendData);
void BLE_SPI_CE_SetHigh(void);
void BLE_SPI_CE_SetLow(void);
#endif

