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
USB_CORE_HANDLE  USB_Device_dev;

static dwt_config_t config = {
    5,                /* Channel number. */
    DWT_PRF_64M,      /* Pulse repetition frequency. */
    DWT_PLEN_128,     /* Preamble length. Used in TX only. */
    DWT_PAC8,         /* Preamble acquisition chunk size. Used in RX only. */
    10,               /* TX preamble code. Used in TX only. */
    10,               /* RX preamble code. Used in RX only. */
    0,                /* 0 to use standard SFD, 1 to use non-standard SFD. */
    DWT_BR_6M8,       /* Data rate. */
    DWT_PHRMODE_STD,  /* PHY header mode. */
    (129 + 8 - 8)     /* SFD timeout (preamble length + 1 + SFD length - PAC size). Used in RX only. */
};
/* Private function prototypes -----------------------------------------------*/
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

  /* The Application layer has only to call USBD_Init to 
  initialize the USB low level driver, the USB device library, the USB clock 
  ,pins and interrupt service routine (BSP) to start the Library*/
  USBD_Init(&USB_Device_dev,
            &USR_desc, 
            &USBD_CDC_cb, 
            &USR_cb);

	LED_Init();
	Delay_Init();
  DW1000_If_Init();

  /*
   * DW1000 Initialize.
   */
  /* Setup DW1000 IRQ pin */  
  DW1000RSTn_NOP_IN();
  /* Reset DW1000 */
  reset_DW1000();
  /* Set SPI clock to 3MHz */
  port_set_dw1000_slowrate();
  /* Init the DW1000 */
  if (dwt_initialise(DWT_LOADUCODE) == DWT_ERROR) {
    //Init of DW1000 Failed
    while (1) {
      LED_TOG();
      DelayTicks(50);
    };
  }

  // Set SPI to 8MHz clock
  port_set_dw1000_fastrate();

  /* Configure DW1000. */
  dwt_configure(&config);

  /* Apply default antenna delay value. See NOTE 2 below. */
  dwt_setrxantennadelay(RX_ANT_DLY);
  dwt_settxantennadelay(TX_ANT_DLY);

  /* Set preamble timeout for expected frames. See NOTE 3 below. */
  //dwt_setpreambledetecttimeout(0); // PRE_TIMEOUT

  //-------------dw1000  ini------end---------------------------
  // IF WE GET HERE THEN THE LEDS WILL BLINK
  ds_responder_task_function(0);
  while(1) {
    LED_TOG();
    DelayTicks(50);
  }
}

uint32_t tim_1s_cnt = 0;
void SysTickIrqCallback(void)
{
  if(tim_1s_cnt < 1000)
    tim_1s_cnt ++;
  else {
    tim_1s_cnt = 0;
//    FrameRateCountCallback(1);
    LED_TOG();
  }
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
