#ifndef __FRAMESTRUCT_H
#define __FRAMESTRUCT_H

#include "RTLS_Conf.h"
#include "PlatformConfig.h"

#ifndef __packed
  /* __packed keyword used to decrease the data type alignment to 1-byte */
  #if defined (__CC_ARM)         /* ARM Compiler */
    #define __packed    __packed
  #elif defined (__ICCARM__)     /* IAR Compiler */
    #define __packed    __packed
  #elif defined   ( __GNUC__ )   /* GNU Compiler */                        
    #define __packed    __attribute__ ((__packed__))
  #elif defined   (__TASKING__)  /* TASKING Compiler */
    #define __packed    __unaligned
  #endif /* __CC_ARM */
#endif

#define FRAME_MAX_LENGTH               (12 + SUPPORT_MAX_ANCHORS * 9)

#define DEFAULT_STX                    (0x5A)

typedef enum {
  poll_msg = 0x1,
  resp_msg = 0x2,
  final_msg = 0x3
} FrameTypeDef;

/*
the tag sends (broadcasts) a single Poll message received by all anchors
*/
__packed typedef struct {
  uint8_t SrcAddr;
} PollMsgDataDef; /* send by tag */

__packed typedef struct {
  uint8_t DstAddr;
  uint8_t SrcAddr;
/*
in order to gather the TOF data for solving, and rather than employing any backhaul infrastructure, 
each anchor sends the TOF that it calculates for a tag, back to the tag in the next Response message to that tag. 
So, as each tag periodically ranges to the anchors, it gets the results from one period during the very next period.
*/
  FloatUnion Distance; /* calculated time-of-flight */
} RespMsgDataDef; /* send by anchor */

/*
In the Final message payload, the tag sends the actual transmission time of the Final message itself, 
along with the send time of the Poll and the times at which it received the responses from each of the anchors.
*/
__packed typedef struct {
  uint8_t SrcAddr;
  uint8_t TS_Number;
  __packed struct {
    uint8_t DstAddr;
    uint32_t tRspRX2PolTX;
    uint32_t tFinTX2RspRX;
  } FinalTS[SUPPORT_MAX_ANCHORS];
} FinalMsgDataDef; /* send by tag */
/*
Each anchor receiving the Final message then has sufficient information to calculate the time-of-flight (TOF) between itself and the tag, 
(assuming that the anchor correctly received the Poll and that the tag correctly received the anchor's Response).
*/

__packed typedef union {
  PollMsgDataDef PollMsg;
  RespMsgDataDef RespMsg;
  FinalMsgDataDef FinalMsg;
} MsgDataDef;

__packed typedef struct {
  uint8_t stx;
  uint8_t SepNbr;
  FrameTypeDef fType;
  MsgDataDef Msg;
  uint16_t CRC16;
} FrameDataDef;

__packed typedef union {
  FrameDataDef Frame;
  uint8_t uData[FRAME_MAX_LENGTH];
} FrameDataUnion;

#define POLL_MSG_LENGTH                (6)
#define RESP_MSG_LENGTH                (11)
#define FINAL_MSG_LENGTH               (7  + SUPPORT_MAX_ANCHORS * 9)

#endif /* __FRAMESTRUCT_H */
