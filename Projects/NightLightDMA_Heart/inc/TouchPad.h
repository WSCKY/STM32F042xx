#ifndef __TOUCHPAD_H
#define __TOUCHPAD_H

#include "stm32f0xx.h"
#include "stm32f0xx_rcc.h"
#include "stm32f0xx_tim.h"
#include "stm32f0xx_gpio.h"
#include "stm32f0xx_misc.h"

#define TP_GPIO_PORT                   GPIOA
#define TP_GPIO_PORT_PIN               GPIO_Pin_1
#define TP_GPIO_PORT_CLK               RCC_AHBPeriph_GPIOA
#define TP_GPIO_PORT_PIN_AF            GPIO_AF_2
#define TP_GPIO_PORT_PINSOURCE         GPIO_PinSource1

#define TP_TIMER                       TIM2
#define TP_TIMER_CLK                   RCC_APB1Periph_TIM2
#define TP_TIMER_CLK_CMD               RCC_APB1PeriphClockCmd
#define TP_TIMER_IT_FLAG               TIM_IT_CC2
#define TP_TIMER_IRQn                  TIM2_IRQn
#define TP_TIMER_IRQHandler            TIM2_IRQHandler

#define DATA_FILTER_DEPTH              10

void TouchPadInit(void);
void TouchPadCheckLoop(void);
uint8_t GetTouchState(void);

#endif /* __TOUCHPAD_H */
