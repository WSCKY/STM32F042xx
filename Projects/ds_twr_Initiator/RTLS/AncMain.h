#ifndef __ANCMAIN_H
#define __ANCMAIN_H

#include "FrameStruct.h"
#include "RTLS_Conf.h"

#if !(INSTANCE_MODE_TAG)

#include "deca_device_api.h"
#include "deca_regs.h"
#include "port_platform.h"

#if (DATA_PRINT_MONITOR)
#include "DataMonitor.h"
#else
#include "HostRespTask.h"
#endif /* (DATA_PRINT_MONITOR) */

#include "FreeRTOS.h"
#include "task.h"

void anc_rtls_task_function(void * pvParameter);

#endif /* !(INSTANCE_MODE_TAG) */

#endif /* __ANCMAIN_H */
