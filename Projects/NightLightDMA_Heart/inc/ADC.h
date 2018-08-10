#ifndef __ADC_H
#define __ADC_H

#include "stm32f0xx_adc.h"
#include "stm32f0xx_dma.h"
#include "stm32f0xx_rcc.h"
#include "stm32f0xx_gpio.h"
#include "stm32f0xx_syscfg.h"

#include "GlobalConfig.h"

void VoltCheckInit(void);
float ReadBatVoltage(void);

#endif /* __ADC_H */
