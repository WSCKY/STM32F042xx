#include "TouchPad.h"

#include <string.h>

static GPIO_InitTypeDef GPIO_InitStructure;

static uint8_t _check_step = 0;
static uint32_t _check_time = 0;
static uint8_t _check_start = 0;
static uint32_t _filter_time = 0;

static uint32_t FiltedCapVal = 0;
static uint32_t CapValBuffer[DATA_FILTER_DEPTH] = {0};
static uint32_t CapSortBuffer[DATA_FILTER_DEPTH] = {0};

static void TIM_Config(void);
static void TouchPinNoPull(void);
static void TouchPinPullDown(void);
static void TouchTimerClear(void);
static void StoreNewData(uint32_t Data);
static uint32_t ReadNewTPValue(void);

void TouchPadInit(void)
{
	/* GPIOA clock enable */
  RCC_AHBPeriphClockCmd(TP_GPIO_PORT_CLK, ENABLE);

  /* TIM2 channel 2 pin (PA.01) configuration */
  GPIO_InitStructure.GPIO_Pin =  TP_GPIO_PORT_PIN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
  GPIO_Init(TP_GPIO_PORT, &GPIO_InitStructure);
	TP_GPIO_PORT->BRR = TP_GPIO_PORT_PIN;
	TIM_Config();
}

void TouchPadCheckLoop(void)
{
	switch(_check_step) {
		case 0:
			_check_time = 0;
			TouchPinPullDown();
			_check_step = 1;
		break;
		case 1:
			if(_check_time >= 10) {
				TouchPinNoPull();
				TouchTimerClear();
				_check_time =0;
				_check_start = 1;
				_check_step = 2;
			}
		break;
		case 2:
			if(_check_time >= 20) {
				StoreNewData(TIM_GetCapture2(TP_TIMER));
				_check_start = 0;
				_check_step = 0;
			}
		break;
		default:
			_check_start = 0;
			_check_step = 0;
		break;
	}
	_check_time ++;

	_filter_time ++;
	if(_filter_time >= 50) {
		_filter_time = 0;
		FiltedCapVal = ReadNewTPValue();
	}
}

static uint32_t ReadNewTPValue(void)
{
	uint8_t i = 0, j = 0;
	memcpy(CapSortBuffer, CapValBuffer, DATA_FILTER_DEPTH << 2);
	for(i = DATA_FILTER_DEPTH - 1; i > 0; i --) {
		for(j = 0; j < i; j ++) {
			if(CapSortBuffer[j] > CapSortBuffer[j + 1]) {
				CapSortBuffer[j] ^= CapSortBuffer[j + 1];
				CapSortBuffer[j + 1] ^= CapSortBuffer[j];
				CapSortBuffer[j] ^= CapSortBuffer[j + 1];
			}
		}
	}
	i = DATA_FILTER_DEPTH >> 1;
	return ((CapSortBuffer[i - 2] + CapSortBuffer[i - 1] + CapSortBuffer[i] + CapSortBuffer[i +1] + CapSortBuffer[i +2]) / 5);
}

uint8_t GetTouchState(void)
{
	if(FiltedCapVal > 4400) return 1;
	return 0;
}

static void StoreNewData(uint32_t Data)
{
	static uint8_t pointer = 0;
	CapValBuffer[pointer ++] = Data;
	if(pointer >= DATA_FILTER_DEPTH) {
		pointer = 0;
	}
}

static void TouchPinNoPull(void)
{
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(TP_GPIO_PORT, &GPIO_InitStructure);
}

static void TouchPinPullDown(void)
{
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
  GPIO_Init(TP_GPIO_PORT, &GPIO_InitStructure);
	TP_GPIO_PORT->BRR = TP_GPIO_PORT_PIN; /* Connect to Ground. */
}

static void TouchTimerClear(void)
{
	TIM_SetCounter(TP_TIMER, 0);
	TIM_ClearITPendingBit(TP_TIMER, TP_TIMER_IT_FLAG);
}

/**
  * @brief  Configure the TIM1 Pins.
  * @param  None
  * @retval None
  */
static void TIM_Config(void)
{
  TIM_ICInitTypeDef  TIM_ICInitStructure;
  NVIC_InitTypeDef NVIC_InitStructure;

  /* TP_TIMER clock enable */
  TP_TIMER_CLK_CMD(TP_TIMER_CLK, ENABLE);

  /* Connect TIM pins to AF2 */
  GPIO_PinAFConfig(TP_GPIO_PORT, TP_GPIO_PORT_PINSOURCE, TP_GPIO_PORT_PIN_AF);

  /* TIM2 configuration: Input Capture mode ---------------------
     The external signal is connected to TIM2 CH2 pin (PA.01)  
     The Rising edge is used as active edge,
     The TIM2 CCR2 is used to compute the frequency value 
  ------------------------------------------------------------ */

  TIM_ICInitStructure.TIM_Channel = TIM_Channel_2;
  TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;
  TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
  TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
  TIM_ICInitStructure.TIM_ICFilter = 0x0;

  TIM_ICInit(TP_TIMER, &TIM_ICInitStructure);

  /* TP_TIMER enable counter */
  TIM_Cmd(TP_TIMER, ENABLE);

  /* Enable the CC2 Interrupt Request */
  TIM_ITConfig(TP_TIMER, TP_TIMER_IT_FLAG, ENABLE);

  /* Enable the TP_TIMER global Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = TP_TIMER_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}

/**
  * @brief  This function handles TIM1 Capture Compare interrupt request.
  * @param  None
  * @retval None
  */
void TP_TIMER_IRQHandler(void)
{ 
  if(TIM_GetITStatus(TP_TIMER, TP_TIMER_IT_FLAG) == SET) {
    /* Clear TIM2 Capture compare interrupt pending bit */
    TIM_ClearITPendingBit(TP_TIMER, TP_TIMER_IT_FLAG);
		if(_check_start == 1) {
			/* Get the Input Capture value */
			StoreNewData(TIM_GetCapture2(TP_TIMER));
			_check_start = 0;
			_check_step = 0;
		}
	}
}
