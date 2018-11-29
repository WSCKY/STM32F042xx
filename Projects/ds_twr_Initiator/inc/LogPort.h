#ifndef __LOGPORT_H
#define __LOGPORT_H

#include "stm32f10x_usart.h"
#include <stdio.h>

#define LOG_USART                      UART4
#define LOG_USART_GPIO                 GPIOC
#define LOG_USART_CLK                  RCC_APB1Periph_UART4
#define LOG_USART_GPIO_CLK             RCC_APB2Periph_GPIOC
#define LOG_USART_RxPin                GPIO_Pin_11
#define LOG_USART_TxPin                GPIO_Pin_10
#define LOG_USART_IRQn                 UART4_IRQn
#define LOG_USART_IRQHandler           UART4_IRQHandler

void LOGPortInit(void);

#endif /* __LOGPORT_H */
