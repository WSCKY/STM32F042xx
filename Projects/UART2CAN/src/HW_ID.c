/**
  ******************************************************************************
  * @file    ./src/HW_ID.c 
  * @author  kyChu
  * @version V1.0.0
  * @date    17-April-2018
  * @brief   Hardware ID Reader.
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "HW_ID.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Hardware ID define.
  * @param  None
  * @retval None
  */
void HW_ID_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	/* HW_ID_GPIO Periph clock enable */
  RCC_AHBPeriphClockCmd(ID_GPIO_PORT_CLK_1 | ID_GPIO_PORT_CLK_2 | ID_GPIO_PORT_CLK_3 | \
												ID_GPIO_PORT_CLK_4 | ID_GPIO_PORT_CLK_5, ENABLE);

  /* Configure HW_ID_GPIO_Pin in input mode */
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;

	GPIO_InitStructure.GPIO_Pin = ID_GPIO_PORT_PIN_1;
  GPIO_Init(ID_GPIO_PORT_1, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = ID_GPIO_PORT_PIN_2;
  GPIO_Init(ID_GPIO_PORT_2, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = ID_GPIO_PORT_PIN_3;
  GPIO_Init(ID_GPIO_PORT_3, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = ID_GPIO_PORT_PIN_4;
  GPIO_Init(ID_GPIO_PORT_4, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = ID_GPIO_PORT_PIN_5;
  GPIO_Init(ID_GPIO_PORT_5, &GPIO_InitStructure);
}

/**
  * @brief  Read Hardware ID Value.
  * @param  None
  * @retval ID Value.
  */
uint8_t ReadHW_ID(void)
{
	uint8_t id = 0x0;
	if((ID_GPIO_PORT_1->IDR & ID_GPIO_PORT_PIN_1) != (uint32_t)Bit_RESET) id |= 0x01;
	if((ID_GPIO_PORT_2->IDR & ID_GPIO_PORT_PIN_2) != (uint32_t)Bit_RESET) id |= 0x02;
	if((ID_GPIO_PORT_3->IDR & ID_GPIO_PORT_PIN_3) != (uint32_t)Bit_RESET) id |= 0x04;
	if((ID_GPIO_PORT_4->IDR & ID_GPIO_PORT_PIN_4) != (uint32_t)Bit_RESET) id |= 0x08;
	if((ID_GPIO_PORT_5->IDR & ID_GPIO_PORT_PIN_5) != (uint32_t)Bit_RESET) id |= 0x10;
	return id;
}

/******************************** END OF FILE *********************************/
