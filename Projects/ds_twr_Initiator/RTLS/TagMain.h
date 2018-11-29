#ifndef __TAGMAIN_H
#define __TAGMAIN_H

#include "FrameStruct.h"
#include "RTLS_Conf.h"

#if (INSTANCE_MODE_TAG)

#include "deca_device_api.h"
#include "deca_regs.h"
#include "port_platform.h"

#if (DATA_PRINT_MONITOR)
#include "DataMonitor.h"
#else
//#include "HostRespTask.h"
#include "CommProtocol.h"
#endif /* (DATA_PRINT_MONITOR) */

//#include "FreeRTOS.h"
//#include "task.h"

void tag_rtls_task_function(void * pvParameter);
void FrameRateCountCallback(uint8_t seconds);

#endif /* (INSTANCE_MODE_TAG) */

#endif /* __TAGMAIN_H */
