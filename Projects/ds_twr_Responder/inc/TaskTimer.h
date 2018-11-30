#ifndef __TASKTIMER_H
#define __TASKTIMER_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f0xx.h"
#include "stm32f0xx_tim.h"
#include "stm32f0xx_rcc.h"
#include "stm32f0xx_misc.h"

void TaskTimerInit(uint32_t TaskRate);

void TaskTimerCallback(void);

#endif /* __TASKTIMER_H */
