/**
  ******************************************************************************
  * @file    ./inc/LED.h 
  * @author  kyChu
  * @version V1.0.0
  * @date    17-April-2018
  * @brief   Header for LED.c module.
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __LED_H
#define __LED_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f0xx.h"
#include "stm32f0xx_gpio.h"
#include "stm32f0xx_rcc.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
#define LED_GPIO_PORT                  GPIOB
#define LED_GPIO_PORT_PIN              GPIO_Pin_0
#define LED_GPIO_PORT_CLK              RCC_AHBPeriph_GPIOB

/* Exported macro ------------------------------------------------------------*/
#define LED_ON()                       do { LED_GPIO_PORT->BRR = LED_GPIO_PORT_PIN; } while(0);
#define LED_OFF()                      do { LED_GPIO_PORT->BSRR = LED_GPIO_PORT_PIN; } while(0);
#define LED_TOG()                      do { LED_GPIO_PORT->ODR ^= LED_GPIO_PORT_PIN; } while(0);

/* Exported functions ------------------------------------------------------- */
void LED_Init(void);

#endif /* __LED_H */

/******************************** END OF FILE *********************************/
