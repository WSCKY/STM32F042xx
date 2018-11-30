#ifndef __DATAMONITOR_H
#define __DATAMONITOR_H

#include "Monitor_If_conf.h"

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

#define MONITOR_DATA_LENGTH            (9)

__packed typedef struct
{
	uint8_t head;
	uint8_t cmd;
	uint8_t length;
	__packed union{
			uint8_t c_data[4];
			float f_data;	
	}data[MONITOR_DATA_LENGTH];
	uint8_t checksum;
} COMM_DATA;

__packed typedef union {
	COMM_DATA ComData;
	uint8_t RawData[MONITOR_DATA_LENGTH * 4 + 4];
} COM_DATA_PACK_DEF;

void SendDataToMonitor(void);
void MonitorUpdateDataPos(float f, uint8_t p);

#endif /* __DATAMONITOR_H */
