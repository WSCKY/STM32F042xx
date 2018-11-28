#include "DW1000.h"

static _DW1000_IRQnCallback pIRQnCallback = 0;

static void SPI_Configuration(void);
static void GPIO_Configuration(void);

/**
  * @brief  Initialize Driver for DW1000.
  * @param  None
  * @retval None
  */
void DW1000_If_Init(void)
{
  SPI_Configuration();
  GPIO_Configuration();
}

int DW1000_SPI_Read(uint16_t headerLength, const uint8_t *headerBuffer, uint32_t readlength, uint8_t *readBuffer)
{
  uint32_t i = 0;
  _DW_SPI_NSS_ENABLE();
  for(i = 0; i < headerLength; i ++) {
    _DW_SPI->DR = headerBuffer[i];
    while((_DW_SPI->SR & SPI_I2S_FLAG_RXNE) == (uint16_t)RESET) {}
    readBuffer[0] = _DW_SPI->DR ; // Dummy read as we write the header
  }
  for(i=0; i<readlength; i++) {
    _DW_SPI->DR = 0;  // Dummy write as we read the message body
    while((_DW_SPI->SR & SPI_I2S_FLAG_RXNE) == (uint16_t)RESET) {}
    readBuffer[i] = _DW_SPI->DR ; //this clears RXNE bit
  }
  _DW_SPI_NSS_DISABLE();
  return 0;
}

int DW1000_SPI_Write(uint16_t headerLength, const uint8_t *headerBuffer, uint32_t bodyLength, const uint8_t *bodyBuffer)
{
  uint32_t i = 0;
  _DW_SPI_NSS_ENABLE();
  for(i = 0; i < headerLength; i ++) {
    _DW_SPI->DR = headerBuffer[i];
    while((_DW_SPI->SR & SPI_I2S_FLAG_RXNE) == (uint16_t)RESET) {}
    _DW_SPI->DR;
  }
  for( i = 0; i < bodyLength; i ++) {
    _DW_SPI->DR = bodyBuffer[i];
    while((_DW_SPI->SR & SPI_I2S_FLAG_RXNE) == (uint16_t)RESET) {}
    _DW_SPI->DR ;
	}
  _DW_SPI_NSS_DISABLE();
  return 0;
}

void DW1000RSTn_DrvLow(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  GPIO_InitStructure.GPIO_Pin = _DW1000_RST_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_Level_3;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_Init(_DW1000_RST_GPIO_PORT, &GPIO_InitStructure);
  _DW1000_RST_GPIO_PORT->BRR = _DW1000_RST_PIN;
}

void DW1000RSTn_NOP_IN(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  GPIO_InitStructure.GPIO_Pin = _DW1000_RST_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_Level_3;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_Init(_DW1000_RST_GPIO_PORT, &GPIO_InitStructure);
}

void SPI_ConfigFastRate(uint32_t scalingfactor)
{
	uint16_t tmpreg = 0;
	if(scalingfactor != DW1000_SPI_HIGH && scalingfactor != DW1000_SPI_LOW) return;
	/* Disable the selected SPI peripheral */
	_DW_SPI->CR1 &= 0xFFBF;
	/* Get the SPIx CR1 value */
	tmpreg = _DW_SPI->CR1;
	/* Clear BR[2:0] bits */
	tmpreg &= 0xFFC7;
	/* Set the scaling bits */
	tmpreg |= scalingfactor;
	/* Write to SPIx CR1 */
	_DW_SPI->CR1 = tmpreg;
	/* Enable the selected SPI peripheral */
	_DW_SPI->CR1 |= 0x0040;
}

