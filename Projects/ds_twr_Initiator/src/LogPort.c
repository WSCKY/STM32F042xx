#include "LogPort.h"

#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)

void LOGPortInit(void)
{
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;

	/* Enable GPIO clock */
  RCC_APB2PeriphClockCmd(LOG_USART_GPIO_CLK | RCC_APB2Periph_AFIO, ENABLE);

	/* Configure LOG_USART Rx as input floating */
  GPIO_InitStructure.GPIO_Pin = LOG_USART_RxPin;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(LOG_USART_GPIO, &GPIO_InitStructure);

  /* Configure LOG_USART Tx as alternate function push-pull */
  GPIO_InitStructure.GPIO_Pin = LOG_USART_TxPin;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(LOG_USART_GPIO, &GPIO_InitStructure);

	/* Enable LOG_USART Clock */
  RCC_APB1PeriphClockCmd(LOG_USART_CLK, ENABLE);

	/* LOG USART configuration -------------------------------------------*/
  /* LOG USART configured as follow:
        - BaudRate = 115200
        - Word Length = 8 Bits
        - One Stop Bit
        - No parity
        - Hardware flow control disabled (RTS and CTS signals)
        - Receive and transmit enabled
  */

  USART_InitStructure.USART_BaudRate = 115200;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  /* Configure LOG_USART */
  USART_Init(LOG_USART, &USART_InitStructure);

	/* Enable the LOG_USART Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = LOG_USART_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

  /* Enable the LOG_USART Receive Interrupt */
  USART_ITConfig(LOG_USART, USART_IT_RXNE, ENABLE);

  /* Enable LOG_USART */
  USART_Cmd(LOG_USART, ENABLE);
}

/**
  * @brief  Retargets the C library printf function to the USART.
  * @param  None
  * @retval None
  */
PUTCHAR_PROTOTYPE
{
  /* Place your implementation of fputc here */
  /* e.g. write a character to the USART */
  USART_SendData(LOG_USART, (uint8_t) ch);

  /* Loop until the end of transmission */
  while (USART_GetFlagStatus(LOG_USART, USART_FLAG_TC) == RESET)
  {}

  return ch;
}

void LOG_USART_IRQHandler(void)
{
	if(SET == USART_GetITStatus(LOG_USART, USART_IT_RXNE)) {
		(void)LOG_USART->DR;
//		UartxReceivedDataCallBack(LOG_USART->DR);
		USART_ClearFlag(LOG_USART, USART_FLAG_RXNE);
	}
}
