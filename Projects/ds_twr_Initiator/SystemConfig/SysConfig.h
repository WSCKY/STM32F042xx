#ifndef __SYSCONFIG_H
#define __SYSCONFIG_H

#include "stm32f0xx.h"

/* print data to monitor /or send to upper */
#define DATA_PRINT_MONITOR             (0)

#define UNUSED_VARIABLE(X)  ((void)(X))
#define UNUSED_PARAMETER(X) UNUSED_VARIABLE(X)
#define UNUSED_RETURN_VALUE(X) UNUSED_VARIABLE(X)

#endif /* __SYSCONFIG_H */
