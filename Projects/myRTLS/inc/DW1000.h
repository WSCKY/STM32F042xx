#ifndef __DW1000_H
#define __DW1000_H

#include "stm32f0xx_spi.h"

/* USE//R_TIMEOUT value for waiting loops. This timeout is just guarantee that the
   application will not remain stuck if the USART communication is corrupted. 
   You may modify this timeout value depending on CPU frequency and application
   conditions (interrupts routines, number of data to transfer, baudrate, CPU
   frequency...). */ 
#define USER_TIMEOUT                    ((uint32_t)0x64) /* Waiting 1s */

/* Communication boards SPIx Interface */
#define _DW_SPI                             SPI1
#define _DW_SPI_CLK                         RCC_APB2Periph_SPI1
#define _DW_SPI_IRQn                        SPI1_IRQn
#define _DW_SPI_IRQHandler                  SPI1_IRQHandler

#define _DW_SPI_SCK_PIN                     GPIO_Pin_5                  /* PB.03 */
#define _DW_SPI_SCK_GPIO_PORT               GPIOA                       /* GPIOB */
#define _DW_SPI_SCK_GPIO_CLK                RCC_AHBPeriph_GPIOB
#define _DW_SPI_SCK_SOURCE                  GPIO_PinSource5
#define _DW_SPI_SCK_AF                      GPIO_AF_0

#define _DW_SPI_MISO_PIN                    GPIO_Pin_6                  /* PE.14 */
#define _DW_SPI_MISO_GPIO_PORT              GPIOA                       /* GPIOE */
#define _DW_SPI_MISO_GPIO_CLK               RCC_AHBPeriph_GPIOA
#define _DW_SPI_MISO_SOURCE                 GPIO_PinSource6
#define _DW_SPI_MISO_AF                     GPIO_AF_0

#define _DW_SPI_MOSI_PIN                    GPIO_Pin_7                  /* PE.15 */
#define _DW_SPI_MOSI_GPIO_PORT              GPIOA                       /* GPIOE */
#define _DW_SPI_MOSI_GPIO_CLK               RCC_AHBPeriph_GPIOA
#define _DW_SPI_MOSI_SOURCE                 GPIO_PinSource7
#define _DW_SPI_MOSI_AF                     GPIO_AF_0

#define SPI_DATASIZE                     SPI_DataSize_8b
#define SPI_DATAMASK                     (uint8_t)0xFF

#endif /* __DW1000_H */
