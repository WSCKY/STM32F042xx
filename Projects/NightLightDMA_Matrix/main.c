/**
  ******************************************************************************
  * @file    ./main.c 
  * @author  kyChu
  * @version V1.0.0
  * @date    17-April-2018
  * @brief   Main program body
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
#define COMBINE_WHITE(x)           ((x) | ((x) << 8) | ((x) << 16))
#define COMBINE_RED(x)             ((x) << 16)
#define COMBINE_GREEN(x)           ((x) << 8)
#define COMBINE_BLUE(x)            (x)
/* Private variables ---------------------------------------------------------*/
uint32_t EXP_COLOR = 0x080000;

uint32_t KeyStatus = 0, LastKey = 0, KeyCmd = 0;
uint32_t LedNumber = WS281x_MATRIX_WIDTH * WS281x_MATRIX_HEIGHT;
static uint32_t ColorVal[WS281x_MATRIX_WIDTH * WS281x_MATRIX_HEIGHT] = {0};

int dir = 1;
uint8_t breath_color = 0;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
int main(void)
{
	uint32_t i = 0;
  /*!< At this stage the microcontroller clock setting is already configured, 
       this is done through SystemInit() function which is called from startup
       file (startup_stm32f0xx.s) before to branch to application main.
       To reconfigure the default setting of SystemInit() function, refer to
       system_stm32f0xx.c file
     */
//	_TimeTicksInit();
	SystemCoreClockUpdate();

	WS2812B_Init();
	PWR_CTRL_Init(); HW_PWR_ON();
	UserButtonInit();
	Delay_Init();

	for(i = 0; i < LedNumber; i ++) {
		ColorVal[i] = 0xFFFFFFFF;
	}
	for(;;) {
		DelayTicks(5);
		KeyStatus = ReadButtonStatus(PWR_KEY);
		if(KeyStatus == 1 && LastKey == 0) {
			KeyCmd ^= 0x01;
			for(i = 0; i < LedNumber; i ++) {
				ColorVal[i] = KeyCmd ? EXP_COLOR : 0x000000;
			}
		}
		LastKey = KeyStatus;
//		for(i = 0; i < 9; i ++) {
//			ColorVal[i] = COMBINE_BLUE(breath_color) | COMBINE_GREEN(breath_color) | COMBINE_RED(breath_color);//
//		}
//		if(breath_color == 0) dir = 1;
//		if(breath_color == 255) dir = -1;
//		breath_color += dir;
		WS281x_Update(ColorVal);
  }
}

void SysTickIrqCallback(void)
{
	
}

#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

/******************************** END OF FILE *********************************/
