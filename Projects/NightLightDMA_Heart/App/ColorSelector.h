#ifndef __COLORSELECTOR_H
#define __COLORSELECTOR_H

#include "WS2812B.h"

#include "LightMode.h"
#include "CurveCtrl.h"

uint32_t ColourSelectLoop(uint8_t luminance);
uint32_t ComputeColour(uint8_t luminance);

#endif /* __COLORSELECTOR_H */
