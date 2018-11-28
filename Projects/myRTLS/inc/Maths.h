#ifndef __MATHS_H
#define __MATHS_H

#include <math.h>
#include <stdint.h>

#define CRC_INIT                       (0x66)

#define LIMIT_MAX(x, y)                (((x) > (y)) ? (y) : (x))
#define LIMIT_MIN(x, y)                (((x) < (y)) ? (y) : (x))
#define LIMIT_RANGE(x, max, min)       (LIMIT_MAX(LIMIT_MIN((x), (min)), (max)))

__packed typedef union {
  uint8_t cData[4];
  float fData;
} FloatUnion;

float apply_deadband(float value, float deadband);
void step_change(float *in, float target, float step, float deadBand);
uint8_t Get_CRC8_Check_Sum(uint8_t *pchMessage, uint32_t dwLength, uint8_t ucCRC8);

#endif /* __MATHS_H */
