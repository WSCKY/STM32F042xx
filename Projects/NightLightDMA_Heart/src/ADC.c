#include "ADC.h"

#define ADC1_DR_Address    0x40012440

static __IO uint16_t RegularConvData = 0;

static void ADC_Config(void);
static void DMA_Config(void);

void VoltCheckInit(void)
{
	/* ADC1 configuration */
	ADC_Config();

	/* DMA configuration */
	DMA_Config();
}

float ReadBatVoltage(void) { return (RegularConvData * SYSTEM_POWER_VOLT * BAT_VOLT_DIV_FACTOR / 4096); }

/**
  * @brief  ADC1 channel configuration
  * @param  None
  * @retval None
  */
static void ADC_Config(void)
{
  ADC_InitTypeDef ADC_InitStructure;
  GPIO_InitTypeDef GPIO_InitStructure;
  /* ADC1 DeInit */
  ADC_DeInit(ADC1);

	/* GPIOA Periph clock enable */
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);

	/* ADC1 Periph clock enable */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);

	/* Configure ADC channel2 as analog input */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	/* Initialize ADC structure */
	ADC_StructInit(&ADC_InitStructure);

  /* Configure the ADC1 in continuous mode withe a resolution equal to 12 bits */
  ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
  ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
  ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
  ADC_InitStructure.ADC_ScanDirection = ADC_ScanDirection_Backward;
  ADC_Init(ADC1, &ADC_InitStructure);

  /* Convert the ADC1 channel2 with 55.5 Cycles as sampling time */
  ADC_ChannelConfig(ADC1, ADC_Channel_2 , ADC_SampleTime_55_5Cycles);

  /* ADC Calibration */
  ADC_GetCalibrationFactor(ADC1);

  /* ADC DMA request in circular mode */
  ADC_DMARequestModeConfig(ADC1, ADC_DMAMode_Circular);

	/* Enable ADC_DMA */
	ADC_DMACmd(ADC1, ENABLE);

	/* Enable the ADC peripheral */
	ADC_Cmd(ADC1, ENABLE);

	/* Wait the ADRDY flag */
	while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_ADRDY));

	/* ADC1 regular Software Start Conv */
	ADC_StartOfConversion(ADC1);
}

/**
  * @brief  DMA channel1 configuration
  * @param  None
  * @retval None
  */
static void DMA_Config(void)
{
	DMA_InitTypeDef DMA_InitStructure;
	/* DMA1 clock enable */
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1 , ENABLE);

	SYSCFG_DMAChannelRemapConfig(SYSCFG_DMARemap_ADC1, DISABLE);

	/* DMA1 Channel1 Config */
	DMA_DeInit(DMA1_Channel1);
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)ADC1_DR_Address;
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)&RegularConvData;
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
	DMA_InitStructure.DMA_BufferSize = 1;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
	DMA_Init(DMA1_Channel1, &DMA_InitStructure);
	/* DMA1 Channel1 enable */
	DMA_Cmd(DMA1_Channel1, ENABLE);
}
