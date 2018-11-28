#ifndef __RTLS_CONF_H
#define __RTLS_CONF_H

#include "SysConfig.h"

#define ANC_ADDR_DEFAULT               (0x0)
#define ANC_ADDR_MASK                  (0x7F)
#define IS_ANC_ADDR(x)                 (!((x) & 0x80))

#define TAG_ADDR_DEFAULT               (0x80)
#define TAG_ADDR_MASK                  (0x80)
#define IS_TAG_ADDR(x)                 ((x) & 0x80)

/*Should be accurately calculated during calibration*/
#define TX_ANT_DLY 16456
#define RX_ANT_DLY 16456

/* UWB microsecond (uus) to device time unit (dtu, around 15.65 ps) conversion factor.
 * 1 uus = 512 / 499.2us and 1us = 499.2 * 128 dtu. */
#define UUS_TO_DWT_TIME 65536

/* Speed of light in air, in metres per second. */
#define SPEED_OF_LIGHT 299702547

/* Delay between frames, in UWB microseconds. See NOTE 4 below. */
/* This is the delay from Frame RX timestamp to TX reply timestamp used for calculating/setting the DW1000's delayed TX function. This includes the
 * frame length of approximately 2.46 ms with above configuration. */
#define POLL_RX_TO_RESP_TX_DLY_UUS 2600
/* This is the delay from the end of the frame transmission to the enable of the receiver, as programmed for the DW1000's wait for response feature. */
#define RESP_TX_TO_FINAL_RX_DLY_UUS 500
/* Receive final timeout. See NOTE 5 below. */
#define FINAL_RX_TIMEOUT_UUS 600
/* This is the delay from Frame RX timestamp to TX reply timestamp used for calculating/setting the DW1000's delayed TX function. This includes the
 * frame length of approximately 2.66 ms with above configuration. */
#define RESP_RX_TO_FINAL_TX_DLY_UUS 400
/* Delay between frames, in UWB microseconds. See NOTE 1 below. */
#define POLL_TX_TO_RESP_RX_DLY_UUS 100
/* This is the delay wait for TAG process the resp message(include resp send time). */
#define TAG_PROC_RESP_RX_DLY_UUS 400

#define INSTANCE_MODE_TAG              (1)
#define SUPPORT_MAX_ANCHORS            (3)

#if (INSTANCE_MODE_TAG)
#define INST_TAG_ID                    (0 | TAG_ADDR_MASK)
#else
#define INST_ANC_ID                    (2 & ANC_ADDR_MASK)
#endif /* (INSTANCE_MODE_TAG) */

#endif /* __RTLS_CONF_H */
