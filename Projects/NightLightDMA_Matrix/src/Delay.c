/**
  ******************************************************************************
  * @file    ./src/Delay.c 
  * @author  kyChu
  * @version V1.0.0
  * @date    17-April-2018
  * @brief   Delay Module Driver.
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "Delay.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static __IO uint32_t TimingDelay;
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Configure System tick timer update per 1ms.
	* @param  None
	* @retval None
	*/
void Delay_Init(void)
{
	if(SysTick_Config(SystemCoreClock / 1000)) while(1);
}

/**
  * @brief  Insert any ticks in program.
  * @param  ticks: time ticks to delay. (unit: ms)
  * @retval None
  */
void DelayTicks(__IO uint32_t nTime)
{
	TimingDelay = nTime;
	while(TimingDelay != 0);
}

/**
  * @brief  Decrements the TimingDelay variable.
  * @param  None
  * @retval None
  */
void TimingDelay_Decrement(void)
{
  if (TimingDelay != 0x00)
  { 
    TimingDelay--;
  }
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
	TimingDelay_Decrement();
	SysTickIrqCallback();
}

/******************************** END OF FILE *********************************/
