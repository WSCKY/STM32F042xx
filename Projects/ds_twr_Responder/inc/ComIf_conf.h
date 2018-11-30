#ifndef __COMIF_CONF_H
#define __COMIF_CONF_H

#include "SysConfig.h"

#include "ComDataTypes.h"

#include "usbd_usr.h"
#include "usbd_cdc_vcp.h"

#define COM_IF_TX_BYTES(p, l)          USB_CDC_SendBufferFast(p, l)
#define COM_IF_TX_CHECK()              USBD_isEnabled()

#define COM_USER_TYPE                  TYPE_DIST_Response = 0x11, \
                                       TYPE_DIST_GROUP_Resp = 0x12,

#define COM_USER_TYPE_DATA             DistDataRespDef DistData; \
                                       DistGroupDataRespDef DistGroup;

#endif /* __COMIF_CONF_H */
