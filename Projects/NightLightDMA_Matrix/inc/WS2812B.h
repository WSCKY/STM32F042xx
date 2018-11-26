/**
  ******************************************************************************
  * @file    ./inc/WS2812B.h 
  * @author  kyChu
  * @version V1.0.0
  * @date    30-July-2018
  * @brief   Header for WS2812B.c module.
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __WS2812B_H
#define __WS2812B_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f0xx.h"
#include "stm32f0xx_gpio.h"
#include "stm32f0xx_rcc.h"
#include "stm32f0xx_tim.h"
#include "stm32f0xx_dma.h"
#include "stm32f0xx_misc.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
#define WS281x_GPIO_PORT               GPIOB
#define WS281x_GPIO_PORT_PIN           GPIO_Pin_4
#define WS281x_GPIO_PORT_CLK           RCC_AHBPeriph_GPIOB
#define WS281x_GPIO_PORT_PIN_AF        GPIO_AF_1
#define WS281x_GPIO_PORT_PIN_SOURCE    GPIO_PinSource4

#define WS281x_TIMER                   TIM3
#define WS281x_TIMER_CLK               RCC_APB1Periph_TIM3
#define WS281x_TIMER_CLK_CMD           RCC_APB1PeriphClockCmd
#define WS281x_TIMER_CCR_ADDR          (0x40000434)

#define WS281x_DMA_CHANNEL             DMA1_Channel4
#define WS281x_DMA_CHANNEL_IRQn        DMA1_Channel4_5_IRQn
#define WS281x_DMA_CHANNEL_IRQHandler  DMA1_Channel4_5_IRQHandler

#define WS281x_DMA_TC_FLAG             DMA1_FLAG_TC4

#define WS281x_MATRIX_WIDTH            20
#define WS281x_MATRIX_HEIGHT           1
#define WS281x_COLOR_DEPTH             24

#define WS2812_R_MASK                       0x00FF0000
#define WS2812_G_MASK                       0x0000FF00
#define WS2812_B_MASK                       0x000000FF

/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void WS2812B_Init(void);
void WS281x_Update(uint32_t *src);

#endif /* __WS2812B_H */

/******************************** END OF FILE *********************************/
