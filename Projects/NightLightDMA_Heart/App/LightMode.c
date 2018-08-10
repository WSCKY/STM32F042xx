#include "LightMode.h"

#include <stdio.h>
#include <string.h>

static uint32_t ColorMemory[WS281x_LED_NUMBER] = {0};
static __IO uint32_t _TextColor = 0x000000;

/*

         ()____________()              1 0 0 0 0 0 0 1               1 . . . . . . 0 
				 __XXXX____XXXX__              0 1 1 0 0 1 1 0               . 2 3 . . 4 5 . 
				 XX____XXXX____XX              1 0 0 1 1 0 0 1               6 . . 7 8 . . 9 
				 XX____________XX              1 0 0 0 0 0 0 1               10. . . . . . 11
				 __XX________XX__              0 1 0 0 0 0 1 0               . 12. . . . 13. 
				 ____XX____XX____              0 0 1 0 0 1 0 0               . . 14. . 15. . 
				 ______XXXX______              0 0 0 1 1 0 0 0               . . . 1617. . . 
				 ()____________()              1 0 0 0 0 0 0 1               18. . . . . . 19

*/

static uint8_t _init_loop = 0;

static uint8_t _dot_mode = 0;
static uint8_t _luminance = 0;
static LIGHT_MODE_DEF _mode = Mode_None;

static uint32_t _time_cnt = 0;
static uint8_t _update_enable = 0;

static void LightUpdateMemory(void);

void LightModeInit(void)
{
	uint8_t i;
	_TextColor = 0x000000;
	for(i = 0; i < WS281x_LED_NUMBER; i ++)
		ColorMemory[i] = _TextColor;
	_init_loop = 0;
	_mode = Mode_None;
	_dot_mode = _DOT_HEART;//_DOT_CORNER;//
	_luminance = _LUMINANCE_LEVEL_0;
}

void LightModeTask(void)
{
	if(_init_loop == 0) {
		if(_time_cnt % 20 == 0) {
			if(_luminance < _LUMINANCE_LEVEL_1) { _luminance ++; }
			else { _init_loop = 1; }
			_TextColor = ComputeColour(_luminance);
			LightUpdateMemory();
			_update_enable = 1;
		} else {
			_update_enable = 0;
		}
	} else {
		switch(_mode) {
			case Mode_None:
				_update_enable = 0;
			break;
			case Mode_Clear:
				LightUpdateMemory();
				_mode = Mode_None;
				_update_enable = 1;
			break;
			case Mode_SetLumin:
				_TextColor = ComputeColour(_luminance);
				LightUpdateMemory();
				_mode = Mode_None;
				_update_enable = 1;
			break;
			default:
				_mode = Mode_None;
			break;
		}
	}

	if(_update_enable) {
		WS281x_Update(ColorMemory);
	}

	_time_cnt ++;
}

static void LightUpdateMemory(void)
{
	uint8_t i;
	uint32_t _c_Color = _TextColor, _h_Color = _TextColor;

	if(_dot_mode == _DOT_NONE) {
		_h_Color = 0x000000;
		_c_Color = 0x000000;
	} else if(_dot_mode == _DOT_CORNER) {
		_h_Color = 0x000000;
	} else if(_dot_mode == _DOT_HEART) {
		_c_Color = 0x000000;
	}

	for(i = 0; i < WS281x_LED_NUMBER; i ++) {
		if(i == 0 || i == 1 || i == 18 || i == 19) {
			ColorMemory[i] = _c_Color;
		} else {
			ColorMemory[i] = _h_Color;
		}
	}
}

inline uint32_t getLightColor(void) { return _TextColor; }
inline void setLightColor(uint32_t color) { _TextColor = color; _mode = Mode_Clear; }
inline uint8_t getLightLuminance(void) { return _luminance; }
inline void setLightLuminance(uint8_t luminance) { _luminance = luminance; _mode = Mode_SetLumin; }
inline void switchDotMode(void) { if(_dot_mode < _DOT_ALL) _dot_mode ++; else _dot_mode = _DOT_CORNER; _mode = Mode_Clear; }
