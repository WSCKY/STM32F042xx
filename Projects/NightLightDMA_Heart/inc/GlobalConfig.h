#ifndef __GLOBALCONFIG_H
#define __GLOBALCONFIG_H

#include "stm32f0xx.h"

#define WS281x_LED_NUMBER              (20) /* Number of LEDs. (time limit: 20 * 24 * 1.25 + 50 = 650us) */

#define SYSTEM_POWER_VOLT              (3.3f)
#define BAT_VOLT_DIV_FACTOR            (2.0f) /* 3.3V -> 10K -> 10K -> 0V */

#endif /* __GLOBALCONFIG_H */
