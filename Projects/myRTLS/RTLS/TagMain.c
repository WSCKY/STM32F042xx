#include "TagMain.h"

#if (INSTANCE_MODE_TAG)

/* alloc static memory to store the RX frame data. */
static FrameDataUnion _frameRX = {0};
/* alloc static memory where stored TX frame data. */
static FrameDataUnion _frameTX = {0};
/* Frame sequence number, incremented after each transmission. */
static uint8_t frame_seq_nb = 0;
/* Hold copy of status register state here for reference so that it can be examined at a debug breakpoint. */
static uint32_t status_reg = 0;
/* Valid resp msg counter */
static uint8_t resp_recv_cnt = 0;
/* Store the information report by resp msg. */
struct { uint8_t srcAddr; float dist; uint64_t rx_ts; } resp_save[SUPPORT_MAX_ANCHORS] = {0};

/* Time-stamps of frames transmission/reception, expressed in device time units.
 * As they are 40-bit wide, we need to define a 64-bit int type to handle them. */
static uint64_t poll_tx_ts;
static uint64_t final_tx_ts;

static uint16_t FrameRate = 0;
static uint16_t FrameCounter = 0;

#if !(DATA_PRINT_MONITOR)
static CommPackageDef TxPacket = {Header1, Header2};
#endif /* !(DATA_PRINT_MONITOR) */

/* Declaration of static functions. */
static uint64_t get_tx_timestamp_u64(void);
static uint64_t get_rx_timestamp_u64(void);
static uint64_t get_sys_timestamp_u64(void);

static void rtls_init(void)
{
  _frameTX.Frame.stx = DEFAULT_STX;
  _frameTX.Frame.fType = poll_msg;
  _frameTX.Frame.SepNbr = frame_seq_nb;
}

/**
  * @brief  send poll message loop.(65ms + 50ms = 115ms)
  * @param  None
  * @retval None
  */
static void poll_send_loop(void)
{
  /* Set expected response's delay and timeout. 
   * As this example only handles one incoming frame with always the same delay and timeout, those values can be set here once for all. */
  dwt_setrxaftertxdelay(POLL_TX_TO_RESP_RX_DLY_UUS);
  dwt_setrxtimeout(65000); // Maximum value timeout with DW1000 is 65ms
  _frameTX.Frame.fType = poll_msg;
  _frameTX.Frame.Msg.PollMsg.SrcAddr = INST_TAG_ID;

  resp_recv_cnt = 0; /* reset resp msg recv counter. */

  while(1) {
    _frameTX.Frame.SepNbr = frame_seq_nb;
    /* Write frame data to DW1000 and prepare transmission. See NOTE 8 below. */
    dwt_writetxdata(POLL_MSG_LENGTH, _frameTX.uData, 0); /* Zero offset in TX buffer. */
    dwt_writetxfctrl(POLL_MSG_LENGTH, 0, 1); /* Zero offset in TX buffer, ranging. */

    /* Start transmission, indicating that a response is expected so that reception is enabled automatically after the frame is sent and the delay
     * set by dwt_setrxaftertxdelay() has elapsed. */
    dwt_starttx(DWT_START_TX_IMMEDIATE | DWT_RESPONSE_EXPECTED);

    /* We assume that the transmission is achieved correctly, poll for reception of a frame or error/timeout. See NOTE 9 below. */
    while (!((status_reg = dwt_read32bitreg(SYS_STATUS_ID)) & (SYS_STATUS_RXFCG | SYS_STATUS_ALL_RX_TO | SYS_STATUS_ALL_RX_ERR)))
    { };

    /* Increment frame sequence number after transmission of the poll message (modulo 256). */
    frame_seq_nb ++;

    if (status_reg & SYS_STATUS_RXFCG) {
      /* Retrieve poll transmission timestamp. */
      poll_tx_ts = get_tx_timestamp_u64();
      break;
    } else {
      /* Clear RX error/timeout events in the DW1000 status register. */
      dwt_write32bitreg(SYS_STATUS_ID, SYS_STATUS_ALL_RX_TO | SYS_STATUS_ALL_RX_ERR);
      /* Reset RX to properly reinitialise LDE operation. */
      dwt_rxreset();
      /* Delay a task for a given number of ticks */
      _delay_ms(5);
//      vTaskDelay(5);
    }
  }
}

