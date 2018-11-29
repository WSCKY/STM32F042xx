#ifndef __DATATYPES_H
#define __DATATYPES_H

#include "SysConfig.h"
#include "RTLS_Conf.h"

/* exact-width signed integer types */
#ifndef int8_t
typedef   signed          char int8_t;
#endif
#ifndef int16_t
typedef   signed short     int int16_t;
#endif
#ifndef int32_t
typedef   signed           int int32_t;
#endif

/* exact-width unsigned integer types */
#ifndef uint8_t
typedef unsigned          char uint8_t;
#endif
#ifndef uint16_t
typedef unsigned short     int uint16_t;
#endif
#ifndef uint32_t
typedef unsigned           int uint32_t;
#endif

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

__packed typedef struct {
  uint8_t TagAddr;
  uint8_t AncAddr;
  float Distance;
} DistDataRespDef;

__packed typedef struct {
  uint8_t TagAddr;
  uint8_t DistNum;
  uint8_t FrameRate;
  __packed struct {
    uint8_t AncAddr;
    float Distance;
  } DistGroup[SUPPORT_MAX_ANCHORS];
} DistGroupDataRespDef;

#endif /* __DATATYPES_H */
