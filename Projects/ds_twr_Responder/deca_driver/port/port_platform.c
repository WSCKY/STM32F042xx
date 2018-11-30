/*! ----------------------------------------------------------------------------
 * @file    port_platform.c
 * @brief   HW specific definitions and functions for portability
 *
 * @attention
 *
 * Copyright 2016 (c) DecaWave Ltd, Dublin, Ireland.
 *
 * All rights reserved.
 *
 * @author DecaWave
 */

#include "port_platform.h"
#include "deca_device_api.h"

/* @fn      setup_DW1000RSTnIRQ
 * @brief   setup the DW_RESET pin mode
 *          0 - output Open collector mode
 *          !0 - input mode with connected EXTI0 IRQ
 * */
void setup_DW1000RSTnIRQ(int enable)
{
}

/* @fn      port_wakeup_dw1000
 * @brief   "slow" waking up of DW1000 using DW_CS only
 * */
void port_wakeup_dw1000(void)
{
}

/* @fn      port_wakeup_dw1000_fast
 * @brief   waking up of DW1000 using DW_CS and DW_RESET pins.
 *          The DW_RESET signalling that the DW1000 is in the INIT state.
 *          the total fast wakeup takes ~2.2ms and depends on crystal startup time
 * */
void port_wakeup_dw1000_fast(void)
{
}

/* @fn      reset_DW1000
 * @brief   DW_RESET pin on DW1000 has 2 functions
 *          In general it is output, but it also can be used to reset the digital
 *          part of DW1000 by driving this pin low.
 *          Note, the DW_RESET pin should not be driven high externally.
 * */
void reset_DW1000(void)
{
  DW1000RSTn_DrvLow();
  _delay_ms(2);
  DW1000RSTn_NOP_IN();
  _delay_ms(2);
}

/* @fn      port_set_dw1000_slowrate
 * @brief   set 2MHz
 *          n
 * */
void port_set_dw1000_slowrate(void)
{
  SPI_ConfigFastRate(DW1000_SPI_LOW);
	_delay_ms(2);	
}

/* @fn      port_set_dw1000_fastrate
 * @brief   set 8MHz
 *         
 * */
void port_set_dw1000_fastrate(void)
{
  SPI_ConfigFastRate(DW1000_SPI_HIGH);
	_delay_ms(2);	
}


void deca_sleep(unsigned int time_ms)
{
  _delay_ms(time_ms);
}

// currently do nothing
decaIrqStatus_t decamutexon(void)           
{
  return 0;	
}

// currently do nothing
void decamutexoff(decaIrqStatus_t s)       
{
}
