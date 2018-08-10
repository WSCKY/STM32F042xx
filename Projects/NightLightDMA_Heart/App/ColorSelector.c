#include "ColorSelector.h"

static uint8_t _step = 0;
static uint8_t r_element = 0xFF, g_element = 0xFF, b_element = 0xFF;
static float red_prop = 1.0f, blue_prop = 1.0f, green_prop = 1.0f;

uint32_t ColourSelectLoop(uint8_t luminance)
{
	switch(_step) {
		case 0:
			g_element --;
			b_element --;
			if(b_element == 0) _step ++;
		break;
		case 1:
			b_element ++;
			if(b_element == 0xFF) _step ++;
		break;
		case 2:
			r_element --;
			if(r_element == 0) _step ++;
		break;
		case 3:
			g_element ++;
			if(g_element == 0xFF) _step ++;
		break;
		case 4:
			b_element --;
			if(b_element == 0) _step ++;
		break;
		case 5:
			r_element ++;
			if(r_element == 0xFF) _step ++;
		break;
		case 6:
			g_element --;
			if(g_element == 0) _step ++;
		break;
		case 7:
			g_element ++;
			b_element ++;
			if(b_element == 0xFF) _step = 0;
		break;
		default:
			r_element = 0xFF;
			g_element = 0;
			b_element = 0;
			_step  = 0;
		break;
	}

	red_prop = (float)r_element / 255.0f;
	green_prop = (float)g_element / 255.0f;
	blue_prop = (float)b_element / 255.0f;

	return ComputeColour(luminance);
//	return COMBINE_RGB(r_element, g_element, b_element);
}

uint32_t ComputeColour(uint8_t luminance)
{
	uint8_t r = Curve_Ctrl(red_prop * luminance);
	uint8_t g = Curve_Ctrl(green_prop * luminance);
	uint8_t b = Curve_Ctrl(blue_prop * luminance);

	return COMBINE_RGB(r, g, b);
}