static void resp_process(FrameDataUnion *pFrameRX, FrameDataUnion *pFrameTX)
{
  uint32_t frame_len;
  uint8_t recv_cnt = SUPPORT_MAX_ANCHORS; /* how many times to receive. */
  do {
    if (status_reg & SYS_STATUS_RXFCG) {
      /* Clear good RX frame event and TX frame sent in the DW1000 status register. */
      dwt_write32bitreg(SYS_STATUS_ID, SYS_STATUS_RXFCG | SYS_STATUS_TXFRS);

      /* A frame has been received, read it into the local buffer. */
      frame_len = dwt_read32bitreg(RX_FINFO_ID) & RX_FINFO_RXFLEN_MASK;
      if (frame_len <= FRAME_MAX_LENGTH) {
        dwt_readrxdata(pFrameRX->uData, frame_len, 0);

        /* Check that the frame is the expected response from the anchor. */
        if(pFrameRX->Frame.stx == DEFAULT_STX && pFrameRX->Frame.fType == resp_msg) {
          if(pFrameRX->Frame.Msg.RespMsg.DstAddr == INST_TAG_ID) { //it's send to me.
            if(resp_recv_cnt < SUPPORT_MAX_ANCHORS) {
              for(uint8_t i = 0; i < resp_recv_cnt; i ++) {
                if(resp_save[i].srcAddr == pFrameRX->Frame.Msg.RespMsg.SrcAddr) {
                  goto processed;
                }
              }
              recv_cnt = SUPPORT_MAX_ANCHORS - pFrameRX->Frame.Msg.RespMsg.SrcAddr;
              resp_save[resp_recv_cnt].srcAddr = pFrameRX->Frame.Msg.RespMsg.SrcAddr; /* who report it. */
              resp_save[resp_recv_cnt].dist = pFrameRX->Frame.Msg.RespMsg.Distance.fData;
              resp_save[resp_recv_cnt].rx_ts = get_rx_timestamp_u64(); /* Retrieve response reception timestamp. */

              pFrameTX->Frame.Msg.FinalMsg.FinalTS[resp_recv_cnt].DstAddr = pFrameRX->Frame.Msg.RespMsg.SrcAddr;
              pFrameTX->Frame.Msg.FinalMsg.FinalTS[resp_recv_cnt].tRspRX2PolTX = resp_save[resp_recv_cnt].rx_ts - poll_tx_ts;
              resp_recv_cnt ++;
            } else {

            }
          }
        }
      }
    } else {
      /* Clear RX error/timeout events in the DW1000 status register. */
      dwt_write32bitreg(SYS_STATUS_ID, SYS_STATUS_ALL_RX_TO | SYS_STATUS_ALL_RX_ERR);
    }
processed:
    recv_cnt --;
    if(recv_cnt) {
      dwt_setrxtimeout(TAG_PROC_RESP_RX_DLY_UUS);
      /* Activate reception immediately. */
      dwt_rxenable(DWT_START_RX_IMMEDIATE);

      /* Poll for reception of a frame or error/timeout. See NOTE 8 below. */
      while (!((status_reg = dwt_read32bitreg(SYS_STATUS_ID)) & (SYS_STATUS_RXFCG | SYS_STATUS_ALL_RX_TO | SYS_STATUS_ALL_RX_ERR)))
      { };
    }
  } while(recv_cnt);
}

