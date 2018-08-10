/**
  ******************************************************************************
  * @file    ./inc/PWR_CTRL.h 
  * @author  kyChu
  * @version V1.0.0
  * @date    30-July-2018
  * @brief   Header for PWR_CTRL.c module.
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __PWR_CTRL_H
#define __PWR_CTRL_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f0xx.h"
#include "stm32f0xx_gpio.h"
#include "stm32f0xx_rcc.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
#define PWR_GPIO_PORT                  GPIOB
#define PWR_GPIO_PORT_PIN              GPIO_Pin_0
#define PWR_GPIO_PORT_CLK              RCC_AHBPeriph_GPIOB

/* Exported macro ------------------------------------------------------------*/
#define HW_PWR_ON()                    do { PWR_GPIO_PORT->BSRR = PWR_GPIO_PORT_PIN; } while(0);
#define HW_PWR_OFF()                   do { PWR_GPIO_PORT->BRR = PWR_GPIO_PORT_PIN; } while(0);

/* Exported functions ------------------------------------------------------- */
void PWR_CTRL_Init(void);

#endif /* __PWR_CTRL_H */

/******************************** END OF FILE *********************************/
