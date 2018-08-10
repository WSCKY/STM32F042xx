#include "WS2812B.h"

#define SEQUENCE_0                     10
#define SEQUENCE_1                     21
#define WS281x_NOP_TICKS               60

#define SRC_SIZE                       (WS281x_LED_NUMBER * WS281x_COLOR_DEPTH + WS281x_NOP_TICKS)

static uint8_t _comp_flag = 1;
static uint8_t SRC_Buffer[SRC_SIZE] = {0};

static DMA_InitTypeDef DMA_InitStructure;

static void TIM_Config(void);
static void DMA_Config(void);

void WS2812B_Init(void)
{
	uint32_t size = 0;
	for(size = 0; size < SRC_SIZE; size ++)
		SRC_Buffer[size] = 0;

	DMA_Config();
	TIM_Config();
}

void WS281x_Update(uint32_t *src)
{
	uint8_t *p = SRC_Buffer;
	uint8_t i, j, color, mask;

	if(_comp_flag != 1) return;

	for(i = 0; i < WS281x_LED_NUMBER; i ++) {
		mask = 0x80;
		color = (src[i] & WS2812_G_MASK) >> 8;
		for(j = 0; j < 8; j ++) {
			if(color & mask)
				p[i * WS281x_COLOR_DEPTH + j] = SEQUENCE_1;
			else
				p[i * WS281x_COLOR_DEPTH + j] = SEQUENCE_0;
			mask >>= 1;
		}
		mask = 0x80;
		color = (src[i] & WS2812_R_MASK) >> 16;
		for(j = 8; j < 16; j ++) {
			if(color & mask)
				p[i * WS281x_COLOR_DEPTH + j] = SEQUENCE_1;
			else
				p[i * WS281x_COLOR_DEPTH + j] = SEQUENCE_0;
			mask >>= 1;
		}
		mask = 0x80;
		color = (src[i] & WS2812_B_MASK) >> 0;
		for(j = 16; j < 24; j ++) {
			if(color & mask)
				p[i * WS281x_COLOR_DEPTH + j] = SEQUENCE_1;
			else
				p[i * WS281x_COLOR_DEPTH + j] = SEQUENCE_0;
			mask >>= 1;
		}
	}
}

/**
  * @brief  Configure the TIMx Pins.
  * @param  None
  * @retval None
  */
static void TIM_Config(void)
{
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
  TIM_OCInitTypeDef  TIM_OCInitStructure;
  GPIO_InitTypeDef GPIO_InitStructure;

  /* GPIO Clock enable */
  RCC_AHBPeriphClockCmd(WS281x_GPIO_PORT_CLK, ENABLE);

	/* GPIO Configuration: Channelx as alternate function push-pull */
	GPIO_InitStructure.GPIO_Pin = WS281x_GPIO_PORT_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP ;
	GPIO_Init(WS281x_GPIO_PORT, &GPIO_InitStructure);

	GPIO_PinAFConfig(WS281x_GPIO_PORT, WS281x_GPIO_PORT_PIN_SOURCE, WS281x_GPIO_PORT_PIN_AF);

  /* TIMx Configuration ---------------------------------------------------
   Generate 1 PWM signals with 0% duty cycles:
   TIMx input clock (TIMxCLK) is set to APB1 clock (PCLK1)
    => TIMxCLK = PCLK1 = SystemCoreClock
   TIMxCLK = SystemCoreClock, Prescaler = 0, TIMx counter clock = SystemCoreClock
   SystemCoreClock is set to 48 MHz for STM32F0xx devices

   The objective is to generate 1 PWM signal at 800 KHz:
     - TIMx_Period = (SystemCoreClock / 800000) - 1
   The channel 1 duty cycle is set to 0%
   The Timer pulse is calculated as follows:
     - ChannelxPulse = DutyCycle * (TIMx_Period - 1) / 100

   Note: 
    SystemCoreClock variable holds HCLK frequency and is defined in system_stm32f0xx.c file.
    Each time the core clock (HCLK) changes, user had to call SystemCoreClockUpdate()
    function to update SystemCoreClock variable value. Otherwise, any configuration
    based on this variable will be incorrect. 
  ----------------------------------------------------------------------- */

	/* TIMx clock enable */
	WS281x_TIMER_CLK_CMD(WS281x_TIMER_CLK , ENABLE);

	/* Time Base configuration */
	TIM_TimeBaseStructure.TIM_Prescaler = 1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	/* Compute the value to be set in ARR regiter to generate signal frequency at 800 Khz */
	TIM_TimeBaseStructure.TIM_Period = 29;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(WS281x_TIMER, &TIM_TimeBaseStructure);

  /* Channel1 Configuration in PWM mode */
	TIM_OCInitStructure.TIM_Pulse = 0;
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Disable;
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;
  TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_Low;
  TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Reset;
  TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCIdleState_Reset;
  TIM_OC1Init(WS281x_TIMER, &TIM_OCInitStructure);
	TIM_OC1PreloadConfig(WS281x_TIMER, TIM_OCPreload_Enable);

	TIM_DMACmd(WS281x_TIMER, TIM_DMA_CC1, ENABLE);

	/* TIMx counter enable */
	TIM_Cmd(WS281x_TIMER, ENABLE);
}

/**
  * @brief  Configure DMA1.
  * @param  None
  * @retval None
  */
static void DMA_Config(void)
{
	/* DMA1 clock enable */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

	/* DeInitialize the DMA Channel */
	DMA_DeInit(WS281x_DMA_CHANNEL);

	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)WS281x_TIMER_CCR_ADDR;
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)SRC_Buffer;
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
	DMA_InitStructure.DMA_BufferSize = SRC_SIZE;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
	DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
	DMA_Init(WS281x_DMA_CHANNEL, &DMA_InitStructure);

	/* Enable DMA Channel */
	DMA_Cmd(WS281x_DMA_CHANNEL, ENABLE);
}

void WS281x_DMA_CHANNEL_IRQHandler(void)
{
	if(DMA_GetFlagStatus(WS281x_DMA_TC_FLAG)) {
		DMA_ClearFlag(WS281x_DMA_TC_FLAG);

	}
}