static void final_send(FrameDataUnion *pFrameTX)
{
  uint32_t final_tx_time;
  /* Compute final message transmission time. See NOTE 10 below. */
  final_tx_time = (get_sys_timestamp_u64() + (RESP_RX_TO_FINAL_TX_DLY_UUS * UUS_TO_DWT_TIME)) >> 8;
  dwt_setdelayedtrxtime(final_tx_time);

  /* Final TX timestamp is the transmission time we programmed plus the TX antenna delay. */
  final_tx_ts = (((uint64)(final_tx_time & 0xFFFFFFFEUL)) << 8) + TX_ANT_DLY;

  pFrameTX->Frame.SepNbr = frame_seq_nb;
  pFrameTX->Frame.fType = final_msg;
  pFrameTX->Frame.Msg.FinalMsg.SrcAddr = INST_TAG_ID;
  pFrameTX->Frame.Msg.FinalMsg.TS_Number = resp_recv_cnt;
//  for(uint8_t i = 0; i < resp_recv_cnt; i ++) {
//    for(uint8_t j = 0; j < resp_recv_cnt; j ++) {
//      if(pFrameTX->Frame.Msg.FinalMsg.FinalTS[i].DstAddr == resp_save[j].srcAddr) {
//        pFrameTX->Frame.Msg.FinalMsg.FinalTS[i].tFinTX2RspRX = final_tx_ts - resp_save[j].rx_ts;
//      }
//    }
//  }
  for(uint8_t i = 0; i < resp_recv_cnt; i ++) {
    pFrameTX->Frame.Msg.FinalMsg.FinalTS[i].tFinTX2RspRX = final_tx_ts - resp_save[i].rx_ts;
  }

  /* Write and send final message. See NOTE 8 below. */
  dwt_writetxdata(FINAL_MSG_LENGTH, _frameTX.uData, 0); /* Zero offset in TX buffer. */
  dwt_writetxfctrl(FINAL_MSG_LENGTH, 0, 1); /* Zero offset in TX buffer, ranging. */
  /* If dwt_starttx() returns an error, abandon this ranging exchange and proceed to the next one. See NOTE 12 below. */
  if(dwt_starttx(DWT_START_TX_DELAYED) == DWT_SUCCESS) {
    /* Poll DW1000 until TX frame sent event set. See NOTE 9 below. */
    while (!(dwt_read32bitreg(SYS_STATUS_ID) & SYS_STATUS_TXFRS))
    { };

    /* Clear TXFRS event. */
    dwt_write32bitreg(SYS_STATUS_ID, SYS_STATUS_TXFRS);

    /* Increment frame sequence number after transmission of the final message (modulo 256). */
    frame_seq_nb ++;
  }
  /* wait for anchor process. */
  _delay_ms(2);
//  vTaskDelay(2);
}

static void tag_rtls_run(void)
{
  poll_send_loop();
  resp_process(&_frameRX, &_frameTX);
  if(resp_recv_cnt) {
    final_send(&_frameTX);
    FrameCounter ++;
  } else {

  }
}

/*! ------------------------------------------------------------------------------------------------------------------
 * @fn get_tx_timestamp_u64()
 *
 * @brief Get the TX time-stamp in a 64-bit variable.
 *        /!\ This function assumes that length of time-stamps is 40 bits, for both TX and RX!
 *
 * @param  none
 *
 * @return  64-bit value of the read time-stamp.
 */
static uint64_t get_tx_timestamp_u64(void)
{
    uint8 ts_tab[5];
    uint64_t ts = 0;
    int i;
    dwt_readtxtimestamp(ts_tab);
    for (i = 4; i >= 0; i--) {
        ts <<= 8;
        ts |= ts_tab[i];
    }
    return ts;
}

/*! ------------------------------------------------------------------------------------------------------------------
 * @fn get_rx_timestamp_u64()
 *
 * @brief Get the RX time-stamp in a 64-bit variable.
 *        /!\ This function assumes that length of time-stamps is 40 bits, for both TX and RX!
 *
 * @param  none
 *
 * @return  64-bit value of the read time-stamp.
 */
static uint64_t get_rx_timestamp_u64(void)
{
    uint8 ts_tab[5];
    uint64_t ts = 0;
    int i;
    dwt_readrxtimestamp(ts_tab);
    for (i = 4; i >= 0; i--) {
        ts <<= 8;
        ts |= ts_tab[i];
    }
    return ts;
}

/*! ------------------------------------------------------------------------------------------------------------------
 * @fn get_sys_timestamp_u64()
 *
 * @brief Get the system time-stamp in a 64-bit variable.
 *        /!\ This function assumes that length of time-stamps is 40 bits, for both TX and RX!
 *
 * @param  none
 *
 * @return  64-bit value of the read time-stamp.
 */
static uint64_t get_sys_timestamp_u64(void)
{
    uint8 ts_tab[5];
    uint64_t ts = 0;
    int i;
    dwt_readsystime(ts_tab);
    for (i = 4; i >= 0; i--) {
        ts <<= 8;
        ts |= ts_tab[i];
    }
    return ts;
}

