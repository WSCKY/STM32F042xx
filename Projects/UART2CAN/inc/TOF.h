/**
  ******************************************************************************
  * @file    ./inc/TOF.h 
  * @author  kyChu
  * @version V1.0.0
  * @date    17-April-2018
  * @brief   Header for TOF.c module.
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __TOF_H
#define __TOF_H

/* Includes ------------------------------------------------------------------*/
#include "CRC16.h"
#include "stm32f0xx_rcc.h"
#include "stm32f0xx_misc.h"
#include "stm32f0xx_gpio.h"
#include "stm32f0xx_usart.h"

/* Exported constants --------------------------------------------------------*/
#define TOF_USART                    USART1
#define TOF_USART_GPIO               GPIOB
#define TOF_USART_AF                 GPIO_AF_0
#define TOF_USART_GPIO_CLK           RCC_AHBPeriph_GPIOB
#define TOF_USART_CLK                RCC_APB2Periph_USART1
#define TOF_USART_RxPin              GPIO_Pin_7
#define TOF_USART_TxPin              GPIO_Pin_6
#define TOF_USART_RxPin_AF           GPIO_PinSource7
#define TOF_USART_TxPin_AF           GPIO_PinSource6
#define TOF_USART_IRQn               USART1_IRQn
#define TOF_USART_IRQHandler         USART1_IRQHandler

/* Exported types ------------------------------------------------------------*/
#if defined(__CC_ARM)
__packed typedef struct
#elif defined(__GNUC__)
__attribute__((packed)) typedef struct
#else
typedef struct
#endif
{
	uint8_t Header1;
	uint8_t Header2;
	uint8_t HighByte;
	uint8_t LowByte;
	uint8_t HighCRC;
	uint8_t LowCRC;
} TOF_ORIGIN_DATA;

/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void TOFDriverInit(void);
uint8_t GetNewTOFData(float *d);

#endif /* __TOF_H */

/******************************** END OF FILE *********************************/