void DW1000_SetIRQnHandler(_DW1000_IRQnCallback pf)
{
  if(pf != 0)
    pIRQnCallback = pf;
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

  /* SPI NSS pin configuration */
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_InitStructure.GPIO_Pin = _DW_SPI_NSS_PIN;
  GPIO_Init(_DW_SPI_NSS_GPIO_PORT, &GPIO_InitStructure);

  _DW_SPI_NSS_DISABLE();

  /* SPI configuration -------------------------------------------------------*/
  SPI_I2S_DeInit(_DW_SPI);
  /* Initializes the SPI communication */
  SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
  SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
  SPI_InitStructure.SPI_DataSize = SPI_DATASIZE;
  SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
  SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
  SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
  /*
  For DW1000, The maximum SPI frequency is 20 MHz when the CLKPLL is locked, otherwise the maximum SPI frequency is 3 MHz.
  */
  SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_16; /* 48 / 16 = 3MHz */
  SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
  SPI_InitStructure.SPI_CRCPolynomial = 7;
  SPI_Init(_DW_SPI, &SPI_InitStructure);

  /* Initialize the FIFO threshold */
  SPI_RxFIFOThresholdConfig(_DW_SPI, SPI_RxFIFOThreshold_QF);

  /* Enable the SPI peripheral */
  SPI_Cmd(_DW_SPI, ENABLE);
}

/**
  * @brief  Configures the different GPIO ports.
  * @param  None
  * @retval None
  */
static void GPIO_Configuration(void)
{
  EXTI_InitTypeDef   EXTI_InitStructure;
  GPIO_InitTypeDef   GPIO_InitStructure;
  NVIC_InitTypeDef   NVIC_InitStructure;
  /* Enable GPIOA clock */
  RCC_AHBPeriphClockCmd(_DW1000_RST_GPIO_CLK | _DW1000_WAKEUP_GPIO_CLK | _DW1000_IRQ_GPIO_CLK, ENABLE);

  /* Configure RST & WAKEUP pin as output pull up. */
  GPIO_InitStructure.GPIO_Pin = _DW1000_WAKEUP_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_Level_3;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_Init(_DW1000_WAKEUP_GPIO_PORT, &GPIO_InitStructure);

/*
  +----------------------+---------------------------------------------------------------------------------------------------------------------------+
  |      Use of RSTn     | Description                                                                                                               |
  +----------------------+---------------------------------------------------------------------------------------------------------------------------+
  | As output to control | RSTn may be used as an output to reset external circuitry as part of an orderly bring up of a system as power is applied. |
  | external circuitry   |                                                                                                                           |
  +----------------------+---------------------------------------------------------------------------------------------------------------------------+
  | As interrupt input   | RSTn may be used as an interrupt input to the external host to indicate that the DW1000 has entered the INIT state.       |
  | to external host     | When RSTn is used in this way care should be taken to ensure that the interrupt pin of the external host does not         |
  |                      | pull-up the RSTn signal which should be left open-drain. Refer to Table 1 and Figure 37(dw1000 datasheet p21).            |
  +----------------------+---------------------------------------------------------------------------------------------------------------------------+
*/
  GPIO_InitStructure.GPIO_Pin = _DW1000_RST_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(_DW1000_RST_GPIO_PORT, &GPIO_InitStructure);

  /* Reset Wake-up pin */
  GPIO_ResetBits(_DW1000_WAKEUP_GPIO_PORT, _DW1000_WAKEUP_PIN);

  /* Configure IRQ pin as input floating */
  GPIO_InitStructure.GPIO_Pin = _DW1000_IRQ_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
  GPIO_Init(_DW1000_IRQ_GPIO_PORT, &GPIO_InitStructure);

  /* Enable SYSCFG clock */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
  /* Connect EXTI0 Line to IRQ pin */
  SYSCFG_EXTILineConfig(_DW1000_IRQ_GPIO_PortSource, _DW1000_IRQ_GPIO_PinSource);

  /* Configure IRQ_EXTI line */
  EXTI_InitStructure.EXTI_Line = _DW1000_IRQ_EXTI_LINE;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);

  /* Enable and set EXTI Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = _DW1000_IRQ_EXTI_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPriority = 0x00;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}

void _DW1000_IRQ_EXTI_IRQHandler(void)
{
  if(EXTI_GetITStatus(_DW1000_IRQ_EXTI_LINE) != RESET) {
    if(pIRQnCallback != 0) {
      pIRQnCallback();
    }
    EXTI_ClearITPendingBit(_DW1000_IRQ_EXTI_LINE);
  }
}