/**
  * @brief task entry function.
  * @param[in] pvParameter   Pointer that will be used as the parameter for the task.
  */
void tag_rtls_task_function(void * pvParameter)
{
  UNUSED_PARAMETER(pvParameter);
  dwt_setleds(DWT_LEDS_ENABLE);
#if !(DATA_PRINT_MONITOR)
  TxPacket.Packet.type = TYPE_DIST_GROUP_Resp;
  TxPacket.Packet.len = sizeof(DistGroupDataRespDef) + 2;
  TxPacket.Packet.PacketData.DistGroup.TagAddr = INST_TAG_ID;
#endif /* !(DATA_PRINT_MONITOR) */
  rtls_init();
  /* Set expected response's delay and timeout. 
   * As this example only handles one incoming frame with always the same delay and timeout, those values can be set here once for all. */
  dwt_setrxaftertxdelay(POLL_TX_TO_RESP_RX_DLY_UUS);
  dwt_setrxtimeout(65000); // Maximum value timeout with DW1000 is 65ms  

  /* Loop forever initiating ranging exchanges. */
  while (true)
  {
    tag_rtls_run();
#if (DATA_PRINT_MONITOR)
    for(uint8_t i = 0; i < resp_recv_cnt; i ++) {
      MonitorUpdateDataPos(resp_save[i].dist, resp_save[i].srcAddr);
    }
    MonitorUpdateDataPos(resp_recv_cnt, SUPPORT_MAX_ANCHORS);
    MonitorUpdateDataPos(FrameRate, 4);
#else
    TxPacket.Packet.PacketData.DistGroup.DistNum = resp_recv_cnt;
    TxPacket.Packet.PacketData.DistGroup.FrameRate = FrameRate;
    for(uint8_t i = 0; i < resp_recv_cnt; i ++) {
      TxPacket.Packet.PacketData.DistGroup.DistGroup[i].AncAddr = resp_save[i].srcAddr;
      TxPacket.Packet.PacketData.DistGroup.DistGroup[i].Distance = resp_save[i].dist;
    }
    SendTxPacket(&TxPacket);
#endif /* (DATA_PRINT_MONITOR) */
    /* Tasks must be implemented to never return... */
  }
}

void FrameRateCountCallback(uint8_t seconds)
{
  FrameRate = FrameCounter / seconds;
  FrameCounter = 0;
}

