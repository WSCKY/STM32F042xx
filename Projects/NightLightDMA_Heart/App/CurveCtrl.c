/**
  ******************************************************************************
  * @file    control.c
  * @author  '^_^'
  * @version V0.0.0
  * @date    2-July-2015
  * @brief   curve control.
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "CurveCtrl.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
const uint16_t PointX[] = {0, 32, 64, 96, 128, 160, 192, 224, 256};
const uint16_t PointY[] = {0, 14, 33, 58, 86, 118, 156, 200, 256};
//const uint16_t PointY[] = {0, 125, 250, 375, 500, 625, 750, 875, 1000};

uint16_t Curve_K[8];
uint16_t Curve_B[8];
/* Private function prototypes -----------------------------------------------*/
static uint16_t Get_64K(uint16_t Ax, uint16_t Ay, uint16_t Bx, uint16_t By);
static uint16_t Get_64B(uint16_t Ax, uint16_t Ay, uint16_t Bx, uint16_t By);
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Control by Curve.//Y = Kx + B
  * @param  
  * @retval 
  */
uint16_t Curve_Ctrl(uint16_t mdata)
{
	uint16_t tmpY;
	uint8_t Curve_index;

	Curve_index = mdata / 32;//0 - 7
	if(Curve_index > 7)
		Curve_index = 7;
	tmpY = (mdata * Curve_K[Curve_index]) + Curve_B[Curve_index];//Y = Kx + B
	tmpY = tmpY / 50.0f;
	return tmpY;
}

/**
  * @brief  Calculate K.
  * @param  
  * @retval 
  */
static uint16_t Get_64K(uint16_t Ax, uint16_t Ay, uint16_t Bx, uint16_t By)
{
	uint16_t Tmp1, Tmp2;
	Tmp1 = By - Ay;
	Tmp2 = Bx - Ax;
	return (Tmp1 * 50 / Tmp2);
}

/**
  * @brief  Calculate B.
  * @param  
  * @retval 
  */
static uint16_t Get_64B(uint16_t Ax, uint16_t Ay, uint16_t Bx, uint16_t By)
{
	uint16_t Tmp1, Tmp2, TmpK;
	Tmp1 = By - Ay;
	Tmp2 = Bx - Ax;
	TmpK = (Tmp1 * 50 / Tmp2);
	return ((Ay * 50 - (Ax * TmpK)));
}

/**
  * @brief  Calculate curve parameter.
  * @param  None
  * @retval None
  */
void CalCtrlCurve(void)
{
	uint8_t Index;
	for(Index = 0; Index < 8; Index ++)
	{
			Curve_K[Index] = Get_64K(PointX[Index], PointY[Index], PointX[Index + 1], PointY[Index + 1]);
			Curve_B[Index] = Get_64B(PointX[Index], PointY[Index], PointX[Index + 1], PointY[Index + 1]);
	}
}

/******************************** END OF FILE *********************************/
