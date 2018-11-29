#ifndef __MONITOR_IF_CONF_H
#define __MONITOR_IF_CONF_H

#include "SysConfig.h"

#include "usb_conf.h"
#include "usbd_usr.h"
#include "usbd_cdc_vcp.h"

#define MONITOR_IF_TX_BYTES(p, l)      USB_CDC_SendBufferFast(p, l)
#define MONITOR_IF_CHECK()             USBD_isEnabled()

#endif /* __MONITOR_IF_CONF_H */
