/*! ----------------------------------------------------------------------------
 * @file    port_platform.h
 * @brief   HW specific definitions and functions for portability
 *
 * @attention
 *
 * Copyright 2015 (c) DecaWave Ltd, Dublin, Ireland.
 *
 * All rights reserved.
 *
 * @author DecaWave
 */


#ifndef PORT_PLATFORM_H_
#define PORT_PLATFORM_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <string.h>

#include "deca_types.h" 
//#include "nrf_drv_spi.h"
//#include "app_util_platform.h"
//#include "nrf_gpio.h"
//#include "nrf_delay.h"
//#include "nrf_log.h"
//#include "boards.h"
//#include "app_error.h"
#include "DW1000.h"
#include "TimerCounter.h"
#include "deca_device_api.h"

#if !defined(true)
#define true                           (0x1)
#endif
#if !defined(false)
#define false                          (0x0)
#endif

/*

Note: Antenna Delay Values	
The sum of the values is the TX to RX antenna delay, this should be experimentally determined by a calibration process. Here we use a hard coded
value (expected to be a little low so a positive error will be seen on the resultant distance estimate. For a real production application, each
device should have its own antenna delay properly calibrated to get good precision when performing range measurements.   

*/

/* Default antenna delay values for 64 MHz PRF.*/
//#define TX_ANT_DLY 16456
//#define RX_ANT_DLY 16456	

	
//int readfromspi(uint16 headerLength, const uint8 *headerBuffer, uint32 readlength, uint8 *readBuffer);
//int writetospi( uint16 headerLength, const uint8 *headerBuffer, uint32 bodylength, const uint8 *bodyBuffer);
decaIrqStatus_t decamutexon(void);
void decamutexoff(decaIrqStatus_t s);

#define writetospi                     DW1000_SPI_Write
#define readfromspi                    DW1000_SPI_Read

/*****************************************************************************************************************//*
**/

 /****************************************************************************//**
  *
  *                                 Types definitions
  *
  *******************************************************************************/
typedef uint64_t        uint64 ;

typedef int64_t         int64 ;


#ifndef FALSE
#define FALSE               0
#endif

#ifndef TRUE
#define TRUE                1
#endif


/****************************************************************************//**
 *
 *                              MACRO function
 *
 *******************************************************************************/


/****************************************************************************//**
 *
 *                              port function prototypes
 *
 *******************************************************************************/

void Sleep(uint32_t Delay);
unsigned long portGetTickCnt(void);

void port_wakeup_dw1000(void);
void port_wakeup_dw1000_fast(void);

void port_set_dw1000_slowrate(void);
void port_set_dw1000_fastrate(void);

void process_dwRSTn_irq(void);
void process_deca_irq(void);


void setup_DW1000RSTnIRQ(int enable);

void reset_DW1000(void);

#if 0
ITStatus EXTI_GetITEnStatus(uint32_t x);

uint32_t port_GetEXT_IRQStatus(void);
uint32_t port_CheckEXT_IRQ(void);
void port_DisableEXT_IRQ(void);
void port_EnableEXT_IRQ(void);
extern uint32_t     HAL_GetTick(void);

#endif

#ifdef __cplusplus
}
#endif

#endif /* PORT_PLATFORM_H_ */
/*
 * Taken from the Linux Kernel
 *
 */

#ifndef _LINUX_CIRC_BUF_H
#define _LINUX_CIRC_BUF_H 1

struct circ_buf {
    char *buf;
    int head;
    int tail;
};

/* Return count in buffer.  */
#define CIRC_CNT(head,tail,size) (((head) - (tail)) & ((size)-1))

/* Return space available, 0..size-1.  We always leave one free char
   as a completely full buffer has head == tail, which is the same as
   empty.  */
#define CIRC_SPACE(head,tail,size) CIRC_CNT((tail),((head)+1),(size))

/* Return count up to the end of the buffer.  Carefully avoid
   accessing head and tail more than once, so they can change
   underneath us without returning inconsistent results.  */
#define CIRC_CNT_TO_END(head,tail,size) \
    ({int end = (size) - (tail); \
      int n = ((head) + end) & ((size)-1); \
      n < end ? n : end;})

/* Return space available up to the end of the buffer.  */
#define CIRC_SPACE_TO_END(head,tail,size) \
    ({int end = (size) - 1 - (head); \
      int n = (end + (tail)) & ((size)-1); \
      n <= end ? n : end+1;})

#endif /* _LINUX_CIRC_BUF_H  */

