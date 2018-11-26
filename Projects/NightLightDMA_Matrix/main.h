/**
  ******************************************************************************
  * @file    ./main.h 
  * @author  kyChu
  * @version V1.0.0
  * @date    17-April-2018
  * @brief   Header for main.c module.
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

/* Includes ------------------------------------------------------------------*/
#include "Delay.h"
#include "PWR_CTRL.h"
#include "UserButton.h"
#include "TimerCounter.h"

#include "WS2812B.h"

/* Exported types ------------------------------------------------------------*/
typedef union {
	float fData;
	uint8_t uData[4];
} _FLOAT_UNION;
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

#endif /* __MAIN_H */

/******************************** END OF FILE *********************************/
