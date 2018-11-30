#include "TaskTImer.h"

void TaskTimerInit(uint32_t TaskRate)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	/* TIM2 clock enable */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

	/* Time base configuration */
  TIM_TimeBaseStructure.TIM_Period = (1000 / TaskRate) - 1;
  TIM_TimeBaseStructure.TIM_Prescaler = (uint16_t) ((SystemCoreClock) / 1000) - 1;
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

	/* NVIC configuration *******************************************************/
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPriority = 0x3;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	/* Clear TIM2 Counter */
	TIM_SetCounter(TIM2, 0);

	/* TIM Interrupts enable */
  TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);

	/* TIM2 counter enable */
  TIM_Cmd(TIM2, ENABLE);
}

/**
  * @brief  This function handles the TIM2 interrupt request.
  * @param  None
  * @retval None
  */
void TIM2_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET) {
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
		TaskTimerCallback();
	}
}
