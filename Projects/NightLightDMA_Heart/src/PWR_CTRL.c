/**
  ******************************************************************************
  * @file    ./src/PWR_CTRL.c 
  * @author  kyChu
  * @version V1.0.0
  * @date    30-July-2018
  * @brief   Power Control GPIO Driver Module.
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "PWR_CTRL.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  set PWR_CTRL Driver Pin Initialization.
	* @param  None
	* @retval None
	*/
void PWR_CTRL_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	/* LED_GPIO Periph clock enable */
  RCC_AHBPeriphClockCmd(PWR_GPIO_PORT_CLK, ENABLE);

  /* Configure LED_GPIO_Pin in output pushpull mode */
  GPIO_InitStructure.GPIO_Pin = PWR_GPIO_PORT_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_Init(PWR_GPIO_PORT, &GPIO_InitStructure);
	HW_PWR_OFF();
}

/******************************** END OF FILE *********************************/
