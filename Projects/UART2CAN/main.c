/**
  ******************************************************************************
  * @file    ./main.c 
  * @author  kyChu
  * @version V1.0.0
  * @date    17-April-2018
  * @brief   Main program body
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static _FLOAT_UNION _tof_data = {0}, _ult_data = {0};
static uint8_t HW_ID = 0x0;
static CanTxMsg TxMessage = {0};
//static CanRxMsg *pRxMessage = 0;

static uint8_t IndicatorDiv = 0;
static uint8_t DataUpdateFlag = 0;
static uint32_t LastTime = 0, CurrentTime = 0;
/* Private function prototypes -----------------------------------------------*/
static void CAN_MsgUpdate(void);
static void CAN_MsgPrepare(void);
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
int main(void)
{
  /*!< At this stage the microcontroller clock setting is already configured, 
       this is done through SystemInit() function which is called from startup
       file (startup_stm32f0xx.s) before to branch to application main.
       To reconfigure the default setting of SystemInit() function, refer to
       system_stm32f0xx.c file
     */
	_TimeTicksInit();

	LED_Init();
	Delay_Init();
	HW_ID_Init();
	CAN_If_Init();
	TOFDriverInit();

	UltDriverInit();

	DelayTicks(100);
	HW_ID = ReadHW_ID();

	CAN_MsgPrepare();
//	pRxMessage = GetCAN_RxMsg();

	CurrentTime = _Get_Micros();
	LastTime = CurrentTime;

	for(;;) {
		CurrentTime = _Get_Micros();
		DataUpdateFlag = GetNewTOFData(&(_tof_data.fData)) | GetNewUltData(&(_ult_data.fData));
		if(DataUpdateFlag) {
			LastTime = CurrentTime;
		} else if((CurrentTime - LastTime) > 60000) {
			DataUpdateFlag = 1;
			LastTime = CurrentTime;
		}

		if(DataUpdateFlag) {
			DataUpdateFlag = 0;

			CAN_MsgUpdate();
			CAN_TransmitData(&TxMessage);
			/* indicate status after send. */
			IndicatorDiv ++;
			if(IndicatorDiv & 0x04) LED_TOG();
		}

		if(GetMsgUpdateFlag()) {}
  }
}

/**
  * @brief  Transmit Structure preparation.
  * @param  pTxMessage: pointer to the Tx Message.
  * @retval None
  */
static void CAN_MsgPrepare(void)
{
	TxMessage.StdId = 0x5A0 | HW_ID;
	TxMessage.ExtId = 0x00;
	TxMessage.RTR = CAN_RTR_DATA;
	TxMessage.IDE = CAN_ID_STD;
	TxMessage.DLC = 8;

	TxMessage.Data[0] = 0;
	TxMessage.Data[1] = 0;
	TxMessage.Data[2] = 0;
	TxMessage.Data[3] = 0;
	TxMessage.Data[4] = 0;
	TxMessage.Data[5] = 0;
	TxMessage.Data[6] = 0;
	TxMessage.Data[7] = 0;
}

/**
  * @brief  Update CAN Message to send.
  * @param  None
  * @retval None
  */
static void CAN_MsgUpdate(void)
{
	TxMessage.Data[0] = _tof_data.uData[0];
	TxMessage.Data[1] = _tof_data.uData[1];
	TxMessage.Data[2] = _tof_data.uData[2];
	TxMessage.Data[3] = _tof_data.uData[3];
	TxMessage.Data[4] = _ult_data.uData[0];
	TxMessage.Data[5] = _ult_data.uData[1];
	TxMessage.Data[6] = _ult_data.uData[2];
	TxMessage.Data[7] = _ult_data.uData[3];
}

#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

/******************************** END OF FILE *********************************/
