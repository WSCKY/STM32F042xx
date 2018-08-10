/**
  ******************************************************************************
  * @file    ./src/UserButton.c 
  * @author  kyChu
  * @version V1.0.0
  * @date    30-July-2018
  * @brief   User Button GPIO Driver Module.
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "UserButton.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static GPIO_TypeDef* UserButtonPort[BUTTON_NUMBER] = BUTTON_GROUP_PORT;
static uint16_t UserButtonPin[BUTTON_NUMBER] = BUTTON_GROUP_PIN;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  User Button Initialize.
	* @param  None.
	* @retval None.
	*/
void UserButtonInit(void)
{
	uint8_t Index = 0;
	GPIO_InitTypeDef GPIO_InitStructure;
	/* Button GPIO Periph clock enable */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA | RCC_AHBPeriph_GPIOB, ENABLE);
	/* Configure HW_ID_GPIO_Pin in input mode */
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;

	for(Index = 0; Index < BUTTON_NUMBER; Index ++) {
		GPIO_InitStructure.GPIO_Pin = UserButtonPin[Index];
		GPIO_Init(UserButtonPort[Index], &GPIO_InitStructure);
	}
}

/**
  * @brief  Return Button Status.
  * @param  id: Button ID.
  * @retval Button Status.
  */
uint32_t ReadButtonStatus(ButtonID id)
{
	if((UserButtonPort[id]->IDR & UserButtonPin[id]) != 0) return 0;
	return 1;
}

/******************************** END OF FILE *********************************/
