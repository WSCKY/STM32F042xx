/**
  ******************************************************************************
  * @file    ./inc/UserButton.h 
  * @author  kyChu
  * @version V1.0.0
  * @date    30-July-2018
  * @brief   Header for UserButton.c module.
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __USERBUTTON_H
#define __USERBUTTON_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f0xx.h"
#include "stm32f0xx_gpio.h"
#include "stm32f0xx_rcc.h"

/* Exported types ------------------------------------------------------------*/
typedef enum {
	PWR_KEY = 0,
} ButtonID;
/* Exported constants --------------------------------------------------------*/
#define BUTTON_NUMBER                  1

#define BUTTON_GROUP_PORT              { GPIOB }
#define BUTTON_GROUP_PIN               { GPIO_Pin_1 }
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void UserButtonInit(void);
uint32_t ReadButtonStatus(ButtonID id);

#endif /* __USERBUTTON_H */
