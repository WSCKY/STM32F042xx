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

#define _DW_SPI_SCK_PIN                     GPIO_Pin_5                  /* PA.05 */
#define _DW_SPI_SCK_GPIO_PORT               GPIOA                       /* GPIOA */
#define _DW_SPI_SCK_GPIO_CLK                RCC_AHBPeriph_GPIOB
#define _DW_SPI_SCK_SOURCE                  GPIO_PinSource5
#define _DW_SPI_SCK_AF                      GPIO_AF_0

#define _DW_SPI_MISO_PIN                    GPIO_Pin_6                  /* PA.06 */
#define _DW_SPI_MISO_GPIO_PORT              GPIOA                       /* GPIOA */
#define _DW_SPI_MISO_GPIO_CLK               RCC_AHBPeriph_GPIOA
#define _DW_SPI_MISO_SOURCE                 GPIO_PinSource6
#define _DW_SPI_MISO_AF                     GPIO_AF_0

#define _DW_SPI_MOSI_PIN                    GPIO_Pin_7                  /* PA.07 */
#define _DW_SPI_MOSI_GPIO_PORT              GPIOA                       /* GPIOA */
#define _DW_SPI_MOSI_GPIO_CLK               RCC_AHBPeriph_GPIOA
#define _DW_SPI_MOSI_SOURCE                 GPIO_PinSource7
#define _DW_SPI_MOSI_AF                     GPIO_AF_0

#define _DW_SPI_NSS_PIN                     GPIO_Pin_4                  /* PA.04 */
#define _DW_SPI_NSS_GPIO_PORT               GPIOA
#define _DW_SPI_NSS_GPIO_CLK                RCC_AHBPeriph_GPIOA

#define _DW1000_RST_PIN                     GPIO_Pin_3                  /* PB.03 */
#define _DW1000_RST_GPIO_PORT               GPIOB
#define _DW1000_RST_GPIO_CLK                RCC_AHBPeriph_GPIOB

#define _DW1000_WAKEUP_PIN                  GPIO_Pin_4                  /* PB.04 */
#define _DW1000_WAKEUP_GPIO_PORT            GPIOB
#define _DW1000_WAKEUP_GPIO_CLK             RCC_AHBPeriph_GPIOB

#define _DW1000_IRQ_PIN                     GPIO_Pin_5                  /* PB.05 */
#define _DW1000_IRQ_GPIO_PORT               GPIOB
#define _DW1000_IRQ_GPIO_CLK                RCC_AHBPeriph_GPIOB
#define _DW1000_IRQ_GPIO_PortSource         EXTI_PortSourceGPIOB
#define _DW1000_IRQ_GPIO_PinSource          EXTI_PinSource5
#define _DW1000_IRQ_EXTI_LINE               EXTI_Line5
#define _DW1000_IRQ_EXTI_IRQn               EXTI4_15_IRQn
#define _DW1000_IRQ_EXTI_IRQHandler         EXTI4_15_IRQHandler

#define _DW_SPI_NSS_ENABLE()                (_DW_SPI_NSS_GPIO_PORT->BRR = _DW_SPI_NSS_PIN)
#define _DW_SPI_NSS_DISABLE()               (_DW_SPI_NSS_GPIO_PORT->BSRR = _DW_SPI_NSS_PIN)

#define _DW1000_WAKEUP_ENABLE()             (_DW1000_WAKEUP_GPIO_PORT->BRR = _DW1000_WAKEUP_PIN)

#define SPI_DATASIZE                        SPI_DataSize_8b
#define SPI_DATAMASK                        (uint8_t)0xFF

#define DW1000_SPI_HIGH                SPI_BaudRatePrescaler_4   /* 48 / 4 = 12MHz */
#define DW1000_SPI_LOW                 SPI_BaudRatePrescaler_16  /* 48 / 16 = 3MHz */

typedef void (*_DW1000_IRQnCallback)(void);

void DW1000_If_Init(void);
void DW1000RSTn_NOP_IN(void);
void DW1000RSTn_DrvLow(void);
void SPI_ConfigFastRate(uint32_t scalingfactor);
void DW1000_SetIRQnHandler(_DW1000_IRQnCallback pf);
int DW1000_SPI_Read(uint16_t headerLength, const uint8_t *headerBuffer, uint32_t readlength, uint8_t *readBuffer);
int DW1000_SPI_Write(uint16_t headerLength, const uint8_t *headerBuffer, uint32_t bodyLength, const uint8_t *bodyBuffer);


#endif /* __DW1000_H */
