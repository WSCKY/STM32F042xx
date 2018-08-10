#ifndef __LIGHTMODE_H
#define __LIGHTMODE_H

#include "WS2812B.h"

#include "ColorSelector.h"

#define COMBINE_WHITE(x)           ((x) | ((x) << 8) | ((x) << 16))
#define COMBINE_RED(x)             ((x) << 16)
#define COMBINE_GREEN(x)           ((x) << 8)
#define COMBINE_BLUE(x)            (x)
#define COMBINE_RGB(r, g, b)       (((uint32_t)(r) << 16) | ((uint32_t)(g) << 8) | (uint32_t)(b))

#define R_Msk                      (0xFF0000)
#define G_Msk                      (0x00FF00)
#define B_Msk                      (0x0000FF)

#define R_Pos                      (16)
#define G_Pos                      (8)
#define B_Pos                      (0)

#define _DOT_NONE                  0
#define _DOT_CORNER                1
#define _DOT_HEART                 2
#define _DOT_ALL                   3

#define _LUMINANCE_LEVEL_0         0
#define _LUMINANCE_LEVEL_1         51
#define _LUMINANCE_LEVEL_2         127
#define _LUMINANCE_LEVEL_3         204

typedef enum {
	Mode_None = 0x0,
	Mode_Clear = 0x1,
	Mode_SetLumin = 0x2,
} LIGHT_MODE_DEF;

void LightModeInit(void);
void LightModeTask(void);

uint32_t getLightColor(void);
void setLightColor(uint32_t color);
uint8_t getLightLuminance(void);
void setLightLuminance(uint8_t luminance);
void switchDotMode(void);

#endif /* __LIGHTMODE_H */