/*****************************************************************************************************************************************************
 * NOTES:
 *
 * 1. The sum of the values is the TX to RX antenna delay, experimentally determined by a calibration process. Here we use a hard coded typical value
 *    but, in a real application, each device should have its own antenna delay properly calibrated to get the best possible precision when performing
 *    range measurements.
 * 2. The messages here are similar to those used in the DecaRanging ARM application (shipped with EVK1000 kit). They comply with the IEEE
 *    802.15.4 standard MAC data frame encoding and they are following the ISO/IEC:24730-62:2013 standard. The messages used are:
 *     - a poll message sent by the initiator to trigger the ranging exchange.
 *     - a response message sent by the responder allowing the initiator to go on with the process
 *     - a final message sent by the initiator to complete the exchange and provide all information needed by the responder to compute the
 *       time-of-flight (distance) estimate.
 *    The first 10 bytes of those frame are common and are composed of the following fields:
 *     - byte 0/1: frame control (0x8841 to indicate a data frame using 16-bit addressing).
 *     - byte 2: sequence number, incremented for each new frame.
 *     - byte 3/4: PAN ID (0xDECA).
 *     - byte 5/6: destination address, see NOTE 3 below.
 *     - byte 7/8: source address, see NOTE 3 below.
 *     - byte 9: function code (specific values to indicate which message it is in the ranging process).
 *    The remaining bytes are specific to each message as follows:
 *    Poll message:
 *     - no more data
 *    Response message:
 *     - byte 10: activity code (0x02 to tell the initiator to go on with the ranging exchange).
 *     - byte 11/12: activity parameter, not used here for activity code 0x02.
 *    Final message:
 *     - byte 10 -> 13: poll message transmission timestamp.
 *     - byte 14 -> 17: response message reception timestamp.
 *     - byte 18 -> 21: final message transmission timestamp.
 *    All messages end with a 2-byte checksum automatically set by DW1000.
 * 3. Source and destination addresses are hard coded constants in this example to keep it simple but for a real product every device should have a
 *    unique ID. Here, 16-bit addressing is used to keep the messages as short as possible but, in an actual application, this should be done only
 *    after an exchange of specific messages used to define those short addresses for each device participating to the ranging exchange.
 * 4. Delays between frames have been chosen here to ensure proper synchronisation of transmission and reception of the frames between the initiator
 *    and the responder and to ensure a correct accuracy of the computed distance. The user is referred to DecaRanging ARM Source Code Guide for more
 *    details about the timings involved in the ranging process.
 * 5. This timeout is for complete reception of a frame, i.e. timeout duration must take into account the length of the expected frame. Here the value
 *    is arbitrary but chosen large enough to make sure that there is enough time to receive the complete response frame sent by the responder at the
 *    110k data rate used (around 3 ms).
 * 6. The preamble timeout allows the receiver to stop listening in situations where preamble is not starting (which might be because the responder is
 *    out of range or did not receive the message to respond to). This saves the power waste of listening for a message that is not coming. We
 *    recommend a minimum preamble timeout of 5 PACs for short range applications and a larger value (e.g. in the range of 50% to 80% of the preamble
 *    length) for more challenging longer range, NLOS or noisy environments.
 * 7. In a real application, for optimum performance within regulatory limits, it may be necessary to set TX pulse bandwidth and TX power, (using
 *    the dwt_configuretxrf API call) to per device calibrated values saved in the target system or the DW1000 OTP memory.
 * 8. dwt_writetxdata() takes the full size of the message as a parameter but only copies (size - 2) bytes as the check-sum at the end of the frame is
 *    automatically appended by the DW1000. This means that our variable could be two bytes shorter without losing any data (but the sizeof would not
 *    work anymore then as we would still have to indicate the full length of the frame to dwt_writetxdata()).
 * 9. We use polled mode of operation here to keep the example as simple as possible but all status events can be used to generate interrupts. Please
 *    refer to DW1000 User Manual for more details on "interrupts". It is also to be noted that STATUS register is 5 bytes long but, as the event we
 *    use are all in the first bytes of the register, we can use the simple dwt_read32bitreg() API call to access it instead of reading the whole 5
 *    bytes.
 * 10. As we want to send final TX timestamp in the final message, we have to compute it in advance instead of relying on the reading of DW1000
 *     register. Timestamps and delayed transmission time are both expressed in device time units so we just have to add the desired response delay to
 *     response RX timestamp to get final transmission time. The delayed transmission time resolution is 512 device time units which means that the
 *     lower 9 bits of the obtained value must be zeroed. This also allows to encode the 40-bit value in a 32-bit words by shifting the all-zero lower
 *     8 bits.
 * 11. In this operation, the high order byte of each 40-bit timestamps is discarded. This is acceptable as those time-stamps are not separated by
 *     more than 2**32 device time units (which is around 67 ms) which means that the calculation of the round-trip delays (needed in the
 *     time-of-flight computation) can be handled by a 32-bit subtraction.
 * 12. When running this example on the EVB1000 platform with the RESP_RX_TO_FINAL_TX_DLY response delay provided, the dwt_starttx() is always
 *     successful. However, in cases where the delay is too short (or something else interrupts the code flow), then the dwt_starttx() might be issued
 *     too late for the configured start time. The code below provides an example of how to handle this condition: In this case it abandons the
 *     ranging exchange to try another one after 1 second. If this error handling code was not here, a late dwt_starttx() would result in the code
 *     flow getting stuck waiting for a TX frame sent event that will never come. The companion "responder" example (ex_05b) should timeout from
 *     awaiting the "final" and proceed to have its receiver on ready to poll of the following exchange.
 * 13. The user is referred to DecaRanging ARM application (distributed with EVK1000 product) for additional practical example of usage, and to the
 *     DW1000 API Guide for more details on the DW1000 driver functions.
 ****************************************************************************************************************************************************/

#endif /* (INSTANCE_MODE_TAG) */
