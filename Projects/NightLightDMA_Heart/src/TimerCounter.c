/**
  ******************************************************************************
  * @file    ./src/TimerCounter.c 
  * @author  kyChu
  * @version V1.0.0
  * @date    17-April-2018
  * @brief   This module provides a timing clock for system.
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "TimerCounter.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static uint32_t OverFlowCnt = 0xFFFFFFFF;
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Configure the clock timer.
	* @param  None
	* @retval None
	*/
void _TimeTicksInit(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	/* TIM14 clock enable */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM14, ENABLE);

	/* Time base configuration */
  TIM_TimeBaseStructure.TIM_Period = 0xFFFF;
  TIM_TimeBaseStructure.TIM_Prescaler = (uint16_t) ((SystemCoreClock) / 1000) - 1; /* 1ms */
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseInit(TIM14, &TIM_TimeBaseStructure);

	/* NVIC configuration *******************************************************/
	NVIC_InitStructure.NVIC_IRQChannel = TIM14_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPriority = 0x2;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	/* Clear TIM14 Counter */
	TIM_SetCounter(TIM14, 0);

	/* TIM Interrupts enable */
  TIM_ITConfig(TIM14, TIM_IT_Update, ENABLE);

	/* TIM14 counter enable */
  TIM_Cmd(TIM14, ENABLE);
}

/*
 * return timer counter value.
 */
uint16_t _Get_Ticks(void) { return TIM14->CNT; }

/*
 * time in millisecond.
 */
uint32_t _Get_Millis(void)
{
	return (OverFlowCnt << 16) + (TIM14->CNT & 0xFFFF);
}

/*
 * return seconds.
 */
uint32_t _Get_Secnds(void)
{
	return ((_Get_Millis()) / 1000);
}

/**
  * @brief  This function handles the TIM14 interrupt request.
  * @param  None
  * @retval None
  */
void TIM14_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM14, TIM_IT_Update) != RESET) {
		TIM_ClearITPendingBit(TIM14, TIM_IT_Update);
		OverFlowCnt ++;
	}
}

/******************************** END OF FILE *********************************/
