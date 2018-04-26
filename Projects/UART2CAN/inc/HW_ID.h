/**
  ******************************************************************************
  * @file    ./inc/HW_ID.h 
  * @author  kyChu
  * @version V1.0.0
  * @date    17-April-2018
  * @brief   Header for HW_ID.c module.
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __HW_ID_H
#define __HW_ID_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f0xx.h"
#include "stm32f0xx_gpio.h"
#include "stm32f0xx_rcc.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
#define ID_GPIO_PORT_1                 GPIOA
#define ID_GPIO_PORT_PIN_1             GPIO_Pin_5
#define ID_GPIO_PORT_CLK_1             RCC_AHBPeriph_GPIOA

#define ID_GPIO_PORT_2                 GPIOA
#define ID_GPIO_PORT_PIN_2             GPIO_Pin_6
#define ID_GPIO_PORT_CLK_2             RCC_AHBPeriph_GPIOA

#define ID_GPIO_PORT_3                 GPIOA
#define ID_GPIO_PORT_PIN_3             GPIO_Pin_7
#define ID_GPIO_PORT_CLK_3             RCC_AHBPeriph_GPIOA

#define ID_GPIO_PORT_4                 GPIOB
#define ID_GPIO_PORT_PIN_4             GPIO_Pin_0
#define ID_GPIO_PORT_CLK_4             RCC_AHBPeriph_GPIOB

#define ID_GPIO_PORT_5                 GPIOB
#define ID_GPIO_PORT_PIN_5             GPIO_Pin_1
#define ID_GPIO_PORT_CLK_5             RCC_AHBPeriph_GPIOB

/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void HW_ID_Init(void);
uint8_t ReadHW_ID(void);

#endif /* __HW_ID_H */

/******************************** END OF FILE *********************************/
