#include "DW1000.h"

static void SPI_Configuration(void);

/**
  * @brief  Initialize Driver for DW1000.
  * @param  None
  * @retval None
  */
void DW1000_If_Init(void)
{
  SPI_Configuration();
}

/**
  * @brief  Configure SPI peripheral.
	* @param  None
	* @retval None
	*/
static void SPI_Configuration(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  SPI_InitTypeDef  SPI_InitStructure;

  /* Enable the SPI periph */
  RCC_APB2PeriphClockCmd(_DW_SPI_CLK, ENABLE);

  /* Enable SCK, MOSI, MISO and NSS GPIO clocks */
  RCC_AHBPeriphClockCmd(_DW_SPI_SCK_GPIO_CLK | _DW_SPI_MISO_GPIO_CLK | _DW_SPI_MOSI_GPIO_CLK, ENABLE);

  GPIO_PinAFConfig(_DW_SPI_SCK_GPIO_PORT, _DW_SPI_SCK_SOURCE, _DW_SPI_SCK_AF);
  GPIO_PinAFConfig(_DW_SPI_MOSI_GPIO_PORT, _DW_SPI_MOSI_SOURCE, _DW_SPI_MOSI_AF);
  GPIO_PinAFConfig(_DW_SPI_MISO_GPIO_PORT, _DW_SPI_MISO_SOURCE, _DW_SPI_MISO_AF);

  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_DOWN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_Level_3;

  /* SPI SCK pin configuration */
  GPIO_InitStructure.GPIO_Pin = _DW_SPI_SCK_PIN;
  GPIO_Init(_DW_SPI_SCK_GPIO_PORT, &GPIO_InitStructure);

  /* SPI  MOSI pin configuration */
  GPIO_InitStructure.GPIO_Pin =  _DW_SPI_MOSI_PIN;
  GPIO_Init(_DW_SPI_MOSI_GPIO_PORT, &GPIO_InitStructure);

  /* SPI MISO pin configuration */
  GPIO_InitStructure.GPIO_Pin = _DW_SPI_MISO_PIN;
  GPIO_Init(_DW_SPI_MISO_GPIO_PORT, &GPIO_InitStructure);

  /* SPI configuration -------------------------------------------------------*/
  SPI_I2S_DeInit(_DW_SPI);
  SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
  SPI_InitStructure.SPI_DataSize = SPI_DATASIZE;
  SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
  SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
  SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
  SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;
  SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
  SPI_InitStructure.SPI_CRCPolynomial = 7;
  SPI_Init(_DW_SPI, &SPI_InitStructure);
}
